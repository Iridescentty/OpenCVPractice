#include <opencv2/opencv.hpp>
using namespace cv;

int main()
{
    VideoCapture capture(0);
    namedWindow("camera", WINDOW_NORMAL);

    while(1)
    {
        Mat frame;
        capture >> frame;
        imshow("camera", frame);
        waitKey(20);
    }
    return 0;
}