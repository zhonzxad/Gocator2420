#include "stdafx.h"
#include "SDK_HalconDemo.h"
#include "SDK_HalconDemoDlg.h"
#include "afxdialogex.h"
#include <afxpriv.h>
#include <algorithm>
#include <halconcpp/halconcpp.h>
#include <windows.h>
#include <string.h>
#include <thread>
#include <ctime>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#pragma warning(disable:4244)
#pragma warning(disable:6283)
#pragma warning(disable:6276)
#pragma warning(disable:6011)
#pragma warning(disable:6387)
#pragma warning(disable:26495)
#pragma warning(disable:26451)

// CSDKHalconDemoDlg 对话框

#define RECEIVE_TIMEOUT			20000000
#define INVALID_RANGE_16BIT     ((signed short)0x8000)          // gocator transmits range data as 16-bit signed integers. 0x8000 signifies invalid range data. 
#define DOUBLE_MAX              ((k64f)1.7976931348623157e+308) // 64-bit double - largest positive value.  
#define INVALID_RANGE_DOUBLE    ((k64f)-DOUBLE_MAX)             // floating point value to represent invalid range data.
#define NUM_PROFILES            2000

//#define SENSOR_IP               "192.168.1.11"
#define SENSOR_IP               "127.0.0.1"

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

//连接PLC标志位
bool conn_flag = false;

CSDKHalconDemoDlg::CSDKHalconDemoDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SDK_HALCONDEMO_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSDKHalconDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IP_CHOOSELIST, m_ipList);
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
	ON_STN_CLICKED(IDC_FILEPATH_SHOW, &CSDKHalconDemoDlg::OnStnClickedFilepathShow)
	ON_CBN_SELCHANGE(IDC_IP_CHOOSELIST, &CSDKHalconDemoDlg::OnCbnSelchangeIpChoose)
	ON_BN_CLICKED(IDC_START_GT, &CSDKHalconDemoDlg::OnBnClickedStartGt)
	ON_BN_CLICKED(IDC_STOP_GT, &CSDKHalconDemoDlg::OnBnClickedStopGt)
END_MESSAGE_MAP()


// CSDKHalconDemoDlg 消息处理程序

BOOL CSDKHalconDemoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	this->SetWindowText(L"Gocator系列相机采集程序");
	//WriteLog(L"程序正确开启");

	// TODO: 在此添加额外的初始化代码
	//::GetClientRect(GetDlgItem(IDC_IMG_HEIG)->m_hWnd, rc);
	//::GetClientRect(GetDlgItem(IDC_SHOW_IMG)->m_hWnd, rc);
	//image = new HalconCpp::HObject();


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

	//mg_dlg = this;
	//SENSOR_IP = "192.168.1.10";

	((CButton *)GetDlgItem(IDC_NOAUTO_RAD))->SetCheck(TRUE);
	((CButton *)GetDlgItem(IDC_TIME_RAD))->SetCheck(TRUE);
	//((CButton *)GetDlgItem(IDC_OUT_RAD))->SetCheck(TRUE);
	//((CButton *)GetDlgItem(IDC_TIME_RAD))->EnableWindow(0);
	//((CButton *)GetDlgItem(IDC_OUT_RAD))->EnableWindow(0);
	ChangeMark = 0;
	//WriteLog(L"默认选择手动采集");
	//WriteLog(L"默认选择时间触发采集");


	//CString IP_Add(SENSOR_IP);
	//((CIPAddressCtrl *)GetDlgItem(IDC_IP_ADDRESS))->SetAddress(*(DWORD *)SENSOR_IP);	//->SetAddress(SENSOR_IP);
	//((CIPAddressCtrl *)GetDlgItem(IDC_IP_ADDRESS))->SetAddress(192, 168, 1, 11);
	//GetDlgItem(IDC_IP_ADDRESS)->SetWindowText((LPCTSTR)SENSOR_IP);
	

	//long l = (DWORD)atol(SENSOR_IP);

	//初始化调用查找IP地址的函数
	//将打开相机的前几步骤放到这里
	prepareToFindIP();
	SearchCamFromIP();

	//int iPos = ((CComboBox*)GetDlgItem(IDC_IP_CHOOSE))->GetCurSel();//当前选中的行。
	((CComboBox*)GetDlgItem(IDC_IP_CHOOSELIST))->SetCurSel(0);//设置第n行内容为显示的内容。


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

