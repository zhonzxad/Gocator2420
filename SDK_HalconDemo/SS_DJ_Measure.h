#pragma once

#include <HALCONCpp/HalconCpp.h>
#include <HALCONCpp/HDevThread.h>
#include <afxwin.h>
#include <algorithm>
#include <chrono>

using namespace HalconCpp;

class SS_DJ_Measure : public CDialogEx //public CDialogEx
{
	// 实现
protected:

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();

public:
	void calculate_lines_gauss_parameters(HTuple hv_MaxLineWidth, HTuple hv_Contrast,
		HTuple* hv_Sigma, HTuple* hv_Low, HTuple* hv_High);

	// Chapter: Develop
	// Short Description: Open a new graphics window that preserves the aspect ratio of the given image. 
	void dev_open_window_fit_image(HalconCpp::HObject ho_Image, HTuple hv_Row, HTuple hv_Column,
		HTuple hv_WidthLimit, HTuple hv_HeightLimit, HTuple* hv_WindowHandle);

	// Local procedures 
	void AllEdges(HalconCpp::HObject ho_Regionselect, HalconCpp::HObject ho_ImageMedian, HTuple hv_ImWidth,
		HTuple hv_ImHeight, HTuple* hv_RtCenterCol, HTuple* hv_SmallRtWidth, HTuple* hv_ColEdge1,
		HTuple* hv_ColEdge2, HTuple* hv_ColEdge3, HTuple* hv_ColEdge4, HTuple* hv_RowAll,
		HTuple* hv_RtCenterRow, HTuple* hv_RtWidth);

	void ChamferPhi(HalconCpp::HObject ho_HeightMedian, HTuple hv_HtHeight, HTuple hv_Width_Cham,
		HTuple hv_RowAll, HTuple hv_Col_Cham, HTuple hv_HtWidth, HTuple hv_PixelSize,
		HTuple* hv_ChamPhi);

	void cir_select(HalconCpp::HObject ho_Selected_maxDis, HalconCpp::HObject* ho_Selected_cir);

	void ComputerTh(HalconCpp::HObject ho_RtRoi, HalconCpp::HObject ho_ImageMean, HTuple* hv_nTh);

	void dside(HalconCpp::HObject ho_Selected_cir, HalconCpp::HObject* ho_result_crack, HTuple hv_width_crack,
		HTuple hv_n, HTuple hv_dis_crack);

	void find_crack_region(HalconCpp::HObject ho_Image_crack_show, HalconCpp::HObject* ho_re_crack, HTuple hv_height_crack,
		HTuple hv_width_crack, HTuple hv_Mean_length, HTuple* hv_kk);

	void find_edges(HalconCpp::HObject ho_ImageMedian, HTuple hv_S_rec_number, HTuple hv_rec_Column,
		HTuple hv_S_rec_length, HTuple hv_S_rec_height, HTuple hv_Im_Width, HTuple hv_Im_Height,
		HTuple* hv_Col_edge1, HTuple* hv_Col_edge2, HTuple* hv_Col_edge3, HTuple* hv_Col_edge4,
		HTuple* hv_re_Row);

	void find_edges1(HalconCpp::HObject ho_ImageMedian, HalconCpp::HObject* ho_Rectangle, HTuple hv_S_rec_number,
		HTuple hv_rec_center_Row, HTuple hv_S_rec_length, HTuple hv_S_rec_height, HTuple hv_Im_Width,
		HTuple hv_Im_Height, HTuple* hv_Col_edge1, HTuple* hv_Col_edge2, HTuple* hv_Col_edge3,
		HTuple* hv_Col_edge4, HTuple* hv_re_Row);

	void find_Length(HTuple hv_Distance, HTuple* hv_Mean_length);

	void find_Length1(HTuple hv_Dist, HTuple* hv_Mean_length);

	void find_mode(HTuple hv_Distance, HTuple* hv_mode_Distance);

	void find_mode2(HTuple hv_Dist, HTuple* hv_re_num, HTuple* hv_uniq_Dist, HTuple* hv_sort_Dist,
		HTuple* hv_round_Dist, HTuple* hv_type_num, HTuple* hv_round_num, HTuple* hv_w,
		HTuple* hv_index_w);

