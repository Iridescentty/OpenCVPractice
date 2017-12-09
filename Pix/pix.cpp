#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <string>
using namespace std;
using namespace cv;

void colorReduce_zhizhen(Mat& inputImage, Mat& outputImage, int div)
{
	outputImage = inputImage.clone();//复制实参到临时变量
	int rowNumber = outputImage.rows;
	int colNumber = outputImage.cols * outputImage.channels();//列数×通道数=每一行元素的个数

	for(int i = 0; i < rowNumber; ++i)
	{
		uchar* data = outputImage.ptr<uchar>(i);//获取第i行的首地址
		for(int j = 0; j < colNumber; ++j)
		{
			data[j] = data[j]/div * div + div/2;
		}
	}
}


void colorReduce_iterator(Mat& inputImage, Mat& outputImage, int div)
{
	outputImage = inputImage.clone();

	//获取迭代器
	Mat_<Vec3b>::iterator it = outputImage.begin<Vec3b>();//初始位置的迭代器
	Mat_<Vec3b>::iterator itend = outputImage.end<Vec3b>();//终止位置的迭代器

	for(; it != itend; ++it)
	{
		(*it)[0] = (*it)[0]/div * div + div/2;
		(*it)[1] = (*it)[1]/div * div + div/2;
		(*it)[2] = (*it)[2]/div * div + div/2;
	}
}


void colorReduce_dynamic(Mat& inputImage, Mat& outputImage, int div)
{
	outputImage = inputImage.clone();
	int rowNumber = outputImage.rows;
	int colNumber = outputImage.cols;

	for(int i = 0; i < rowNumber; ++i)
	{
		for(int j = 0; j < colNumber; ++j)
		{
			outputImage.at<Vec3b>(i, j)[0] = outputImage.at<Vec3b>(i, j)[0]/div*div + div/2;//蓝色通道
			outputImage.at<Vec3b>(i, j)[1] = outputImage.at<Vec3b>(i, j)[1]/div*div + div/2;//绿色通道
			outputImage.at<Vec3b>(i, j)[2] = outputImage.at<Vec3b>(i, j)[2]/div*div + div/2;//红色通道
		}
	}
}


int main()
{
	Mat srcImage = imread("../picture/lena.jpg");
	namedWindow("srcImage", WINDOW_NORMAL);
	imshow("srcImage", srcImage);

	//按原始图的参数规格来创建效果图
	Mat dstImage;
	dstImage.create(srcImage.rows, srcImage.cols, srcImage.type());//效果图的大小、类型与原图片相同

	double time0 = static_cast<double>(getTickCount());

	int number;
	char c;
//	string s;
	//调用颜色空间缩减函数

	while(1)
	{
		cout << "请输入编号：" << endl << "0表示退出，1表示用指针操作，2表示用迭代器操作，3表示用动态地址操作" << endl;
		cin >> number;
		if(number == 0)
		{
			exit(0);
		}
		else if(number == 1)
		{
			colorReduce_zhizhen(srcImage, dstImage, 32);
			break;
		}
		else if(number == 2)
		{
			colorReduce_iterator(srcImage, dstImage, 32);
			break;
		}
		else if(number == 3)
		{
			colorReduce_dynamic(srcImage, dstImage, 32);
			break;
		}
		else
		{
			cout << "输入有误，请重新输入!!!" << endl;
			continue;
		}	

// 		time0 = ((double)getTickCount() - time0) / getTickFrequency();
// 		cout << "此方法的运行时间为：" << time0 << "秒" << endl;
// 		cout << "111" << endl;
// 		namedWindow("dstImage", WINDOW_NORMAL);
// 		imshow("dstImage", dstImage);
//		cout << "222" << endl;
/*		while(1)
		{
			cout << "是否继续：y/Y继续，n/N结束" << endl;
//			getchar(c);
//			cin >> s;
			if(c == 'y' || 'Y')
			{
				cout << "333" << endl;
				break;
			}
			else if(c == 'n' || 'N')
			{
				cout << "444" << endl;
				exit(0);
				cout << "555" << endl;
			}
			else
			{
				cout << "输入有误，请重新输入!!!" << endl;
				continue;
			}
		}
*/		
	}
	time0 = ((double)getTickCount() - time0) / getTickFrequency();
	cout << "此方法的运行时间为：" << time0 << "秒" << endl;
//	cout << "111" << endl;
	namedWindow("dstImage", WINDOW_NORMAL);
	imshow("dstImage", dstImage);
	
	waitKey(0);
	return 0;
}
