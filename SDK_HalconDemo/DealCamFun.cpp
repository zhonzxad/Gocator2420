#include "stdafx.h"
#include "DealCamFun.h"
#include <iostream>
#include <HALCONCpp/HalconCpp.h>
#include <HALCONCpp/HDevThread.h>
#include "resource.h"
#include "SDK_HalconDemoDlg.h"

using namespace HalconCpp;

//HTuple  hv_Width, hv_Height;
//HTuple  hv_WindowHandle;
//CSDKHalconDemoDlg m_mainDlg;
extern CSDKHalconDemoDlg* mg_dlg;

BOOL DealCamFun::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	
	//m_mainDlg.m_hWnd;
	// TODO: 在此添加额外的初始化代码
	//::GetClientRect(GetDlgItem(IDC_IMG_DEALSHOW)->m_hWnd, rc);
	//m_mainDlg.GetClientRect((LPRECT)GetDlgItem(IDC_IMG_DEALSHOW)->m_hWnd);
	
	
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void DealCamFun::FindCircRegion(HalconCpp::HObject ho_X, HalconCpp::HObject* ho_CircRegion)
{

	// Local iconic variables
	HalconCpp::HObject  ho_Region, ho_RegionFillUp, ho_RegionOpening;
	HalconCpp::HObject  ho_RegionTrans;

	// Local control variables
	HTuple  hv_Value;

	Threshold(ho_X, &ho_Region, 1000, 100000);
	//对阈值后的区域做填充操作，得到封闭周面区域。（原因：周面会出现丢帧现象）
	FillUpShape(ho_Region, &ho_RegionFillUp, "area", 1, 10000);
	//开运算的作用是除去样品以外的区域
	OpeningCircle(ho_RegionFillUp, &ho_RegionOpening, 3.5);
	//求得周面所在外接矩形区域
	ShapeTrans(ho_RegionOpening, &ho_RegionTrans, "rectangle1");
	//求得外接矩形的特征值，特征值分别为矩形的高和宽，左上角行列坐标，右下角行列坐标
	RegionFeatures(ho_RegionTrans, (((((HTuple("height").Append("width")).Append("row1")).Append("column1")).Append("row2")).Append("column2")),
		&hv_Value);
	//求得样品一周的矩形区域，求得除去倒角的矩形区域
	//不锈钢样品一
	GenRectangle1(&(*ho_CircRegion), HTuple(hv_Value[2]), HTuple(hv_Value[3]) + 20, HTuple(hv_Value[4]) - 300,
		HTuple(hv_Value[5]) - 20);
	//不锈钢样品二
	//gen_rectangle1 (CircRegion, Value[2], Value[3]+20, Value[4]-220, Value[5]-20)
	//不锈钢样品三
	//gen_rectangle1 (CircRegion, Value[2], Value[3]+20, Value[4]-257, Value[5]-20)
	//磁性样品
	//gen_rectangle1 (CircRegion, Value[2], Value[3]+20, Value[4]-230, Value[5]-20)
	//difference (RegionTrans, CircRegion, RegionDifference)
	return;
}

