// CameraDlg.cpp : implementation file
//

#include "pch.h"
#include "OPENCVwithMFC.h"
#include "CameraDlg.h"
#include "afxdialogex.h"


// CCameraDlg dialog

IMPLEMENT_DYNAMIC(CCameraDlg, CDialogEx)

CCameraDlg::CCameraDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CAMERA_DIALOG, pParent)
{

}

CCameraDlg::~CCameraDlg()
{
}

void CCameraDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CAMERA, m_picture);
}


BEGIN_MESSAGE_MAP(CCameraDlg, CDialogEx)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_REFRESH, &CCameraDlg::OnBnClickedRefresh)
END_MESSAGE_MAP()


// CCameraDlg message handlers


BOOL CCameraDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here

	//capture.open("http://192.168.8.1:8080/?action=stream");
	capture.open(0);

	if (!capture.isOpened())
	{
		MessageBox(_T("Camera not opened \n"));
	}
	else
	{
		MessageBox(_T("Camera opened \n"));
	}

	// 웹캠 크기를  320x240으로 지정
	//capture.set(CAP_PROP_FRAME_WIDTH, 320);
	//capture.set(CAP_PROP_FRAME_HEIGHT, 240);

	SetTimer(1000, 30, NULL);
	clickedRefresh = true;

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void CCameraDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	capture.read(mat_result);

	//DrawCenterLine(mat_result);			// 수평, 수직 센터 라인 그리기

	//이곳에 OpenCV 함수들을 적용합니다.
	//여기에서는 그레이스케일 이미지로 변환합니다.
	//cvtColor(mat_frame, mat_frame, COLOR_BGR2GRAY);
	// 
	//DetectEdge(mat_frame);
	if (clickedRefresh)
	{
		clickedRefresh = false;
		DetectRectangle();
	}
	
	//for (size_t i = 0; i < contours.size(); i++)
	//{
	//	approxPolyDP(Mat(contours[i]), approx, arcLength(Mat(contours[i]), true) * 0.02, true);

	//	if (fabs(contourArea(Mat(approx))) > 100)  //면적이 일정크기 이상이어야 한다. 
	//	{
	//		int size = (int)approx.size();

	//		//Contour를 근사화한 직선을 그린다.
	//		if (size >= 3 && size <= 5)
	//		{
	//			line(mat_result, approx[0], approx[approx.size() - 1], Scalar(0, 255, 0), 3);

	//			for (int k = 0; k < size - 1; k++)
	//				line(mat_result, approx[k], approx[k + 1], Scalar(0, 255, 0), 3);
	//		}
	//	}
	//} 
	// 
	// 
	//for (size_t idx = 0; idx < contours.size(); idx++)
	//{
	//	RotatedRect rect = minAreaRect(contours[idx]);
	//	double areaRatio = abs(contourArea(contours[idx])) / (rect.size.width * rect.size.height);
	//	//drawContours(contours_img, contours, idx, Scalar(255, 0, 255), 2);
	//}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	//화면에 보여주기 위한 처리입니다.
	int bpp = 8 * (int)mat_result.elemSize();
	assert((bpp == 8 || bpp == 24 || bpp == 32));

	int padding = 0;
	//32 bit image is always DWORD aligned because each pixel requires 4 bytes
	if (bpp < 32)
		padding = 4 - (mat_result.cols % 4);

	if (padding == 4)
		padding = 0;

	int border = 0;
	//32 bit image is always DWORD aligned because each pixel requires 4 bytes
	if (bpp < 32)
	{
		border = 4 - (mat_result.cols % 4);
	}

	Mat mat_temp;
	if (border > 0 || mat_result.isContinuous() == false)
	{
		// Adding needed columns on the right (max 3 px)
		cv::copyMakeBorder(mat_result, mat_temp, 0, 0, 0, border, cv::BORDER_CONSTANT, 0);
	}
	else
	{
		mat_temp = mat_result;
	}

	RECT r;
	m_picture.GetClientRect(&r);
	cv::Size winSize(r.right, r.bottom);

	cimage_mfc.Create(winSize.width, winSize.height, 24);


	BITMAPINFO* bitInfo = (BITMAPINFO*)malloc(sizeof(BITMAPINFO) + 256 * sizeof(RGBQUAD));
	bitInfo->bmiHeader.biBitCount = bpp;
	bitInfo->bmiHeader.biWidth = mat_temp.cols;
	bitInfo->bmiHeader.biHeight = -mat_temp.rows;
	bitInfo->bmiHeader.biPlanes = 1;
	bitInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitInfo->bmiHeader.biCompression = BI_RGB;
	bitInfo->bmiHeader.biClrImportant = 0;
	bitInfo->bmiHeader.biClrUsed = 0;
	bitInfo->bmiHeader.biSizeImage = 0;
	bitInfo->bmiHeader.biXPelsPerMeter = 0;
	bitInfo->bmiHeader.biYPelsPerMeter = 0;


	//그레이스케일 인경우 팔레트가 필요
	//if (bpp == 8)
	//{
	//	RGBQUAD* palette = bitInfo->bmiColors;
	//	for (int i = 0; i < 256; i++)
	//	{
	//		palette[i].rgbBlue = palette[i].rgbGreen = palette[i].rgbRed = (BYTE)i;
	//		palette[i].rgbReserved = 0;
	//	}
	//}


	// Image is bigger or smaller than into destination rectangle
	// we use stretch in full rect

	if (mat_temp.cols == winSize.width && mat_temp.rows == winSize.height)
	{
		// source and destination have same size
		// transfer memory block
		// NOTE: the padding border will be shown here. Anyway it will be max 3px width

		SetDIBitsToDevice(cimage_mfc.GetDC(),
			//destination rectangle
			0, 0, winSize.width, winSize.height,
			0, 0, 0, mat_temp.rows,
			mat_temp.data, bitInfo, DIB_RGB_COLORS);
	}
	else
	{
		// destination rectangle
		int destx = 0, desty = 0;
		int destw = winSize.width;
		int desth = winSize.height;

		// rectangle defined on source bitmap
		// using imgWidth instead of mat_temp.cols will ignore the padding border
		int imgx = 0, imgy = 0;
		int imgWidth = mat_temp.cols - border;
		int imgHeight = mat_temp.rows;

		StretchDIBits(cimage_mfc.GetDC(),
			destx, desty, destw, desth,
			imgx, imgy, imgWidth, imgHeight,
			mat_temp.data, bitInfo, DIB_RGB_COLORS, SRCCOPY);
	}


	HDC dc = ::GetDC(m_picture.m_hWnd);
	cimage_mfc.BitBlt(dc, 0, 0);


	::ReleaseDC(m_picture.m_hWnd, dc);

	cimage_mfc.ReleaseDC();
	cimage_mfc.Destroy();

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	CDialogEx::OnTimer(nIDEvent);
}

