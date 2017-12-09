#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <string>
using namespace cv;
using namespace std;

Mat g_srcImage, g_dstImage;
int g_nTrackbarNumber = 0;//0表示腐蚀erode，1表示膨胀dilate
int g_nStructElementSize = 3;//结构元素（内核矩阵）的尺寸

void Process ( );//腐蚀和膨胀的处理函数
void on_TrackbarNumChange ( int, void * );
void on_ElementSizeChange ( int, void *);

int main ( )
{
	string s;
	cout << "请输入图片名称（无需加后缀，默认为jpg格式）：" << endl;
	cin >> s;
	s += ".jpg";
	g_srcImage = imread ( s );
	if ( !g_srcImage.data )
	{
		cout << "读取图片失败" << endl;
		return false;
	}

	namedWindow ( "原图", WINDOW_NORMAL );
	imshow ( "原图", g_srcImage );

	//进行初次腐蚀操作并显示效果图
	namedWindow ( "效果图", WINDOW_NORMAL );
	//获取自定义核
	Mat element = getStructuringElement ( MORPH_RECT, Size ( 2*g_nStructElementSize+1, 2*g_nStructElementSize+1 ), Point ( g_nStructElementSize, g_nStructElementSize ) );
	erode ( g_srcImage, g_dstImage, element );
	imshow ( "效果图", g_dstImage );

	//创建轨迹条
	createTrackbar ( "腐蚀/膨胀", "效果图", &g_nTrackbarNumber, 1, on_TrackbarNumChange );
	createTrackbar ( "内核尺寸", "效果图", &g_nStructElementSize, 21, on_ElementSizeChange );

	//输出一些帮助信息
	cout << "\t0表示腐蚀erode，1表示膨胀dilate\n\t按下q键退出" << endl;
	//轮询获取按键信息，若是q键，程序退出
	while ( char (waitKey ( 1 ) ) != 'q' ) { }
	return 0;
}

//进行自定义的腐蚀和膨胀操作
void Process ( )
{
	Mat element = getStructuringElement ( MORPH_RECT, Size ( 2*g_nStructElementSize+1, 2*g_nStructElementSize+1 ), Point ( g_nStructElementSize, g_nStructElementSize ) );

	//进行腐蚀或者膨胀操作
	if ( g_nTrackbarNumber == 0 )
	{
		erode ( g_srcImage, g_dstImage, element );
	}
	else
	{
		dilate ( g_srcImage, g_dstImage, element );
	}
	imshow ( "效果图", g_dstImage );
}

void on_TrackbarNumChange ( int, void * )
{
	//腐蚀和膨胀之间效果已经切换，回调函数体内需要调用一次Process函数，
	//使改变后的效果立即生效并显示出来
	Process ( );
}

void on_ElementSizeChange ( int, void * )
{
	//内核尺寸已改变，回调函数体内需要调用一次Process函数，
	//使改变后的效果立即生效并显示出来
	Process ( );
}