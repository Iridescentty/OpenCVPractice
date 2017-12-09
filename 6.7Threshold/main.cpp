#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
using namespace std;
using namespace cv;
#define WINDOW_NAME "【程序窗口】"

int g_nThresholdValue = 100;
int g_nThresholdType = 3;
Mat g_srcImage, g_grayImage, g_dstImage;

static void ShowHelpText();
void on_Threshold(int, void*);

int main()
{
  g_srcImage = imread("../lena.jpg");
  if(!g_srcImage.data)
  {
    printf("读取图片有误!\n");
    return false;
  }
  
  //保存一份原图的灰度图
  
  cvtColor(g_srcImage, g_grayImage, COLOR_RGB2GRAY);
  
  namedWindow(WINDOW_NAME, CV_WINDOW_AUTOSIZE);
  
  //创建滑动条来控制阈值
  
  createTrackbar("mode", WINDOW_NAME, &g_nThresholdType, 4, on_Threshold);
  createTrackbar("parameter value", WINDOW_NAME, &g_nThresholdValue, 255, on_Threshold);
  
  //初始化自定义的阈值回调函数
  
  on_Threshold(0, 0);
  
  while(1)
  {
    int key;
    key = waitKey(20);
    if((char)key == 27)
    {
      break;
    }
  }
  return 0;
}

void on_Threshold(int, void*)
{
  threshold(g_grayImage, g_dstImage, g_nThresholdValue, 255, g_nThresholdType);
  imshow(WINDOW_NAME, g_dstImage);
}





