void CCameraDlg::DrawCenterLine(Mat mat_frame)
{
	line(mat_frame, Point(mat_frame.cols / 2 + 1, 0),
		Point(mat_frame.cols / 2 + 1, mat_frame.rows - 1),
		Scalar(0, 0, 255));

	// horizontal line
	line(mat_frame, Point(0, mat_frame.rows / 2 + 1),
		Point(mat_frame.cols - 1, mat_frame.rows / 2 + 1),
		Scalar(0, 0, 255));
}

bool CCameraDlg::compareContourAreas(vector<Point> contour1, vector<Point> contour2)
{
	double i = fabs(contourArea(Mat(contour1)));
	double j = fabs(contourArea(Mat(contour2)));
	return (i > j);
}

void CCameraDlg::DetectEdge(Mat mat_frame)
{
	// Declare the output variables
	Mat dst, cdst, cdstP;

	// Loads an image

	// Check if image is loaded fine

	// Edge detection
	Canny(mat_frame, dst, 50, 200, 3);
	// Copy edges to the images that will display the results in BGR
	cvtColor(dst, cdst, COLOR_GRAY2BGR);
	cdstP = cdst.clone();
	// Standard Hough Line Transform
	vector<Vec2f> lines; // will hold the results of the detection
	HoughLines(dst, lines, 1, CV_PI / 180, 150, 0, 0); // runs the actual detection
	// Draw the lines
	for (size_t i = 0; i < lines.size(); i++)
	{
		float rho = lines[i][0], theta = lines[i][1];
		Point pt1, pt2;
		double a = cos(theta), b = sin(theta);
		double x0 = a * rho, y0 = b * rho;
		pt1.x = cvRound(x0 + 1000 * (-b));
		pt1.y = cvRound(y0 + 1000 * (a));
		pt2.x = cvRound(x0 - 1000 * (-b));
		pt2.y = cvRound(y0 - 1000 * (a));
		line(cdst, pt1, pt2, Scalar(0, 0, 255), 3, LINE_AA);
	}
	// Probabilistic Line Transform
	vector<Vec4i> linesP; // will hold the results of the detection
	HoughLinesP(dst, linesP, 1, CV_PI / 180, 50, 50, 10); // runs the actual detection
	// Draw the lines
	for (size_t i = 0; i < linesP.size(); i++)
	{
		Vec4i l = linesP[i];
		line(mat_frame, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0, 0, 255), 3, LINE_AA);
	}
}

