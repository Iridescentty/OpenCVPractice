#include "cv.h"
#include "highgui.h"

int main(int argc, char** argv)
{
    cvNamedWindow("window", 0.9);
    CvCapture* capture = cvCreateFileCapture("fly.avi");
    IplImage* frame;
    while(1)
    {
        frame = cvQueryFrame(capture);
        if(!frame)
            break;
        cvShowImage("window", frame);
        char c = cvWaitKey(33);
        if(c == 27)
            break;
    }
    cvReleaseCapture(&capture);
    cvDestroyWindow("window");
}