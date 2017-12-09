#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
// OpenCV 特征检测模块
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/xfeatures2d/nonfree.hpp>
//#include <opencv2/nonfree/nonfree.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <iostream>
#include <fstream>
#include <string>
using namespace std;
using namespace cv;
using namespace xfeatures2d;

Mat g_srcImage, g_dstImage1, g_dstImage2, g_dstImage3, g_dstImage4, g_dstImage5;
int g_nBoxFilterValue = 6;//方框滤波参数值
int g_nMeanBlurValue = 10;//均值滤波参数值
int g_nGaussianBlurValue = 6;//高斯滤波参数值
int g_nMedianBlurValue = 10;//中值滤波参数值
int g_nBilateralFilterValue = 10;//双边滤波参数值

//轨迹条的回调函数
static void on_BoxFilter ( int, void * );//方框滤波
static void on_MeanBlur ( int, void * );//均值滤波
static void on_GaussianBlur ( int, void * );//高斯滤波
static void on_MedianBlur ( int, void * );//中值滤波
static void on_BilateralFilter ( int, void * );//双边滤波

//ofstream of("/home/chenxinghui/桌面/实验截图/Gauss/orb.txt");
ofstream of("/home/chenxinghui/桌面/实验截图/Gauss/sift.txt");

int main ( )
{
	//改变console字体颜色
//	system ( "color5E" );

	// string s;
	// cout << "请输入图片名称（无需加后缀，默认为png格式）：" << endl;
	// cin >> s;
	// s += ".png";
	g_srcImage = imread ( "2.png" );
	if( !g_srcImage.data )
	{
		cout << "读取图片错误！";
		return false;
	}

	g_dstImage1 = g_srcImage.clone ( );
	g_dstImage2 = g_srcImage.clone ( );
	g_dstImage3 = g_srcImage.clone ( );
	g_dstImage4 = g_srcImage.clone ( );
	g_dstImage5 = g_srcImage.clone ( );

	namedWindow ( "原图窗口", WINDOW_NORMAL );
	imshow ( "原图窗口", g_srcImage );

	//===========方框滤波============//
	// namedWindow ( "方框滤波", WINDOW_NORMAL );
	// //创建轨迹条
	// createTrackbar ( "内核值：", "方框滤波", &g_nBoxFilterValue, 40, on_BoxFilter );
	// on_BoxFilter ( g_nBoxFilterValue, 0 );
	// imshow ( "方框滤波", g_dstImage1 );

	//============均值滤波============//
	// namedWindow ( "均值滤波", WINDOW_NORMAL );
	// //创建轨迹条
	// createTrackbar ( "内核值：", "均值滤波", &g_nMeanBlurValue, 40, on_MeanBlur );
	// on_MeanBlur ( g_nMeanBlurValue, 0 );
	// imshow ( "均值滤波", g_dstImage2 );

	//============高斯滤波============//
	namedWindow ( "高斯滤波", WINDOW_NORMAL );
	//创建轨迹条
	createTrackbar ( "内核值：", "高斯滤波", &g_nGaussianBlurValue, 40, on_GaussianBlur );
	on_GaussianBlur ( g_nGaussianBlurValue, 0 );
	imshow ( "高斯滤波" , g_dstImage3 );

	//============中值滤波============//
	// namedWindow ( "中值滤波", WINDOW_NORMAL );
	// //创建轨迹条
	// createTrackbar ( "内核值：", "中值滤波", &g_nMedianBlurValue, 40, on_MedianBlur );
	// on_MedianBlur ( g_nMedianBlurValue, 0 );
	// imshow ( "中值滤波", g_dstImage4 );

	//============双边滤波============//
	// namedWindow ( "双边滤波", WINDOW_NORMAL );
	// //创建轨迹条
	// createTrackbar ( "内核值：", "双边滤波",  &g_nBilateralFilterValue, 40, on_BilateralFilter );
	// on_BilateralFilter ( g_nBilateralFilterValue, 0 );
	// imshow ( "双边滤波", g_dstImage5 );

	//============================================
	//输出一些帮助信息
	cout << endl << "\t请调整滚动条观察图像效果\n\n" << "\t按下“q”键时，程序退出！\n";
	//按下“q”键时，程序退出
	while ( char (waitKey ( 1 ) ) != 'q' ) { }
//	imwrite( "/home/chenxinghui/桌面/实验截图/Gauss/Gauss.png", g_dstImage3 ) ;
	return 0;
}