//回调函数
kStatus kCall onData(void* ctx, void* sys, void* dataset)
{
	
	clock_t endTime, startTime;
	clock_t	Cam_runTime;	//拍摄时间量
	startTime = clock();

	//创建回调内部的调用指针
	CSDKHalconDemoDlg* m_pCallBack = (CSDKHalconDemoDlg*)(AfxGetApp()->m_pMainWnd);

	//创建图像对象指针;
	HalconCpp::HObject* ho_Height = new HalconCpp::HObject();
	HalconCpp::HObject* ho_Intens = new HalconCpp::HObject();

	//m_pCallBack->ho_Height = new HalconCpp::HObject();
	//m_pCallBack->ho_Intens = new HalconCpp::HObject();

	double ZResolution;

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
					m_pCallBack->WriteLog(strTemp);
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
				ZResolution = NM_TO_MM(GoSurfaceMsg_ZResolution(surfaceMsg));

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

				//将高度数据转化为Halcon数据,int2-tif unit2-png
				HalconCpp::GenImage1(ho_Height, "int2", width, length, (Hlong)myBuffer);

				//另一种转换方法
				HalconCpp::HObject  ho_ImageConverted;
				HalconCpp::ConvertImageType(*ho_Height, &ho_ImageConverted, "real");
				HalconCpp::ScaleImage(ho_ImageConverted, ho_Height, 1, 32768);
				HalconCpp::ConvertImageType(*ho_Height, ho_Height, "uint2");

				//显示图像到对应窗口，1表示高度图
				m_pCallBack->ShowImg(1, ho_Height);
				HalconCpp::DispObj(*ho_Height, hv_windowhandle);

				//HalconCpp::WriteImage(*ho_Height_Back, "png", 0, "E:\\wxm\\confine\\72-0.5\\18\\height\\" + (m_pCallBack->hv_a.TupleString(".2d")));
				//mg_dlg->SaveImg(mg_dlg->hv_a, ho_Height_Back);

				//线程存图方法
				/*CString strTemp = ("D:\\zhangxuan\\1\\height\\" + (m_pCallBack->hv_a.TupleString(".4d"))).S();
				//m_pCallBack->SaveImg(0, ho_Height_Back, strTemp);
				std::thread thread_SaveImg(&CSDKHalconDemoDlg::SaveImg, m_pCallBack, 1, ho_Height_Back, strTemp);
				thread_SaveImg.detach();*/

				//m_pCallBack->WriteLog(L"输出高度数据完成");
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

				//将亮度数据转换为Halcon数据 byte-tif byte-bmp
				HalconCpp::GenImage1(ho_Intens, "byte", width, length, (Hlong)myBuffer);

				//另一种转换方法
				/*HalconCpp::HObject  ho_ImageConverted;
				HalconCpp::ConvertImageType(*mg_dlg->ho_Intens, &ho_ImageConverted, "real");
				HalconCpp::ScaleImage(ho_ImageConverted, mg_dlg->ho_Intens, 256, 0);
				HalconCpp::ConvertImageType(*mg_dlg->ho_Intens, mg_dlg->ho_Intens, "uint2");
				ho_Intens_Back = mg_dlg->ho_Intens;*/
				
				//显示图像到对应窗口，2表示亮度图
				m_pCallBack->ShowImg(2, ho_Intens);
				HalconCpp::DispObj(*ho_Intens, hv_windowhandle);
				//HalconCpp::WriteImage(*ho_Intens_Back, "bmp", 0, "E:\\wxm\\confine\\72-0.5\\18\\intens\\" + (m_pCallBack->hv_b.TupleString(".2d")));
				//mg_dlg->SaveImg(mg_dlg->hv_b, ho_Intens_Back);

				//线程存图方法
				/*CString strTemp = ("D:\\zhangxuan\\1\\intens\\" + (m_pCallBack->hv_b.TupleString(".4d"))).S();
				//m_pCallBack->SaveImg(0, ho_Height_Back, strTemp);
				std::thread thread_SaveImg(&CSDKHalconDemoDlg::SaveImg, m_pCallBack, 2, ho_Intens_Back, strTemp);
				thread_SaveImg.detach();*/

				//m_pCallBack->WriteLog(L"输出亮度信息完成");
				//删除变量空间
				delete myBuffer;
			}
			break;
			}
		}
	
		endTime = clock();
		Cam_runTime = (double)(endTime - startTime) / CLOCKS_PER_SEC;

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
		//m_SS_DJ->Deal_SS_DJ(ZResolution,*ho_Height_Back, *ho_Intens_Back);
		
		//mg_dlg->SendImgToDeal(*ho_Height_Back, *ho_Intens_Back);
		 
		//进入保存图片程序中
		
		//回调函数计数操作
		CString str;
		str.Format(L"当前回调函数的计数为：%d", m_pCallBack->TimeCount);
		m_pCallBack->WriteLog(str);
		m_pCallBack->TimeCount += 1;

		//变量增加
		//适用于存放在相同的文件夹里
		/*mg_dlg->hv_a += 2;
		mg_dlg->hv_b += 2;*/
		//适用于存放在不同的文件夹里
		m_pCallBack->hv_a += 1;
		m_pCallBack->hv_b += 1;
		m_pCallBack->CamCount += 1;

		//清空缓存操作
		/*ho_Height_Back = NULL;
		ho_Intens_Back = NULL;
		m_pCallBack->ho_Height = NULL;
		m_pCallBack->ho_Intens = NULL;
		m_pCallBack = NULL;*/


		//拍摄计数停止位
		if (m_pCallBack->CamCount > NUM_PROFILES)
		{
			m_pCallBack->WriteLog(L"拍摄已完成！！");
			m_pCallBack->CamCount = 1;
			return kERROR;
		}
		else
		{
			//暂停采集标志位
			if (m_pCallBack->PauseMark)
			{
				delete ho_Height;
				delete ho_Intens;
				//delete m_pCallBack;
				m_pCallBack = NULL;
				return kOK;
			}  
			else
			{
				m_pCallBack->WriteLog(L"拍摄已暂停！！");
				m_pCallBack->CamCount = 1;

				free(ho_Height);
				free(ho_Intens);
			
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

//保存实际的点云拍摄数据到本地文件
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
		WriteLog(L"错误的文件名称");
	fclose(fpWrite);
	WriteLog(L"数据输出已完成");
}

CString CSDKHalconDemoDlg::OpenChangePathDlg()
{
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
			UpdateData();
		}
		pMalloc->Release();	//释放接口
	}


	return SaveFilePath;
}

