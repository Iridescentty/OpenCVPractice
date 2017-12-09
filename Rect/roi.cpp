#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
using namespace cv;
using namespace std;

void LoadImage(vector<string> &strImageLeft, vector<string> &strImageRight)
{
  for(int i = 0; i < 3727; ++i)
  {
    stringstream ss;
    ss << setfill('0') << setw(6) << i;
    string str = "/home/chenxinghui/save_ZED_pictures/ROI/";
    string left = str + "image_left/" + ss.str() + ".png";
    string right = str + "image_right/" + ss.str() + ".png";
    strImageLeft.push_back(left);
    strImageRight.push_back(right);
  }
}

int main()
{
  vector<string> src_image_left, src_image_right;
  LoadImage(src_image_left, src_image_right);
  const int count_image_left = src_image_left.size();
  Mat image_left, image_right;
  vector<Mat> image_left_roi, image_right_roi;
  for(int i = 0; i < count_image_left; ++i)
  {
    image_left = imread(src_image_left[i]);
    image_right = imread(src_image_right[i]);
    Rect rect_left(0, 135, image_left.cols, image_left.rows*0.6);
    Rect rect_right(0, 135, image_right.cols, image_right.rows*0.6);
    Mat temp_left(image_left, rect_left);
    Mat temp_right(image_right, rect_right);
    image_left_roi.push_back(temp_left);
    image_right_roi.push_back(temp_right);
  }
  
  for(int i = 0; i < count_image_left; ++i)
  {
    stringstream sst;
    sst << setfill('0') << setw(6) << i;
    imwrite("/home/chenxinghui/save_ZED_pictures/ROI/image_left_roi/" + sst.str() + ".png", image_left_roi[i]);
    imwrite("/home/chenxinghui/save_ZED_pictures/ROI/image_right_roi/" + sst.str() + ".png", image_right_roi[i]);
  }
/*
  Mat src_image;
  namedWindow("src", CV_WINDOW_AUTOSIZE);
  imshow("src", src_image);
  Rect rect(0, 135, src_image.cols, src_image.rows*0.6);
  Mat temp(src_image, rect);
  namedWindow("roi", CV_WINDOW_AUTOSIZE);
  imshow("roi", temp);
  waitKey(0);
*/ 
}
