// SDK_HalconDemoDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "SDK_HalconDemo.h"
#include "SDK_HalconDemoDlg.h"
#include "afxdialogex.h"
#include <algorithm>
#include <halconcpp/halconcpp.h>
#include <thread>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#pragma warning(disable:4244)
#pragma warning(disable:6283)
#pragma warning(disable:6011)
#pragma warning(disable:26495)
#pragma warning(disable:26451)

// CSDKHalconDemoDlg 对话框

#define RECEIVE_TIMEOUT			20000000
#define INVALID_RANGE_16BIT     ((signed short)0x8000)          // gocator transmits range data as 16-bit signed integers. 0x8000 signifies invalid range data. 
#define DOUBLE_MAX              ((k64f)1.7976931348623157e+308) // 64-bit double - largest positive value.  
#define INVALID_RANGE_DOUBLE    ((k64f)-DOUBLE_MAX)             // floating point value to represent invalid range data.
#define NUM_PROFILES            50

#define SENSOR_IP               "192.168.1.11"
//#define SENSOR_IP               "127.0.0.1"

#define NM_TO_MM(VALUE) (((k64f)(VALUE))/1000000.0)
#define UM_TO_MM(VALUE) (((k64f)(VALUE))/1000.0)

kStatus kCall onData(void* ctx, void* sys, void* dataset);

CSDKHalconDemoDlg* mg_dlg = NULL;
DealCamFun* m_QX_JC = NULL;
SS_DJ_Measure* m_SS_DJ = NULL;

CSDKHalconDemoDlg::DataContext contextPointer;
CFont font;

HalconCpp::HTuple  Width, Height;
HalconCpp::HTuple hv_windowhandle;

CSDKHalconDemoDlg::CSDKHalconDemoDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SDK_HALCONDEMO_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSDKHalconDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSDKHalconDemoDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_OpenCamera, &CSDKHalconDemoDlg::OnBnClickedOpencamera)
	ON_BN_CLICKED(IDC_StopCamera, &CSDKHalconDemoDlg::OnBnClickedStopcamera)
	ON_BN_CLICKED(IDC_AcquireImage, &CSDKHalconDemoDlg::OnBnClickedAcquireimage)
	ON_EN_CHANGE(IDC_SHOW_INFO, &CSDKHalconDemoDlg::OnEnChangeShowInfo)
	ON_BN_CLICKED(IDC_TEST_1, &CSDKHalconDemoDlg::OnBnClickedTest1)
	ON_BN_CLICKED(IDC_PAUSECAM, &CSDKHalconDemoDlg::OnBnClickedPausecam)
	ON_BN_CLICKED(IDC_CHANGE_PATH, &CSDKHalconDemoDlg::OnBnClickedChangePath)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CSDKHalconDemoDlg 消息处理程序

BOOL CSDKHalconDemoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标


	// TODO: 在此添加额外的初始化代码
	//::GetClientRect(GetDlgItem(IDC_IMG_HEIG)->m_hWnd, rc);
	//::GetClientRect(GetDlgItem(IDC_SHOW_IMG)->m_hWnd, rc);
	//image = new HalconCpp::HObject();
	ho_Height = new HalconCpp::HObject();
	ho_Intens = new HalconCpp::HObject();

	GetDlgItem(IDC_AcquireImage)->EnableWindow(0);
	GetDlgItem(IDC_StopCamera)->EnableWindow(0);
	GetDlgItem(IDC_CHANGE_PATH)->EnableWindow(1);
	GetDlgItem(IDC_PAUSECAM)->EnableWindow(0);

	ChangeMark = true;
	PauseMark = true;
	CamCount = 1;

	font.CreatePointFont(200, _T("宋体"));
	GetDlgItem(IDC_STA_HEIGHT)->SetFont(&font);
	GetDlgItem(IDC_STA_INTENS)->SetFont(&font);
	GetDlgItem(IDC_STA_QX)->SetFont(&font);
	GetDlgItem(IDC_STA_DJ)->SetFont(&font);

	GetDlgItem(IDC_STA_HEIGHT)->SetWindowText(L"高度图");
	GetDlgItem(IDC_STA_INTENS)->SetWindowText(L"亮度图");;
	GetDlgItem(IDC_STA_QX)->SetWindowText(L"缺陷检测结果");;
	GetDlgItem(IDC_STA_DJ)->SetWindowText(L"倒角角度结果");;


	SetDlgItemText(IDC_FILEPATH_SHOW, L"E:\\VSDebug\\SDK_HalconDemo\\image");

	mg_dlg = this;
	
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CSDKHalconDemoDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CSDKHalconDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