void CSDKHalconDemoDlg::SearchCamFromIP()
{
	for (int i = 0; i < GoSystem_SensorCount(system); i++)
	{
		GoAddressInfo addressInfo;
		GoSensor sensor = GoSystem_SensorAt(system, i);

		if ((status = GoSensor_Address(sensor, &addressInfo)) == kOK)
		{
			kChar addressString[64];
			kIpAddress_Format(addressInfo.address, addressString, 64);
			printf("Discovers sensor %d with IP %s\n", GoSensor_Id(sensor), addressString);
			
			m_ipList.InsertString(i, (LPCTSTR)(CString)addressString);
			//.InsertString(i, addressString);
		}
	}


	//在选择IP是默认会选择一边采集模式
	if (((CButton *)GetDlgItem(IDC_AUTO_RAD))->GetCheck())
		WriteLog(L"选择了自动采集模式");
	if (((CButton *)GetDlgItem(IDC_NOAUTO_RAD))->GetCheck())
		WriteLog(L"选择了手动采集模式");
	if (((CButton *)GetDlgItem(IDC_TIME_RAD))->GetCheck())
		WriteLog(L"选择了时间触发模式");
	if (((CButton *)GetDlgItem(IDC_OUT_RAD))->GetCheck())
		WriteLog(L"选择了外部触发模式");
}

