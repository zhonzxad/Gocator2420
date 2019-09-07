
//==============================================================================
/// <summary>
///		IOServer访问句柄接口定义文件
/// </summary>
///	<file>
///		项目名:	组态王7.0
///		文件名: IkxIOServerHandle.h
///		作  者: 段长明
///	</file>
///	<copyright>
/// 	本代码所有版权为亚控科技所有，任何个人或者组织不得以任何方式复制、转载以挪作
/// 他用，否则将构成对亚控科技的产权知识侵权。
///	
///		联系: http://www.kingview.com/
///	</copyright>
///	<versions>
///		<version>
///			V0.10 : 段长明 2009-05-12 10:01 创建文件
///			V0.20 : 段长明 2009-09-01  支持冗余节点	
///		</version>
///	</versions>
//==============================================================================

//==============================================================================
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __IkxIOServerHandle_H_INCLUDED__
#define __IkxIOServerHandle_H_INCLUDED__
class CIOServerNode;
BEGIN_NAMESPACE(NkxIOAPI)

//==============================================================================
//	IOServer访问接口接口标识符
//==============================================================================
// {D6450ABB-00B6-4395-9BF6-9E49A3968382}
static const CkxIID KXIID_IkxIOServerHandle = 
{ 0xd6450abb, 0xb6, 0x4395, { 0x9b, 0xf6, 0x9e, 0x49, 0xa3, 0x96, 0x83, 0x82 } };

//==============================================================================
//	IOServer访问句柄接口
//==============================================================================
struct IkxIOServerHandle : IkxUnknown
{
	//==============================================================================
	//
	// 一、网络通讯接口
	//
	//==============================================================================


	//==============================================================================
	//	同步网络连接
	//==============================================================================

	/// <summary>
	/// 	同步网络通讯连接
	/// </summary>
	/// <returns>
	///		如果成功返回KVIO_OK,如果失败返回KVIO_FAIL
	/// </returns>
	/// <remarks>
	///     
	/// </remarks>
	virtual HRESULT SyncNetworkCommConnect() = 0;

	/// <summary>
	/// 	同步网络通讯断开连接
	/// </summary>
	/// <returns>
	///		如果成功返回KVIO_OK,如果失败返回KVIO_FAIL
	/// </returns>
	/// <remarks>
	///     
	/// </remarks>
	virtual HRESULT SyncNetworkCommDisConnect() = 0;

	//==============================================================================
	//	网络通讯服务,适用于异步通讯连接方式
	//	连接信息通过IkxIOServerCallBack中的OnConnectWithServer和OnDisConnectWithServer来反馈
	//==============================================================================

	/// <summary>
	/// 	启动网络通讯服务
	/// </summary>
	/// <returns>
	///		如果成功返回KVIO_OK,如果失败返回KVIO_FAIL
	/// </returns>
	/// <remarks>
	///     
	/// </remarks>
	virtual HRESULT StartNetworkCommService() = 0;


	/// <summary>
	/// 	停止网络通讯服务
	/// </summary>
	/// <returns>
	///		如果成功返回KVIO_OK,如果失败返回KVIO_FAIL
	/// </returns>
	/// <remarks>
	///     
	/// </remarks>
	virtual HRESULT StopNetworkCommService() = 0;

	//==============================================================================
	//	连接信息
	//==============================================================================

	/// <summary>
	/// 	获取连接状态
	/// </summary>
	/// <param name="bConnect">
	///		连接状态，TRUE:连接 FALSE连接断开
	/// </param>
	/// <returns>
	///		如果成功返回KVIO_OK,如果失败返回KVIO_FAIL
	/// </returns>
	/// <remarks>
	///     如果是双IO冗余，只要能连接上某个IO即为连接上
	/// </remarks>
	virtual HRESULT IsConnected(OUT BOOL& bConnect) = 0;

	/// <summary>
	/// 	获取主IOServer连接状态
	/// </summary>
	/// <param name="bConnect">
	///		连接状态，TRUE:连接 FALSE连接断开
	/// </param>
	/// <returns>
	///		如果成功返回KVIO_OK,如果失败返回KVIO_FAIL
	/// </returns>
	/// <remarks>
	///     
	/// </remarks>
	virtual HRESULT IsPrimaryIOServerConnected(OUT BOOL& bConnect) = 0;