kStatus kCall onData(void* ctx, void* sys, void* dataset)
{

	//CSDKHalconDemoDlg* m_dlg = (CSDKHalconDemoDlg*)mg_dlg;
	HalconCpp::HObject* ho_Height_Back = NULL;
	HalconCpp::HObject* ho_Intens_Back = NULL;

	/*try
	{*/
		for (int i = 0; i < GoDataSet_Count(dataset); ++i)
		{
			GoDataMsg dataObj = GoDataSet_At(dataset, i);
			//选择解析传感器的数据包
			switch (GoDataMsg_Type(dataObj))
			{
			case GO_DATA_MESSAGE_TYPE_STAMP:
			{
				GoStampMsg stampMsg = dataObj;

				for (unsigned int i = 0; i < GoStampMsg_Count(stampMsg); i++)
				{
					GoStamp* stamp = GoStampMsg_At(stampMsg, i);

					CString strTemp;
					strTemp.Format(_T("拍摄第_%llu_张照片"), stamp->frameIndex);
					mg_dlg->WriteLog(strTemp);
				}
			}
			break;
			//均匀点云
			case GO_DATA_MESSAGE_TYPE_UNIFORM_SURFACE:
			{
				GoSurfaceMsg surfaceMsg = dataObj;

				int length = GoSurfaceMsg_Length(surfaceMsg);
				int width = GoSurfaceMsg_Width(surfaceMsg);

				//Z轴方向分辨率
				//double ZResolution = NM_TO_MM(GoSurfaceMsg_ZResolution(surfaceMsg));

				//创建一个数组用来接受对应的高度数据
				short* myBuffer = new short[length * width];
				for (int rowIdx = 0; rowIdx < length; rowIdx++)
				{
					//获取每行数据
					short* tmp = GoSurfaceMsg_RowAt(surfaceMsg, rowIdx);
					for (int colIdx = 0; colIdx < width; colIdx++)
					{
						myBuffer[rowIdx * width + colIdx] = tmp[colIdx];
						
					}
				}


				//获取对应的点云数据
				unsigned int rowIdx, colIdx;
				double XResolution = NM_TO_MM(GoSurfaceMsg_XResolution(surfaceMsg));
				double YResolution = NM_TO_MM(GoSurfaceMsg_YResolution(surfaceMsg));
				double ZResolution = NM_TO_MM(GoSurfaceMsg_ZResolution(surfaceMsg));
				double XOffset = UM_TO_MM(GoSurfaceMsg_XOffset(surfaceMsg));
				double YOffset = UM_TO_MM(GoSurfaceMsg_YOffset(surfaceMsg));
				double ZOffset = UM_TO_MM(GoSurfaceMsg_ZOffset(surfaceMsg));
				CSDKHalconDemoDlg::ProfilePoint** surfaceBuffer = NULL;
				k32u surfaceBufferHeight = 0;
				
				//allocate memory if needed
				if (surfaceBuffer == NULL)
				{
					surfaceBuffer = (CSDKHalconDemoDlg::ProfilePoint**)malloc(GoSurfaceMsg_Length(surfaceMsg) * sizeof(CSDKHalconDemoDlg::ProfilePoint*));

					for (unsigned int i = 0; i < GoSurfaceMsg_Length(surfaceMsg); i++)
					{
						surfaceBuffer[i] = (CSDKHalconDemoDlg::ProfilePoint*)malloc(GoSurfaceMsg_Width(surfaceMsg) * sizeof(CSDKHalconDemoDlg::ProfilePoint));
					}

					surfaceBufferHeight = GoSurfaceMsg_Length(surfaceMsg);
				}

				for (rowIdx = 0; rowIdx < GoSurfaceMsg_Length(surfaceMsg); rowIdx++)
				{
					k16s* data = GoSurfaceMsg_RowAt(surfaceMsg, rowIdx);

					for (colIdx = 0; colIdx < GoSurfaceMsg_Width(surfaceMsg); colIdx++)
					{
						// gocator transmits range data as 16-bit signed integers
						// to translate 16-bit range data to engineering units, the calculation for each point is: 
						//          X: XOffset + columnIndex * XResolution 
						//          Y: YOffset + rowIndex * YResolution
						//          Z: ZOffset + height_map[rowIndex][columnIndex] * ZResolution

						surfaceBuffer[rowIdx][colIdx].x = XOffset + XResolution * colIdx;
						surfaceBuffer[rowIdx][colIdx].y = YOffset + YResolution * rowIdx;


						if (data[colIdx] != INVALID_RANGE_16BIT)
						{
							surfaceBuffer[rowIdx][colIdx].z = ZOffset + ZResolution * data[colIdx];
						}
						else
						{
							surfaceBuffer[rowIdx][colIdx].z = INVALID_RANGE_DOUBLE;
						}
					}
				}
				//输出对应数据到文件
				//errno_t err;
				//FILE* fpWrite;
				//remove("C:/Users/Administrator/Desktop/pointcloud/heightPC.txt");
				////FILE *fpWrite = fopen("PointCloud.txt", "w+");
				//err = fopen_s(&fpWrite, "C:/Users/Administrator/Desktop/pointcloud/heightPC.txt", "w");
				//if (err == 0)
				//{
				//	for (unsigned int i = 0; i < GoSurfaceMsg_Length(surfaceMsg); i++)
				//	{
				//		for (unsigned int j = 0; j < GoSurfaceMsg_Width(surfaceMsg); j++)
				//		{
				//			fprintf(fpWrite, "|%-4.3f||%-4.3f||%-4.3f|\n", 
				//				surfaceBuffer[i][j].x, surfaceBuffer[i][j].y, surfaceBuffer[i][j].z);
				//		}
				//	}
				//}
				//else
				//	mg_dlg->WriteLog(L"错误的文件名称");
				//fclose(fpWrite);
				//mg_dlg->WriteLog(L"数据输出已完成");
				//获取对应的点云数据结束


				//将高度数据转化为Halcon数据,int2-tif unit2-png
				HalconCpp::GenImage1(mg_dlg->ho_Height, "int2", width, length, (Hlong)myBuffer);
				//显示图像到对应窗口，1表示高度图
				ho_Height_Back = mg_dlg->ho_Height;

				//将16位int2的高度图数据转换为16位的uint2数据
				HalconCpp::HObject ho_Image1, ho_Image11;
				HTuple  hv_Row, hv_Column, hv_Grayval, hv_Width;
				HTuple  hv_Height, hv_Min, hv_Max, hv_Range;
				GetRegionPoints(*ho_Height_Back, &hv_Row, &hv_Column);
				GetGrayval(*ho_Height_Back, hv_Row, hv_Column, &hv_Grayval);
				GetImageSize(*ho_Height_Back, &hv_Width, &hv_Height);
				GenImageConst(&ho_Image1, "uint2", hv_Width, hv_Height);
				SetGrayval(ho_Image1, hv_Row, hv_Column, (hv_Grayval + 32768));
				ConvertImageType(ho_Image1, &ho_Image11, "uint2");
				MinMaxGray(ho_Image11, ho_Image11, 0, &hv_Min, &hv_Max, &hv_Range);
				ho_Height_Back = &ho_Image11;

				mg_dlg->ShowImg(1, ho_Height_Back);
				HalconCpp::DispObj(*ho_Height_Back, hv_windowhandle);

				HalconCpp::WriteImage(*ho_Height_Back, "png", 0, "C:/Users/Administrator/Desktop/19.8.22/4/height/" + (mg_dlg->hv_a.TupleString(".2d")));
				//mg_dlg->SaveImg(mg_dlg->hv_a, ho_Height_Back);
				mg_dlg->WriteLog(L"输出高度数据完成");
				//删除变量空间
				delete myBuffer;
			}
			break;
			case GO_DATA_MESSAGE_TYPE_SURFACE_INTENSITY:
			{
				GoSurfaceIntensityMsg surfaceIntMsg = dataObj;
				int length = GoSurfaceIntensityMsg_Length(surfaceIntMsg);
				int width = GoSurfaceIntensityMsg_Width(surfaceIntMsg);


				//创建一个数组用来接受对应的亮度数据
				byte* myBuffer = new byte[length * width];
				for (int rowIdx = 0; rowIdx < length; rowIdx++)
				{
					//获取每行数据
					byte* tmp = GoSurfaceIntensityMsg_RowAt(surfaceIntMsg, rowIdx);
					for (int colIdx = 0; colIdx < width; colIdx++)
					{
						myBuffer[rowIdx * width + colIdx] = tmp[colIdx];
					}
				}


				//获取亮度的点云数据
				unsigned int rowIdx, colIdx;
				double XResolution = NM_TO_MM(GoSurfaceIntensityMsg_XResolution(surfaceIntMsg));
				double YResolution = NM_TO_MM(GoSurfaceIntensityMsg_YResolution(surfaceIntMsg));
				double XOffset = UM_TO_MM(GoSurfaceIntensityMsg_XOffset(surfaceIntMsg));
				double YOffset = UM_TO_MM(GoSurfaceIntensityMsg_YOffset(surfaceIntMsg));
				CSDKHalconDemoDlg::ProfilePoint** surfaceBuffer = NULL;
				k32u surfaceBufferHeight = 0;
				unsigned int  j;
				//allocate memory if needed
				if (surfaceBuffer == NULL)
				{
					surfaceBuffer = (CSDKHalconDemoDlg::ProfilePoint**)malloc(GoSurfaceIntensityMsg_Length(surfaceIntMsg) * sizeof(CSDKHalconDemoDlg::ProfilePoint*));

					for (j = 0; j < GoSurfaceIntensityMsg_Length(surfaceIntMsg); j++)
					{
						surfaceBuffer[j] = (CSDKHalconDemoDlg::ProfilePoint*)malloc(GoSurfaceIntensityMsg_Width(surfaceIntMsg) * sizeof(CSDKHalconDemoDlg::ProfilePoint));
					}


					surfaceBufferHeight = GoSurfaceIntensityMsg_Length(surfaceIntMsg);
				}

				for (rowIdx = 0; rowIdx < GoSurfaceIntensityMsg_Length(surfaceIntMsg); rowIdx++)
				{
					k8u* data = GoSurfaceIntensityMsg_RowAt(surfaceIntMsg, rowIdx);

					// gocator transmits intensity data as an 8-bit grayscale image of identical width and height as the corresponding height map
					for (colIdx = 0; colIdx < GoSurfaceIntensityMsg_Width(surfaceIntMsg); colIdx++)
					{
						surfaceBuffer[rowIdx][colIdx].x = XOffset + XResolution * colIdx;
						surfaceBuffer[rowIdx][colIdx].y = YOffset + YResolution * rowIdx;
						surfaceBuffer[rowIdx][colIdx].intensity = data[colIdx];
					}

				}

				//输出亮度点云对应数据到文件
				//errno_t err;
				//FILE* fpWrite;
				//remove("C:/Users/Administrator/Desktop/pointcloud/Intens.txt");
				////FILE *fpWrite = fopen("PointCloud.txt", "w+");
				//err = fopen_s(&fpWrite, "C:/Users/Administrator/Desktop/pointcloud/Intens.txt", "w");
				//if (err == 0)
				//{
				//	for (unsigned int i = 0; i < GoSurfaceIntensityMsg_Length(surfaceIntMsg); i++)
				//	{
				//		for (unsigned int j = 0; j < GoSurfaceIntensityMsg_Width(surfaceIntMsg); j++)
				//		{
				//			fprintf(fpWrite, "|%-4.3f||%-4.3f||%-4.3c|\n", 
				//				surfaceBuffer[i][j].x, surfaceBuffer[i][j].y, surfaceBuffer[i][j].intensity);
				//		}
				//	}
				//}
				//else
				//	mg_dlg->WriteLog(L"错误的文件名称");
				//fclose(fpWrite);
				//mg_dlg->WriteLog(L"数据输出已完成");


				//输出亮度buffer数据到文件
				//FILE* fpWrite; int v, m; CString strTemp;
				//remove("C:/Users/Administrator/Desktop/pointcloud/IntensData.txt");
				////FILE *fpWrite = fopen("PointCloud.txt", "w+");
				//errno_t err = fopen_s(&fpWrite, "C:/Users/Administrator/Desktop/pointcloud/IntensData.txt", "w");
				//if (err == 0)
				//{
				//	for (int i = 0; i < (sizeof(myBuffer) / sizeof(myBuffer[0] - 1)); i++)
				//	{
				//		//fprintf(fpWrite, "%f", myBuffer[i]);

				//		v = myBuffer[i] / 16;
				//		m = myBuffer[i] % 16;

				//		if (v >= 0 && v <= 9)
				//			v += 48;
				//		else if (v >= 97 && v <= 122)
				//			v += 87;
				//		else
				//			v += 55;
				//		if (m >= 0 && m <= 9)
				//			m += 48;
				//		else if (m >= 97 && m <= 122)
				//			m += 87;
				//		else
				//			m += 55;

				//		char vChar = (char)v;	//ASCII转字符
				//		char mChar = (char)m;

				//		char strKVCTmp[3] = { vChar,mChar };
				//		strTemp += strcat(strKVCTmp, " ");

				//		fprintf(fpWrite, "%s", strTemp);
				//	}
				//}
				//else
				//	mg_dlg->WriteLog(L"错误的文件名称");
				//fclose(fpWrite);
				//mg_dlg->WriteLog(L"数据输出已完成");
				//结束获取亮度的点云数据

				//将亮度数据转换为Halcon数据 byte-tif byte-bmp
				HalconCpp::GenImage1(mg_dlg->ho_Intens, "byte", width, length, (Hlong)myBuffer);
				ho_Intens_Back = mg_dlg->ho_Intens;

				//将8位byte的亮度图数据转换为16位的uint2数据
				/*HalconCpp::HObject ho_Image1, ho_Image11;
				HTuple  hv_Row, hv_Column, hv_Grayval, hv_Width;
				HTuple  hv_Height, hv_Min, hv_Max, hv_Range;
				GetRegionPoints(*mg_dlg->ho_Intens, &hv_Row, &hv_Column);
				GetGrayval(*mg_dlg->ho_Intens, hv_Row, hv_Column, &hv_Grayval);
				GetImageSize(*mg_dlg->ho_Intens, &hv_Width, &hv_Height);
				GenImageConst(&ho_Image1, "uint2", hv_Width, hv_Height);
				SetGrayval(ho_Image1, hv_Row, hv_Column, (hv_Grayval * 256));
				ConvertImageType(ho_Image1, &ho_Image11, "uint2");
				MinMaxGray(ho_Image11, ho_Image11, 0, &hv_Min, &hv_Max, &hv_Range);
				ho_Intens_Back = &ho_Image11;*/

				//显示图像到对应窗口，2表示亮度图
				
				mg_dlg->ShowImg(2, ho_Intens_Back);
				HalconCpp::DispObj(*ho_Intens_Back, hv_windowhandle);
				HalconCpp::WriteImage(*ho_Intens_Back, "bmp", 0, "C:/Users/Administrator/Desktop/19.8.22/4/intens/" + (mg_dlg->hv_b.TupleString(".2d")));
				//mg_dlg->SaveImg(mg_dlg->hv_b, ho_Intens_Back);
				mg_dlg->WriteLog(L"输出亮度信息完成");
				//删除变量空间
				delete myBuffer;
			}
			break;
			}
		}
	
		//缺陷测试代码
		/*HalconCpp::HObject  ho_Image1, ho_Image2;

		ReadImage(&ho_Image1,
			"C:/Users/zxuan/Desktop/0809/1.png");
		ReadImage(&ho_Image2,
			"C:/Users/zxuan/Desktop/0809/2.png");
		m_QX_JC->DealCamCpp(ho_Image1, ho_Image2);*/

		//缺陷检测代码
		//m_QX_JC->DealCamCpp(*ho_Height_Back, *ho_Intens_Back);
		//倒角检测代码
		//m_SS_DJ->Deal_SS_DJ(*ho_Height_Back, *ho_Intens_Back);

		//mg_dlg->SendImgToDeal(*ho_Height_Back, *ho_Intens_Back);
		 
		//进入保存图片程序中
		

		//变量增加
		//适用于存放在相同的文件夹里
		/*mg_dlg->hv_a += 2;
		mg_dlg->hv_b += 2;*/
		//适用于存放在不同的文件夹里
		mg_dlg->hv_a += 1;
		mg_dlg->hv_b += 1;
		mg_dlg->CamCount += 1;

		//拍摄计数停止位
		if (mg_dlg->CamCount > NUM_PROFILES)
		{
			mg_dlg->WriteLog(L"拍摄已完成！！");
			mg_dlg->CamCount = 1;
			return kERROR;
		}
		else
		{
			//暂停采集标志位
			if (mg_dlg->PauseMark)
				return kOK;
			else
			{
				mg_dlg->WriteLog(L"拍摄已暂停！！");
				mg_dlg->CamCount = 1;
				return kERROR;
			}
			return kOK;
		}
		//return kOK;
	//}
	//////捕获Halcon错误信息
	//catch (HalconCpp::HException& HDevExpDefaultException)
	//{
	//	printf(HDevExpDefaultException.ErrorMessage());
	////	CString strTemp;
	////	strTemp.Format(HDevExpDefaultException.ErrorMessage());
	////	mg_dlg->WriteLog(strTemp);
	//}
}