void DealCamFun::FindDefect_high(HalconCpp::HObject ho_CircImage, HalconCpp::HObject* ho_Defect_high, HTuple hv_Height)
{

	// Local iconic variables
	HalconCpp::HObject  ho_Rectangle, ho_Region, ho_RegionFillUp;
	HalconCpp::HObject  ho_RegionOpening, ho_ConnectedRegions;

	// Local control variables
	HTuple  hv_HeightMaxValue0, hv_k, hv_Value, hv_HeightMaxValueSorted;
	HTuple  hv_StandardValue, hv_StandardHeightValue;

	hv_HeightMaxValue0 = HTuple();
	//对图像进行遍历，求高度图像的最大值，放到数组 HeightMaxValue0 中
	{
		HTuple end_val2 = hv_Height;
		HTuple step_val2 = 3;
		for (hv_k = 0; hv_k.Continue(end_val2, step_val2); hv_k += step_val2)
		{
			GenRectangle1(&ho_Rectangle, hv_k, 0, hv_k + 3, 2000);
			//计算小矩形所在图像的最大高度值
			GrayFeatures(ho_Rectangle, ho_CircImage, "max", &hv_Value);
			hv_HeightMaxValue0 = hv_HeightMaxValue0.TupleConcat(hv_Value);
		}
	}
	TupleSort(hv_HeightMaxValue0, &hv_HeightMaxValueSorted);
	//对所有的最大高度值进行排序
	TupleSelectRange(hv_HeightMaxValueSorted, (hv_HeightMaxValueSorted.TupleLength()) / 4,
		((hv_HeightMaxValueSorted.TupleLength()) / 4) * 3, &hv_StandardValue);
	//求出高度图像的高度平均值，即得到基准面
	TupleMean(hv_StandardValue, &hv_StandardHeightValue);
	Threshold(ho_CircImage, &ho_Region, 0, hv_StandardHeightValue - 1000);
	//填充缺陷区域中的小的空洞
	FillUpShape(ho_Region, &ho_RegionFillUp, "area", 1, 1500);
	//去掉小的与大缺陷区域不连接的小区域
	OpeningCircle(ho_RegionFillUp, &ho_RegionOpening, 5);
	Connection(ho_RegionOpening, &ho_ConnectedRegions);
	GenEmptyObj(&(*ho_Defect_high));
	SelectShape(ho_ConnectedRegions, &(*ho_Defect_high), "area", "and", 1500, 99999999999999999);
	return;
}

void DealCamFun::FindDefect_light(HalconCpp::HObject ho_Y, HalconCpp::HObject ho_CircRegion, HalconCpp::HObject* ho_Defect_light)
{

	// Local iconic variables
	HalconCpp::HObject  ho_HightImage, ho_HightImageSmooth, ho_Regions;
	HalconCpp::HObject  ho_ConnectedRegions, ho_RegionClosing, ho_SelectedRegions;
	HalconCpp::HObject  ho_RegionDifference, ho_RegionErosion;

	ReduceDomain(ho_Y, ho_CircRegion, &ho_HightImage);
	SmoothImage(ho_HightImage, &ho_HightImageSmooth, "deriche2", 0.5);
	Threshold(ho_HightImageSmooth, &ho_Regions, 50000, 600000);
	Connection(ho_Regions, &ho_ConnectedRegions);
	OpeningCircle(ho_ConnectedRegions, &ho_RegionClosing, 10);
	SelectShape(ho_RegionClosing, &ho_SelectedRegions, "area", "and", 20000, 9999999999999);
	Difference(ho_CircRegion, ho_SelectedRegions, &ho_RegionDifference);
	ErosionCircle(ho_RegionDifference, &ho_RegionErosion, 3.5);
	Connection(ho_RegionErosion, &ho_ConnectedRegions);
	SelectShape(ho_ConnectedRegions, &(*ho_Defect_light), "area", "and", 1500, 99999999);
	return;
}