	/// <summary>
	/// 	获取从IOServer连接状态
	/// </summary>
	/// <param name="bConnect">
	///		连接状态，TRUE:连接 FALSE连接断开
	/// </param>
	/// <returns>
	///		如果成功返回KVIO_OK,如果失败返回KVIO_FAIL
	/// </returns>
	/// <remarks>
	///     
	/// </remarks>
	virtual HRESULT IsSecondaryIOServerConnected(OUT BOOL& bConnect) = 0;

	/// <summary>
	/// 	获取主IO当前连接的IP地址
	/// </summary>
	/// <param name="strCurConnectIP">
	///		当前连接的IP地址
	/// </param>
	/// <returns>
	///		如果成功返回KVIO_OK,如果失败返回KVIO_FAIL
	/// </returns>
	/// <remarks>
	///     
	/// </remarks>
	virtual HRESULT GetPrimaryIOServerConnectedIP(OUT std::wstring& strCurConnectIP) = 0;

	/// <summary>
	/// 	获取连接主IO的SCADA信息
	/// </summary>
	/// <param name="ClientScadaInfoSeq">
	///		连接主IO的SCADA信息
	/// </param>
	/// <returns>
	///		如果成功返回KVIO_OK,如果失败返回KVIO_FAIL
	/// </returns>
	/// <remarks>
	///     
	/// </remarks>
	virtual HRESULT GetScadaClientInfo(OUT std::vector<ScadaClientInfo>& ClientScadaInfoSeq) = 0;

	/// <summary>
	/// 	获取从IO当前连接的IP地址
	/// </summary>
	/// <param name="strCurConnectIP">
	///		当前连接的IP地址
	/// </param>
	/// <returns>
	///		如果成功返回KVIO_OK,如果失败返回KVIO_FAIL
	/// </returns>
	/// <remarks>
	///     
	/// </remarks>
	virtual HRESULT GetSecondaryIOServerConnectedIP(OUT std::wstring& strCurConnectIP) = 0;

	//==============================================================================
	// 
	//	二、开发配置接口
	//
	//==============================================================================

	//获取变量访问ID
	virtual HRESULT GetTagAccessIDsByNames(
		IN  const std::vector<std::wstring>&	    TagNames,
		OUT std::vector<DWORD>&						TagAccessIDs,
		OUT std::vector<HRESULT>&					ErrorCodes)=0;


	//==============================================================================
	//
	//	三、运行访问接口
	//
	//==============================================================================

	//==============================================================================
	//	IOServer状态
	//==============================================================================

	/// <summary>
	/// 	获取IOServer工作状态
	/// </summary>
	/// <param name="Status">
	///		IOServer工作状态
	/// </param>
	/// <returns>
	///		如果成功返回KVIO_OK,如果失败返回相应错误码
	/// </returns>
	/// <remarks>
	///     IOServer工作状态，具体定义参看kxWorkStatus
	///		如果是双IO冗余，获取的是激活机器的状态
	/// </remarks>
	virtual HRESULT GetWorkStatus(OUT  WORD& Status) = 0;

	/// <summary>
	/// 	获取IOServer运行状态
	/// </summary>
	/// <param name="Status">
	///		IOServer运行状态
	/// </param>
	/// <returns>
	///		如果成功返回KVIO_OK,如果失败返回相应错误码
	/// </returns>
	/// <remarks>
	///     IOServer运行状态，具体定义参看kxRunningStatus
	///		如果是双IO冗余，获取的是激活机器的状态
	/// </remarks>
	virtual HRESULT GetRunningStatus(OUT WORD& Status ) = 0;

	/// <summary>
	/// 	获取主IOServer的状态
	/// </summary>
	/// <param name="Status">
	///		主IOServer状态
	/// </param>
	/// <returns>
	///		如果成功返回KVIO_OK,如果失败返回相应错误码
	/// </returns>
	/// <remarks>
	///     IOServer状态，具体定义参看kxServerStatus
	/// </remarks>
	virtual HRESULT GetPrimaryIOServerStatus(OUT  WORD& Status) = 0;

	/// <summary>
	/// 	获取从IOServer的状态
	/// </summary>
	/// <param name="Status">
	///		从IOServer状态
	/// </param>
	/// <returns>
	///		如果成功返回KVIO_OK,如果失败返回相应错误码
	/// </returns>
	/// <remarks>
	///     IOServer状态，具体定义参看kxServerStatus
	/// </remarks>
	virtual HRESULT GetSecondaryIOServerStatus(OUT  WORD& Status) = 0;