	void find_tm(HTuple hv_Distance, HTuple* hv_mode_Distance);

	void find_tm1(HTuple hv_Dist, HTuple* hv_mode_Dist);

	void find_tp(HTuple hv_Distance_L1, HTuple hv_Distance_L2, HTuple hv_Distance_w1,
		HTuple hv_Distance_w2, HTuple* hv_Distance_L2Out, HTuple* hv_Distance_w1Out,
		HTuple* hv_Distance_w2Out, HTuple* hv_Index1);

	void find_tp1(HTuple hv_L2_Dist, HTuple hv_w1_Dist, HTuple hv_w2_Dist, HTuple hv_L1_Dist,
		HTuple* hv_L2out_Dist, HTuple* hv_w1out_Dist, HTuple* hv_w2out_Dist, HTuple* hv_Index1);

	void findCrackRegion2(HalconCpp::HObject ho_Image, HalconCpp::HObject* ho_RtRoi, HalconCpp::HObject* ho_Boundary,
		HalconCpp::HObject* ho_ImageMean, HalconCpp::HObject* ho_DarkRegion, HalconCpp::HObject* ho_RegionIntersection,
		HalconCpp::HObject* ho_DarkRegionConn, HalconCpp::HObject* ho_SelectedRegions, HalconCpp::HObject* ho_SelectedRegionsUnion,
		HalconCpp::HObject* ho_RegionClosing1, HalconCpp::HObject* ho_RegionClosing1Conn, HalconCpp::HObject* ho_SelectedRegions2,
		HalconCpp::HObject* ho_CrackRegions, HalconCpp::HObject* ho_ObjectSelected, HalconCpp::HObject* ho_RtObjectSelected,
		HalconCpp::HObject* ho_RegionDilation, HalconCpp::HObject* ho_RegionIntersection2, HalconCpp::HObject* ho_ImageMeanReduced,
		HalconCpp::HObject* ho_RtObjectSelectedDarkRegion, HalconCpp::HObject* ho_RtObjectSelectedDarkRegionClosing,
		HalconCpp::HObject* ho_RtObjectSelectedDarkRegionOpening, HalconCpp::HObject* ho_RtObjectSelectedDarkRegionOpeningConn,
		HalconCpp::HObject* ho_SelectedRegions3, HalconCpp::HObject* ho_SelectedRegions3Union, HalconCpp::HObject* ho_RegionClosing3,
		HalconCpp::HObject* ho_RegionOpening3, HalconCpp::HObject* ho_Rt1, HalconCpp::HObject* ho_Rt2, HTuple hv_StartX,
		HTuple hv_EndX, HTuple hv_Border, HTuple* hv_Width, HTuple* hv_Height, HTuple* hv_Mean,
		HTuple* hv_Deviation, HTuple* hv_nTh, HTuple* hv_Number1, HTuple* hv_ii, HTuple* hv_Row1,
		HTuple* hv_Column1, HTuple* hv_Row2, HTuple* hv_Column2, HTuple* hv_Mean1, HTuple* hv_Deviation1,
		HTuple* hv_Row, HTuple* hv_Column, HTuple* hv_Radius, HTuple* hv_nW, HTuple* hv_Row11,
		HTuple* hv_Column11, HTuple* hv_Row21, HTuple* hv_Column21, HTuple* hv_MeanRt1,
		HTuple* hv_DeviationRt1, HTuple* hv_MeanRt2, HTuple* hv_DeviationRt2);

	void findRegionCrack2(HalconCpp::HObject ho_Image, HalconCpp::HObject* ho_CrackBinImage, HalconCpp::HObject* ho_CrackRegions,
		HTuple hv_StartX, HTuple hv_EndX, HTuple hv_n, HTuple* hv_CrackRegionsNumber);

	void getcrack(HalconCpp::HObject ho_BinImage_crack, HalconCpp::HObject* ho_result_crack, HTuple hv_n,
		HTuple hv_Line_MaxWidth, HTuple hv_width_crack);

