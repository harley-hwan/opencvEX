#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int ac, char** av)
{
	Mat img = imread("deer.jpg", 0);
	Mat img_gaussian, img_canny;
	Mat img_sobel_x, img_sobel_y, img_sobel;
	Mat img_lap;

	GaussianBlur(img, img_gaussian, Size(3, 3), 0, 0, 4);

	Sobel(img_gaussian, img_sobel_x, CV_8U, 1, 0, 3, (1.0), (0, 0), 4);
	Sobel(img_gaussian, img_sobel_y, CV_8U, 0, 1, 3, (1.0), (0, 0), 4);
	addWeighted(img_sobel_x, 1, img_sobel_y, 1, 0, img_sobel);

	Laplacian(img_gaussian, img_lap, CV_8U, 3, (1.0), (0, 0), 4);

	Canny(img, img_canny, 80, 120);

	imshow("original", img);
	imshow("img_sobel_x", img_sobel_x);
	imshow("img_sobel_y", img_sobel_y);
	imshow("img_sobel", img_sobel);
	imshow("img_lap", img_lap);
	imshow("img_canny", img_canny);
	waitKey(0);

	return 0;
}