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
#include <cstdlib>
using namespace std;
using namespace cv;
using namespace xfeatures2d;

// ofstream f("/home/chenxinghui/桌面/实验截图/Size/orb.txt", ios::app);
ofstream f("/home/chenxinghui/桌面/实验截图/Size/sift.txt", ios::app);

int main()
{
	Mat src, dst;
	// string s;
	// cout << "请输入图片名称（无需加后缀，默认为png格式）：" << endl;
	// cin >> s;
	// s += ".png";
	src = imread( "2.png" );

	namedWindow("src",WINDOW_AUTOSIZE);
	namedWindow("dst",WINDOW_AUTOSIZE);

	double scale;
	cout << "Input scale: " << endl;
	cin >> scale;

	// dst_size.width = (int)( src.cols * scale );
	// dst_size.height = (int)( src.rows * scale );
	// dst.create( dst_size.width,  dst_size.height, CV_8UC3 );
	Size dst_size;
	dst_size = Size( src.cols * scale, src.rows * scale );
	dst = Mat( dst_size, CV_32S );
	resize(src, dst, dst_size );	
	
	cout << "scale = " << scale << endl;
	//Ptr<ORB> orb = ORB::create();
	Ptr<SIFT> sift = SIFT::create();
	vector< cv::KeyPoint > kp1, kp2;
	//orb->detect(src, kp1);
	//orb->detect(dst, kp2);
	sift->detect(src, kp1);
	sift->detect(dst, kp2);
	cout << "Key points of dstimage: " << kp2.size() << endl;
	cv::Mat imgShow;
	cv::drawKeypoints(dst, kp2, imgShow, cv::Scalar::all(-1), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

	cv::Mat desp1, desp2;
	//orb->compute(src, kp1, desp1);
	//orb->compute(dst, kp2, desp2);
	sift->compute(src, kp1, desp1);
	sift->compute(dst, kp2, desp2);

	imshow("src", src);
	imshow("dst", imgShow);

	vector< cv::DMatch > matches;
	//BFMatcher bf(NORM_HAMMING); 
	//bf.match(desp1, desp2, matches);
	cv::FlannBasedMatcher matcher;
	matcher.match(desp1, desp2, matches);
	cout << "Find total " << matches.size() << " matches." << endl;

	cv::Mat imgMatches;
	cv::drawMatches(src, kp1, dst, kp2, matches, imgMatches);

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
	cv::drawMatches(src, kp1, dst, kp2, goodMatches, imgMatches);

	double result;
	result = (double)goodMatches.size() / (double)kp2.size();
	f << scale << "\t" << kp2.size() << "\t" << goodMatches.size() << "\t" << result * 100 << "%" << endl;

	while(char(waitKey(1)) != 'q'){}
	// string t = "/home/chenxinghui/桌面/实验截图/Size/";
	// char buffer[20];
	// gcvt( scale, 15, buffer );
	// string ts;
	// ts = buffer;
	// t += ts + ".png";
	// imwrite(t, dst);
	f.close();
	return 0;
}
	