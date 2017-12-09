#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using namespace std;
using namespace cv;
#define WINDOW_NAME "【程序窗口】"

//全局变量
Mat g_srcImage, g_dstImage, g_tmpImage;

int main()
{
  g_srcImage = imread("../lena.jpg");
  if(!g_srcImage.data)
  {
    printf("读取srcImage错误!\n");
    return false;
  }
  
  namedWindow(WINDOW_NAME, CV_WINDOW_AUTOSIZE);
  imshow(WINDOW_NAME, g_srcImage);
  
  g_tmpImage = g_srcImage;
  g_dstImage = g_tmpImage;
  
  int key = 0;
  
  //轮询获取按键信息
  
  while(1)
  {
    key = waitKey(9);//读取键值到key变量中
    
    switch(key)
    {
      case 27:
	return 0;
	break;
	
      case 'q':
	return 0;
	break;
	
      //图片放大
	
      case 'a'://[pyrUp]
	pyrUp(g_tmpImage, g_dstImage, Size(g_tmpImage.cols*2, g_tmpImage.rows*2));
	printf("检测到A被按下，开始基于pyrUp函数的图片放大：图片尺寸x2\n");
	break;
	
      case 'w'://[resize]
	resize(g_tmpImage, g_dstImage, Size(g_tmpImage.cols*2, g_tmpImage.rows*2));
	printf("检测到W被按下，开始基于resize函数的图片放大：图片尺寸x2\n");
	break;
	
      //图片缩小
	
      case 'd':
	pyrDown(g_tmpImage, g_dstImage, Size(g_tmpImage.cols/2, g_tmpImage.rows/2));
	printf("检测到D被按下，开始基于pyrDown函数的图片缩小：图片尺寸/2\n");
	break;
	
      case 's':
	resize(g_tmpImage, g_dstImage, Size(g_tmpImage.cols/2, g_tmpImage.rows/2));
	printf("检测到S被按下，开始基于resize函数的图片缩小：图片尺寸/2\n");
	break;
    }
    
    imshow(WINDOW_NAME, g_dstImage);
    
    //将g_dstImage赋给g_tmpImage，方便下一次循环
    
    g_tmpImage = g_dstImage;
  }
  return 0;
}





















