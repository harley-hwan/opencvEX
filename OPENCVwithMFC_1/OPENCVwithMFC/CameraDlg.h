#pragma once

#include "opencv2/opencv.hpp"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace std;
using namespace cv;

// CCameraDlg dialog

class CCameraDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCameraDlg)

public:
	CCameraDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CCameraDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CAMERA_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	
	// for camera func
	CStatic m_picture;
	VideoCapture capture;
	Mat mat_frame;
	Mat mat_result;
	CImage cimage_mfc;

	vector<vector<Point> > contours;
	vector<Point2f> approx;
	BOOL clickedRefresh;

	afx_msg bool compareContourAreas(vector<Point> contour1, vector<Point> contour2);
	afx_msg void DetectEdge(Mat mat_frame);
	afx_msg void setLabel(Mat& image, string str, vector<Point> contour);
	afx_msg void DetectRectangle();
	afx_msg void DrawCenterLine(Mat mat_frame);

	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedRefresh();
	
};