//日志文件
void CSDKHalconDemoDlg::WriteLog(CString s)
{
	auto now = std::chrono::system_clock::now();
	auto ms = std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch());

	CStringW str;
	//tmp.Format(L"%03d ", ms % 1000);
	//str.Insert(0, tmp);
	str.Insert(0, CTime::GetCurrentTime().Format(L"> %Y-%m-%d %H:%M:%S: "));
	str.Append(s);
	//str.Format(_T("\r\n"));
	//str += "\r\n";
	//str.Insert(str.GetLength(), L"\r\n");
	str.Append(L"\r\n");

	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_SHOW_INFO);
	int len = pEdit->GetWindowTextLength();
	if (len >= static_cast<int>(pEdit->GetLimitText()))
		pEdit->SetSel(0, -1);
	else
		pEdit->SetSel(-1, -1);
	pEdit->ReplaceSel(str);

	//GetDlgItem(IDC_SHOW_INFO)->SetWindowTextW(str);
	
}

//保存点云文件到本地
void CSDKHalconDemoDlg::ToSavePointCloud(CString fileNameTemp,ProfilePoint** surfaceBuffer, int row, int col, bool flag)
{
	//TODO 保存surfaceBuffer到对应文件目录
	errno_t err;
	FILE* fpWrite;
	int i, j = 0;
	CString fileName = L"C:/Users/Administrator/Desktop/pointcloud/";
	fileName.Append(fileNameTemp);
	remove((char*)(LPCTSTR)fileName);
	//FILE *fpWrite = fopen("PointCloud.txt", "w+");
	err = fopen_s(&fpWrite, (char*)(LPCTSTR)fileName, "w");
	if (err == 0)
	{
		for (i = 0; i < row; i++)
		{
			for (j = 0; j < col; j++)
			{
				if (flag)
					fprintf(fpWrite, "|%-4.3f||%-4.3f||%-4.3f|\n", surfaceBuffer[i][j].x, surfaceBuffer[i][j].y, surfaceBuffer[i][j].z);
				else
					fprintf(fpWrite, "|%-4.3f||%-4.3f||%-4.3c|\n", surfaceBuffer[i][j].x, surfaceBuffer[i][j].y, surfaceBuffer[i][j].intensity);
			}
		}
	}
	else
		mg_dlg->WriteLog(L"错误的文件名称");
	fclose(fpWrite);
	mg_dlg->WriteLog(L"数据输出已完成");
}

