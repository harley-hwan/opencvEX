#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int ac, char** av)
{
	Mat img = imread("deer.jpg", 0);

	Mat img_copy_1, img_copy_2;
	img.copyTo(img_copy_1);
	img_copy_2 = img;

	threshold(img_copy_2, img_copy_2, 150, 255, THRESH_BINARY);

	imshow("img_copy", img_copy_1);
	imshow("img_copy_2", img_copy_2);
	imshow("img", img);

	waitKey(0);
	return 0;
}