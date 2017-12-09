#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
using namespace cv;
using namespace std;

int main( )
{
	Mat srcImage = imread ( "../picture/lena.jpg", 0);
	if ( !srcImage.data )
	{
		cout << "读取图片错误，请确定目录下是否有指定图片存在！" ;
		return false;
	}
	namedWindow ( "srcImage", WINDOW_NORMAL );
	imshow ( "srcImage", srcImage );

	//将输入图像延扩到最佳尺寸，边界用0补充
	int m = getOptimalDFTSize ( srcImage.rows );
	int n = getOptimalDFTSize ( srcImage.cols );

	//将添加的像素初始化为0
	Mat padded;
	copyMakeBorder ( srcImage, padded, 0, m - srcImage.rows, 0, n - srcImage.cols, BORDER_CONSTANT, Scalar::all ( 0 ) );

	//为傅里叶变换的结果（实部和虚部）分配存储空间
	//将planes数组组合合并成一个多通道的数组complexI
	Mat planes [] = { Mat_<float> ( padded ), Mat::zeros ( padded.size ( ), CV_32F ) };
	Mat complexI;
	merge ( planes, 2, complexI );

	//进行就地离散傅里叶变换
	dft ( complexI, complexI );

	//将复数转换为幅值
	split (complexI, planes );//将多通道数组complexI分离成几个单通道数组
	//下标为0的为实数，下标为1的为虚数
	magnitude ( planes [0], planes [1], planes [0] ); 
	Mat magnitudeImage = planes [0];

	//进行对数尺度缩放
	magnitudeImage += Scalar::all (1);
	log ( magnitudeImage, magnitudeImage );

	//剪切和重分布幅度图像象限，若有奇数行或奇数列，进行频谱裁剪
	magnitudeImage = magnitudeImage ( Rect ( 0, 0, magnitudeImage.cols & -2, magnitudeImage.rows & -2 ) );

	//重新排列傅里叶图像中的象限，使得原点位于图像中心
	int cx = magnitudeImage.cols / 2;
	int cy = magnitudeImage.rows / 2;
	Mat q0 ( magnitudeImage, Rect ( 0, 0, cx, cy ) );//ROI区域的左上
	Mat q1 ( magnitudeImage, Rect ( cx, 0, cx, cy ) );//ROI区域的右上
	Mat q2 ( magnitudeImage, Rect ( 0, cy, cx, cy ) );//ROI区域的左下
	Mat q3 ( magnitudeImage, Rect ( cx, cy, cx, cy ) );//ROI区域的右下

	//交换象限（左上与右下进行交换）
	Mat tmp;
	q0.copyTo ( tmp );
	q3.copyTo ( q0 );
	tmp.copyTo ( q3 );

	//交换象限（右上与左下进行交换）
	q1.copyTo ( tmp );
	q2.copyTo ( q1 );
	tmp.copyTo ( q2 );

	//归一化，用0到1之间的浮点值得将矩阵变换为可视的图像格式
	normalize ( magnitudeImage, magnitudeImage, 0, 1, NORM_MINMAX );

	namedWindow ( "频谱幅值", WINDOW_NORMAL );
	imshow ( "频谱幅值", magnitudeImage );
	waitKey ( );
	return 0;
}