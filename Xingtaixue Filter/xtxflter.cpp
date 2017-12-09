#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using namespace std;
using namespace cv;

Mat g_srcImage, g_dstImage;
int g_nElementShape = MORPH_RECT;//元素结构的形状

//变量接收的TrackBar位置参数
int g_nMaxIterationNum = 10;
int g_nOpenCloseNum = 0;
int g_nErodeDilateNum = 0;
int g_nTopBlackHatNum = 0;

static void on_OpenClose(int, void*);//回调函数
static void on_ErodeDilate(int, void*);//回调函数
static void on_TopBlackHat(int, void*);//回调函数
static void ShowHelpText();//帮助文字显示

int main()
{
	string s, t;
	s = "/home/chenxinghui/桌面/Image Processing/Xingtaixue Filter/";
	cout << "请输入图片名称（无需加后缀，默认为jpg格式）：" << endl;
	cin >> t;
	s += t + ".jpg";
	g_srcImage = imread( s );
	if( !g_srcImage.data )
	{
		cout << "读取图片错误！";
		return false;
	}

	namedWindow( "src" , WINDOW_NORMAL );
	imshow( "src", g_srcImage );

	namedWindow( "开运算/闭运算" , WINDOW_NORMAL );
	namedWindow( "腐蚀/膨胀", WINDOW_NORMAL );
	namedWindow( "顶帽/黑帽", WINDOW_NORMAL );

	g_nOpenCloseNum = 9;
	g_nErodeDilateNum = 9;
	g_nTopBlackHatNum = 2;

	createTrackbar( "迭代值:", "开运算/闭运算", &g_nOpenCloseNum, g_nMaxIterationNum * 2 + 1, on_OpenClose );
	createTrackbar( "迭代值", "腐蚀/膨胀", &g_nErodeDilateNum, g_nMaxIterationNum * 2 + 1, on_ErodeDilate );
	createTrackbar( "迭代值", "顶帽/黑帽", &g_nTopBlackHatNum, g_nMaxIterationNum * 2 + 1, on_TopBlackHat );

	//轮询获取按键信息
	while(1)
	{
		int c;

		on_OpenClose( g_nOpenCloseNum, 0 );
		on_ErodeDilate( g_nErodeDilateNum, 0 );
		on_TopBlackHat( g_nTopBlackHatNum, 0 );

		//获取按键
		c = waitKey(0);

		//按下键盘按键Q或者ESC，程序退出
		if( (char)c == 'q' || (char)c == 27 )
			break;
		
		//按下键盘按键1,使用椭圆（Elliptic）结构元素MORPH_ELLIPSE
		if( (char)c == 49 )//按键1的ASCII码为49
			g_nElementShape = MORPH_ELLIPSE;
		
		//按下键盘按键2,使用矩形（Rectangle）结构元素MORPH_RECT
		else if( (char)c == 50 )//按键2的ASCII码为50
			g_nElementShape = MORPH_RECT;
		
		//按下键盘按键3,使用十字形（Cross-shaped）结构元素MORPH_CROSS
		else if( (char)c == 51 )//按键3的ASCII码为51
			g_nElementShape = MORPH_CROSS;
		
		//按下键盘按键space,在矩形、椭圆、十字形结构元素中循环
		else if( (char)c == ' ' )
			g_nElementShape = ( g_nElementShape + 1) % 3;
	}
	return 0;
}

//开运算/闭运算窗口的回调函数
static void on_OpenClose(int, void*)
{
	//偏移量的定义
	int offset = g_nOpenCloseNum - g_nMaxIterationNum;//偏移量
	int Absolute_offset = offset > 0 ? offset : -offset;//偏移量绝对值

	//自定义核
	Mat element = getStructuringElement(g_nElementShape, Size(Absolute_offset*2+1, Absolute_offset*2+1), Point(Absolute_offset, Absolute_offset) );

	//进行操作
	if( offset < 0 )
		morphologyEx( g_srcImage, g_dstImage, MORPH_OPEN, element );
	else
		morphologyEx( g_srcImage, g_dstImage, MORPH_CLOSE, element );
	imshow( "开运算/闭运算", g_dstImage );
}

//腐蚀/膨胀窗口的回调函数
static void on_ErodeDilate(int, void*)
{
	//偏移量的定义
	int offset = g_nErodeDilateNum - g_nMaxIterationNum;//偏移量
	int Absolute_offset = offset > 0 ? offset : -offset;//偏移量绝对值

	//自定义核
	Mat element = getStructuringElement(g_nElementShape, Size(Absolute_offset*2+1, Absolute_offset*2+1), Point(Absolute_offset, Absolute_offset) );

	//进行操作
	if( offset < 0 )
		erode( g_srcImage, g_dstImage, element );
	else
		dilate( g_srcImage, g_dstImage, element );

	imshow( "腐蚀/膨胀", g_dstImage );
}

//顶帽/黑帽窗口的回调函数
static void on_TopBlackHat(int, void*)
{
	//偏移量的定义
	int offset = g_nTopBlackHatNum - g_nMaxIterationNum;//偏移量
	int Absolute_offset = offset > 0 ? offset : -offset;//偏移量绝对值

	//自定义核
	Mat element = getStructuringElement(g_nElementShape, Size(Absolute_offset*2+1, Absolute_offset*2+1), Point(Absolute_offset, Absolute_offset) );

	//进行操作
	if( offset < 0 )
		morphologyEx( g_srcImage, g_dstImage, MORPH_TOPHAT, element );
	else
		morphologyEx( g_srcImage, g_dstImage, MORPH_BLACKHAT, element );
	imshow( "顶帽/黑帽", g_dstImage );
}