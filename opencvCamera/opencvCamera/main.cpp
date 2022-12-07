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
    //cap.open(0);
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

    Mat frame, mat_result;

    while (cap.read(frame))
    {
		Mat mat_gray;
		cvtColor(frame, mat_gray, COLOR_BGR2GRAY);

		Mat binary_image;
		threshold(mat_gray, mat_gray, 0, 255, THRESH_BINARY_INV | THRESH_OTSU);

		//contour를 찾는다.
		vector<vector<Point> > contours;
		findContours(mat_gray, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);

		//contour를 근사화한다.
		vector<Point2f> approx;
		mat_result = frame.clone();

		for (size_t i = 0; i < contours.size(); i++)
		{
			approxPolyDP(Mat(contours[i]), approx, arcLength(Mat(contours[i]), true) * 0.02, true);

			if (fabs(contourArea(Mat(approx))) > 100)  //면적이 일정크기 이상이어야 한다. 
			{

				int size = (int)approx.size();

				//Contour를 근사화한 직선을 그린다.
				if (size == 4)
				{
					line(mat_result, approx[0], approx[approx.size() - 1], Scalar(0, 255, 0), 3);

					for (int k = 0; k < size - 1; k++)
						line(mat_result, approx[k], approx[k + 1], Scalar(0, 255, 0), 3);

					for (int k = 0; k < size; k++)
						circle(mat_result, approx[k], 3, Scalar(0, 0, 255));
				}
			}
		}


        // vertical line
        line(mat_result, Point(mat_result.cols / 2 + 1, 0),
            Point(mat_result.cols / 2 + 1, mat_result.rows - 1),
            Scalar(0, 0, 255));
        
        // horizontal line
        line(mat_result, Point(0, mat_result.rows / 2 + 1),
            Point(mat_result.cols - 1, mat_result.rows / 2 + 1),
            Scalar(0, 0, 255));

        imshow("Frame", mat_result);
        if (waitKey(10) == 27) 
            break;;
    }

    destroyAllWindows();
    return 0;
}
