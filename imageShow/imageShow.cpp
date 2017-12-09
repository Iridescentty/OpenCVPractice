#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
using namespace cv;

int main()
{
    Mat dota = imread("../dota.jpg");
    Mat logo = imread("../logo.jpg");

    namedWindow("dota",WINDOW_NORMAL);
    imshow("dota",dota);

    namedWindow("logo",WINDOW_NORMAL);
    imshow("logo",logo);

    Mat imageROI;
    // imageROI = dota(Rect(800,350,logo.cols,logo.rows));
    imageROI = dota(Range(350,350+logo.rows),Range(800,800+logo.cols));

    addWeighted(imageROI,0.5,logo,0.3,0.,imageROI);

    namedWindow("dota+logo",WINDOW_NORMAL);
    imshow("dota+logo",dota);

    imwrite("1.jpg",dota);
    waitKey();
    return 0;
}