//重绘计算面积
void CSDKHalconDemoDlg::OnDrawShowInfo(int t,HalconCpp::HObject* img)
{
	try
	{
		GetImageSize(*img, &Width, &Height);

		switch (t)
		{
		case 1:
			::GetClientRect(GetDlgItem(IDC_IMG_HEIG)->m_hWnd, rc);
			break;
		case 2:
			::GetClientRect(GetDlgItem(IDC_IMG_INTENS)->m_hWnd, rc);
			break;
		case 3:
			::GetClientRect(GetDlgItem(IDC_IMG_DEALSHOW)->m_hWnd, rc);
			break;
		case 4:
			::GetClientRect(GetDlgItem(IDC_IMG_DEALSHOW2)->m_hWnd, rc);
			break;
		default:
			break;
		}	
		
		xScale = (double)rc.Width() / (double)Width;
		yScale = (double)rc.Height() / (double)Height;
		scale = std::min<double>(xScale, yScale);
		bmpShowWidth = (double)Width * scale;
		bmpShowHeight = (double)Height * scale;
		xWndStart = ((double)rc.Width() - bmpShowWidth) / (double)2;
		yWndStart = ((double)rc.Height() - bmpShowHeight) / (double)2;

		// 计算等比缩放的位置和尺寸
		Invalidate();
		UpdateWindow();
	}
	catch (HalconCpp::HException& HDevExpDefaultException)
	{
		printf(HDevExpDefaultException.ErrorMessage());
	}
	
}

//根据计算后的图像大小得到对应显示窗口的显示句柄
void CSDKHalconDemoDlg::ShowImg(int t, HalconCpp::HObject *img)
{
	try
	{
		OnDrawShowInfo(t, img);

		switch (t)
		{
		case 1:
			HalconCpp::OpenWindow(yWndStart, xWndStart, bmpShowWidth, bmpShowHeight,
				(Hlong)GetDlgItem(IDC_IMG_HEIG)->m_hWnd, "visible", "", &hv_windowhandle);
			break;
		case 2:
			HalconCpp::OpenWindow(yWndStart, xWndStart, bmpShowWidth, bmpShowHeight,
				(Hlong)GetDlgItem(IDC_IMG_INTENS)->m_hWnd, "visible", "", &hv_windowhandle);
			break;
		case 3:
			HalconCpp::OpenWindow(yWndStart, xWndStart, bmpShowWidth, bmpShowHeight,
				(Hlong)GetDlgItem(IDC_IMG_DEALSHOW)->m_hWnd, "visible", "", &hv_windowhandle);
			break;
		case 4:
			HalconCpp::OpenWindow(yWndStart, xWndStart, bmpShowWidth, bmpShowHeight,
				(Hlong)GetDlgItem(IDC_IMG_DEALSHOW2)->m_hWnd, "visible", "", &hv_windowhandle);

			break;
		default:
			break;
		}

		HalconCpp::SetPart(hv_windowhandle, 1, 1, Height, Width);
		HalconCpp::SetColor(hv_windowhandle, "red");
		//HalconCpp::DispObj(*img, hv_windowhandle);
	}
	catch (const std::exception& stdEx)
	{
		printf(stdEx.what());
	}
}

//具体执行显示图片的函数
void CSDKHalconDemoDlg::displaySend(HalconCpp::HObject img)
{
	HalconCpp::DispObj(img, hv_windowhandle);
}