void CSDKHalconDemoDlg::prepareToFindIP()
{
	if ((status = kApiLib_Construct(&apiLib)) != kOK)
	{
		printf("Error: kApiLib_Construct:%d\n", status);
		return;
	}

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

	//显示句柄用完之后要记得及时销毁
	//销毁句柄的方式测试
	HalconCpp::CloseWindow(hv_windowhandle);
	//HDevWindowStack::Pop();
	//HalconCpp::CloseWindow(HDevWindowStack::Pop());
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

//保存拍摄后的图片数据到对应的文件
//-1表示参数错误  
//1 表示文件已存在
//Savemark 1表示高度图  2表示亮度图
void CSDKHalconDemoDlg::SaveImg(int SaveInfoMark, HalconCpp::HObject* img, CString filepath)
{
	//转换CString 到 char
	int len = WideCharToMultiByte(CP_ACP, 0, filepath, -1, NULL, 0, NULL, NULL);
	char *filepath_char = new char[len + 1];
	WideCharToMultiByte(CP_ACP, 0, filepath, -1, filepath_char, len, NULL, NULL);

	//成功则返回0，失败则返回-1，错误原因存于errno
	if (std::remove(filepath_char) == 0)
		WriteLog(L"成功移除原有已存在的文件\n");

	/*FILE *fp;
	if (filepath_char == NULL || filepath_char[0] == '\0') 
		return -1;//参数为空，即错误参数。
	
	else if (fopen(filepath_char, "r"))  //文件存在
	{
		//删除文件
		if (!remove(filepath_char))
			WriteLog(L"删除文件成功\n");
		else
			WriteLog(L"删除文件失败\n");

		//创建文件
		if (fopen(filepath_char, "w+"))
			WriteLog(L"创建文件夹成功\n");
		else
			WriteLog(L"创建文件夹失败\n");
	}
	else
	{
		//创建文件
		if (fopen(filepath_char, "w+"))
			WriteLog(L"创建文件夹成功\n");
		else
			WriteLog(L"创建文件夹失败\n");
	}*/
	/*else
	{
		fp = fopen(filepath_char, "r");//以只读方式打开
		if (fp)//打开成功，表示文件已经存在。
		{
			fclose(fp);//关闭文件

			return 1;//返回文件已存在。 
		}
		else
		{
			fp = fopen(filepath_char, "w+");//以只写方式创建文件。 w是为了输出操作 w+是为了输入输出操作  两张操作对于文件已存在的的，都会将文件舍弃，按照空的
			if (fp == NULL)
				return 2;//创建失败。
			fclose(fp);//关闭文件。
			//return 0;//创建成功。
		}

		return 0;//创建成功。
	}*/
	

	/*CString strPath; //= GetWindowText(IDC_STA_SHOWPATH,);
	GetDlgItem(IDC_FILEPATH_SHOW)->GetWindowTextW(filepath); //->GetWindowText(strPath);

	filepath += "\\";*/

	//创建线程失败，改为在外部创建
	//std::thread thread_SaveImg(&HalconCpp::WriteImage, *img, "tiff", 0, strPath + (hv_num.TupleString(".3d")));
	//thread_SaveImg.detach();
	//HalconCpp::WriteImage(*img, "tiff", 0, filepath + (hv_num.TupleString(".3d")));
	
	switch (SaveInfoMark)
	{
	case 1:  //1表示高度图
		HalconCpp::WriteImage(*img, "png", 0, filepath_char);
		WriteLog(L"保存高度图信息完成");
		break;
	case 2:	//2表示亮度图
		HalconCpp::WriteImage(*img, "bmp", 0, filepath_char);
		WriteLog(L"保存亮度图信息完成");
		break;
	default:
		break;
	}

}

void CSDKHalconDemoDlg::OnBnClickedOpencamera()
{
	// TODO: 在此添加控件通知处理程序代码 

	//前面几步放到了专用的函数中
	//为了方便先初始化寻找IP程序

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
		strTemp.Format(_T("寻找相机IP地址失败，错误码:%d"), status);
		WriteLog(strTemp);
		MessageBox(L"请重新点击开启相机按钮来重试连接相机");
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

	// retrieve setup handle
	if ((setup = GoSensor_Setup(sensor)) == kNULL)
	{
		printf("Error: GoSensor_Setup: Invalid Handle\n");
		return;
	}

	//触发模式的选择
	if (((CButton *)GetDlgItem(IDC_OUT_RAD))->GetCheck())
		GoSetup_SetTriggerSource(setup, GO_TRIGGER_INPUT);
	else if (((CButton *)GetDlgItem(IDC_TIME_RAD))->GetCheck())
		GoSetup_SetTriggerSource(setup, GO_TRIGGER_TIME);

	GoSensor_Flush(sensor);

	// enable sensor data channel
	if ((status = GoSystem_EnableData(system, kTRUE)) != kOK)
	{
		CString strTemp;
		strTemp.Format(_T("Error: GoSensor_EnableData:%d"), status);
		WriteLog(strTemp);
		return;
	}

	/*ChangeMark = true;
	PauseMark = true;
	GetDlgItem(IDC_AcquireImage)->EnableWindow(1);

	WriteLog(_T("相机打开成功！"));*/

	if (((CButton *)GetDlgItem(IDC_AUTO_RAD))->GetCheck())
	{
		//连接PLC
		if (ConnectPLC())
		{
			WriteLog(L"正确连接PLC");
			ChangeMark = true;
			PauseMark = true;
			GetDlgItem(IDC_AcquireImage)->EnableWindow(1);

			WriteLog(_T("相机打开成功！"));
		}
		else
		{
			ChangeMark = false;

			GetDlgItem(IDC_CHANGE_PATH)->EnableWindow(1);
			GetDlgItem(IDC_OpenCamera)->EnableWindow(1);
			GetDlgItem(IDC_PAUSECAM)->EnableWindow(0);
			GetDlgItem(IDC_StopCamera)->EnableWindow(0);
			GetDlgItem(IDC_AcquireImage)->EnableWindow(0);

			WriteLog(L"操作失败");
		}
	}
	else if (((CButton *)GetDlgItem(IDC_NOAUTO_RAD))->GetCheck())
	{
		ChangeMark = true;
		PauseMark = true;
		GetDlgItem(IDC_AcquireImage)->EnableWindow(1);

		WriteLog(_T("相机打开成功！"));
	}
	else
	{
		ChangeMark = false;

		GetDlgItem(IDC_CHANGE_PATH)->EnableWindow(1);
		GetDlgItem(IDC_OpenCamera)->EnableWindow(1);
		GetDlgItem(IDC_PAUSECAM)->EnableWindow(0);
		GetDlgItem(IDC_StopCamera)->EnableWindow(0);
		GetDlgItem(IDC_AcquireImage)->EnableWindow(0);

		WriteLog(L"操作失败");
	}
}

void CSDKHalconDemoDlg::OnBnClickedAcquireimage()
{
	PauseMark = true;
	GetDlgItem(IDC_AcquireImage)->EnableWindow(0);
	// TODO: 在此添加控件通知处理程序代码
	/*try
	{*/
	if (((CButton *)GetDlgItem(IDC_AUTO_RAD))->GetCheck())
		SyncWrite(5029, 1, L"PLC开始运行");

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
		//停止PLC
		if (((CButton *)GetDlgItem(IDC_AUTO_RAD))->GetCheck())
			SyncWrite(5030, 1, L"PLC停止");

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
		GoDestroy(apiLib);

		WriteLog(_T("相机已关闭！"));
	}
	catch (const std::exception& ex)
	{
		printf(ex.what());
	}
	
}