//方框滤波操作的回调函数
static void on_BoxFilter ( int, void * )
{
	//方框滤波操作
	boxFilter ( g_srcImage, g_dstImage1, -1, Size ( g_nBoxFilterValue+1, g_nBoxFilterValue+1) );
	imshow ( "方框滤波", g_dstImage1 );
}

//均值滤波操作的回调函数
static void on_MeanBlur ( int, void * )
{
	//均值滤波操作
	blur ( g_srcImage, g_dstImage2, Size ( g_nMeanBlurValue+1, g_nMeanBlurValue+1), Point ( -1, -1 ) );
	imshow ( "均值滤波", g_dstImage2 );
}

//高斯滤波操作的回调函数
static void on_GaussianBlur ( int, void * )
{
	//高斯滤波操作
	GaussianBlur ( g_srcImage, g_dstImage3, Size ( g_nGaussianBlurValue*2+1, g_nGaussianBlurValue*2+1), 0, 0 );
	cout << "高斯滤波参数值 = " << g_nGaussianBlurValue << endl;
	//Ptr<ORB> orb = ORB::create();
	Ptr<SIFT> sift = SIFT::create();
	vector< cv::KeyPoint > kp1, kp2;
	//orb->detect(g_srcImage, kp1);
	//orb->detect(g_dstImage3, kp2);
	sift->detect(g_srcImage, kp1);
	sift->detect(g_dstImage3, kp2);
	cout << "Key points of dstimage: " << kp2.size() << endl;
	cv::Mat imgShow;
	cv::drawKeypoints(g_dstImage3, kp2, imgShow, cv::Scalar::all(-1), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

	cv::Mat desp1, desp2;
	//orb->compute(g_srcImage, kp1, desp1);
    	//orb->compute(g_dstImage3, kp2, desp2);
	sift->compute(g_srcImage, kp1, desp1);
    	sift->compute(g_dstImage3, kp2, desp2);

	imshow ( "src", g_srcImage );
	imshow ( "dst", g_dstImage3 );
	imshow ( "imgShow", imgShow );

	vector< cv::DMatch > matches;
	//BFMatcher bf(NORM_HAMMING); 
	//bf.match(desp1, desp2, matches);
	cv::FlannBasedMatcher matcher;
	matcher.match(desp1, desp2, matches);
    	cout << "Find total " << matches.size() << " matches." << endl;

    	cv::Mat imgMatches;
    	cv::drawMatches(g_srcImage, kp1, g_dstImage3, kp2, matches, imgMatches);

    	vector< cv::DMatch > goodMatches;
	double minDis = 9999;
	for(size_t i = 0; i < matches.size(); ++i)
	{
	    if(matches[i].distance < minDis)
	        minDis = matches[i].distance;
	}

	for(size_t i = 0; i < matches.size(); ++i)
	{
	    if(matches[i].distance < 4*minDis)
	        goodMatches.push_back(matches[i]);
	}

	// 显示 good matches
	cout << "good matches = " << goodMatches.size() << endl;
	cv::drawMatches(g_srcImage, kp1, g_dstImage3, kp2, goodMatches, imgMatches);

	double result;
	result = (double)goodMatches.size() / (double)kp2.size();
	of  << g_nGaussianBlurValue << "\t" << kp2.size() << "\t" << goodMatches.size() << "\t"  << result * 100 << "%" << endl;
	imshow ( "高斯滤波", g_dstImage3 );
}

//中值滤波操作的回调函数
static void on_MedianBlur ( int, void * )
{
	//中值滤波操作
	medianBlur ( g_srcImage, g_dstImage4, g_nMedianBlurValue*2+1 );
	imshow ( "中值滤波", g_dstImage4 );
}

//双边滤波操作的回调函数
static void on_BilateralFilter ( int, void * )
{
	//双边滤波操作
	bilateralFilter ( g_srcImage, g_dstImage5, g_nBilateralFilterValue, g_nBilateralFilterValue*2, g_nBilateralFilterValue/2 );
	imshow ( "双边滤波", g_dstImage5 );
}