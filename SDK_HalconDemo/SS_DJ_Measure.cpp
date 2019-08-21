#include "stdafx.h"
#include "SS_DJ_Measure.h"
#include <iostream>
#include <HALCONCpp/HalconCpp.h>
#include <HALCONCpp/HDevThread.h>
#include "resource.h"
#include "SDK_HalconDemoDlg.h"

extern CSDKHalconDemoDlg* mg_dlg;

BOOL SS_DJ_Measure::OnInitDialog()
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

void SS_DJ_Measure::calculate_lines_gauss_parameters(HTuple hv_MaxLineWidth, HTuple hv_Contrast, HTuple* hv_Sigma, HTuple* hv_Low, HTuple* hv_High)
{
	// Local iconic variables

  // Local control variables
	HTuple  hv_ContrastHigh, hv_ContrastLow, hv_HalfWidth;
	HTuple  hv_Help;

	//Check control parameters
	if (0 != ((hv_MaxLineWidth.TupleLength()) != 1))
	{
		throw HException("Wrong number of values of control parameter: 1");
	}
	if (0 != ((hv_MaxLineWidth.TupleIsNumber()).TupleNot()))
	{
		throw HException("Wrong type of control parameter: 1");
	}
	if (0 != (hv_MaxLineWidth <= 0))
	{
		throw HException("Wrong value of control parameter: 1");
	}
	if (0 != (HTuple((hv_Contrast.TupleLength()) != 1).TupleAnd((hv_Contrast.TupleLength()) != 2)))
	{
		throw HException("Wrong number of values of control parameter: 2");
	}
	if (0 != (((hv_Contrast.TupleIsNumber()).TupleMin()) == 0))
	{
		throw HException("Wrong type of control parameter: 2");
	}
	//Set and check ContrastHigh
	hv_ContrastHigh = ((const HTuple&)hv_Contrast)[0];
	if (0 != (hv_ContrastHigh < 0))
	{
		throw HException("Wrong value of control parameter: 2");
	}
	//Set or derive ContrastLow
	if (0 != ((hv_Contrast.TupleLength()) == 2))
	{
		hv_ContrastLow = ((const HTuple&)hv_Contrast)[1];
	}
	else
	{
		hv_ContrastLow = hv_ContrastHigh / 3.0;
	}
	//Check ContrastLow
	if (0 != (hv_ContrastLow < 0))
	{
		throw HException("Wrong value of control parameter: 2");
	}
	if (0 != (hv_ContrastLow > hv_ContrastHigh))
	{
		throw HException("Wrong value of control parameter: 2");
	}
	//
	//Calculate the parameters Sigma, Low, and High for lines_gauss
	if (0 != (hv_MaxLineWidth < (HTuple(3.0).TupleSqrt())))
	{
		//Note that LineWidthMax < sqrt(3.0) would result in a Sigma < 0.5,
		//which does not make any sense, because the corresponding smoothing
		//filter mask would be of size 1x1.
		//To avoid this, LineWidthMax is restricted to values greater or equal
		//to sqrt(3.0) and the contrast values are adapted to reflect the fact
		//that lines that are thinner than sqrt(3.0) pixels have a lower contrast
		//in the smoothed image (compared to lines that are sqrt(3.0) pixels wide).
		hv_ContrastLow = (hv_ContrastLow * hv_MaxLineWidth) / (HTuple(3.0).TupleSqrt());
		hv_ContrastHigh = (hv_ContrastHigh * hv_MaxLineWidth) / (HTuple(3.0).TupleSqrt());
		hv_MaxLineWidth = HTuple(3.0).TupleSqrt();
	}
	//Convert LineWidthMax and the given contrast values into the input parameters
	//Sigma, Low, and High required by lines_gauss
	hv_HalfWidth = hv_MaxLineWidth / 2.0;
	(*hv_Sigma) = hv_HalfWidth / (HTuple(3.0).TupleSqrt());
	hv_Help = ((-2.0 * hv_HalfWidth) / ((HTuple(6.283185307178).TupleSqrt()) * ((*hv_Sigma).TuplePow(3.0)))) * ((-0.5 * ((hv_HalfWidth / (*hv_Sigma)).TuplePow(2.0))).TupleExp());
	(*hv_High) = (hv_ContrastHigh * hv_Help).TupleFabs();
	(*hv_Low) = (hv_ContrastLow * hv_Help).TupleFabs();
	return;
}

void SS_DJ_Measure::dev_open_window_fit_image(HalconCpp::HObject ho_Image, HTuple hv_Row, HTuple hv_Column, HTuple hv_WidthLimit, HTuple hv_HeightLimit, HTuple* hv_WindowHandle)
{
	// Local iconic variables

  // Local control variables
	HTuple  hv_MinWidth, hv_MaxWidth, hv_MinHeight;
	HTuple  hv_MaxHeight, hv_ResizeFactor, hv_ImageWidth, hv_ImageHeight;
	HTuple  hv_TempWidth, hv_TempHeight, hv_WindowWidth, hv_WindowHeight;

	//This procedure opens a new graphics window and adjusts the size
	//such that it fits into the limits specified by WidthLimit
	//and HeightLimit, but also maintains the correct image aspect ratio.
	//
	//If it is impossible to match the minimum and maximum extent requirements
	//at the same time (f.e. if the image is very long but narrow),
	//the maximum value gets a higher priority,
	//
	//Parse input tuple WidthLimit
	if (0 != (HTuple((hv_WidthLimit.TupleLength()) == 0).TupleOr(hv_WidthLimit < 0)))
	{
		hv_MinWidth = 500;
		hv_MaxWidth = 800;
	}
	else if (0 != ((hv_WidthLimit.TupleLength()) == 1))
	{
		hv_MinWidth = 0;
		hv_MaxWidth = hv_WidthLimit;
	}
	else
	{
		hv_MinWidth = ((const HTuple&)hv_WidthLimit)[0];
		hv_MaxWidth = ((const HTuple&)hv_WidthLimit)[1];
	}
	//Parse input tuple HeightLimit
	if (0 != (HTuple((hv_HeightLimit.TupleLength()) == 0).TupleOr(hv_HeightLimit < 0)))
	{
		hv_MinHeight = 400;
		hv_MaxHeight = 600;
	}
	else if (0 != ((hv_HeightLimit.TupleLength()) == 1))
	{
		hv_MinHeight = 0;
		hv_MaxHeight = hv_HeightLimit;
	}
	else
	{
		hv_MinHeight = ((const HTuple&)hv_HeightLimit)[0];
		hv_MaxHeight = ((const HTuple&)hv_HeightLimit)[1];
	}
	//
	//Test, if window size has to be changed.
	hv_ResizeFactor = 1;
	GetImageSize(ho_Image, &hv_ImageWidth, &hv_ImageHeight);
	//First, expand window to the minimum extents (if necessary).
	if (0 != (HTuple(hv_MinWidth > hv_ImageWidth).TupleOr(hv_MinHeight > hv_ImageHeight)))
	{
		hv_ResizeFactor = (((hv_MinWidth.TupleReal()) / hv_ImageWidth).TupleConcat((hv_MinHeight.TupleReal()) / hv_ImageHeight)).TupleMax();
	}
	hv_TempWidth = hv_ImageWidth * hv_ResizeFactor;
	hv_TempHeight = hv_ImageHeight * hv_ResizeFactor;
	//Then, shrink window to maximum extents (if necessary).
	if (0 != (HTuple(hv_MaxWidth < hv_TempWidth).TupleOr(hv_MaxHeight < hv_TempHeight)))
	{
		hv_ResizeFactor = hv_ResizeFactor * ((((hv_MaxWidth.TupleReal()) / hv_TempWidth).TupleConcat((hv_MaxHeight.TupleReal()) / hv_TempHeight)).TupleMin());
	}
	hv_WindowWidth = hv_ImageWidth * hv_ResizeFactor;
	hv_WindowHeight = hv_ImageHeight * hv_ResizeFactor;

	//Resize window
	//�������ڲ�����ԭ��������halcon�Ĵ��ڣ�ע�ͺ����Լ��Ŀؼ���ʾ
	//SetWindowAttr("background_color", "black");
	//OpenWindow(hv_Row, hv_Column, hv_WindowWidth, hv_WindowHeight, 0, "visible", "", &(*hv_WindowHandle));
	//HDevWindowStack::Push((*hv_WindowHandle));
	//if (HDevWindowStack::IsOpen())
		//SetPart(HDevWindowStack::GetActive(), 0, 0, hv_ImageHeight - 1, hv_ImageWidth - 1);
	return;
}

void SS_DJ_Measure::AllEdges(HalconCpp::HObject ho_Regionselect, HalconCpp::HObject ho_ImageMedian, HTuple hv_ImWidth, 
	HTuple hv_ImHeight, HTuple* hv_RtCenterCol, HTuple* hv_SmallRtWidth, HTuple* hv_ColEdge1, 
	HTuple* hv_ColEdge2, HTuple* hv_ColEdge3, HTuple* hv_ColEdge4, HTuple* hv_RowAll, HTuple* hv_RtCenterRow, HTuple* hv_RtWidth)
{
	// Local iconic variables
	HalconCpp::HObject  ho_RtRegion, ho_Rectangle;

	// Local control variables
	HTuple  hv_RtArea, hv_Rt_WH, hv_RtHeight, hv_HalfHeight;
	HTuple  hv_SmallRtNum, hv_Each, hv_EachRow, hv_EachCol;
	HTuple  hv_MeasureHandle, hv_GrayValues, hv_Function, hv_SmoothedFunction1;
	HTuple  hv_Derivative1, hv_Min1, hv_Max1, hv_MaskL, hv_MaskR;
	HTuple  hv_pointL, hv_pointL_X, hv_pointL_Y, hv_pointR;
	HTuple  hv_pointR_X, hv_pointR_Y, hv_Index, hv_L_Index;
	HTuple  hv_chamferL, hv_chamferLL, hv_chamferLR, hv_chamferR;
	HTuple  hv_chamferRL, hv_chamferRR;

	//�õ����ε����ĵ�����RtCenterRow, RtCenterCol�����RtWidth���������Ʒָ����Ƽ������͵��Ǽ������
	ShapeTrans(ho_Regionselect, &ho_RtRegion, "rectangle2");
	AreaCenter(ho_RtRegion, &hv_RtArea, &(*hv_RtCenterRow), &(*hv_RtCenterCol));
	//�õ�����Ŀ�͸�
	RegionFeatures(ho_RtRegion, (HTuple("width").Append("height")), &hv_Rt_WH);
	(*hv_RtWidth) = ((const HTuple&)hv_Rt_WH)[0];
	hv_RtHeight = ((const HTuple&)hv_Rt_WH)[1];
	//���Ǻͳ��ȼ��---------------------------
	//�ߴ���С���ΰ��
	hv_HalfHeight = 12;
	//SmallRtNum С���θ���
	//+1ʹС���������¶ԳƵ�������
	hv_SmallRtNum = (hv_RtHeight / (1 + (2 * hv_HalfHeight))).TupleInt();
	//С�������ұ߽�ֱ���10������
	(*hv_SmallRtWidth) = (((*hv_RtWidth) / 2) + 10).TupleInt();
	//��С�����󵹽���߽�
	(*hv_ColEdge1) = HTuple();
	//��С�����󵹽��ұ߽�
	(*hv_ColEdge2) = HTuple();
	//��С�����ҵ�����߽�
	(*hv_ColEdge3) = HTuple();
	//��С�����ҵ����ұ߽�
	(*hv_ColEdge4) = HTuple();
	//��С����������������
	(*hv_RowAll) = HTuple();
	//��ÿ��С���ο��ڵ��ǵı߽�����

	{
		HTuple end_val27 = hv_SmallRtNum - 1;
		HTuple step_val27 = 1;
		for (hv_Each = 0; hv_Each.Continue(end_val27, step_val27); hv_Each += step_val27)
		{

			hv_EachRow = (25 * hv_Each) + hv_HalfHeight;
			hv_EachCol = (*hv_RtCenterCol);
			//������������,���ο��ӻ�
			GenRectangle2(&ho_Rectangle, hv_EachRow, hv_EachCol, 0, (*hv_SmallRtWidth), hv_HalfHeight);
			GenMeasureRectangle2(hv_EachRow, hv_EachCol, 0, (*hv_SmallRtWidth), hv_HalfHeight,
				hv_ImWidth, hv_ImHeight, "bilinear", &hv_MeasureHandle);
			MeasureProjection(ho_ImageMedian, hv_MeasureHandle, &hv_GrayValues);
			//ѡ����͵�������ֵΪ��������***�Ҷ�ֵ��ֵ���Ϊ���������������������ƫ��
			CreateFunct1dArray(hv_GrayValues, &hv_Function);
			SmoothFunct1dGauss(hv_Function, 2, &hv_SmoothedFunction1);
			//���Ǳ�Ե���ȱ仯���磬һ�׵����仯���ԣ�������������ľֲ����ֵ���Ҳ��������С�ľֲ���Сֵ
			DerivateFunct1d(hv_SmoothedFunction1, "first", &hv_Derivative1);
			//��һ�׵����ҵ��Ǳ�Ե������һ�׺����ֲ���С���ĵ㡣Min1, Max1��ʾ�ֲ���Сֵ�����ֵ��λ��
			LocalMinMaxFunct1d(hv_Derivative1, "strict_min_max", "true", &hv_Min1, &hv_Max1);
			//ѡ�񵹽ǵĴ���λ�ã����ٸ���
			//MaskL����������߽����С��100��ֵ��������MaskR���������ұ߽����120����������ұ߽�֮���ֵ������
			hv_MaskL = HTuple::TupleGenSequence(0, (hv_Max1.TupleLength()) - 1, 1).TupleSelectMask(hv_Max1.TupleLessElem(100));
			hv_MaskR = HTuple::TupleGenSequence(0, (hv_Min1.TupleLength()) - 1, 1).TupleSelectMask(hv_Min1.TupleGreaterElem((*hv_RtWidth) - 100));
			//�󵹽����Ե���ܳ��ֵ�λ��
			hv_pointL = HTuple(hv_Max1[hv_MaskL]);
			//�ҳ��󵹽����Ե���ܳ��ֵ�λ����һ�׵����еĵ������
			GetPairFunct1d(hv_Derivative1, hv_pointL.TupleRound(), &hv_pointL_X, &hv_pointL_Y);
			hv_pointR = HTuple(hv_Min1[hv_MaskR]);
			GetPairFunct1d(hv_Derivative1, hv_pointR.TupleRound(), &hv_pointR_X, &hv_pointR_Y);

			//�ҵ��󵹽Ƿ�Χ�ڼ���ֵ��һ�׵�����Y�����ֵ���е���������ֵ��λ��С��Ϊ�󵹽����Ե��λ�ô��Ϊ�󵹽��ұ�Ե
			//��������
			TupleSortIndex(hv_pointL_Y, &hv_Index);
			TupleLength(hv_Index, &hv_L_Index);
			TupleSelect(hv_pointL, HTuple(hv_Index[hv_L_Index - 1]).TupleConcat(HTuple(hv_Index[hv_L_Index - 2])),
				&hv_chamferL);
			TupleMin(hv_chamferL, &hv_chamferLL);
			TupleMax(hv_chamferL, &hv_chamferLR);
			//�ҵ��ҵ��Ƿ�Χ�ڼ�Сֵ��һ�׵�����Y�����ֵ���е���С������ֵ��λ��С��Ϊ�ҵ������Ե��λ�ô��Ϊ�ҵ����ұ�Ե
			TupleSortIndex(hv_pointR_Y, &hv_Index);
			TupleSelect(hv_pointR, HTuple(hv_Index[0]).TupleConcat(HTuple(hv_Index[1])),
				&hv_chamferR);
			TupleMin(hv_chamferR, &hv_chamferRL);
			TupleMax(hv_chamferR, &hv_chamferRR);


			//���󵹽����Ե���󵹽��ұ�Ե���ҵ������Ե���ҵ����ұ�Ե�ֱ�����ĸ�����
			(*hv_ColEdge1)[hv_Each] = hv_chamferLL;
			(*hv_ColEdge2)[hv_Each] = hv_chamferLR;
			(*hv_ColEdge3)[hv_Each] = hv_chamferRL;
			(*hv_ColEdge4)[hv_Each] = hv_chamferRR;
			(*hv_RowAll)[hv_Each] = hv_EachRow;
			//
			CloseMeasure(hv_MeasureHandle);
		}
	}

	//���Ե��λ�ã��������ʱ�����жϣ�
	//dev_open_window_fit_image (ImageMedian, 0, 0, -1, -1, WindowHandle)
	//dev_display (ImageMedian)
	//tuple_gen_const (|ColEdge1|, SmallRtWidth, ROI_width)
	//tuple_gen_const (|ColEdge1|, RtCenterCol, Column_re_T)
	//Col_e1 := Column_re_T-(ROI_width-ColEdge1)
	//Col_e2 := Column_re_T-(ROI_width-ColEdge2)
	//Col_e3 := Column_re_T-(ROI_width-ColEdge3)
	//Col_e4 := Column_re_T-(ROI_width-ColEdge4)
	//dev_set_color ('red')

	//gen_cross_contour_xld (Cross, RowAll, Col_e1, 6, 0.785398)
	//gen_cross_contour_xld (Cross, RowAll, Col_e2, 6, 0.785398)
	//gen_cross_contour_xld (Cross, RowAll, Col_e3, 6, 0.785398)
	//gen_cross_contour_xld (Cross, RowAll, Col_e4, 6, 0.785398)

	return;
}

