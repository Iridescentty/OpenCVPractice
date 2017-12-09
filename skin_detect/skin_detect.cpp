#include <iostream>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <cvaux.h>
#include <highgui.h>
#include <cxcore.h>
#include <math.h>
//#include <windows.h>

struct buck
{
	int p[3][3];
};

struct buck get_point(IplImage* pic,int x,int y)   //每个点的判断
{
	struct buck bk;
	//定义上一行y-1的与x相邻的三个坐标点
	if(y==0)     //第一行
	{
		bk.p[0][0]=0;
		bk.p[0][1]=0;
		bk.p[0][2]=0;
	}
	else
	{
		unsigned char* ptr=(unsigned char*)(pic->imageData+(y-1)*pic->widthStep);  //图像前一行的坐标
		if(x==0) bk.p[0][0]=0;   //最左边的行坐标点 
		else bk.p[0][0]=ptr[x-1];  //定义bk为上一行相邻的三个行坐标点
		bk.p[0][1]=ptr[x];  
		if(x==pic->width) bk.p[0][2]=0;
		else bk.p[0][2]=ptr[x+1];
	}
	//定义本行y的与x相邻的三个坐标点
	unsigned char* ptr=(unsigned char*)(pic->imageData+y*pic->widthStep);   //本行坐标点
	if(x==0) bk.p[1][0]=0;      //本行三个相邻坐标点
	else bk.p[1][0]=ptr[x-1];
	bk.p[1][1]=ptr[x];
	if(x==pic->width) bk.p[1][2]=0;
	else bk.p[1][2]=ptr[x+1];
	//定义下一行y+1的与x相邻的三个坐标点
	if(y==pic->height-1)
	{
		bk.p[2][0]=0;
		bk.p[2][1]=0;
		bk.p[2][2]=0;
	}
	else
	{
		unsigned char* ptr=(unsigned char*)(pic->imageData+(y+1)*pic->widthStep);
		if(x==0) bk.p[2][0]=0;
		else bk.p[2][0]=ptr[x-1];
		bk.p[2][1]=ptr[x];
		if(x==pic->width) bk.p[2][2]=0;
		else bk.p[2][2]=ptr[x+1];
	}
	return bk;
}

unsigned char get_spoint(IplImage* pic,int x,int y)
{
	unsigned char* ptr=(unsigned char*)(pic->imageData+y*pic->widthStep);
	return ptr[x];   //本行的坐标点返回
}

void cvSkinSegment(IplImage* img, IplImage* mask)//原始图像及目标图像
{
	CvSize imageSize = cvSize(img -> width, img -> height);
	IplImage *imgY = cvCreateImage(imageSize, IPL_DEPTH_8U, 1);
	IplImage *imgCb = cvCreateImage(imageSize, IPL_DEPTH_8U, 1);
	IplImage *imgCr = cvCreateImage(imageSize, IPL_DEPTH_8U, 1);

	IplImage *imgYCrCb = cvCreateImage(imageSize, img -> depth, img -> nChannels);
	cvCvtColor(img, imgYCrCb, CV_BGR2YCrCb);
	cvSplit(imgYCrCb, imgY, imgCr, imgCb, 0);//得到每一通道的数据
	int y, cr, cb, l, x1, y1, value;
	unsigned char *pY, *pCr, *pCb, *pMask;
	
	pY = (unsigned char *)img -> imageData;//Y通道的第一行数据的指针
	pCr = (unsigned char *)imgCr -> imageData;
	pCb = (unsigned char *)imgCb -> imageData;
	pMask = (unsigned char *)mask -> imageData;//目标图像的第一行数据的指针
	cvSetZero(mask);//目标图像全部为零
	l = img -> height * img -> width;//图像的元素个数
	for(int i = 0; i < l; ++i)
	{
		y = *pY;
		cr = *pCr;
		cb = *pCb;
		cb -= 109;
		cr -= 152;
		x1 = (819*cr - 614*cb) / 32 + 51;
		y1 = (819*cr + 614*cb) / 32 + 77;
		x1 = x1*41 / 1024;
		y1 = y1*73 / 1024;
		value = x1*x1 + y1*y1;//构造椭圆的模型
		if(y < 100)
			(*pMask) = (value < 700) ? 255 : 0;//对齐进行值得选择，要不255，要不0  //在不同的亮度下呈现出不同的亮度
		else
			(*pMask) = (value < 850) ? 255 : 0;//255 is the skin, 0 is the background   //255 纯白色 0 纯黑色
		pY++;
		pCr++;
		pCb++;
		pMask++;
	}

	cvReleaseImage(&imgY);
	cvReleaseImage(&imgCr);
	cvReleaseImage(&imgCb);
	cvReleaseImage(&imgYCrCb);
}

