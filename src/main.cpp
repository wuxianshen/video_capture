/**************************************************************************

           Copyright(C), 2016-2026, tao.jing All rights reserved

 **************************************************************************
   File   : main.cpp
   Author : tao.jing
   Date   : 19-3-28
   Brief  :
**************************************************************************/

#include "mw_capture.h"
#include "image_handler.h"
#include <iostream>
#include <sstream>

#ifdef OPEN_CV_INSTALLED
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#endif

int mw_capture(int argc, char** argv)
{
  bool ret = mw_capture::instance()->init();
  if ( ret )
  {
    std::cout << "MW Capture init successfully..." << std::endl;
  }

  ret = mw_capture::instance()->start_capture();
  if ( ret )
  {
    std::cout << "Start Capture successfully..." << std::endl;
  }

  image_handler::instance()->start_image_process();

  char inputChar;
  std::cin >> inputChar;

  image_handler::instance()->stop_image_process();
  mw_capture::instance()->clear();

  return 0;
}

int main(int argc, char** argv)
{
  cv::VideoCapture cap(0);
  cap.set(CV_CAP_PROP_FRAME_WIDTH, 1920);
  cap.set(CV_CAP_PROP_FRAME_HEIGHT, 1080);
  if( !cap.isOpened() )
  {
    std::cout << "Cannot open usb device!" << std::endl;
    return 0;
  }
  cv::Mat frame;
  unsigned int cnt = 0;
  while( cnt < 20000 )
  {
    cap >> frame;
    if ( frame.empty() )
    {
      std::cout << "Empty image..." << std::endl;
      break;
    }

    if ( cnt % 200 == 0 )
    {
      std::stringstream sstr;
      sstr << "img_" << cnt << ".jpg";
      cv::imwrite(sstr.str(), frame);
    }

    cv::imshow("USBCapture", frame);
    cv::waitKey(1);

    cnt++;
  }

  cap.release();

  //image_handler::instance()->start_image_process();

  char inputChar;
  std::cin >> inputChar;

  //image_handler::instance()->stop_image_process();

  return 0;
}