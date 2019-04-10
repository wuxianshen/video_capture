#include <opencv2/opencv.hpp>
#include "obj_detect.h"
int main()
{
    cv::Mat frame;
    MixDetector detector;
    detector.setup();
    VideoCapture capture(0);
        bool stop=false;
        while(!stop)
        {
            if (!capture.read(frame))
            {
                printf("fail to read.\n");
                return 0;
            }
            detector.loadImg(frame);
            int c=waitKey(30);
                  if((char)c==27)
                      break;
                  else if(c>=0)
                      waitKey(0);
        }
        capture.release();
        return 1;
}
