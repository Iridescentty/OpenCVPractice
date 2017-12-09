#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
// OpenCV 特征检测模块
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/xfeatures2d/nonfree.hpp>
//#include <opencv2/nonfree/nonfree.hpp>
#include <opencv2/calib3d/calib3d.hpp>

#include <iostream>
#include <fstream>
using namespace std;
using namespace cv;
using namespace xfeatures2d;

static void on_ContrastAndBright ( int, void *);
static void ShowHelpText ();

int g_nContrastValue;//对比度值
int g_nBrightValue;//亮度值
Mat g_srcImage, g_dstImage;
ofstream of("/home/chenxinghui/桌面/实验截图/Light/orb.txt");
//ofstream of("/home/chenxinghui/桌面/实验截图/Light/sift.txt");

int main ( )
{
	// string s;
	// cout << "请输入图片名称（无需加后缀，默认为png格式）：" << endl;
	// cin >> s;
	// s += ".png";
	g_srcImage = imread ( "2.png" );

	if( !g_srcImage.data )
	{
		cout << "读取图片错误，请确定目录下是否有该图片！" << endl;
		return false;
	}
	g_dstImage = Mat :: zeros ( g_srcImage.size ( ), g_srcImage.type ( ) );

	//设定对比度和亮度的初值
	g_nContrastValue = 20;
	g_nBrightValue = 80;

	namedWindow ( "dst", WINDOW_NORMAL );

	//创建轨迹条
	createTrackbar ( "对比度 : ", "dstGirl", &g_nContrastValue, 300, on_ContrastAndBright );
	createTrackbar ( "亮 度：", "dstGirl", &g_nBrightValue, 200, on_ContrastAndBright );

	//进行回调函数初始化
	on_ContrastAndBright ( g_nContrastValue, 0 );
	on_ContrastAndBright ( g_nBrightValue, 0 );

	//输出一些帮助信息
	cout << endl << "\t请调整滚动条观察图像效果\n\n" << "\t按下“q”键时，程序退出！\n";
	//按下“q”键时，程序退出
	while( char ( waitKey ( 1 ) ) != 'q') {}
//	imwrite( "/home/chenxinghui/桌面/实验截图/Light/Light.png", g_dstImage ) ;
	return 0;
}

//改变图像对比度和亮度值的回调函数
static void on_ContrastAndBright ( int, void *)
{
	namedWindow ( "src", WINDOW_NORMAL );

	//三个for循环，执行运算 g_dstImage ( i, j ) = a * g_srcImage ( i, j ) + b;
	for( int y = 0; y < g_srcImage.rows; ++y )
	{
		for( int x = 0; x < g_srcImage.cols; ++x )
		{
			for( int c = 0; c < 3; ++c )
			{
				g_dstImage.at<Vec3b>( y, x )[c] = saturate_cast<uchar> ( ( g_nContrastValue * 0.01) * ( g_srcImage.at<Vec3b>( y, x )[c]) + g_nBrightValue );
			}
		}
	}

	cout << "对比度 = " << g_nContrastValue << "\t" << "亮度值 = " << g_nBrightValue << endl;
	Ptr<ORB> orb = ORB::create();
	//Ptr<SIFT> sift = SIFT::create();
	vector< cv::KeyPoint > kp1, kp2;
	orb->detect(g_srcImage, kp1);
	orb->detect(g_dstImage, kp2);
	//sift->detect(g_srcImage, kp1);
	//sift->detect(g_dstImage, kp2);
	cout << "Key points of dstimage: " << kp2.size() << endl;
	cv::Mat imgShow;
	cv::drawKeypoints(g_dstImage, kp2, imgShow, cv::Scalar::all(-1), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

	cv::Mat desp1, desp2;
	orb->compute(g_srcImage, kp1, desp1);
    	orb->compute(g_dstImage, kp2, desp2);
	//sift->compute(g_srcImage, kp1, desp1);
    	//sift->compute(g_dstImage, kp2, desp2);

	imshow ( "src", g_srcImage );
	imshow ( "dst", g_dstImage );
	imshow ( "imgShow", imgShow );

	vector< cv::DMatch > matches;
	BFMatcher bf(NORM_HAMMING); 
	bf.match(desp1, desp2, matches);
	//cv::FlannBasedMatcher matcher;
	//matcher.match(desp1, desp2, matches);
    	cout << "Find total " << matches.size() << " matches." << endl;

    	cv::Mat imgMatches;
    	cv::drawMatches(g_srcImage, kp1, g_dstImage, kp2, matches, imgMatches);

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
	cv::drawMatches(g_srcImage, kp1, g_dstImage, kp2, goodMatches, imgMatches);

	double result;
	result = (double)goodMatches.size() / (double)kp2.size();
	of  << g_nContrastValue << "\t" << kp2.size() << "\t" << goodMatches.size() << "\t"  << result * 100 << "%" << endl;
}
//代码中的saturate_cast模板函数，其用于溢出保护，大致原理：
// if ( data < 0 )
// 	data = 0;
// else if ( data > 255 )
// 	data = 255;