#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

String filename = "/home/chenxinghui/opencv-3.1.0/data/haarcascades/haarcascade_eye.xml";
CascadeClassifier eye_detector;

int main(int argc, char** argv)
{
  Mat src = imread("/home/chenxinghui/redeyes.jpg");
  if(src.empty())
  {
    printf("could not load image...\n");
    return -1;
  }
  if(!eye_detector.load(filename))
  {
    printf("could not load data file...\n");
    return -1;
  }
  
  Mat imgOut = src.clone();
  Mat gray;
  vector<Rect> eyes;
  cvtColor(src, gray, CV_BGR2GRAY);
  equalizeHist(gray, gray);
  eye_detector.detectMultiScale(gray, eyes, 1.1, 3, 0, Size(100, 100));
  for(size_t t = 0; t < eyes.size(); ++t)
  {
    //通道分离
    Mat eye = src(eyes[t]);
    vector<Mat> bgr(3);
    split(eye, bgr);
    
    //基于像素模型的红眼区域检测
    Mat mask = (bgr[2] > 150) & (bgr[2] > (bgr[1] + bgr[0]));
    
    //区域填充与提取
    Mat mask_floodfill = mask.clone();
    floodFill(mask_floodfill, cv::Point(0, 0), Scalar(255));
    Mat mask2;
    bitwise_not(mask_floodfill, mask2);
    mask = (mask2 | mask);
    dilate(mask, mask, Mat(), Point(-1, -1), 3, 1, 1);
    if(t == 1)
    {
      imshow("mask", mask);
    }
    
    //修复
    Mat mean = (bgr[0] + bgr[1]) / 2;
    mean.copyTo(bgr[0], mask);
    mean.copyTo(bgr[1], mask);
    mean.copyTo(bgr[2], mask);
    
    //回填
    Mat eyeOut;
    merge(bgr, eyeOut);
    eyeOut.copyTo(imgOut(eyes[t]));
    
    rectangle(src, eyes[t], Scalar(0, 0, 255), 2, 8, 0);
  }
  
  
  imshow("input", src);
  imshow("output", imgOut);
  
  waitKey(0);
  return 0;
}




































