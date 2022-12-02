#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <string>

using namespace std;
using namespace cv;

int main(int, char**)
{
    VideoCapture cap;
    cap.open("http://192.168.8.1:8080/?action=stream");
    int w = cvRound(cap.get(CAP_PROP_FRAME_WIDTH));
    int h = cvRound(cap.get(CAP_PROP_FRAME_HEIGHT));
    
    int centorW = w / 2 + 1;
    int centorH = h / 2 + 1;

    if (!cap.isOpened())
    {
        cout << "can not open Camera" << endl;
        return -1;
    }

    cout << "Frame width: " << cvRound(cap.get(CAP_PROP_FRAME_WIDTH)) << endl;
    cout << "Frame height: " << cvRound(cap.get(CAP_PROP_FRAME_HEIGHT)) << endl;
    cout << "Frame count: " << cvRound(cap.get(CAP_PROP_FRAME_COUNT)) << endl;

    double fps = cap.get(CAP_PROP_FPS);
    cout << "FPS: " << fps << endl;

    int delay = cvRound(1000 / fps);
    cout << "delay: " << delay << endl;

    //cap.set(CAP_PROP_FRAME_WIDTH, 1000);
    //cap.set(CAP_PROP_FRAME_HEIGHT, 1000);

    Mat frame;

    while (cap.read(frame))
    {
        // vertical line
        line(frame, Point(frame.cols / 2 + 1, 0),
            Point(frame.cols / 2 + 1, frame.rows - 1),
            Scalar(0, 0, 255));
        
        // horizontal line
        line(frame, Point(0, frame.rows / 2 + 1),
            Point(frame.cols - 1, frame.rows / 2 + 1),
            Scalar(0, 0, 255));

        imshow("Frame", frame);
        if (waitKey(10) == 27) 
            break;;
    }

    destroyAllWindows();
    return 0;
}