	/// <summary>
	/// 	获取IOServer冗余状态
	/// </summary>
	/// <param name="bPrimary">
	///		是否为Primary IOServer
	/// </param>
	/// <param name="bActive">
	///		是否激活，TRUE为激活，FALSE为待机
	/// </param>
	/// <returns>
	///		如果成功返回KVIO_OK,如果失败返回相应错误码
	/// </returns>
	virtual HRESULT GetRedunActiveFlag(
		IN BOOL								bPrimary ,
		OUT BOOL&							bActive ) = 0;

	/// <summary>
	/// 	获取IOServer冗余状态
	/// </summary>
	/// <param name="Status">
	///		冗余工作状态，见kxRedunStatus
	/// </param>
	/// <returns>
	///		如果成功返回KVIO_OK,如果失败返回相应错误码
	/// </returns>
	virtual HRESULT GetRedundantStatus(
		OUT WORD&							Status,
		IN	BOOL	bAsync = TRUE,
		IN	INT		nTimeout = 3000) = 0;	//modified by pengfei.xing 2013/09/04


	//==============================================================================
	//	IOServer同步读写接口
	//	注：调用驱动程序的IOServer，不支持同步读写接口，需要相关功能的请使用异步读写接口
	//==============================================================================

	/// <summary>
	/// 	同步读操作
	/// </summary>
	/// <param name="TagAccessIDs">
	/// 	变量ID序列
	/// </param>
	/// <param name="TagAccessFieldIDs">
	/// 	变量对应域ID序列
	/// </param>
	/// <param name="TagsValue">
	/// 	变量值序列
	/// </param>
	/// <param name="ErrorCodes">
	/// 	错误码序列
	/// </param>
	/// <returns>
	///		如果成功返回KVIO_OK,如果失败返回相应错误码
	/// </returns>
	/// <remarks>
	///		调用驱动程序的IOServer，不支持同步读写接口
	///		如果是双IO冗余，实际操作的是激活的IO
	/// </remarks>
	virtual HRESULT SyncReadVariables(
		IN const std::vector<DWORD> &		TagAccessIDs,
		IN const std::vector<DWORD> &		TagAccessFieldIDs,
		IN WORD								DataSource,
		OUT std::vector<kxTagValue> &		TagsValue,
		OUT std::vector<HRESULT> &			ErrorCodes) = 0;

	/// <summary>
	/// 	同步写操作
	/// </summary>
	/// <param name="TagsValue">
	/// 	变量值序列
	/// </param>
	/// <param name="ErrorCodes">
	/// 	错误码序列
	/// </param>
	/// <returns>
	///		如果成功返回KVIO_OK,如果失败返回相应错误码
	/// </returns>
	/// <remarks>
	///		调用驱动程序的IOServer，不支持同步读写接口
	///		如果是双IO冗余，实际操作的是激活的IO
	/// </remarks>
	virtual HRESULT SyncWriteVariables(
		IN const std::vector<kxTagValue> &	TagsValue,
		OUT std::vector<HRESULT>&			ErrorCodes) = 0;


	//==============================================================================
	//	IOServer变量异步读写接口
	//==============================================================================

	/// <summary>
	/// 	异步读操作
	/// </summary>
	/// <param name="TagAccessIDs">
	///		变量ID序列
	/// </param>
	/// <param name="TagAccessFieldIDs">
	/// 	变量对应域ID序列
	/// </param>
	/// <param name="ErrorCodes">
	///		错误码序列
	/// </param>
	/// <returns>
	///		如果成功返回KVIO_OK,如果失败返回相应错误码
	/// </returns>
	/// <remarks>
	///     单次读，变量值通过回调中的OnReceivedVaribleValue返回
	///		如果是双IO冗余，实际操作的是激活的IO
	/// </remarks>
	virtual HRESULT AsyncReadVariables(
		IN const std::vector<DWORD>  &		TagAccessIDs, 
		IN const std::vector<DWORD> &		TagAccessFieldIDs,
		IN WORD								DataSource,
		OUT std::vector<HRESULT>&			ErrorCodes) =0;