void SS_DJ_Measure::ChamferPhi(HalconCpp::HObject ho_HeightMedian, HTuple hv_HtHeight, HTuple hv_Width_Cham, HTuple hv_RowAll, HTuple hv_Col_Cham, HTuple hv_HtWidth, HTuple hv_PixelSize, HTuple* hv_ChamPhi)
{
	// Local iconic variables
	HalconCpp::HObject  ho_Rectangle;

	// Local control variables
	HTuple  hv_SmallRt_HalfHeight, hv_SmallRt_Num;
	HTuple  hv_SmallRtWidth, hv_zResolution, hv_Each, hv_EachRow;
	HTuple  hv_EachCol, hv_EachSmallRtWidth, hv_MeasureHandle;
	HTuple  hv_GrayValues, hv_XEnd, hv_TanChamPhi, hv_ChamRadian;
	HTuple  hv_C_Phi, hv_Dist, hv_MeanV, hv_IndexError_M;

	//�ߴ���С���ΰ��
	hv_SmallRt_HalfHeight = 12;
	//SmallRt_Num С���θ���
	//+1ʹС���������¶ԳƵ�������
	hv_SmallRt_Num = (hv_HtHeight / (1 + (2 * hv_SmallRt_HalfHeight))).TupleInt();
	//���ǿ�ȼ�С����һ����
	hv_SmallRtWidth = (hv_Width_Cham / 2) - 3;
	//��С���ε��ǽǶ�
	(*hv_ChamPhi) = HTuple();
	hv_zResolution = 0.0001;


	{
		HTuple end_val12 = (hv_RowAll.TupleLength()) - 1;
		HTuple step_val12 = 1;
		for (hv_Each = 0; hv_Each.Continue(end_val12, step_val12); hv_Each += step_val12)
		{

			hv_EachRow = HTuple(hv_RowAll[hv_Each]);
			hv_EachCol = HTuple(hv_Col_Cham[hv_Each]);
			hv_EachSmallRtWidth = HTuple(hv_SmallRtWidth[hv_Each]);
			//������������,���ο��ӻ�
			GenRectangle2(&ho_Rectangle, hv_EachRow, hv_EachCol, 0, hv_EachSmallRtWidth,
				hv_SmallRt_HalfHeight);
			GenMeasureRectangle2(hv_EachRow, hv_EachCol, 0, hv_EachSmallRtWidth, hv_SmallRt_HalfHeight,
				hv_HtWidth, hv_HtHeight, "bilinear", &hv_MeasureHandle);
			MeasureProjection(ho_HeightMedian, hv_MeasureHandle, &hv_GrayValues);
			hv_XEnd = ((hv_GrayValues.TupleLength()) - 1) * hv_PixelSize;
			hv_TanChamPhi[hv_Each] = hv_XEnd / (((HTuple(hv_GrayValues[(hv_GrayValues.TupleLength()) - 1]) - HTuple(hv_GrayValues[0])).TupleAbs()) * hv_zResolution);
		}
	}
	TupleAtan(hv_TanChamPhi, &hv_ChamRadian);
	hv_C_Phi = (hv_ChamRadian / 3.14) * 180;

	//�Ը�����λ���������õ��ĵ��ǽǶ����������޳��쳣ֵ�����ֵΪ���󵹽ǽǶ�
	hv_Dist = hv_C_Phi;
	MeanValue(hv_Dist, &hv_MeanV, &hv_IndexError_M);
	(*hv_ChamPhi) = hv_MeanV;

	return;
}

void SS_DJ_Measure::cir_select(HalconCpp::HObject ho_Selected_maxDis, HalconCpp::HObject* ho_Selected_cir)
{
	// Local iconic variables

  // Local control variables
	HTuple  hv_Circularity, hv_a, hv_s;

	CircularityXld(ho_Selected_maxDis, &hv_Circularity);
	hv_a = HTuple();
	{
		HTuple end_val2 = (hv_Circularity.TupleLength()) - 1;
		HTuple step_val2 = 1;
		for (hv_s = 0; hv_s.Continue(end_val2, step_val2); hv_s += step_val2)
		{
			if (0 != (HTuple(hv_Circularity[hv_s]) < 0.25))
			{
				hv_a = hv_a.TupleConcat(hv_s + 1);
			}
		}
	}
	SelectObj(ho_Selected_maxDis, &(*ho_Selected_cir), hv_a);
	return;
}

void SS_DJ_Measure::ComputerTh(HalconCpp::HObject ho_RtRoi, HalconCpp::HObject ho_ImageMean, HTuple* hv_nTh)
{
	// Local iconic variables
	HalconCpp::HObject  ho_ImageReduced11;

	// Local control variables
	HTuple  hv_AbsoluteHisto, hv_RelativeHisto, hv_RelativeHistoIndices;
	HTuple  hv_MaxIndex, hv_fLitTh, hv_ii;

	//******���ݻҶ�ֱ��ͼ���������ֵ******
	ReduceDomain(ho_ImageMean, ho_RtRoi, &ho_ImageReduced11);
	GrayHisto(ho_RtRoi, ho_ImageMean, &hv_AbsoluteHisto, &hv_RelativeHisto);
	TupleSortIndex(hv_RelativeHisto, &hv_RelativeHistoIndices);
	hv_MaxIndex = ((const HTuple&)hv_RelativeHistoIndices)[(hv_RelativeHistoIndices.TupleLength()) - 1];
	//
	hv_fLitTh = 1.0 / 256;
	for (hv_ii = hv_MaxIndex; hv_ii >= 0; hv_ii += -1)
	{
		if (0 != (HTuple(hv_RelativeHisto[hv_ii]) < hv_fLitTh))
		{
			(*hv_nTh) = hv_ii;
			break;
		}
	}
	//
	//
	return;
}

void SS_DJ_Measure::dside(HalconCpp::HObject ho_Selected_cir, HalconCpp::HObject* ho_result_crack, HTuple hv_width_crack, HTuple hv_n, HTuple hv_dis_crack)
{
	// Local iconic variables
	HalconCpp::HObject  ho_Selected_k;

	// Local control variables
	HTuple  hv_kk, hv_ww, hv_conNumber, hv_k, hv_Row_k;
	HTuple  hv_Col_k, hv_ju1, hv_pixel_number, hv_Sorted1, hv_Indices;
	HTuple  hv_indices, hv_scale1, hv_ju2, hv_Sorted2, hv_scale2;

	hv_kk = HTuple();
	hv_ww = hv_width_crack;
	CountObj(ho_Selected_cir, &hv_conNumber);
	{
		HTuple end_val3 = hv_conNumber;
		HTuple step_val3 = 1;
		for (hv_k = 1; hv_k.Continue(end_val3, step_val3); hv_k += step_val3)
		{
			SelectObj(ho_Selected_cir, &ho_Selected_k, hv_k);
			GetContourXld(ho_Selected_k, &hv_Row_k, &hv_Col_k);
			hv_ju1 = hv_ww - hv_Col_k;
			hv_pixel_number = hv_ju1.TupleLength();
			TupleConcat(hv_ju1, hv_dis_crack, &hv_ju1);
			TupleSort(hv_ju1, &hv_Sorted1);
			TupleFind(hv_Sorted1, hv_dis_crack, &hv_Indices);
			//
			if (0 != ((hv_Indices.TupleLength()) > 1))
			{
				hv_indices = ((const HTuple&)hv_Indices)[0];
			}
			else
			{
				hv_indices = hv_Indices;
			}
			//
			hv_scale1 = (hv_indices + 0.1) / (hv_pixel_number + 0.1);
			//
			hv_ju2 = hv_Col_k - 0;
			TupleConcat(hv_ju2, hv_dis_crack, &hv_ju2);
			TupleSort(hv_ju2, &hv_Sorted2);
			TupleFind(hv_Sorted2, hv_dis_crack, &hv_Indices);
			//
			if (0 != ((hv_Indices.TupleLength()) > 1))
			{
				hv_indices = ((const HTuple&)hv_Indices)[0];
			}
			else
			{
				hv_indices = hv_Indices;
			}
			//
			hv_scale2 = (hv_indices + 0.1) / (hv_pixel_number + 0.1);
			//
			if (0 != (HTuple(hv_scale1 < 0.4).TupleAnd(hv_scale2 < 0.4)))
			{
				hv_kk = hv_kk.TupleConcat(hv_k);
			}
		}
	}
	SelectObj(ho_Selected_cir, &(*ho_result_crack), hv_kk);
	return;
}

void SS_DJ_Measure::find_crack_region(HalconCpp::HObject ho_Image_crack_show, HalconCpp::HObject* ho_re_crack, HTuple hv_height_crack, HTuple hv_width_crack, HTuple hv_Mean_length, HTuple* hv_kk)
{
	// Local iconic variables
	HalconCpp::HObject  ho_ImageDeviation, ho_ImageDeviationMean;
	HalconCpp::HObject  ho_LightRegionDynThresh, ho_ConnectedRegions1, ho_SelectedRegions;
	HalconCpp::HObject  ho_EmptyRegion, ho_re_crack_1, ho_Rectangle1, ho_Rectangle0;
	HalconCpp::HObject  ho_Rectangle2, ho_Rectangle3, ho_ObjectsConcat;
	HalconCpp::HObject  ho_RegionUnion, ho_ImageReduced, ho_Region_empty;
	HalconCpp::HObject  ho_BinImage_empty, ho_Empty_region_iamge, ho_Rectangle;
	HalconCpp::HObject  ho_RegionFillUp, ho_BinImage_edge_defect, ho_BinImage_all;
	HalconCpp::HObject  ho_Image_add, ho_Rectangle_enter, ho_re_Concat;
	HalconCpp::HObject  ho_Select_ci1, ho_Select_ci2, ho_re_Concat_1;

	// Local control variables
	HTuple  hv_renum_1, hv_kk1, hv_i, hv_r_1, hv_r_2;
	HTuple  hv_c_1, hv_c_2, hv_w_i, hv_h_i, hv_min_value, hv_w_bin;
	HTuple  hv_h_bin, hv_k_up, hv_nk, hv_Row_n1, hv_Row_n2;
	HTuple  hv_Column_n, hv_MeasureHandle, hv_GrayValues1, hv_Sum1;
	HTuple  hv_num_Concat, hv_re, hv_e_in, hv_ci, hv_Value1;
	HTuple  hv_Value2, hv_MinDistance, hv_Row11, hv_Column11;
	HTuple  hv_Row21, hv_Column21, hv_renum, hv_area_i, hv_W_i;

	//* ������δ����ڻҶ�ֵ�ı�׼ƫ�  ��ֵƽ��
	DeviationImage(ho_Image_crack_show, &ho_ImageDeviation, 15, 15);
	MeanImage(ho_ImageDeviation, &ho_ImageDeviationMean, 25, 25);
	DynThreshold(ho_ImageDeviation, ho_ImageDeviationMean, &ho_LightRegionDynThresh,
		5, "light");
	Connection(ho_LightRegionDynThresh, &ho_ConnectedRegions1);
	SelectShape(ho_ConnectedRegions1, &ho_SelectedRegions, "area", "and", 1500, 99999);
	ClosingCircle(ho_SelectedRegions, &ho_SelectedRegions, 3);
	//**�ж�������������
	CountObj(ho_SelectedRegions, &hv_renum_1);
	GenEmptyRegion(&ho_EmptyRegion);
	if (0 != (ho_SelectedRegions == ho_EmptyRegion))
	{
		hv_renum_1 = 0;
		//    break()
	}
	//**����С���ο�ȣ�ȥ��С����
	hv_kk1 = HTuple();
	{
		HTuple end_val16 = hv_renum_1;
		HTuple step_val16 = 1;
		for (hv_i = 1; hv_i.Continue(end_val16, step_val16); hv_i += step_val16)
		{
			SelectObj(ho_SelectedRegions, &ho_re_crack_1, hv_i);
			RegionFeatures(ho_re_crack_1, "row1", &hv_r_1);
			RegionFeatures(ho_re_crack_1, "row2", &hv_r_2);
			RegionFeatures(ho_re_crack_1, "column1", &hv_c_1);
			RegionFeatures(ho_re_crack_1, "column2", &hv_c_2);
			hv_w_i = hv_r_2 - hv_r_1;
			hv_h_i = hv_c_2 - hv_c_1;
			hv_min_value = 0;
			if (0 != (hv_w_i > hv_h_i))
			{
				hv_min_value = hv_h_i;
			}
			else
			{
				hv_min_value = hv_w_i;
			}
			//
			if (0 != (HTuple(hv_min_value > 90).TupleOr(hv_min_value < 60)))
			{
				hv_kk1 = hv_kk1.TupleConcat(hv_i);
			}
		}
	}
	SelectObj(ho_SelectedRegions, &ho_SelectedRegions, hv_kk1);
	//*
	GenRectangle1(&ho_Rectangle1, 0, 0, hv_height_crack, 10);
	GenRectangle1(&ho_Rectangle0, 0, hv_width_crack - 10, hv_height_crack, hv_width_crack);
	GenRectangle1(&ho_Rectangle2, 0, 20, 10, hv_width_crack - 20);
	GenRectangle1(&ho_Rectangle3, hv_height_crack - 10, 20, hv_height_crack, hv_width_crack - 20);
	ConcatObj(ho_SelectedRegions, ho_Rectangle0, &ho_ObjectsConcat);
	ConcatObj(ho_ObjectsConcat, ho_Rectangle1, &ho_ObjectsConcat);
	ConcatObj(ho_ObjectsConcat, ho_Rectangle2, &ho_ObjectsConcat);
	ConcatObj(ho_ObjectsConcat, ho_Rectangle3, &ho_ObjectsConcat);
	//
	Union1(ho_ObjectsConcat, &ho_RegionUnion);
	//
	ReduceDomain(ho_Image_crack_show, ho_RegionUnion, &ho_ImageReduced);
	Threshold(ho_ImageReduced, &ho_Region_empty, 0, 255);
	RegionToBin(ho_Region_empty, &ho_BinImage_empty, 255, 0, hv_width_crack, hv_height_crack);
	//
	//
	GenEmptyRegion(&ho_Empty_region_iamge);
	hv_w_bin = hv_width_crack.TupleRound();
	hv_h_bin = hv_height_crack.TupleRound();
	//
	hv_k_up = HTuple();
	{
		HTuple end_val58 = hv_h_bin / 40;
		HTuple step_val58 = 1;
		for (hv_nk = 0; hv_nk.Continue(end_val58, step_val58); hv_nk += step_val58)
		{
			hv_Row_n1 = (41 * hv_nk) + 20;
			hv_Row_n2 = (hv_h_bin - (41 * hv_nk)) - 20;
			hv_Column_n = hv_w_bin / 2;
			//������������
			GenRectangle2(&ho_Rectangle, hv_Row_n1, hv_Column_n, 0, hv_Column_n, 20);
			GenMeasureRectangle2(hv_Row_n1, hv_Column_n, 0, hv_Column_n, 20, hv_w_bin, hv_h_bin,
				"bilinear", &hv_MeasureHandle);
			MeasureProjection(ho_BinImage_empty, hv_MeasureHandle, &hv_GrayValues1);
			TupleSum(hv_GrayValues1, &hv_Sum1);
			if (0 != (hv_Sum1 != 5100))
			{
				ConcatObj(ho_Empty_region_iamge, ho_Rectangle, &ho_Empty_region_iamge);
			}
			CloseMeasure(hv_MeasureHandle);
		}
	}
	Union1(ho_Empty_region_iamge, &ho_Empty_region_iamge);
	ReduceDomain(ho_BinImage_empty, ho_Empty_region_iamge, &ho_ImageReduced);
	Threshold(ho_ImageReduced, &ho_Region_empty, 10, 255);
	FillUp(ho_Region_empty, &ho_RegionFillUp);
	RegionToBin(ho_RegionUnion, &ho_BinImage_edge_defect, 255, 0, hv_width_crack, hv_height_crack);
	RegionToBin(ho_RegionFillUp, &ho_BinImage_all, 255, 0, hv_width_crack, hv_height_crack);
	AddImage(ho_BinImage_all, ho_BinImage_edge_defect, &ho_Image_add, 0.5, 0);
	GenRectangle1(&ho_Rectangle_enter, 12, 12, hv_height_crack - 12, hv_width_crack - 12);
	ReduceDomain(ho_Image_add, ho_Rectangle_enter, &ho_Image_add);
	Threshold(ho_Image_add, &ho_re_Concat, 128, 150);
	//*   ������
	ClosingCircle(ho_re_Concat, &ho_re_Concat, 8);
	Connection(ho_re_Concat, &ho_re_Concat);
	SelectShape(ho_re_Concat, &ho_re_Concat, "area", "and", 150, 99999);
	ClosingCircle(ho_re_Concat, &ho_re_Concat, 15);
	//*ȥ�������� ��һ��ȥ�������� ������������ֵ������
	CountObj(ho_re_Concat, &hv_num_Concat);
	TupleGenSequence(1, hv_num_Concat, 1, &hv_re);
	hv_e_in = HTuple();
	{
		HTuple end_val91 = hv_num_Concat - 1;
		HTuple step_val91 = 1;
		for (hv_ci = 1; hv_ci.Continue(end_val91, step_val91); hv_ci += step_val91)
		{
			SelectObj(ho_re_Concat, &ho_Select_ci1, hv_ci);
			SelectObj(ho_re_Concat, &ho_Select_ci2, hv_ci + 1);
			RegionFeatures(ho_Select_ci1, "max_diameter", &hv_Value1);
			RegionFeatures(ho_Select_ci2, "max_diameter", &hv_Value2);
			DistanceRrMin(ho_Select_ci1, ho_Select_ci2, &hv_MinDistance, &hv_Row11, &hv_Column11,
				&hv_Row21, &hv_Column21);
			if (0 != (HTuple(HTuple(hv_MinDistance < 15).TupleAnd(hv_Value1 > (hv_width_crack - 30))).TupleAnd(hv_Value2 > (hv_width_crack - 30))))
			{
				hv_e_in = hv_e_in.TupleConcat(hv_ci - 1);
				hv_e_in = hv_e_in.TupleConcat(hv_ci);
			}
		}
	}
	TupleRemove(hv_re, hv_e_in, &hv_re);
	SelectObj(ho_re_Concat, &ho_re_Concat_1, hv_re);
	//
	CountObj(ho_re_Concat_1, &hv_renum);
	GenEmptyRegion(&ho_EmptyRegion);
	if (0 != (ho_re_Concat_1 == ho_EmptyRegion))
	{
		hv_renum = 0;
	}
	//*�ڶ���ȥ��������
	(*hv_kk) = HTuple();
	{
		HTuple end_val112 = hv_renum;
		HTuple step_val112 = 1;
		for (hv_i = 1; hv_i.Continue(end_val112, step_val112); hv_i += step_val112)
		{
			SelectObj(ho_re_Concat_1, &(*ho_re_crack), hv_i);
			RegionFeatures((*ho_re_crack), "area", &hv_area_i);
			RegionFeatures((*ho_re_crack), "row1", &hv_r_1);
			RegionFeatures((*ho_re_crack), "row2", &hv_r_2);
			RegionFeatures((*ho_re_crack), "column1", &hv_c_1);
			RegionFeatures((*ho_re_crack), "column2", &hv_c_2);
			hv_w_i = hv_r_2 - hv_r_1;
			hv_h_i = hv_c_2 - hv_c_1;
			hv_W_i = hv_area_i / (hv_w_i + hv_h_i);
			//
			if (0 != (hv_W_i < 15))
			{
				(*hv_kk) = (*hv_kk).TupleConcat(hv_i);
			}
		}
	}
	SelectObj(ho_re_Concat_1, &(*ho_re_crack), (*hv_kk));
	return;
}

