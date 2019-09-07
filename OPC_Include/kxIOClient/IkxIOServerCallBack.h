//==============================================================================
/// <summary>
///		IOServer�ص��ӿ�
/// </summary>
///	<file>
///		��Ŀ��:	��̬��7.0
///		�ļ���: IkxIOServerCallBack.h
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
///			V0.20�� �γ��� 2009-09-01  ֧������ڵ㣬��������״̬������״̬
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
//	IOServer�ص��ӿ�
//  ע1���ص�����ʵ�ֵ��߼�Ӧ�þ����ܼ򵥣��������ٶȷ��ص��ã�����Է����������������
//  ע2���ص�����ʵ���У���Ҫ����ioserver�Ľӿڣ��������ܻᵼ�·ֲ�ʽ��������
//==============================================================================
struct IkxIOServerCallBack  : IkxUnknown 
{
	//==============================================================================
	//	�ɼ����в���
	//==============================================================================

	/// <summary>
	///		����������ӻص�������
	/// </summary>
	/// <param name="NodeType">
	///		�ڵ����ͣ�Ĭ�ϵ�һ�ڵ�
	/// </param>
	/// <returns>
	///   ����ɹ�����KVIO_OK,���ʧ�ܷ���KVIO_FAIL
	/// </returns>
	/// <remarks>
	///     
	/// </remarks>
	virtual HRESULT OnConnectWithServer(IN WORD NodeType = KVIO_NODE_SINGLE) = 0;


	/// <summary>
	///		����������ӶϿ��ص�������
	/// </summary>
	/// <param name="NodeType">
	///		�ڵ����ͣ�Ĭ�ϵ�һ�ڵ�
	/// </param>
	/// <returns>
	///   ����ɹ�����KVIO_OK,���ʧ�ܷ���KVIO_FAIL
	/// </returns>
	/// <remarks>
	///     
	/// </remarks>
	virtual HRESULT OnDisConnectWithServer(IN WORD NodeType = KVIO_NODE_SINGLE) = 0;


	/// <summary>
	/// 	�������ݱ仯�ص��ӿ�
	/// </summary>
	/// <param name="TagsValue">
	///		������������
	/// </param>
	/// <returns>
	///		�ɹ�����KVIO_OK��ʧ�ܷ�����Ӧ�Ĵ�����
	/// </returns>
	/// <remarks>
	///		���ڶ����첽���ص��ӿ�
	/// </remarks>
	virtual HRESULT OnReceivedVariableValue(IN const std::vector<kxTagValue>& TagsValue ) = 0;

	/// <summary>
	///		д�����ص��ӿ�
	/// </summary>
	/// <param name="TagAccessIDs">
	///		����ID����
	/// </param>
	/// <param name="TagAccessFieldIDs">
	/// 	������Ӧ��ID����
	/// </param>
	/// <param name="ErrorCodes">
	///		����������
	/// </param>
	/// <returns>
	///		�ɹ�����KVIO_OK��ʧ�ܷ�����Ӧ�Ĵ�����
	/// </returns>
	/// <remarks>
	///		�첽д�ص�
	/// </remarks>
	virtual HRESULT OnWriteVariablesCompleted(
		IN const std::vector<DWORD>&			TagAccessIDs, 
		IN const std::vector<DWORD>&			TagAccessFieldIDs,
		IN const std::vector<HRESULT>&			ErrorCodes) = 0;

	/// <summary>
	///		�������ص��ӿ�
	/// </summary>
	/// <param name="TagsValue">
	///		������������
	/// </param>
	/// <returns>
	///		�ɹ�����KVIO_OK��ʧ�ܷ�����Ӧ�Ĵ�����
	/// </returns>
	/// <remarks>
	///		�첽���ص�
	/// </remarks>
	virtual HRESULT OnReadVariablesCompleted(	
		IN const std::vector<kxTagValue>& TagsValue,
		IN const std::vector<HRESULT>&	  ErrorCodes) = 0;

	/// <summary>
	/// 	IOServer����״̬�仯�ص��ӿ�
	/// </summary>
	/// <param name="NodeType">
	///		�ڵ����ͣ�Ĭ�ϵ�һ�ڵ�
	/// </param>
	/// <param name="Status">
	///		IOServer����״̬��ֵ�μ�kxWorkStatus	
	/// <returns>
	///		�ɹ�����KVIO_OK��ʧ�ܷ�����Ӧ�Ĵ�����
	/// </returns>
	/// <remarks>
	///		
	/// </remarks>
	virtual HRESULT OnWorkStatusChange(IN WORD NodeType ,IN  WORD  Status) = 0;

	/// <summary>
	/// 	IOServer����״̬�仯�ص��ӿ�
	/// </summary>
	/// <param name="NodeType">
	///		�ڵ����ͣ�Ĭ�ϵ�һ�ڵ�
	/// </param>
	/// <param name="Status">
	///		IOServer����״̬��ֵ�μ�kxRunningStatus	
	/// <returns>
	///		�ɹ�����KVIO_OK��ʧ�ܷ�����Ӧ�Ĵ�����
	/// </returns>
	/// <remarks>
	///		δʵ��
	/// </remarks>
	virtual HRESULT OnRunningStatusChange(IN WORD NodeType ,IN  WORD  Status) = 0;

	/// <summary>
	/// 	�ɼ�ģ��Ԫ�ز����ص��ӿ�
	/// </summary>
	/// <param name="CollectElementOperateInfos">
	///		�ɼ�ģ��Ԫ�ز�����Ϣ
	/// </param>
	/// <returns>
	///		�ɹ�����KVIO_OK��ʧ�ܷ�����Ӧ�Ĵ�����
	/// </returns>
	/// <remarks>
	///		
	/// </remarks>
	virtual HRESULT OnCollectElementOperateChange(IN const std::vector<kxCollectElementOperateInfo>&	CollectElementOperateInfos) = 0;

	/// <summary>
	/// 	�豸ͨѶ״̬�仯�ص�����
	/// </summary>
	/// <param name="DeviceStatusInfos">
	/// 	�豸״̬��Ϣ����
	/// </param>
	/// <returns>
	///		����ɹ�����KVIO_OK,���ʧ�ܷ�����Ӧ������
	/// </returns>
	/// <remarks>
	/// </remarks>
	virtual HRESULT OnDeviceStatusChange(
		IN const std::vector<kxDeviceStatusInfo>& DeviceStatusInfos) = 0;
};

END_NAMESPACE(NkxIOAPI)

#endif #define __IkxIOServerCallBack_H_INCLUDED__