//链接状态回调，yong.shi
KS_RET WINAPI OnConnectStatusChanged(
	IN KS_HANDLE								Handle,
	IN KSIO_CONNECT_STATUS						ConnectStatus)
{
	if (ConnectStatus == KSIO_STATUS_PRIMARY_CONNECT || ConnectStatus == KSIO_STATUS_SECONDARY_CONNECT)
	{
		conn_flag = true;
	}

	return KS_OK;
}

//注册异步写变量回调
KS_RET WINAPI OnTagsValueWriteCallBack(IN KS_HANDLE							Handle,
	IN PKSIO_LONGARRAY						TagIDArray,
	IN PKSIO_LONGARRAY						TagAccessFieldIDArray,
	IN PKSIO_LONGARRAY						ErrorCodeArray)
{
	KSIOAPIFreeLongArray(TagIDArray);

	KSIOAPIFreeLongArray(TagAccessFieldIDArray);

	KSIOAPIFreeLongArray(ErrorCodeArray);

	KS_RET re = KS_FAIL;
	return re = KS_OK;
}


KS_RET WINAPI OnTagsValueCollectCallBack(IN KS_HANDLE Handle, IN PKSIO_TAG_VALUE_ARRAY TagValueArray)
{
	KS_UINT32 Number = TagValueArray->Number;
	KS_UINT32 nCount = 0;

	KS_RET hRet = KS_FAIL;
	CSDKHalconDemoDlg* zhis = (CSDKHalconDemoDlg*)(AfxGetApp()->m_pMainWnd);

	std::lock_guard<std::mutex> g(zhis->_mtx);
	for (nCount = 0; nCount < Number; nCount++)
	{
		KSIOTagValue IOTagValue = TagValueArray->TagValueArray[nCount];
		KS_UINT32 TagAccessID = IOTagValue.TagID;

		//读取回调的信息
		if (TagAccessID == 5003)
		{
			if (IOTagValue.TagValue.bitVal == 1)
			{
				CString strText;
				strText.Format(_T("系统急停开关按下"));
				zhis->WriteLog(strText);
			}
			else if (IOTagValue.TagValue.bitVal == 0)
			{
				CString strText;
				strText.Format(_T("系统急停开关抬起"));
				zhis->WriteLog(strText);
			}
		}
	}//FOR 
	KSIOAPIFreeTagsValue(TagValueArray);//此处一定要释放

	return hRet;
}

