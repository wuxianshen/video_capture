/**************************************************************************

           Copyright(C), 2016-2026, tao.jing All rights reserved

 **************************************************************************
   File   : image_handler.cpp
   Author : tao.jing
   Date   : 19-3-29
   Brief  : 
**************************************************************************/
#include "image_handler.h"
#include "mw_capture.h"
#include <condition_variable>
#include <chrono>

void image_process_loop();

image_handler* image_handler::instance()
{
  static image_handler inst;
  return &inst;
}

image_handler::image_handler()
: be_processing(false)
{

}

bool image_handler::start_image_process()
{
  if ( be_processing )
  {
    return false;
  }
  be_processing = true;
  img_process_thread = std::thread(image_process_loop);
  return true;
}

bool image_handler::stop_image_process()
{
  if ( !be_processing )
  {
    return false;
  }
  be_processing = false;
  std::thread::id null_thread_id;
  if ( null_thread_id != img_process_thread.get_id() )
  {
    if ( img_process_thread.joinable() )
    {
      img_process_thread.join();
    }
  }
}

bool image_handler::img_process()
{
  if ( cur_img.size().width == 0 || cur_img.size().height == 0 )
  {
    return false;
  }

  cv::namedWindow("CurrentImage");
  std::lock_guard<std::mutex> auto_locK(cur_img_mt);
  cv::imshow("CurrentImage", cur_img);
  cv::waitKey(1);
  return true;
}

bool image_handler::img_update(unsigned int time_out)
{
  //Get current img from mw_capture
  std::cv_status ret;
  {
    std::chrono::milliseconds msec(time_out);
    std::unique_lock<std::mutex> auto_lock(mw_capture::instance()->capture_img_event_mt);
    ret = mw_capture::instance()->capture_img_event_cv.wait_for(auto_lock, msec);
  }
  if (ret == std::cv_status::timeout)
  {
    // wait timeout
    return false;
  }

  {
    //Update current image
    std::lock_guard<std::mutex> auto_locK(cur_img_mt);
    cur_img = mw_capture::instance()->get_cur_img();
  }

  return true;
}

void image_process_loop()
{
  image_handler* _handler = image_handler::instance();
  while( _handler->be_processing )
  {
    if ( _handler->img_update(20) )
    {
      _handler->img_process();
    }
    else
    {
      //wait new capture event timeout
      continue;
    }
  }
}