void DealCamFun::DealCamCpp(HalconCpp::HObject ho_Height, HalconCpp::HObject ho_Intens)
{
	// Local iconic variables
	HalconCpp::HObject  ho_CircRegion, ho_CircImage;
	HalconCpp::HObject  ho_Defect_high, ho_Defect_light, ho_MistakingRegion;
	HalconCpp::HObject  ho_ObjectSelected, ho_RegionInter, ho_Defect;

	// Local control variables
	HTuple  hv_Width, hv_Height, hv_WindowHandle;
	HTuple  hv_Number, hv_i, hv_RegionInterArea, hv_Row, hv_Column;
	HTuple  hv_Area1, hv_Row1, hv_Column1, hv_Area2, hv_Row2;
	HTuple  hv_Column2, hv_Area_percent, hv_DefectArea;


	//计算图像的尺寸，Width指图像的宽，Height指图像的高
	GetImageSize(ho_Height, &hv_Width, &hv_Height);
	//打开图形窗口
	//SetWindowAttr("background_color", "black");
	//OpenWindow(0, 0, hv_Width / 2, hv_Height / 2, 0, "visible", "", &hv_WindowHandle);
	//HDevWindowStack::Push(hv_WindowHandle);
	//得到除去倒角的一周的区域
	FindCircRegion(ho_Height, &ho_CircRegion);
	//得到样件一圈周面图像
	ReduceDomain(ho_Height, ho_CircRegion, &ho_CircImage);
	//if (HDevWindowStack::IsOpen())
		//DispObj(ho_CircImage, HDevWindowStack::GetActive());
	//对高度图像进行处理
	//如果高度图上没有缺陷，则高度值都在一个值附近波动，认为超出该值的为缺陷区域
	//找出高度图的基准面（把高度图像按照小矩形进行分割，计算出最大的高度值）
	//定义：HeightMaxValue0 为存放 3个像素宽的小矩形所在高度图像的最大高度值
	FindDefect_high(ho_CircImage, &ho_Defect_high, hv_Height);
	//if (HDevWindowStack::IsOpen())
		//DispObj(ho_Defect_high, HDevWindowStack::GetActive());
	//利用亮度图找缺陷样品,先粗找缺陷区域(目的是：排除抖动时，误找的缺陷区域)
	FindDefect_light(ho_Intens, ho_CircRegion, &ho_Defect_light);
	//if (HDevWindowStack::IsOpen())
		//DispObj(ho_Defect_light, HDevWindowStack::GetActive());
	//找误判缺陷区域
	CountObj(ho_Defect_high, &hv_Number);
	GenEmptyObj(&ho_MistakingRegion);
	{
		HTuple end_val32 = hv_Number;
		HTuple step_val32 = 1;
		for (hv_i = 1; hv_i.Continue(end_val32, step_val32); hv_i += step_val32)
		{
			SelectObj(ho_Defect_high, &ho_ObjectSelected, hv_i);
			Intersection(ho_ObjectSelected, ho_Defect_light, &ho_RegionInter);
			AreaCenter(ho_RegionInter, &hv_RegionInterArea, &hv_Row, &hv_Column);
			if (0 != (hv_RegionInterArea == 0))
			{
				Union2(ho_MistakingRegion, ho_ObjectSelected, &ho_MistakingRegion);
			}
		}
	}

	Difference(ho_Defect_high, ho_MistakingRegion, &ho_Defect);
	//计算缺陷面积占比
	AreaCenter(ho_Defect, &hv_Area1, &hv_Row1, &hv_Column1);
	AreaCenter(ho_CircRegion, &hv_Area2, &hv_Row2, &hv_Column2);
	hv_Area_percent = 0;
	//把整数转化为浮点数
	TupleReal(hv_Area2, &hv_Area2);
	TupleReal(hv_Area1, &hv_Area1);
	hv_Area_percent = hv_Area1 / hv_Area2;
	if (0 != ((hv_Area_percent.TupleLength()) == 0))
	{
		hv_DefectArea = 0;
	}
	else
	{
		TupleSum(hv_Area_percent, &hv_DefectArea);
	}
	//if (HDevWindowStack::IsOpen())
		//DispObj(ho_CircImage, HDevWindowStack::GetActive());
	//if (HDevWindowStack::IsOpen())
		//DispObj(ho_Intens, HDevWindowStack::GetActive());
	//if (HDevWindowStack::IsOpen())
		//DispObj(ho_Defect, HDevWindowStack::GetActive());

	mg_dlg->ShowImg(3, &ho_Intens);
	mg_dlg->displaySend(ho_Intens);
	mg_dlg->displaySend(ho_Defect);
	//fwrite_string (FileHandle_defect, (Index+1)+'\t')
	//fwrite_string (FileHandle_defect, DefectArea+'\t')
	//fnew_line (FileHandle_defect)
	//close_file (FileHandle_defect)
}