//连接PLC程序
bool CSDKHalconDemoDlg::ConnectPLC()
{
	// TODO: 在此添加控件通知处理程序代码
	auto algtime1 = std::chrono::steady_clock::now();
	m_ksConnectOption.ServerNetIP1 = (KS_WSTR)L"127.0.0.1";					// 服务端网络1IP
	m_ksConnectOption.ServerNetPort1 = 12380;					// 服务端网络1端口

	//m_ksConnectOption.ClientNetIP1  = NULL;

	m_ksConnectOption.ClientMachineName = (KS_WSTR)L"hp";				// Client机器名称

	m_ksConnectOption.ApplicationName = (KS_WSTR)L"OpcKSIO";						// 应用程序名称

	m_ksConnectOption.ClientType = 0;						// Client类型

	m_ksConnectOption.TimeOut = 5000;					// 连接超时，单位ms

	m_ksConnectOption.HeartBeatDetectInterval = 2;		//	心跳检测间隔，单位s
	m_ksConnectOption.MaxAllowErrorsBeforeDisconnect = 3;	//	关闭一个连接之前允许出现的最大连接错误数目

	conn_flag = false;

	KS_RET hRet = KSIOAPIServerConnect(&m_ksConnectOption, m_hIOServerHandle);

	//回调函数，在连上后再注册
	//注册链接状态回调
	KSIOAPIRegisterConnectStatusChangedCallbackFunc(m_hIOServerHandle, OnConnectStatusChanged);

	//注册异步写变量回调
	KSIOAPIRegisterWriteCompleteCallbackFunc(m_hIOServerHandle, OnTagsValueWriteCallBack);

	//注册周期读回调 ，自动去browse所有的tag 
	KSIOAPIRegisterCollectValueCallbackFunc(m_hIOServerHandle, OnTagsValueCollectCallBack);

	auto time1 = std::chrono::steady_clock::now();
	while (true)
	{
		if (conn_flag)
			break;

		auto time2 = std::chrono::steady_clock::now();
		if (std::chrono::duration_cast<std::chrono::seconds>(time2 - time1).count() > 3)
			break;

		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

	if (!(conn_flag && m_hIOServerHandle))
	{
		KSIOAPIServerDisconnect(m_hIOServerHandle);     //modified by yong.shi,2016-3-22
		AfxMessageBox(_T("连接服务器失败"));
		return false;
	}
	else
	{
		//GetDlgItem(IDC_ButCon)->EnableWindow(FALSE);
		//GetDlgItem(IDC_BUTTON_DisConn)->EnableWindow(TRUE);

		Subscribe(5033);
		Subscribe(5032);
		Subscribe(5002);
		Subscribe(5006);
		Subscribe(5013);
		Subscribe(5021);
		Subscribe(5027);
	}

	SYSTEMTIME sys_time;
	GetLocalTime(&sys_time);

	auto algtime2 = std::chrono::steady_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(algtime2 - algtime1).count();
	CString str;
	str.Format(_T("%lld"), duration);
	WriteLog(str);

	return true;
}

//PLC异步写方法
void CSDKHalconDemoDlg::SyncWrite(KS_UINT32 id, KS_BOOL val, LPCTSTR log)
{
	KSIOTagValue m_IOTagValue; //异步写时用

	m_IOTagValue.TagID = id;
	m_IOTagValue.TagValue.bitVal = val;
	m_IOTagValue.TagFieldID = 0;
	m_IOTagValue.QualityStamp = KDAQ_OPC_UNCERTAIN;
	m_IOTagValue.TimeStamp.Seconds = 0;
	m_IOTagValue.TimeStamp.Millisecs = 0;
	m_IOTagValue.TagValue.DataType = KSIO_TAG_DATATYPE_BOOL; // KSIO_TAG_DATATYPE_INT64

	PKSIO_TAG_VALUE_ARRAY pTagValueArray = new KSIOTagValueArray;
	KS_UINT32 nCount = 1;
	pTagValueArray->Number = nCount;
	pTagValueArray->TagValueArray = new KSIOTagValue[nCount];

	for (size_t Index = 0; Index < nCount; Index++)
	{
		pTagValueArray->TagValueArray[Index] = m_IOTagValue;
	}

	PKSIO_LONGARRAY	pErrorCodeArray = new KSIO_LONGARRAY;
	memset(pErrorCodeArray, 0, sizeof(KSIO_LONGARRAY));

	PKSIO_TIMESTAMP pTimestamp = new KSIO_TIMESTAMP;
	pTimestamp->Seconds = 0;
	pTimestamp->Millisecs = 0;

	if (KS_OK == KSIOAPISyncWriteTags(m_hIOServerHandle, FALSE, pTagValueArray, pErrorCodeArray, pTimestamp))
	{
		CString strLog;
		strLog.Format(_T("%s成功"), log);
		this->WriteLog(strLog);
	}
	else
	{
		CString strLog;
		strLog.Format(_T("%s失败"), log);
		this->WriteLog(strLog);
	}

	if (pTagValueArray != NULL)
	{
		if (pTagValueArray->TagValueArray != NULL)
		{
			delete[]  pTagValueArray->TagValueArray;
			pTagValueArray->TagValueArray = NULL;
		}
		delete pTagValueArray;
		pTagValueArray = NULL;
	}

	if (pErrorCodeArray != NULL)
	{
		if (pErrorCodeArray->LongArray != NULL)
		{
			delete[] pErrorCodeArray->LongArray;
			pErrorCodeArray->LongArray = NULL;
		}
		delete pErrorCodeArray;
		pErrorCodeArray = NULL;
	}

	delete pTimestamp;
}

//PLC读方法
KS_BOOL CSDKHalconDemoDlg::Read(KS_UINT32 id)
{
	PKSIO_LONGARRAY pTagIDArray = new KSIO_LONGARRAY;
	KS_UINT32 nCount = 1;
	pTagIDArray->Number = nCount;
	pTagIDArray->LongArray = new KS_UINT32[nCount];
	for (size_t Index = 0; Index < nCount; Index++) //全部读一次
	{
		pTagIDArray->LongArray[Index] = id;
	}

	PKSIO_LONGARRAY	pErrorCodeArray = new KSIO_LONGARRAY;
	memset(pErrorCodeArray, 0, sizeof(KSIO_LONGARRAY));

	//add by yong.shi
	PKSIO_LONGARRAY	pTagAccessFieldIDArray = new KSIO_LONGARRAY;
	pTagAccessFieldIDArray->Number = nCount;
	pTagAccessFieldIDArray->LongArray = new KS_UINT32[nCount];
	for (size_t Index = 0; Index < nCount; Index++)
	{
		pTagAccessFieldIDArray->LongArray[Index] = 0;
	}
	//end

	PKSIO_TAG_VALUE_ARRAY pTagValueArray = new KSIOTagValueArray;
	pTagValueArray->Number = nCount;
	pTagValueArray->TagValueArray = new KSIOTagValue[nCount];
	ZeroMemory(pTagValueArray, sizeof(KSIOTagValueArray)); //必须初始化

	PKSIO_TIMESTAMP pTimestamp = new KSIO_TIMESTAMP;
	pTimestamp->Seconds = 0;
	pTimestamp->Millisecs = 0;

	KS_INT64 result = 0;
	KS_RET hRet = KSIOAPISyncReadTags(m_hIOServerHandle, KSIO_DS_CACHE, pTagIDArray, pTagAccessFieldIDArray, pTagValueArray, pErrorCodeArray, pTimestamp);

	if (hRet == KS_OK)
	{
		if (pTagValueArray->TagValueArray)
		{
			result = pTagValueArray->TagValueArray->TagValue.i8Val;
		}
	}

	if (pTagIDArray != NULL)
	{
		if (pTagIDArray->LongArray != NULL)
		{
			delete[] pTagIDArray->LongArray;
			pTagIDArray->LongArray = NULL;
		}
		delete pTagIDArray;
		pTagIDArray = NULL;
	}

	if (pTagAccessFieldIDArray != NULL)
	{
		if (pTagAccessFieldIDArray->LongArray != NULL)
		{
			delete[] pTagAccessFieldIDArray->LongArray;
			pTagAccessFieldIDArray->LongArray = NULL;
		}
		delete pTagAccessFieldIDArray;
		pTagAccessFieldIDArray = NULL;
	}

	if (pErrorCodeArray != NULL)
	{
		if (pErrorCodeArray->LongArray != NULL)
		{
			delete[] pErrorCodeArray->LongArray;
			pErrorCodeArray->LongArray = NULL;
		}
		delete pErrorCodeArray;
		pErrorCodeArray = NULL;
	}

	if (pTagValueArray != NULL)
	{
		if (pTagValueArray->TagValueArray != NULL)
		{
			delete[]  pTagValueArray->TagValueArray;
			pTagValueArray->TagValueArray = NULL;
		}
		delete pTagValueArray;
		pTagValueArray = NULL;
	}

	delete pTimestamp;

	//KSIOAPIFreeLongArray(ErrorCodeArray);	

	return result;
}

//PLC订阅方法
void CSDKHalconDemoDlg::Subscribe(KS_UINT32 id, bool isSub)
{
	//订阅点 全部订阅
	PKSIO_LONGARRAY pTagAccessIDArray = new KSIO_LONGARRAY;
	KS_UINT32 nCount = 1;
	pTagAccessIDArray->Number = nCount;
	pTagAccessIDArray->LongArray = new KS_UINT32[nCount];
	for (size_t Index = 0; Index < nCount; Index++)
	{
		pTagAccessIDArray->LongArray[Index] = id;
	}

	PKSIO_LONGARRAY	pErrorCodeArray = new KSIO_LONGARRAY;
	memset(pErrorCodeArray, 0, sizeof(KSIO_LONGARRAY));
	//add by yong.shi
	PKSIO_LONGARRAY	pTagAccessFieldIDArray = new KSIO_LONGARRAY;
	pTagAccessFieldIDArray->Number = nCount;
	pTagAccessFieldIDArray->LongArray = new KS_UINT32[nCount];
	for (size_t Index = 0; Index < nCount; Index++)
	{
		pTagAccessFieldIDArray->LongArray[Index] = 0;
	}

	if (isSub)
	{
		KS_RET  hRet = KSIOAPISubscribeTagValuesChange(m_hIOServerHandle, pTagAccessIDArray, pTagAccessFieldIDArray, pErrorCodeArray);
		if (hRet != KS_OK)
		{
			this->WriteLog(_T("订阅数据失败"));
		}
	}
	else
	{
		//取消订阅
		KS_RET  hRet = KSIOAPIUnsubscribeTagValuesChange(m_hIOServerHandle, pTagAccessIDArray, pTagAccessFieldIDArray, pErrorCodeArray);
		if (hRet != KS_OK)
		{
			this->WriteLog(_T("取消订阅失败"));
		}
	}

	//释放内存
	if (pTagAccessIDArray != NULL)
	{
		if (pTagAccessIDArray->LongArray != NULL)
		{
			delete[] pTagAccessIDArray->LongArray;
			pTagAccessIDArray->LongArray = NULL;
		}
		delete pTagAccessIDArray;
		pTagAccessIDArray = NULL;
	}

	if (pTagAccessFieldIDArray != NULL)
	{
		if (pTagAccessFieldIDArray->LongArray != NULL)
		{
			delete[] pTagAccessFieldIDArray->LongArray;
			pTagAccessFieldIDArray->LongArray = NULL;
		}
		delete pTagAccessFieldIDArray;
		pTagAccessFieldIDArray = NULL;
	}

	if (pErrorCodeArray != NULL)
	{
		if (pErrorCodeArray->LongArray != NULL)
		{
			delete[] pErrorCodeArray->LongArray;
			pErrorCodeArray->LongArray = NULL;
		}
		delete pErrorCodeArray;
		pErrorCodeArray = NULL;
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
		//GetDlgItem(IDC_AcquireImage)->EnableWindow(1);  //目的是当采集按钮点击之后不允许再次点击
		GetDlgItem(IDC_PAUSECAM)->EnableWindow(1);
		GetDlgItem(IDC_StopCamera)->EnableWindow(1);
		GetDlgItem(IDC_CHANGE_PATH)->EnableWindow(0);
		GetDlgItem(IDC_OpenCamera)->EnableWindow(0);
	}

	if (((CButton *)GetDlgItem(IDC_AUTO_RAD))->GetCheck() ||
		((CButton *)GetDlgItem(IDC_NOAUTO_RAD))->GetCheck())
	{
		((CButton *)GetDlgItem(IDC_TIME_RAD))->EnableWindow(1);
		((CButton *)GetDlgItem(IDC_OUT_RAD))->EnableWindow(1);
	}

}


void CSDKHalconDemoDlg::OnBnClickedTest1()
{
	// TODO: 在此添加控件通知处理程序代码

	//OnBnClickedChangePath();

	CSDKHalconDemoDlg::OpenChangePathDlg();
	
	//HalconCpp::HObject  ho_Image1, ho_Image2;
	//ReadImage(&ho_Image1, "C:/Users/Administrator/Desktop/test/1/1/01.png");
	//ReadImage(&ho_Image2, "C:/Users/Administrator/Desktop/test/1/2/01.png");
	//ReadImage(&ho_Image1, "E:/ZhangXuan/SDK_HalconDemo/1.png");
	//ReadImage(&ho_Image2, "E:/ZhangXuan/SDK_HalconDemo/2.png");
	//ReadImage(&ho_Image1, "C:/Users/Administrator/Desktop/1/001.tif");
	//ReadImage(&ho_Image2, "C:/Users/Administrator/Desktop/1/002.tif");
	//测试缺陷处理程序
	//m_QX_JC->DealCamCpp(ho_Image1, ho_Image2);
	//std::thread thread_QX_JCObj(&DealCamFun::DealCamCpp, m_QX_JC, ho_Image1, ho_Image2);
	//thread_QX_JCObj.detach();
	//测试倒角尺寸测量程序
	//m_SS_DJ->Deal_SS_DJ(0.015,ho_Image1, ho_Image2);
	//std::thread thread_DJ_JCObj(&SS_DJ_Measure::Deal_SS_DJ, m_SS_DJ, ho_Image1, ho_Image2);
	//thread_DJ_JCObj.detach();

}

void CSDKHalconDemoDlg::OnBnClickedPausecam()
{
	// TODO: 在此添加控件通知处理程序代码
	PauseMark = false;
	GetDlgItem(IDC_AcquireImage)->EnableWindow(1);
	WriteLog(_T("暂停相机成功！"));
}


void CSDKHalconDemoDlg::OnBnClickedChangePath()
{
	// TODO: 在此添加控件通知处理程序代码
	//更改存储路径确认按钮
}


void CSDKHalconDemoDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	OnBnClickedStopcamera();

	CDialogEx::OnClose();
}