	/// <summary>
	/// 	异步写操作
	/// </summary>
	/// <param name="TagsValue">
	///		变量值序列
	/// </param>
	/// <param name="ErrorCodes">
	///		错误码序列
	/// </param>
	/// <returns>
	///		如果成功返回KVIO_OK,如果失败返回相应错误码
	/// </returns>
	/// <remarks>
	///     异步写操作的执行结果，通过回调中的OnWriteVariblesComplete返回
	///		如果是双IO冗余，实际操作的是激活的IO
	/// </remarks>
	virtual HRESULT AsyncWriteVariables(
		IN const std::vector<kxTagValue>  &	TagsValue, 
		OUT std::vector<HRESULT>&			ErrorCodes) = 0;


	//==============================================================================
	//	IOServer周期采集接口
	//==============================================================================

	/// <summary>
	/// 	周期采集变量
	/// </summary>
	/// <param name="TagAccessIDs">
	///		变量ID序列
	/// </param>
	/// <param name="TagAccessFieldIDs">
	/// 	变量对应域ID序列
	/// </param>
	/// <param name="ErrorCodes">
	///		错误码序列
	/// </param>
	/// <param name="NodeTypeForRedun">
	///		双IO冗余时的节点类型，如果不是冗余不用管该参数，也可以为KVIO_NODE_SINGLE
	/// </param>
	/// <returns>
	///		如果成功返回KVIO_OK,如果失败返回相应错误码
	/// </returns>
	/// <remarks>
	///     变量值通过回调中的OnReceivedVaribleValue返回
	/// </remarks>
	virtual HRESULT ReadTagsByInterval(
		IN const std::vector<DWORD> &		TagAccessIDs, 
		IN const std::vector<DWORD> &		TagAccessFieldIDs,
		OUT std::vector<HRESULT>&			ErrorCodes,
		IN WORD								NodeTypeForRedun ) =0;	

	/// <summary>
	/// 	停止周期采集变量
	/// </summary>
	/// <param name="TagAccessIDs">
	///		变量ID序列
	/// </param>
	/// <param name="TagAccessFieldIDs">
	/// 	变量对应域ID序列
	/// </param>
	/// <param name="ErrorCodes">
	///		错误码序列
	/// </param>
	/// <param name="NodeTypeForRedun">
	///		双IO冗余时的节点类型，如果不是冗余不用管该参数，也可以为KVIO_NODE_SINGLE
	/// </param>
	/// <returns>
	///		如果成功返回KVIO_OK,如果失败返回相应错误码
	/// </returns>
	/// <remarks>
	///     
	/// </remarks>
	virtual HRESULT StopReadTagsByInterval(
		IN const std::vector<DWORD> &		TagAccessIDs, 
		IN const std::vector<DWORD> &		TagAccessFieldIDs,
		OUT std::vector<HRESULT>&			ErrorCodes,
		IN WORD								NodeTypeForRedun ) =0;

	//add by hongchao.li 2012/09/07
	//Task ID:25606 IOServer AdjustTime
	//==============================================================================
	//	设置校时
	//==============================================================================

	/// <summary>
	/// 	设置向IOServer所在机器校时
	/// </summary>
	/// <param name="bAdjustTime">
	/// 	是否校时
	/// </param>
	/// <param name="AdjustTimeInterval">
	/// 	校时间隔，单位s
	/// </param>
	/// <param name="NodeTypeForRedun">
	///		双IO冗余时的节点类型，如果不是冗余不用管该参数，也可以为KVIO_NODE_SINGLE
	/// </param>
	/// <returns>
	///		如果成功返回KVIO_OK,如果失败返回相应错误码
	/// </returns>
	/// <remarks>
	///		不设置即默认状态，不校时
	/// </remarks>
	virtual HRESULT SetMachineAdjustTime(
		IN BOOL											bAdjustTime,
		IN DWORD										AdjustTimeInterval,
		IN WORD											NodeTypeForRedun = KVIO_NODE_REDUN_BOTH)  = 0;

	//add end by hongchao.li 2012/09/07	

