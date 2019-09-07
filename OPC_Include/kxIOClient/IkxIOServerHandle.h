
//==============================================================================
/// <summary>
///		IOServer���ʾ���ӿڶ����ļ�
/// </summary>
///	<file>
///		��Ŀ��:	��̬��7.0
///		�ļ���: IkxIOServerHandle.h
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
///			V0.10 : �γ��� 2009-05-12 10:01 �����ļ�
///			V0.20 : �γ��� 2009-09-01  ֧������ڵ�	
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
//	IOServer���ʽӿڽӿڱ�ʶ��
//==============================================================================
// {D6450ABB-00B6-4395-9BF6-9E49A3968382}
static const CkxIID KXIID_IkxIOServerHandle = 
{ 0xd6450abb, 0xb6, 0x4395, { 0x9b, 0xf6, 0x9e, 0x49, 0xa3, 0x96, 0x83, 0x82 } };

//==============================================================================
//	IOServer���ʾ���ӿ�
//==============================================================================
struct IkxIOServerHandle : IkxUnknown
{
	//==============================================================================
	//
	// һ������ͨѶ�ӿ�
	//
	//==============================================================================


	//==============================================================================
	//	ͬ����������
	//==============================================================================

	/// <summary>
	/// 	ͬ������ͨѶ����
	/// </summary>
	/// <returns>
	///		����ɹ�����KVIO_OK,���ʧ�ܷ���KVIO_FAIL
	/// </returns>
	/// <remarks>
	///     
	/// </remarks>
	virtual HRESULT SyncNetworkCommConnect() = 0;

	/// <summary>
	/// 	ͬ������ͨѶ�Ͽ�����
	/// </summary>
	/// <returns>
	///		����ɹ�����KVIO_OK,���ʧ�ܷ���KVIO_FAIL
	/// </returns>
	/// <remarks>
	///     
	/// </remarks>
	virtual HRESULT SyncNetworkCommDisConnect() = 0;

	//==============================================================================
	//	����ͨѶ����,�������첽ͨѶ���ӷ�ʽ
	//	������Ϣͨ��IkxIOServerCallBack�е�OnConnectWithServer��OnDisConnectWithServer������
	//==============================================================================

	/// <summary>
	/// 	��������ͨѶ����
	/// </summary>
	/// <returns>
	///		����ɹ�����KVIO_OK,���ʧ�ܷ���KVIO_FAIL
	/// </returns>
	/// <remarks>
	///     
	/// </remarks>
	virtual HRESULT StartNetworkCommService() = 0;


	/// <summary>
	/// 	ֹͣ����ͨѶ����
	/// </summary>
	/// <returns>
	///		����ɹ�����KVIO_OK,���ʧ�ܷ���KVIO_FAIL
	/// </returns>
	/// <remarks>
	///     
	/// </remarks>
	virtual HRESULT StopNetworkCommService() = 0;

	//==============================================================================
	//	������Ϣ
	//==============================================================================

	/// <summary>
	/// 	��ȡ����״̬
	/// </summary>
	/// <param name="bConnect">
	///		����״̬��TRUE:���� FALSE���ӶϿ�
	/// </param>
	/// <returns>
	///		����ɹ�����KVIO_OK,���ʧ�ܷ���KVIO_FAIL
	/// </returns>
	/// <remarks>
	///     �����˫IO���ֻ࣬Ҫ��������ĳ��IO��Ϊ������
	/// </remarks>
	virtual HRESULT IsConnected(OUT BOOL& bConnect) = 0;

	/// <summary>
	/// 	��ȡ��IOServer����״̬
	/// </summary>
	/// <param name="bConnect">
	///		����״̬��TRUE:���� FALSE���ӶϿ�
	/// </param>
	/// <returns>
	///		����ɹ�����KVIO_OK,���ʧ�ܷ���KVIO_FAIL
	/// </returns>
	/// <remarks>
	///     
	/// </remarks>
	virtual HRESULT IsPrimaryIOServerConnected(OUT BOOL& bConnect) = 0;

