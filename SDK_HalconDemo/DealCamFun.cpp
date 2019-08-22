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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	
	//m_mainDlg.m_hWnd;
	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	//::GetClientRect(GetDlgItem(IDC_IMG_DEALSHOW)->m_hWnd, rc);
	//m_mainDlg.GetClientRect((LPRECT)GetDlgItem(IDC_IMG_DEALSHOW)->m_hWnd);
	
	
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void DealCamFun::FindCircRegion(HalconCpp::HObject ho_X, HalconCpp::HObject* ho_CircRegion)
{

	// Local iconic variables
	HalconCpp::HObject  ho_Region, ho_RegionFillUp, ho_RegionOpening;
	HalconCpp::HObject  ho_RegionTrans;

	// Local control variables
	HTuple  hv_Value;

	Threshold(ho_X, &ho_Region, 1000, 100000);
	//����ֵ������������������õ�����������򡣣�ԭ���������ֶ�֡����
	FillUpShape(ho_Region, &ho_RegionFillUp, "area", 1, 10000);
	//������������ǳ�ȥ��Ʒ���������
	OpeningCircle(ho_RegionFillUp, &ho_RegionOpening, 3.5);
	//�������������Ӿ�������
	ShapeTrans(ho_RegionOpening, &ho_RegionTrans, "rectangle1");
	//�����Ӿ��ε�����ֵ������ֵ�ֱ�Ϊ���εĸߺͿ����Ͻ��������꣬���½���������
	RegionFeatures(ho_RegionTrans, (((((HTuple("height").Append("width")).Append("row1")).Append("column1")).Append("row2")).Append("column2")),
		&hv_Value);
	//�����Ʒһ�ܵľ���������ó�ȥ���ǵľ�������
	//�������Ʒһ
	GenRectangle1(&(*ho_CircRegion), HTuple(hv_Value[2]), HTuple(hv_Value[3]) + 20, HTuple(hv_Value[4]) - 300,
		HTuple(hv_Value[5]) - 20);
	//�������Ʒ��
	//gen_rectangle1 (CircRegion, Value[2], Value[3]+20, Value[4]-220, Value[5]-20)
	//�������Ʒ��
	//gen_rectangle1 (CircRegion, Value[2], Value[3]+20, Value[4]-257, Value[5]-20)
	//������Ʒ
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
	//��ͼ����б�������߶�ͼ������ֵ���ŵ����� HeightMaxValue0 ��
	{
		HTuple end_val2 = hv_Height;
		HTuple step_val2 = 3;
		for (hv_k = 0; hv_k.Continue(end_val2, step_val2); hv_k += step_val2)
		{
			GenRectangle1(&ho_Rectangle, hv_k, 0, hv_k + 3, 2000);
			//����С��������ͼ������߶�ֵ
			GrayFeatures(ho_Rectangle, ho_CircImage, "max", &hv_Value);
			hv_HeightMaxValue0 = hv_HeightMaxValue0.TupleConcat(hv_Value);
		}
	}
	TupleSort(hv_HeightMaxValue0, &hv_HeightMaxValueSorted);
	//�����е����߶�ֵ��������
	TupleSelectRange(hv_HeightMaxValueSorted, (hv_HeightMaxValueSorted.TupleLength()) / 4,
		((hv_HeightMaxValueSorted.TupleLength()) / 4) * 3, &hv_StandardValue);
	//����߶�ͼ��ĸ߶�ƽ��ֵ�����õ���׼��
	TupleMean(hv_StandardValue, &hv_StandardHeightValue);
	Threshold(ho_CircImage, &ho_Region, 0, hv_StandardHeightValue - 1000);
	//���ȱ�������е�С�Ŀն�
	FillUpShape(ho_Region, &ho_RegionFillUp, "area", 1, 1500);
	//ȥ��С�����ȱ���������ӵ�С����
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


	//����ͼ��ĳߴ磬Widthָͼ��Ŀ�Heightָͼ��ĸ�
	GetImageSize(ho_Height, &hv_Width, &hv_Height);
	//��ͼ�δ���
	//SetWindowAttr("background_color", "black");
	//OpenWindow(0, 0, hv_Width / 2, hv_Height / 2, 0, "visible", "", &hv_WindowHandle);
	//HDevWindowStack::Push(hv_WindowHandle);
	//�õ���ȥ���ǵ�һ�ܵ�����
	FindCircRegion(ho_Height, &ho_CircRegion);
	//�õ�����һȦ����ͼ��
	ReduceDomain(ho_Height, ho_CircRegion, &ho_CircImage);
	//if (HDevWindowStack::IsOpen())
		//DispObj(ho_CircImage, HDevWindowStack::GetActive());
	//�Ը߶�ͼ����д���
	//����߶�ͼ��û��ȱ�ݣ���߶�ֵ����һ��ֵ������������Ϊ������ֵ��Ϊȱ������
	//�ҳ��߶�ͼ�Ļ�׼�棨�Ѹ߶�ͼ����С���ν��зָ��������ĸ߶�ֵ��
	//���壺HeightMaxValue0 Ϊ��� 3�����ؿ��С�������ڸ߶�ͼ������߶�ֵ
	FindDefect_high(ho_CircImage, &ho_Defect_high, hv_Height);
	//if (HDevWindowStack::IsOpen())
		//DispObj(ho_Defect_high, HDevWindowStack::GetActive());
	//��������ͼ��ȱ����Ʒ,�ȴ���ȱ������(Ŀ���ǣ��ų�����ʱ�����ҵ�ȱ������)
	FindDefect_light(ho_Intens, ho_CircRegion, &ho_Defect_light);
	//if (HDevWindowStack::IsOpen())
		//DispObj(ho_Defect_light, HDevWindowStack::GetActive());
	//������ȱ������
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
	//����ȱ�����ռ��
	AreaCenter(ho_Defect, &hv_Area1, &hv_Row1, &hv_Column1);
	AreaCenter(ho_CircRegion, &hv_Area2, &hv_Row2, &hv_Column2);
	hv_Area_percent = 0;
	//������ת��Ϊ������
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