	// Added by chunming.suo 2014/05/06 16:54:39 [增加IO独立运行的KS授权校验] Begin
	/// <summary>
	/// 	向IOServer所在机器设置KS授权
	/// </summary>
	/// <param name="bDemoFlag">
	/// 	是否Demo
	/// </param>
	/// <param name="NodeTypeForRedun">
	///		双IO冗余时的节点类型，如果不是冗余不用管该参数，也可以为KVIO_NODE_SINGLE
	/// </param>
	/// <returns>
	///		如果成功返回KVIO_OK,如果失败返回相应错误码
	/// </returns>
	/// <remarks>
	///		
	/// </remarks>
	virtual HRESULT SetKSDemoFlag(
		IN BOOL											bDemoFlag,
		IN WORD											NodeTypeForRedun ) = 0;
	// End

	//==============================================================================
	//
	//	四、运行监视接口
	//
	//==============================================================================

	//==============================================================================
	//	5、IOServer运行性能监视
	//==============================================================================

	/// <summary>
	/// 	获取当前CPU使用信息
	/// </summary>
	/// <param name="CurCPUUse">
	/// 	当前CPU使用率
	/// </param>
	/// <param name="CurCPUPercentUse">
	/// 	当前CPU使用率百分比
	/// </param>
	/// <param name="Reserved1">
	/// 	保留参数1
	/// </param>
	/// <param name="Reserved2">
	/// 	保留参数2
	/// </param>
	/// <returns>
	///		如果成功返回KVIO_OK,如果失败返回相应错误码
	/// </returns>
	/// <remarks>
	///		
	/// </remarks>
	virtual HRESULT GetCurrentCPUConsume(
		OUT DWORD&							CurCPUUse,
		OUT WORD&							CurCPUPercentUse,
		OUT DWORD&							Reserved1,
		OUT DWORD&							Reserved2) = 0;

	/// <summary>
	/// 	获取当前内存使用信息
	/// </summary>
	/// <param name="CurMemUse">
	/// 	当前内存使用率
	/// </param>
	/// <param name="CurMemPercentUse">
	/// 	当前内存使用率百分比
	/// </param>
	/// <param name="Reserved1">
	/// 	保留参数1
	/// </param>
	/// <param name="Reserved2">
	/// 	保留参数2
	/// </param>
	/// <returns>
	///		如果成功返回KVIO_OK,如果失败返回相应错误码
	/// </returns>
	/// <remarks>
	///		
	/// </remarks>
	virtual HRESULT GetCurrentMemComsume(
		OUT DWORD&							CurMemUse,
		OUT WORD&							CurMemPercentUse,
		OUT DWORD&							Reserved1,
		OUT DWORD&							Reserved2) = 0;

	/// <summary>
	/// 	获取当前运行的线程个数
	/// </summary>
	/// <param name="CurThreadNum">
	/// 	当前运行的线程个数
	/// </param>
	/// <returns>
	///		如果成功返回KVIO_OK,如果失败返回相应错误码
	/// </returns>
	/// <remarks>
	///		
	/// </remarks>
	virtual HRESULT GetCurrentThreadNum(OUT WORD&	CurThreadNum ) = 0;

	//==============================================================================
	//	注册回调函数
	//==============================================================================
	/// <summary>
	/// 	注册回调函数
	/// </summary>
	/// <param name="pfnIOServerCallBackFunc">
	/// 	回调接口函数指针
	/// </param>
	/// <returns>
	///		如果成功返回KVIO_OK,如果失败返回相应错误码
	/// </returns>
	/// <remarks>
	///		
	/// </remarks>
	virtual HRESULT RegisterIOServerCallBackFunc(
		IN IkxIOServerCallBack* pfnIOServerCallBackFunc) = 0;

	/// <summary>
	/// 	反注册回调函数
	/// </summary>
	/// <param name="pfnIOServerCallBackFunc">
	/// 	回调接口函数指针
	/// </param>
	/// <returns>
	///		如果成功返回KVIO_OK,如果失败返回相应错误码
	/// </returns>
	/// <remarks>
	///		
	/// </remarks>
	virtual HRESULT UnRegisterIOServerCallBackFunc(
		IN IkxIOServerCallBack* pfnIOServerCallBackFunc) = 0;


	//==============================================================================
	//
	//	3.5的，OEM暂时不用
	//
	//==============================================================================
//==============================================================================
	// 
	//	二、开发配置接口
	//
	//==============================================================================