	/// <summary>
	/// 	��ȡ��IOServer����״̬
	/// </summary>
	/// <param name="bConnect">
	///		����״̬��TRUE:���� FALSE���ӶϿ�
	/// </param>
	/// <returns>
	///		����ɹ�����KVIO_OK,���ʧ�ܷ���KVIO_FAIL
	/// </returns>
	/// <remarks>
	///     
	/// </remarks>
	virtual HRESULT IsSecondaryIOServerConnected(OUT BOOL& bConnect) = 0;

	/// <summary>
	/// 	��ȡ��IO��ǰ���ӵ�IP��ַ
	/// </summary>
	/// <param name="strCurConnectIP">
	///		��ǰ���ӵ�IP��ַ
	/// </param>
	/// <returns>
	///		����ɹ�����KVIO_OK,���ʧ�ܷ���KVIO_FAIL
	/// </returns>
	/// <remarks>
	///     
	/// </remarks>
	virtual HRESULT GetPrimaryIOServerConnectedIP(OUT std::wstring& strCurConnectIP) = 0;

	/// <summary>
	/// 	��ȡ������IO��SCADA��Ϣ
	/// </summary>
	/// <param name="ClientScadaInfoSeq">
	///		������IO��SCADA��Ϣ
	/// </param>
	/// <returns>
	///		����ɹ�����KVIO_OK,���ʧ�ܷ���KVIO_FAIL
	/// </returns>
	/// <remarks>
	///     
	/// </remarks>
	virtual HRESULT GetScadaClientInfo(OUT std::vector<ScadaClientInfo>& ClientScadaInfoSeq) = 0;

	/// <summary>
	/// 	��ȡ��IO��ǰ���ӵ�IP��ַ
	/// </summary>
	/// <param name="strCurConnectIP">
	///		��ǰ���ӵ�IP��ַ
	/// </param>
	/// <returns>
	///		����ɹ�����KVIO_OK,���ʧ�ܷ���KVIO_FAIL
	/// </returns>
	/// <remarks>
	///     
	/// </remarks>
	virtual HRESULT GetSecondaryIOServerConnectedIP(OUT std::wstring& strCurConnectIP) = 0;

	//==============================================================================
	// 
	//	�����������ýӿ�
	//
	//==============================================================================

	//��ȡ��������ID
	virtual HRESULT GetTagAccessIDsByNames(
		IN  const std::vector<std::wstring>&	    TagNames,
		OUT std::vector<DWORD>&						TagAccessIDs,
		OUT std::vector<HRESULT>&					ErrorCodes)=0;


	//==============================================================================
	//
	//	�������з��ʽӿ�
	//
	//==============================================================================

	//==============================================================================
	//	IOServer״̬
	//==============================================================================

	/// <summary>
	/// 	��ȡIOServer����״̬
	/// </summary>
	/// <param name="Status">
	///		IOServer����״̬
	/// </param>
	/// <returns>
	///		����ɹ�����KVIO_OK,���ʧ�ܷ�����Ӧ������
	/// </returns>
	/// <remarks>
	///     IOServer����״̬�����嶨��ο�kxWorkStatus
	///		�����˫IO���࣬��ȡ���Ǽ��������״̬
	/// </remarks>
	virtual HRESULT GetWorkStatus(OUT  WORD& Status) = 0;

	/// <summary>
	/// 	��ȡIOServer����״̬
	/// </summary>
	/// <param name="Status">
	///		IOServer����״̬
	/// </param>
	/// <returns>
	///		����ɹ�����KVIO_OK,���ʧ�ܷ�����Ӧ������
	/// </returns>
	/// <remarks>
	///     IOServer����״̬�����嶨��ο�kxRunningStatus
	///		�����˫IO���࣬��ȡ���Ǽ��������״̬
	/// </remarks>
	virtual HRESULT GetRunningStatus(OUT WORD& Status ) = 0;

