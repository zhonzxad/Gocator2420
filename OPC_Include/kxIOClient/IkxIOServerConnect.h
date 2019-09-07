 //==============================================================================
/// <summary>
///		IOServer连接接口定义文件
/// </summary>
///	<file>
///		项目名:	组态王7.0
///		文件名: IkxIOServerConnect.h
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
///			V0.20 : 段长明 2009-09-01  支持冗余节点	
///		</version>
///	</versions>
//==============================================================================

//==============================================================================
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __IkxIOServerConnect_H_INCLUDED__
#define __IkxIOServerConnect_H_INCLUDED__

BEGIN_NAMESPACE(NkxIOAPI)

//==============================================================================
//	IOServer连接接口标识符
//==============================================================================
// {D6450ABB-00B6-4395-9BF6-9E49A3968383}
static const CkxIID KXIID_IkxIOServerConnect = 
{ 0xd6450abb, 0xb6, 0x4395, { 0x9b, 0xf6, 0x9e, 0x49, 0xa3, 0x96, 0x83, 0x83 } };


//==============================================================================
//	IOServer的连接接口
//==============================================================================

struct IkxIOServerConnect : IkxUnknown 
{
	
	/// <summary>
	/// 	初始化通讯环境
	/// </summary>
	/// <returns>
	///		如果成功返回KVIO_OK,如果失败返回相应错误码
	/// </returns>
	virtual HRESULT InitCommunicationEnv() =0;

	/// <summary>
	/// 	释放通讯环境
	/// </summary>
	/// <returns>
	///		如果成功返回KVIO_OK,如果失败返回相应错误码
	/// </returns>
	virtual HRESULT ReleaseCommunicationEnv() =0;

	/// <summary>
	/// 	创建连接Server的句柄
	/// </summary>
	/// <param name="ConnectionOption">
	///		连接选项
	/// </param>
	/// <param name="pServerHandle">
	///		连接句柄，输入需为空
	/// </param>
	/// <returns>
	///		如果成功返回KVIO_OK,如果失败返回KVIO_FAIL
	/// </returns>
	/// <remarks>
	///     默认Sever端的端口为****
	/// </remarks>
	virtual HRESULT CreateConnectServerHandle( 
		IN const kxConnectOption &		ConnectionOption,
		OUT IkxIOServerHandle*&			pServerHandle ) = 0;

	/// <summary>
	/// 	创建连接Server的句柄(适应冗余IOServer的扩展方法)
	/// </summary>
	/// <param name="PrimaryIOConnOption">
	///		Primary连接选项
	/// </param>
	/// <param name="SecondaryIOConnOption">
	///		Secondary连接选项
	/// </param>
	/// <param name="pServerHandle">
	///		连接句柄，输入需为空
	/// </param>
	/// <returns>
	///		如果成功返回KVIO_OK,如果失败返回KVIO_FAIL
	/// </returns>
	/// <remarks>
	///     默认Sever端的端口为****
	/// </remarks>
	virtual HRESULT CreateConnectServerHandleEx(
		IN const kxConnectOption&		PrimaryIOConnOption,
		IN const kxConnectOption&		SecondaryIOConnOption,
		OUT IkxIOServerHandle*&			pServerHandle) = 0;

	/// <summary>
	/// 	注册回调
	/// </summary>
	/// <param name="IOServerHandle">
	///		连接主IOSever的句柄
	/// </param>	
	/// <returns>
	///		如果成功返回KVIO_OK,如果失败返回KVIO_FAIL
	/// </returns>	
	/// </remarks>
	virtual HRESULT CreateIOServerCallBack( 
		IN IkxIOServerHandle*&		IOServerHandle		)=0 ;

	///// <summary>
	///// 	释放回调
	///// </summary> 
	///// </param>
	///// <returns>
	/////		如果成功返回KVIO_OK,如果失败返回KVIO_FAIL
	///// </returns>	
	///// </remarks>
	virtual HRESULT  ReleaseIOServerCallBack(IN IkxIOServerHandle*&		IOServerHandle)=0;

	/// <summary>
	/// 	释放连接Server的句柄
	/// </summary>
	/// <param name="pServerHandle">
	///		连接句柄，输入不能为空,操作成功后变为空
	/// </param>
	/// <returns>
	///		如果成功返回KVIO_OK,如果失败返回KVIO_FAIL
	/// </returns>
	/// <remarks>
	///     
	/// </remarks>
	virtual HRESULT ReleaseConnectServerHandle( IN  OUT IkxIOServerHandle*& pServerHandle ) = 0;

	/// <summary> 
	///		获得接口标识符。
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
		return KXIID_IkxIOServerConnect; 
	}
};
END_NAMESPACE(NkxIOAPI)

#endif #define __IkxIOServerConnect_H_INCLUDED__