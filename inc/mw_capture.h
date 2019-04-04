/**************************************************************************

           Copyright(C), 2016-2026, tao.jing All rights reserved

 **************************************************************************
   File   : mw_capture.h
   Author : tao.jing
   Date   : 19-3-28
   Brief  : 
**************************************************************************/
#ifndef JMW_CAPTURE_MW_CAPTURE_H
#define JMW_CAPTURE_MW_CAPTURE_H

#ifdef OPEN_CV_INSTALLED
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#endif

#include "MWFOURCC.h"
#include "LibMWCapture/MWCapture.h"

#include <thread>
#include <mutex>
#include <condition_variable>

#define IMAGE_SIZE_X 1920
#define IMAGE_SIZE_Y 1080

class mw_capture
{
public:
  static mw_capture* instance();

  bool init();

  bool start_capture();
  bool stop_capture();

  bool clear();

  HCHANNEL open_channel();

  cv::Mat get_cur_img();

private:
  mw_capture();

public:
  bool be_capturing;

  HCHANNEL channel_handler;
  HNOTIFY  notify_handler;

  MWCAP_PTR notify_event;
  MWCAP_PTR capture_event;

  HANDLE64  pb_image;

  MWCAP_VIDEO_BUFFER_INFO   videoBufferInfo;
  MWCAP_VIDEO_FRAME_INFO    videoFrameInfo;
  MWCAP_VIDEO_SIGNAL_STATUS videoSignalStatus;

  DWORD dwFourcc;
  DWORD dwMinStride;
  DWORD dwImageSize;

  cv::Mat cur_img;

  std::mutex cur_img_mt;

  std::mutex              capture_img_event_mt;
  std::condition_variable capture_img_event_cv;

private:
  bool initialized;

  LONGLONG total_time;

  std::thread capture_thread;
};

#endif //JMW_CAPTURE_MW_CAPTURE_H
