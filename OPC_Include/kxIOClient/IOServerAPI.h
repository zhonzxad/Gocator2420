//==============================================================================
/// <summary>
///		IOServer3.5�����ӿڣ���KS��KH��MES����������IOMaster������������ͳһʹ��
/// </summary>
///	<file>
///		��Ŀ��:	��̬��7.0
///		�ļ���: IOServerAPI.h
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
///			V0.9 : �γ��� 2010-02-22	��3.0��API�������޸�
///			V1.0 : �γ��� 2010-10-26	��ӱ�������ID�������ݱ�������ID���ʱ�������
///		</version>
///	</versions>
//==============================================================================

//==============================================================================
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __KingIOServerAPI_H_INCLUDED__
#define __KingIOServerAPI_H_INCLUDED__
//==============================================================================
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>

//==============================================================================
#pragma pack( push , KSIOSERVERAPI )
#pragma pack( 1 )
//==============================================================================
#ifdef  __cplusplus
extern "C" {
#endif

	//==============================================================================
	//
	//	һ���������������
	//
	//==============================================================================

	//==============================================================================
	// 1���궨��
	//==============================================================================
#define KSIOAPI				_stdcall

#define KSWSTR(str)		    L##str

#define KSOK(err)		    ( ((KS_INT32)(err)) ==  0 )
#define KSERR(err)		    ( ((KS_INT32)(err)) !=  0 )

#define IN 
#define OUT
#define IN_OUT

	//==============================================================================
	// 2���������Ͷ���
	//==============================================================================
	typedef bool						KS_BOOL;
	typedef char						KS_CHAR;
	typedef wchar_t						KS_WCHAR;
	typedef unsigned char				KS_BYTE;
	typedef unsigned char				KS_BOOLEAN;
	typedef float						KS_FLOAT32;
	typedef double						KS_FLOAT64;
	typedef signed char					KS_INT8;
	typedef unsigned char				KS_UINT8;
	typedef short						KS_INT16;
	typedef unsigned short				KS_UINT16;
	typedef long						KS_INT32;
	typedef unsigned long				KS_UINT32;
	typedef __int64						KS_INT64;
	typedef unsigned __int64			KS_UINT64;
	typedef void						KS_VOID;
	typedef void*						KS_PTR;
	typedef KS_BYTE*					KS_BINARY;
	typedef KS_CHAR*					KS_STR;
	typedef const KS_CHAR*				KS_CSTR;
	typedef KS_WCHAR*					KS_WSTR;
	typedef const KS_WCHAR*				KS_CWSTR;
	typedef KS_STR*						KS_STR_ARRAY;
	typedef KS_WSTR*					KS_WSTR_ARRAY;
	typedef long						KS_RET;						/// API�������÷�����
	typedef KS_PTR						KS_HANDLE;					/// ���Ӿ��

	//==============================================================================
	//	3�������붨��
	//==============================================================================

#define		KS_OK						0							/// û�д���
#define		KS_FAIL						-1							/// һ���Դ���
#define		KS_TIMEOUT					-2							/// ��ʱ����
#define		KS_OUT_OF_MEMORY			-3 							/// �ڴ����
#define		KS_NOT_CONNECTED			-4 							/// û�л��޷���������
#define		KS_ACCESS_DENIED			-5 							/// ���ʱ��ܾ�
#define		KS_NOT_SUPPORTED			-6 							/// ��֧�ֵ����Ի򷽷�
#define		KS_INVALID_USER				-7 							/// ��Ч�û�������
#define		KS_INVALID_TAGNAME			-8 							/// ��Ч������
#define		KS_INVALID_ARG				-9 							/// ��Ч����
#define		KS_NO_DATA					-10 						/// ���ݲ�����
#define		KS_NETWORK_ERROR			-11							/// �������
#define		KS_SYSTEM_ERROR				-12							/// ϵͳ����
#define		KS_NOT_FOUND 				-13 						/// �޷��ҵ�����
#define		KS_SERVICE_NOT_RUNNING		-14 						/// ����������״̬
#define		KS_BUFFER_TOO_SMALL			-15							/// ������̫С
#define		KS_NO_MORE_DATA 			-16							/// û�и��������
#define		KS_DEADLOCK					-17 						/// ����������
#define		KS_ALREADY_EXIST 			-18							/// �����Ѿ�����
#define		KS_DUPLICATE	 			-19							/// �����ظ���
#define		KS_OVERFLOW		 			-20							/// ���������
#define		KS_ABORTED		 			-21							/// �Ѿ��˳�
#define		KS_LOAD_RESOURCE_FAIL		-22							/// װ����Դʧ��
#define		KS_SERVICE_STATUS_ERROR		-23							/// ����״̬����
#define		KS_INITIAL_COM_ERROR		-24							/// ��ʼ��COM��ʧ��
#define		KS_NOT_INITIAL				-25							/// δ��ʼ��


	//==============================================================================
	//
	//	�������ö������
	//
	//==============================================================================

	//==============================================================================
	//	1��ͨ������
	//==============================================================================

	/// <summary>
	///		IOServer����
	/// </summary>
	typedef enum KSIOServerType
	{
		KSIO_SERVER_TYPE_IO			= 0,		//IO��IOServer
		KSIO_SERVER_TYPE_OPC		= 1,		//OPC��IOServer
		KSIO_SERVER_TYPE_DDE		= 2,		//DDE��IOServer
		KSIO_SERVER_TYPE_NetKV		= 3,		//����վ���IOServer
	} KSIO_SERVER_TYPE;


	/// <summary>
	///		IOClient����
	/// </summary>
	typedef enum KSIOClientType
	{
		KSIO_CLIENT_TYPE_TOUCHVIEW		= 0,	//��̬������ϵͳ,��ҵ��Ҳ�����
		KSIO_CLIENT_TYPE_TOUCHMAKE		= 1,	//��̬������ϵͳ
		KSIO_CLIENT_TYPE_ACCESS_CLIENT	= 4,	//�������ʿͻ���
		KSIO_CLIENT_TYPE_MONITOR_CLIENT	= 5,	//���ӿͻ���
		KSIO_CLIENT_TYPE_LOCAL_TEST		= 6,	//���ز��Կͻ���
		KSIO_CLIENT_TYPE_REDUNIOSERVER	= 7,	//����IOServer
	} KSIO_CLIENT_TYPE;

	/// <summary>
	///		����״̬
	/// </summary>
	typedef enum KSIONodeType
	{
			KSIO_NODE_SINGLE		= 0,	//��һ�ڵ�
			KSIO_NODE_REDUN_MASTER	= 1,	//��������ڵ�
			KSIO_NODE_REDUN_SLAVE	= 2,	//����Ĵӽڵ�
	} KSIO_NODE_TYPE;

	/// <summary>
	///		����״̬
	/// </summary>
	typedef enum KSIOConnectStatus
	{
		KSIO_STATUS_PRIMARY_CONNECT				= 0,		//���ڵ�����ڵ�����
		KSIO_STATUS_PRIMARY_DISCONNECT			= 1,		//���ڵ�����ڵ�Ͽ�����
		KSIO_STATUS_SECONDARY_CONNECT			= 2,		//�ӽڵ�����
		KSIO_STATUS_SECONDARY_DISCONNECT		= 3,		//�ӽڵ�Ͽ�����
	} KSIO_CONNECT_STATUS;

	/// <summary>
	///		����״̬
	/// </summary>
	typedef enum KSIOWorkStatus
	{
		KSIO_STATUS_START				= 0,		//����
		KSIO_STATUS_STOP				= 1,		//ֹͣ
	} KSIO_WORK_STATUS;

	/// <summary>
	///		����״̬
	/// </summary>
	typedef enum KSIORunningStatus
	{
		KSIO_RUN_STATUS_NORMAL			= 0x0000,	//����״̬
		KSIO_RUN_STATUS_NOT_INIT		= 0x0001,	//��ʼ��ʧ��
		KSIO_RUN_STATUS_FAULT			= 0x0002,   //����
		KSIO_RUN_STATUS_BUSY			= 0x0004,	//��������
	} KSIO_RUN_STATUS;

	/// <summary>
	///		�û�����Ȩ��
	/// </summary>
	typedef enum KSIOUserPrivileges
	{
		KSIO_USER_PRIVILEGE_LEVEL0		= 0x0000,	//����Ȩ��0
		KSIO_USER_PRIVILEGE_LEVEL1		= 0x0001,	//����Ȩ��1
		KSIO_USER_PRIVILEGE_LEVEL2		= 0x0002,	//����Ȩ��2
		KSIO_USER_PRIVILEGE_LEVEL3		= 0x0004,	//����Ȩ��3
		KSIO_USER_PRIVILEGE_LEVEL4		= 0x0010,	//����Ȩ��4
		KSIO_USER_PRIVILEGE_LEVEL5		= 0x0020,	//����Ȩ��5
		KSIO_USER_PRIVILEGE_LEVEL_ALL	= 0xffff,	//����Ȩ��
	} KSIO_USER_PRIVILEGES;

	/// <summary>
	///		��д����ģʽ
	/// </summary>
	typedef enum KSIOAccessMode
	{
		KSIO_ACCESS_READ_ONLY			= 0,		//ֻ��
		KSIO_ACCESS_WRITE_ONLY			= 1,		//ֻд
		KSIO_ACCESS_READ_WRITE			= 2,		//��д,Ԥ����	
	} KSIO_ACCESS_MODE;

	/// <summary>
	///		�ɼ�������Դ
	/// </summary>
	typedef enum KSIODataSource
	{
		KSIO_DS_CACHE					= 0,		//��CACHE��Դ����
		KSIO_DS_DEVICE					= 1,		//���豸��Դ����
	} KSIO_DATASOUCE;


		/// <summary>
	/// 	�ɼ�ģ��Ԫ������
	/// </summary>
	typedef enum KSIOItemType
	{		
		KSIO_COLLECT_PROJECT     = 1,
		KSIO_COLLECT_CHANNEL		,		// ��·
		KSIO_COLLECT_DEVICE			,		// �豸
		KSIO_COLLECT_GROUP			,		// ��
		KSIO_COLLECT_TAG			,	// ����
		KSIO_COLLECT_PROJECT_ISSTART   //�Ƿ�����
	}KSIO_ITEM_TYPE;


	/// <summary>
	/// 	�ɼ�ģ��Ԫ��״̬
	/// </summary>
	typedef enum KSIOItemState
	{
		KSIO_ITEM_STATE_NONE=0x0000,     ///��ʼ
		KSIO_ITEM_STATE_DELETED,         ///ɾ��
		KSIO_ITEM_STATE_STOP,            ///ֹͣ
		KSIO_ITEM_STATE_FAULT,           ///����
		KSIO_ITEM_STATE_SUSPEND,         ///����
		KSIO_ITEM_STATE_BUSY,            ///æµ
		KSIO_ITEM_STATE_RUN,             ///����
		KSIO_ITEM_STATE_ISSTART,          //���������ť��
		KSIO_ITEM_STATE_ISSTOP            //���ֹͣ������ť��
	}KSIO_ITEM_STATE;
	/// <summary>
	///		������������
	/// </summary>
	typedef enum KSIOTagDataType
	{
		KSIO_TAG_DATATYPE_BOOL		=  1,		//��ɢ
		KSIO_TAG_DATATYPE_CHAR		=  2,		//�з���8λ����
		KSIO_TAG_DATATYPE_BYTE      =  3,		//�޷���8λ����
		KSIO_TAG_DATATYPE_SHORT		=  4,		//�з���16λ����
		KSIO_TAG_DATATYPE_WORD		=  5,		//�޷���16λ����
		KSIO_TAG_DATATYPE_LONG		=  6,		//�з���32λ����
		KSIO_TAG_DATATYPE_DWORD		=  7,		//�޷���32λ����
		KSIO_TAG_DATATYPE_INT64		=  8,		//�з���64λ����
		KSIO_TAG_DATATYPE_FLOAT		=  9,		//�����ȸ���
		KSIO_TAG_DATATYPE_DOUBLE 	=  10,		//˫���ȸ���
		KSIO_TAG_DATATYPE_STRING	=  11,		//�ַ���
	} KSIO_TAG_DATA_TYPE;

	/// <summary> 
	/// ��������
	/// </summary> 
	typedef enum KSIODataQuality
	{
		// QQSSSSLL ( OPC 3.0:�Ͱ�λ������Q����״̬S������״̬L���� )
		KDAQ_OPC_QUALITY_MASK				= 0xC0, 		/// Quality BITMASK
		KDAQ_OPC_STATUS_MASK				= 0xFC, 		/// Quality & Substatus mask
		KDAQ_OPC_LIMIT_MASK					= 0x03, 		/// Limit BITMask

		// Quality( ���ֽڵĸ���λ����6λ�͵�7λ BITMASK )
		KDAQ_OPC_BAD						= 0x00, 		/// ������
		KDAQ_OPC_UNCERTAIN					= 0x40, 		/// ���ɿ�����
		KDAQ_OPC_NA							= 0x80, 		/// N/A
		KDAQ_OPC_GOOD						= 0xC0, 		/// �õ�����

		// Substatus( ���ֽڵ��м���λ����2λ����5λ )
		// Substatus for BAD Quality
		KDAQ_OPC_CONFIG_ERROR				= 0x04, 		/// ���ô���
		KDAQ_OPC_NOT_CONNECTED				= 0x08, 		/// û�������豸
		KDAQ_OPC_DEVICE_FAILURE				= 0x0c, 		/// �豸ʧ��
		KDAQ_OPC_SENSOR_FAILURE				= 0x10, 		/// ������ʧ��( limit���ܹ��ṩ������Ϣ )
		KDAQ_OPC_LAST_KNOWN					= 0x14, 		/// ��һ�βɼ���ֵ( ͨѶʧ�� )
		KDAQ_OPC_COMM_FAILURE				= 0x18, 		/// ͨѶʧ��( ������һ�βɼ�ֵ���� )
		KDAQ_OPC_OUT_OF_SERVICE				= 0x1C, 		/// �豸ͣ��
		KDAQ_OPC_WAITING_FOR_INITIAL_DATA	= 0x20, 		/// ��δȡ���豸����

		// Substatus for UNCERTAIN Quality
		KDAQ_OPC_LAST_USABLE				= 0x44, 		/// ��һ������ֵ
		KDAQ_OPC_SENSOR_CAL					= 0x50, 		/// ������ֵ����ȷ
		KDAQ_OPC_EGU_EXCEEDED				= 0x54, 		/// ������
		KDAQ_OPC_SUB_NORMAL					= 0x58, 		/// ֵ�Ӷ������Դ�õ�����ȱ���㹻��ĺ�����

		// Substatus for GOOD Quality
		KDAQ_OPC_LOCAL_OVERRIDE				= 0xD8, 		/// ֵ������( GOOD )

		// Limit( ���ֽڵĵ���λ����0λ����1λ )
		KDAQ_OPC_LIMIT_OK					= 0x00, 		/// ������OK
		KDAQ_OPC_LIMIT_LOW					= 0x01, 		/// ����
		KDAQ_OPC_LIMIT_HIGH					= 0x02, 		/// ����
		KDAQ_OPC_LIMIT_CONST				= 0x03, 		/// ����
	} KSIO_DATA_QUALITY; 


	//==============================================================================
	//	2������KS3.0�汾ʹ�õ�����
	//==============================================================================

	/// <summary>
	///		�豸ͨѶ״̬
	/// </summary>
	typedef enum KSIODeviceCommStatus
	{
		KSIO_DEVICE_COMM_STATUS_OK				= 0,		//����
		KSIO_DEVICE_COMM_STATUS_STOP			= 1,		//ֹͣ��һ��Ϊ������Ʋ����вɼ�
		KSIO_DEVICE_COMM_STATUS_INITIAL_FAIL	= 2,		//�豸��ʼ��ʧ��
		KSIO_DEVICE_COMM_STATUS_FAULT			= 3,		//���ϣ�����ͨѶ���ϻ����豸����Ĺ���
		KSIO_DEVICE_COMM_STATUS_SYS_STOP		= 4,		//ϵͳֹͣ����û�д�������״̬
	} KSIO_DEV_COMM_STATUS;

	/// <summary>
	///		�Ĵ�����������
	/// </summary>
	/// <remarks>
	///      ����Ʒ����ȷ��KS3.5�м������ݱ���IO���Բ�����ʹ�ø�����
	/// </remarks>
	typedef enum KSIORegisterDataType
	{
		KSIO_REG_DATATYPE_OTHER			=	0x000,		//��ָ����������
		KSIO_REG_DATATYPE_BIT			=	0x0001,		//1λ����Χ�ǣ�0��1
		KSIO_REG_DATATYPE_BYTE			=	0x0002,     //8λ��1���ֽڣ���Χ�ǣ�0---255
		KSIO_REG_DATATYPE_SHORT			=	0x0004,		//2���ֽڣ���Χ�ǣ�-32768---32767
		KSIO_REG_DATATYPE_USHORT		=	0x0008,     //16λ��2���ֽڣ���Χ�ǣ�0---65535
		KSIO_REG_DATATYPE_BCD			=	0x0010,     //16λ��2���ֽڣ���Χ�ǣ�0---9999
		KSIO_REG_DATATYPE_LONG			=	0x0020,     //32λ��4���ֽڣ���Χ�ǣ�-2147483648����2147483647
		KSIO_REG_DATATYPE_LONGBCD		=	0x0040,     //32λ��4���ֽڣ���Χ�ǣ�0---4294967295
		KSIO_REG_DATATYPE_FLOAT			=	0x0080,     //32λ��4���ֽڣ���Χ�ǣ�10e-38---10e38����Чλ7λ
		KSIO_REG_DATATYPE_STRING		=	0x0100,		//128���ַ�����
		KSIO_REG_DATATYPE_DOUBLE		=	0x0200,     //˫���ȸ�����
		KSIO_REG_DATATYPE_BLOB			=	0x0400,     //��������
		KSIO_REG_DATATYPE_INT64		=	0x0800,     //64λ��8���ֽ� // add by pengfei.xing 2015/01/23 //zhanzhi,INT64
	} KSIO_REG_DATA_TYPE;



	
	/// <summary>
	///		ͨ���������ݽṹ
	/// </summary>
	typedef struct KSIOServerProperty
	{
		KS_WSTR     IOServerName;
		KS_WSTR     ConfigFilePath;
		
		KS_BOOL		bUseSpecNetCard;	
		KS_UINT16   IOServerPort;

		KS_WSTR     IOServerIP;
		KS_WSTR     BackUpIP; 
		KS_WSTR	    LocalSpecNetCardIP;	
	


		KS_UINT32   RedunFailoverMode; 

		KS_WSTR	    RedunStationIP;			
		KS_WSTR	    RedunStationBackUpIP;	
        KS_WSTR	    RedunSpecNetCardIP;	
			 		
		KS_UINT16   HeartBeatDetectInterval;
		KS_UINT16   MaxAllowErrorsBeforeDisconnect;

	}KSIO_SERVER_PROPERTY,*PKSIO_SERVER_PROPERTY;

	/// <summary>
	/// ͨ����¼��
	/// </summary>
	typedef struct KSIOServerPropertyArray
	{
		KS_UINT32						Number;
		//ͨ��������Ŀ
		PKSIO_SERVER_PROPERTY			ServerPropertiesArray;			//ͨ����������
	} KSIO_SERVER_PROPERTY_ARRAY, *PSIO_SERVER_PROPERTY_ARRAY;



	

	/// <summary>
	///		ͨ���������ݽṹ
	/// </summary>
	typedef struct KSIOChannelProperty
	{
		KS_UINT32				ChannelID;
		//�ɼ�ͨ��ID
		KS_WSTR					ChannelName;
		//�ɼ�ͨ������	
		CLSID					ClsID;
		//clsid
		KS_WSTR					ChannelDescription;
		//�ɼ�ͨ������


		KS_WSTR					ChannelDriver;
		KS_UINT16				ChannelType;
		//ͨ�����ͣ������kxChannelType


		KS_UINT32				Timeout;
		//��·��ʱms
		//Ԥ����
		//������        //��̫����			//OPC
		KS_UINT32				Reserved1;//������        //�����˿ں�
		KS_UINT32				Reserved2;//����λ
		KS_UINT32				Reserved3;//ֹͣλ
		KS_UINT32				Reserved4;//��żУ��
		KS_UINT32				Reserved5;//����
		KS_WSTR					Reserved6;//����ID							//proid
		KS_WSTR					Reserved7;													//machine name opc
		KS_WSTR					Reserved8;
		KS_WSTR					Reserved9;
		KS_WSTR					Reserved10;
	}KSIO_CHANNEL_PROPERTY,*PKSIO_CHANNEL_PROPERTY;


	/// <summary>
	/// ͨ����¼��
	/// </summary>
	typedef struct KSIOChannelPropertyArray
	{
		KS_UINT32						Number;
		//ͨ��������Ŀ
		PKSIO_CHANNEL_PROPERTY			ChannelPropertiesArray;			//ͨ����������
	} KSIO_CHANNEL_PROPERTY_ARRAY, *PKSIO_CHANNEL_PROPERTY_ARRAY;






	/// <summary>
	///		�豸�������ݽṹ
	/// </summary>
	typedef struct KSIODeviceProperty
	{
		KS_UINT32				DeviceID;				//�豸ID

		KS_WSTR					DeviceName;				//ͨѶ����
		KS_WSTR					DeviceSeries;			//����ϵ��
		KS_WSTR					DeviceDescription;		//�豸����
		KS_WSTR					DeviceAddrString;		//�豸��ַ��


        KS_UINT32				ChannelID;				//�ɼ�ͨ��ID




		KS_UINT32				RetryInterval;			//���Լ��
		KS_UINT32				RetryIntervalUnit;		//���Լ����λ������ʱʹ��
		KS_UINT32				MaxRetryTime;			//��ָ�ʱ��
		KS_UINT32				MaxRetryTimeUnit;		//��ָ�ʱ�䵥λ������ʱʹ��
		KS_UINT16				RedundanceStyle;		//�������ͣ�,�����࣬����Ϊ���豸������Ϊ���豸
		KS_UINT32				RedunDeviceID;			//���������ӣ��豸ID
		KS_UINT32				RedunDeviceChannelID;	//���������ӣ��豸����ͨ����ID
		KS_BOOL					MainDevicePriority;		//�Ƿ�����ʹ�����豸

	}KSIO_DEVICE_PROPERTY,*PKSIO_DEVICE_PROPERTY;

	/// <summary>
	/// �豸��¼��
	/// </summary>
	typedef struct KSIODevicePropertyArray
	{
		KS_UINT32						Number;							//�豸������Ŀ
		PKSIO_DEVICE_PROPERTY			DevicePropertiesArray;			//�豸��������
	} KSIO_DEVICE_PROPERTY_ARRAY, *PKSIO_DEVICE_PROPERTY_ARRAY;

	//==============================================================================
	//	3����������ʹ�õ�����
	//==============================================================================

	/// <summary>
	///		ͨ������
	/// </summary>
	typedef enum KSIOChannelType
	{
		KSIO_CHANNEL_TYPE_NULL				= 0x0000,		//����·
		KSIO_CHANNEL_TYPE_COM				= 0x0001,		//����
		KSIO_CHANNEL_TYPE_ETHERNET			= 0x0002,		//��̫��
		KSIO_CHANNEL_TYPE_MODEM				= 0x0004,		//Modem
		KSIO_CHANNEL_TYPE_GPRS				= 0x0008,		//GPRS,�����⴮��
		KSIO_CHANNEL_TYPE_USB				= 0x0010,		//USB
		KSIO_CHANNEL_TYPE_CONTROL_BUS		= 0x0020,		//ControlBus
		KSIO_CHANNEL_TYPE_BOARD_CARD		= 0x0040,		//BoardCard

	} KSIO_CHANNEL_TYPE;

	/// <summary>
	/// 	���ݿ�������з�ʽ
	/// </summary>
	typedef enum KSIODataBlockQueueMode
	{
		KSIO_BLOCKQUEUE_OVERLOAD		= 0,		// ͨ���������ϵ��ȷ���ɼ�����
		KSIO_BLOCKQUEUE_GROUP			= 1,		// ���ղɼ����ڷ���
		KSIO_BLOCKQUEUE_USER			= 2,		// �û��ֶ�����
	} KSIO_DATABLOCK_QUEUE_MODE;



	/// <summary>
	/// 	�ɼ�ģ��Ԫ�ز�������
	/// </summary>
	enum KSIOCollectElementOperateType
	{
		KSIO_COLLECT_ELEMENT_ADD		= 0,		// ��
		KSIO_COLLECT_ELEMENT_REMOVE		= 1,		// ɾ
		KSIO_COLLECT_ELEMENT_MODIFY		= 2,		// ��
	};

	//==============================================================================
	//
	//	����������ݽṹ
	//
	//==============================================================================

	//==============================================================================
	//	1������������ؽṹ
	//==============================================================================

	/// <summary>
	/// 	����ѡ��
	/// </summary>
	typedef struct KSIOConnectOption
	{
		KS_WSTR						UserName;						// �û���
		KS_WSTR						Password;						// ����

		//��һ����
		KS_WSTR						ServerNetIP1;					// ���������1IP
		KS_UINT16					ServerNetPort1;					// ���������1�˿�
		KS_WSTR						ClientNetIP1;					// �ͻ�������1IP��Ŀǰδʹ��
		KS_UINT16					ClientNetPort1;					// �ͻ�������1�˿ڣ�Ŀǰδʹ��

		//�ڶ�����
		KS_WSTR						ServerNetIP2;					// ���������2IP
		KS_UINT16					ServerNetPort2;					// ���������2�˿ڣ�Ŀǰδʹ��
		KS_WSTR						ClientNetIP2;					// �ͻ�������2IP��Ŀǰδʹ��
		KS_UINT16					ClientNetPort2;					// �ͻ�������2�˿ڣ�Ŀǰδʹ��

		//��������
		KS_WSTR						ServerNetIP3;					// ���������3IP
		KS_UINT16					ServerNetPort3;					// ���������3�˿ڣ�Ŀǰδʹ��
		KS_WSTR						ClientNetIP3;					// �ͻ�������3IP�� Ŀǰδʹ��
		KS_UINT16					ClientNetPort3;					// �ͻ�������3�˿ڣ� Ŀǰδʹ��

		//��������
		KS_WSTR						ServerNetIP4;					// ���������4IP
		KS_UINT16					ServerNetPort4;					// ���������4�˿ڣ�Ŀǰδʹ��
		KS_WSTR						ClientNetIP4;					// �ͻ�������4IP��Ŀǰδʹ��
		KS_UINT16					ClientNetPort4;					// �ͻ�������4�˿ڣ�Ŀǰδʹ��

		//��������
		KS_WSTR						ServerNetIP5;					// ���������5IP
		KS_UINT16					ServerNetPort5;					// ���������5�˿ڣ�Ŀǰδʹ��
		KS_WSTR						ClientNetIP5;					// ���������5IP�� Ŀǰδʹ��
		KS_UINT16					ClientNetPort5;					// ���������5�˿ڣ�Ŀǰδʹ��


		KS_WSTR						ClientMachineName;				// Client��������
		KS_WSTR						ApplicationName;				// Ӧ�ó�������
		KS_UINT16					ClientType; 					// Client ���ͣ���kxIOClientType

		KS_UINT16					TimeOut;						// ���ӳ�ʱ����λms

		KS_UINT16					HeartBeatDetectInterval;		//	�������������λms
		KS_UINT16					MaxAllowErrorsBeforeDisconnect;	//	�ر�һ������֮ǰ������ֵ�������Ӵ�����Ŀ

		KS_UINT16	    			Reserved1; 						// �����Ķ�������
		KS_UINT16 					Reserved2; 						// �����Ķ�������
		KS_WSTR						Reserved3; 						// �����Ķ�������
		KS_WSTR						Reserved4; 						// �����Ķ�������
	}KSIO_CONNECTION_OPTION , *PKSIO_CONNECTION_OPTION;

	//==============================================================================
	//	2������������ݽṹ
	//==============================================================================

	/// <summary> 
	///		�û�
	/// </summary> 
	typedef struct KSIOUser
	{
		KS_WSTR						Name;							// �û�����
		KS_WSTR						Description;					// �û�����
		KS_WSTR						Password;						// �û�����
		WORD						Privileges;						// �û�Ȩ��,��KSIO_USER_PRIVILEGES
	} KSIO_USER, *PKSIO_USER;

	typedef struct KSIOUserArray
	{
		KS_UINT32					Number;							//�û���Ϣ����
		PKSIO_USER					UserArray;						//�û���Ϣ����
	} KSIO_USER_ARRAY, *PKSIO_USER_ARRAY;

	/// <summary> 
	///		����ֵ
	/// </summary> 
	typedef struct KSIOValue
	{
		KS_UINT16					DataType;						// ��������
		union
		{
			KS_BOOL					bitVal;							// ��������
			KS_INT8					i1Val;							// ���ֽ�����
			KS_INT16				i2Val;							// ˫�ֽ�����
			KS_INT32				i4Val;							// ���ֽ�����
			KS_INT64				i8Val;							// ���ֽ�����
			KS_UINT8				ui1Val;							// ���ֽ�����(�޷���)	
			KS_UINT16				ui2Val;							// ˫�ֽ�����(�޷���)
			KS_UINT32				ui4Val;							// ���ֽ�����(�޷���)
			KS_UINT64				ui8Val;							// ���ֽ�����(�޷���)
			KS_FLOAT32				r4Val;							// �����ȸ�����
			KS_FLOAT64				r8Val;							// ˫���ȸ�����
			KS_WSTR					wstrVal;						// Unicode�ַ���
		};					
	} KSIO_VALUE , *PSIO_VALUE;

	/// <summary> 
	///		ʱ���
	/// </summary> 
	typedef struct KSIOTimeStamp				
	{
		KS_UINT32					Seconds;						//��1970/01/01 00:00:00(UTC)����������
		KS_UINT16					Millisecs;						//������
	} KSIO_TIMESTAMP , *PKSIO_TIMESTAMP;


	/// <summary> 
	///		����ֵ
	/// </summary> 
	typedef struct KSIOTagValue
	{
		KS_UINT32					TagID;							//����ID,��ID��IOServer����
		KS_UINT32					TagFieldID;						//������ID,��ID��IOServerָ��
		KSIO_VALUE					TagValue;						//����ֵ
		KSIO_TIMESTAMP				TimeStamp;						//ʱ���
		KS_UINT32					QualityStamp;					//������
	} KSIO_TAG_VALUE, *PKSIO_TAG_VALUE;

	/// <summary> 
	///		����ֵ����
	/// </summary> 
	typedef struct KSIOTagValueArray
	{
		KS_UINT32                   Number;							//����ֵ����
		PKSIO_TAG_VALUE				TagValueArray;					//����ֵ����

	}KSIO_TAG_VALUE_ARRAY,*PKSIO_TAG_VALUE_ARRAY;



	/// <summary> 
	///		�ɼ�ģ��Ԫ�ز���
	/// </summary> 
	typedef struct KSIOCollectElementOperate
	{
		KS_UINT16					CollectElementType;				//�ɼ�ģ��Ԫ������ KSIOCollectElementType
		KS_UINT16					CollectElementOperateType;		//�ɼ�ģ��Ԫ�ز������� KSIOCollectElementOperateType
		KS_WSTR						CollectElementName;				//Ԫ����
	}KSIO_COLLECTELMENT_OPERATE, *PKSIO_COLLECTELMENT_OPERATE;

	/// <summary> 
	///		�ɼ�ģ��Ԫ�ز�������
	/// </summary> 
	typedef struct KSIOCollectElementOperateArray
	{
		KS_UINT32                   Number;							//�ɼ�ģ��Ԫ�ز�������
		PKSIO_COLLECTELMENT_OPERATE	CollectElementOperateArray;		//�ɼ�ģ��Ԫ�ز�������

	}KSIO_COLLECTELMENT_OPERATE_ARRAY,*PKSIO_COLLECTELMENT_OPERATE_ARRAY;

	/// <summary> 
	/// ��������
	/// </summary> 
	typedef struct KSIOTagProperty
	{
		//==============================================================================
		//	��������
		//==============================================================================
		KS_UINT32						TagAccessID;					/// ��������ID
		KS_WSTR							TagName;						/// �������� :  NAME
		KS_WSTR							TagFullName;					/// �������� :  CHANNEL.DEVICE.GROUP.NAME
		KS_WSTR							Description;					/// ��������	

		//==============================================================================
		//	��������
		//==============================================================================
		KS_UINT16						TagDataType;					///�����������ͣ���ֵʹ�ü�KSIO_TAG_DATA_TYPE

		// ֵ��
		KSIO_VALUE 						MaxRawValue;					/// ����ԭʼ���ֵ 			
		KSIO_VALUE 						MinRawValue;					/// ����ԭʼ��Сֵ
		KSIO_VALUE 						MaxValue;						/// �����������ֵ 			
		KSIO_VALUE 						MinValue;						/// ����������Сֵ

		KS_UINT16						ConvertType;                    /// �����任���� ��ֵʹ�ü� kxTagConvertType
		KS_BOOL					        IsFilter;				        /// �Ƿ��������
		KS_FLOAT32					    DeadBand;					    /// �����ٷֱ�

		//==============================================================================
		//	��������
		//==============================================================================

		KS_WSTR							ChannelName;					// ͨ������
		KS_WSTR							DeviceName;						// �豸����
		KS_WSTR							GroupName;						// ������

		//==============================================================================
		//	�ɼ�����
		//==============================================================================

		KS_BOOL							CollectControl;				    /// �ɼ�����(�Ƿ�ɼ���־)
		KS_UINT32 						CollectInterval;				/// �ɼ����(��������:��λms)			
		KS_UINT32 						CollectOffset;				    /// �ɼ�ƫ��(��λ:��λms)
		KS_INT32						TimeZoneBias;					/// ��UTC��׼ʱ��Ĳ�ֵ(��λ������)
		KS_INT32						TimeAdjustment;					/// ʱ��У��(��λ:����)

		//==============================================================================
		//	��������,ע����Ϊ����3.0�汾ʹ��
		//==============================================================================
		KS_UINT32						ChannelID;						///�ɼ�ͨ��ID
		KS_UINT32						DeviceID;						///�ɼ��豸ID
		KS_UINT32						DataBlockID;					///���ݿ�ID

		//==============================================================================
		//	������������
		//==============================================================================
		KS_WSTR							ItemName;						///Item���ƣ�Item��������������

		//����Item��
		//ͨ�õ�IOServer�ǼĴ���+ƫ���ַ�������
		//OPC��IOServerΪOPC Item������
		//DDE��IOServerΪDDE Item������
		KS_WSTR				        	RegName;
		KS_UINT16						RegType;
		KS_UINT16						ItemDataType;					///Item�������ͣ���ֵʹ�ü�KSIO_REG_DATA_TYPE��DDE��������
		KS_UINT16						ItemAccessMode;					///��д����ģʽ,��ֵʹ�ü�KSIO_ACCESS_MODE��DDE��������

		//==============================================================================
		//	��������
		//==============================================================================

		KS_UINT32						TagGeneral1;
		KS_UINT32						TagGeneral2;
		KS_UINT32						TagGeneral3;
		KS_UINT32						TagGeneral4;
		KS_UINT32						TagGeneral5;

		KS_WSTR							TagGeneral11;
		KS_WSTR							TagGeneral12;
		KS_WSTR							TagGeneral13;
		KS_WSTR							TagGeneral14;
		KS_WSTR							TagGeneral15;

	} KSIO_TAG_PROPERTY,*PKSIO_TAG_PROPERTY;

	/// <summary> 
	/// ������¼��
	/// </summary> 
	typedef struct KSIOTagPropertyArray
	{
		KS_UINT32						Number;							/// ����������Ŀ
		PKSIO_TAG_PROPERTY				TagPropertiesArray;				/// ������������
	} KSIO_TAG_PROPERTY_ARRAY, *PKSIO_TAG_PROPERTY_ARRAY;

	//==============================================================================
	//	3���豸������ݽṹ��������KS3.0�汾ʹ��
	//==============================================================================

	/// <summary> 
	///		�豸״̬
	/// </summary> 
	typedef struct KSIODeviceStatus
	{
		KS_WSTR						IOServerName;			//IOServer����
		KS_UINT32					ChannelID;				//�ɼ�ͨ��ID,��IOServer����,��IOServer�Ͽ��Բ鿴
		KS_WSTR						ChannelName;			//�ɼ�ͨ������
		KS_UINT32					DeviceID;				//�ɼ��豸ID,��IOServer����,��IOServer�Ͽ��Բ鿴
		KS_WSTR						DeviceName;				//�ɼ��豸����
		KS_UINT16					DeviceStatus;			//�豸״̬,��KSIO_DEV_COMM_STATUS

		//���±���
		KS_UINT32					Reserved1;				//�����Ķ�������
		KS_UINT32					Reserved2;				//�����Ķ�������
		KS_WSTR						Reserved3;				//�����Ķ�������
		KS_WSTR						Reserved4;				//�����Ķ�������
	} KSIO_DEV_STATUS, *PKSIO_DEV_STATUS;


	/// <summary> 
	///		�豸״̬
	/// </summary> 
	typedef struct KSIODeviceStatusArray
	{
		KS_UINT32					Number;					//�豸״̬����
		PKSIO_DEV_STATUS			DeviceStatusArray;		//�豸״̬����
	} KSIO_DEV_STATUS_ARRAY, *PKSIO_DEV_STATUS_ARRAY;

	//<summary>
	//		�Ĵ�������
	//</summary>
	/// <remarks>
	///      ����Ʒ����ȷ��KS3.5�м������ݱ���IO���Բ�����ʹ�øýṹ
	/// </remarks>
	typedef struct KSIORegister 
	{
		KS_WSTR						RegisterName;			//�Ĵ�������
		KS_UINT32					LowOffset;				//�͵�ַƫ��
		KS_UINT32					UpperOffset;			//�ߵ�ַƫ��
		KS_UINT16					RegisterDataType;		//�Ĵ�����������
		KS_UINT16					AccessMode;				//Ŀǰû�õ�
	} KSIO_REGISTER, *PKSIO_REGISTER;

	//<summary>
	//		�Ĵ�����������
	//</summary>
	/// <remarks>
	///      ����Ʒ����ȷ��KS3.5�м������ݱ���IO���Բ�����ʹ�øýṹ
	/// </remarks>
	typedef struct KSIORegisterArray
	{
		KS_UINT16					Number;					//�Ĵ������Ը���
		PKSIO_REGISTER				RegisterArray;			//�Ĵ�����������			
	} KSIO_REGISTER_ARRAY, *PKSIO_REGISTER_ARRAY;


	//==============================================================================
	//	4�����йؼ������ṹ��˵��
	//		3.5�汾IOServerû�������ݿ�ĸ��API��Ҳû����ͨ�����豸����Ϣ
	//		����ͨ�����豸��Ϣ��ͨ��ϵͳ���ӱ�����ʵ�ֵģ���˲���3.0�汾������
	//==============================================================================


	//==============================================================================
	//	5���������ݽṹ
	//==============================================================================

	/// <summary>
	/// 	��������
	/// </summary>
	typedef struct KSIOLongArray
	{
		KS_UINT32					Number;
		KS_UINT32*					LongArray;

	}KSIO_LONGARRAY,*PKSIO_LONGARRAY;

	/// <summary>
	/// 	�ַ�������
	/// </summary>
	typedef struct KSIOStringArray
	{
		KS_UINT32					Number;
		KS_WSTR*					StringArray;

	}KSIO_STRINGARRAY,*PKSIO_STRINGARRAY;


	//==============================================================================
	//
	//	�ġ��ص���������  ע���ص������˶���������ڵ�İ��ݴ���
	//
	//==============================================================================

	/// <summary>
	///		����������ӻص�������
	/// </summary>
	/// <param name="Handle">
	/// 	���Ӿ��
	/// </param>
	/// <param name="ConnectStatus">
	///		����״̬
	/// </param>
	typedef KS_RET ( CALLBACK *KSIO_CONNECT_STATUS_CHANGED_CALLBACKFUNC )(
		IN KS_HANDLE								Handle, 
		IN KSIO_CONNECT_STATUS						ConnectStatus);

	/// <summary>
	/// 	IOServer����״̬�仯�ص��ӿ�
	/// </summary>
	/// <param name="Handle">
	/// 	���Ӿ��
	/// </param>
	/// <param name="NodeType">
	/// 	IOServer�ڵ�����
	/// </param>
	/// <param name="WorkStatus">
	///		IOServer����״̬	
	/// </param>
	typedef KS_RET ( CALLBACK *KSIO_WORK_STATUS_CHANGED_CALLBACKFUNC )(
		IN KS_HANDLE								Handle, 
		IN KSIO_NODE_TYPE							NodeType, 
		IN KSIO_WORK_STATUS							WorkStatus);

	/// <summary>
	/// 	IOServer����״̬�仯�ص��ӿ�
	/// </summary>
	/// <param name="Handle">
	/// 	���Ӿ��
	/// </param>
	/// <param name="NodeType">
	/// 	IOServer�ڵ�����
	/// </param>
	/// <param name="RunningStatus">
	///		IOServer����״̬
	/// </param>
	typedef KS_RET ( CALLBACK *KSIO_RUNNING_STATUS_CHANGED_CALLBACKFUNC )(
		IN KS_HANDLE								Handle, 
		IN KSIO_NODE_TYPE							NodeType, 
		IN KSIO_RUN_STATUS							RunningStatus);

	/// <summary> 
	/// 	�ɼ����ݻص��ӿ�
	/// </summary> 
	/// <param name="Handle">
	/// 	���Ӿ��
	/// </param>
	/// <param name="VaribleValueArray">
	///		����ֵ����
	/// </param>
	typedef KS_RET ( CALLBACK *KSIO_COLLECT_VALUE_CALLBACKFUNC )(
		IN KS_HANDLE								Handle, 
		IN PKSIO_TAG_VALUE_ARRAY					TagValueArray);

	/// <summary>
	///		д������ɻص��ӿ�
	/// </summary>
	/// <param name="Handle">
	/// 	���Ӿ��
	/// </param>
	/// <param name="TagIDArray">
	///		����ID����
	/// </param>
	/// <param name="TagAccessFieldIDArray">
	///		����������ID����
	/// </param>
	/// <param name="ErrorCodeArray">
	///		����������
	/// </param>
	typedef KS_RET ( CALLBACK * KSIO_WRITE_COMPLTED_CALLBACKFUNC ) (
		IN KS_HANDLE							Handle,
		IN PKSIO_LONGARRAY						TagIDArray,
		IN PKSIO_LONGARRAY						TagAccessFieldIDArray,
		IN PKSIO_LONGARRAY						ErrorCodeArray);

	/// <summary>
	///		��������ɻص��ӿ�
	/// </summary>
	/// <param name="Handle">
	/// 	���Ӿ��
	/// </param>
	/// <param name="TagValueArray">
	///		����ֵ����
	/// </param>
	/// <param name="ErrorCodeArray">
	///		����������
	/// </param>
	typedef KS_RET ( CALLBACK * KSIO_READ_COMPLTED_CALLBACKFUNC ) (
		IN KS_HANDLE							Handle,
		IN PKSIO_TAG_VALUE_ARRAY				TagValueArray,
		IN PKSIO_LONGARRAY						ErrorCodeArray);

	/// <summary>
	///		�ɼ�ģ��Ԫ�ز����ص��ӿ�
	/// </summary>
	/// <param name="Handle">
	/// 	���Ӿ��
	/// </param>
	/// <param name="CollectElementOperateArray">
	///		�ɼ�ģ��Ԫ�ز�������
	/// </param>
	typedef KS_RET ( CALLBACK * KSIO_COLLECTELEMENT_OPERTATE_CHANGED_CALLBACKFUNC ) (
		IN KS_HANDLE							Handle,
		IN PKSIO_COLLECTELMENT_OPERATE_ARRAY	CollectElementOperateArray);

	/// <summary>
	///		�豸״̬�仯�ص��ӿ�
	/// </summary>
	/// <param name="Handle">
	/// 	���Ӿ��
	/// </param>
	/// <param name="DeviceStatusArray">
	///		�豸״̬����
	/// </param>
	/// <remarks>
	///     ��KS3.0�汾ʹ��
	/// </remarks>
	typedef KS_RET ( CALLBACK * KSIO_DEVICE_STATUS_CHANGE_CALLBACKFUNC) (
		IN KS_HANDLE							Handle,
		IN PKSIO_DEV_STATUS_ARRAY				DeviceStatusArray);

	//==============================================================================
	//
	//	�塢�����ӿں���
	//
	//==============================================================================

	/// <summary>
	/// 	��ʼ��IOServer�ӿ�
	/// </summary>
	/// <returns>
	///		����ɹ�����KS_OK,���ʧ�ܷ�����Ӧ������
	/// </returns>
	/// <remarks>
	///     ������ʼ���������Ӻ�������Դ
	/// </remarks>
	KS_RET KSIOAPI KSIOAPIInitalize(IN const KS_WSTR String);


	/// <summary>
	/// 	�ͷ�IOServer�ӿ���Դ
	/// </summary>
	/// <returns>
	///		����ɹ�����KS_OK,���ʧ�ܷ�����Ӧ������
	/// </returns>
	/// <remarks>
	///     ����ͻ���ռ�õ���Դ�������������õ���Դ
	/// </remarks>
	KS_RET KSIOAPI KSIOAPIRelease();


	/// <summary>
	///    �ͷ��ַ�����Դ
	/// </summary>
	/// <param name="String">
	///		���ͷŵ��ַ���ָ��
	/// </param>
	/// <returns>
	///		����ɹ�����KS_OK,���ʧ�ܷ�����Ӧ������
	/// </returns>
	KS_RET KSIOAPI KSIOAPIFreeString(IN KS_WSTR& String);


	/// <summary>
	/// 	�ͷ��ַ���������Դ
	/// </summary>
	/// <param name="StringArray">
	///		���ͷŵ��ַ�������
	/// </param>
	/// <returns>
	///		����ɹ�����KS_OK,���ʧ�ܷ�����Ӧ������
	/// </returns>
	KS_RET KSIOAPI KSIOAPIFreeStringArray(IN PKSIO_STRINGARRAY StringArray);

	/// <summary>
	/// 	�ͷ�����������Դ
	/// </summary>
	/// <param name="LongArray">
	///		���ͷŵ���������
	/// </param>
	/// <returns>
	///		����ɹ�����KS_OK,���ʧ�ܷ�����Ӧ������
	/// </returns>
	KS_RET KSIOAPI KSIOAPIFreeLongArray(IN PKSIO_LONGARRAY LongArray);

	/// <summary>
	/// 	�ͷű�����¼��
	/// </summary>
	/// <param name="TagPropertiesArray">
	///		������������
	/// </param>
	/// <returns>
	///		����ɹ�����KS_OK,���ʧ�ܷ�����Ӧ������
	/// </returns>
	KS_RET KSIOAPI KSIOAPIFreeCollectTags(IN PKSIO_TAG_PROPERTY_ARRAY TagPropertiesArray);

	/// <summary>
	/// 	�ͷű���ֵ������Դ
	/// </summary>
	/// <param name="TagValueArray">
	///		����ֵ������Դ
	/// </param>
	/// <returns>
	///		����ɹ�����KS_OK,���ʧ�ܷ�����Ӧ������
	/// </returns>
	KS_RET KSIOAPI KSIOAPIFreeTagsValue(IN PKSIO_TAG_VALUE_ARRAY TagValueArray);

	/// <summary>
	/// 	�ͷżĴ���������Դ
	/// </summary>
	/// <param name="RegisterArray">
	///		�Ĵ���������Դ
	/// </param>
	/// <returns>
	///		����ɹ�����KS_OK,���ʧ�ܷ�����Ӧ������
	/// </returns>
	KS_RET KSIOAPI KSIOAPIRegistersArray(IN PKSIO_REGISTER_ARRAY RegisterArray);



	
	/// <summary>
	/// 	�ͷŹ��̼�¼��
	/// </summary>
	/// <param name="TagPropertiesArray">
	///		������������
	/// </param>
	/// <returns>
	///		����ɹ�����KS_OK,���ʧ�ܷ�����Ӧ������
	/// </returns>
	KS_RET KSIOAPI KSIOAPIFreeProjects(IN PSIO_SERVER_PROPERTY_ARRAY PropertiesArray);

	/// <summary>
	/// 	�ͷ�ͨ����¼��
	/// </summary>
	/// <param name="TagPropertiesArray">
	///		������������
	/// </param>
	/// <returns>
	///		����ɹ�����KS_OK,���ʧ�ܷ�����Ӧ������
	/// </returns>
	KS_RET KSIOAPI KSIOAPIFreeChannels(IN PKSIO_CHANNEL_PROPERTY_ARRAY PropertiesArray);

	/// <summary>
	/// 	�ͷ��豸��¼��
	/// </summary>
	/// <param name="TagPropertiesArray">
	///		������������
	/// </param>
	/// <returns>
	///		����ɹ�����KS_OK,���ʧ�ܷ�����Ӧ������
	/// </returns>
	KS_RET KSIOAPI KSIOAPIFreeDevices(IN PKSIO_DEVICE_PROPERTY_ARRAY PropertiesArray);
	/// <summary>
	/// 	�ͷű�����¼��
	/// </summary>
	/// <param name="TagPropertiesArray">
	///		������������
	/// </param>
	/// <returns>
	///		����ɹ�����KS_OK,���ʧ�ܷ�����Ӧ������
	/// </returns>
	KS_RET KSIOAPI KSIOAPIFreeCollectTags(IN PKSIO_TAG_PROPERTY_ARRAY TagPropertiesArray);

	//==============================================================================
	//
	//	������������������ƽӿں���
	//
	//==============================================================================

	/// <summary>
	/// 	����IOServer
	/// </summary>
	/// <param name="ConnectOption">
	///		����ѡ��
	/// </param>
	/// <param name="Handle">
	///		IOServer���Ӿ��
	/// </param>
	/// <returns>
	///		����ɹ�����KS_OK,���ʧ�ܷ�����Ӧ������
	/// </returns>
	/// <remarks>
	///     �����������ݿ⣬���ӳɹ���������һ����Ч���ݿ������Ժ�������ݿ�ĵ��ö�����ʹ�øþ��
	/// </remarks>
	KS_RET KSIOAPI KSIOAPIServerConnect(
		IN PKSIO_CONNECTION_OPTION					ConnectOption ,
		OUT KS_HANDLE&								Handle);

	/// <summary>
	/// 	����IOServer��չ����
	/// </summary>
	/// <param name="PrimaryIOConnOption">
	///		��Ϊ�����Primary IOServer����ѡ��
	/// </param>
	/// <param name="SecondaryIOConnOption">
	///		��Ϊ�����Secondary IOServer����ѡ��
	/// </param>
	/// <param name="Handle">
	///		IOServer���Ӿ��
	/// </param>
	/// <returns>
	///		����ɹ�����KS_OK,���ʧ�ܷ�����Ӧ������
	/// </returns>
	/// <remarks>
	///     �������IOServer���࣬ʹ�ø÷���
	/// </remarks>
	KS_RET KSIOAPI KSIOAPIServerConnectEx(
		IN PKSIO_CONNECTION_OPTION					PrimaryIOConnOption ,
		IN PKSIO_CONNECTION_OPTION					SecondaryIOConnOption ,
		OUT KS_HANDLE&								Handle);


	/// <summary>
	/// 	��ȡ����״̬
	/// </summary>
	/// <param name="Handle">
	///		IOServer���Ӿ��
	/// </param>
	/// <param name="bConnect">
	///		����״̬��true:���� false���ӶϿ�
	/// </param>
	/// <returns>
	///		����ɹ�����KS_OK,���ʧ�ܷ�����Ӧ������
	/// </returns>
	/// <remarks>
	///     
	/// </remarks>
	KS_RET KSIOAPI  KSIOAPIServerIsConnected(
		IN KS_HANDLE								Handle,
		OUT KS_BOOL&								bConnect);

	/// <summary>
	/// 	�Ͽ�����
	/// </summary>
	/// <param name="Handle">
	///		IOServer���Ӿ��
	/// </param>
	/// <returns>
	///		����ɹ�����KS_OK,���ʧ�ܷ�����Ӧ������
	/// </returns>
	/// <remarks>
	///     ���ڶϿ�DBHandle����ʶ������
	/// </remarks>
	KS_RET KSIOAPI KSIOAPIServerDisconnect(IN_OUT KS_HANDLE& Handle);

	//==============================================================================
	//
	//	�ߡ��û�����ӿں���
	//	
	//==============================================================================

	/// <summary>
	/// 	�����û�
	/// </summary>
	/// <param name="Handle">
	///		IOServer���Ӿ��
	/// </param>
	/// <param name="OperatorName">
	///		����Ա����
	/// </param>
	/// <param name="OperatorPassWord">
	///		����Ա����
	/// </param>
	/// <param name="User">
	///		�û���Ϣ
	/// </param>
	/// <returns>
	///   ����ɹ�����KS_OK,���ʧ�ܷ�����Ӧ������
	/// </returns>
	/// <remarks>
	///     
	/// </remarks>
	KS_RET KSIOAPI	KSIOAPIAddUser(
		IN KS_HANDLE								Handle,
		IN KS_WSTR									OperatorName,
		IN KS_WSTR									OperatorPassWord,
		IN PKSIO_USER								User);

	/// <summary>
	/// 	ɾ���û�
	/// </summary>
	/// <param name="Handle">
	///		IOServer���Ӿ��
	/// </param>
	/// <param name="OperatorName">
	///		����Ա����
	/// </param>
	/// <param name="OperatorPassWord">
	///		����Ա����
	/// </param>
	/// <param name="Name">
	///		�û�����
	/// </param>
	/// <returns>
	///   ����ɹ�����KS_OK,���ʧ�ܷ�����Ӧ������
	/// </returns>
	/// <remarks>
	///     
	/// </remarks>
	KS_RET KSIOAPI KSIOAPIDeleteUser(
		IN KS_HANDLE								Handle,
		IN KS_WSTR									OperatorName,
		IN KS_WSTR									OperatorPassWord,
		IN KS_WSTR									Name );

	/// <summary>
	/// 	ö��ϵͳ�û���Ϣ
	/// </summary>
	/// <param name="Handle">
	///		IOServer���Ӿ��
	/// </param>
	/// <param name="OperatorName">
	///		����Ա����
	/// </param>
	/// <param name="OperatorPassWord">
	///		����Ա����
	/// </param>
	/// <param name="UserArray">
	///		�û���Ϣ����
	/// </param>
	/// <returns>
	///   ����ɹ�����KS_OK,���ʧ�ܷ�����Ӧ������
	/// </returns>
	/// <remarks>
	///     
	/// </remarks>
	KS_RET KSIOAPI KSIOAPIEnumUsers(
		IN KS_HANDLE								Handle,
		IN KS_WSTR									OperatorName,
		IN KS_WSTR									OperatorPassWord,
		OUT PKSIO_USER_ARRAY						UserArray);

	/// <summary>
	/// 	ͨ���û����ƻ�ȡ�û���Ϣ
	/// </summary>
	/// <param name="Handle">
	///		IOServer���Ӿ��
	/// <param name="Name">
	///		�û�����
	/// </param>
	/// <param name="OperatorName">
	///		����Ա����
	/// </param>
	/// <param name="OperatorPassWord">
	///		����Ա����
	/// </param>
	/// </param>
	/// <param name="User">
	///		�û���Ϣ
	/// </param>
	/// <returns>
	///   ����ɹ�����KS_OK,���ʧ�ܷ�����Ӧ������
	/// </returns>
	/// <remarks>
	///     
	/// </remarks>
	KS_RET KSIOAPI KSIOAPIGetUserByName(
		IN KS_HANDLE								Handle,
		IN KS_WSTR									OperatorName,
		IN KS_WSTR									OperatorPassWord,
		IN KS_WSTR									Name,
		OUT PKSIO_USER								User);	

	//==============================================================================
	//
	//	�ˡ�������������֤�ӿں���������KS3.0������
	//	
	//==============================================================================

	/// <summary>
	/// 	ö������ͨ��������
	/// </summary>
	/// <param name="Handle">
	///		IOServer���Ӿ��
	/// </param>
	/// <param name="ChannelNameArray">
	///		ͨ����������
	/// </param>
	/// <returns>
	///   ����ɹ�����KS_OK,���ʧ�ܷ�����Ӧ������
	/// </returns>
	/// <remarks>
	///     
	/// </remarks>
	KS_RET KSIOAPI KSIOAPIEnumAllChannelNames(
		IN KS_HANDLE								Handle,
		OUT PKSIO_STRINGARRAY						ChannelNameArray) ;


	/// <summary>
	/// 	����ͨ�����ƻ�ȡͨ��ID
	/// </summary>
	/// <param name="Handle">
	///		IOServer���Ӿ��
	/// </param>
	/// <param name="ChannelNameArray">
	///		ͨ����������
	/// </param>
	/// <param name="ChannelIDArray">
	///		ͨ��ID����
	/// </param>
	/// <param name="ErrorCodeArray">
	///		����������
	/// </param>
	/// <returns>
	///   ����ɹ�����KS_OK,���ʧ�ܷ�����Ӧ������
	/// </returns>
	/// <remarks>
	///     
	/// </remarks>
	KS_RET KSIOAPI KSIOAPIGetChannelIDByName(
		IN KS_HANDLE								Handle,
		IN PKSIO_STRINGARRAY						ChannelNameArray, 
		OUT PKSIO_LONGARRAY							ChannelIDArray, 
		OUT PKSIO_LONGARRAY							ErrorCodeArray) ;

	/// <summary>
	/// 	ö��ͨ���µ������豸����
	/// </summary>
	/// <param name="Handle">
	///		IOServer���Ӿ��
	/// </param>
	/// <param name="ChannelName">
	///		ͨ������
	/// </param>
	/// <param name="DeviceNameArray">
	///		�豸�������飬ָ�豸�߼���
	/// </param>
	/// <returns>
	///   ����ɹ�����KS_OK,���ʧ�ܷ�����Ӧ������
	/// </returns>
	/// <remarks>
	///     
	/// </remarks>
	KS_RET KSIOAPI KSIOAPIEnumChannelAllDeviceNames(
		IN KS_HANDLE								Handle,
		IN KS_WSTR									ChannelName, 
		OUT PKSIO_STRINGARRAY						DeviceNameArray) ;


	/// <summary>
	/// 	�����豸����ȡ�豸ID
	/// </summary>
	/// <param name="Handle">
	///		IOServer���Ӿ��
	/// </param>
	/// <param name="ChannelNameArray">
	///		ͨ��������
	/// </param>
	/// <param name="DeviceNameArray">
	///		�豸�������飬ָ�豸�߼���
	/// </param>
	/// <param name="DeviceIDArray">
	///		�豸ID����
	/// </param>
	/// <param name="ErrorCodeArray">
	///		����������
	/// </param>
	/// <returns>
	///   ����ɹ�����KS_OK,���ʧ�ܷ�����Ӧ������
	/// </returns>
	/// <remarks>
	///     
	/// </remarks>
	KS_RET KSIOAPI KSIOAPIGetDeviceIDByName(
		IN KS_HANDLE								Handle,
		IN PKSIO_STRINGARRAY						ChannelNameArray, 
		IN PKSIO_STRINGARRAY						DeviceNameArray, 
		OUT PKSIO_LONGARRAY							DeviceIDArray, 
		OUT PKSIO_LONGARRAY							ErrorCodeArray) ;

	/// <summary>
	/// 	ö���豸�µ��������ݿ�
	/// </summary>
	/// <param name="Handle">
	///		IOServer���Ӿ��
	/// </param>
	/// <param name="ChannelName">
	///		ͨ������
	/// </param>
	/// <param name="DeviceName">
	///		�豸���ƣ�ָ�豸�߼���
	/// </param>
	/// <param name="DataBlockNameArray">
	///		���ݿ���������
	/// </param>
	/// <returns>
	///   ����ɹ�����KS_OK,���ʧ�ܷ�����Ӧ������
	/// </returns>
	/// <remarks>
	///     
	/// </remarks>
	KS_RET KSIOAPI KSIOAPIEnumDeviceAllDataBlockNames(
		IN KS_HANDLE								Handle,
		IN KS_WSTR									ChannelName, 
		IN KS_WSTR									DeviceName, 
		OUT PKSIO_STRINGARRAY						DataBlockNameArray) ;


	/// <summary>
	/// 	�������ƻ�ȡ���ݿ�ID
	/// </summary>
	/// <param name="Handle">
	///		IOServer���Ӿ��
	/// </param>
	/// <param name="ChannelNameArray">
	///		ͨ����������
	/// </param>
	/// <param name="DeviceNameArray">
	///		�豸�������飬ָ�豸�߼���
	/// </param>
	/// <param name="DataBlockNameArray">
	///		���ݿ���������
	/// </param>
	/// <param name="DataBlockIDArray">
	///		���ݿ�ID����
	/// </param>
	/// <param name="ErrorCodeArray">
	///		����������
	/// </param>
	/// <returns>
	///   ����ɹ�����KS_OK,���ʧ�ܷ�����Ӧ������
	/// </returns>
	/// <remarks>
	///     
	/// </remarks>
	KS_RET KSIOAPI KSIOAPIGetDataBlockIDByName(
		IN KS_HANDLE								Handle,
		IN PKSIO_STRINGARRAY						ChannelNameArray, 
		IN PKSIO_STRINGARRAY						DeviceNameArray, 
		IN PKSIO_STRINGARRAY						DataBlockNameArray, 
		OUT PKSIO_LONGARRAY							DataBlockIDArray, 
		OUT PKSIO_LONGARRAY							ErrorCodeArray) ;

	/// <summary>
	/// 	��ȡ�豸�ļĴ�����Ϣ
	/// </summary>
	/// <param name="Handle">
	///		IOServer���Ӿ��
	/// </param>
	/// <param name="ChannelID">
	///		��·ID
	/// <param name="DeviceID">
	///		�豸ID
	/// </param>
	/// <param name="RegInfoArray">
	///		�Ĵ�����Ϣ����
	/// </param>
	/// <returns>
	///   ����ɹ�����KS_OK,���ʧ�ܷ�����Ӧ������
	/// </returns>
	/// <remarks>
	///      ����Ʒ����ȷ��KS3.5�м������ݱ���IO���Բ�����ʹ�øú���
	/// </remarks>
	KS_RET KSIOAPI KSIOAPIGetRegInfobyDeviceID(
		IN KS_HANDLE								Handle,
		IN KS_UINT32								ChannelID,
		IN KS_UINT32								DeviceID, 
		OUT PKSIO_REGISTER_ARRAY					RegInfoArray) ;


	/// <summary>
	/// 	��ȡ���ݿ�ļĴ�����Ϣ
	/// </summary>
	/// <param name="Handle">
	///		IOServer���Ӿ��
	/// </param>
	/// <param name="ChannelID">
	///		��·ID
	/// <param name="DeviceID">
	///		�豸ID
	/// <param name="DataBlockID">
	///		���ݿ�ID
	/// </param>
	/// <param name="RegInfo">
	///		�Ĵ�����Ϣ
	/// </param>
	/// <returns>
	///   ����ɹ�����KS_OK,���ʧ�ܷ�����Ӧ������
	/// </returns>
	/// <remarks>
	///      ����Ʒ����ȷ��KS3.5�м������ݱ���IO���Բ�����ʹ�øú���
	/// </remarks>
	KS_RET KSIOAPI KSIOAPIGetRegInfobyDataBlockID(
		IN KS_HANDLE								Handle,
		IN KS_UINT32								ChannelID,
		IN KS_UINT32								DeviceID, 
		IN KS_UINT32								DataBlockID, 
		OUT PKSIO_REGISTER							RegInfo) ;

	/// <summary>
	/// 	��ȡ���ݿ�ɼ�����
	/// </summary>
	/// <param name="Handle">
	///		IOServer���Ӿ��
	/// </param>
	/// <param name="ChannelID">
	///		��·ID
	/// <param name="DeviceID">
	///		�豸ID
	/// <param name="CollectInterval">
	///		���ݿ�ɼ�����
	/// </param>
	/// <returns>
	///   ����ɹ�����KS_OK,���ʧ�ܷ�����Ӧ������
	/// </returns>
	/// <remarks>
	///     
	/// </remarks>
	KS_RET KSIOAPI KSIOAPIGetDataBlockCollectInterval(
		IN KS_HANDLE								Handle,
		IN KS_UINT32								ChannelID,
		IN KS_UINT32								DeviceID, 
		IN KS_UINT32								DataBlockID, 
		OUT KS_UINT32&								CollectInterval) ;

	//==============================================================================
	//
	//	�š����з��ʽӿں���
	//
	//==============================================================================


		//==============================================================================
	//	2��IOServer������״̬
	//==============================================================================

	/// <summary>
	/// 	���ƶ�������״̬
	/// </summary>
	/// <param name="Handle">
	///		IOServer���Ӿ��
	/// </param>
	/// <param name=" ItemType ">
	///		���ʽڵ����ͣ��ο�
	/// </param>
	/// <param name=" ItemIDs ">
	///		���ʽڵ�ID����
	/// </param>
	/// <param name=" ItemStates ">
	///		�������ȡ������ڵ�
	/// </param>
	/// <returns>
	///		����ɹ�����KS_OK,���ʧ�ܷ�����Ӧ������
	/// </returns>
	/// <remarks>
	///		����Ѿ�����Ҳ����KS_OK
	/// </remarks>
	KS_RET KSIOAPI KSIOAPISetItemState(
		IN KS_HANDLE								Handle,
		IN KS_UINT16                     ItemType,
		IN KS_UINT32							ItemID,
		IN KS_UINT16						ItemState);


	/// <summary>
	/// 	���ƶ�������״̬
	/// </summary>
	/// <param name="Handle">
	///		IOServer���Ӿ��
	/// </param>
	/// <param name=" ItemType ">
	///		���ʽڵ����ͣ��ο�
	/// </param>
	/// <param name=" ItemIDs ">
	///		���ʽڵ�ID����
	/// </param>
	/// <param name=" ItemStates ">
	///		�������ȡ������ڵ�
	/// </param>
	/// <returns>
	///		����ɹ�����KS_OK,���ʧ�ܷ�����Ӧ������
	/// </returns>
	/// <remarks>
	///		����Ѿ�����Ҳ����KS_OK
	/// </remarks>
	KS_RET KSIOAPI KSIOAPIGetItemState(
		IN KS_HANDLE								Handle,
		IN KS_UINT16                     ItemType,
		IN KS_UINT32							ItemID,
		OUT KS_UINT16						ItemState);
	//==============================================================================
	//	1�������ɼ����ýӿ�
	//==============================================================================

	/// <summary>
	/// 	��ӱ���������Ϣ
	/// </summary>
	/// <param name="Handle">
	///		IOServer���Ӿ��
	/// </param>
	/// <param name="TagPropertyArray">
	///		�ɼ���������
	/// </param>
	/// <param name="ErrorCodeArray">
	///		����������
	/// </param>
	/// <returns>
	///		����ɹ�����KS_OK,���ʧ�ܷ�����Ӧ������
	/// </returns>
	/// <remarks>
	///		����KS3.0������
	/// </remarks>
	KS_RET KSIOAPI KSIOAPIAddCollectTags(
		IN KS_HANDLE								Handle, 
		IN PKSIO_TAG_PROPERTY_ARRAY					TagPropertyArray,
		OUT PKSIO_LONGARRAY							ErrorCodeArray);

	/// <summary>
	/// 	ɾ���ɼ�����
	/// </summary>
	/// <param name="Handle">
	///		IOServer���Ӿ��
	/// </param>
	/// <param name="TagAccessIDArray">
	///		�ɼ���������ID����
	/// </param>
	/// <param name="ErrorCodeArray">
	///		����������
	/// </param>
	/// <returns>
	///		����ɹ�����KS_OK,���ʧ�ܷ�����Ӧ������
	/// </returns>
	/// <remarks>
	///		����KS3.0������
	/// </remarks>
	KS_RET KSIOAPI KSIOAPIRemoveCollectTags(
		IN KS_HANDLE								Handle, 
		IN PKSIO_LONGARRAY							TagAccessIDArray,
		OUT PKSIO_LONGARRAY							ErrorCodeArray);


	/// <summary> 
	///		��λ�����ɼ�����
	/// </summary> 
	/// <param name="Handle">
	///		IOServer���Ӿ��
	/// </param>
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
	/// <param name="TagPropertyArray">
	///		������������
	/// </param>
	/// <returns>
	///		����ɹ�����KS_OK,���ʧ�ܷ�����Ӧ������
	/// </returns>
	/// <remarks>
	///		��ͨ��-�豸-�����Ĳ�λ�
	/// </remarks>
	KS_RET KSIOAPI	KSIOAPIBrowseCollectTagsHierarchical(
		IN KS_HANDLE								Handle,
		IN KS_CWSTR									BrowsePosition,
		IN KS_BOOL									BrowseRecursive,
		IN KS_CWSTR									BranchFilterMask,
		IN KS_CWSTR									TagNameMask,
		IN KS_CWSTR									TagDescriptionMask,
		OUT PKSIO_STRINGARRAY						ChildNodeNames,
		OUT PKSIO_TAG_PROPERTY_ARRAY					TagPropertyArray );


	/// <summary>
	/// 	���ݱ���ȫ����ȡ��������ID
	/// </summary>
	/// <param name="Handle">
	///		IOServer���Ӿ��
	/// </param>
	/// <param name="TagFullNameArray">
	///		�ɼ�����ȫ������
	/// </param>
	/// <param name="TagAccessIDArray">
	///		�ɼ���������ID����
	/// </param>
	/// <param name="ErrorCodeArray">
	///		����������
	/// </param>
	/// <returns>
	///		����ɹ�����KS_OK,���ʧ�ܷ�����Ӧ������
	/// </returns>
	/// <remarks>
	///		
	/// </remarks>
	KS_RET KSIOAPI KSIOAPIGetTagAccessIDsByNames(
		IN KS_HANDLE								Handle, 
		IN PKSIO_STRINGARRAY						TagFullNameArray, 
		OUT PKSIO_LONGARRAY							TagAccessIDArray,
		OUT PKSIO_LONGARRAY							ErrorCodeArray);

	//==============================================================================
	//	2��IOServer������״̬
	//==============================================================================

	/// <summary>
	/// 	����IOServer����
	/// </summary>
	/// <param name="Handle">
	///		IOServer���Ӿ��
	/// </param>
	/// <param name="OperatorName">
	///		����Ա����
	/// </param>
	/// <param name="OperatorPassWord">
	///		����Ա����
	/// </param>
	/// <returns>
	///		����ɹ�����KS_OK,���ʧ�ܷ�����Ӧ������
	/// </returns>
	/// <remarks>
	///		����Ѿ�����Ҳ����KS_OK
	/// </remarks>
	KS_RET KSIOAPI KSIOAPIServerStart(
		IN KS_HANDLE								Handle,
		IN KS_WSTR									OperatorName,
		IN KS_WSTR									OperatorPassWord);

	/// <summary>
	/// 	ֹͣIOServer����
	/// </summary>
	/// <param name="Handle">
	///		IOServer���Ӿ��
	/// </param>
	/// <param name="OperatorName">
	///		����Ա����
	/// </param>
	/// <param name="OperatorPassWord">
	///		����Ա����
	/// </param>
	/// <returns>
	///		����ɹ�����KS_OK,���ʧ�ܷ�����Ӧ������
	/// </returns>
	/// <remarks>
	///		����Ѿ�ֹͣҲ����KS_OK
	/// </remarks>
	KS_RET KSIOAPI KSIOAPIServerStop(
		IN KS_HANDLE								Handle,
		IN KS_WSTR									OperatorName,
		IN KS_WSTR									OperatorPassWord);

	/// <summary>
	/// 	��ȡIOServer����״̬
	/// </summary>
	/// <param name="Handle">
	///		IOServer���Ӿ��
	/// </param>
	/// <param name="Status">
	///		IOServer����״̬�����嶨��ο�KSIO_WORK_STATUS
	/// </param>
	/// <returns>
	///		����ɹ�����KS_OK,���ʧ�ܷ�����Ӧ������
	/// </returns>
	/// <remarks>
	///   
	/// </remarks>
	KS_RET KSIOAPI KSIOAPIGetWorkStatus(
		IN KS_HANDLE								Handle, 
		OUT KS_UINT16&								Status);

	/// <summary>
	/// 	��ȡIOServer����״̬
	/// </summary>
	/// <param name="Handle">
	///		IOServer���Ӿ��
	/// </param>
	/// <param name="Status">
	///		IOServer����״̬�����嶨��ο�KSIO_RUN_STATUS
	/// </param>
	/// <returns>
	///		����ɹ�����KS_OK,���ʧ�ܷ�����Ӧ������
	/// </returns>
	/// <remarks>
	///     
	/// </remarks>
	KS_RET KSIOAPI KSIOAPIGetRunningStatus(
		IN KS_HANDLE								Handle,
		OUT KS_UINT16&								Status);

	//==============================================================================
	//	3��IOServerͬ����д�ӿ�
	//==============================================================================

	/// <summary>
	/// 	ͬ��������
	/// </summary>
	/// <param name="Handle">
	///		IOServer���Ӿ��
	/// </param>
	/// <param name="DataSource">
	///		�ɼ�������Դ����KSIO_DATASOUCE
	/// </param>
	/// <param name="TagAccessIDArray">
	///		�ɼ���������ID����
	/// </param>
	/// <param name="TagAccessFieldIDArray">
	///		�ɼ�����������ID����
	/// </param>
	/// <param name="TagValueArray">
	/// 	����ֵ����
	/// </param>
	/// <param name="ErrorCodeArray">
	///		����������
	/// </param>
	/// <param name="AcquairTime">
	///		��ȡ����ʱ�䣬
	///		DataSource ΪCHCHEʱ��AcquairTime =  ˫�����紫��ʱ��
	///		DataSource ΪDEVICEʱ��AcquairTime =  �ɼ�ʱ��+˫�����紫��ʱ��
	/// </param>
	/// <returns>
	///		����ɹ�����KS_OK,���ʧ�ܷ�����Ӧ������
	/// </returns>
	KS_RET KSIOAPI KSIOAPISyncReadTags(
		IN KS_HANDLE								Handle,
		IN KS_UINT16								DataSource,
		IN PKSIO_LONGARRAY							TagAccessIDArray,
		IN PKSIO_LONGARRAY							TagAccessFieldIDArray,
		OUT PKSIO_TAG_VALUE_ARRAY					TagValueArray,
		OUT PKSIO_LONGARRAY							ErrorCodeArray,
		OUT PKSIO_TIMESTAMP							AcquairTime);

	/// <summary>
	/// 	ͬ��д������ָд���������豸
	/// </summary>
	/// <param name="Handle">
	///		IOServer���Ӿ��
	/// </param>
	/// <param name="bReadBack">
	/// 	�Ƿ�ض�
	/// </param>
	/// <param name="TagValueArray">
	/// 	����ֵ����
	/// </param>
	/// <param name="ErrorCodeArray">
	///		����������
	/// </param>
	/// <param name="TransferTime">
	///		д�����ʱ��
	///		��bReadBack ΪTRUEʱ��TransferTime =д��ʱ��+�ض�ʱ��+˫������ʱ�䡣
	///		��bReadBack ΪFALSEʱ��TransferTime =д��ʱ��+˫������ʱ�䡣
	/// </param>
	/// <returns>
	///		����ɹ�����KS_OK,���ʧ�ܷ�����Ӧ������
	/// </returns>
	KS_RET KSIOAPI KSIOAPISyncWriteTags(
		IN KS_HANDLE								Handle,
		IN BOOL										bReadBack,
		IN PKSIO_TAG_VALUE_ARRAY					TagValueArray,
		OUT PKSIO_LONGARRAY							ErrorCodeArray,
		OUT PKSIO_TIMESTAMP							TransferTime);

	//==============================================================================
	//	4��IOServer�����첽��д�ӿ�
	//==============================================================================

	/// <summary>
	/// 	�첽������
	/// </summary>
	/// <param name="Handle">
	///		IOServer���Ӿ��
	/// </param>
	/// <param name="DataSource">
	///		�ɼ�������Դ����KSIO_DATASOUCE
	/// </param>
	/// <param name="TagAccessIDArray">
	///		�ɼ���������ID����
	/// </param>
	/// <param name="TagAccessFieldIDArray">
	///		�ɼ�����������ID����
	/// </param>
	/// <param name="ErrorCodeArray">
	///		����������
	/// </param>
	/// <returns>
	///		����ɹ�����KS_OK,���ʧ�ܷ�����Ӧ������
	/// </returns>
	/// <remarks>
	///    
	/// </remarks>
	KS_RET KSIOAPI KSIOAPIAsyncReadTags(
		IN KS_HANDLE								Handle,
		IN KS_UINT16								DataSource,
		IN PKSIO_LONGARRAY							TagAccessIDArray,
		IN PKSIO_LONGARRAY							TagAccessFieldIDArray,
		OUT PKSIO_LONGARRAY							ErrorCodeArray);

	/// <summary>
	/// 	�첽д����
	/// </summary>
	/// <param name="Handle">
	///		IOServer���Ӿ��
	/// </param>
	/// <param name="TagValueArray">
	///		����ֵ����
	/// </param>
	/// <param name="ErrorCodeArray">
	///		����������
	/// </param>
	/// <returns>
	///		����ɹ�����KS_OK,���ʧ�ܷ�����Ӧ������
	/// </returns>
	/// <remarks>
	///     
	/// </remarks>
	KS_RET KSIOAPI KSIOAPIAsyncWriteTags(
		IN KS_HANDLE								Handle,
		IN PKSIO_TAG_VALUE_ARRAY					TagValueArray,
		OUT PKSIO_LONGARRAY							ErrorCodeArray);

	//==============================================================================
	//	5��IOServer���ı���ֵ�仯�ӿ�
	//==============================================================================

	/// <summary>
	/// 	���Ĳɼ�����ֵ�仯
	/// </summary>
	/// <param name="Handle">
	///		IOServer���Ӿ��
	/// </param>
	/// <param name="TagAccessIDArray">
	///		�ɼ���������ID����
	/// </param>
	/// <param name="TagAccessFieldIDArray">
	///		�ɼ�����������ID����
	/// </param>
	/// <param name="ErrorCodeArray">
	///		����������
	/// </param>
	/// <returns>
	///		����ɹ�����KS_OK,���ʧ�ܷ�����Ӧ������
	/// </returns>
	/// <remarks>
	///     
	/// </remarks>
	KS_RET KSIOAPI KSIOAPISubscribeTagValuesChange(
		IN KS_HANDLE								Handle,
		IN PKSIO_LONGARRAY							TagAccessIDArray,
		IN PKSIO_LONGARRAY							TagAccessFieldIDArray,
		OUT PKSIO_LONGARRAY							ErrorCodeArray);

	/// <summary>
	/// 	ȡ�����Ĳɼ�����ֵ�仯
	/// </summary>
	/// <param name="Handle">
	///		IOServer���Ӿ��
	/// </param>
	/// <param name="TagAccessIDArray">
	///		�ɼ���������ID����
	/// </param>
	/// <param name="TagAccessFieldIDArray">
	///		�ɼ�����������ID����
	/// </param>
	/// <param name="ErrorCodeArray">
	///		����������
	/// </param>
	/// <returns>
	///		����ɹ�����KS_OK,���ʧ�ܷ�����Ӧ������
	/// </returns>
	/// <remarks>
	///     
	/// </remarks>
	KS_RET KSIOAPI KSIOAPIUnsubscribeTagValuesChange(
		IN KS_HANDLE								Handle,
		IN PKSIO_LONGARRAY							TagAccessIDArray,
		IN PKSIO_LONGARRAY							TagAccessFieldIDArray,
		OUT PKSIO_LONGARRAY							ErrorCodeArray);

	//==============================================================================
	//	7�����ڲɼ��豸������״̬����˵��
	//		3.5��IOServer�жԴ���ͨ��ͨ�����豸�Ŀ���ϵͳ������״̬ϵͳ����ʵ�ֵ�
	//		���û����ֱ�ӿ���ͨ�����豸�ķ����ͻ�ȡͨ�����豸״̬�ķ���
	//==============================================================================

	//==============================================================================
	//	8��IOServer�Ļص�����ע��ӿ�
	//==============================================================================

	/// <summary>
	/// 	ע�������������״̬�仯�ص�
	/// </summary>
	/// <param name="Handle">
	///		IOServer���Ӿ��
	/// </param>
	/// <param name="pCallbackFunc">
	///		�ص�����ָ�룬�����ָ��ΪNULL,����ȡ��ע��
	/// </param>
	/// <returns>
	///		����ɹ�����KS_OK,���ʧ�ܷ�����Ӧ������
	/// </returns>
	/// <remarks>
	///     
	/// </remarks>
	KS_RET KSIOAPI KSIOAPIRegisterConnectStatusChangedCallbackFunc(
		IN KS_HANDLE								Handle,
		IN KSIO_CONNECT_STATUS_CHANGED_CALLBACKFUNC	pCallbackFunc);

	/// <summary>
	/// 	ע��IOServer����״̬�仯�ص�
	/// </summary>
	/// <param name="Handle">
	///		IOServer���Ӿ��
	/// </param>
	/// <param name="pCallbackFunc">
	///		�ص�����ָ�룬�����ָ��ΪNULL,����ȡ��ע��
	/// </param>
	/// <returns>
	///		����ɹ�����KS_OK,���ʧ�ܷ�����Ӧ������
	/// </returns>
	/// <remarks>
	///     
	/// </remarks>
	KS_RET KSIOAPI KSIOAPIRegisterWorkStatusChangedCallbackFunc(
		IN KS_HANDLE								Handle,
		IN KSIO_WORK_STATUS_CHANGED_CALLBACKFUNC	pCallbackFunc);

	/// <summary>
	/// 	ע��IOServer����״̬�仯�ص�
	/// </summary>
	/// <param name="Handle">
	///		IOServer���Ӿ��
	/// </param>
	/// <param name="pCallbackFunc">
	///		�ص�����ָ�룬�����ָ��ΪNULL,����ȡ��ע��
	/// </param>
	/// <returns>
	///		����ɹ�����KS_OK,���ʧ�ܷ�����Ӧ������
	/// </returns>
	/// <remarks>
	///     
	/// </remarks>
	KS_RET KSIOAPI KSIOAPIRegisterRunningStatusChangedCallbackFunc(
		IN KS_HANDLE								Handle,
		IN KSIO_RUNNING_STATUS_CHANGED_CALLBACKFUNC	pCallbackFunc);

	/// <summary>
	/// 	ע������ɼ����ݻص�����
	/// </summary>
	/// <param name="Handle">
	///		IOServer���Ӿ��
	/// </param>
	/// <param name="pCallbackFunc">
	///		�ص�����ָ�룬�����ָ��ΪNULL,����ȡ��ע��
	/// </param>
	/// <returns>
	///		����ɹ�����KS_OK,���ʧ�ܷ�����Ӧ������
	/// </returns>
	/// <remarks>
	///     
	/// </remarks>
	KS_RET KSIOAPI KSIOAPIRegisterCollectValueCallbackFunc(
		IN KS_HANDLE								Handle,
		IN KSIO_COLLECT_VALUE_CALLBACKFUNC			pCallbackFunc);

	/// <summary>
	/// 	ע���첽д������ɻص�����
	/// </summary>
	/// <param name="Handle">
	///		IOServer���Ӿ��
	/// </param>
	/// <param name="pCallbackFunc">
	///		�ص�����ָ�룬�����ָ��ΪNULL,����ȡ��ע��
	/// </param>
	/// <returns>
	///		����ɹ�����KS_OK,���ʧ�ܷ�����Ӧ������
	/// </returns>
	/// <remarks>
	///     
	/// </remarks>
	KS_RET KSIOAPI KSIOAPIRegisterWriteCompleteCallbackFunc(
		IN KS_HANDLE								Handle,
		IN KSIO_WRITE_COMPLTED_CALLBACKFUNC			pCallbackFunc);

	/// <summary>
	/// 	ע���첽��������ɻص�����
	/// </summary>
	/// <param name="Handle">
	///		IOServer���Ӿ��
	/// </param>
	/// <param name="pCallbackFunc">
	///		�ص�����ָ�룬�����ָ��ΪNULL,����ȡ��ע��
	/// </param>
	/// <returns>
	///		����ɹ�����KS_OK,���ʧ�ܷ�����Ӧ������
	/// </returns>
	/// <remarks>
	///     
	/// </remarks>
	KS_RET KSIOAPI KSIOAPIRegisterReadCompleteCallbackFunc(
		IN KS_HANDLE								Handle,
		IN KSIO_READ_COMPLTED_CALLBACKFUNC			pCallbackFunc);

	/// <summary>
	/// 	ע��ɼ�ģ��Ԫ�ز����ص�����
	/// </summary>
	/// <param name="Handle">
	///		IOServer���Ӿ��
	/// </param>
	/// <param name="pCallbackFunc">
	///		�ص�����ָ�룬�����ָ��ΪNULL,����ȡ��ע��
	/// </param>
	/// <returns>
	///		����ɹ�����KS_OK,���ʧ�ܷ�����Ӧ������
	/// </returns>
	/// <remarks>
	///     
	/// </remarks>
	KS_RET KSIOAPI KSIOAPIRegisterCollectElementOperateChangeCallbackFunc(
		IN KS_HANDLE										Handle,
		IN KSIO_COLLECTELEMENT_OPERTATE_CHANGED_CALLBACKFUNC	pCallbackFunc);

	/// <summary>
	/// 	ע���豸״̬�仯�ص�����
	/// </summary>
	/// <param name="Handle">
	///		IOServer���Ӿ��
	/// </param>
	/// <param name="pCallbackFunc">
	///		�ص�����ָ�룬�����ָ��ΪNULL,����ȡ��ע��
	/// </param>
	/// <returns>
	///		����ɹ�����KS_OK,���ʧ�ܷ�����Ӧ������
	/// </returns>
	/// <remarks>
	///     
	/// </remarks>
	KS_RET KSIOAPI KSIOAPIRegisterDeviceStatusChangeCallbackFunc(
		IN KS_HANDLE								Handle,
		IN KSIO_DEVICE_STATUS_CHANGE_CALLBACKFUNC	pCallbackFunc);

	//==============================================================================
	//
	//	ʮ��IOServer�������ܽӿں���
	//
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
	///		��һ��ִ��ʱ��IOServer�˳�ʼ���ڲ���Ա������KS_FAIL���ڶ���ִ�вŷ�����ȷֵ
	/// </remarks>
	KS_RET KSIOAPI KSIOAPIGetCurrentCPUConsume(
		IN KS_HANDLE							Handle,
		OUT KS_UINT32&							CurCPUUse,
		OUT KS_UINT16&							CurCPUPercentUse,
		OUT KS_UINT32&							Reserved1,
		OUT KS_UINT32&							Reserved2);

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
	KS_RET KSIOAPI KSIOAPIGetCurrentMemComsume(
		IN KS_HANDLE							Handle,
		OUT KS_UINT32&							CurMemUse,
		OUT KS_UINT16&							CurMemPercentUse,
		OUT KS_UINT32&							Reserved1,
		OUT KS_UINT32&							Reserved2) ;


	//���ӱȽ����õ�������ú��� duyisu

	// ����
	KS_RET KSIOAPI KSIOAPIBrowserProjects(
		IN KS_HANDLE								Handle, 
		IN KS_CWSTR									Mask, 
		OUT PSIO_SERVER_PROPERTY_ARRAY				ProjectPropertyArray);


    //ͨ��
	KS_RET KSIOAPI KSIOAPIBrowserChannels(
		IN KS_HANDLE								Handle, 
		IN KS_CWSTR									Mask, 
		OUT PKSIO_CHANNEL_PROPERTY_ARRAY				ChannelPropertyArray);

   //�豸
	KS_RET KSIOAPI KSIOAPIBrowserDevices(
		IN KS_HANDLE								Handle, 
		IN KS_CWSTR									Mask, 
		OUT PKSIO_DEVICE_PROPERTY_ARRAY				DevicePropertyArray);

	//����
	KS_RET KSIOAPI KSIOAPIBrowseCollectTags(
		IN KS_HANDLE								Handle, 
		IN KS_CWSTR									TagNameMask, 
		OUT PKSIO_TAG_PROPERTY_ARRAY					TagPropertyArray);

	//end
	//==============================================================================
#ifdef  __cplusplus
}
#endif
//==============================================================================
#pragma pack( pop , KSIOSERVERAPI )
//==============================================================================

#endif #define __KingIOServerAPI_H_INCLUDED__

