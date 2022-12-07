
// OPENCVwithMFCDlg.h : header file
//

#pragma once

#include "opencv2/opencv.hpp"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include "CameraDlg.h"
#include "CEXPLORER1.h"

using namespace cv;

// COPENCVwithMFCDlg dialog
class COPENCVwithMFCDlg : public CDialogEx
{
// Construction
public:
	COPENCVwithMFCDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_OPENCVWITHMFC_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);


	afx_msg void OnBnClickedCamera();
	CEXPLORER1 m_explorer;
};
