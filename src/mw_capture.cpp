/**************************************************************************

           Copyright(C), 2016-2026, tao.jing All rights reserved

 **************************************************************************
   File   : mw_capture.cpp
   Author : tao.jing
   Date   : 19-3-28
   Brief  :
**************************************************************************/
#include "mw_capture.h"
#include <string.h>
#include <sys/eventfd.h>
#include <sys/select.h>
#include <unistd.h>
#include <iostream>

void capture_loop();

mw_capture::mw_capture()
: channel_handler(nullptr) //_capture->channel_handler
, notify_handler(0) //hNotify
, notify_event(0)   //hNotifyEvent
, capture_event(0)  //hCaptureEvent
, pb_image(0)       //pbImage
, total_time(0LL)   //llTotalTime
, be_capturing(false)
, initialized(false)
{

}

mw_capture* mw_capture::instance()
{
  static mw_capture inst;
  return &inst;
}

bool mw_capture::init()
{
  if ( initialized )
  {
    return false;
  }

  if(!MWCaptureInitInstance())
  {
    std::cout << "MWCapture init finished." << std::endl;
  }

  MWRefreshDevice();

  channel_handler = open_channel();
  if(nullptr == channel_handler)
  {
    return false;
  }

  MWCAP_CHANNEL_INFO videoInfo = { 0 };
  if (MW_SUCCEEDED != MWGetChannelInfo(channel_handler, &videoInfo))
  {
    std::cout << "ERROR: Can't get channel info!" << std::endl;
    return false;
  }

  capture_event = MWCreateEvent();
  if(capture_event == 0)
  {
    std::cout << "Create timer event error" << std::endl;
    return false;
  }

  notify_event = MWCreateEvent();
  if(notify_event==0)
  {
    std::cout << "Create notify event error;" << std::endl;
    return false;
  }

  int cx=1920;
  int cy=1080;

  dwFourcc    = MWFOURCC_BGR24;
  dwMinStride = FOURCC_CalcMinStride(dwFourcc,cx,4);
  dwImageSize = FOURCC_CalcImageSize(dwFourcc,cx,cy,dwMinStride);
  pb_image = (HANDLE64)(unsigned long)malloc(dwImageSize);

  initialized = true;
  return true;
}

bool mw_capture::clear()
{
  if ( !initialized )
  {
    return false;
  }

  stop_capture();

  MWUnregisterNotify(channel_handler, notify_handler);
  MWStopVideoCapture(channel_handler);

  if (channel_handler != nullptr)
  {
    MWCloseChannel(channel_handler);
    channel_handler = nullptr;
  }

  if(notify_event != 0)
  {
    MWCloseEvent(notify_event);
    notify_event = 0;
  }

  if(capture_event != 0)
  {
    MWCloseEvent(capture_event);
    capture_event = 0;
  }

  if(pb_image != 0)
  {
    free((void *)(unsigned long)pb_image);
    pb_image = 0;
  }

  MWCaptureExitInstance();

  std::cout << "MW Capture Closed." << std::endl;
  initialized = false;
  return true;
}

bool mw_capture::start_capture()
{
  MW_RESULT xr;
  xr = MWStartVideoCapture(channel_handler, capture_event);
  if (xr != MW_SUCCEEDED)
  {
    std::cout << "ERROR: Start Video Capture error!" << std::endl;
    return false;
  }

  MWGetVideoBufferInfo(channel_handler, &videoBufferInfo);
  MWGetVideoFrameInfo(channel_handler, videoBufferInfo.iNewestBufferedFullFrame, &videoFrameInfo);
  MWGetVideoSignalStatus(channel_handler, &videoSignalStatus);

  switch (videoSignalStatus.state)
  {
    case MWCAP_VIDEO_SIGNAL_NONE:
      std::cout << "ERRPR: Input signal status: NONE" << std::endl;
      break;
    case MWCAP_VIDEO_SIGNAL_UNSUPPORTED:
      std::cout << "ERRPR: Input signal status: Unsupported" << std::endl;
      break;
    case MWCAP_VIDEO_SIGNAL_LOCKING:
      std::cout << "ERRPR: Input signal status: Locking" << std::endl;
      break;
    case MWCAP_VIDEO_SIGNAL_LOCKED:
      std::cout << "Input signal status: Locked" << std::endl;
      break;
  }

  if (videoSignalStatus.state != MWCAP_VIDEO_SIGNAL_LOCKED)
  {
    MWStopVideoCapture(channel_handler);
    return false;
  }

  std::cout << "Input signal resolution: " << videoSignalStatus.cx << " * "
            << videoSignalStatus.cy << std::endl;
  double fps = (videoSignalStatus.bInterlaced == TRUE) ?
                (double)20000000LL / videoSignalStatus.dwFrameDuration :
                (double)10000000LL / videoSignalStatus.dwFrameDuration;
  std::cout << "Input signal fps: " << fps << std::endl;

  notify_handler = MWRegisterNotify(channel_handler, notify_event, MWCAP_NOTIFY_VIDEO_FRAME_BUFFERED);
  if (notify_handler == 0)
  {
      MWStopVideoCapture(channel_handler);
      std::cout << "ERROR: Register Notify error..." << std::endl;
      return false;
  }
  std::cout << "Begin to capture with fps: " << fps << std::endl;
  fflush(stdout);

  be_capturing = true;
  capture_thread = std::thread(capture_loop);
  return true;
}

