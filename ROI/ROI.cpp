#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
using namespace std;
using namespace cv;

int main()
{
	Mat srcImage = imread("dota.jpg");
	Mat logoImage = imread("logo.jpg");
	namedWindow("dota", WINDOW_NORMAL);
	imshow("dota", srcImage);
	namedWindow("logo", WINDOW_NORMAL);
	imshow("logo", logoImage);
	// if( !srcImage )
	// {
	// 	cout << "读取srcImage错误！" << endl;
	// 	return;
	// }
	// if( !logoImage )
	// {
	// 	cout << "读取logoImage错误！" << endl;
	// 	return;
	// }
	Mat imageROI = srcImage( Rect( 10, 30, logoImage.cols , logoImage.rows ) );

	//加载掩膜（必须是灰度图）
	Mat mask = imread("logo.jpg", 0);

	//将掩膜复制到ROI
	logoImage.copyTo( imageROI, mask );
	namedWindow( "ROI", WINDOW_NORMAL );
	imshow("ROI", srcImage );
	waitKey();
	return 0;
}