	void Go2GenTL_ParseData(HalconCpp::HObject ho_Image, HalconCpp::HObject* ho_HeightMap, HalconCpp::HObject* ho_Intensity,
		HTuple hv_Index, HTuple* hv_FrameCount, HTuple* hv_Timestamp, HTuple* hv_EncoderPosition,
		HTuple* hv_EncoderIndex, HTuple* hv_Inputs, HTuple* hv_xOffset, HTuple* hv_xResolution,
		HTuple* hv_yOffset, HTuple* hv_yResolution, HTuple* hv_zOffset, HTuple* hv_zResolution,
		HTuple* hv_Width, HTuple* hv_Length, HTuple* hv_HasIntensity, HTuple* hv_NumScans);
	void Go2GenTLStamp(HalconCpp::HObject ho_Stamps, HTuple hv_Index, HTuple* hv_Value);

	void image_deal(HalconCpp::HObject ho_BinImage_crack, HTuple hv_width_crack, HTuple hv_n, HTuple hv_height_crack,
		HTuple hv_high_image, HTuple* hv_begin_row, HTuple* hv_end_row);

	void max_distance(HalconCpp::HObject ho_Selected_Length, HalconCpp::HObject* ho_Selected_maxDis);

	void MeanValue(HTuple hv_Dist, HTuple* hv_MeanV, HTuple* hv_IndexError_M);

	void PelletEdges(HalconCpp::HObject ho_Image, HalconCpp::HObject* ho_ImageOut, HalconCpp::HObject* ho_Rectangle_select,
		HalconCpp::HObject* ho_Image_se, HalconCpp::HObject* ho_ImageMedian, HalconCpp::HObject* ho_Region, HalconCpp::HObject* ho_RegionOpening,
		HalconCpp::HObject* ho_Regions, HalconCpp::HObject* ho_Regionselect, HalconCpp::HObject* ho_Cross, HalconCpp::HObject* ho_Cross1,
		HalconCpp::HObject* ho_Cross2, HalconCpp::HObject* ho_Cross3, HalconCpp::HObject* ho_Cross4, HTuple hv_ImWidth,
		HTuple hv_RegionMin, HTuple hv_RegionMax, HTuple hv_PixelSize, HTuple* hv_ValidLength,
		HTuple* hv_NewImWidth, HTuple* hv_NewImHeight, HTuple* hv_thre_value, HTuple* hv_Value,
		HTuple* hv_region_number, HTuple* hv_PelletExist, HTuple* hv_RtCenterCol, HTuple* hv_SmallRtWidth,
		HTuple* hv_ColEdge1, HTuple* hv_ColEdge2, HTuple* hv_ColEdge3, HTuple* hv_ColEdge4,
		HTuple* hv_RowAll, HTuple* hv_W1, HTuple* hv_W2, HTuple* hv_L1, HTuple* hv_L2,
		HTuple* hv_NewL2, HTuple* hv_NewW1, HTuple* hv_NewW2, HTuple* hv_IndexError,
		HTuple* hv_Dist, HTuple* hv_Mean_length, HTuple* hv_PIXELwidth_L, HTuple* hv_ChamWidthR,
		HTuple* hv_PIXELwidth_R, HTuple* hv_ChamWidthL, HTuple* hv_PelletLength, HTuple* hv_ROI_width,
		HTuple* hv_Column_re_T, HTuple* hv_Col_e1, HTuple* hv_Col_e2, HTuple* hv_Col_e3,
		HTuple* hv_Col_e4);

