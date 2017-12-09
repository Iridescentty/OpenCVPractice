#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using namespace std;
using namespace cv;

Mat g_srcImage, g_dstImage, g_midImage;
vector<Vec4i> g_lines;

int g_nthreshold = 100;

static void on_HoughLines(int, void*);

int main()
{
  Mat g_srcImage = imread("../112.jpg");
  imshow("[srcImage]", g_srcImage);
  
  namedWindow("[dstImage]", CV_WINDOW_AUTOSIZE);
  createTrackbar("value", "[dstImage]", &g_nthreshold, 200, on_HoughLines);
  
  Canny(g_srcImage, g_midImage, 50, 200, 3);
  cvtColor(g_midImage, g_dstImage, COLOR_GRAY2BGR);
  
  on_HoughLines(g_nthreshold, 0);
  HoughLinesP(g_midImage, g_lines, 1, CV_PI/180, 80, 50, 10);
  
  imshow("[dstImage]", g_dstImage);
  
  waitKey(0);
  
  return 0;
}

void on_HoughLines(int, void*)
{
  Mat dstImage = g_dstImage.clone();
  Mat midImage = g_midImage.clone();
  
  vector<Vec4i> mylines;
  HoughLinesP(midImage, mylines, 1, CV_PI/180, g_nthreshold, 50, 10);
  
  for(size_t i = 0; i < mylines.size(); ++i)
  {
    Vec4i l = mylines[i];
    line(dstImage, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(23, 180, 55), 1, LINE_AA);
  }
  
  imshow("[dstImage]", dstImage);
}



