// Chapter: Graphics / Text
// Short Description: Set font independent of OS 
//设置在图片上显示文字的大小颜色
void CSDKHalconDemoDlg::set_display_font(HTuple hv_WindowHandle, HTuple hv_Size, HTuple hv_Font, HTuple hv_Bold,
	HTuple hv_Slant)
{
	//// Local iconic variables

	//// Local control variables
	
	HTuple  hv_OS, hv_BufferWindowHandle, hv_Ascent;
	HTuple  hv_Descent, hv_Width, hv_Height, hv_Scale, hv_Exception;
	HTuple  hv_SubFamily, hv_Fonts, hv_SystemFonts, hv_Guess;
	HTuple  hv_I, hv_Index, hv_AllowedFontSizes, hv_Distances;
	HTuple  hv_Indices, hv_FontSelRegexp, hv_FontsCourier;

	//This procedure sets the text font of the current window with
	//the specified attributes.
	//It is assumed that following fonts are installed on the system:
	//Windows: Courier New, Arial Times New Roman
	//Mac OS X: CourierNewPS, Arial, TimesNewRomanPS
	//Linux: courier, helvetica, times
	//Because fonts are displayed smaller on Linux than on Windows,
	//a scaling factor of 1.25 is used the get comparable results.
	//For Linux, only a limited number of font sizes is supported,
	//to get comparable results, it is recommended to use one of the
	//following sizes: 9, 11, 14, 16, 20, 27
	//(which will be mapped internally on Linux systems to 11, 14, 17, 20, 25, 34)
	//
	//Input parameters:
	//WindowHandle: The graphics window for which the font will be set
	//Size: The font size. If Size=-1, the default of 16 is used.
	//Bold: If set to 'true', a bold font is used
	//Slant: If set to 'true', a slanted font is used
	//
	GetSystem("operating_system", &hv_OS);
	// dev_get_preferences(...); only in hdevelop
	// dev_set_preferences(...); only in hdevelop
	if (0 != (HTuple(hv_Size == HTuple()).TupleOr(hv_Size == -1)))
	{
		hv_Size = 16;
	}
	if (0 != ((hv_OS.TupleSubstr(0, 2)) == HTuple("Win")))
	{
		//Set font on Windows systems
		try
		{
			//Check, if font scaling is switched on
			OpenWindow(0, 0, 256, 256, 0, "buffer", "", &hv_BufferWindowHandle);
			HalconCpp::SetFont(hv_BufferWindowHandle, "-Consolas-16-*-0-*-*-1-");
			GetStringExtents(hv_BufferWindowHandle, "test_string", &hv_Ascent, &hv_Descent,
				&hv_Width, &hv_Height);
			//Expected width is 110
			hv_Scale = 110.0 / hv_Width;
			hv_Size = (hv_Size * hv_Scale).TupleInt();
			HalconCpp::CloseWindow(hv_BufferWindowHandle);
		}
		// catch (Exception) 
		catch (HalconCpp::HException& HDevExpDefaultException)
		{
			HDevExpDefaultException.ToHTuple(&hv_Exception);
			//throw (Exception)
		}
		if (0 != (HTuple(hv_Font == HTuple("Courier")).TupleOr(hv_Font == HTuple("courier"))))
		{
			hv_Font = "Courier New";
		}
		else if (0 != (hv_Font == HTuple("mono")))
		{
			hv_Font = "Consolas";
		}
		else if (0 != (hv_Font == HTuple("sans")))
		{
			hv_Font = "Arial";
		}
		else if (0 != (hv_Font == HTuple("serif")))
		{
			hv_Font = "Times New Roman";
		}
		if (0 != (hv_Bold == HTuple("true")))
		{
			hv_Bold = 1;
		}
		else if (0 != (hv_Bold == HTuple("false")))
		{
			hv_Bold = 0;
		}
		else
		{
			hv_Exception = "Wrong value of control parameter Bold";
			throw HalconCpp::HException(hv_Exception);
		}
		if (0 != (hv_Slant == HTuple("true")))
		{
			hv_Slant = 1;
		}
		else if (0 != (hv_Slant == HTuple("false")))
		{
			hv_Slant = 0;
		}
		else
		{
			hv_Exception = "Wrong value of control parameter Slant";
			throw HalconCpp::HException(hv_Exception);
		}
		try
		{
			HalconCpp::SetFont(hv_WindowHandle, ((((((("-" + hv_Font) + "-") + hv_Size) + "-*-") + hv_Slant) + "-*-*-") + hv_Bold) + "-");
		}
		// catch (Exception) 
		catch (HalconCpp::HException& HDevExpDefaultException)
		{
			HDevExpDefaultException.ToHTuple(&hv_Exception);
			//throw (Exception)
		}
	}
	else if (0 != ((hv_OS.TupleSubstr(0, 2)) == HTuple("Dar")))
	{
		//Set font on Mac OS X systems. Since OS X does not have a strict naming
		//scheme for font attributes, we use tables to determine the correct font
		//name.
		hv_SubFamily = 0;
		if (0 != (hv_Slant == HTuple("true")))
		{
			hv_SubFamily = hv_SubFamily | 1;
		}
		else if (0 != (hv_Slant != HTuple("false")))
		{
			hv_Exception = "Wrong value of control parameter Slant";
			throw HalconCpp::HException(hv_Exception);
		}
		if (0 != (hv_Bold == HTuple("true")))
		{
			hv_SubFamily = hv_SubFamily | 2;
		}
		else if (0 != (hv_Bold != HTuple("false")))
		{
			hv_Exception = "Wrong value of control parameter Bold";
			throw HalconCpp::HException(hv_Exception);
		}
		if (0 != (hv_Font == HTuple("mono")))
		{
			hv_Fonts.Clear();
			hv_Fonts[0] = "Menlo-Regular";
			hv_Fonts[1] = "Menlo-Italic";
			hv_Fonts[2] = "Menlo-Bold";
			hv_Fonts[3] = "Menlo-BoldItalic";
		}
		else if (0 != (HTuple(hv_Font == HTuple("Courier")).TupleOr(hv_Font == HTuple("courier"))))
		{
			hv_Fonts.Clear();
			hv_Fonts[0] = "CourierNewPSMT";
			hv_Fonts[1] = "CourierNewPS-ItalicMT";
			hv_Fonts[2] = "CourierNewPS-BoldMT";
			hv_Fonts[3] = "CourierNewPS-BoldItalicMT";
		}
		else if (0 != (hv_Font == HTuple("sans")))
		{
			hv_Fonts.Clear();
			hv_Fonts[0] = "ArialMT";
			hv_Fonts[1] = "Arial-ItalicMT";
			hv_Fonts[2] = "Arial-BoldMT";
			hv_Fonts[3] = "Arial-BoldItalicMT";
		}
		else if (0 != (hv_Font == HTuple("serif")))
		{
			hv_Fonts.Clear();
			hv_Fonts[0] = "TimesNewRomanPSMT";
			hv_Fonts[1] = "TimesNewRomanPS-ItalicMT";
			hv_Fonts[2] = "TimesNewRomanPS-BoldMT";
			hv_Fonts[3] = "TimesNewRomanPS-BoldItalicMT";
		}
		else
		{
			//Attempt to figure out which of the fonts installed on the system
			//the user could have meant.
			QueryFont(hv_WindowHandle, &hv_SystemFonts);
			hv_Fonts.Clear();
			hv_Fonts.Append(hv_Font);
			hv_Fonts.Append(hv_Font);
			hv_Fonts.Append(hv_Font);
			hv_Fonts.Append(hv_Font);
			hv_Guess.Clear();
			hv_Guess.Append(hv_Font);
			hv_Guess.Append(hv_Font + "-Regular");
			hv_Guess.Append(hv_Font + "MT");
			{
				HTuple end_val100 = (hv_Guess.TupleLength()) - 1;
				HTuple step_val100 = 1;
				for (hv_I = 0; hv_I.Continue(end_val100, step_val100); hv_I += step_val100)
				{
					TupleFind(hv_SystemFonts, HTuple(hv_Guess[hv_I]), &hv_Index);
					if (0 != (hv_Index != -1))
					{
						hv_Fonts[0] = HTuple(hv_Guess[hv_I]);
						break;
					}
				}
			}
			//Guess name of slanted font
			hv_Guess.Clear();
			hv_Guess.Append(hv_Font + "-Italic");
			hv_Guess.Append(hv_Font + "-ItalicMT");
			hv_Guess.Append(hv_Font + "-Oblique");
			{
				HTuple end_val109 = (hv_Guess.TupleLength()) - 1;
				HTuple step_val109 = 1;
				for (hv_I = 0; hv_I.Continue(end_val109, step_val109); hv_I += step_val109)
				{
					TupleFind(hv_SystemFonts, HTuple(hv_Guess[hv_I]), &hv_Index);
					if (0 != (hv_Index != -1))
					{
						hv_Fonts[1] = HTuple(hv_Guess[hv_I]);
						break;
					}
				}
			}
			//Guess name of bold font
			hv_Guess.Clear();
			hv_Guess.Append(hv_Font + "-Bold");
			hv_Guess.Append(hv_Font + "-BoldMT");
			{
				HTuple end_val118 = (hv_Guess.TupleLength()) - 1;
				HTuple step_val118 = 1;
				for (hv_I = 0; hv_I.Continue(end_val118, step_val118); hv_I += step_val118)
				{
					TupleFind(hv_SystemFonts, HTuple(hv_Guess[hv_I]), &hv_Index);
					if (0 != (hv_Index != -1))
					{
						hv_Fonts[2] = HTuple(hv_Guess[hv_I]);
						break;
					}
				}
			}
			//Guess name of bold slanted font
			hv_Guess.Clear();
			hv_Guess.Append(hv_Font + "-BoldItalic");
			hv_Guess.Append(hv_Font + "-BoldItalicMT");
			hv_Guess.Append(hv_Font + "-BoldOblique");
			{
				HTuple end_val127 = (hv_Guess.TupleLength()) - 1;
				HTuple step_val127 = 1;
				for (hv_I = 0; hv_I.Continue(end_val127, step_val127); hv_I += step_val127)
				{
					TupleFind(hv_SystemFonts, HTuple(hv_Guess[hv_I]), &hv_Index);
					if (0 != (hv_Index != -1))
					{
						hv_Fonts[3] = HTuple(hv_Guess[hv_I]);
						break;
					}
				}
			}
		}
		hv_Font = ((const HTuple&)hv_Fonts)[hv_SubFamily];
		try
		{
			HalconCpp::SetFont(hv_WindowHandle, (hv_Font + "-") + hv_Size);
		}
		// catch (Exception) 
		catch (HalconCpp::HException& HDevExpDefaultException)
		{
			HDevExpDefaultException.ToHTuple(&hv_Exception);
			//throw (Exception)
		}
	}
	else
	{
		//Set font for UNIX systems
		hv_Size = hv_Size * 1.25;
		hv_AllowedFontSizes.Clear();
		hv_AllowedFontSizes[0] = 11;
		hv_AllowedFontSizes[1] = 14;
		hv_AllowedFontSizes[2] = 17;
		hv_AllowedFontSizes[3] = 20;
		hv_AllowedFontSizes[4] = 25;
		hv_AllowedFontSizes[5] = 34;
		if (0 != ((hv_AllowedFontSizes.TupleFind(hv_Size)) == -1))
		{
			hv_Distances = (hv_AllowedFontSizes - hv_Size).TupleAbs();
			TupleSortIndex(hv_Distances, &hv_Indices);
			hv_Size = ((const HTuple&)hv_AllowedFontSizes)[HTuple(hv_Indices[0])];
		}
		if (0 != (HTuple(hv_Font == HTuple("mono")).TupleOr(hv_Font == HTuple("Courier"))))
		{
			hv_Font = "courier";
		}
		else if (0 != (hv_Font == HTuple("sans")))
		{
			hv_Font = "helvetica";
		}
		else if (0 != (hv_Font == HTuple("serif")))
		{
			hv_Font = "times";
		}
		if (0 != (hv_Bold == HTuple("true")))
		{
			hv_Bold = "bold";
		}
		else if (0 != (hv_Bold == HTuple("false")))
		{
			hv_Bold = "medium";
		}
		else
		{
			hv_Exception = "Wrong value of control parameter Bold";
			throw HalconCpp::HException(hv_Exception);
		}
		if (0 != (hv_Slant == HTuple("true")))
		{
			if (0 != (hv_Font == HTuple("times")))
			{
				hv_Slant = "i";
			}
			else
			{
				hv_Slant = "o";
			}
		}
		else if (0 != (hv_Slant == HTuple("false")))
		{
			hv_Slant = "r";
		}
		else
		{
			hv_Exception = "Wrong value of control parameter Slant";
			throw HalconCpp::HException(hv_Exception);
		}
		try
		{
			HalconCpp::SetFont(hv_WindowHandle, ((((((("-adobe-" + hv_Font) + "-") + hv_Bold) + "-") + hv_Slant) + "-normal-*-") + hv_Size) + "-*-*-*-*-*-*-*");
		}
		// catch (Exception) 
		catch (HalconCpp::HException& HDevExpDefaultException)
		{
			HDevExpDefaultException.ToHTuple(&hv_Exception);
			if (0 != (HTuple((hv_OS.TupleSubstr(0, 4)) == HTuple("Linux")).TupleAnd(hv_Font == HTuple("courier"))))
			{
				QueryFont(hv_WindowHandle, &hv_Fonts);
				hv_FontSelRegexp = (("^-[^-]*-[^-]*[Cc]ourier[^-]*-" + hv_Bold) + "-") + hv_Slant;
				hv_FontsCourier = (hv_Fonts.TupleRegexpSelect(hv_FontSelRegexp)).TupleRegexpMatch(hv_FontSelRegexp);
				if (0 != ((hv_FontsCourier.TupleLength()) == 0))
				{
					hv_Exception = "Wrong font name";
					//throw (Exception)
				}
				else
				{
					try
					{
						HalconCpp::SetFont(hv_WindowHandle, ((HTuple(hv_FontsCourier[0]) + "-normal-*-") + hv_Size) + "-*-*-*-*-*-*-*");
					}
					// catch (Exception) 
					catch (HalconCpp::HException& HDevExpDefaultException)
					{
						HDevExpDefaultException.ToHTuple(&hv_Exception);
						//throw (Exception)
					}
				}
			}
			//throw (Exception)
		}
	}
	// dev_set_preferences(...); only in hdevelop
	return;

}

