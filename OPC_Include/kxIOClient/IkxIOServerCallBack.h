//==============================================================================
/// <summary>
///		IOServer回调接口
/// </summary>
///	<file>
///		项目名:	组态王7.0
///		文件名: IkxIOServerCallBack.h
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
///			V0.10 : 段长明 2009-05-12  创建文件
///			V0.20： 段长明 2009-09-01  支持冗余节点，调整工作状态和运行状态
///		</version>
///	</versions>
//==============================================================================

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __IkxIOServerCallBack_H_INCLUDED__
#define __IkxIOServerCallBack_H_INCLUDED__


BEGIN_NAMESPACE(NkxIOAPI)

//==============================================================================
//	IOServer回调接口
//  注1：回调函数实现的逻辑应该尽可能简单，以最快的速度返回调用，避免对服务器端造成阻塞。
//  注2：回调函数实现中，不要调用ioserver的接口，这样可能会导致分布式的死锁。
//==============================================================================
struct IkxIOServerCallBack  : IkxUnknown 
{
	//==============================================================================
	//	采集运行部分
	//==============================================================================

	/// <summary>
	///		与服务器连接回调处理函数
	/// </summary>
	/// <param name="NodeType">
	///		节点类型，默认单一节点
	/// </param>
	/// <returns>
	///   如果成功返回KVIO_OK,如果失败返回KVIO_FAIL
	/// </returns>
	/// <remarks>
	///     
	/// </remarks>
	virtual HRESULT OnConnectWithServer(IN WORD NodeType = KVIO_NODE_SINGLE) = 0;


	/// <summary>
	///		与服务器连接断开回调处理函数
	/// </summary>
	/// <param name="NodeType">
	///		节点类型，默认单一节点
	/// </param>
	/// <returns>
	///   如果成功返回KVIO_OK,如果失败返回KVIO_FAIL
	/// </returns>
	/// <remarks>
	///     
	/// </remarks>
	virtual HRESULT OnDisConnectWithServer(IN WORD NodeType = KVIO_NODE_SINGLE) = 0;


	/// <summary>
	/// 	变量数据变化回调接口
	/// </summary>
	/// <param name="TagsValue">
	///		变量数据序列
	/// </param>
	/// <returns>
	///		成功返回KVIO_OK，失败返回相应的错误码
	/// </returns>
	/// <remarks>
	///		周期读、异步读回调接口
	/// </remarks>
	virtual HRESULT OnReceivedVariableValue(IN const std::vector<kxTagValue>& TagsValue ) = 0;

	/// <summary>
	///		写变量回调接口
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
	///		成功返回KVIO_OK，失败返回相应的错误码
	/// </returns>
	/// <remarks>
	///		异步写回调
	/// </remarks>
	virtual HRESULT OnWriteVariablesCompleted(
		IN const std::vector<DWORD>&			TagAccessIDs, 
		IN const std::vector<DWORD>&			TagAccessFieldIDs,
		IN const std::vector<HRESULT>&			ErrorCodes) = 0;

	/// <summary>
	///		读变量回调接口
	/// </summary>
	/// <param name="TagsValue">
	///		变量数据序列
	/// </param>
	/// <returns>
	///		成功返回KVIO_OK，失败返回相应的错误码
	/// </returns>
	/// <remarks>
	///		异步读回调
	/// </remarks>
	virtual HRESULT OnReadVariablesCompleted(	
		IN const std::vector<kxTagValue>& TagsValue,
		IN const std::vector<HRESULT>&	  ErrorCodes) = 0;

	/// <summary>
	/// 	IOServer工作状态变化回调接口
	/// </summary>
	/// <param name="NodeType">
	///		节点类型，默认单一节点
	/// </param>
	/// <param name="Status">
	///		IOServer工作状态，值参见kxWorkStatus	
	/// <returns>
	///		成功返回KVIO_OK，失败返回相应的错误码
	/// </returns>
	/// <remarks>
	///		
	/// </remarks>
	virtual HRESULT OnWorkStatusChange(IN WORD NodeType ,IN  WORD  Status) = 0;

	/// <summary>
	/// 	IOServer运行状态变化回调接口
	/// </summary>
	/// <param name="NodeType">
	///		节点类型，默认单一节点
	/// </param>
	/// <param name="Status">
	///		IOServer运行状态，值参见kxRunningStatus	
	/// <returns>
	///		成功返回KVIO_OK，失败返回相应的错误码
	/// </returns>
	/// <remarks>
	///		未实现
	/// </remarks>
	virtual HRESULT OnRunningStatusChange(IN WORD NodeType ,IN  WORD  Status) = 0;

	/// <summary>
	/// 	采集模块元素操作回调接口
	/// </summary>
	/// <param name="CollectElementOperateInfos">
	///		采集模块元素操作信息
	/// </param>
	/// <returns>
	///		成功返回KVIO_OK，失败返回相应的错误码
	/// </returns>
	/// <remarks>
	///		
	/// </remarks>
	virtual HRESULT OnCollectElementOperateChange(IN const std::vector<kxCollectElementOperateInfo>&	CollectElementOperateInfos) = 0;

	/// <summary>
	/// 	设备通讯状态变化回调函数
	/// </summary>
	/// <param name="DeviceStatusInfos">
	/// 	设备状态信息序列
	/// </param>
	/// <returns>
	///		如果成功返回KVIO_OK,如果失败返回相应错误码
	/// </returns>
	/// <remarks>
	/// </remarks>
	virtual HRESULT OnDeviceStatusChange(
		IN const std::vector<kxDeviceStatusInfo>& DeviceStatusInfos) = 0;
};

END_NAMESPACE(NkxIOAPI)

#endif #define __IkxIOServerCallBack_H_INCLUDED__