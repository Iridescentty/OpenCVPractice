#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using namespace cv;

int main()
{
    Mat srcImage = imread("../lena.jpg");

    imshow("srcImage", srcImage);

    Mat dstImage;
    blur(srcImage, dstImage, Size(7,7));

    imshow("dstImage", dstImage);
    waitKey(0);
    return 0;
}