	/// <summary>
	/// 	��ȡ��IOServer��״̬
	/// </summary>
	/// <param name="Status">
	///		��IOServer״̬
	/// </param>
	/// <returns>
	///		����ɹ�����KVIO_OK,���ʧ�ܷ�����Ӧ������
	/// </returns>
	/// <remarks>
	///     IOServer״̬�����嶨��ο�kxServerStatus
	/// </remarks>
	virtual HRESULT GetPrimaryIOServerStatus(OUT  WORD& Status) = 0;

	/// <summary>
	/// 	��ȡ��IOServer��״̬
	/// </summary>
	/// <param name="Status">
	///		��IOServer״̬
	/// </param>
	/// <returns>
	///		����ɹ�����KVIO_OK,���ʧ�ܷ�����Ӧ������
	/// </returns>
	/// <remarks>
	///     IOServer״̬�����嶨��ο�kxServerStatus
	/// </remarks>
	virtual HRESULT GetSecondaryIOServerStatus(OUT  WORD& Status) = 0;

	/// <summary>
	/// 	��ȡIOServer����״̬
	/// </summary>
	/// <param name="bPrimary">
	///		�Ƿ�ΪPrimary IOServer
	/// </param>
	/// <param name="bActive">
	///		�Ƿ񼤻TRUEΪ���FALSEΪ����
	/// </param>
	/// <returns>
	///		����ɹ�����KVIO_OK,���ʧ�ܷ�����Ӧ������
	/// </returns>
	virtual HRESULT GetRedunActiveFlag(
		IN BOOL								bPrimary ,
		OUT BOOL&							bActive ) = 0;

	/// <summary>
	/// 	��ȡIOServer����״̬
	/// </summary>
	/// <param name="Status">
	///		���๤��״̬����kxRedunStatus
	/// </param>
	/// <returns>
	///		����ɹ�����KVIO_OK,���ʧ�ܷ�����Ӧ������
	/// </returns>
	virtual HRESULT GetRedundantStatus(
		OUT WORD&							Status,
		IN	BOOL	bAsync = TRUE,
		IN	INT		nTimeout = 3000) = 0;	//modified by pengfei.xing 2013/09/04


	//==============================================================================
	//	IOServerͬ����д�ӿ�
	//	ע���������������IOServer����֧��ͬ����д�ӿڣ���Ҫ��ع��ܵ���ʹ���첽��д�ӿ�
	//==============================================================================

	/// <summary>
	/// 	ͬ��������
	/// </summary>
	/// <param name="TagAccessIDs">
	/// 	����ID����
	/// </param>
	/// <param name="TagAccessFieldIDs">
	/// 	������Ӧ��ID����
	/// </param>
	/// <param name="TagsValue">
	/// 	����ֵ����
	/// </param>
	/// <param name="ErrorCodes">
	/// 	����������
	/// </param>
	/// <returns>
	///		����ɹ�����KVIO_OK,���ʧ�ܷ�����Ӧ������
	/// </returns>
	/// <remarks>
	///		�������������IOServer����֧��ͬ����д�ӿ�
	///		�����˫IO���࣬ʵ�ʲ������Ǽ����IO
	/// </remarks>
	virtual HRESULT SyncReadVariables(
		IN const std::vector<DWORD> &		TagAccessIDs,
		IN const std::vector<DWORD> &		TagAccessFieldIDs,
		IN WORD								DataSource,
		OUT std::vector<kxTagValue> &		TagsValue,
		OUT std::vector<HRESULT> &			ErrorCodes) = 0;

	/// <summary>
	/// 	ͬ��д����
	/// </summary>
	/// <param name="TagsValue">
	/// 	����ֵ����
	/// </param>
	/// <param name="ErrorCodes">
	/// 	����������
	/// </param>
	/// <returns>
	///		����ɹ�����KVIO_OK,���ʧ�ܷ�����Ӧ������
	/// </returns>
	/// <remarks>
	///		�������������IOServer����֧��ͬ����д�ӿ�
	///		�����˫IO���࣬ʵ�ʲ������Ǽ����IO
	/// </remarks>
	virtual HRESULT SyncWriteVariables(
		IN const std::vector<kxTagValue> &	TagsValue,
		OUT std::vector<HRESULT>&			ErrorCodes) = 0;