void SS_DJ_Measure::find_edges(HalconCpp::HObject ho_ImageMedian, HTuple hv_S_rec_number, HTuple hv_rec_Column, HTuple hv_S_rec_length, HTuple hv_S_rec_height, HTuple hv_Im_Width, HTuple hv_Im_Height, HTuple* hv_Col_edge1, HTuple* hv_Col_edge2, HTuple* hv_Col_edge3, HTuple* hv_Col_edge4, HTuple* hv_re_Row)
{
	// Local iconic variables
	HalconCpp::HObject  ho_Rectangle;

	// Local control variables
	HTuple  hv_n, hv_Row_n, hv_Column_n, hv_MeasureHandle;
	HTuple  hv_GrayValues, hv_Function, hv_SmoothedFunction1;
	HTuple  hv_Derivative1, hv_Min, hv_Max, hv_point_R, hv_point_RX;
	HTuple  hv_point_RY, hv_point_L, hv_point_LX, hv_point_LY;
	HTuple  hv_Indices, hv_last_point, hv_last_Max_n, hv_last_Min_n;
	HTuple  hv_L_Indices, hv_begin_point, hv_begin_Max_n, hv_begin_Min_n;

	{
		HTuple end_val0 = hv_S_rec_number;
		HTuple step_val0 = 1;
		for (hv_n = 0; hv_n.Continue(end_val0, step_val0); hv_n += step_val0)
		{
			//ÿ61�����ص����о����
			//�д�0��ʼ������������
			hv_Row_n = (31 * hv_n) + 15;
			hv_Column_n = hv_rec_Column;
			//������������,���ο��ӻ�
			GenRectangle2(&ho_Rectangle, hv_Row_n, hv_Column_n, 0, hv_S_rec_length, hv_S_rec_height);
			GenMeasureRectangle2(hv_Row_n, hv_Column_n, 0, hv_S_rec_length, hv_S_rec_height,
				hv_Im_Width, hv_Im_Height, "bilinear", &hv_MeasureHandle);
			MeasureProjection(ho_ImageMedian, hv_MeasureHandle, &hv_GrayValues);
			//****ѡ����͵�������ֵΪ��������***�Ҷ�ֵ��ֵ���Ϊ���������������������ƫ��
			CreateFunct1dArray(hv_GrayValues, &hv_Function);
			SmoothFunct1dGauss(hv_Function, 4, &hv_SmoothedFunction1);
			DerivateFunct1d(hv_SmoothedFunction1, "first", &hv_Derivative1);
			LocalMinMaxFunct1d(hv_Derivative1, "strict_min_max", "true", &hv_Min, &hv_Max);

			if (0 != (HTuple((hv_Min.TupleLength()) < 4).TupleOr((hv_Max.TupleLength()) < 4)))
			{
				TupleLastN(hv_Min, (hv_Min.TupleLength()) - 2, &hv_point_R);
				GetPairFunct1d(hv_Derivative1, hv_point_R.TupleRound(), &hv_point_RX, &hv_point_RY);

				TupleSelect(hv_Max, (HTuple(0).Append(1)), &hv_point_L);
				GetPairFunct1d(hv_Derivative1, hv_point_L.TupleRound(), &hv_point_LX, &hv_point_LY);
			}
			else
			{
				TupleLastN(hv_Min, (hv_Min.TupleLength()) - 4, &hv_point_R);
				GetPairFunct1d(hv_Derivative1, hv_point_R.TupleRound(), &hv_point_RX, &hv_point_RY);

				TupleSelect(hv_Max, (((HTuple(0).Append(1)).Append(2)).Append(3)), &hv_point_L);
				GetPairFunct1d(hv_Derivative1, hv_point_L.TupleRound(), &hv_point_LX, &hv_point_LY);
			}
			//�����С��ת��Ϊ���������
			//tuple_last_n (Min, |Min|-4, point_R)
			//get_pair_funct_1d (Derivative1, round(point_R), point_RX, point_RY)
			//�ҳ�  ��  ��������λ�ã��ҳ�����7��Ԫ������С����
			TupleSortIndex(hv_point_RY, &hv_Indices);
			TupleSelect(hv_point_R, HTuple(hv_Indices[0]).TupleConcat(HTuple(hv_Indices[1])),
				&hv_last_point);
			//---------------------------------------
			TupleMax(hv_last_point, &hv_last_Max_n);
			TupleMin(hv_last_point, &hv_last_Min_n);
			//������7���Ҷȱ仯���ҵ�λ��
			//tuple_select (Max, [0,1,2,3], point_L)
			//get_pair_funct_1d (Derivative1, round(point_L), point_LX, point_LY)
			//�ҳ�  ǰ  ��������λ�ã��ҳ�����7��Ԫ�����������
			TupleSortIndex(hv_point_LY, &hv_Indices);
			TupleLength(hv_Indices, &hv_L_Indices);
			TupleSelect(hv_point_L, HTuple(hv_Indices[hv_L_Indices - 1]).TupleConcat(HTuple(hv_Indices[hv_L_Indices - 2])),
				&hv_begin_point);
			//---------------------------------------
			TupleMax(hv_begin_point, &hv_begin_Max_n);
			TupleMin(hv_begin_point, &hv_begin_Min_n);
			//****
			(*hv_Col_edge1)[hv_n] = hv_begin_Min_n;
			(*hv_Col_edge2)[hv_n] = hv_begin_Max_n;
			(*hv_Col_edge3)[hv_n] = hv_last_Min_n;
			(*hv_Col_edge4)[hv_n] = hv_last_Max_n;
			(*hv_re_Row)[hv_n] = hv_Row_n;
			//
			CloseMeasure(hv_MeasureHandle);
		}
	}
	return;
}

void SS_DJ_Measure::find_edges1(HalconCpp::HObject ho_ImageMedian, HalconCpp::HObject* ho_Rectangle, HTuple hv_S_rec_number, HTuple hv_rec_center_Row, HTuple hv_S_rec_length, HTuple hv_S_rec_height, HTuple hv_Im_Width, HTuple hv_Im_Height, HTuple* hv_Col_edge1, HTuple* hv_Col_edge2, HTuple* hv_Col_edge3, HTuple* hv_Col_edge4, HTuple* hv_re_Row)
{
	// Local iconic variables

  // Local control variables
	HTuple  hv_n, hv_n_Row, hv_n_Column, hv_MeasureHandle;
	HTuple  hv_GrayValues, hv_Function, hv_SmoothedFunction1;
	HTuple  hv_Derivative1, hv_Min, hv_Max, hv_point_R, hv_point_RX;
	HTuple  hv_point_RY, hv_point_L, hv_point_LX, hv_point_LY;
	HTuple  hv_Index, hv_last_point, hv_last_Max_n, hv_last_Min_n;
	HTuple  hv_L_Index, hv_begin_point, hv_begin_Max_n, hv_begin_Min_n;

	{
		HTuple end_val0 = hv_S_rec_number;
		HTuple step_val0 = 1;
		for (hv_n = 0; hv_n.Continue(end_val0, step_val0); hv_n += step_val0)
		{
			//ÿ61�����ص����о����
			//�д�0��ʼ������������
			hv_n_Row = (31 * hv_n) + hv_S_rec_height;
			hv_n_Column = hv_rec_center_Row;
			//������������,���ο��ӻ�
			GenRectangle2(&(*ho_Rectangle), hv_n_Row, hv_n_Column, 0, hv_S_rec_length, hv_S_rec_height);
			GenMeasureRectangle2(hv_n_Row, hv_n_Column, 0, hv_S_rec_length, hv_S_rec_height,
				hv_Im_Width, hv_Im_Height, "bilinear", &hv_MeasureHandle);
			MeasureProjection(ho_ImageMedian, hv_MeasureHandle, &hv_GrayValues);
			//****ѡ����͵�������ֵΪ��������***�Ҷ�ֵ��ֵ���Ϊ���������������������ƫ��
			CreateFunct1dArray(hv_GrayValues, &hv_Function);
			SmoothFunct1dGauss(hv_Function, 4, &hv_SmoothedFunction1);
			DerivateFunct1d(hv_SmoothedFunction1, "first", &hv_Derivative1);
			//���㺯���ֲ���С���ĵ�
			LocalMinMaxFunct1d(hv_Derivative1, "strict_min_max", "true", &hv_Min, &hv_Max);
			//
			if (0 != (HTuple((hv_Min.TupleLength()) < 4).TupleOr((hv_Max.TupleLength()) < 4)))
			{
				TupleLastN(hv_Min, (hv_Min.TupleLength()) - 2, &hv_point_R);
				GetPairFunct1d(hv_Derivative1, hv_point_R.TupleRound(), &hv_point_RX, &hv_point_RY);
				//
				TupleSelect(hv_Max, (HTuple(0).Append(1)), &hv_point_L);
				GetPairFunct1d(hv_Derivative1, hv_point_L.TupleRound(), &hv_point_LX, &hv_point_LY);
			}
			else
			{
				TupleLastN(hv_Min, (hv_Min.TupleLength()) - 4, &hv_point_R);
				GetPairFunct1d(hv_Derivative1, hv_point_R.TupleRound(), &hv_point_RX, &hv_point_RY);
				//
				TupleSelect(hv_Max, (((HTuple(0).Append(1)).Append(2)).Append(3)), &hv_point_L);
				GetPairFunct1d(hv_Derivative1, hv_point_L.TupleRound(), &hv_point_LX, &hv_point_LY);
			}
			//�����С��ת��Ϊ���������
			//tuple_last_n (Min, |Min|-4, point_R)
			//get_pair_funct_1d (Derivative1, round(point_R), point_RX, point_RY)
			//�ҳ�  ��  ��������λ�ã��ҳ�����7��Ԫ������С����
			TupleSortIndex(hv_point_RY, &hv_Index);
			TupleSelect(hv_point_R, HTuple(hv_Index[0]).TupleConcat(HTuple(hv_Index[1])),
				&hv_last_point);
			//---------------------------------------
			TupleMax(hv_last_point, &hv_last_Max_n);
			TupleMin(hv_last_point, &hv_last_Min_n);
			//������7���Ҷȱ仯���ҵ�λ��
			//tuple_select (Max, [0,1,2,3], point_L)
			//get_pair_funct_1d (Derivative1, round(point_L), point_LX, point_LY)
			//�ҳ�  ǰ  ��������λ�ã��ҳ�����7��Ԫ�����������
			TupleSortIndex(hv_point_LY, &hv_Index);
			TupleLength(hv_Index, &hv_L_Index);
			TupleSelect(hv_point_L, HTuple(hv_Index[hv_L_Index - 1]).TupleConcat(HTuple(hv_Index[hv_L_Index - 2])),
				&hv_begin_point);
			//---------------------------------------
			TupleMax(hv_begin_point, &hv_begin_Max_n);
			TupleMin(hv_begin_point, &hv_begin_Min_n);
			//****
			(*hv_Col_edge1)[hv_n] = hv_begin_Min_n;
			(*hv_Col_edge2)[hv_n] = hv_begin_Max_n;
			(*hv_Col_edge3)[hv_n] = hv_last_Min_n;
			(*hv_Col_edge4)[hv_n] = hv_last_Max_n;
			(*hv_re_Row)[hv_n] = hv_n_Row;
			//
			CloseMeasure(hv_MeasureHandle);
		}
	}
	return;
}

void SS_DJ_Measure::find_Length(HTuple hv_Distance, HTuple* hv_Mean_length)
{
	// Local iconic variables

  // Local control variables
	HTuple  hv_mode_Distance, hv_Index1, hv_n1, hv_Index1_n;
	HTuple  hv_Distance_select;

	find_mode(hv_Distance, &hv_mode_Distance);
	hv_Index1 = HTuple();
	hv_n1 = 0;
	{
		HTuple end_val3 = (hv_Distance.TupleLength()) - 1;
		HTuple step_val3 = 1;
		for (hv_Index1_n = 0; hv_Index1_n.Continue(end_val3, step_val3); hv_Index1_n += step_val3)
		{
			if (0 != (((HTuple(hv_Distance[hv_Index1_n]) - hv_mode_Distance).TupleAbs()) > 4))
			{
				hv_Index1[hv_n1] = hv_Index1_n;
				hv_n1 += 1;
			}
		}
	}
	TupleRemove(hv_Distance, hv_Index1, &hv_Distance_select);
	TupleMean(hv_Distance_select, &(*hv_Mean_length));
	return;
}

void SS_DJ_Measure::find_Length1(HTuple hv_Dist, HTuple* hv_Mean_length)
{
	// Local iconic variables

 // Local control variables
	HTuple  hv_re_num, hv_uniq_Dist1, hv_sort_Dist;
	HTuple  hv_round_Dist, hv_type_num, hv_round_num, hv_w;
	HTuple  hv_index_w, hv_max_Index, hv_mode_Dist, hv_index1;
	HTuple  hv_select_Dist;

	find_mode2(hv_Dist, &hv_re_num, &hv_uniq_Dist1, &hv_sort_Dist, &hv_round_Dist,
		&hv_type_num, &hv_round_num, &hv_w, &hv_index_w);
	//�ҳ����Ƶ�ζ�Ӧ��������������ֵ
	TupleFind(hv_re_num, hv_re_num.TupleMax(), &hv_max_Index);
	hv_mode_Dist = HTuple(hv_uniq_Dist1[HTuple(hv_max_Index[0])]);
	//�ҳ�������������4������      0705  �¸ĵ�  ��һ
	hv_index1 = HTuple::TupleGenSequence(0, (hv_Dist.TupleLength()) - 1, 1).TupleSelectMask((hv_Dist.TupleGreaterElem(hv_mode_Dist + 4)).TupleOr(hv_Dist.TupleLessElem(hv_mode_Dist - 4)));
	TupleRemove(hv_Dist, hv_index1, &hv_select_Dist);
	TupleMean(hv_select_Dist, &(*hv_Mean_length));
	return;
}

void SS_DJ_Measure::find_mode(HTuple hv_Distance, HTuple* hv_mode_Distance)
{
	// Local iconic variables

  // Local control variables
	HTuple  hv_sortDistance, hv_Distance_round, hv_uniqDistance;
	HTuple  hv_edges_w_num, hv_re_num, hv_round_num, hv_w, hv_index_w;
	HTuple  hv_Indice_max;

	TupleSort(hv_Distance, &hv_sortDistance);
	TupleRound(hv_sortDistance, &hv_Distance_round);
	TupleUniq(hv_Distance_round, &hv_uniqDistance);
	//
	hv_edges_w_num = hv_uniqDistance.TupleLength();
	TupleGenConst(hv_edges_w_num, 0, &hv_re_num);
	hv_round_num = hv_Distance_round.TupleLength();

	{
		HTuple end_val8 = hv_edges_w_num - 1;
		HTuple step_val8 = 1;
		for (hv_w = 0; hv_w.Continue(end_val8, step_val8); hv_w += step_val8)
		{
			TupleFind(hv_Distance_round, HTuple(hv_uniqDistance[hv_w]), &hv_index_w);
			hv_re_num[hv_w] = hv_index_w.TupleLength();
		}
	}

	//
	TupleFind(hv_re_num, hv_re_num.TupleMax(), &hv_Indice_max);
	(*hv_mode_Distance) = HTuple(hv_uniqDistance[HTuple(hv_Indice_max[0])]);
	return;
}

void SS_DJ_Measure::find_mode2(HTuple hv_Dist, HTuple* hv_re_num, HTuple* hv_uniq_Dist, HTuple* hv_sort_Dist, HTuple* hv_round_Dist, HTuple* hv_type_num, HTuple* hv_round_num, HTuple* hv_w, HTuple* hv_index_w)
{
	// Local iconic variables

	TupleSort(hv_Dist, &(*hv_sort_Dist));
	TupleRound((*hv_sort_Dist), &(*hv_round_Dist));
	TupleUniq((*hv_round_Dist), &(*hv_uniq_Dist));
	//
	(*hv_type_num) = (*hv_uniq_Dist).TupleLength();
	TupleGenConst((*hv_type_num), 0, &(*hv_re_num));
	(*hv_round_num) = (*hv_round_Dist).TupleLength();
	//**����ѭ���Ҷ�Ӧ������������������ֵ����Ƶ��
	{
		HTuple end_val8 = (*hv_type_num) - 1;
		HTuple step_val8 = 1;
		for ((*hv_w) = 0; (*hv_w).Continue(end_val8, step_val8); (*hv_w) += step_val8)
		{
			TupleFind((*hv_round_Dist), HTuple((*hv_uniq_Dist)[(*hv_w)]), &(*hv_index_w));
			(*hv_re_num)[(*hv_w)] = (*hv_index_w).TupleLength();
		}
	}
	return;
}

void SS_DJ_Measure::find_tm(HTuple hv_Distance, HTuple* hv_mode_Distance)
{
	// Local iconic variables

  // Local control variables
	HTuple  hv_sortDistance, hv_Distance_round, hv_uniqDistance;
	HTuple  hv_edges_w_num, hv_re_num, hv_round_num, hv_w, hv_index_w;
	HTuple  hv_Indice_max, hv_mode_Distance1_f, hv_mode_Distance1;
	HTuple  hv_Indices_down, hv_Indices_up, hv_mode_Distance1_num;
	HTuple  hv_mode_Distance2_num, hv_mode_Distance2, hv_re_num_new;
	HTuple  hv_uniqDistance_new, hv_Reduced_re_num, hv_Reduced_uniq_Distance;
	HTuple  hv_Indice_max_2, hv_mode_Distance2_f;

	TupleSort(hv_Distance, &hv_sortDistance);
	TupleRound(hv_sortDistance, &hv_Distance_round);
	TupleUniq(hv_Distance_round, &hv_uniqDistance);
	//
	hv_edges_w_num = hv_uniqDistance.TupleLength();
	TupleGenConst(hv_edges_w_num, 0, &hv_re_num);
	hv_round_num = hv_Distance_round.TupleLength();
	//*****�������Ҷ�Ӧ������
	{
		HTuple end_val8 = hv_edges_w_num - 1;
		HTuple step_val8 = 1;
		for (hv_w = 0; hv_w.Continue(end_val8, step_val8); hv_w += step_val8)
		{
			TupleFind(hv_Distance_round, HTuple(hv_uniqDistance[hv_w]), &hv_index_w);
			hv_re_num[hv_w] = hv_index_w.TupleLength();
		}
	}
	//

	TupleFind(hv_re_num, hv_re_num.TupleMax(), &hv_Indice_max);
	hv_mode_Distance1_f = hv_re_num.TupleMax();
	hv_mode_Distance1 = HTuple(hv_uniqDistance[HTuple(hv_Indice_max[0])]);
	//*****
	TupleFind(hv_sortDistance, hv_mode_Distance1 - 1, &hv_Indices_down);
	TupleFind(hv_sortDistance, hv_mode_Distance1 + 1, &hv_Indices_up);
	//
	hv_mode_Distance1_num = (hv_mode_Distance1_f + (hv_Indices_down.TupleLength())) + (hv_Indices_up.TupleLength());
	//

	if (0 != (hv_mode_Distance1_num > 35))
	{

		hv_mode_Distance2_num = 0;
		hv_mode_Distance2 = 0;

	}
	else
	{
		//*****�Ѹ�34   max(re_num_new)���� Indice_max
		hv_re_num_new = hv_re_num;
		hv_uniqDistance_new = hv_uniqDistance;
		TupleRemove(hv_re_num_new, hv_Indice_max, &hv_Reduced_re_num);
		TupleRemove(hv_uniqDistance_new, hv_Indice_max, &hv_Reduced_uniq_Distance);
		TupleFind(hv_Reduced_re_num, hv_Reduced_re_num.TupleMax(), &hv_Indice_max_2);

		hv_mode_Distance2_f = HTuple(hv_Reduced_re_num[hv_Indice_max_2]);
		hv_mode_Distance2 = HTuple(hv_Reduced_uniq_Distance[hv_Indice_max_2]);
		//��ԭ���ң����ǰ� �޳������֮���������
		TupleFind(hv_sortDistance, hv_mode_Distance2 - 1, &hv_Indices_down);
		TupleFind(hv_sortDistance, hv_mode_Distance2 + 1, &hv_Indices_up);
		//
		hv_mode_Distance2_num = (hv_mode_Distance2_f + (hv_Indices_down.TupleLength())) + (hv_Indices_up.TupleLength());

	}

	//
	(*hv_mode_Distance) = 0;
	//
	if (0 != (HTuple(hv_mode_Distance2_num > 15).TupleAnd(hv_mode_Distance1_num > 15)))
	{
		//
		if (0 != (hv_mode_Distance2 > hv_mode_Distance1))
		{
			(*hv_mode_Distance) = hv_mode_Distance2 * 2;
		}
		else
		{
			(*hv_mode_Distance) = hv_mode_Distance1 * 2;
		}
		//
	}
	else
	{
		//
		if (0 != (hv_mode_Distance2_num > hv_mode_Distance1_num))
		{
			(*hv_mode_Distance) = hv_mode_Distance2 * 2;
		}
		else
		{
			(*hv_mode_Distance) = hv_mode_Distance1 * 2;
		}
		//
	}
	return;
}