	void Program1(HalconCpp::HObject ho_Image, HalconCpp::HObject* ho_ImgOri, HalconCpp::HObject* ho_ImageOut, HalconCpp::HObject* ho_Rectangle_select,
		HalconCpp::HObject* ho_Image_se, HalconCpp::HObject* ho_ImageMedian, HalconCpp::HObject* ho_Region, HalconCpp::HObject* ho_RegionOpening,
		HalconCpp::HObject* ho_Regions, HalconCpp::HObject* ho_Regionselect, HalconCpp::HObject* ho_Cross, HalconCpp::HObject* ho_Cross1,
		HalconCpp::HObject* ho_Cross2, HalconCpp::HObject* ho_Cross3, HalconCpp::HObject* ho_Cross4, HalconCpp::HObject* ho_CrackShow,
		HalconCpp::HObject* ho_ShowRectangle, HalconCpp::HObject* ho_ImageShow, HalconCpp::HObject* ho_CrackBinImage,
		HalconCpp::HObject* ho_CrackRegions, HalconCpp::HObject* ho_ValidRecRegion, HalconCpp::HObject* ho_Region2, HalconCpp::HObject* ho_CrackCenters,
		HalconCpp::HObject* ho_SelectCenters, HalconCpp::HObject* ho_UnionCenters, HalconCpp::HObject* ho_Unionlights,
		HalconCpp::HObject* ho_CrackResult, HalconCpp::HObject* ho_result_crack, HTuple hv_RegionMin, HTuple hv_RegionMax,
		HTuple hv_PixelSize, HTuple hv_n, HTuple hv_WidthCrack, HTuple* hv_ImWidth, HTuple* hv_ImHeight,
		HTuple* hv_ValidLength, HTuple* hv_NewImWidth, HTuple* hv_NewImHeight, HTuple* hv_thre_value,
		HTuple* hv_Value, HTuple* hv_region_number, HTuple* hv_PelletExist, HTuple* hv_RtCenterCol,
		HTuple* hv_SmallRtWidth, HTuple* hv_ColEdge1, HTuple* hv_ColEdge2, HTuple* hv_ColEdge3,
		HTuple* hv_ColEdge4, HTuple* hv_RowAll, HTuple* hv_W1, HTuple* hv_W2, HTuple* hv_L1,
		HTuple* hv_L2, HTuple* hv_NewL2, HTuple* hv_NewW1, HTuple* hv_NewW2, HTuple* hv_IndexError,
		HTuple* hv_Dist, HTuple* hv_Mean_length, HTuple* hv_PIXELwidth_L, HTuple* hv_ChamWidthR,
		HTuple* hv_PIXELwidth_R, HTuple* hv_ChamWidthL, HTuple* hv_PelletLength, HTuple* hv_ROI_width,
		HTuple* hv_Column_re_T, HTuple* hv_Col_e1, HTuple* hv_Col_e2, HTuple* hv_Col_e3,
		HTuple* hv_Col_e4, HTuple* hv_CracksNum, HTuple* hv_CracksLength, HTuple* hv_CracksLengthsSum,
		HTuple* hv_LineMaxWidth, HTuple* hv_StartX, HTuple* hv_EndX, HTuple* hv_HeightCrack,
		HTuple* hv_CrackRegionsNumber, HTuple* hv_CracksArea, HTuple* hv_MaxLineWidth,
		HTuple* hv_Contrast, HTuple* hv_Sigma, HTuple* hv_Low, HTuple* hv_High, HTuple* hv_CracksLengths,
		HTuple* hv_Pellet_exist, HTuple* hv_NG);

	void TruePoints(HTuple hv_L1, HTuple hv_L2, HTuple hv_W1, HTuple hv_W2, HTuple* hv_NewL2,
		HTuple* hv_NewW1, HTuple* hv_NewW2, HTuple* hv_IndexError);

	void TupleRates(HTuple hv_NeWTuple, HTuple* hv_SortTuple, HTuple* hv_RoundTuple,
		HTuple* hv_UniqTuple, HTuple* hv_TypeNum, HTuple* hv_EachRates, HTuple* hv_TupleNum,
		HTuple* hv_EachIndexs);

	void ValidRegion(HalconCpp::HObject ho_CrackBinImage, HalconCpp::HObject* ho_ValidRecRegion, HTuple hv_HeightCrack);

	void Deal_SS_DJ(double ZResolution,HalconCpp::HObject img_Height, HalconCpp::HObject img_Intens);

public:
	//CSDKHalconDemoDlg* mg_dlg = NULL;

	HTuple  hv_ChamPhi_L, hv_ChamPhi_R;		//左右倒角宽度
	clock_t endTime, startTime;
	clock_t DJ_JC_runTime;	//倒角检测时间
};

