#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <vector>

using namespace std;
using namespace cv;

int main(int argc, char* argv[])
{
	Mat src = imread("test.jpg");
	Mat dst;
	vector<Mat> bgr;

	split(src,bgr);

	merge(bgr, dst);
	imshow("dst", dst);

	Mat tmp(src.size(), CV_8U, Scalar(0));
	vector<Mat> b, g, r;
	for(int i = 0; i < 3; ++i)
	{
		if(i == 0)
			b.push_back(bgr[0]);
		else
			b.push_back(tmp);
		if(i == 1)
			g.push_back(bgr[1]);
		else
			g.push_back(tmp);
		if(i == 2)
			r.push_back(bgr[2]);
		else
			r.push_back(tmp);
	}
	
	merge(b, dst);
	imwrite("b.jpg",dst);
	imshow("b", dst);

	merge(g, dst);
	imwrite("g.jpg",dst);
	imshow("g", dst);

	merge(r, dst);
	imwrite("r.jpg",dst);
	imshow("r", dst);

	waitKey();
	return 0;
}
