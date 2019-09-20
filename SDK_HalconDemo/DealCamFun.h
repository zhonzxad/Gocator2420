#pragma once

#include <HALCONCpp/HalconCpp.h>
#include <HALCONCpp/HDevThread.h>
#include <afxwin.h>
#include <algorithm>
#include <ctime>
#include <chrono>

using namespace HalconCpp;

class DealCamFun : public CDialogEx
{
	
protected:
	HWND m_hWnd;            // must be first data member
	virtual BOOL OnInitDialog();

public:
	void FindCircRegion(HalconCpp::HObject ho_X, HalconCpp::HObject* ho_CircRegion);
	void FindDefect_high(HalconCpp::HObject ho_CircImage, HalconCpp::HObject* ho_Defect_high, HTuple hv_Height);
	void FindDefect_light(HalconCpp::HObject ho_Y, HalconCpp::HObject ho_CircRegion, HalconCpp::HObject* ho_Defect_light);

	

	void DealCamCpp(HalconCpp::HObject ho_Heigh, HalconCpp::HObject ho_Intens);

public:
	//CSDKHalconDemoDlg* mg_dlg = NULL;

	HTuple  hv_DefectArea;  //缺陷占比量
	clock_t endTime, startTime;
	clock_t QX_JC_runTime;	//缺陷程序运行时间
};