	//==============================================================================
	//	IOServer�����첽��д�ӿ�
	//==============================================================================

	/// <summary>
	/// 	�첽������
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
	///		����ɹ�����KVIO_OK,���ʧ�ܷ�����Ӧ������
	/// </returns>
	/// <remarks>
	///     ���ζ�������ֵͨ���ص��е�OnReceivedVaribleValue����
	///		�����˫IO���࣬ʵ�ʲ������Ǽ����IO
	/// </remarks>
	virtual HRESULT AsyncReadVariables(
		IN const std::vector<DWORD>  &		TagAccessIDs, 
		IN const std::vector<DWORD> &		TagAccessFieldIDs,
		IN WORD								DataSource,
		OUT std::vector<HRESULT>&			ErrorCodes) =0;

	/// <summary>
	/// 	�첽д����
	/// </summary>
	/// <param name="TagsValue">
	///		����ֵ����
	/// </param>
	/// <param name="ErrorCodes">
	///		����������
	/// </param>
	/// <returns>
	///		����ɹ�����KVIO_OK,���ʧ�ܷ�����Ӧ������
	/// </returns>
	/// <remarks>
	///     �첽д������ִ�н����ͨ���ص��е�OnWriteVariblesComplete����
	///		�����˫IO���࣬ʵ�ʲ������Ǽ����IO
	/// </remarks>
	virtual HRESULT AsyncWriteVariables(
		IN const std::vector<kxTagValue>  &	TagsValue, 
		OUT std::vector<HRESULT>&			ErrorCodes) = 0;


	//==============================================================================
	//	IOServer���ڲɼ��ӿ�
	//==============================================================================

	/// <summary>
	/// 	���ڲɼ�����
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
	/// <param name="NodeTypeForRedun">
	///		˫IO����ʱ�Ľڵ����ͣ�����������಻�ùܸò�����Ҳ����ΪKVIO_NODE_SINGLE
	/// </param>
	/// <returns>
	///		����ɹ�����KVIO_OK,���ʧ�ܷ�����Ӧ������
	/// </returns>
	/// <remarks>
	///     ����ֵͨ���ص��е�OnReceivedVaribleValue����
	/// </remarks>
	virtual HRESULT ReadTagsByInterval(
		IN const std::vector<DWORD> &		TagAccessIDs, 
		IN const std::vector<DWORD> &		TagAccessFieldIDs,
		OUT std::vector<HRESULT>&			ErrorCodes,
		IN WORD								NodeTypeForRedun ) =0;	

	/// <summary>
	/// 	ֹͣ���ڲɼ�����
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
	/// <param name="NodeTypeForRedun">
	///		˫IO����ʱ�Ľڵ����ͣ�����������಻�ùܸò�����Ҳ����ΪKVIO_NODE_SINGLE
	/// </param>
	/// <returns>
	///		����ɹ�����KVIO_OK,���ʧ�ܷ�����Ӧ������
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
	//	����Уʱ
	//==============================================================================

	/// <summary>
	/// 	������IOServer���ڻ���Уʱ
	/// </summary>
	/// <param name="bAdjustTime">
	/// 	�Ƿ�Уʱ
	/// </param>
	/// <param name="AdjustTimeInterval">
	/// 	Уʱ�������λs
	/// </param>
	/// <param name="NodeTypeForRedun">
	///		˫IO����ʱ�Ľڵ����ͣ�����������಻�ùܸò�����Ҳ����ΪKVIO_NODE_SINGLE
	/// </param>
	/// <returns>
	///		����ɹ�����KVIO_OK,���ʧ�ܷ�����Ӧ������
	/// </returns>
	/// <remarks>
	///		�����ü�Ĭ��״̬����Уʱ
	/// </remarks>
	virtual HRESULT SetMachineAdjustTime(
		IN BOOL											bAdjustTime,
		IN DWORD										AdjustTimeInterval,
		IN WORD											NodeTypeForRedun = KVIO_NODE_REDUN_BOTH)  = 0;

	//add end by hongchao.li 2012/09/07	

