/**************************************************************************

           Copyright(C), 2016-2026, tao.jing All rights reserved

 **************************************************************************
   File   : image_handler.h
   Author : tao.jing
   Date   : 19-3-29
   Brief  : 
**************************************************************************/

#ifndef JMW_CAPTURE_IMAGE_HANDLER_H
#define JMW_CAPTURE_IMAGE_HANDLER_H

#ifdef OPEN_CV_INSTALLED
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#endif

#include <thread>
#include <mutex>

class image_handler
{
public:
  static image_handler* instance();

  bool start_image_process();
  bool stop_image_process();

  bool img_process();

  bool img_update(unsigned int time_out = 20);

private:
  image_handler();

public:
  bool be_processing;

  cv::Mat cur_img;

private:
  std::thread img_process_thread;

  std::mutex cur_img_mt;
};

#endif //JMW_CAPTURE_IMAGE_HANDLER_H