// Chapter: Graphics / Text
// Short Description: This procedure writes a text message. 
//设置在图片上显示文字
void CSDKHalconDemoDlg::disp_message(HTuple hv_WindowHandle, HTuple hv_String, HTuple hv_CoordSystem,
	HTuple hv_Row, HTuple hv_Column, HTuple hv_Color, HTuple hv_Box)
{

	// Local iconic variables

	// Local control variables
	HTuple  hv_Red, hv_Green, hv_Blue, hv_Row1Part;
	HTuple  hv_Column1Part, hv_Row2Part, hv_Column2Part, hv_RowWin;
	HTuple  hv_ColumnWin, hv_WidthWin, hv_HeightWin, hv_MaxAscent;
	HTuple  hv_MaxDescent, hv_MaxWidth, hv_MaxHeight, hv_R1;
	HTuple  hv_C1, hv_FactorRow, hv_FactorColumn, hv_UseShadow;
	HTuple  hv_ShadowColor, hv_Exception, hv_Width, hv_Index;
	HTuple  hv_Ascent, hv_Descent, hv_W, hv_H, hv_FrameHeight;
	HTuple  hv_FrameWidth, hv_R2, hv_C2, hv_DrawMode, hv_CurrentColor;

	//This procedure displays text in a graphics window.
	//
	//Input parameters:
	//WindowHandle: The WindowHandle of the graphics window, where
	//   the message should be displayed
	//String: A tuple of strings containing the text message to be displayed
	//CoordSystem: If set to 'window', the text position is given
	//   with respect to the window coordinate system.
	//   If set to 'image', image coordinates are used.
	//   (This may be useful in zoomed images.)
	//Row: The row coordinate of the desired text position
	//   If set to -1, a default value of 12 is used.
	//Column: The column coordinate of the desired text position
	//   If set to -1, a default value of 12 is used.
	//Color: defines the color of the text as string.
	//   If set to [], '' or 'auto' the currently set color is used.
	//   If a tuple of strings is passed, the colors are used cyclically
	//   for each new textline.
	//Box: If Box[0] is set to 'true', the text is written within an orange box.
	//     If set to' false', no box is displayed.
	//     If set to a color string (e.g. 'white', '#FF00CC', etc.),
	//       the text is written in a box of that color.
	//     An optional second value for Box (Box[1]) controls if a shadow is displayed:
	//       'true' -> display a shadow in a default color
	//       'false' -> display no shadow (same as if no second value is given)
	//       otherwise -> use given string as color string for the shadow color
	//
	//Prepare window
	GetRgb(hv_WindowHandle, &hv_Red, &hv_Green, &hv_Blue);
	GetPart(hv_WindowHandle, &hv_Row1Part, &hv_Column1Part, &hv_Row2Part, &hv_Column2Part);
	GetWindowExtents(hv_WindowHandle, &hv_RowWin, &hv_ColumnWin, &hv_WidthWin, &hv_HeightWin);
	SetPart(hv_WindowHandle, 0, 0, hv_HeightWin - 1, hv_WidthWin - 1);
	//
	//default settings
	if (0 != (hv_Row == -1))
	{
		hv_Row = 12;
	}
	if (0 != (hv_Column == -1))
	{
		hv_Column = 12;
	}
	if (0 != (hv_Color == HTuple()))
	{
		hv_Color = "";
	}
	//
	hv_String = (("" + hv_String) + "").TupleSplit("\n");
	//
	//Estimate extentions of text depending on font size.
	GetFontExtents(hv_WindowHandle, &hv_MaxAscent, &hv_MaxDescent, &hv_MaxWidth, &hv_MaxHeight);
	if (0 != (hv_CoordSystem == HTuple("window")))
	{
		hv_R1 = hv_Row;
		hv_C1 = hv_Column;
	}
	else
	{
		//Transform image to window coordinates
		hv_FactorRow = (1. * hv_HeightWin) / ((hv_Row2Part - hv_Row1Part) + 1);
		hv_FactorColumn = (1. * hv_WidthWin) / ((hv_Column2Part - hv_Column1Part) + 1);
		hv_R1 = ((hv_Row - hv_Row1Part) + 0.5) * hv_FactorRow;
		hv_C1 = ((hv_Column - hv_Column1Part) + 0.5) * hv_FactorColumn;
	}
	//
	//Display text box depending on text size
	hv_UseShadow = 1;
	hv_ShadowColor = "gray";
	if (0 != (HTuple(hv_Box[0]) == HTuple("true")))
	{
		hv_Box[0] = "#fce9d4";
		hv_ShadowColor = "#f28d26";
	}
	if (0 != ((hv_Box.TupleLength()) > 1))
	{
		if (0 != (HTuple(hv_Box[1]) == HTuple("true")))
		{
			//Use default ShadowColor set above
		}
		else if (0 != (HTuple(hv_Box[1]) == HTuple("false")))
		{
			hv_UseShadow = 0;
		}
		else
		{
			hv_ShadowColor = ((const HTuple&)hv_Box)[1];
			//Valid color?
			try
			{
				SetColor(hv_WindowHandle, HTuple(hv_Box[1]));
			}
			// catch (Exception) 
			catch (HalconCpp::HException& HDevExpDefaultException)
			{
				HDevExpDefaultException.ToHTuple(&hv_Exception);
				hv_Exception = "Wrong value of control parameter Box[1] (must be a 'true', 'false', or a valid color string)";
				throw HalconCpp::HException(hv_Exception);
			}
		}
	}
	if (0 != (HTuple(hv_Box[0]) != HTuple("false")))
	{
		//Valid color?
		try
		{
			SetColor(hv_WindowHandle, HTuple(hv_Box[0]));
		}
		// catch (Exception) 
		catch (HalconCpp::HException& HDevExpDefaultException)
		{
			HDevExpDefaultException.ToHTuple(&hv_Exception);
			hv_Exception = "Wrong value of control parameter Box[0] (must be a 'true', 'false', or a valid color string)";
			throw HalconCpp::HException(hv_Exception);
		}
		//Calculate box extents
		hv_String = (" " + hv_String) + " ";
		hv_Width = HTuple();
		{
			HTuple end_val93 = (hv_String.TupleLength()) - 1;
			HTuple step_val93 = 1;
			for (hv_Index = 0; hv_Index.Continue(end_val93, step_val93); hv_Index += step_val93)
			{
				GetStringExtents(hv_WindowHandle, HTuple(hv_String[hv_Index]), &hv_Ascent,
					&hv_Descent, &hv_W, &hv_H);
				hv_Width = hv_Width.TupleConcat(hv_W);
			}
		}
		hv_FrameHeight = hv_MaxHeight * (hv_String.TupleLength());
		hv_FrameWidth = (HTuple(0).TupleConcat(hv_Width)).TupleMax();
		hv_R2 = hv_R1 + hv_FrameHeight;
		hv_C2 = hv_C1 + hv_FrameWidth;
		//Display rectangles
		GetDraw(hv_WindowHandle, &hv_DrawMode);
		SetDraw(hv_WindowHandle, "fill");
		//Set shadow color
		SetColor(hv_WindowHandle, hv_ShadowColor);
		if (0 != hv_UseShadow)
		{
			DispRectangle1(hv_WindowHandle, hv_R1 + 1, hv_C1 + 1, hv_R2 + 1, hv_C2 + 1);
		}
		//Set box color
		SetColor(hv_WindowHandle, HTuple(hv_Box[0]));
		DispRectangle1(hv_WindowHandle, hv_R1, hv_C1, hv_R2, hv_C2);
		SetDraw(hv_WindowHandle, hv_DrawMode);
	}
	//Write text.
	{
		HTuple end_val115 = (hv_String.TupleLength()) - 1;
		HTuple step_val115 = 1;
		for (hv_Index = 0; hv_Index.Continue(end_val115, step_val115); hv_Index += step_val115)
		{
			hv_CurrentColor = ((const HTuple&)hv_Color)[hv_Index % (hv_Color.TupleLength())];
			if (0 != (HTuple(hv_CurrentColor != HTuple("")).TupleAnd(hv_CurrentColor != HTuple("auto"))))
			{
				SetColor(hv_WindowHandle, hv_CurrentColor);
			}
			else
			{
				SetRgb(hv_WindowHandle, hv_Red, hv_Green, hv_Blue);
			}
			hv_Row = hv_R1 + (hv_MaxHeight * hv_Index);
			SetTposition(hv_WindowHandle, hv_Row, hv_C1);
			WriteString(hv_WindowHandle, HTuple(hv_String[hv_Index]));
		}
	}
	//Reset changed window settings
	SetRgb(hv_WindowHandle, hv_Red, hv_Green, hv_Blue);
	SetPart(hv_WindowHandle, hv_Row1Part, hv_Column1Part, hv_Row2Part, hv_Column2Part);
	return;
}