	// Added by chunming.suo 2014/05/06 16:54:39 [����IO�������е�KS��ȨУ��] Begin
	/// <summary>
	/// 	��IOServer���ڻ�������KS��Ȩ
	/// </summary>
	/// <param name="bDemoFlag">
	/// 	�Ƿ�Demo
	/// </param>
	/// <param name="NodeTypeForRedun">
	///		˫IO����ʱ�Ľڵ����ͣ�����������಻�ùܸò�����Ҳ����ΪKVIO_NODE_SINGLE
	/// </param>
	/// <returns>
	///		����ɹ�����KVIO_OK,���ʧ�ܷ�����Ӧ������
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
	//	�ġ����м��ӽӿ�
	//
	//==============================================================================

	//==============================================================================
	//	5��IOServer�������ܼ���
	//==============================================================================

	/// <summary>
	/// 	��ȡ��ǰCPUʹ����Ϣ
	/// </summary>
	/// <param name="CurCPUUse">
	/// 	��ǰCPUʹ����
	/// </param>
	/// <param name="CurCPUPercentUse">
	/// 	��ǰCPUʹ���ʰٷֱ�
	/// </param>
	/// <param name="Reserved1">
	/// 	��������1
	/// </param>
	/// <param name="Reserved2">
	/// 	��������2
	/// </param>
	/// <returns>
	///		����ɹ�����KVIO_OK,���ʧ�ܷ�����Ӧ������
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
	/// 	��ȡ��ǰ�ڴ�ʹ����Ϣ
	/// </summary>
	/// <param name="CurMemUse">
	/// 	��ǰ�ڴ�ʹ����
	/// </param>
	/// <param name="CurMemPercentUse">
	/// 	��ǰ�ڴ�ʹ���ʰٷֱ�
	/// </param>
	/// <param name="Reserved1">
	/// 	��������1
	/// </param>
	/// <param name="Reserved2">
	/// 	��������2
	/// </param>
	/// <returns>
	///		����ɹ�����KVIO_OK,���ʧ�ܷ�����Ӧ������
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
	/// 	��ȡ��ǰ���е��̸߳���
	/// </summary>
	/// <param name="CurThreadNum">
	/// 	��ǰ���е��̸߳���
	/// </param>
	/// <returns>
	///		����ɹ�����KVIO_OK,���ʧ�ܷ�����Ӧ������
	/// </returns>
	/// <remarks>
	///		
	/// </remarks>
	virtual HRESULT GetCurrentThreadNum(OUT WORD&	CurThreadNum ) = 0;

	//==============================================================================
	//	ע��ص�����
	//==============================================================================
	/// <summary>
	/// 	ע��ص�����
	/// </summary>
	/// <param name="pfnIOServerCallBackFunc">
	/// 	�ص��ӿں���ָ��
	/// </param>
	/// <returns>
	///		����ɹ�����KVIO_OK,���ʧ�ܷ�����Ӧ������
	/// </returns>
	/// <remarks>
	///		
	/// </remarks>
	virtual HRESULT RegisterIOServerCallBackFunc(
		IN IkxIOServerCallBack* pfnIOServerCallBackFunc) = 0;

	/// <summary>
	/// 	��ע��ص�����
	/// </summary>
	/// <param name="pfnIOServerCallBackFunc">
	/// 	�ص��ӿں���ָ��
	/// </param>
	/// <returns>
	///		����ɹ�����KVIO_OK,���ʧ�ܷ�����Ӧ������
	/// </returns>
	/// <remarks>
	///		
	/// </remarks>
	virtual HRESULT UnRegisterIOServerCallBackFunc(
		IN IkxIOServerCallBack* pfnIOServerCallBackFunc) = 0;


	//==============================================================================
	//
	//	3.5�ģ�OEM��ʱ����
	//
	//==============================================================================
//==============================================================================
	// 
	//	�����������ýӿ�
	//
	//==============================================================================