	/// <summary> 
	///		平面化浏览采集工程属性
	///</summary> 
	/// <param name="Mask">
	///		掩码，可以带通配符
	/// </param>
	/// <param name="IoServerProperties">
	///		工程属性序列
	/// </param>
	/// <returns>
	///		如果成功返回KVIO_OK,如果失败返回相应错误码
	/// </returns>
	/// <remarks>
	///   
	/// </remarks>
	virtual HRESULT BrowseProjects(
		IN const std::wstring&						Mask,
		OUT std::vector<kxIOServerProperty> &		Properties ) =0;

	/// <summary> 
	/// 	平面化浏览采集通道
	/// </summary> 
	/// <param name="Mask">
	/// 	掩码，可以带通配符
	/// </param>

	/// <param name="Properties">
	///		通道属性序列
	/// </param>
	/// <returns>
	///		如果成功返回KVIO_OK,如果失败返回相应错误码
	/// </returns>
	/// <remarks>
	///   
	/// </remarks>
	virtual HRESULT BrowseChannels(
		IN const std::wstring&						Mask,
		OUT std::vector<kxChannelProperty> &		Properties )=0 ;

	/// <summary> 
	/// 	平面化浏览采集设备
	/// </summary> 
	/// <param name="Mask">
	/// 	掩码，可以带通配符
	/// </param>
	/// <param name="Properties">
	///		属性序列
	/// </param>
	/// <returns>
	///		如果成功返回KVIO_OK,如果失败返回相应错误码
	/// </returns>
	/// <remarks>
	///   
	/// </remarks>
	virtual HRESULT BrowseDevices(
		IN const std::wstring&						Mask,
		OUT std::vector<kxDeviceProperty> &			Properties ) =0;


	/// <summary> 
	/// 	平面化浏览采集变量
	/// </summary> 
	/// <param name="TagNameMask">
	/// 	变量名称掩码，可以带通配符
	/// </param>
	/// <param name="Properties">
	///		变量属性序列
	/// </param>
	/// <returns>
	///		如果成功返回KVIO_OK,如果失败返回相应错误码
	/// </returns>
	/// <remarks>
	///   
	/// </remarks>
	virtual HRESULT BrowseCollectTags(
		IN const std::wstring&						Mask,
		OUT std::vector<kxTagProperty> &			Properties )=0;
	
	/// <summary> 
	///		层次化浏览采集变量
	/// </summary> 
	/// <param name="BrowsePosition">
	///		浏览节点位置，如果为空则表示根节点
	/// </param>
	/// <param name="BrowseRecursive">
	///		是否递归浏览所有子节点，如果为真，则浏览所有子节点的变量配置，此时不返回子节点信息
	/// </param>
	/// <param name="BranchFilterMask">
	///		分枝过滤掩码（只有当BrowseRecursive为假时，此条件才会被应用到被浏览节点的次级分枝上）
	/// </param>
	/// <param name="TagNameMask">
	/// 	变量名称掩码，可以带通配符
	/// </param>
	/// <param name="TagDescriptionMask">
	/// 	变量描述，可以带通配符，也可以为空（NULL）
	/// </param>
	/// <param name="ChildNodeNames">
	///		用于保存返回的子节点名称（只有当BrowseRecursive为假时）
	/// </param>
	/// <param name="Properties">
	///		变量属性序列
	/// </param>
	/// <returns>
	///		如果成功返回KS_OK,如果失败返回相应错误码
	/// </returns>
	/// <remarks>
	///		按通道-设备-变量的层次化
	/// </remarks>
	virtual HRESULT BrowseCollectTagsHierarchical(
		IN const std::wstring&						BrowsePosition,
		IN BOOL										BrowseRecursive,
		IN const std::wstring&						BranchFilterMask,
		IN const std::wstring&						TagNameMask,
		IN const std::wstring&						TagDescriptionMask,
		OUT std::vector<std::wstring>&				ChildNodeNames,
		OUT std::vector<kxTagProperty> &			TagProperties ) = 0;


	//==============================================================================
	//	接口标识符
	//==============================================================================

	/// <summary> 
	///		获得接口标识符
	/// </summary> 
	/// <returns>
	///		接口标识符。
	/// </returns>
	/// <remarks>
	///
	///     所有的接口都必须实现该接口函数，模拟Visual C++中的__uuidof操作
	///	符的功能。
	/// 
	/// </remarks>
	static const CkxIID& GetIID() 
	{ 
		return KXIID_IkxIOServerHandle ;
	}

};

END_NAMESPACE(NkxIOAPI)

#endif #define __IkxIOServerHandle_H_INCLUDED__