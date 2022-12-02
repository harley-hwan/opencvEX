#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int ac, char** av)
{
	Mat img = imread("road.png", 1);
	
	Mat img_line;
	img.copyTo(img_line);

	line(img_line, Point(230, 370), Point(450, 450), Scalar::all(255), 1, 8, 0);		// white line
	line(img_line, Point(230, 370), Point(0, 450), Scalar::all(255), 1, 8, 0);			// white line
	line(img_line, Point(230, 370), Point(231, 330), Scalar(0, 250, 0), 1, LINE_4, 0);	// green line
	line(img_line, Point(230, 370), Point(220, 600), Scalar(0, 0, 255), 2, 8, 0);		// red line

	imshow("img", img);
	imshow("img_line", img_line);

	waitKey(0);
	return 0;
}