void SS_DJ_Measure::find_tm1(HTuple hv_Dist, HTuple* hv_mode_Dist)
{
	// Local iconic variables

  // Local control variables
	HTuple  hv_DistOut, hv_re_num, hv_uniq_Dist, hv_sort_Dist;
	HTuple  hv_round_Dist, hv_type_num, hv_round_num, hv_w;
	HTuple  hv_index_w, hv_f_mode1, hv_max_Index, hv_mode_Dist1;
	HTuple  hv_Index_down, hv_Index_up, hv_num_mode1, hv_num_mode2;
	HTuple  hv_mode_Dist2, hv_re_num_new, hv_uniqDist_new, hv_Reduced_re_num;
	HTuple  hv_Reduced_uniq_Dist, hv_f_mode2, hv_max_Index_2;

	hv_DistOut = hv_Dist;
	//����ֵ��С����֮һ����С����������
	hv_DistOut = hv_DistOut / 2.0;
	//**  �Ż�0705
	find_mode2(hv_DistOut, &hv_re_num, &hv_uniq_Dist, &hv_sort_Dist, &hv_round_Dist,
		&hv_type_num, &hv_round_num, &hv_w, &hv_index_w);
	//�����Ƶ��
	hv_f_mode1 = hv_re_num.TupleMax();
	//���Ƶ�ζ�Ӧ������
	TupleFind(hv_re_num, hv_f_mode1, &hv_max_Index);
	//���Ƶ�ζ�Ӧ����ֵ
	hv_mode_Dist1 = HTuple(hv_uniq_Dist[HTuple(hv_max_Index[0])]);
	//�ֱ��ҳ����Ƶ������Ӧ��ֵ�Ӽ�1��������ֵ��Ӧ������
	TupleFind(hv_round_Dist, hv_mode_Dist1 - 1, &hv_Index_down);
	TupleFind(hv_round_Dist, hv_mode_Dist1 + 1, &hv_Index_up);
	//��������ֵ���ֵ���Ƶ��
	hv_num_mode1 = (hv_f_mode1 + (hv_Index_down.TupleLength())) + (hv_Index_up.TupleLength());
	//�����Ƶ�δ��ڲ�������������֮����ֱ���϶�����
	if (0 != (hv_num_mode1 > ((hv_round_num * 2) / 3)))
	{
		//
		hv_num_mode2 = 0;
		hv_mode_Dist2 = 0;
		//
	}
	else
	{
		//�����Ƶ��С�ڲ�������������֮�������ж��β���
		hv_re_num_new = hv_re_num;
		hv_uniqDist_new = hv_uniq_Dist;
		//Ƶ���������޳�ǰ���������Ƶ��ֵ
		TupleRemove(hv_re_num_new, hv_max_Index, &hv_Reduced_re_num);
		//��ֵ�����������޳�ǰ���������Ƶ����ֵ
		TupleRemove(hv_uniqDist_new, hv_max_Index, &hv_Reduced_uniq_Dist);
		//��������ѡ��ʣ��Ƶ�����������Ƶ��
		hv_f_mode2 = hv_Reduced_re_num.TupleMax();
		//�����Ƶ������Ӧ������
		TupleFind(hv_Reduced_re_num, hv_f_mode2, &hv_max_Index_2);
		//�����Ƶ�ζ�Ӧ����ֵ
		hv_mode_Dist2 = HTuple(hv_Reduced_uniq_Dist[hv_max_Index_2]);
		//�ֱ��ҳ����Ƶ������Ӧ��ֵ�Ӽ�1��������ֵ��Ӧ������
		TupleFind(hv_sort_Dist, hv_mode_Dist2 - 1, &hv_Index_down);
		TupleFind(hv_sort_Dist, hv_mode_Dist2 + 1, &hv_Index_up);
		//��������ֵ���ֵ���Ƶ��
		hv_num_mode2 = (hv_f_mode2 + (hv_Index_down.TupleLength())) + (hv_Index_up.TupleLength());
		//
	}
	//
	//������������ֵ
	(*hv_mode_Dist) = 0;
	//����������������������Ƶ�κ���Դ�С�ҳ�����������
	if (0 != (HTuple(HTuple(hv_num_mode2 > (hv_round_num / 3)).TupleAnd(hv_num_mode1 > (hv_round_num / 3))).TupleOr(HTuple(hv_num_mode2 < (hv_round_num / 3)).TupleAnd(hv_num_mode1 < (hv_round_num / 3)))))
	{
		//��ֵ������������Ʒ�ĳ��ȣ���2�ָ���ֵ
		(*hv_mode_Dist) = ((hv_mode_Dist1.TupleConcat(hv_mode_Dist2)).TupleMax()) * 2;
	}
	else
	{
		//����������ֵ���͵�Ƶ��ȷ������
		if (0 != (hv_num_mode2 > hv_num_mode1))
		{
			(*hv_mode_Dist) = hv_mode_Dist2 * 2;
		}
		else
		{
			(*hv_mode_Dist) = hv_mode_Dist1 * 2;
		}
	}
	return;
}

void SS_DJ_Measure::find_tp(HTuple hv_Distance_L1, HTuple hv_Distance_L2, HTuple hv_Distance_w1, HTuple hv_Distance_w2, 
	HTuple* hv_Distance_L2Out, HTuple* hv_Distance_w1Out, HTuple* hv_Distance_w2Out, HTuple* hv_Index1)
{
	//
	return;
}

void SS_DJ_Measure::find_tp1(HTuple hv_L2_Dist, HTuple hv_w1_Dist, HTuple hv_w2_Dist, HTuple hv_L1_Dist, 
	HTuple* hv_L2out_Dist, HTuple* hv_w1out_Dist, HTuple* hv_w2out_Dist, HTuple* hv_Index1)
{
	// Local iconic variables

  // Local control variables
	HTuple  hv_Sort_dis, hv_Dist_half, hv_Dist, hv_mode_Dist;

	(*hv_L2out_Dist) = hv_L2_Dist;
	(*hv_w1out_Dist) = hv_w1_Dist;
	(*hv_w2out_Dist) = hv_w2_Dist;
	//Distance_L1������Ʒȥ�����������ĳ���
	//��Ʒȱ�ݲ��ֳ��Ƚ�С��ȡ����֮����Ϊ���ų�ȱ�ݵĸ��š�
	TupleSort(hv_L1_Dist, &hv_Sort_dis);
	TupleLastN(hv_Sort_dis, (hv_Sort_dis.TupleLength()) / 3, &hv_Dist_half);
	//
	hv_Dist = hv_Dist_half;
	//��װ���ӣ�������                                         0707   B 2
	find_tm1(hv_Dist, &hv_mode_Dist);
	//****************                                         0707  E2
	//�ҳ�������������4������      0705  �¸ĵ�  ��һ
	(*hv_Index1) = HTuple::TupleGenSequence(0, (hv_L1_Dist.TupleLength()) - 1, 1).TupleSelectMask((hv_L1_Dist.TupleGreaterElem(hv_mode_Dist + 4)).TupleOr(hv_L1_Dist.TupleLessElem(hv_mode_Dist - 4)));
	//�����ϱ��ҳ������ڸ�ԭ�������޳���Ӧֵ
	TupleRemove((*hv_L2out_Dist), (*hv_Index1), &(*hv_L2out_Dist));
	TupleRemove((*hv_w1out_Dist), (*hv_Index1), &(*hv_w1out_Dist));
	TupleRemove((*hv_w2out_Dist), (*hv_Index1), &(*hv_w2out_Dist));
	return;
}

void SS_DJ_Measure::findCrackRegion2(HalconCpp::HObject ho_Image, HalconCpp::HObject* ho_RtRoi, HalconCpp::HObject* ho_Boundary, HalconCpp::HObject* ho_ImageMean, 
	HalconCpp::HObject* ho_DarkRegion, HalconCpp::HObject* ho_RegionIntersection, HalconCpp::HObject* ho_DarkRegionConn,
	HalconCpp::HObject* ho_SelectedRegions, HalconCpp::HObject* ho_SelectedRegionsUnion, HalconCpp::HObject* ho_RegionClosing1, 
	HalconCpp::HObject* ho_RegionClosing1Conn, HalconCpp::HObject* ho_SelectedRegions2, HalconCpp::HObject* ho_CrackRegions, 
	HalconCpp::HObject* ho_ObjectSelected, HalconCpp::HObject* ho_RtObjectSelected, HalconCpp::HObject* ho_RegionDilation, 
	HalconCpp::HObject* ho_RegionIntersection2, HalconCpp::HObject* ho_ImageMeanReduced, HalconCpp::HObject* ho_RtObjectSelectedDarkRegion, 
	HalconCpp::HObject* ho_RtObjectSelectedDarkRegionClosing, HalconCpp::HObject* ho_RtObjectSelectedDarkRegionOpening, HalconCpp::HObject* ho_RtObjectSelectedDarkRegionOpeningConn, 
	HalconCpp::HObject* ho_SelectedRegions3, HalconCpp::HObject* ho_SelectedRegions3Union, HalconCpp::HObject* ho_RegionClosing3, HalconCpp::HObject* ho_RegionOpening3, 
	HalconCpp::HObject* ho_Rt1, HalconCpp::HObject* ho_Rt2, HTuple hv_StartX, HTuple hv_EndX, HTuple hv_Border, HTuple* hv_Width, HTuple* hv_Height, HTuple* hv_Mean, 
	HTuple* hv_Deviation, HTuple* hv_nTh, HTuple* hv_Number1, HTuple* hv_ii, HTuple* hv_Row1, HTuple* hv_Column1, HTuple* hv_Row2, HTuple* hv_Column2, HTuple* hv_Mean1, 
	HTuple* hv_Deviation1, HTuple* hv_Row, HTuple* hv_Column, HTuple* hv_Radius, HTuple* hv_nW, HTuple* hv_Row11, HTuple* hv_Column11, HTuple* hv_Row21, HTuple* hv_Column21,
	HTuple* hv_MeanRt1, HTuple* hv_DeviationRt1, HTuple* hv_MeanRt2, HTuple* hv_DeviationRt2)
{
	// Local iconic variables

	if (HDevWindowStack::IsOpen())
		DispObj(ho_Image, HDevWindowStack::GetActive());

	GetImageSize(ho_Image, &(*hv_Width), &(*hv_Height));
	GenRectangle1(&(*ho_RtRoi), 0, hv_StartX + 60, (*hv_Height), hv_EndX - 60);
	Boundary((*ho_RtRoi), &(*ho_Boundary), "inner");
	Intensity((*ho_RtRoi), ho_Image, &(*hv_Mean), &(*hv_Deviation));

	MeanImage(ho_Image, &(*ho_ImageMean), 55, 55);
	ComputerTh((*ho_RtRoi), (*ho_ImageMean), &(*hv_nTh));
	Threshold((*ho_ImageMean), &(*ho_DarkRegion), 0, (*hv_nTh) / 2);
	Intersection((*ho_RtRoi), (*ho_DarkRegion), &(*ho_RegionIntersection));

	Connection((*ho_RegionIntersection), &(*ho_DarkRegionConn));
	SelectShape((*ho_DarkRegionConn), &(*ho_SelectedRegions), (HTuple("width").Append("height")),
		"or", (HTuple(50).Append(50)), (HTuple(5000).Append(10000)));

	Union1((*ho_SelectedRegions), &(*ho_SelectedRegionsUnion));
	ClosingRectangle1((*ho_SelectedRegionsUnion), &(*ho_RegionClosing1), 55, 55);

	Connection((*ho_RegionClosing1), &(*ho_RegionClosing1Conn));
	SelectShape((*ho_RegionClosing1Conn), &(*ho_SelectedRegions2), (HTuple("width").Append("height")),
		"or", (HTuple(500).Append(500)), (HTuple(5000).Append(10000)));

	GenEmptyObj(&(*ho_Rt2));
	CountObj((*ho_SelectedRegions2), &(*hv_Number1));
	{
		HTuple end_val22 = (*hv_Number1);
		HTuple step_val22 = 1;
		for ((*hv_ii) = 1; (*hv_ii).Continue(end_val22, step_val22); (*hv_ii) += step_val22)
		{
			SelectObj((*ho_SelectedRegions2), &(*ho_ObjectSelected), (*hv_ii));
			SmallestRectangle1((*ho_ObjectSelected), &(*hv_Row1), &(*hv_Column1), &(*hv_Row2),
				&(*hv_Column2));
			if (0 != (HTuple((*hv_Column1) == (hv_StartX + hv_Border)).TupleOr((*hv_Column2) == (hv_EndX - hv_Border))))
			{
				if (0 != (((*hv_Row2) - (*hv_Row1)) > ((*hv_Column2) - (*hv_Column1))))
				{
					//*******�����������ʱ��,�߱ȿ������*****
					continue;
				}
			}
			GenRectangle1(&(*ho_RtObjectSelected), (*hv_Row1), (*hv_Column1), (*hv_Row2),
				(*hv_Column2));
			DilationRectangle1((*ho_RtObjectSelected), &(*ho_RegionDilation), 1000, 1000);
			Intersection((*ho_RegionDilation), (*ho_RtRoi), &(*ho_RegionIntersection2));

			Intensity((*ho_RegionIntersection2), (*ho_ImageMean), &(*hv_Mean1), &(*hv_Deviation1));
			ReduceDomain((*ho_ImageMean), (*ho_RegionIntersection2), &(*ho_ImageMeanReduced)
			);
			Threshold((*ho_ImageMeanReduced), &(*ho_RtObjectSelectedDarkRegion), 0, (*hv_Mean1) - (*hv_Deviation1));
			ClosingRectangle1((*ho_RtObjectSelectedDarkRegion), &(*ho_RtObjectSelectedDarkRegionClosing),
				5, 5);
			OpeningRectangle1((*ho_RtObjectSelectedDarkRegionClosing), &(*ho_RtObjectSelectedDarkRegionOpening),
				25, 25);
			Connection((*ho_RtObjectSelectedDarkRegionOpening), &(*ho_RtObjectSelectedDarkRegionOpeningConn)
			);

			SelectShape((*ho_RtObjectSelectedDarkRegionOpeningConn), &(*ho_SelectedRegions3),
				(HTuple("width").Append("height")), "or", (HTuple(500).Append(500)), (HTuple(5000).Append(10000)));
			Union1((*ho_SelectedRegions3), &(*ho_SelectedRegions3Union));
			ClosingRectangle1((*ho_SelectedRegions3Union), &(*ho_RegionClosing3), 900, 900);
			OpeningRectangle1((*ho_RegionClosing3), &(*ho_RegionOpening3), 55, 55);

			InnerCircle((*ho_RegionOpening3), &(*hv_Row), &(*hv_Column), &(*hv_Radius));
			if (0 != ((*hv_Radius) < 90))
			{
				//*******����Ҫ���ȵ���100
				//***********���������Ǻ���**********
				(*hv_nW) = 100;
				SmallestRectangle1((*ho_RegionOpening3), &(*hv_Row11), &(*hv_Column11), &(*hv_Row21),
					&(*hv_Column21));
				if (0 != (((*hv_Row21) - (*hv_Row11)) > ((*hv_Column21) - (*hv_Column11))))
				{
					//**����
					GenRectangle1(&(*ho_Rt1), (*hv_Row11), (*hv_Column11) - (*hv_nW), (*hv_Row21),
						(*hv_Column11));
					GenRectangle1(&(*ho_Rt2), (*hv_Row11), (*hv_Column21), (*hv_Row21), (*hv_Column21) + (*hv_nW));
				}
				else
				{
					//**����s
					GenRectangle1(&(*ho_Rt1), HTuple(0).TupleMax2((*hv_Row11) - (*hv_nW)), (*hv_Column11),
						(*hv_Row11), (*hv_Column21));
					GenRectangle1(&(*ho_Rt2), (*hv_Row21), (*hv_Column11), ((*hv_Height) - 1).TupleMin2((*hv_Row21) + (*hv_nW)),
						(*hv_Column21));
				}


				Intensity((*ho_Rt1), ho_Image, &(*hv_MeanRt1), &(*hv_DeviationRt1));
				Intensity((*ho_Rt2), ho_Image, &(*hv_MeanRt2), &(*hv_DeviationRt2));

				//stop ()
				if (0 != ((((*hv_MeanRt1) - (*hv_MeanRt2)).TupleAbs()) < 40))
				{
					ConcatObj((*ho_Rt2), (*ho_RegionOpening3), &(*ho_Rt2));
				}
			}
		}
	}
	return;
}