bool mw_capture::stop_capture()
{
  be_capturing = false;
  std::thread::id null_thread_id;
  if ( null_thread_id != capture_thread.get_id() )
  {
    if ( capture_thread.joinable() )
    {
      capture_thread.join();
    }
  }
  return true;
}

HCHANNEL mw_capture::open_channel()
{
  HCHANNEL _channel = nullptr;
  int channel_count = MWGetChannelCount();

  if (0 == channel_count)
  {
    std::cout << "ERROR: Can't find channels!" << std::endl;
    return nullptr;
  }
  std::cout << "Find " << channel_count << " channels" << std::endl;

  int nProDevCount = 0;
  int nProDevChannel[32] = {-1};
  for (int i = 0; i < channel_count; i++)
  {
    MWCAP_CHANNEL_INFO info;
    MW_RESULT mr = MWGetChannelInfoByIndex(i, &info);
    if ((strcmp(info.szFamilyName, "Pro Capture") == 0) || (strcmp(info.szFamilyName, "Eco Capture") == 0))
    {
        std::cout << "Find " << info.szFamilyName << " ..." << std::endl;
        nProDevChannel[nProDevCount] = i;
        nProDevCount++;
    }
  }

  if (nProDevCount <= 0)
  {
    std::cout << "ERROR: Can't find pro or eco channels! " << std::endl;
    return nullptr;
  }

  std::cout << "Find " << nProDevCount << "pro or eco channels." << std::endl;

  // Get <board id > <channel id> or <channel index>
  MWCAP_CHANNEL_INFO videoInfo = { 0 };
  if (MW_SUCCEEDED != MWGetChannelInfoByIndex(nProDevChannel[0], &videoInfo))
  {
    std::cout << "ERROR: Can't get channel info!" << std::endl;
    return nullptr;
  }

  // Open channel
  char path[128] = {0};
  MWGetDevicePath(nProDevChannel[0], path);
  _channel = MWOpenChannelByPath(path);
  if (_channel == nullptr)
  {
    std::cout << "ERROR: Open channel " << 0 << std::endl;
    return nullptr;
  }
  return _channel;
}

cv::Mat mw_capture::get_cur_img()
{
  {
    std::lock_guard<std::mutex> auto_locak(cur_img_mt);
    return cur_img.clone();
  }
}

void capture_loop()
{
  std::cout << "Capture loop start..." << std::endl;
  mw_capture* _capture = mw_capture::instance();
  while ( _capture->be_capturing )
  {
    MW_RESULT xr;
    int nRet = MWWaitEvent(_capture->notify_event, 1000);
    if( nRet <= 0 )
    {
      //ToDo : fix some timeout bug
      std::cout << "Error:wait notify error or timeout..." << std::endl;
      continue;
    }

    ULONGLONG ullStatusBits = 0;
    xr = MWGetNotifyStatus(_capture->channel_handler, _capture->notify_handler, &ullStatusBits);
    if (xr != MW_SUCCEEDED)
    {
      std::cout << "Error: MWGetNotifyStatus..." << std::endl;
      continue;
    }

    xr = MWGetVideoBufferInfo(_capture->channel_handler, &_capture->videoBufferInfo);
    if (xr != MW_SUCCEEDED)
    {
      continue;
    }

    xr = MWGetVideoFrameInfo(_capture->channel_handler, _capture->videoBufferInfo.iNewestBufferedFullFrame,
                             &_capture->videoFrameInfo);
    if (xr != MW_SUCCEEDED)
    {
       continue;
    }

    if (ullStatusBits & MWCAP_NOTIFY_VIDEO_FRAME_BUFFERED == 0)
    {
      continue;
    }
    xr = MWCaptureVideoFrameToVirtualAddressEx(_capture->channel_handler,
                                               _capture->videoBufferInfo.iNewestBufferedFullFrame,
                                               (unsigned char *)(_capture->pb_image),
                                               _capture->dwImageSize,
                                               _capture->dwMinStride,
                                               1,0,
                                               _capture->dwFourcc,
                                               IMAGE_SIZE_X, IMAGE_SIZE_Y,
                                               0,0,0,0,0,100,0,100,0,
                                               MWCAP_VIDEO_DEINTERLACE_BLEND,
                                               MWCAP_VIDEO_ASPECT_RATIO_CROPPING,
                                               0,0,0,0,
                                               MWCAP_VIDEO_COLOR_FORMAT_UNKNOWN,
                                               MWCAP_VIDEO_QUANTIZATION_UNKNOWN,
                                               MWCAP_VIDEO_SATURATION_UNKNOWN);
    if (xr != MW_SUCCEEDED)
    {
       continue;
    }

    nRet = MWWaitEvent(_capture->capture_event, 1000);
    if(nRet<=0)
    {
      //ToDo : fix some failures bug
      std::cout << "Error:wait capture event error or timeout" << std::endl;
      continue;
    }

    cv::Mat image(cv::Size(IMAGE_SIZE_X, IMAGE_SIZE_Y), CV_8UC3, reinterpret_cast<void*>(_capture->pb_image));

    {
      //Store current image
      std::lock_guard<std::mutex> auto_locak(_capture->cur_img_mt);
      cv::flip(image, _capture->cur_img, 0);
    }

    {
      std::lock_guard<std::mutex> auto_lock(_capture->capture_img_event_mt);
    }
    _capture->capture_img_event_cv.notify_all();
  }
}