void CSDKHalconDemoDlg::OnStnClickedFilepathShow()
{
	// TODO: 在此添加控件通知处理程序代码

	//OnBnClickedChangePath();

	//if (((CButton *)GetDlgItem(IDC_CHANGE_PATH))->GetCheck())
	
	//点击输入框出现对应的选择存储地址按钮
	CSDKHalconDemoDlg::OpenChangePathDlg();
}


void CSDKHalconDemoDlg::OnCbnSelchangeIpChoose()
{
	// TODO: 在此添加控件通知处理程序代码

	int index = m_ipList.GetCurSel();//获取当前索引
	CString strTemp;
	m_ipList.GetLBText(index, strTemp);

	//声明标识
	USES_CONVERSION;
	//函数T2A和W2A均支持ATL和MFC中的字符
	//SENSOR_IP = T2A(strTemp);
	CString str = L"当前IP地址改为：";
	str += strTemp;
	WriteLog(str);


	//用宏定义改IP地址方法失败
	/*#ifndef SENSOR_IP
	#define SENSOR_IP strTemp
	#endif*/

	/*#ifdef SENSOR_IP
	#undef SENSOR_IP
	#endif

	#ifndef SENSOR_IP
	#define SENSOR_IP strTemp
	#endif*/	

	//每一次选择完IP地址后都会默认更新一遍采集方式
	if (((CButton *)GetDlgItem(IDC_AUTO_RAD))->GetCheck())
		WriteLog(L"选择了自动采集模式");
	if (((CButton *)GetDlgItem(IDC_NOAUTO_RAD))->GetCheck())
		WriteLog(L"选择了手动采集模式");
	if (((CButton *)GetDlgItem(IDC_TIME_RAD))->GetCheck())
		WriteLog(L"选择了时间触发模式");
	if (((CButton *)GetDlgItem(IDC_OUT_RAD))->GetCheck())
		WriteLog(L"选择了外部触发模式");
}


void CSDKHalconDemoDlg::OnBnClickedStartGt()
{
	// TODO: 在此添加控件通知处理程序代码
	//先ConnectPLC 然后再使用syswrite
}


void CSDKHalconDemoDlg::OnBnClickedStopGt()
{
	// TODO: 在此添加控件通知处理程序代码
	//先ConnectPLC 然后再使用syswrite
}
