#include <iostream>
#include "cv.h"
#include "highgui.h"
using namespace std;
int main(int argc, char** argv)
{
    CvCapture* capture = 0;
    capture = cvCreateFileCapture("fly.avi");
    if(capture)
    {
        return -1;
    }
    IplImage *bgr_frame = cvQueryFrame(capture);
    double fps = cvGetCaptureProperty(capture, CV_CAP_PROP_FPS);
    CvSize size = cvSize(
        (int) cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH),
        (int) cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT)
        );
    cout << "start" << endl;
    CvVideoWriter *writer = cvCreateVideoWriter(
        "a.avi",
        CV_FOURCC('M', 'J', 'P', 'G'),
        fps,
        size
        );
    IplImage* logpolar_frame = cvCreateImage(
        size,
        IPL_DEPTH_8U,
        3
        );
    cout << "end" << endl;
    int i = 0;
    while((bgr_frame = cvQueryFrame(capture)) != NULL)
    {
        cvLogPolar(bgr_frame, logpolar_frame, cvPoint2D32f(bgr_frame->width/2,bgr_frame->height/2), 40, CV_INTER_LINEAR+CV_WARP_FILL_OUTLIERS);
        cvWriteFrame(writer, logpolar_frame);
        cout << i << endl;
        ++i;
    }
    cvReleaseVideoWriter(&writer);
    cvReleaseImage(&logpolar_frame);
    cvReleaseCapture(&capture);
    return 0;
}