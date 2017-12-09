#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
using namespace cv;

#define WINDOW_NAME "[线性混合实例]"

const int g_nMaxAlphaValue = 100;
int g_nAlphaValueSlider;
double g_dAlphaValue;
double g_dBetaValue;

Mat g_srcImage1;
Mat g_srcImage2;
Mat g_dstImage;

//响应滑动条的回调函数
void on_Trackbar(int, void*)
{
    g_dAlphaValue = (double) g_nAlphaValueSlider / g_nMaxAlphaValue;
    g_dBetaValue = (1.0 - g_dAlphaValue);

    addWeighted(g_srcImage1, g_dAlphaValue, g_srcImage2,g_dBetaValue, 0.0, g_dstImage);

    imshow(WINDOW_NAME, g_dstImage);
}

int main(int argc, char** argv)
{
    g_srcImage1 = imread("../picture/b.jpg");
    g_srcImage2 = imread("../picture/g.jpg");
    if(!g_srcImage1.data)
    {
        printf("读取第一幅图片错误，请确定目录下是否有imread函数指定图片存在～！ \n");
        return -1;
    }
    if(!g_srcImage2.data)
    {
        printf("读取第二幅图片错误，请确定目录下是否有imread函数指定图片存在～！ \n");
        return -1;
    }

    //设置滑动条初始值为70
    g_nAlphaValueSlider = 70;

    namedWindow(WINDOW_NAME, WINDOW_NORMAL);

    char TrackbarName[50];
    sprintf(TrackbarName, "透明值 %d", g_nMaxAlphaValue);

    createTrackbar(TrackbarName, WINDOW_NAME, &g_nAlphaValueSlider, g_nMaxAlphaValue, on_Trackbar);

    //结果在回调函数中显示
    on_Trackbar(g_nAlphaValueSlider, 0);

    waitKey(0);
    return 0;
}