#ifndef JJ_OBJ_DETECT
#define JJ_OBJ_DETECT

#include "darknet.h"
#include "improcess.h"
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/tracking/tracker.hpp>

using namespace std;
using namespace cv;

class MixDetector
{
public:
    void setOption();
    void setup();
    void loadImg(Mat frame);
    ~MixDetector();
private:
    string cfgfile = "../yolov3.cfg";
    string weightfile = "../yolov3.weights";


    float thresh=0.5;//参数设置
    float nms=0.35;
    int classes=80;




    network *net;
    Mat rgbImg;
    vector<string> classNamesVec;


    Ptr<TrackerMIL> tracker;
    Rect2d bbox;
    bool tracking=false;
    int tracking_frame=0;

    float colors[6][3]= { {1,0,1}, {0,0,1},{0,1,1},{0,1,0},{1,1,0},{1,0,0} };
    float get_color(int c, int x, int max);
};
#endif // JJ_OBJ_DETECT

