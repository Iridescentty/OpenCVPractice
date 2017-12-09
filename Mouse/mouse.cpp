#include <opencv2/opencv.hpp>
using namespace cv;

#define WINDOW_NAME "[程序窗口]"

void on_MouseHandle(int event, int x, int y, int flags, void* param);
void DrawRectangle(Mat& img, Rect box);
void ShowHelpText();

Rect g_rectangle;
bool g_bDrawingBox = false;//是否进行绘制
RNG g_rng(12345);

int main(int argc, char** argv)
{
    g_rectangle = Rect(-1, -1, 0, 0);
    Mat srcImage(600, 800, CV_8UC3), tempImage;
    srcImage.copyTo(tempImage);
    g_rectangle = Rect(-1, -1, 0, 0);
    srcImage = Scalar::all(0);

    //设置鼠标操作回调函数
    namedWindow(WINDOW_NAME, WINDOW_NORMAL);
    setMouseCallback(WINDOW_NAME, on_MouseHandle, (void*)&srcImage);

    //程序主循环，当进行绘制的标识为真时，进行绘制
    while(1)
    {
        srcImage.copyTo(tempImage);//复制源图到临时变量
        if(g_bDrawingBox)
            DrawRectangle(tempImage, g_rectangle);//当进行绘制的标识符为真，则进行绘制
        imshow(WINDOW_NAME, tempImage);
        if(waitKey(10) == 27)
            break;//按下ESC键，程序退出
    }

    return 0;
}

//鼠标回调函数，根据不同的鼠标事件进行不同的操作
void on_MouseHandle(int event, int x, int y, int flags, void* param)
{
    Mat& image = *(Mat*) param;
    switch(event)
    {
        //鼠标移动消息
        case EVENT_MOUSEMOVE:
        {
            if(g_bDrawingBox)//如果是否进行绘制的标识为真，则记录下长和宽到RECT型变量中
            {
                g_rectangle.width = x - g_rectangle.x;
                g_rectangle.height = y - g_rectangle.y;
            }
        }
        break;

        //左键按下消息
        case EVENT_LBUTTONDOWN:
        {
            g_bDrawingBox = true;
            g_rectangle = Rect(x, y, 0, 0);//记录起始点
        }
        break;

        //左键抬起消息
        case EVENT_LBUTTONUP:
        {
            g_bDrawingBox = false;
            //对宽和高小于0的处理
            if(g_rectangle.width < 0)
            {
                g_rectangle.x += g_rectangle.width;
                g_rectangle.width *= -1;
            }

            if(g_rectangle.height < 0)
            {
                g_rectangle.y += g_rectangle.height;
                g_rectangle.height *= -1;
            }
            //调用函数进行绘制
            DrawRectangle(image, g_rectangle);
        }
        break;
    }
}

//自定义的矩形绘制函数
void DrawRectangle(Mat& img, Rect box)
{
    rectangle(img, box.tl(), box.br(), Scalar(g_rng.uniform(0,255), g_rng.uniform(0,255), g_rng.uniform(0,255)));//随机颜色
}