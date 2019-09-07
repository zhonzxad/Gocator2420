 //==============================================================================
/// <summary>
///		IOServer���ӽӿڶ����ļ�
/// </summary>
///	<file>
///		��Ŀ��:	��̬��7.0
///		�ļ���: IkxIOServerConnect.h
///		��  ��: �γ���
///	</file>
///	<copyright>
/// 	���������а�ȨΪ�ǿؿƼ����У��κθ��˻�����֯�������κη�ʽ���ơ�ת����Ų��
/// ���ã����򽫹��ɶ��ǿؿƼ��Ĳ�Ȩ֪ʶ��Ȩ��
///	
///		��ϵ: http://www.kingview.com/
///	</copyright>
///	<versions>
///		<version>
///			V0.10 : �γ��� 2009-05-12  �����ļ�
///			V0.20 : �γ��� 2009-09-01  ֧������ڵ�	
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
//	IOServer���ӽӿڱ�ʶ��
//==============================================================================
// {D6450ABB-00B6-4395-9BF6-9E49A3968383}
static const CkxIID KXIID_IkxIOServerConnect = 
{ 0xd6450abb, 0xb6, 0x4395, { 0x9b, 0xf6, 0x9e, 0x49, 0xa3, 0x96, 0x83, 0x83 } };


//==============================================================================
//	IOServer�����ӽӿ�
//==============================================================================

struct IkxIOServerConnect : IkxUnknown 
{
	
	/// <summary>
	/// 	��ʼ��ͨѶ����
	/// </summary>
	/// <returns>
	///		����ɹ�����KVIO_OK,���ʧ�ܷ�����Ӧ������
	/// </returns>
	virtual HRESULT InitCommunicationEnv() =0;

	/// <summary>
	/// 	�ͷ�ͨѶ����
	/// </summary>
	/// <returns>
	///		����ɹ�����KVIO_OK,���ʧ�ܷ�����Ӧ������
	/// </returns>
	virtual HRESULT ReleaseCommunicationEnv() =0;

	/// <summary>
	/// 	��������Server�ľ��
	/// </summary>
	/// <param name="ConnectionOption">
	///		����ѡ��
	/// </param>
	/// <param name="pServerHandle">
	///		���Ӿ����������Ϊ��
	/// </param>
	/// <returns>
	///		����ɹ�����KVIO_OK,���ʧ�ܷ���KVIO_FAIL
	/// </returns>
	/// <remarks>
	///     Ĭ��Sever�˵Ķ˿�Ϊ****
	/// </remarks>
	virtual HRESULT CreateConnectServerHandle( 
		IN const kxConnectOption &		ConnectionOption,
		OUT IkxIOServerHandle*&			pServerHandle ) = 0;

	/// <summary>
	/// 	��������Server�ľ��(��Ӧ����IOServer����չ����)
	/// </summary>
	/// <param name="PrimaryIOConnOption">
	///		Primary����ѡ��
	/// </param>
	/// <param name="SecondaryIOConnOption">
	///		Secondary����ѡ��
	/// </param>
	/// <param name="pServerHandle">
	///		���Ӿ����������Ϊ��
	/// </param>
	/// <returns>
	///		����ɹ�����KVIO_OK,���ʧ�ܷ���KVIO_FAIL
	/// </returns>
	/// <remarks>
	///     Ĭ��Sever�˵Ķ˿�Ϊ****
	/// </remarks>
	virtual HRESULT CreateConnectServerHandleEx(
		IN const kxConnectOption&		PrimaryIOConnOption,
		IN const kxConnectOption&		SecondaryIOConnOption,
		OUT IkxIOServerHandle*&			pServerHandle) = 0;

	/// <summary>
	/// 	ע��ص�
	/// </summary>
	/// <param name="IOServerHandle">
	///		������IOSever�ľ��
	/// </param>	
	/// <returns>
	///		����ɹ�����KVIO_OK,���ʧ�ܷ���KVIO_FAIL
	/// </returns>	
	/// </remarks>
	virtual HRESULT CreateIOServerCallBack( 
		IN IkxIOServerHandle*&		IOServerHandle		)=0 ;

	///// <summary>
	///// 	�ͷŻص�
	///// </summary> 
	///// </param>
	///// <returns>
	/////		����ɹ�����KVIO_OK,���ʧ�ܷ���KVIO_FAIL
	///// </returns>	
	///// </remarks>
	virtual HRESULT  ReleaseIOServerCallBack(IN IkxIOServerHandle*&		IOServerHandle)=0;

	/// <summary>
	/// 	�ͷ�����Server�ľ��
	/// </summary>
	/// <param name="pServerHandle">
	///		���Ӿ�������벻��Ϊ��,�����ɹ����Ϊ��
	/// </param>
	/// <returns>
	///		����ɹ�����KVIO_OK,���ʧ�ܷ���KVIO_FAIL
	/// </returns>
	/// <remarks>
	///     
	/// </remarks>
	virtual HRESULT ReleaseConnectServerHandle( IN  OUT IkxIOServerHandle*& pServerHandle ) = 0;

	/// <summary> 
	///		��ýӿڱ�ʶ����
	/// </summary> 
	/// <returns>
	///		�ӿڱ�ʶ����
	/// </returns>
	/// <remarks>
	///
	///     ���еĽӿڶ�����ʵ�ָýӿں�����ģ��Visual C++�е�__uuidof����
	///	���Ĺ��ܡ�
	/// 
	/// </remarks>
	static const CkxIID& GetIID() 
	{ 
		return KXIID_IkxIOServerConnect; 
	}
};
END_NAMESPACE(NkxIOAPI)

#endif #define __IkxIOServerConnect_H_INCLUDED__