//显示倒角尺寸到图片位置
void CSDKHalconDemoDlg::displayImgInfo(HTuple hv_ChamPhi_L,HTuple hv_ChamPhi_R)
{
	set_display_font(hv_windowhandle, 12, "sans", "true", "false");
	disp_message(hv_windowhandle, "ChamPhi_L=" + hv_ChamPhi_L + "\r\n"+ "ChamPhi_R=" + hv_ChamPhi_R, 
		"window", 12, 12,"black", "true");
}

//缺陷检测和倒角测量，增加一个中转函数调用，方便创建线程
void CSDKHalconDemoDlg::SendImgToDeal(HalconCpp::HObject ho_Height_Back, HalconCpp::HObject ho_Intens_Back)
{
	//std::thread thread_QX_JCObj(&DealCamFun::DealCamCpp, m_QX_JC, ho_Height_Back, ho_Intens_Back);
	//thread_QX_JCObj.detach();
	//m_QX_JC->DealCamCpp(ho_Height_Back, ho_Intens_Back);

	//std::thread thread_DJ_JCObj(&SS_DJ_Measure::Deal_SS_DJ, m_SS_DJ, ho_Height_Back, ho_Intens_Back);
	//thread_DJ_JCObj.detach();
	//m_SS_DJ->Deal_SS_DJ(ho_Height_Back, ho_Intens_Back);
}

//保存文件函数
void CSDKHalconDemoDlg::SaveImg(HTuple hv_num, HalconCpp::HObject* img)
{
	CString strPath; //= GetWindowText(IDC_STA_SHOWPATH,);
	GetDlgItem(IDC_FILEPATH_SHOW)->GetWindowTextW(strPath); //->GetWindowText(strPath);

	strPath += "\\";

	//创建线程失败，改为在外部创建
	//std::thread thread_SaveImg(&HalconCpp::WriteImage, *img, "tiff", 0, strPath + (hv_num.TupleString(".3d")));
	//thread_SaveImg.detach();
	//HalconCpp::WriteImage(*img, "tiff", 0, strPath + (hv_num.TupleString(".3d")));
}