void SS_DJ_Measure::findRegionCrack2(HalconCpp::HObject ho_Image, HalconCpp::HObject* ho_CrackBinImage, HalconCpp::HObject* ho_CrackRegions, 
	HTuple hv_StartX, HTuple hv_EndX, HTuple hv_n, HTuple* hv_CrackRegionsNumber)
{
	// Local iconic variables
	HalconCpp::HObject  ho_RtRoi, ho_Boundary, ho_ImageMean;
	HalconCpp::HObject  ho_DarkRegion, ho_RegionIntersection, ho_DarkRegionConn;
	HalconCpp::HObject  ho_SelectedRegions, ho_SelectedRegionsUnion, ho_RegionClosing1;
	HalconCpp::HObject  ho_RegionClosing1Conn, ho_SelectedRegions2, ho_ObjectSelected;
	HalconCpp::HObject  ho_ValidRect, ho_ValidRectDilation, ho_ValidRegion;
	HalconCpp::HObject  ho_ValidImage, ho_CrackRegion, ho_CrackRegionClosing;
	HalconCpp::HObject  ho_CrackRegionOpening, ho_CrackRegionConn, ho_SelectedRegions3;
	HalconCpp::HObject  ho_SelectedRegions3Union, ho_RegionClosing3, ho_RegionOpening3;
	HalconCpp::HObject  ho_Rt1, ho_Rt2;

	// Local control variables
	HTuple  hv_Width, hv_Height, hv_Border, hv_nTh;
	HTuple  hv_Number1, hv_obj_i, hv_Row1, hv_ColumnL, hv_Row2;
	HTuple  hv_ColumnR, hv_Mean1, hv_Deviation1, hv_Row, hv_Column;
	HTuple  hv_Radius, hv_nW, hv_Row11, hv_Column11, hv_Row21;
	HTuple  hv_Column21, hv_MeanRt1, hv_DeviationRt1, hv_MeanRt2;
	HTuple  hv_DeviationRt2;

	GetImageSize(ho_Image, &hv_Width, &hv_Height);
	//**�������ڲ࣬���޳�60�����ؿ��
	hv_Border = 60;
	GenRectangle1(&ho_RtRoi, 0, hv_StartX + hv_Border, hv_Height, hv_EndX - hv_Border);
	Boundary(ho_RtRoi, &ho_Boundary, "inner");

	MeanImage(ho_Image, &ho_ImageMean, 55, 55);
	//*��������Ӧ��ֵ
	ComputerTh(ho_RtRoi, ho_ImageMean, &hv_nTh);
	Threshold(ho_ImageMean, &ho_DarkRegion, 0, hv_nTh / 2);
	//*ͨ����RtRoi���������󽻼���ȥ���߽����
	Intersection(ho_RtRoi, ho_DarkRegion, &ho_RegionIntersection);
	//*ͨ����ͨ/ѡ��ȥ����ɢ�������ĸ���
	Connection(ho_RegionIntersection, &ho_DarkRegionConn);
	SelectShape(ho_DarkRegionConn, &ho_SelectedRegions, (HTuple("width").Append("height")),
		"or", (HTuple(50).Append(50)), (HTuple(5000).Append(10000)));
	//*�ϲ�ʣ�����򣬱�����
	Union1(ho_SelectedRegions, &ho_SelectedRegionsUnion);
	ClosingRectangle1(ho_SelectedRegionsUnion, &ho_RegionClosing1, 55, 55);
	//*ͨ����ͨ/ѡ���ٴ�ȥ���ϴ���ɢ�������ĸ���
	Connection(ho_RegionClosing1, &ho_RegionClosing1Conn);
	SelectShape(ho_RegionClosing1Conn, &ho_SelectedRegions2, (HTuple("width").Append("height")),
		"or", (HTuple(500).Append(500)), (HTuple(5000).Append(10000)));
	//**Ĭ��ʣ������Ϊ���ƣ����д���
	GenEmptyObj(&(*ho_CrackRegions));
	CountObj(ho_SelectedRegions2, &hv_Number1);
	{
		HTuple end_val24 = hv_Number1;
		HTuple step_val24 = 1;
		for (hv_obj_i = 1; hv_obj_i.Continue(end_val24, step_val24); hv_obj_i += step_val24)
		{
			SelectObj(ho_SelectedRegions2, &ho_ObjectSelected, hv_obj_i);
			SmallestRectangle1(ho_ObjectSelected, &hv_Row1, &hv_ColumnL, &hv_Row2, &hv_ColumnR);
			//**�ж���������������ơ����н������ƣ���ˮƽ����ᴩʱ����ֱ����������ҪС��ˮƽ���򣬲�Ȼ�����
			//**�ж��Ƿ��������ҷ���ᴩ����,��Column1
			if (0 != (HTuple(hv_ColumnL == (hv_StartX + hv_Border)).TupleOr(hv_ColumnR == (hv_EndX - hv_Border))))
			{
				//**���ߴ��ڿ��������������ԣ�ȥ��������
				if (0 != ((hv_Row2 - hv_Row1) > (hv_ColumnR - hv_ColumnL)))
				{
					//*****�������������ԣ�����������һ��������
					continue;
				}
			}

			//*������������ж�λ��
			GenRectangle1(&ho_ValidRect, hv_Row1, hv_ColumnL, hv_Row2, hv_ColumnR);
			DilationRectangle1(ho_ValidRect, &ho_ValidRectDilation, 1000, 1000);
			Intersection(ho_ValidRectDilation, ho_RtRoi, &ho_ValidRegion);
			//**�������������ֵ����
			Intensity(ho_ValidRegion, ho_ImageMean, &hv_Mean1, &hv_Deviation1);
			ReduceDomain(ho_ImageMean, ho_ValidRegion, &ho_ValidImage);
			Threshold(ho_ValidImage, &ho_CrackRegion, 0, hv_Mean1 - hv_Deviation1);
			ClosingRectangle1(ho_CrackRegion, &ho_CrackRegionClosing, 5, 5);
			OpeningRectangle1(ho_CrackRegionClosing, &ho_CrackRegionOpening, 25, 25);
			Connection(ho_CrackRegionOpening, &ho_CrackRegionConn);
			//**Ϊ��ȥ��������λ������ֵ�����в����ĸ���С����
			SelectShape(ho_CrackRegionConn, &ho_SelectedRegions3, (HTuple("width").Append("height")),
				"or", (HTuple(500).Append(500)), (HTuple(5000).Append(10000)));
			Union1(ho_SelectedRegions3, &ho_SelectedRegions3Union);
			//**����ֵ900����֤����ȱ�ݷ�գ���Ӱ�����������Բ�������       ????
			ClosingRectangle1(ho_SelectedRegions3Union, &ho_RegionClosing3, 900, 900);
			OpeningRectangle1(ho_RegionClosing3, &ho_RegionOpening3, 55, 55);

			//����������ƿ�ȣ�ȡֵ90���ж��Ƿ�Ϊ����
			InnerCircle(ho_RegionOpening3, &hv_Row, &hv_Column, &hv_Radius);
			if (0 != (hv_Radius < 90))
			{
				//*******����Ҫ���ȵ���100�����Ʒ�����ˮƽ����ֱ
				//***********���������Ǻ���**********
				//**ͨ�����������������У��Ҷ�ֵ�Ĳ���Ĵ�С����ӳ��
				//**����������ʵ���ƻ��Ǿ���ȱ�ݵ�����(ȱ�ݵ���ֵ����ӳ�������ƣ�ͨ���Ҷ�ֵ�Ĳ��죬���жϣ�
				//region_features (RegionOpening3, 'phi', Value)
				//dilation_rectangle1 (RegionOpening3, RegionDilation1, 200, 200)
				//difference (RegionOpening3, RegionDilation1, RegionDifference)

				hv_nW = 100;
				SmallestRectangle1(ho_RegionOpening3, &hv_Row11, &hv_Column11, &hv_Row21, &hv_Column21);
				if (0 != ((hv_Row21 - hv_Row11) > (hv_Column21 - hv_Column11)))
				{
					//**����
					GenRectangle1(&ho_Rt1, hv_Row11, hv_Column11 - hv_nW, hv_Row21, hv_Column11);
					GenRectangle1(&ho_Rt2, hv_Row11, hv_Column21, hv_Row21, hv_Column21 + hv_nW);
				}
				else
				{
					//**����
					GenRectangle1(&ho_Rt1, HTuple(0).TupleMax2(hv_Row11 - hv_nW), hv_Column11,
						hv_Row11, hv_Column21);
					GenRectangle1(&ho_Rt2, hv_Row21, hv_Column11, (hv_Height - 1).TupleMin2(hv_Row21 + hv_nW),
						hv_Column21);
				}

				//**�����������ڲ��Ҷ�ֵ����
				Intensity(ho_Rt1, ho_Image, &hv_MeanRt1, &hv_DeviationRt1);
				Intensity(ho_Rt2, ho_Image, &hv_MeanRt2, &hv_DeviationRt2);

				//������������������ڣ��ҶȲ���С��40���ж�Ϊ��������
				//stop ()
				if (0 != (((hv_MeanRt1 - hv_MeanRt2).TupleAbs()) < 40))
				{
					ConcatObj((*ho_CrackRegions), ho_RegionOpening3, &(*ho_CrackRegions));
				}
			}
		}
	}

	CountObj((*ho_CrackRegions), &(*hv_CrackRegionsNumber));
	RegionToBin((*ho_CrackRegions), &(*ho_CrackBinImage), 255, 0, hv_Width, hv_Height);
	ZoomImageFactor((*ho_CrackBinImage), &(*ho_CrackBinImage), hv_n, hv_n, "constant");
	return;
}

void SS_DJ_Measure::getcrack(HalconCpp::HObject ho_BinImage_crack, HalconCpp::HObject* ho_result_crack, HTuple hv_n, HTuple hv_Line_MaxWidth, HTuple hv_width_crack)
{
	// Local iconic variables
	HalconCpp::HObject  ho_crack_Centers, ho_Unionlights, ho_Selectedlight1;
	HalconCpp::HObject  ho_Selected_Length, ho_Selected_maxDis, ho_Selected_cir;

	// Local control variables
	HTuple  hv_MaxLineWidth, hv_Contrast, hv_Sigma;
	HTuple  hv_Low, hv_High;

	//**��ɭ������ȡ������
	hv_MaxLineWidth = hv_Line_MaxWidth;
	hv_Contrast = 100;
	calculate_lines_gauss_parameters(hv_MaxLineWidth, hv_Contrast, &hv_Sigma, &hv_Low,
		&hv_High);
	LinesGauss(ho_BinImage_crack, &ho_crack_Centers, hv_Sigma, hv_Low, hv_High, "light",
		"false", "parabolic", "true");
	//**�������߽������ӣ�ѡ�񣬵õ�ʵ�ʵ�����
	SelectContoursXld(ho_crack_Centers, &ho_crack_Centers, "contour_length", 25, 2000000,
		-0.5, 0.5);
	UnionCollinearContoursXld(ho_crack_Centers, &ho_crack_Centers, 35, 1, 15, 0.3,
		"attr_keep");
	//
	UnionAdjacentContoursXld(ho_crack_Centers, &ho_Unionlights, 25, 1, "attr_keep");
	SelectContoursXld(ho_Unionlights, &ho_Selectedlight1, "contour_length", 40, 2000000,
		-0.5, 0.5);
	UnionAdjacentContoursXld(ho_Selectedlight1, &ho_Unionlights, 50, 1, "attr_keep");
	UnionCollinearContoursXld(ho_Unionlights, &ho_Unionlights, 50, 1, 25, 0.3, "attr_keep");
	SelectContoursXld(ho_Unionlights, &ho_Selected_Length, "contour_length", 80, 999999999999,
		-0.5, 0.5);
	//***�����,С��80���޳�
	max_distance(ho_Selected_Length, &ho_Selected_maxDis);
	//***Բ�ȣ�����0.25���޳�
	cir_select(ho_Selected_maxDis, &ho_Selected_cir);
	//****
	(*ho_result_crack) = ho_Selected_cir;
	//
	return;
}

void SS_DJ_Measure::Go2GenTL_ParseData(HalconCpp::HObject ho_Image, HalconCpp::HObject* ho_HeightMap, HalconCpp::HObject* ho_Intensity, 
	HTuple hv_Index, HTuple* hv_FrameCount, HTuple* hv_Timestamp, HTuple* hv_EncoderPosition, HTuple* hv_EncoderIndex, 
	HTuple* hv_Inputs, HTuple* hv_xOffset, HTuple* hv_xResolution, HTuple* hv_yOffset, HTuple* hv_yResolution, HTuple* hv_zOffset, 
	HTuple* hv_zResolution, HTuple* hv_Width, HTuple* hv_Length, HTuple* hv_HasIntensity, HTuple* hv_NumScans)
{
	// Local iconic variables
	HalconCpp::HObject  ho_Stamps;

	// Local control variables
	HTuple  hv_channelCount, hv_monoImageWidth, hv_monoImageHeight;
	HTuple  hv_IntensityPosition, hv_start, hv_end;

	CountChannels(ho_Image, &hv_channelCount);

	if (0 != (hv_channelCount == 1))
	{

		GetImageSize(ho_Image, &hv_monoImageWidth, &hv_monoImageHeight);
		CropRectangle1(ho_Image, &ho_Stamps, hv_monoImageHeight - 1, 0, hv_monoImageHeight - 1,
			hv_monoImageWidth);

		Go2GenTLStamp(ho_Stamps, 1, &(*hv_FrameCount));
		Go2GenTLStamp(ho_Stamps, 2, &(*hv_Timestamp));
		Go2GenTLStamp(ho_Stamps, 3, &(*hv_EncoderPosition));
		Go2GenTLStamp(ho_Stamps, 4, &(*hv_EncoderIndex));
		Go2GenTLStamp(ho_Stamps, 5, &(*hv_Inputs));
		Go2GenTLStamp(ho_Stamps, 6, &(*hv_xOffset));
		Go2GenTLStamp(ho_Stamps, 7, &(*hv_xResolution));
		Go2GenTLStamp(ho_Stamps, 8, &(*hv_yOffset));
		Go2GenTLStamp(ho_Stamps, 9, &(*hv_yResolution));
		Go2GenTLStamp(ho_Stamps, 10, &(*hv_zOffset));
		Go2GenTLStamp(ho_Stamps, 11, &(*hv_zResolution));
		Go2GenTLStamp(ho_Stamps, 12, &(*hv_Width));
		Go2GenTLStamp(ho_Stamps, 13, &(*hv_Length));
		Go2GenTLStamp(ho_Stamps, 14, &(*hv_HasIntensity));
		Go2GenTLStamp(ho_Stamps, 15, &(*hv_NumScans));

		//* Mono output does not support multiple heightmaps, assuming index is 0
		CropRectangle1(ho_Image, &(*ho_HeightMap), 0, 0, (*hv_Length) - 1, (*hv_Width));
		if (0 != ((*hv_HasIntensity) == 1))
		{
			hv_IntensityPosition = (hv_monoImageHeight - 1) / 2;
			CropRectangle1(ho_Image, &(*ho_Intensity), hv_IntensityPosition, 0, (hv_IntensityPosition + (*hv_Length)) - 1,
				(*hv_Width));
		}
		else
		{
			GenImageConst(&(*ho_Intensity), "byte", (*hv_Width), (*hv_Length));
		}

	}
	else if (0 != (hv_channelCount == 3))
	{

		Decompose3(ho_Image, &(*ho_HeightMap), &(*ho_Intensity), &ho_Stamps);

		Go2GenTLStamp(ho_Stamps, 1, &(*hv_FrameCount));
		Go2GenTLStamp(ho_Stamps, 2, &(*hv_Timestamp));
		Go2GenTLStamp(ho_Stamps, 3, &(*hv_EncoderPosition));
		Go2GenTLStamp(ho_Stamps, 4, &(*hv_EncoderIndex));
		Go2GenTLStamp(ho_Stamps, 5, &(*hv_Inputs));
		Go2GenTLStamp(ho_Stamps, 6, &(*hv_xOffset));
		Go2GenTLStamp(ho_Stamps, 7, &(*hv_xResolution));
		Go2GenTLStamp(ho_Stamps, 8, &(*hv_yOffset));
		Go2GenTLStamp(ho_Stamps, 9, &(*hv_yResolution));
		Go2GenTLStamp(ho_Stamps, 10, &(*hv_zOffset));
		Go2GenTLStamp(ho_Stamps, 11, &(*hv_zResolution));
		Go2GenTLStamp(ho_Stamps, 12, &(*hv_Width));
		Go2GenTLStamp(ho_Stamps, 13, &(*hv_Length));
		Go2GenTLStamp(ho_Stamps, 14, &(*hv_HasIntensity));
		Go2GenTLStamp(ho_Stamps, 15, &(*hv_NumScans));

		//* Split Height Map
		hv_start = hv_Index * (*hv_Length);
		hv_end = (hv_Index + 1) * (*hv_Length);

		if (0 != ((*hv_NumScans) > hv_Index))
		{
			CropRectangle1((*ho_HeightMap), &(*ho_HeightMap), hv_start, 0, hv_end, (*hv_Width));
			if (0 != ((*hv_HasIntensity) == 1))
			{
				CropRectangle1((*ho_Intensity), &(*ho_Intensity), hv_start, 0, hv_end, (*hv_Width));
			}
		}

	}

	//* Change Offset and resolution back to floating point and described in mm
	(*hv_xOffset) = (*hv_xOffset) / 1000000.0;
	(*hv_yOffset) = (*hv_yOffset) / 1000000.0;
	(*hv_zOffset) = (*hv_zOffset) / 1000000.0;
	(*hv_xResolution) = (*hv_xResolution) / 1000000.0;
	(*hv_yResolution) = (*hv_yResolution) / 1000000.0;
	(*hv_zResolution) = (*hv_zResolution) / 1000000.0;

	return;
}

void SS_DJ_Measure::Go2GenTLStamp(HalconCpp::HObject ho_Stamps, HTuple hv_Index, HTuple* hv_Value)
{
	// Local iconic variables

  // Local control variables
	HTuple  hv_test_value, hv_is64bit, hv_tempvalue0;
	HTuple  hv_tempvalue1, hv_tempvalue2, hv_tempvalue3;

	//Check if we are on a 64-bit machine
	//����ԭʼ����λ�ã��д�����
	hv_test_value = (HTuple)(int)0xFFFFFFFF;

	if (0 != (hv_test_value > 0))
	{
		hv_is64bit = 1;
	}
	else
	{
		hv_is64bit = 0;
	}

	GetGrayval(ho_Stamps, 0, hv_Index * 4, &hv_tempvalue0);
	GetGrayval(ho_Stamps, 0, (hv_Index * 4) + 1, &hv_tempvalue1);
	GetGrayval(ho_Stamps, 0, (hv_Index * 4) + 2, &hv_tempvalue2);
	GetGrayval(ho_Stamps, 0, (hv_Index * 4) + 3, &hv_tempvalue3);

	//The actual stamp from the Gocator is 64-bit. tempvalue0 is the most significant 16-bit (i.e. the top bit is the sign bit)
	//The code below assumes we only need the bottom 32-bit information. User need to update this conversion function if they want to
	//return 64-bit value.

	if (0 != (hv_is64bit == 1))
	{
		TupleLsh(hv_tempvalue0, 48, &hv_tempvalue0);
		TupleLsh(hv_tempvalue1, 32, &hv_tempvalue1);
		TupleLsh(hv_tempvalue2, 16, &hv_tempvalue2);
		TupleLsh(hv_tempvalue3, 0, &hv_tempvalue3);
		(*hv_Value) = ((hv_tempvalue0 + hv_tempvalue1) + hv_tempvalue2) + hv_tempvalue3;
	}
	else
	{
		TupleLsh(hv_tempvalue2, 16, &hv_tempvalue2);
		TupleLsh(hv_tempvalue3, 0, &hv_tempvalue3);
		(*hv_Value) = hv_tempvalue2 + hv_tempvalue3;
	}
	return;
}

