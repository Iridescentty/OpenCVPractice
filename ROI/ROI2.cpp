#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
using namespace cv;
using namespace std;

bool ROI_AddImage();
bool LinearBlending();
bool ROI_LinearBlending();

int main()
{
	system("color 5E");

	if( ROI_AddImage ( ) && LinearBlending ( ) && ROI_LinearBlending ( ) )
	{
		cout << endl << "运行成功，得出了你需要的图像！";
	}
	waitKey(0);
	return 0;
}

//ROI_AddImage() , 利用刚兴趣区域ROI实现图像叠加
bool ROI_AddImage ( )
{
	Mat srcImage = imread("dota.jpg");
	Mat logoImage = imread("logo.jpg");
	namedWindow("dota", WINDOW_NORMAL);
	imshow("dota", srcImage);
	namedWindow("logo", WINDOW_NORMAL);
	imshow("logo", logoImage);
	if( !srcImage.data )
	{
		cout << "读取srcImage错误！" << endl;
		return false;
	}
	if( !logoImage.data )
	{
		cout << "读取logoImage错误！" << endl;
		return false;
	}
	Mat imageROI = srcImage( Rect( 200, 250, logoImage.cols , logoImage.rows ) );

	//加载掩膜（必须是灰度图）
	Mat mask = imread("logo.jpg", 0);

	//将掩膜复制到ROI
	logoImage.copyTo( imageROI, mask );
	namedWindow( "ROI", WINDOW_NORMAL );
	imshow("ROI", srcImage );
	return true;
}

//LinearBlending() , 利用cv::addWeighted()函数实现图像线性混合
bool LinearBlending ( )
{
	double alphaValue = 0.5;
	double betaValue;
	Mat srcImage2, srcImage3, dstImage;


	//两幅图片需为同样的类型和尺寸
	srcImage2 = imread("r.jpg");
	srcImage3 = imread("g.jpg");

	if( !srcImage2.data )
	{
		cout << "读取srcImage2错误！" << endl;
		return false;
	}
	if( !srcImage3.data )
	{
		cout << "读取srcImage3错误！" << endl;
		return false;
	}

	//进行图像混合加权操作
	betaValue = ( 1.0 - alphaValue );
	addWeighted( srcImage2, alphaValue, srcImage3, betaValue, 0.0, dstImage );

	namedWindow( "2.线性混合实例（原图）", WINDOW_NORMAL );
	imshow( "2.线性混合实例（原图）", srcImage2 );

	namedWindow( "3.线性混合实例（效果图）", WINDOW_NORMAL );
	imshow( "3.线性混合实例（效果图）", dstImage );

	return true;
}

//ROI_LinearBlending() , 线性混合实现函数， 指定区域线性图像混合。利用cv::addWeighted()函数结合定义
bool ROI_LinearBlending ( )
{
	Mat srcImage4 = imread ( "dota.jpg" );
	Mat logoImage = imread ( "logo.jpg" );

	if( !srcImage4.data )
	{
		cout << "读取srcImage2错误！" << endl;
		return false;
	}
	if( !logoImage.data )
	{
		cout << "读取logoImage错误！" << endl;
		return false;
	}

	Mat imageROI;
	//方法一
	imageROI = srcImage4 ( Rect ( 200, 250, logoImage.cols, logoImage.rows ) );
	//方法二
//	imageROI = srcImage4 ( Range ( 250, 250 + logoImage.rows ), Range ( 200, 200 + logoImage.cols ) );

	addWeighted ( imageROI, 0.5, logoImage, 0.3, 0., imageROI );

	namedWindow( "4.区域线性图像混合", WINDOW_NORMAL );
	imshow( "4.区域线性图像混合" , imageROI );
	return true;
}