void CSDKHalconDemoDlg::OnBnClickedOpencamera()
{
	// TODO: 在此添加控件通知处理程序代码  
	
	if ((status = GoSdk_Construct(&api)) != kOK)
	{
		CString strTemp;
		strTemp.Format(_T("Error: GoSdk_Construct:%d"), status); 
		WriteLog(strTemp);
		return;
	}

	// construct GoSystem object
	if ((status = GoSystem_Construct(&system, kNULL)) != kOK)
	{
		CString strTemp;
		strTemp.Format(_T("Error: GoSystem_Construct:%d"), status);
		WriteLog(strTemp);
		return;
	}

	// Parse IP address into address data structure
	kIpAddress_Parse(&ipAddress, SENSOR_IP);

	// obtain GoSensor object by sensor IP address
	if ((status = GoSystem_FindSensorByIpAddress(system, &ipAddress, &sensor)) != kOK)
	{
		ChangeMark = false;

		GetDlgItem(IDC_CHANGE_PATH)->EnableWindow(1);
		GetDlgItem(IDC_OpenCamera)->EnableWindow(1);
		GetDlgItem(IDC_PAUSECAM)->EnableWindow(0);
		GetDlgItem(IDC_StopCamera)->EnableWindow(0);
		GetDlgItem(IDC_AcquireImage)->EnableWindow(0);

		CString strTemp;
		strTemp.Format(_T("Error: GoSystem_FindSensor:%d"), status);
		WriteLog(strTemp);
		return;
	}

	// create connection to GoSensor object
	if ((status = GoSensor_Connect(sensor)) != kOK)
	{
		CString strTemp;
		strTemp.Format(_T("Error: GoSensor_Connect:%d"), status);
		WriteLog(strTemp);
		return;
	}

	// enable sensor data channel
	if ((status = GoSystem_EnableData(system, kTRUE)) != kOK)
	{
		CString strTemp;
		strTemp.Format(_T("Error: GoSensor_EnableData:%d"), status);
		WriteLog(strTemp);
		return;
	}

	ChangeMark = true;
	PauseMark = true;

	WriteLog(_T("相机打开成功！"));
}


void CSDKHalconDemoDlg::OnBnClickedStopcamera()
{
	// TODO: 在此添加控件通知处理程序代码
	ChangeMark = false;
	GetDlgItem(IDC_AcquireImage)->EnableWindow(0);
	GetDlgItem(IDC_StopCamera)->EnableWindow(0);
	GetDlgItem(IDC_PAUSECAM)->EnableWindow(0);
	GetDlgItem(IDC_CHANGE_PATH)->EnableWindow(1);
	GetDlgItem(IDC_OpenCamera)->EnableWindow(1);
	
	//暂停相机时会执行这句话，防止关闭出现故障
	try
	{
		// 停止传感器
		if ((status = GoSystem_Stop(system)) != kOK)
		{
			CString strTemp;
			strTemp.Format(_T("Error: GoSystem_Stop:%d"), status);
			WriteLog(strTemp);
			return;
		}

		// 销毁传感器对象
		GoDestroy(system);
		GoDestroy(api);

		WriteLog(_T("相机已关闭！"));
	}
	catch (const std::exception& ex)
	{
		printf(ex.what());
	}
	
}


void CSDKHalconDemoDlg::OnBnClickedAcquireimage()
{
	PauseMark = true;
	// TODO: 在此添加控件通知处理程序代码
	/*try
	{*/

	// set data handler to receive data asynchronously
	if ((status = GoSystem_SetDataHandler(system, onData, &contextPointer)) != kOK)
	{
		CString strTemp;
		strTemp.Format(_T("Error: GoSystem_SetDataHandler:%d"), status);
		WriteLog(strTemp);
		return;
	}

	// start Gocator sensor
	if ((status = GoSystem_Start(system)) != kOK)
	{
		CString strTemp;
		strTemp.Format(_T("Error: GoSystem_Start:%d"), status);
		WriteLog(strTemp);
		return;
	}
}


void CSDKHalconDemoDlg::OnEnChangeShowInfo()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	if (ChangeMark)
	{
		GetDlgItem(IDC_AcquireImage)->EnableWindow(1);
		GetDlgItem(IDC_PAUSECAM)->EnableWindow(1);
		GetDlgItem(IDC_StopCamera)->EnableWindow(1);
		GetDlgItem(IDC_CHANGE_PATH)->EnableWindow(0);
		GetDlgItem(IDC_OpenCamera)->EnableWindow(0);
	}
}


void CSDKHalconDemoDlg::OnBnClickedTest1()
{
	// TODO: 在此添加控件通知处理程序代码

	
	HalconCpp::HObject  ho_Image1, ho_Image2;
	ReadImage(&ho_Image1, "C:/Users/Administrator/Desktop/test/1/1/01.png");
	ReadImage(&ho_Image2, "C:/Users/Administrator/Desktop/test/1/2/01.png");
	//ReadImage(&ho_Image1, "E:/ZhangXuan/SDK_HalconDemo/1.png");
	//ReadImage(&ho_Image2, "E:/ZhangXuan/SDK_HalconDemo/2.png");
	//ReadImage(&ho_Image1, "C:/Users/Administrator/Desktop/1/001.tif");
	//ReadImage(&ho_Image2, "C:/Users/Administrator/Desktop/1/002.tif");
	//测试缺陷处理程序
	m_QX_JC->DealCamCpp(ho_Image1, ho_Image2);
	//std::thread thread_QX_JCObj(&DealCamFun::DealCamCpp, m_QX_JC, ho_Image1, ho_Image2);
	//thread_QX_JCObj.detach();
	//测试倒角尺寸测量程序
	m_SS_DJ->Deal_SS_DJ(ho_Image1, ho_Image2);
	//std::thread thread_DJ_JCObj(&SS_DJ_Measure::Deal_SS_DJ, m_SS_DJ, ho_Image1, ho_Image2);
	//thread_DJ_JCObj.detach();

}

void CSDKHalconDemoDlg::OnBnClickedPausecam()
{
	// TODO: 在此添加控件通知处理程序代码
	PauseMark = false;
	WriteLog(_T("暂停相机成功！"));
}


void CSDKHalconDemoDlg::OnBnClickedChangePath()
{
	// TODO: 在此添加控件通知处理程序代码
	// TODO: 在此添加控件通知处理程序代码
	HWND hwnd = GetSafeHwnd();   //得到窗口句柄
	CString SaveFilePath;

	LPMALLOC pMalloc;
	if (::SHGetMalloc(&pMalloc) == NOERROR)	//取得IMalloc分配器接口
	{
		BROWSEINFO bi;
		TCHAR pszBuffer[MAX_PATH];
		LPITEMIDLIST pidl;
		bi.hwndOwner = hwnd;
		bi.pidlRoot = NULL;
		bi.pszDisplayName = pszBuffer;
		bi.lpszTitle = _T("选择文件夹"); //选择目录对话框的上部分的标题
									//添加新建文件夹按钮 BIF_NEWDIALOGSTYLE
		bi.ulFlags = BIF_NEWDIALOGSTYLE | BIF_RETURNONLYFSDIRS | BIF_RETURNFSANCESTORS;
		bi.lpfn = NULL;
		if ((pidl = ::SHBrowseForFolder(&bi)) != NULL)  //取得IMalloc分配器接口
		{
			if (::SHGetPathFromIDList(pidl, pszBuffer)) //获得一个文件系统路径
			{
				SaveFilePath = pszBuffer;
			}
			pMalloc->Free(pidl);	//释放内存
			SetDlgItemText(IDC_FILEPATH_SHOW, SaveFilePath);
		}
		pMalloc->Release();	//释放接口
	}
}


void CSDKHalconDemoDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	OnBnClickedStopcamera();

	CDialogEx::OnClose();
}