void SS_DJ_Measure::image_deal(HalconCpp::HObject ho_BinImage_crack, HTuple hv_width_crack, HTuple hv_n, HTuple hv_height_crack, 
	HTuple hv_high_image, HTuple* hv_begin_row, HTuple* hv_end_row)
{
	// Local iconic variables
	HalconCpp::HObject  ho_Rectangle;

	// Local control variables
	HTuple  hv_w_bin, hv_h_bin, hv_k_up, hv_k_down;
	HTuple  hv_nk, hv_Row_n1, hv_Row_n2, hv_Column_n, hv_MeasureHandle;
	HTuple  hv_GrayValues1, hv_Sum1, hv_GrayValues2, hv_Sum2;
	HTuple  hv_b_up, hv_b_down;

	//
	hv_w_bin = hv_width_crack.TupleRound();
	hv_h_bin = hv_height_crack.TupleRound();
	//
	hv_k_up = HTuple();
	hv_k_down = HTuple();
	//
	//*�ֱ���ϵ��ºʹ��µ��ϣ�����11�����Ϊ20��С����
	//*���С�����а��������������עΪ1������Ϊ0��
	//*�ж������Ƿ���Ϊ�ɼ�ԭ�򣬷�Ϊ���������֡�
	for (hv_nk = 0; hv_nk <= 10; hv_nk += 1)
	{
		hv_Row_n1 = (41 * hv_nk) + 20;
		hv_Row_n2 = (hv_h_bin - (41 * hv_nk)) - 20;
		hv_Column_n = hv_w_bin / 2;
		//�����ϲ�������
		GenRectangle2(&ho_Rectangle, hv_Row_n1, hv_Column_n, 0, hv_Column_n, 20);
		GenMeasureRectangle2(hv_Row_n1, hv_Column_n, 0, hv_Column_n, 20, hv_w_bin, hv_h_bin,
			"bilinear", &hv_MeasureHandle);
		MeasureProjection(ho_BinImage_crack, hv_MeasureHandle, &hv_GrayValues1);
		TupleSum(hv_GrayValues1, &hv_Sum1);
		if (0 != (hv_Sum1 > 0))
		{
			hv_k_up = hv_k_up.TupleConcat(1);
		}
		else
		{
			hv_k_up = hv_k_up.TupleConcat(0);
		}
		//
		//�����²�������
		GenRectangle2(&ho_Rectangle, hv_Row_n2, hv_Column_n, 0, hv_Column_n, 20);
		GenMeasureRectangle2(hv_Row_n2, hv_Column_n, 0, hv_Column_n, 20, hv_w_bin, hv_h_bin,
			"bilinear", &hv_MeasureHandle);
		MeasureProjection(ho_BinImage_crack, hv_MeasureHandle, &hv_GrayValues2);
		TupleSum(hv_GrayValues2, &hv_Sum2);
		if (0 != (hv_Sum2 > 0))
		{
			hv_k_down = hv_k_down.TupleConcat(1);
		}
		else
		{
			hv_k_down = hv_k_down.TupleConcat(0);
		}
		//
	}
	//*��ʼλ�ó�ʼ����1212�Ǹ���5.5��Ʒ���ĳ�ֵ��
	//*����ǰ��Ĭ��Ϊ�����������ƣ�����һ�����Ƴ���С
	//*��11�����εĸ߶ȡ�
	(*hv_begin_row) = 0;
	(*hv_end_row) = hv_high_image;
	//���ϱ�11�����������ڣ���0ʱ�������С�������������ʼ������
	if (0 != ((hv_k_up.TupleSum()) < 11))
	{
		TupleFind(hv_k_up, 0, &hv_b_up);
		(*hv_begin_row) = HTuple(hv_b_up[0]) * 41;
		(*hv_end_row) = (*hv_begin_row) + hv_high_image;
	}
	//*����ϱ�11��С���ζ������ƣ���ͬ�����������±߿�ʼ���������ֹ������
	if (0 != (HTuple((hv_k_down.TupleSum()) < 11).TupleAnd((hv_k_up.TupleSum()) == 11)))
	{
		TupleFind(hv_k_down, 0, &hv_b_down);
		(*hv_end_row) = hv_h_bin - (HTuple(hv_b_down[0]) * 41);
		(*hv_begin_row) = (*hv_end_row) - hv_high_image;
	}
	//*���������������������㣬�������ʼλ�ð���ʼ��ֵ���㡣
	return;
}

void SS_DJ_Measure::max_distance(HalconCpp::HObject ho_Selected_Length, HalconCpp::HObject* ho_Selected_maxDis)
{
	// Local iconic variables

  // Local control variables
	HTuple  hv_xuhao, hv_Row1, hv_Column1, hv_Row21;
	HTuple  hv_Column21, hv_Diameter1, hv_s;

	hv_xuhao = HTuple();
	DiameterXld(ho_Selected_Length, &hv_Row1, &hv_Column1, &hv_Row21, &hv_Column21,
		&hv_Diameter1);
	{
		HTuple end_val2 = hv_Diameter1.TupleLength();
		HTuple step_val2 = 1;
		for (hv_s = 1; hv_s.Continue(end_val2, step_val2); hv_s += step_val2)
		{
			if (0 != (HTuple(hv_Diameter1[hv_s - 1]) > 80))
			{
				hv_xuhao = hv_xuhao.TupleConcat(hv_s);
			}
		}
	}
	SelectObj(ho_Selected_Length, &(*ho_Selected_maxDis), hv_xuhao);
	return;
}

void SS_DJ_Measure::MeanValue(HTuple hv_Dist, HTuple* hv_MeanV, HTuple* hv_IndexError_M)
{
	// Local iconic variables

  // Local control variables
	HTuple  hv_NeWTuple, hv_SortTuple, hv_RoundTuple;
	HTuple  hv_UniqTuple, hv_TypeNum, hv_EachRates, hv_TupleNum;
	HTuple  hv_EachIndexs, hv_ModeV, hv_NewDist;

	hv_NeWTuple = hv_Dist;
	TupleRates(hv_NeWTuple, &hv_SortTuple, &hv_RoundTuple, &hv_UniqTuple, &hv_TypeNum,
		&hv_EachRates, &hv_TupleNum, &hv_EachIndexs);
	//�ҳ����Ƶ�ζ�Ӧ������ֵ
	hv_ModeV = ((const HTuple&)hv_UniqTuple)[0];
	//�ҳ�������������1������
	(*hv_IndexError_M) = HTuple::TupleGenSequence(0, (hv_Dist.TupleLength()) - 1, 1).TupleSelectMask((hv_Dist.TupleGreaterElem(hv_ModeV + 1)).TupleOr(hv_Dist.TupleLessElem(hv_ModeV - 1)));
	TupleRemove(hv_Dist, (*hv_IndexError_M), &hv_NewDist);
	TupleMean(hv_NewDist, &(*hv_MeanV));
	return;
}

void SS_DJ_Measure::PelletEdges(HalconCpp::HObject ho_Image, HalconCpp::HObject* ho_ImageOut, HalconCpp::HObject* ho_Rectangle_select, HalconCpp::HObject* ho_Image_se, 
	HalconCpp::HObject* ho_ImageMedian, HalconCpp::HObject* ho_Region, HalconCpp::HObject* ho_RegionOpening, HalconCpp::HObject* ho_Regions, HalconCpp::HObject* ho_Regionselect,
	HalconCpp::HObject* ho_Cross, HalconCpp::HObject* ho_Cross1, HalconCpp::HObject* ho_Cross2, HalconCpp::HObject* ho_Cross3, HalconCpp::HObject* ho_Cross4,
	HTuple hv_ImWidth, HTuple hv_RegionMin, HTuple hv_RegionMax, HTuple hv_PixelSize, HTuple* hv_ValidLength, 
	HTuple* hv_NewImWidth, HTuple* hv_NewImHeight, HTuple* hv_thre_value, HTuple* hv_Value, HTuple* hv_region_number, 
	HTuple* hv_PelletExist, HTuple* hv_RtCenterCol, HTuple* hv_SmallRtWidth, HTuple* hv_ColEdge1, HTuple* hv_ColEdge2, 
	HTuple* hv_ColEdge3, HTuple* hv_ColEdge4, HTuple* hv_RowAll, HTuple* hv_W1, HTuple* hv_W2, HTuple* hv_L1, HTuple* hv_L2, 
	HTuple* hv_NewL2, HTuple* hv_NewW1, HTuple* hv_NewW2, HTuple* hv_IndexError, HTuple* hv_Dist, HTuple* hv_Mean_length, 
	HTuple* hv_PIXELwidth_L, HTuple* hv_ChamWidthR, HTuple* hv_PIXELwidth_R, HTuple* hv_ChamWidthL, HTuple* hv_PelletLength, 
	HTuple* hv_ROI_width, HTuple* hv_Column_re_T, HTuple* hv_Col_e1, HTuple* hv_Col_e2, HTuple* hv_Col_e3, HTuple* hv_Col_e4)
{
	// Local iconic variables

  // Local control variables
	HTuple  hv_RtCenterRow, hv_RtWidth;

	(*ho_ImageOut) = ho_Image;
	//**��ȡ�ߴ���ͼƬ
	ZoomImageFactor((*ho_ImageOut), &(*ho_ImageOut), 0.5, 0.5, "constant");
	//������Ч����
	(*hv_ValidLength) = 2423;
	GenRectangle1(&(*ho_Rectangle_select), 0, 0, (*hv_ValidLength) - 1, hv_ImWidth);
	ReduceDomain((*ho_ImageOut), (*ho_Rectangle_select), &(*ho_Image_se));
	CropDomainRel((*ho_Image_se), &(*ho_Image_se), 0, 0, 0, 0);
	GetImageSize((*ho_Image_se), &(*hv_NewImWidth), &(*hv_NewImHeight));
	MedianImage((*ho_Image_se), &(*ho_ImageMedian), "circle", 12, "mirrored");
	//��ֵ
	(*hv_thre_value) = 20;
	Threshold((*ho_ImageMedian), &(*ho_Region), (*hv_thre_value), 256);
	//**ȥ������Ȥ���������ĸ�������
	OpeningRectangle1((*ho_Region), &(*ho_RegionOpening), 150, 150);
	Connection((*ho_RegionOpening), &(*ho_Regions));
	//***
	RegionFeatures((*ho_Regions), "area", &(*hv_Value));
	SelectShape((*ho_Regions), &(*ho_Regionselect), "area", "and", hv_RegionMin, hv_RegionMax);
	CountObj((*ho_Regionselect), &(*hv_region_number));

	//switch (region_number)

	//case 0:

	(*hv_PelletExist) = 0;
	//    break

	//case 1:

	AllEdges((*ho_Regionselect), (*ho_ImageMedian), (*hv_NewImWidth), (*hv_NewImHeight),
		&(*hv_RtCenterCol), &(*hv_SmallRtWidth), &(*hv_ColEdge1), &(*hv_ColEdge2),
		&(*hv_ColEdge3), &(*hv_ColEdge4), &(*hv_RowAll), &hv_RtCenterRow, &hv_RtWidth);

	(*hv_W1) = (*hv_ColEdge2) - (*hv_ColEdge1);
	(*hv_W2) = (*hv_ColEdge4) - (*hv_ColEdge3);
	(*hv_L1) = (*hv_ColEdge3) - (*hv_ColEdge2);
	(*hv_L2) = (*hv_ColEdge4) - (*hv_ColEdge1);
	//��װ���ӣ��޳��쳣ֵ
	TruePoints((*hv_L1), (*hv_L2), (*hv_W1), (*hv_W2), &(*hv_NewL2), &(*hv_NewW1),
		&(*hv_NewW2), &(*hv_IndexError));

	//*****����൹��
	(*hv_Dist) = (*hv_NewW1);
	find_Length1((*hv_Dist), &(*hv_Mean_length));
	(*hv_PIXELwidth_L) = (*hv_Mean_length);
	(*hv_ChamWidthR) = (*hv_Mean_length) * hv_PixelSize;
	//*****���Ҳ൹��
	(*hv_Dist) = (*hv_NewW2);
	find_Length1((*hv_Dist), &(*hv_Mean_length));
	(*hv_PIXELwidth_R) = (*hv_Mean_length);
	(*hv_ChamWidthL) = (*hv_Mean_length) * hv_PixelSize;
	//*****����Ʒ����
	(*hv_Dist) = (*hv_NewL2);
	find_Length1((*hv_Dist), &(*hv_Mean_length));
	(*hv_PelletLength) = (*hv_Mean_length) * hv_PixelSize;


	TupleRemove((*hv_ColEdge1), (*hv_IndexError), &(*hv_ColEdge1));
	TupleRemove((*hv_ColEdge2), (*hv_IndexError), &(*hv_ColEdge2));
	TupleRemove((*hv_ColEdge3), (*hv_IndexError), &(*hv_ColEdge3));
	TupleRemove((*hv_ColEdge4), (*hv_IndexError), &(*hv_ColEdge4));
	TupleRemove((*hv_RowAll), (*hv_IndexError), &(*hv_RowAll));
	TupleGenConst((*hv_ColEdge1).TupleLength(), (*hv_SmallRtWidth), &(*hv_ROI_width));
	TupleGenConst((*hv_ColEdge1).TupleLength(), (*hv_RtCenterCol), &(*hv_Column_re_T));
	(*hv_Col_e1) = (*hv_Column_re_T) - ((*hv_ROI_width) - (*hv_ColEdge1));
	(*hv_Col_e2) = (*hv_Column_re_T) - ((*hv_ROI_width) - (*hv_ColEdge2));
	(*hv_Col_e3) = (*hv_Column_re_T) - ((*hv_ROI_width) - (*hv_ColEdge3));
	(*hv_Col_e4) = (*hv_Column_re_T) - ((*hv_ROI_width) - (*hv_ColEdge4));

	GenEmptyObj(&(*ho_Cross));
	GenCrossContourXld(&(*ho_Cross1), (*hv_RowAll), (*hv_Col_e1), 16, 0.785398);
	GenCrossContourXld(&(*ho_Cross2), (*hv_RowAll), (*hv_Col_e2), 16, 0.785398);
	GenCrossContourXld(&(*ho_Cross3), (*hv_RowAll), (*hv_Col_e3), 16, 0.785398);
	GenCrossContourXld(&(*ho_Cross4), (*hv_RowAll), (*hv_Col_e4), 16, 0.785398);
	ConcatObj((*ho_Cross), (*ho_Cross1), &(*ho_Cross));
	ConcatObj((*ho_Cross), (*ho_Cross2), &(*ho_Cross));
	ConcatObj((*ho_Cross), (*ho_Cross3), &(*ho_Cross));
	ConcatObj((*ho_Cross), (*ho_Cross4), &(*ho_Cross));
	return;
}

