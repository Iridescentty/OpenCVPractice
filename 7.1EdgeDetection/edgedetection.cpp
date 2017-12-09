#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using namespace cv;

Mat g_srcImage, g_srcGrayImage, g_dstImage;

//Canny边缘检测相关变量

Mat g_cannyDetectedEdges;
int g_cannyLowThreshold = 1;

//Sobel边缘检测相关变量

Mat g_sobelGradient_X, g_sobelGradient_Y;
Mat g_sobelAbsGradient_X , g_sobelAbsGradient_Y;
int g_sobelKernelSize = 1;

Mat g_scharrGradient_X, g_scharrGradient_Y;
Mat g_scharrAbsGradient_X, g_scharrAbsGradient_Y;

static void on_Canny(int, void*);
static void on_Sobel(int, void*);
//封装了Scharr边缘检测相关代码的函数

void Scharr();

int main()
{
  g_srcImage = imread("../test.jpg");
  if(!g_srcImage.data)
  {
    printf("read picture wrong!\n");
    return false;
  }
  
  namedWindow("[srcImage]");
  imshow("[srcImage]", g_srcImage);
  
  g_dstImage.create(g_srcImage.size(), g_srcImage.type());
  
  cvtColor(g_srcImage, g_srcGrayImage, COLOR_BGR2GRAY);
  
  namedWindow("[dstImage]Canny edge detection", CV_WINDOW_AUTOSIZE);
  namedWindow("[dstImage]Sobel edge detection", CV_WINDOW_AUTOSIZE);
  
  createTrackbar("parameter value: ", "[dstImage]Canny edge detection", &g_cannyLowThreshold, 120, on_Canny);
  createTrackbar("parameter value: ", "[dstImage]Sobel edge detection", &g_sobelKernelSize, 3, on_Sobel);
  
  on_Canny(0, 0);
  on_Sobel(0, 0);
  
  Scharr();
  
  while((char(waitKey(1)) != 'q')) {}
  
  return 0;
}

//on_Canny()
void on_Canny(int, void*)
{
  //先用3x3内核降噪
  blur(g_srcGrayImage, g_cannyDetectedEdges, Size(3, 3));
  
  Canny(g_cannyDetectedEdges, g_cannyDetectedEdges, g_cannyLowThreshold, g_cannyLowThreshold*3, 3);
  
  //先将g_dstImage内的所有元素设置为0
  g_dstImage = Scalar::all(0);
  
  //使用Canny算子输出的边缘图g_cannyDetectedEdges作为掩码，来将原图g_srcImage拷到目标图g_dstImage中
  g_srcImage.copyTo(g_dstImage, g_cannyDetectedEdges);
  
  imshow("[dstImage]Canny edge detection", g_dstImage);
}

void on_Sobel(int, void*)
{
  //求X方向梯度
  Sobel(g_srcImage, g_sobelGradient_X, CV_16S, 1, 0, (2*g_sobelKernelSize+1), 1, 1, BORDER_DEFAULT);
  convertScaleAbs(g_sobelGradient_X, g_sobelAbsGradient_X);//计算绝对值，并将结果转换成8位
  
  //求Y方向梯度
  Sobel(g_srcImage, g_sobelGradient_Y, CV_16S, 0, 1, (2*g_sobelKernelSize+1), 1, 1, BORDER_DEFAULT);
  convertScaleAbs(g_sobelGradient_Y, g_sobelAbsGradient_Y);
  
  //合并梯度
  addWeighted(g_sobelAbsGradient_X, 0.5, g_sobelAbsGradient_Y, 0.5, 0, g_dstImage);
  
  imshow("[dstImage]Sobel edge detection", g_dstImage);
  
}

void Scharr()
{
  //X
  Scharr(g_srcImage, g_scharrGradient_X, CV_16S, 1, 0, 1, 0, BORDER_DEFAULT);
  convertScaleAbs(g_scharrGradient_X, g_scharrAbsGradient_X);
  
  //Y
  Scharr(g_srcImage, g_scharrGradient_Y, CV_16S, 0, 1, 1, 0, BORDER_DEFAULT);
  convertScaleAbs(g_scharrGradient_Y, g_scharrAbsGradient_Y);
  
  //合并梯度
  addWeighted(g_scharrAbsGradient_X, 0.5, g_scharrAbsGradient_Y, 0.5, 0, g_dstImage);
  
  imshow("[dstImage]Scharr filter", g_dstImage);
}




