	/// <summary> 
	///		ƽ�滯����ɼ���������
	///</summary> 
	/// <param name="Mask">
	///		���룬���Դ�ͨ���
	/// </param>
	/// <param name="IoServerProperties">
	///		������������
	/// </param>
	/// <returns>
	///		����ɹ�����KVIO_OK,���ʧ�ܷ�����Ӧ������
	/// </returns>
	/// <remarks>
	///   
	/// </remarks>
	virtual HRESULT BrowseProjects(
		IN const std::wstring&						Mask,
		OUT std::vector<kxIOServerProperty> &		Properties ) =0;

	/// <summary> 
	/// 	ƽ�滯����ɼ�ͨ��
	/// </summary> 
	/// <param name="Mask">
	/// 	���룬���Դ�ͨ���
	/// </param>

	/// <param name="Properties">
	///		ͨ����������
	/// </param>
	/// <returns>
	///		����ɹ�����KVIO_OK,���ʧ�ܷ�����Ӧ������
	/// </returns>
	/// <remarks>
	///   
	/// </remarks>
	virtual HRESULT BrowseChannels(
		IN const std::wstring&						Mask,
		OUT std::vector<kxChannelProperty> &		Properties )=0 ;

	/// <summary> 
	/// 	ƽ�滯����ɼ��豸
	/// </summary> 
	/// <param name="Mask">
	/// 	���룬���Դ�ͨ���
	/// </param>
	/// <param name="Properties">
	///		��������
	/// </param>
	/// <returns>
	///		����ɹ�����KVIO_OK,���ʧ�ܷ�����Ӧ������
	/// </returns>
	/// <remarks>
	///   
	/// </remarks>
	virtual HRESULT BrowseDevices(
		IN const std::wstring&						Mask,
		OUT std::vector<kxDeviceProperty> &			Properties ) =0;


	/// <summary> 
	/// 	ƽ�滯����ɼ�����
	/// </summary> 
	/// <param name="TagNameMask">
	/// 	�����������룬���Դ�ͨ���
	/// </param>
	/// <param name="Properties">
	///		������������
	/// </param>
	/// <returns>
	///		����ɹ�����KVIO_OK,���ʧ�ܷ�����Ӧ������
	/// </returns>
	/// <remarks>
	///   
	/// </remarks>
	virtual HRESULT BrowseCollectTags(
		IN const std::wstring&						Mask,
		OUT std::vector<kxTagProperty> &			Properties )=0;
	
	/// <summary> 
	///		��λ�����ɼ�����
	/// </summary> 
	/// <param name="BrowsePosition">
	///		����ڵ�λ�ã����Ϊ�����ʾ���ڵ�
	/// </param>
	/// <param name="BrowseRecursive">
	///		�Ƿ�ݹ���������ӽڵ㣬���Ϊ�棬����������ӽڵ�ı������ã���ʱ�������ӽڵ���Ϣ
	/// </param>
	/// <param name="BranchFilterMask">
	///		��֦�������루ֻ�е�BrowseRecursiveΪ��ʱ���������ŻᱻӦ�õ�������ڵ�Ĵμ���֦�ϣ�
	/// </param>
	/// <param name="TagNameMask">
	/// 	�����������룬���Դ�ͨ���
	/// </param>
	/// <param name="TagDescriptionMask">
	/// 	�������������Դ�ͨ�����Ҳ����Ϊ�գ�NULL��
	/// </param>
	/// <param name="ChildNodeNames">
	///		���ڱ��淵�ص��ӽڵ����ƣ�ֻ�е�BrowseRecursiveΪ��ʱ��
	/// </param>
	/// <param name="Properties">
	///		������������
	/// </param>
	/// <returns>
	///		����ɹ�����KS_OK,���ʧ�ܷ�����Ӧ������
	/// </returns>
	/// <remarks>
	///		��ͨ��-�豸-�����Ĳ�λ�
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
	//	�ӿڱ�ʶ��
	//==============================================================================

	/// <summary> 
	///		��ýӿڱ�ʶ��
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
		return KXIID_IkxIOServerHandle ;
	}

};

END_NAMESPACE(NkxIOAPI)

#endif #define __IkxIOServerHandle_H_INCLUDED__