void SS_DJ_Measure::Program1(HalconCpp::HObject ho_Image, HalconCpp::HObject* ho_ImgOri, HalconCpp::HObject* ho_ImageOut, HalconCpp::HObject* ho_Rectangle_select, 
	HalconCpp::HObject* ho_Image_se, HalconCpp::HObject* ho_ImageMedian, HalconCpp::HObject* ho_Region, HalconCpp::HObject* ho_RegionOpening, HalconCpp::HObject* ho_Regions,
	HalconCpp::HObject* ho_Regionselect, HalconCpp::HObject* ho_Cross, HalconCpp::HObject* ho_Cross1, HalconCpp::HObject* ho_Cross2, HalconCpp::HObject* ho_Cross3,
	HalconCpp::HObject* ho_Cross4, HalconCpp::HObject* ho_CrackShow, HalconCpp::HObject* ho_ShowRectangle, HalconCpp::HObject* ho_ImageShow, HalconCpp::HObject* ho_CrackBinImage,
	HalconCpp::HObject* ho_CrackRegions, HalconCpp::HObject* ho_ValidRecRegion, HalconCpp::HObject* ho_Region2, HalconCpp::HObject* ho_CrackCenters,
	HalconCpp::HObject* ho_SelectCenters, HalconCpp::HObject* ho_UnionCenters, HalconCpp::HObject* ho_Unionlights, HalconCpp::HObject* ho_CrackResult,
	HalconCpp::HObject* ho_result_crack, HTuple hv_RegionMin, HTuple hv_RegionMax, HTuple hv_PixelSize, HTuple hv_n,
	HTuple hv_WidthCrack, HTuple* hv_ImWidth, HTuple* hv_ImHeight, HTuple* hv_ValidLength, HTuple* hv_NewImWidth, 
	HTuple* hv_NewImHeight, HTuple* hv_thre_value, HTuple* hv_Value, HTuple* hv_region_number, HTuple* hv_PelletExist, HTuple* hv_RtCenterCol, 
	HTuple* hv_SmallRtWidth, HTuple* hv_ColEdge1, HTuple* hv_ColEdge2, HTuple* hv_ColEdge3, HTuple* hv_ColEdge4, HTuple* hv_RowAll, 
	HTuple* hv_W1, HTuple* hv_W2, HTuple* hv_L1, HTuple* hv_L2, HTuple* hv_NewL2, HTuple* hv_NewW1, HTuple* hv_NewW2, HTuple* hv_IndexError, 
	HTuple* hv_Dist, HTuple* hv_Mean_length, HTuple* hv_PIXELwidth_L, HTuple* hv_ChamWidthR, HTuple* hv_PIXELwidth_R, HTuple* hv_ChamWidthL, 
	HTuple* hv_PelletLength, HTuple* hv_ROI_width, HTuple* hv_Column_re_T, HTuple* hv_Col_e1, HTuple* hv_Col_e2, HTuple* hv_Col_e3, 
	HTuple* hv_Col_e4, HTuple* hv_CracksNum, HTuple* hv_CracksLength, HTuple* hv_CracksLengthsSum, HTuple* hv_LineMaxWidth, 
	HTuple* hv_StartX, HTuple* hv_EndX, HTuple* hv_HeightCrack, HTuple* hv_CrackRegionsNumber, HTuple* hv_CracksArea, 
	HTuple* hv_MaxLineWidth, HTuple* hv_Contrast, HTuple* hv_Sigma, HTuple* hv_Low, HTuple* hv_High, HTuple* hv_CracksLengths, 
	HTuple* hv_Pellet_exist, HTuple* hv_NG)
{
	// Local iconic variables

  // Local control variables
	HTuple  hv_RtCenterRow, hv_RtWidth;

	(*ho_ImageOut) = ho_Image;
	GetImageSize((*ho_ImageOut), &(*hv_ImWidth), &(*hv_ImHeight));
	(*ho_ImgOri) = ho_Image;
	//**��ȡ�ߴ���ͼƬ
	ZoomImageFactor((*ho_ImageOut), &(*ho_ImageOut), 0.5, 0.5, "constant");
	//������Ч����
	(*hv_ValidLength) = 2423;
	GenRectangle1(&(*ho_Rectangle_select), 0, 0, (*hv_ValidLength) - 1, (*hv_ImWidth));
	ReduceDomain((*ho_ImageOut), (*ho_Rectangle_select), &(*ho_Image_se));
	CropDomainRel((*ho_Image_se), &(*ho_Image_se), 0, 0, 0, 0);
	GetImageSize((*ho_Image_se), &(*hv_NewImWidth), &(*hv_NewImHeight));
	MedianImage((*ho_Image_se), &(*ho_ImageMedian), "circle", 12, "mirrored");
	//��ֵ
	(*hv_thre_value) = 20;
	Threshold((*ho_ImageMedian), &(*ho_Region), (*hv_thre_value), 256);
	//**ȥ������Ȥ���������ĸ�������
	OpeningRectangle1((*ho_Region), &(*ho_RegionOpening), 150, 150);
	Connection((*ho_RegionOpening), &(*ho_Regions));
	//***
	RegionFeatures((*ho_Regions), "area", &(*hv_Value));
	SelectShape((*ho_Regions), &(*ho_Regionselect), "area", "and", hv_RegionMin, hv_RegionMax);
	CountObj((*ho_Regionselect), &(*hv_region_number));

	switch ((*hv_region_number).I())
	{

	case 0:

		(*hv_PelletExist) = 0;
		break;

	case 1:

		AllEdges((*ho_Regionselect), (*ho_ImageMedian), (*hv_NewImWidth), (*hv_NewImHeight),
			&(*hv_RtCenterCol), &(*hv_SmallRtWidth), &(*hv_ColEdge1), &(*hv_ColEdge2),
			&(*hv_ColEdge3), &(*hv_ColEdge4), &(*hv_RowAll), &hv_RtCenterRow, &hv_RtWidth);

		(*hv_W1) = (*hv_ColEdge2) - (*hv_ColEdge1);
		(*hv_W2) = (*hv_ColEdge4) - (*hv_ColEdge3);
		(*hv_L1) = (*hv_ColEdge3) - (*hv_ColEdge2);
		(*hv_L2) = (*hv_ColEdge4) - (*hv_ColEdge1);
		//��װ���ӣ��޳��쳣ֵ
		TruePoints((*hv_L1), (*hv_L2), (*hv_W1), (*hv_W2), &(*hv_NewL2), &(*hv_NewW1),
			&(*hv_NewW2), &(*hv_IndexError));

		//*****����൹��
		(*hv_Dist) = (*hv_NewW1);
		find_Length1((*hv_Dist), &(*hv_Mean_length));
		(*hv_PIXELwidth_L) = (*hv_Mean_length);
		(*hv_ChamWidthR) = (*hv_Mean_length) * hv_PixelSize;
		//*****���Ҳ൹��
		(*hv_Dist) = (*hv_NewW2);
		find_Length1((*hv_Dist), &(*hv_Mean_length));
		(*hv_PIXELwidth_R) = (*hv_Mean_length);
		(*hv_ChamWidthL) = (*hv_Mean_length) * hv_PixelSize;
		//*****����Ʒ����
		(*hv_Dist) = (*hv_NewL2);
		find_Length1((*hv_Dist), &(*hv_Mean_length));
		(*hv_PelletLength) = (*hv_Mean_length) * hv_PixelSize;


		TupleRemove((*hv_ColEdge1), (*hv_IndexError), &(*hv_ColEdge1));
		TupleRemove((*hv_ColEdge2), (*hv_IndexError), &(*hv_ColEdge2));
		TupleRemove((*hv_ColEdge3), (*hv_IndexError), &(*hv_ColEdge3));
		TupleRemove((*hv_ColEdge4), (*hv_IndexError), &(*hv_ColEdge4));
		TupleRemove((*hv_RowAll), (*hv_IndexError), &(*hv_RowAll));
		TupleGenConst((*hv_ColEdge1).TupleLength(), (*hv_SmallRtWidth), &(*hv_ROI_width));
		TupleGenConst((*hv_ColEdge1).TupleLength(), (*hv_RtCenterCol), &(*hv_Column_re_T));
		(*hv_Col_e1) = (*hv_Column_re_T) - ((*hv_ROI_width) - (*hv_ColEdge1));
		(*hv_Col_e2) = (*hv_Column_re_T) - ((*hv_ROI_width) - (*hv_ColEdge2));
		(*hv_Col_e3) = (*hv_Column_re_T) - ((*hv_ROI_width) - (*hv_ColEdge3));
		(*hv_Col_e4) = (*hv_Column_re_T) - ((*hv_ROI_width) - (*hv_ColEdge4));

		GenEmptyObj(&(*ho_Cross));
		GenCrossContourXld(&(*ho_Cross1), (*hv_RowAll), (*hv_Col_e1), 16, 0.785398);
		GenCrossContourXld(&(*ho_Cross2), (*hv_RowAll), (*hv_Col_e2), 16, 0.785398);
		GenCrossContourXld(&(*ho_Cross3), (*hv_RowAll), (*hv_Col_e3), 16, 0.785398);
		GenCrossContourXld(&(*ho_Cross4), (*hv_RowAll), (*hv_Col_e4), 16, 0.785398);
		ConcatObj((*ho_Cross), (*ho_Cross1), &(*ho_Cross));
		ConcatObj((*ho_Cross), (*ho_Cross2), &(*ho_Cross));
		ConcatObj((*ho_Cross), (*ho_Cross3), &(*ho_Cross));
		ConcatObj((*ho_Cross), (*ho_Cross4), &(*ho_Cross));


		// stop(...); only in hdevelop
		//����ԭʼ����λ�ã��д�����
		if (HDevWindowStack::IsOpen())
		{
			HalconCpp::CloseWindow(HDevWindowStack::Pop());
			//CloseWindow();
		}
			
		//*�ߴ������-----------------------------------------------------------------------
		//***���Ƽ��-----------------------------------------------------------------------
		//***��ȡ���Ƽ��ͼ��------------------------------------------------------------------------
		//������Ŀ
		(*hv_CracksNum) = 0;
		//���Ƴ���
		(*hv_CracksLength) = HTuple();
		//�����ܳ���
		(*hv_CracksLengthsSum) = 0;
		//��ɭ�����������ߵ������        ********** ����
		(*hv_LineMaxWidth) = 50;
		ho_Image = (*ho_ImgOri);
		//    StartX := (rec_center_Row-rec_width/2+PIXELwidth_L)*2
		//    EndX   := (rec_center_Row+rec_width/2-PIXELwidth_R)*2

		//������ʾ���ƴ�����,ԭͼ���0.5��
		ZoomImageFactor((*ho_ImageOut), &(*ho_CrackShow), hv_n, hv_n, "constant");
		//    WidthCrack:=Im_Width
		(*hv_HeightCrack) = (*hv_ValidLength);
		//������ʾͼ���ʼ��(������ʱ������ʾ) 2423����ֱ��5.5��Ʒһ�ܣ�ʵ�ʴ���ʱ��������Ʒʵ��ֱ�������仯
		GenRectangle1(&(*ho_ShowRectangle), 0, 0, (*hv_HeightCrack), hv_WidthCrack);
		ReduceDomain((*ho_CrackShow), (*ho_ShowRectangle), &(*ho_ImageShow));
		CropDomainRel((*ho_ImageShow), &(*ho_ImageShow), 0, 0, 0, 0);

		findRegionCrack2((*ho_ImageOut), &(*ho_CrackBinImage), &(*ho_CrackRegions), (*hv_StartX),
			(*hv_EndX), hv_n, &(*hv_CrackRegionsNumber));

		if (HDevWindowStack::IsOpen())
			ClearWindow(HDevWindowStack::GetActive());
		if (HDevWindowStack::IsOpen())
			DispObj((*ho_ImageOut), HDevWindowStack::GetActive());
		if (HDevWindowStack::IsOpen())
			DispObj((*ho_CrackRegions), HDevWindowStack::GetActive());

		if (0 != ((*hv_CrackRegionsNumber) > 0))
		{
			//*******������ͼ��������ָ����������ͼ�񣬱������Ʊ����·ָ�
			ValidRegion((*ho_CrackBinImage), &(*ho_ValidRecRegion), (*hv_HeightCrack));
			ReduceDomain((*ho_CrackBinImage), (*ho_ValidRecRegion), &(*ho_CrackBinImage)
			);
			ReduceDomain((*ho_CrackShow), (*ho_ValidRecRegion), &(*ho_ImageShow));
			CropDomainRel((*ho_CrackBinImage), &(*ho_CrackBinImage), 0, 0, 0, 0);
			CropDomainRel((*ho_ImageShow), &(*ho_ImageShow), 0, 0, 0, 0);
			Threshold((*ho_CrackBinImage), &(*ho_Region2), 128, 255);
			RegionFeatures((*ho_Region2), "area", &(*hv_CracksArea));
			if (0 != ((*hv_CracksArea) > 2000))
			{
				//**��ɭ������ȡ������
				(*hv_MaxLineWidth) = (*hv_LineMaxWidth);
				(*hv_Contrast) = 100;
				calculate_lines_gauss_parameters((*hv_MaxLineWidth), (*hv_Contrast), &(*hv_Sigma),
					&(*hv_Low), &(*hv_High));
				LinesGauss((*ho_CrackBinImage), &(*ho_CrackCenters), (*hv_Sigma), (*hv_Low),
					(*hv_High), "light", "false", "parabolic", "true");
				//**�������߽������ӣ�ѡ�񣬵õ�ʵ�ʵ�����
				SelectContoursXld((*ho_CrackCenters), &(*ho_SelectCenters), "contour_length",
					30, 2000000, -0.5, 0.5);
				UnionCollinearContoursXld((*ho_SelectCenters), &(*ho_UnionCenters), 60, 2,
					15, 0.3, "attr_keep");
				UnionAdjacentContoursXld((*ho_UnionCenters), &(*ho_Unionlights), 25, 1, "attr_keep");
				SelectContoursXld((*ho_Unionlights), &(*ho_CrackResult), "contour_length",
					70, 20000, -0.5, 0.5);
			}
			else
			{
				GenEmptyObj(&(*ho_CrackResult));
			}
		}
		else
		{
			GenEmptyObj(&(*ho_CrackResult));
		}

		LengthXld((*ho_CrackResult), &(*hv_CracksLengths));

		//��ʱ��ƷΪ����Ʒ
		if (0 != (((*hv_CracksLengths).TupleSum()) < 200))
		{
			GenEmptyObj(&(*ho_result_crack));
			(*hv_CracksNum) = 0;
			(*hv_CracksLength) = HTuple();
			(*hv_CracksLengthsSum) = 0;
		}

		(*hv_CracksNum) = (*hv_CracksLengths).TupleLength();
		(*hv_CracksLength) = (*hv_CracksLengths);
		(*hv_CracksLengthsSum) = (*hv_CracksLengths).TupleSum();


		// stop(...); only in hdevelop
		// stop(...); only in hdevelop
		//����ԭʼ����λ�ã��д�����
		if (HDevWindowStack::IsOpen())
		{
			HalconCpp::CloseWindow(HDevWindowStack::Pop());
			//CloseWindow();
		}

		break;

	default:

		(*hv_Pellet_exist) = 1;
		(*hv_NG) = 1;
		break;

	}
	return;
}

void SS_DJ_Measure::TruePoints(HTuple hv_L1, HTuple hv_L2, HTuple hv_W1, HTuple hv_W2, HTuple* hv_NewL2, HTuple* hv_NewW1, HTuple* hv_NewW2, HTuple* hv_IndexError)
{
	// Local iconic variables

  // Local control variables
	HTuple  hv_SortL1, hv_SelectL1, hv_NeWTuple, hv_SortTuple;
	HTuple  hv_RoundTuple, hv_UniqTuple, hv_TypeNum, hv_EachRates;
	HTuple  hv_TupleNum, hv_EachIndexs, hv_OriNumMode1, hv_Mode1;
	HTuple  hv_Index_down, hv_Index_up, hv_NumMode1, hv_NumMode2;
	HTuple  hv_Mode2, hv_OriNumMode2, hv_ModeD;

	//L1������Ʒȥ�����������ĳ���
	//**********��L1������*************8
	//��Ʒȱ�ݲ��ֳ��Ƚ�С��ȡ����֮����Ϊ���ų�ȱ�ݵĸ��š�
	TupleSort(hv_L1, &hv_SortL1);
	//ѡ���������n����Ԫ���β������Ԫ��
	TupleLastN(hv_SortL1, (hv_SortL1.TupleLength()) / 3, &hv_SelectL1);
	//����ֵ��С����֮һ����С����������
	hv_SelectL1 = hv_SelectL1 / 2.0;

	hv_NeWTuple = hv_SelectL1;
	//���ܣ����������Ƶ�Σ�������Ƶ�δӴ�С�������
	TupleRates(hv_NeWTuple, &hv_SortTuple, &hv_RoundTuple, &hv_UniqTuple, &hv_TypeNum,
		&hv_EachRates, &hv_TupleNum, &hv_EachIndexs);

	//�����Ƶ��
	hv_OriNumMode1 = ((const HTuple&)hv_EachRates)[0];
	//���Ƶ�ζ�Ӧ����ֵ
	hv_Mode1 = ((const HTuple&)hv_UniqTuple)[0];
	//�ֱ��ҳ����Ƶ������Ӧ��ֵ�Ӽ�1��������ֵ��Ӧ������
	TupleFind(hv_RoundTuple, hv_Mode1 - 1, &hv_Index_down);
	TupleFind(hv_RoundTuple, hv_Mode1 + 1, &hv_Index_up);
	if (0 != (hv_Index_down == -1))
	{
		hv_Index_down = HTuple();
	}
	if (0 != (hv_Index_up == -1))
	{
		hv_Index_up = HTuple();
	}
	//��������ֵ���ֵ���Ƶ��
	hv_NumMode1 = (hv_OriNumMode1 + (hv_Index_down.TupleLength())) + (hv_Index_up.TupleLength());
	//�����Ƶ�δ��ڲ�������������֮����ֱ���϶�����
	if (0 != (hv_NumMode1 > (((hv_RoundTuple.TupleLength()) * 2) / 3)))
	{
		hv_NumMode2 = 0;
		hv_Mode2 = 0;
	}
	else
	{
		//�����Ƶ��С�ڲ�������������֮�������ж��β���
		//�ҳ��ڶ����Ƶ��
		hv_OriNumMode2 = ((const HTuple&)hv_EachRates)[1];
		//��ڶ����Ƶ�ζ�Ӧ����ֵ
		hv_Mode2 = ((const HTuple&)hv_UniqTuple)[1];
		//�ֱ��ҳ��ڶ����Ƶ������Ӧ��ֵ�Ӽ�1��������ֵ��Ӧ������
		TupleFind(hv_RoundTuple, hv_Mode2 - 1, &hv_Index_down);
		TupleFind(hv_RoundTuple, hv_Mode2 + 1, &hv_Index_up);
		if (0 != (hv_Index_down == -1))
		{
			hv_Index_down = HTuple();
		}
		if (0 != (hv_Index_up == -1))
		{
			hv_Index_up = HTuple();
		}
		//��������ֵ���ֵ���Ƶ��
		hv_NumMode2 = (hv_OriNumMode2 + (hv_Index_down.TupleLength())) + (hv_Index_up.TupleLength());
	}

	//������������ֵ
	hv_ModeD = 0;
	//����������������������Ƶ�κ���Դ�С�ҳ�����������
	//��������������ʱ��������1/3���С��1/3����С��1/3������������ɢ������ֵ���ΪL1ֵ����������������ʱ���������ΪL1ֵ��
	if (0 != (HTuple(HTuple(hv_NumMode2 > ((hv_RoundTuple.TupleLength()) / 3)).TupleAnd(hv_NumMode1 > ((hv_RoundTuple.TupleLength()) / 3))).TupleOr(HTuple(hv_NumMode2 < ((hv_RoundTuple.TupleLength()) / 3)).TupleAnd(hv_NumMode1 < ((hv_RoundTuple.TupleLength()) / 3)))))
	{
		//��ֵ������������Ʒ�ĳ��ȣ���2�ָ���ֵ
		hv_ModeD = ((hv_Mode1.TupleConcat(hv_Mode2)).TupleMax()) * 2;
	}
	else
	{
		//����������ֵ���͵�Ƶ��ȷ������
		if (0 != (hv_NumMode2 > hv_NumMode1))
		{
			hv_ModeD = hv_Mode2 * 2;
		}
		else
		{
			hv_ModeD = hv_Mode1 * 2;
		}
	}


	//�ҳ�������������2������
	(*hv_IndexError) = HTuple::TupleGenSequence(0, (hv_L1.TupleLength()) - 1, 1).TupleSelectMask((hv_L1.TupleGreaterElem(hv_ModeD + 2)).TupleOr(hv_L1.TupleLessElem(hv_ModeD - 2)));
	//�����ϱ��ҳ������ڸ�ԭ�������޳���Ӧֵ
	TupleRemove(hv_L2, (*hv_IndexError), &(*hv_NewL2));
	TupleRemove(hv_W1, (*hv_IndexError), &(*hv_NewW1));
	TupleRemove(hv_W2, (*hv_IndexError), &(*hv_NewW2));

	return;
}

void SS_DJ_Measure::TupleRates(HTuple hv_NeWTuple, HTuple* hv_SortTuple, HTuple* hv_RoundTuple, HTuple* hv_UniqTuple, HTuple* hv_TypeNum, HTuple* hv_EachRates, HTuple* hv_TupleNum, HTuple* hv_EachIndexs)
{
	// Local iconic variables

  // Local control variables
	HTuple  hv_each, hv_Indices;

	TupleSort(hv_NeWTuple, &(*hv_SortTuple));
	//��Ԫ��ת��Ϊ����Ԫ�顣
	TupleRound((*hv_SortTuple), &(*hv_RoundTuple));
	TupleUniq((*hv_RoundTuple), &(*hv_UniqTuple));
	(*hv_TypeNum) = (*hv_UniqTuple).TupleLength();
	//�����ض����ȵ�Ԫ�鲢��ʼ����Ԫ�ء�
	TupleGenConst((*hv_TypeNum), 0, &(*hv_EachRates));
	(*hv_TupleNum) = (*hv_RoundTuple).TupleLength();
	//*����ѭ���Ҷ�Ӧ������������������ֵ����Ƶ��
	{
		HTuple end_val9 = (*hv_TypeNum) - 1;
		HTuple step_val9 = 1;
		for (hv_each = 0; hv_each.Continue(end_val9, step_val9); hv_each += step_val9)
		{
			TupleFind((*hv_RoundTuple), HTuple((*hv_UniqTuple)[hv_each]), &(*hv_EachIndexs));
			(*hv_EachRates)[hv_each] = (*hv_EachIndexs).TupleLength();
		}
	}

	TupleSortIndex((*hv_EachRates), &hv_Indices);
	(*hv_EachRates) = HTuple((*hv_EachRates)[hv_Indices]);
	(*hv_UniqTuple) = HTuple((*hv_UniqTuple)[hv_Indices]);
	TupleInverse((*hv_EachRates), &(*hv_EachRates));
	TupleInverse((*hv_UniqTuple), &(*hv_UniqTuple));
	return;
}

