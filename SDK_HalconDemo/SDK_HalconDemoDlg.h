
// SDK_HalconDemoDlg.h: 头文件
//
#pragma once
#include <GoSdk\GoSdk.h>
#include <kApi\kApi.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <vector>
#include <halconcpp\HalconCpp.h>
#include <halconcpp\HException.h>
#include <iostream>
#include <chrono>
#include <halconcpp\HalconCppForwards.h>
#include "DealCamFun.h"
#include "SS_DJ_Measure.h"


// CSDKHalconDemoDlg 对话框
class CSDKHalconDemoDlg : public CDialogEx
{
// 构造
public:
	CSDKHalconDemoDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SDK_HALCONDEMO_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

// 实现
protected:
	HWND m_hWnd;            // must be first data member
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	CRect rc;
	CRect rc1;
	CRect rc2;
	CRect rc3;
	short int* height_map_memory = NULL;
	unsigned char* intensity_image_memory = NULL;
	k32u surfaceBufferHeight = 0;
	//HalconCpp::HObject *image;
	HalconCpp::HObject* ho_Height;
	HalconCpp::HObject* ho_Intens;
	
	HalconCpp::HObject uniformSurface_Short;
	HalconCpp::HTuple hv_a = 1, hv_b = 1;
	

	kAssembly api = kNULL;
	GoSystem system = kNULL;
	GoSensor sensor = kNULL;
	kStatus status;
	kIpAddress ipAddress;
	GoDataSet dataset = kNULL;
	unsigned int i, j;
	double xScale;
	double yScale;
	double scale;
	double bmpShowWidth;
	double bmpShowHeight;
	double xWndStart;
	double yWndStart;
	
	//置位为0时表示相机状态关闭，可以更改地址
	bool ChangeMark;
	//置位1时表示正常拍摄，置位0时表示暂停
	bool PauseMark;

	int CamCount = 1;

	typedef struct
	{
		double x;   // x-coordinate in engineering units (mm) - position along laser line
		double y;   // y-coordinate in engineering units (mm) - position along the direction of travel
		double z;   // z-coordinate in engineering units (mm) - height (at the given x position)
		unsigned char intensity;
	}ProfilePoint;

	typedef struct
	{
		k32u count;
	}DataContext;

	afx_msg void OnBnClickedOpencamera();
	afx_msg void OnBnClickedStopcamera();
	afx_msg void OnBnClickedAcquireimage();
	afx_msg void OnEnChangeShowInfo();
	afx_msg void OnBnClickedTest1();
	afx_msg void OnBnClickedPausecam();

	void WriteLog(CString s);
	void OnDrawShowInfo(int t, HalconCpp::HObject* img);
	void ShowImg(int t, HalconCpp::HObject *img);
	void set_display_font(HTuple hv_WindowHandle, HTuple hv_Size, HTuple hv_Font, HTuple hv_Bold, HTuple hv_Slant);
	void disp_message(HTuple hv_WindowHandle, HTuple hv_String, HTuple hv_CoordSystem, HTuple hv_Row, HTuple hv_Column, HTuple hv_Color, HTuple hv_Box);
	void displaySend(HalconCpp::HObject img);
	void displayImgInfo(HTuple hv_ChamPhi_L, HTuple hv_ChamPhi_R);
	void SendImgToDeal(HalconCpp::HObject img1, HalconCpp::HObject img2);
	void SaveImg(HTuple hv_num, HalconCpp::HObject* img);
	void ToSavePointCloud(CString fileName, ProfilePoint** surfaceBuffer, int row, int col, bool flag);
	


	afx_msg void OnBnClickedChangePath();
	afx_msg void OnClose();
};