int main(int argc, char** argv)
{
	CvCapture* capture;
	CvCapture* capture1;
	IplImage* frame, pic, picx;
	IplImage* frame1;

	cvNamedWindow("out", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("prc", CV_WINDOW_AUTOSIZE);
	capture = cvCreateCameraCapture(0);
	int hand_pos[3] = {0, 0, 0};
	while(1)
	{
		frame = cvQueryFrame(capture);
		if(!frame)
			break;
		cvSmooth(frame,frame, CV_GAUSSIAN, 3, 3);
		IplImage* gray = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 1);
		IplImage* pic = cvCreateImage(cvSize(gray -> width, gray -> height), gray -> depth, 1);
		IplImage* picx = cvCreateImage(cvSize(gray -> width, gray -> height), gray -> depth, 1);
		IplImage* out = cvCreateImage(cvSize(gray -> width, gray -> height), gray -> depth, 1);

		for(int y = 0; y < out -> height; ++y)
		{
			uchar* ptr = (uchar*)(out -> imageData + y*out -> widthStep);//每一行的数据
			for(int x = 0; x < out -> width; ++x)//指向每一个数据
			{
				ptr[x] = 0;//输出点初始化为零
			}
		}
		cvCvtColor(frame, gray, CV_RGB2GRAY);//转化为灰度
		cvSkinSegment(frame, pic);//进行皮肤检测
		for(int y = 0; y < out -> height; ++y)
		{
			uchar* ptr1 = (uchar*)(frame -> imageData + y*frame -> widthStep);//图像的每行数据
			uchar* ptr2 = (uchar*)(pic -> imageData + y*pic -> widthStep);//另一个指向检测处皮肤的
			for(int x = 0; x < out -> width; ++x)
			{
				if(ptr1[x*3] > 160 && ptr1[x*3+1] > 160 && ptr1[x*3+2] > 160 && abs(ptr1[x*3] - ptr1[x*3+1]) <= 30 && abs(ptr1[x*3] - ptr1[x*3+2]) <= 30 && abs(ptr1[x*3+1] - ptr1[x*3+2]) <= 30)
					ptr2[x] = 0;
			}
		}
		cvDilate(pic, pic, NULL, 5);//形态学的膨胀
		cvErode(pic, pic, NULL, 5);//腐蚀  //去除噪声点
		int c_n = 0, c_now, c_xx, num_c[256];
		for(int i = 0; i < 256; ++i)
			num_c[i] = 0;
		//此部分找到每一个相连通的部分， 并对每一个连通的部分进行赋值，使其被赋予不同的值！
		for(int y = 0; y < pic -> height; ++y)//图像的高度
		{
			uchar* ptr = (uchar*)(pic -> imageData + y*pic -> widthStep);//已检测皮肤的图像
			uchar* ptr1 = (uchar*)(out -> imageData + y*out -> widthStep);//将要输出的图像
			for(int x = 0; x < pic -> width; ++x)
			{
				if(ptr[x] == 255)//此图像只有0和255   //皮肤检测的误差很大pic的点 //如果是皮肤
				{
					struct buck bk = get_point(out, x, y);//得到小范围的块3 * 3
					if((bk.p[0][0] + bk.p[0][1] + bk.p[0][2] + bk.p[1][0] + bk.p[1][1] + bk.p[1][2] + bk.p[2][0] + bk.p[2][1] + bk.p[2][2]) == 0)//小范围都是皮肤点
					{
						ptr1[x] = (++c_n);//那个像素点是第几个连通的皮肤点！ 对输出的out图像进行赋值  //c_n记录这是第几个连通域
					}
					else//新图的此处周围已经有皮肤点了
					{
						c_now = 0;
						for(int j = 0; j < 3; ++j)
						{
							for(int i = 0; i < 3; ++i)
							{
								if(bk.p[j][i] > 0 && c_now == 0)
									c_now = bk.p[j][i];
								if(bk.p[j][i] > 0 && bk.p[j][i] != c_now && c_now > 0)
								{
									c_xx = bk.p[j][i];
									for(int yy = 0; yy < out -> height; ++yy)
									{
										uchar* ptr2 = (uchar*)(out -> imageData + yy*out -> widthStep);
										for(int xx = 0; xx < out -> width; ++xx)//此时的像素点
										{
											if(ptr2[xx] == c_xx)
											{
												ptr2[xx] = c_now;
											}
										}
									}
								}
							}
						}
						ptr1[x] = c_now;//并且此点也为那个前一个连通的区域
					}
				}
			}
		}
		for(int y = 0; y < out -> height; ++y)
		{
			uchar* ptr = (uchar*)(out -> imageData + y*out -> widthStep);
			for(int x = 0; x < out -> width; ++x)
			{
				if(ptr[x] == 0)
					continue;
				num_c[ptr[x]]++;
			}
		}
		c_n = num_c[0];
		c_now = 0;
		for(int i = 1; i < 256; ++i)
		{
			if(num_c[i] > c_n)
			{
				c_now = i;
				c_n = num_c[i];
			}
		}
		int hand_all = 0, hand_x = 0, hand_y = 0;
		for(int y = 0; y < out -> height; ++y)
		{
			uchar* ptr = (uchar*)(out -> imageData + y*out -> widthStep);
			for(int x = 0; x < out -> width; ++x)
			{
				if(ptr[x] == c_now)
				{
					ptr[x] = 255;
					hand_all++;
					hand_x += x;
					hand_y += y;
				}
				else
				{
					ptr[x] = 0;
				}
			}
		}
		if(hand_all > out -> width*out -> height / 7)
		{
			hand_pos[0] = hand_pos[1];
			hand_pos[1] = hand_pos[2];
			hand_pos[2] = hand_y / hand_all;

			if(hand_pos[0] * hand_pos[1] * hand_pos[2] > 0)
			{
				if(hand_pos[0] < hand_pos[1] && hand_pos[1] < hand_pos[2] && (hand_pos[2] - hand_pos[0]) > 100)
				{
					printf("1\n");
					//keybd_event(34, 0, 0, 0);
					hand_pos[0] = 0;
					hand_pos[1] = 0;
					hand_pos[2] = 0;
				}
				if(hand_pos[0] > hand_pos[1] && hand_pos[1] > hand_pos[2] && (hand_pos[0] - hand_pos[2]) > 100)
				{
					printf("2\n");
					//keybd_event(33, 0, 0, 0);
					hand_pos[0] = 0;
					hand_pos[1] = 0;
					hand_pos[2] = 0;
				}
			}
		}
		cvShowImage("out", out);
		cvShowImage("prc", pic);
		char c = cvWaitKey(20);
		if(c == 27)
			break;
	}
	cvReleaseCapture(&capture);
	cvDestroyWindow("1");
	cvDestroyWindow("11");
	cvDestroyWindow("2");
	return 0;
}