void SS_DJ_Measure::ValidRegion(HalconCpp::HObject ho_CrackBinImage, HalconCpp::HObject* ho_ValidRecRegion, HTuple hv_HeightCrack)
{
	// Local iconic variables
	HalconCpp::HObject  ho_Rectangle;

	// Local control variables
	HTuple  hv_WidthCrack1, hv_HeightCrack1, hv_k_up;
	HTuple  hv_k_down, hv_nk, hv_Row_n1, hv_Row_n2, hv_Column_n;
	HTuple  hv_MeasureHandle1, hv_GrayValues1, hv_Sum1, hv_MeasureHandle2;
	HTuple  hv_GrayValues2, hv_Sum2, hv_BeginRow, hv_EndRow;
	HTuple  hv_b_up, hv_b_down;

	GetImageSize(ho_CrackBinImage, &hv_WidthCrack1, &hv_HeightCrack1);
	hv_k_up = HTuple();
	hv_k_down = HTuple();
	//*�ֱ���ϵ��ºʹ��µ��ϣ�����11�����Ϊ80��С����
	//*���С�����а��������������עΪ1������Ϊ0��
	//*�ж������Ƿ���Ϊ�ɼ�ԭ�򣬷�Ϊ���������֡�
	for (hv_nk = 0; hv_nk <= 10; hv_nk += 1)
	{
		hv_Row_n1 = (81 * hv_nk) + 40;
		hv_Row_n2 = (hv_HeightCrack1 - (81 * hv_nk)) - 40;
		hv_Column_n = hv_WidthCrack1 / 2;
		//�����ϲ�������
		GenRectangle2(&ho_Rectangle, hv_Row_n1, hv_Column_n, 0, hv_Column_n, 40);
		GenMeasureRectangle2(hv_Row_n1, hv_Column_n, 0, hv_Column_n, 40, hv_WidthCrack1,
			hv_HeightCrack1, "bilinear", &hv_MeasureHandle1);
		MeasureProjection(ho_CrackBinImage, hv_MeasureHandle1, &hv_GrayValues1);
		TupleSum(hv_GrayValues1, &hv_Sum1);
		if (0 != (hv_Sum1 > 0))
		{
			hv_k_up = hv_k_up.TupleConcat(1);
		}
		else
		{
			hv_k_up = hv_k_up.TupleConcat(0);
		}
		//�����²�������
		GenRectangle2(&ho_Rectangle, hv_Row_n2, hv_Column_n, 0, hv_Column_n, 40);
		GenMeasureRectangle2(hv_Row_n2, hv_Column_n, 0, hv_Column_n, 40, hv_WidthCrack1,
			hv_HeightCrack1, "bilinear", &hv_MeasureHandle2);
		MeasureProjection(ho_CrackBinImage, hv_MeasureHandle2, &hv_GrayValues2);
		TupleSum(hv_GrayValues2, &hv_Sum2);
		if (0 != (hv_Sum2 > 0))
		{
			hv_k_down = hv_k_down.TupleConcat(1);
		}
		else
		{
			hv_k_down = hv_k_down.TupleConcat(0);
		}
		//
		CloseMeasure(hv_MeasureHandle1);
		CloseMeasure(hv_MeasureHandle2);
	}

	//*��ʼλ�ó�ʼ����2423�Ǹ���5.5��Ʒ���ĳ�ֵ��
	//*����ǰ��Ĭ��Ϊ�����������ƣ�����һ�����Ƴ���С
	//*��11�����εĸ߶ȡ�
	hv_BeginRow = 0;
	hv_EndRow = hv_HeightCrack;
	//���ϱ�11�����������ڣ���0ʱ�������С�������������ʼ������
	if (0 != ((hv_k_up.TupleSum()) < 11))
	{
		TupleFind(hv_k_up, 0, &hv_b_up);
		hv_BeginRow = HTuple(hv_b_up[0]) * 81;
		hv_EndRow = hv_BeginRow + hv_HeightCrack;
	}
	//*����ϱ�11��С���ζ������ƣ���ͬ�����������±߿�ʼ���������ֹ������
	if (0 != (HTuple((hv_k_down.TupleSum()) < 11).TupleAnd((hv_k_up.TupleSum()) == 11)))
	{
		TupleFind(hv_k_down, 0, &hv_b_down);
		hv_EndRow = hv_HeightCrack - (HTuple(hv_b_down[0]) * 81);
		hv_BeginRow = hv_EndRow - hv_HeightCrack;
	}
	//*���������������������㣬�������ʼλ�ð���ʼ��ֵ���㡣

	//�ָ��Ҫ��������򼰶�Ӧ����ʾ����
	GenRectangle1(&(*ho_ValidRecRegion), hv_BeginRow, 0, hv_EndRow, hv_WidthCrack1);
	return;
}

void SS_DJ_Measure::Deal_SS_DJ(HalconCpp::HObject ho_Height, HalconCpp::HObject ho_Intensity)
{
	// Local iconic variables
	HalconCpp::HObject  ho_Region, ho_RegionClosing, ho_Regions, ho_Regionselect, ho_ImageMedian;;
	HalconCpp::HObject  ho_HeightMedian, ho_Cross;

	// Local control variables
	HTuple  hv_PelletExist, hv_ChamWidthL, hv_ChamWidthR;
	HTuple  hv_ChamPhi_L, hv_ChamPhi_R, hv_PelletLength, hv_PixelSize;
	HTuple  hv_RegionMax, hv_RegionMin, hv_WindowHandle, hv_ImWidth;
	HTuple  hv_ImHeight, hv_Rg_area, hv_region_number, hv_RtCenterCol;
	HTuple  hv_SmallRtWidth, hv_ColEdge1, hv_ColEdge2, hv_ColEdge3;
	HTuple  hv_ColEdge4, hv_RowAll, hv_RtCenterRow, hv_RtWidth;
	HTuple  hv_W1, hv_W2, hv_L1, hv_L2, hv_NewL2, hv_NewW1;
	HTuple  hv_NewW2, hv_IndexError, hv_Dist, hv_MeanV, hv_IndexError_M;
	HTuple  hv_ChamL, hv_IndexError_ChamL, hv_ChamR, hv_IndexError_ChamR;
	HTuple  hv_PelletL, hv_IndexError_PelletLg, hv_IndexErrorUnion1;
	HTuple  hv_IndexErrorUnion2, hv_ROI_width, hv_Column_re_T;
	HTuple  hv_Col_e1, hv_Col_e2, hv_Col_e3, hv_Col_e4, hv_Width_ChamL;
	HTuple  hv_Width_ChamR, hv_Col_ChamL, hv_Col_ChamR, hv_Width_Cham;
	HTuple  hv_Col_Cham, hv_HtWidth, hv_HtHeight, hv_ChamPhi;

	//˼�룺������ͼ��ȡ��Ʒ���ǣ����Ǳ�Ե����ͻ�����ԣ���һ�׵�����ȡ��
	//��V2�汾���ݽ��в��䣬���Ӳ������ǽǶȲ��֣�˼�룺���ݾ����ȡ�ĻҶ�ֵ���ֱ�ߣ�����ֱ��б��
	//��֮ǰ�汾��ȣ�ȷ����ȷ����ʱ����С����������֮��Ĳ�࣬Ҫ����ӿ���
	//���⣺��switch (region_number)����1ʱ��ô����������ᴩ��ȱ�ݣ�ȱ������������Ч����Χ����ֵ���ܳ�����Ʒ����Ϊ����


	//����ͼ���ʽת��
	//ConvertImageType(ho_Height, &ho_Height, "uint2");
	//ConvertImageType(ho_Intensity, &ho_Intensity, "uint2");

	//��������----�������
	//��Ʒ����
	hv_PelletExist = 1;
	//�Ƿ��з�
	//NG := 0
	//���ǿ��ֵ���Ƕ�ֵ
	hv_ChamWidthL = 0;
	hv_ChamWidthR = 0;
	hv_ChamPhi_L = 0;
	hv_ChamPhi_R = 0;
	//о�鳤��
	hv_PelletLength = 0;

	//��������----����ʹ�ñ���
	//ͼ������ϵ��
	//n := 0.5
	//ʵ�����ش�С�����ף� 0.015
	hv_PixelSize = 0.015;
	//��Ʒ����ѡ���С��������
	hv_RegionMax = 9999999;
	hv_RegionMin = 400000;

	//���������ļ�

	//ReadImage(&ho_Height, "C:/Users/zxuan/Desktop/0809/1.png");

	//ReadImage(&ho_Intensity, "C:/Users/zxuan/Desktop/0809/2.png");
	dev_open_window_fit_image(ho_Intensity, 0, 0, -1, -1, &hv_WindowHandle);
	//��ȡ�ߴ���ͼƬ
	//zoom_image_factor (Image, Image, 0.5, 0.5, 'constant')
	//decompose3 (Image, Height, Intensity, Timestamp)
	//Go2GenTLStamp (Timestamp, 10, zOffset)
	//zOffset := zOffset / 1000000.0
	//Go2GenTLStamp (Timestamp, 11, zResolution)
	//zResolution := zResolution / 1000000.0
	MedianImage(ho_Intensity, &ho_ImageMedian, "circle", 5, "mirrored");
	GetImageSize(ho_ImageMedian, &hv_ImWidth, &hv_ImHeight);
	Threshold(ho_ImageMedian, &ho_Region, 20, 99999);
	//**ȥ������Ȥ���������ĸ�������
	ClosingRectangle1(ho_Region, &ho_RegionClosing, 25, 25);
	Connection(ho_RegionClosing, &ho_Regions);
	RegionFeatures(ho_Regions, "area", &hv_Rg_area);
	//***
	SelectShape(ho_Regions, &ho_Regionselect, "area", "and", hv_RegionMin, hv_RegionMax);
	CountObj(ho_Regionselect, &hv_region_number);

	switch (hv_region_number.I())
	{

	case 0:

		hv_PelletExist = 0;
		break;

	case 1:
		//ѭ�����ҵ���Ʒ�ĸ��߽�����λ�ã�С�������߽�Ϊ���ԭ�㣩
		AllEdges(ho_Regionselect, ho_ImageMedian, hv_ImWidth, hv_ImHeight, &hv_RtCenterCol,
			&hv_SmallRtWidth, &hv_ColEdge1, &hv_ColEdge2, &hv_ColEdge3, &hv_ColEdge4,
			&hv_RowAll, &hv_RtCenterRow, &hv_RtWidth);
		//�ĸ��߽���룬W�����ǿ�ȣ�L�������
		hv_W1 = hv_ColEdge2 - hv_ColEdge1;
		hv_W2 = hv_ColEdge4 - hv_ColEdge3;
		hv_L1 = hv_ColEdge3 - hv_ColEdge2;
		hv_L2 = hv_ColEdge4 - hv_ColEdge1;
		//��װ���ӣ����ݵ��Ǽ����L1�����������޳��쳣ֵ����� NewL2, NewW1, NewW2
		TruePoints(hv_L1, hv_L2, hv_W1, hv_W2, &hv_NewL2, &hv_NewW1, &hv_NewW2, &hv_IndexError);
		//����൹�ǿ�� ChamWidthL
		hv_Dist = hv_NewW1;
		MeanValue(hv_Dist, &hv_MeanV, &hv_IndexError_M);
		hv_ChamL = hv_MeanV;
		hv_ChamWidthL = hv_ChamL * hv_PixelSize;
		hv_IndexError_ChamL = hv_IndexError_M;
		//���Ҳ൹�ǿ�� ChamWidthR
		hv_Dist = hv_NewW2;
		MeanValue(hv_Dist, &hv_MeanV, &hv_IndexError_M);
		hv_ChamR = hv_MeanV;
		hv_ChamWidthR = hv_ChamR * hv_PixelSize;
		hv_IndexError_ChamR = hv_IndexError_M;
		//����Ʒ����   PelletLength
		hv_Dist = hv_NewL2;
		MeanValue(hv_Dist, &hv_MeanV, &hv_IndexError_M);
		hv_PelletL = hv_MeanV;
		hv_PelletLength = hv_PelletL * hv_PixelSize;
		hv_IndexError_PelletLg = hv_IndexError_M;

		//���㵹�����꣨ע�⣺IndexError��IndexErrorUnion2�ǲ�ͬԪ�������
		TupleUnion(hv_IndexError_ChamL, hv_IndexError_ChamR, &hv_IndexErrorUnion1);
		TupleUnion(hv_IndexErrorUnion1, hv_IndexError_PelletLg, &hv_IndexErrorUnion2);
		TupleRemove(hv_ColEdge1, hv_IndexError, &hv_ColEdge1);
		TupleRemove(hv_ColEdge2, hv_IndexError, &hv_ColEdge2);
		TupleRemove(hv_ColEdge3, hv_IndexError, &hv_ColEdge3);
		TupleRemove(hv_ColEdge4, hv_IndexError, &hv_ColEdge4);
		TupleRemove(hv_RowAll, hv_IndexError, &hv_RowAll);
		TupleRemove(hv_ColEdge1, hv_IndexErrorUnion2, &hv_ColEdge1);
		TupleRemove(hv_ColEdge2, hv_IndexErrorUnion2, &hv_ColEdge2);
		TupleRemove(hv_ColEdge3, hv_IndexErrorUnion2, &hv_ColEdge3);
		TupleRemove(hv_ColEdge4, hv_IndexErrorUnion2, &hv_ColEdge4);
		TupleRemove(hv_RowAll, hv_IndexErrorUnion2, &hv_RowAll);
		TupleGenConst(hv_ColEdge1.TupleLength(), hv_SmallRtWidth, &hv_ROI_width);
		TupleGenConst(hv_ColEdge1.TupleLength(), hv_RtCenterCol, &hv_Column_re_T);
		hv_Col_e1 = hv_Column_re_T - (hv_ROI_width - hv_ColEdge1);
		hv_Col_e2 = hv_Column_re_T - (hv_ROI_width - hv_ColEdge2);
		hv_Col_e3 = hv_Column_re_T - (hv_ROI_width - hv_ColEdge3);
		hv_Col_e4 = hv_Column_re_T - (hv_ROI_width - hv_ColEdge4);

		//���ڵ��ǽǶȼ��㣬�󵹽ǿ�� Width_ChamL ;�ҵ��ǿ�� Width_ChamR   (��ʱ��ȥ��ȫ������������
		hv_Width_ChamL = hv_Col_e2 - hv_Col_e1;
		hv_Width_ChamR = hv_Col_e4 - hv_Col_e3;
		hv_Col_ChamL = (hv_Col_e2 + hv_Col_e1) / 2;
		hv_Col_ChamR = (hv_Col_e4 + hv_Col_e3) / 2;

		//***********���ǽǶȼ���*******************
		//�����󵹽ǽǶ�
		//���ǿ��
		hv_Width_Cham = hv_Width_ChamL;
		//���ɲ���������ĵ�������
		hv_Col_Cham = hv_Col_ChamL;
		//ת��ͼ�����ͣ���ͼ��߶���Ϣת����ʵ�ʵĺ��׵�λֵ
		//convert_image_type (Height, HeightMapReal, 'real')
		//scale_image (HeightMapReal, HeightMapReal, zResolution, zOffset)
		MedianImage(ho_Height, &ho_HeightMedian, "circle", 5, "mirrored");
		GetImageSize(ho_HeightMedian, &hv_HtWidth, &hv_HtHeight);
		//�����󵹽ǽǶ�  ChamPhi_L
		ChamferPhi(ho_HeightMedian, hv_HtHeight, hv_Width_Cham, hv_RowAll, hv_Col_Cham,
			hv_HtWidth, hv_PixelSize, &hv_ChamPhi);
		hv_ChamPhi_L = hv_ChamPhi;

		//�����ҵ��ǽǶ�
		//���ǿ��
		hv_Width_Cham = hv_Width_ChamR;
		//���ɲ���������ĵ�������
		hv_Col_Cham = hv_Col_ChamR;
		//�����ҵ��ǽǶ�  ChamPhi_R
		ChamferPhi(ho_HeightMedian, hv_HtHeight, hv_Width_Cham, hv_RowAll, hv_Col_Cham,
			hv_HtWidth, hv_PixelSize, &hv_ChamPhi);
		hv_ChamPhi_R = hv_ChamPhi;

		//MessageBox((LPCTSTR)1);
		//MessageBox((LPCTSTR)hv_ChamPhi_L.I());

		//printf("�󵹽ǿ��Ϊ��%d", hv_ChamPhi_L.I());
		//printf("�ҵ��ǿ��Ϊ��%d", hv_ChamPhi_R.I());

		CString strTempL, strTempR,LTempNum, RTempNum;
		float LTemp,RTemp;
		LTemp = (float)hv_ChamPhi_L.D();
		RTemp = (float)hv_ChamPhi_R.D();

		strTempL.Insert(0, L"�󵹽ǵĽǶ�Ϊ��");
		strTempR.Insert(0, L"�ҵ��ǵĽǶ�Ϊ��");

		LTempNum.Format(_T("%4.4f"), LTemp);
		RTempNum.Format(_T("%4.4f"), RTemp);

		strTempL += LTempNum;
		strTempR += RTempNum;
		
		mg_dlg->WriteLog(strTempL);
		mg_dlg->WriteLog(strTempR);
		//mg_dlg->ShowImg(4, &ho_HeightMedian);
		//mg_dlg->displaySend(ho_HeightMedian);

		

		//***************��ʾ************
		//dev_open_window_fit_image(ho_ImageMedian, 0, 0, -1, -1, &hv_WindowHandle);
		if (HDevWindowStack::IsOpen())
			DispObj(ho_Intensity, HDevWindowStack::GetActive());
		if (HDevWindowStack::IsOpen())
			SetColor(HDevWindowStack::GetActive(), "red");
		GenCrossContourXld(&ho_Cross, hv_RowAll, hv_Col_e1, 6, 0.785398);
		GenCrossContourXld(&ho_Cross, hv_RowAll, hv_Col_e2, 6, 0.785398);
		GenCrossContourXld(&ho_Cross, hv_RowAll, hv_Col_e3, 6, 0.785398);
		GenCrossContourXld(&ho_Cross, hv_RowAll, hv_Col_e4, 6, 0.785398);

		mg_dlg->ShowImg(4, &ho_Intensity);
		mg_dlg->displaySend(ho_Intensity);
		mg_dlg->displayImgInfo(hv_ChamPhi_L, hv_ChamPhi_R);

		break;
	
	}

	//����ǰ�ļ����и���Ʒ��Ӧ�ߴ�����������һ�𣬱����ظ��Լ���
	//ChamWidthL_all[m2] := ChamWidthL
	//ChamWidthR_all[m2] := ChamWidthR
	//PelletLength_all[m2] := PelletLength
	//ChamPhi_L_all[m2] := ChamPhi_L
	//ChamPhi_R_all[m2] := ChamPhi_R
	//dump_window_image (Image1, WindowHandle)
	//write_image (Image1, 'png', 0, 'C:/Users/Administrator/Desktop/69-0-8/'+m2)
	//* endfor 
	//
	//�ߴ������**********
}
