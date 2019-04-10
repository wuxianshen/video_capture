#include "obj_detect.h"
using namespace std;
using namespace cv;



float MixDetector::get_color(int c, int x, int max)
{
    float ratio = ((float)x/max)*5;
    int i = floor(ratio);
    int j = ceil(ratio);
    ratio -= i;
    float r = (1-ratio) * colors[i][c] + ratio*colors[j][c];
    return r;
}
void MixDetector::setup()
{
    net=load_network((char*)cfgfile.c_str(),(char*)weightfile.c_str(),0);//加载网络模型
    set_batch_network(net, 1);
    ifstream classNamesFile("../coco.names");//标签文件coco有80类
    if (classNamesFile.is_open()){
        string className = "";
        while (getline(classNamesFile, className))
            classNamesVec.push_back(className);
    }
}
void MixDetector::loadImg(Mat frame)
{
    if(tracking==false)
    {
    cvtColor(frame, rgbImg, cv::COLOR_BGR2RGB);

    float* srcImg;
    size_t srcSize=rgbImg.rows*rgbImg.cols*3*sizeof(float);
    srcImg=(float*)malloc(srcSize);

    imgConvert(rgbImg,srcImg);//将图像转为yolo形式

    float* resizeImg;
    size_t resizeSize=net->w*net->h*3*sizeof(float);
    resizeImg=(float*)malloc(resizeSize);
    imgResize(srcImg,resizeImg,frame.cols,frame.rows,net->w,net->h);//缩放图像

    network_predict(net,resizeImg);//网络推理
    int nboxes=0;
    detection *dets=get_network_boxes(net,rgbImg.cols,rgbImg.rows,thresh,0.5,0,1,&nboxes);

    if(nms){
        do_nms_sort(dets,nboxes,classes,nms);
    }

    vector<cv::Rect>boxes;
    boxes.clear();
    vector<int>classNames;

    for (int i = 0; i < nboxes; i++){
        bool flag=0;
        int className;
        for(int j=0;j<classes;j++){
            if(dets[i].prob[j]>thresh){
                if(!flag){
                    flag=1;
                    className=j;
                }
            }
        }
        if(flag){
            int left = (dets[i].bbox.x - dets[i].bbox.w / 2.)*frame.cols;
            int right = (dets[i].bbox.x + dets[i].bbox.w / 2.)*frame.cols;
            int top = (dets[i].bbox.y - dets[i].bbox.h / 2.)*frame.rows;
            int bot = (dets[i].bbox.y + dets[i].bbox.h / 2.)*frame.rows;

            if (left < 0)
                left = 0;
            if (right > frame.cols - 1)
                right = frame.cols - 1;
            if (top < 0)
                top = 0;
            if (bot > frame.rows - 1)
                bot = frame.rows - 1;

            Rect box(left, top, fabs(left - right), fabs(top - bot));
            boxes.push_back(box);
            classNames.push_back(className);
        }
    }
    free_detections(dets, nboxes);

    for(int i=0;i<boxes.size();i++){
        int offset = classNames[i]*123457 % 80;
        float red = 255*get_color(2,offset,80);
        float green = 255*get_color(1,offset,80);
        float blue = 255*get_color(0,offset,80);


        rectangle(frame,boxes[i],Scalar(blue,green,red),2);

        String label = String(classNamesVec[classNames[i]]);
        int baseLine = 0;
        Size labelSize = getTextSize(label, FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseLine);
        putText(frame, label, Point(boxes[i].x, boxes[i].y + labelSize.height),
                FONT_HERSHEY_SIMPLEX, 1, Scalar(red, blue, green),2);
    }
    if(boxes.size()>0)
    {
       tracking=true;
       tracking_frame=0;
       bbox=boxes[0];
       tracker=TrackerMIL::create();;
       tracker->init(frame,bbox);
    }
    imshow("output",frame);

    free(srcImg);
    free(resizeImg);
    }
    else if(tracking==true)
    {
        //tracking
          tracker->update(frame,bbox);
          rectangle(frame,bbox, Scalar(255, 0, 0), 2, 1);
          imshow("output", frame);
          tracking_frame++;
          if(tracking_frame>=15)
          {
              tracking=false;
              tracker->~Tracker();
          }
     }
}
MixDetector::~MixDetector()
{
    free_network(net);
    tracker->~Tracker();
}