void CCameraDlg::setLabel(Mat& image, string str, vector<Point> contour)
{
	int fontface = FONT_HERSHEY_SIMPLEX;
	double scale = 0.5;
	int thickness = 1;
	int baseline = 0;

	Size text = getTextSize(str, fontface, scale, thickness, &baseline);
	Rect r = boundingRect(contour);

	Point pt(r.x + ((r.width - text.width) / 2), r.y + ((r.height + text.height) / 2));
	rectangle(image, pt + Point(0, baseline), pt + Point(text.width, -text.height), CV_RGB(200, 200, 200), FILLED);
	putText(image, str, pt, fontface, scale, CV_RGB(0, 0, 0), thickness, 8);
}

void CCameraDlg::DetectRectangle()
{
	Mat mat_gray, mat_canny;
	cvtColor(mat_result, mat_gray, COLOR_BGR2GRAY);

	blur(mat_gray, mat_canny, Size(3, 3));
	Canny(mat_canny, mat_canny, 50, 100);
	//Canny(mat_canny, mat_canny, nThreshold, nThreshold * ratio, kernelSize);

	Mat kernel = getStructuringElement(MORPH_ELLIPSE, Size(11, 11));
	Mat morph;
	morphologyEx(mat_canny, morph, MORPH_CLOSE, kernel);

	//size_t idx, i;
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;

	//contour를 찾는다.
	findContours(morph, contours, hierarchy, RETR_CCOMP, CHAIN_APPROX_SIMPLE, Point(0, 0));

	//Mat contours_img;

	//cvtColor(mat_canny, mat_result, COLOR_GRAY2BGR);
	//for (size_t idx = 0; idx < contours.size(); idx++)
	//{
	//	RotatedRect rect = minAreaRect(contours[idx]);
	//	double areaRatio = abs(contourArea(contours[idx])) / (rect.size.width * rect.size.height);
	//	//drawContours(contours_img, contours, idx, Scalar(255, 0, 255), 2);
	//}


	//vector<Point> poly;

	//Mat poly_img, ractangle_img;
	////cvtColor(mat_canny, poly_img, COLOR_GRAY2BGR);
	////cvtColor(mat_canny, mat_result, COLOR_GRAY2BGR);

	//for (idx = 0; idx < contours.size(); idx++)
	//{
	//	approxPolyDP(contours[idx], poly, 1, true);

	//	for (i = 0; i < poly.size(); i++)
	//	{
	//		//line(poly_img, poly[i], poly[(i + 1) % poly.size()], Scalar(0, 255, 255), 2);
	//		if (poly.size() == 4)
	//		{
	//			line(mat_result, poly[i], poly[(i + 1) % poly.size()], (0, 125, 125), 2);
	//		}
	//	}
	//}

}

void CCameraDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: Add your message handler code here
}


void CCameraDlg::OnBnClickedRefresh()
{
	// TODO: Add your control notification handler code here
	clickedRefresh = true;	
}
