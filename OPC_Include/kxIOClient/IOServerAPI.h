//==============================================================================
/// <summary>
///		IOServer3.5版对外接口，供KS、KH、MES、监视器、IOMaster、第三方程序统一使用
/// </summary>
///	<file>
///		项目名:	组态王7.0
///		文件名: IOServerAPI.h
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
///			V0.9 : 段长明 2010-02-22	在3.0版API基础上修改
///			V1.0 : 段长明 2010-10-26	添加变量访问ID，并依据变量访问ID访问变量数据
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
	//	一、相关声明、定义
	//
	//==============================================================================

	//==============================================================================
	// 1、宏定义
	//==============================================================================
#define KSIOAPI				_stdcall

#define KSWSTR(str)		    L##str

#define KSOK(err)		    ( ((KS_INT32)(err)) ==  0 )
#define KSERR(err)		    ( ((KS_INT32)(err)) !=  0 )

#define IN 
#define OUT
#define IN_OUT

	//==============================================================================
	// 2、数据类型定义
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
	typedef long						KS_RET;						/// API函数调用返回码
	typedef KS_PTR						KS_HANDLE;					/// 连接句柄

	//==============================================================================
	//	3、错误码定义
	//==============================================================================

#define		KS_OK						0							/// 没有错误
#define		KS_FAIL						-1							/// 一般性错误
#define		KS_TIMEOUT					-2							/// 超时错误
#define		KS_OUT_OF_MEMORY			-3 							/// 内存错误
#define		KS_NOT_CONNECTED			-4 							/// 没有或无法建立连接
#define		KS_ACCESS_DENIED			-5 							/// 访问被拒绝
#define		KS_NOT_SUPPORTED			-6 							/// 不支持的属性或方法
#define		KS_INVALID_USER				-7 							/// 无效用户或密码
#define		KS_INVALID_TAGNAME			-8 							/// 无效变量名
#define		KS_INVALID_ARG				-9 							/// 无效参数
#define		KS_NO_DATA					-10 						/// 数据不存在
#define		KS_NETWORK_ERROR			-11							/// 网络错误
#define		KS_SYSTEM_ERROR				-12							/// 系统错误
#define		KS_NOT_FOUND 				-13 						/// 无法找到对象
#define		KS_SERVICE_NOT_RUNNING		-14 						/// 服务不在运行状态
#define		KS_BUFFER_TOO_SMALL			-15							/// 缓冲区太小
#define		KS_NO_MORE_DATA 			-16							/// 没有更多的数据
#define		KS_DEADLOCK					-17 						/// 发生了死锁
#define		KS_ALREADY_EXIST 			-18							/// 对象已经存在
#define		KS_DUPLICATE	 			-19							/// 发现重复项
#define		KS_OVERFLOW		 			-20							/// 发生了溢出
#define		KS_ABORTED		 			-21							/// 已经退出
#define		KS_LOAD_RESOURCE_FAIL		-22							/// 装载资源失败
#define		KS_SERVICE_STATUS_ERROR		-23							/// 服务状态错误
#define		KS_INITIAL_COM_ERROR		-24							/// 初始化COM库失败
#define		KS_NOT_INITIAL				-25							/// 未初始化


	//==============================================================================
	//
	//	二、相关枚举类型
	//
	//==============================================================================

	//==============================================================================
	//	1、通用类型
	//==============================================================================

	/// <summary>
	///		IOServer类型
	/// </summary>
	typedef enum KSIOServerType
	{
		KSIO_SERVER_TYPE_IO			= 0,		//IO的IOServer
		KSIO_SERVER_TYPE_OPC		= 1,		//OPC的IOServer
		KSIO_SERVER_TYPE_DDE		= 2,		//DDE的IOServer
		KSIO_SERVER_TYPE_NetKV		= 3,		//网络站点的IOServer
	} KSIO_SERVER_TYPE;


	/// <summary>
	///		IOClient类型
	/// </summary>
	typedef enum KSIOClientType
	{
		KSIO_CLIENT_TYPE_TOUCHVIEW		= 0,	//组态王运行系统,工业库也用这个
		KSIO_CLIENT_TYPE_TOUCHMAKE		= 1,	//组态王开发系统
		KSIO_CLIENT_TYPE_ACCESS_CLIENT	= 4,	//其他访问客户端
		KSIO_CLIENT_TYPE_MONITOR_CLIENT	= 5,	//监视客户端
		KSIO_CLIENT_TYPE_LOCAL_TEST		= 6,	//本地测试客户端
		KSIO_CLIENT_TYPE_REDUNIOSERVER	= 7,	//冗余IOServer
	} KSIO_CLIENT_TYPE;

	/// <summary>
	///		工作状态
	/// </summary>
	typedef enum KSIONodeType
	{
			KSIO_NODE_SINGLE		= 0,	//单一节点
			KSIO_NODE_REDUN_MASTER	= 1,	//冗余的主节点
			KSIO_NODE_REDUN_SLAVE	= 2,	//冗余的从节点
	} KSIO_NODE_TYPE;

	/// <summary>
	///		连接状态
	/// </summary>
	typedef enum KSIOConnectStatus
	{
		KSIO_STATUS_PRIMARY_CONNECT				= 0,		//单节点或主节点连接
		KSIO_STATUS_PRIMARY_DISCONNECT			= 1,		//单节点或主节点断开连接
		KSIO_STATUS_SECONDARY_CONNECT			= 2,		//从节点连接
		KSIO_STATUS_SECONDARY_DISCONNECT		= 3,		//从节点断开连接
	} KSIO_CONNECT_STATUS;

	/// <summary>
	///		工作状态
	/// </summary>
	typedef enum KSIOWorkStatus
	{
		KSIO_STATUS_START				= 0,		//启动
		KSIO_STATUS_STOP				= 1,		//停止
	} KSIO_WORK_STATUS;

	/// <summary>
	///		运行状态
	/// </summary>
	typedef enum KSIORunningStatus
	{
		KSIO_RUN_STATUS_NORMAL			= 0x0000,	//正常状态
		KSIO_RUN_STATUS_NOT_INIT		= 0x0001,	//初始化失败
		KSIO_RUN_STATUS_FAULT			= 0x0002,   //故障
		KSIO_RUN_STATUS_BUSY			= 0x0004,	//超载运行
	} KSIO_RUN_STATUS;

	/// <summary>
	///		用户访问权限
	/// </summary>
	typedef enum KSIOUserPrivileges
	{
		KSIO_USER_PRIVILEGE_LEVEL0		= 0x0000,	//访问权限0
		KSIO_USER_PRIVILEGE_LEVEL1		= 0x0001,	//访问权限1
		KSIO_USER_PRIVILEGE_LEVEL2		= 0x0002,	//访问权限2
		KSIO_USER_PRIVILEGE_LEVEL3		= 0x0004,	//访问权限3
		KSIO_USER_PRIVILEGE_LEVEL4		= 0x0010,	//访问权限4
		KSIO_USER_PRIVILEGE_LEVEL5		= 0x0020,	//访问权限5
		KSIO_USER_PRIVILEGE_LEVEL_ALL	= 0xffff,	//所有权限
	} KSIO_USER_PRIVILEGES;

	/// <summary>
	///		读写操作模式
	/// </summary>
	typedef enum KSIOAccessMode
	{
		KSIO_ACCESS_READ_ONLY			= 0,		//只读
		KSIO_ACCESS_WRITE_ONLY			= 1,		//只写
		KSIO_ACCESS_READ_WRITE			= 2,		//读写,预留用	
	} KSIO_ACCESS_MODE;

	/// <summary>
	///		采集数据来源
	/// </summary>
	typedef enum KSIODataSource
	{
		KSIO_DS_CACHE					= 0,		//从CACHE来源数据
		KSIO_DS_DEVICE					= 1,		//从设备来源数据
	} KSIO_DATASOUCE;


		/// <summary>
	/// 	采集模块元素类型
	/// </summary>
	typedef enum KSIOItemType
	{		
		KSIO_COLLECT_PROJECT     = 1,
		KSIO_COLLECT_CHANNEL		,		// 链路
		KSIO_COLLECT_DEVICE			,		// 设备
		KSIO_COLLECT_GROUP			,		// 组
		KSIO_COLLECT_TAG			,	// 变量
		KSIO_COLLECT_PROJECT_ISSTART   //是否启动
	}KSIO_ITEM_TYPE;


	/// <summary>
	/// 	采集模块元素状态
	/// </summary>
	typedef enum KSIOItemState
	{
		KSIO_ITEM_STATE_NONE=0x0000,     ///初始
		KSIO_ITEM_STATE_DELETED,         ///删除
		KSIO_ITEM_STATE_STOP,            ///停止
		KSIO_ITEM_STATE_FAULT,           ///故障
		KSIO_ITEM_STATE_SUSPEND,         ///挂起
		KSIO_ITEM_STATE_BUSY,            ///忙碌
		KSIO_ITEM_STATE_RUN,             ///运行
		KSIO_ITEM_STATE_ISSTART,          //点击启动按钮了
		KSIO_ITEM_STATE_ISSTOP            //点击停止启动按钮了
	}KSIO_ITEM_STATE;
	/// <summary>
	///		变量数据类型
	/// </summary>
	typedef enum KSIOTagDataType
	{
		KSIO_TAG_DATATYPE_BOOL		=  1,		//离散
		KSIO_TAG_DATATYPE_CHAR		=  2,		//有符号8位整数
		KSIO_TAG_DATATYPE_BYTE      =  3,		//无符号8位整数
		KSIO_TAG_DATATYPE_SHORT		=  4,		//有符号16位整数
		KSIO_TAG_DATATYPE_WORD		=  5,		//无符号16位整数
		KSIO_TAG_DATATYPE_LONG		=  6,		//有符号32位整数
		KSIO_TAG_DATATYPE_DWORD		=  7,		//无符号32位整数
		KSIO_TAG_DATATYPE_INT64		=  8,		//有符号64位整数
		KSIO_TAG_DATATYPE_FLOAT		=  9,		//单精度浮点
		KSIO_TAG_DATATYPE_DOUBLE 	=  10,		//双精度浮点
		KSIO_TAG_DATATYPE_STRING	=  11,		//字符串
	} KSIO_TAG_DATA_TYPE;

	/// <summary> 
	/// 数据质量
	/// </summary> 
	typedef enum KSIODataQuality
	{
		// QQSSSSLL ( OPC 3.0:低八位由质量Q、子状态S和限制状态L构成 )
		KDAQ_OPC_QUALITY_MASK				= 0xC0, 		/// Quality BITMASK
		KDAQ_OPC_STATUS_MASK				= 0xFC, 		/// Quality & Substatus mask
		KDAQ_OPC_LIMIT_MASK					= 0x03, 		/// Limit BITMask

		// Quality( 低字节的高两位：第6位和第7位 BITMASK )
		KDAQ_OPC_BAD						= 0x00, 		/// 坏数据
		KDAQ_OPC_UNCERTAIN					= 0x40, 		/// 不可靠数据
		KDAQ_OPC_NA							= 0x80, 		/// N/A
		KDAQ_OPC_GOOD						= 0xC0, 		/// 好的数据

		// Substatus( 低字节的中间四位：第2位－第5位 )
		// Substatus for BAD Quality
		KDAQ_OPC_CONFIG_ERROR				= 0x04, 		/// 配置错误
		KDAQ_OPC_NOT_CONNECTED				= 0x08, 		/// 没有连接设备
		KDAQ_OPC_DEVICE_FAILURE				= 0x0c, 		/// 设备失败
		KDAQ_OPC_SENSOR_FAILURE				= 0x10, 		/// 传感器失败( limit域能够提供附加信息 )
		KDAQ_OPC_LAST_KNOWN					= 0x14, 		/// 上一次采集的值( 通讯失败 )
		KDAQ_OPC_COMM_FAILURE				= 0x18, 		/// 通讯失败( 且无上一次采集值可用 )
		KDAQ_OPC_OUT_OF_SERVICE				= 0x1C, 		/// 设备停机
		KDAQ_OPC_WAITING_FOR_INITIAL_DATA	= 0x20, 		/// 尚未取得设备数据

		// Substatus for UNCERTAIN Quality
		KDAQ_OPC_LAST_USABLE				= 0x44, 		/// 上一个可用值
		KDAQ_OPC_SENSOR_CAL					= 0x50, 		/// 传感器值不精确
		KDAQ_OPC_EGU_EXCEEDED				= 0x54, 		/// 超量程
		KDAQ_OPC_SUB_NORMAL					= 0x58, 		/// 值从多个数据源得到，但缺少足够多的好数据

		// Substatus for GOOD Quality
		KDAQ_OPC_LOCAL_OVERRIDE				= 0xD8, 		/// 值被覆盖( GOOD )

		// Limit( 低字节的低两位：第0位、第1位 )
		KDAQ_OPC_LIMIT_OK					= 0x00, 		/// 上下限OK
		KDAQ_OPC_LIMIT_LOW					= 0x01, 		/// 下限
		KDAQ_OPC_LIMIT_HIGH					= 0x02, 		/// 上限
		KDAQ_OPC_LIMIT_CONST				= 0x03, 		/// 常量
	} KSIO_DATA_QUALITY; 


	//==============================================================================
	//	2、兼容KS3.0版本使用的类型
	//==============================================================================

	/// <summary>
	///		设备通讯状态
	/// </summary>
	typedef enum KSIODeviceCommStatus
	{
		KSIO_DEVICE_COMM_STATUS_OK				= 0,		//正常
		KSIO_DEVICE_COMM_STATUS_STOP			= 1,		//停止，一般为上面控制不进行采集
		KSIO_DEVICE_COMM_STATUS_INITIAL_FAIL	= 2,		//设备初始化失败
		KSIO_DEVICE_COMM_STATUS_FAULT			= 3,		//故障，包括通讯故障或者设备本身的故障
		KSIO_DEVICE_COMM_STATUS_SYS_STOP		= 4,		//系统停止，即没有处于运行状态
	} KSIO_DEV_COMM_STATUS;

	/// <summary>
	///		寄存器数据类型
	/// </summary>
	/// <remarks>
	///      若产品定义确定KS3.5中继续兼容变量IO属性操作，使用该类型
	/// </remarks>
	typedef enum KSIORegisterDataType
	{
		KSIO_REG_DATATYPE_OTHER			=	0x000,		//不指定数据类型
		KSIO_REG_DATATYPE_BIT			=	0x0001,		//1位；范围是：0或1
		KSIO_REG_DATATYPE_BYTE			=	0x0002,     //8位，1个字节；范围是：0---255
		KSIO_REG_DATATYPE_SHORT			=	0x0004,		//2个字节；范围是：-32768---32767
		KSIO_REG_DATATYPE_USHORT		=	0x0008,     //16位，2个字节；范围是：0---65535
		KSIO_REG_DATATYPE_BCD			=	0x0010,     //16位，2个字节；范围是：0---9999
		KSIO_REG_DATATYPE_LONG			=	0x0020,     //32位，4个字节；范围是：-2147483648——2147483647
		KSIO_REG_DATATYPE_LONGBCD		=	0x0040,     //32位，4个字节；范围是：0---4294967295
		KSIO_REG_DATATYPE_FLOAT			=	0x0080,     //32位，4个字节；范围是：10e-38---10e38，有效位7位
		KSIO_REG_DATATYPE_STRING		=	0x0100,		//128个字符长度
		KSIO_REG_DATATYPE_DOUBLE		=	0x0200,     //双精度浮点数
		KSIO_REG_DATATYPE_BLOB			=	0x0400,     //二进制流
		KSIO_REG_DATATYPE_INT64		=	0x0800,     //64位，8个字节 // add by pengfei.xing 2015/01/23 //zhanzhi,INT64
	} KSIO_REG_DATA_TYPE;



	
	/// <summary>
	///		通道属性数据结构
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
	/// 通道记录集
	/// </summary>
	typedef struct KSIOServerPropertyArray
	{
		KS_UINT32						Number;
		//通道属性数目
		PKSIO_SERVER_PROPERTY			ServerPropertiesArray;			//通道属性数组
	} KSIO_SERVER_PROPERTY_ARRAY, *PSIO_SERVER_PROPERTY_ARRAY;



	

	/// <summary>
	///		通道属性数据结构
	/// </summary>
	typedef struct KSIOChannelProperty
	{
		KS_UINT32				ChannelID;
		//采集通道ID
		KS_WSTR					ChannelName;
		//采集通道名称	
		CLSID					ClsID;
		//clsid
		KS_WSTR					ChannelDescription;
		//采集通道描述


		KS_WSTR					ChannelDriver;
		KS_UINT16				ChannelType;
		//通道类型，具体见kxChannelType


		KS_UINT32				Timeout;
		//链路超时ms
		//预留域
		//串口用        //以太网用			//OPC
		KS_UINT32				Reserved1;//波特率        //监听端口号
		KS_UINT32				Reserved2;//数据位
		KS_UINT32				Reserved3;//停止位
		KS_UINT32				Reserved4;//奇偶校验
		KS_UINT32				Reserved5;//流控
		KS_WSTR					Reserved6;//串口ID							//proid
		KS_WSTR					Reserved7;													//machine name opc
		KS_WSTR					Reserved8;
		KS_WSTR					Reserved9;
		KS_WSTR					Reserved10;
	}KSIO_CHANNEL_PROPERTY,*PKSIO_CHANNEL_PROPERTY;


	/// <summary>
	/// 通道记录集
	/// </summary>
	typedef struct KSIOChannelPropertyArray
	{
		KS_UINT32						Number;
		//通道属性数目
		PKSIO_CHANNEL_PROPERTY			ChannelPropertiesArray;			//通道属性数组
	} KSIO_CHANNEL_PROPERTY_ARRAY, *PKSIO_CHANNEL_PROPERTY_ARRAY;






	/// <summary>
	///		设备属性数据结构
	/// </summary>
	typedef struct KSIODeviceProperty
	{
		KS_UINT32				DeviceID;				//设备ID

		KS_WSTR					DeviceName;				//通讯卡名
		KS_WSTR					DeviceSeries;			//驱动系列
		KS_WSTR					DeviceDescription;		//设备描述
		KS_WSTR					DeviceAddrString;		//设备地址串


        KS_UINT32				ChannelID;				//采集通道ID




		KS_UINT32				RetryInterval;			//尝试间隔
		KS_UINT32				RetryIntervalUnit;		//尝试间隔单位，配置时使用
		KS_UINT32				MaxRetryTime;			//最长恢复时间
		KS_UINT32				MaxRetryTimeUnit;		//最长恢复时间单位，配置时使用
		KS_UINT16				RedundanceStyle;		//冗余类型，,无冗余，，作为主设备，，作为从设备
		KS_UINT32				RedunDeviceID;			//冗余主（从）设备ID
		KS_UINT32				RedunDeviceChannelID;	//冗余主（从）设备所属通道的ID
		KS_BOOL					MainDevicePriority;		//是否优先使用主设备

	}KSIO_DEVICE_PROPERTY,*PKSIO_DEVICE_PROPERTY;

	/// <summary>
	/// 设备记录集
	/// </summary>
	typedef struct KSIODevicePropertyArray
	{
		KS_UINT32						Number;							//设备属性数目
		PKSIO_DEVICE_PROPERTY			DevicePropertiesArray;			//设备属性数组
	} KSIO_DEVICE_PROPERTY_ARRAY, *PKSIO_DEVICE_PROPERTY_ARRAY;

	//==============================================================================
	//	3、供监视器使用的类型
	//==============================================================================

	/// <summary>
	///		通道类型
	/// </summary>
	typedef enum KSIOChannelType
	{
		KSIO_CHANNEL_TYPE_NULL				= 0x0000,		//无链路
		KSIO_CHANNEL_TYPE_COM				= 0x0001,		//串口
		KSIO_CHANNEL_TYPE_ETHERNET			= 0x0002,		//以太网
		KSIO_CHANNEL_TYPE_MODEM				= 0x0004,		//Modem
		KSIO_CHANNEL_TYPE_GPRS				= 0x0008,		//GPRS,即虚拟串口
		KSIO_CHANNEL_TYPE_USB				= 0x0010,		//USB
		KSIO_CHANNEL_TYPE_CONTROL_BUS		= 0x0020,		//ControlBus
		KSIO_CHANNEL_TYPE_BOARD_CARD		= 0x0040,		//BoardCard

	} KSIO_CHANNEL_TYPE;

	/// <summary>
	/// 	数据块队列排列方式
	/// </summary>
	typedef enum KSIODataBlockQueueMode
	{
		KSIO_BLOCKQUEUE_OVERLOAD		= 0,		// 通过计算过载系数确定采集队列
		KSIO_BLOCKQUEUE_GROUP			= 1,		// 按照采集周期分组
		KSIO_BLOCKQUEUE_USER			= 2,		// 用户手动配置
	} KSIO_DATABLOCK_QUEUE_MODE;



	/// <summary>
	/// 	采集模块元素操作类型
	/// </summary>
	enum KSIOCollectElementOperateType
	{
		KSIO_COLLECT_ELEMENT_ADD		= 0,		// 增
		KSIO_COLLECT_ELEMENT_REMOVE		= 1,		// 删
		KSIO_COLLECT_ELEMENT_MODIFY		= 2,		// 改
	};

	//==============================================================================
	//
	//	三、相关数据结构
	//
	//==============================================================================

	//==============================================================================
	//	1、连接配置相关结构
	//==============================================================================

	/// <summary>
	/// 	连接选项
	/// </summary>
	typedef struct KSIOConnectOption
	{
		KS_WSTR						UserName;						// 用户名
		KS_WSTR						Password;						// 密码

		//第一网络
		KS_WSTR						ServerNetIP1;					// 服务端网络1IP
		KS_UINT16					ServerNetPort1;					// 服务端网络1端口
		KS_WSTR						ClientNetIP1;					// 客户端网络1IP，目前未使用
		KS_UINT16					ClientNetPort1;					// 客户端网络1端口，目前未使用

		//第二网络
		KS_WSTR						ServerNetIP2;					// 服务端网络2IP
		KS_UINT16					ServerNetPort2;					// 服务端网络2端口，目前未使用
		KS_WSTR						ClientNetIP2;					// 客户端网络2IP，目前未使用
		KS_UINT16					ClientNetPort2;					// 客户端网络2端口，目前未使用

		//第三网络
		KS_WSTR						ServerNetIP3;					// 服务端网络3IP
		KS_UINT16					ServerNetPort3;					// 服务端网络3端口，目前未使用
		KS_WSTR						ClientNetIP3;					// 客户端网络3IP， 目前未使用
		KS_UINT16					ClientNetPort3;					// 客户端网络3端口， 目前未使用

		//第四网络
		KS_WSTR						ServerNetIP4;					// 服务端网络4IP
		KS_UINT16					ServerNetPort4;					// 服务端网络4端口，目前未使用
		KS_WSTR						ClientNetIP4;					// 客户端网络4IP，目前未使用
		KS_UINT16					ClientNetPort4;					// 客户端网络4端口，目前未使用

		//第五网络
		KS_WSTR						ServerNetIP5;					// 服务端网络5IP
		KS_UINT16					ServerNetPort5;					// 服务端网络5端口，目前未使用
		KS_WSTR						ClientNetIP5;					// 服务端网络5IP， 目前未使用
		KS_UINT16					ClientNetPort5;					// 服务端网络5端口，目前未使用


		KS_WSTR						ClientMachineName;				// Client机器名称
		KS_WSTR						ApplicationName;				// 应用程序名称
		KS_UINT16					ClientType; 					// Client 类型，见kxIOClientType

		KS_UINT16					TimeOut;						// 连接超时，单位ms

		KS_UINT16					HeartBeatDetectInterval;		//	心跳检测间隔，单位ms
		KS_UINT16					MaxAllowErrorsBeforeDisconnect;	//	关闭一个连接之前允许出现的最大连接错误数目

		KS_UINT16	    			Reserved1; 						// 保留的额外属性
		KS_UINT16 					Reserved2; 						// 保留的额外属性
		KS_WSTR						Reserved3; 						// 保留的额外属性
		KS_WSTR						Reserved4; 						// 保留的额外属性
	}KSIO_CONNECTION_OPTION , *PKSIO_CONNECTION_OPTION;

	//==============================================================================
	//	2、变量相关数据结构
	//==============================================================================

	/// <summary> 
	///		用户
	/// </summary> 
	typedef struct KSIOUser
	{
		KS_WSTR						Name;							// 用户名称
		KS_WSTR						Description;					// 用户描述
		KS_WSTR						Password;						// 用户密码
		WORD						Privileges;						// 用户权限,见KSIO_USER_PRIVILEGES
	} KSIO_USER, *PKSIO_USER;

	typedef struct KSIOUserArray
	{
		KS_UINT32					Number;							//用户信息个数
		PKSIO_USER					UserArray;						//用户信息数组
	} KSIO_USER_ARRAY, *PKSIO_USER_ARRAY;

	/// <summary> 
	///		数据值
	/// </summary> 
	typedef struct KSIOValue
	{
		KS_UINT16					DataType;						// 数据类型
		union
		{
			KS_BOOL					bitVal;							// 布尔类型
			KS_INT8					i1Val;							// 单字节整数
			KS_INT16				i2Val;							// 双字节整数
			KS_INT32				i4Val;							// 四字节整数
			KS_INT64				i8Val;							// 八字节整数
			KS_UINT8				ui1Val;							// 单字节整数(无符号)	
			KS_UINT16				ui2Val;							// 双字节整数(无符号)
			KS_UINT32				ui4Val;							// 四字节整数(无符号)
			KS_UINT64				ui8Val;							// 八字节整数(无符号)
			KS_FLOAT32				r4Val;							// 单精度浮点数
			KS_FLOAT64				r8Val;							// 双精度浮点数
			KS_WSTR					wstrVal;						// Unicode字符串
		};					
	} KSIO_VALUE , *PSIO_VALUE;

	/// <summary> 
	///		时间戳
	/// </summary> 
	typedef struct KSIOTimeStamp				
	{
		KS_UINT32					Seconds;						//自1970/01/01 00:00:00(UTC)以来的秒数
		KS_UINT16					Millisecs;						//毫秒数
	} KSIO_TIMESTAMP , *PKSIO_TIMESTAMP;


	/// <summary> 
	///		变量值
	/// </summary> 
	typedef struct KSIOTagValue
	{
		KS_UINT32					TagID;							//变量ID,该ID由IOServer产生
		KS_UINT32					TagFieldID;						//变量域ID,该ID由IOServer指定
		KSIO_VALUE					TagValue;						//变量值
		KSIO_TIMESTAMP				TimeStamp;						//时间戳
		KS_UINT32					QualityStamp;					//质量戳
	} KSIO_TAG_VALUE, *PKSIO_TAG_VALUE;

	/// <summary> 
	///		变量值数组
	/// </summary> 
	typedef struct KSIOTagValueArray
	{
		KS_UINT32                   Number;							//变量值个数
		PKSIO_TAG_VALUE				TagValueArray;					//变量值数组

	}KSIO_TAG_VALUE_ARRAY,*PKSIO_TAG_VALUE_ARRAY;



	/// <summary> 
	///		采集模块元素操作
	/// </summary> 
	typedef struct KSIOCollectElementOperate
	{
		KS_UINT16					CollectElementType;				//采集模块元素类型 KSIOCollectElementType
		KS_UINT16					CollectElementOperateType;		//采集模块元素操作类型 KSIOCollectElementOperateType
		KS_WSTR						CollectElementName;				//元素名
	}KSIO_COLLECTELMENT_OPERATE, *PKSIO_COLLECTELMENT_OPERATE;

	/// <summary> 
	///		采集模块元素操作数组
	/// </summary> 
	typedef struct KSIOCollectElementOperateArray
	{
		KS_UINT32                   Number;							//采集模块元素操作个数
		PKSIO_COLLECTELMENT_OPERATE	CollectElementOperateArray;		//采集模块元素操作数组

	}KSIO_COLLECTELMENT_OPERATE_ARRAY,*PKSIO_COLLECTELMENT_OPERATE_ARRAY;

	/// <summary> 
	/// 变量属性
	/// </summary> 
	typedef struct KSIOTagProperty
	{
		//==============================================================================
		//	常规属性
		//==============================================================================
		KS_UINT32						TagAccessID;					/// 变量访问ID
		KS_WSTR							TagName;						/// 变量名称 :  NAME
		KS_WSTR							TagFullName;					/// 变量名称 :  CHANNEL.DEVICE.GROUP.NAME
		KS_WSTR							Description;					/// 变量描述	

		//==============================================================================
		//	数据属性
		//==============================================================================
		KS_UINT16						TagDataType;					///变量数据类型，其值使用见KSIO_TAG_DATA_TYPE

		// 值域
		KSIO_VALUE 						MaxRawValue;					/// 变量原始最大值 			
		KSIO_VALUE 						MinRawValue;					/// 变量原始最小值
		KSIO_VALUE 						MaxValue;						/// 变量工程最大值 			
		KSIO_VALUE 						MinValue;						/// 变量工程最小值

		KS_UINT16						ConvertType;                    /// 变量变换类型 其值使用见 kxTagConvertType
		KS_BOOL					        IsFilter;				        /// 是否过滤数据
		KS_FLOAT32					    DeadBand;					    /// 死区百分比

		//==============================================================================
		//	关联属性
		//==============================================================================

		KS_WSTR							ChannelName;					// 通道名称
		KS_WSTR							DeviceName;						// 设备名称
		KS_WSTR							GroupName;						// 组名称

		//==============================================================================
		//	采集属性
		//==============================================================================

		KS_BOOL							CollectControl;				    /// 采集控制(是否采集标志)
		KS_UINT32 						CollectInterval;				/// 采集间隔(采样周期:单位ms)			
		KS_UINT32 						CollectOffset;				    /// 采集偏移(相位:单位ms)
		KS_INT32						TimeZoneBias;					/// 与UTC标准时间的差值(单位：分钟)
		KS_INT32						TimeAdjustment;					/// 时间校正(单位:毫秒)

		//==============================================================================
		//	关联属性,注：此为兼容3.0版本使用
		//==============================================================================
		KS_UINT32						ChannelID;						///采集通道ID
		KS_UINT32						DeviceID;						///采集设备ID
		KS_UINT32						DataBlockID;					///数据块ID

		//==============================================================================
		//	变量其他属性
		//==============================================================================
		KS_WSTR							ItemName;						///Item名称，Item是与变量相关联的

		//关于Item，
		//通用的IOServer是寄存器+偏移字符串名称
		//OPC的IOServer为OPC Item的名称
		//DDE的IOServer为DDE Item的名称
		KS_WSTR				        	RegName;
		KS_UINT16						RegType;
		KS_UINT16						ItemDataType;					///Item数据类型，其值使用见KSIO_REG_DATA_TYPE，DDE不起作用
		KS_UINT16						ItemAccessMode;					///读写访问模式,其值使用见KSIO_ACCESS_MODE，DDE不起作用

		//==============================================================================
		//	保留属性
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
	/// 变量记录集
	/// </summary> 
	typedef struct KSIOTagPropertyArray
	{
		KS_UINT32						Number;							/// 变量属性数目
		PKSIO_TAG_PROPERTY				TagPropertiesArray;				/// 变量属性数组
	} KSIO_TAG_PROPERTY_ARRAY, *PKSIO_TAG_PROPERTY_ARRAY;

	//==============================================================================
	//	3、设备相关数据结构，供兼容KS3.0版本使用
	//==============================================================================

	/// <summary> 
	///		设备状态
	/// </summary> 
	typedef struct KSIODeviceStatus
	{
		KS_WSTR						IOServerName;			//IOServer名称
		KS_UINT32					ChannelID;				//采集通道ID,由IOServer产生,在IOServer上可以查看
		KS_WSTR						ChannelName;			//采集通道名称
		KS_UINT32					DeviceID;				//采集设备ID,由IOServer产生,在IOServer上可以查看
		KS_WSTR						DeviceName;				//采集设备名称
		KS_UINT16					DeviceStatus;			//设备状态,见KSIO_DEV_COMM_STATUS

		//如下保留
		KS_UINT32					Reserved1;				//保留的额外属性
		KS_UINT32					Reserved2;				//保留的额外属性
		KS_WSTR						Reserved3;				//保留的额外属性
		KS_WSTR						Reserved4;				//保留的额外属性
	} KSIO_DEV_STATUS, *PKSIO_DEV_STATUS;


	/// <summary> 
	///		设备状态
	/// </summary> 
	typedef struct KSIODeviceStatusArray
	{
		KS_UINT32					Number;					//设备状态个数
		PKSIO_DEV_STATUS			DeviceStatusArray;		//设备状态数组
	} KSIO_DEV_STATUS_ARRAY, *PKSIO_DEV_STATUS_ARRAY;

	//<summary>
	//		寄存器属性
	//</summary>
	/// <remarks>
	///      若产品定义确定KS3.5中继续兼容变量IO属性操作，使用该结构
	/// </remarks>
	typedef struct KSIORegister 
	{
		KS_WSTR						RegisterName;			//寄存器名称
		KS_UINT32					LowOffset;				//低地址偏移
		KS_UINT32					UpperOffset;			//高地址偏移
		KS_UINT16					RegisterDataType;		//寄存器数据类型
		KS_UINT16					AccessMode;				//目前没用到
	} KSIO_REGISTER, *PKSIO_REGISTER;

	//<summary>
	//		寄存器属性数组
	//</summary>
	/// <remarks>
	///      若产品定义确定KS3.5中继续兼容变量IO属性操作，使用该结构
	/// </remarks>
	typedef struct KSIORegisterArray
	{
		KS_UINT16					Number;					//寄存器属性个数
		PKSIO_REGISTER				RegisterArray;			//寄存器属性数组			
	} KSIO_REGISTER_ARRAY, *PKSIO_REGISTER_ARRAY;


	//==============================================================================
	//	4、对有关监视器结构的说明
	//		3.5版本IOServer没有了数据块的概念、API中也没有了通道、设备等信息
	//		对于通道、设备信息是通过系统监视变量来实现的，因此不对3.0版本做兼容
	//==============================================================================


	//==============================================================================
	//	5、辅助数据结构
	//==============================================================================

	/// <summary>
	/// 	整型数组
	/// </summary>
	typedef struct KSIOLongArray
	{
		KS_UINT32					Number;
		KS_UINT32*					LongArray;

	}KSIO_LONGARRAY,*PKSIO_LONGARRAY;

	/// <summary>
	/// 	字符串数组
	/// </summary>
	typedef struct KSIOStringArray
	{
		KS_UINT32					Number;
		KS_WSTR*					StringArray;

	}KSIO_STRINGARRAY,*PKSIO_STRINGARRAY;


	//==============================================================================
	//
	//	四、回调函数定义  注：回调屏蔽了对主从冗余节点的包容处理
	//
	//==============================================================================

	/// <summary>
	///		与服务器连接回调处理函数
	/// </summary>
	/// <param name="Handle">
	/// 	连接句柄
	/// </param>
	/// <param name="ConnectStatus">
	///		连接状态
	/// </param>
	typedef KS_RET ( CALLBACK *KSIO_CONNECT_STATUS_CHANGED_CALLBACKFUNC )(
		IN KS_HANDLE								Handle, 
		IN KSIO_CONNECT_STATUS						ConnectStatus);

	/// <summary>
	/// 	IOServer工作状态变化回调接口
	/// </summary>
	/// <param name="Handle">
	/// 	连接句柄
	/// </param>
	/// <param name="NodeType">
	/// 	IOServer节点类型
	/// </param>
	/// <param name="WorkStatus">
	///		IOServer工作状态	
	/// </param>
	typedef KS_RET ( CALLBACK *KSIO_WORK_STATUS_CHANGED_CALLBACKFUNC )(
		IN KS_HANDLE								Handle, 
		IN KSIO_NODE_TYPE							NodeType, 
		IN KSIO_WORK_STATUS							WorkStatus);

	/// <summary>
	/// 	IOServer运行状态变化回调接口
	/// </summary>
	/// <param name="Handle">
	/// 	连接句柄
	/// </param>
	/// <param name="NodeType">
	/// 	IOServer节点类型
	/// </param>
	/// <param name="RunningStatus">
	///		IOServer运行状态
	/// </param>
	typedef KS_RET ( CALLBACK *KSIO_RUNNING_STATUS_CHANGED_CALLBACKFUNC )(
		IN KS_HANDLE								Handle, 
		IN KSIO_NODE_TYPE							NodeType, 
		IN KSIO_RUN_STATUS							RunningStatus);

	/// <summary> 
	/// 	采集数据回调接口
	/// </summary> 
	/// <param name="Handle">
	/// 	连接句柄
	/// </param>
	/// <param name="VaribleValueArray">
	///		变量值数组
	/// </param>
	typedef KS_RET ( CALLBACK *KSIO_COLLECT_VALUE_CALLBACKFUNC )(
		IN KS_HANDLE								Handle, 
		IN PKSIO_TAG_VALUE_ARRAY					TagValueArray);

	/// <summary>
	///		写变量完成回调接口
	/// </summary>
	/// <param name="Handle">
	/// 	连接句柄
	/// </param>
	/// <param name="TagIDArray">
	///		变量ID数组
	/// </param>
	/// <param name="TagAccessFieldIDArray">
	///		变量访问域ID数组
	/// </param>
	/// <param name="ErrorCodeArray">
	///		错误码数组
	/// </param>
	typedef KS_RET ( CALLBACK * KSIO_WRITE_COMPLTED_CALLBACKFUNC ) (
		IN KS_HANDLE							Handle,
		IN PKSIO_LONGARRAY						TagIDArray,
		IN PKSIO_LONGARRAY						TagAccessFieldIDArray,
		IN PKSIO_LONGARRAY						ErrorCodeArray);

	/// <summary>
	///		读变量完成回调接口
	/// </summary>
	/// <param name="Handle">
	/// 	连接句柄
	/// </param>
	/// <param name="TagValueArray">
	///		变量值数组
	/// </param>
	/// <param name="ErrorCodeArray">
	///		错误码数组
	/// </param>
	typedef KS_RET ( CALLBACK * KSIO_READ_COMPLTED_CALLBACKFUNC ) (
		IN KS_HANDLE							Handle,
		IN PKSIO_TAG_VALUE_ARRAY				TagValueArray,
		IN PKSIO_LONGARRAY						ErrorCodeArray);

	/// <summary>
	///		采集模块元素操作回调接口
	/// </summary>
	/// <param name="Handle">
	/// 	连接句柄
	/// </param>
	/// <param name="CollectElementOperateArray">
	///		采集模块元素操作数组
	/// </param>
	typedef KS_RET ( CALLBACK * KSIO_COLLECTELEMENT_OPERTATE_CHANGED_CALLBACKFUNC ) (
		IN KS_HANDLE							Handle,
		IN PKSIO_COLLECTELMENT_OPERATE_ARRAY	CollectElementOperateArray);

	/// <summary>
	///		设备状态变化回调接口
	/// </summary>
	/// <param name="Handle">
	/// 	连接句柄
	/// </param>
	/// <param name="DeviceStatusArray">
	///		设备状态数组
	/// </param>
	/// <remarks>
	///     供KS3.0版本使用
	/// </remarks>
	typedef KS_RET ( CALLBACK * KSIO_DEVICE_STATUS_CHANGE_CALLBACKFUNC) (
		IN KS_HANDLE							Handle,
		IN PKSIO_DEV_STATUS_ARRAY				DeviceStatusArray);

	//==============================================================================
	//
	//	五、辅助接口函数
	//
	//==============================================================================

	/// <summary>
	/// 	初始化IOServer接口
	/// </summary>
	/// <returns>
	///		如果成功返回KS_OK,如果失败返回相应错误码
	/// </returns>
	/// <remarks>
	///     用来初始化网络连接和其他资源
	/// </remarks>
	KS_RET KSIOAPI KSIOAPIInitalize(IN const KS_WSTR String);


	/// <summary>
	/// 	释放IOServer接口资源
	/// </summary>
	/// <returns>
	///		如果成功返回KS_OK,如果失败返回相应错误码
	/// </returns>
	/// <remarks>
	///     清理客户端占用的资源和网络连接所用的资源
	/// </remarks>
	KS_RET KSIOAPI KSIOAPIRelease();


	/// <summary>
	///    释放字符串资源
	/// </summary>
	/// <param name="String">
	///		待释放的字符串指针
	/// </param>
	/// <returns>
	///		如果成功返回KS_OK,如果失败返回相应错误码
	/// </returns>
	KS_RET KSIOAPI KSIOAPIFreeString(IN KS_WSTR& String);


	/// <summary>
	/// 	释放字符串数组资源
	/// </summary>
	/// <param name="StringArray">
	///		待释放的字符串数组
	/// </param>
	/// <returns>
	///		如果成功返回KS_OK,如果失败返回相应错误码
	/// </returns>
	KS_RET KSIOAPI KSIOAPIFreeStringArray(IN PKSIO_STRINGARRAY StringArray);

	/// <summary>
	/// 	释放整数数组资源
	/// </summary>
	/// <param name="LongArray">
	///		待释放的整数数组
	/// </param>
	/// <returns>
	///		如果成功返回KS_OK,如果失败返回相应错误码
	/// </returns>
	KS_RET KSIOAPI KSIOAPIFreeLongArray(IN PKSIO_LONGARRAY LongArray);

	/// <summary>
	/// 	释放变量记录集
	/// </summary>
	/// <param name="TagPropertiesArray">
	///		变量属性数组
	/// </param>
	/// <returns>
	///		如果成功返回KS_OK,如果失败返回相应错误码
	/// </returns>
	KS_RET KSIOAPI KSIOAPIFreeCollectTags(IN PKSIO_TAG_PROPERTY_ARRAY TagPropertiesArray);

	/// <summary>
	/// 	释放变量值数组资源
	/// </summary>
	/// <param name="TagValueArray">
	///		变量值数组资源
	/// </param>
	/// <returns>
	///		如果成功返回KS_OK,如果失败返回相应错误码
	/// </returns>
	KS_RET KSIOAPI KSIOAPIFreeTagsValue(IN PKSIO_TAG_VALUE_ARRAY TagValueArray);

	/// <summary>
	/// 	释放寄存器数组资源
	/// </summary>
	/// <param name="RegisterArray">
	///		寄存器数组资源
	/// </param>
	/// <returns>
	///		如果成功返回KS_OK,如果失败返回相应错误码
	/// </returns>
	KS_RET KSIOAPI KSIOAPIRegistersArray(IN PKSIO_REGISTER_ARRAY RegisterArray);



	
	/// <summary>
	/// 	释放工程记录集
	/// </summary>
	/// <param name="TagPropertiesArray">
	///		变量属性数组
	/// </param>
	/// <returns>
	///		如果成功返回KS_OK,如果失败返回相应错误码
	/// </returns>
	KS_RET KSIOAPI KSIOAPIFreeProjects(IN PSIO_SERVER_PROPERTY_ARRAY PropertiesArray);

	/// <summary>
	/// 	释放通道记录集
	/// </summary>
	/// <param name="TagPropertiesArray">
	///		变量属性数组
	/// </param>
	/// <returns>
	///		如果成功返回KS_OK,如果失败返回相应错误码
	/// </returns>
	KS_RET KSIOAPI KSIOAPIFreeChannels(IN PKSIO_CHANNEL_PROPERTY_ARRAY PropertiesArray);

	/// <summary>
	/// 	释放设备记录集
	/// </summary>
	/// <param name="TagPropertiesArray">
	///		变量属性数组
	/// </param>
	/// <returns>
	///		如果成功返回KS_OK,如果失败返回相应错误码
	/// </returns>
	KS_RET KSIOAPI KSIOAPIFreeDevices(IN PKSIO_DEVICE_PROPERTY_ARRAY PropertiesArray);
	/// <summary>
	/// 	释放变量记录集
	/// </summary>
	/// <param name="TagPropertiesArray">
	///		变量属性数组
	/// </param>
	/// <returns>
	///		如果成功返回KS_OK,如果失败返回相应错误码
	/// </returns>
	KS_RET KSIOAPI KSIOAPIFreeCollectTags(IN PKSIO_TAG_PROPERTY_ARRAY TagPropertiesArray);

	//==============================================================================
	//
	//	六、服务器连接与控制接口函数
	//
	//==============================================================================

	/// <summary>
	/// 	连接IOServer
	/// </summary>
	/// <param name="ConnectOption">
	///		连接选项
	/// </param>
	/// <param name="Handle">
	///		IOServer连接句柄
	/// </param>
	/// <returns>
	///		如果成功返回KS_OK,如果失败返回相应错误码
	/// </returns>
	/// <remarks>
	///     用于连接数据库，连接成功后函数返回一个有效数据库句柄，以后对于数据库的调用都必须使用该句柄
	/// </remarks>
	KS_RET KSIOAPI KSIOAPIServerConnect(
		IN PKSIO_CONNECTION_OPTION					ConnectOption ,
		OUT KS_HANDLE&								Handle);

	/// <summary>
	/// 	连接IOServer扩展方法
	/// </summary>
	/// <param name="PrimaryIOConnOption">
	///		互为冗余的Primary IOServer连接选项
	/// </param>
	/// <param name="SecondaryIOConnOption">
	///		互为冗余的Secondary IOServer连接选项
	/// </param>
	/// <param name="Handle">
	///		IOServer连接句柄
	/// </param>
	/// <returns>
	///		如果成功返回KS_OK,如果失败返回相应错误码
	/// </returns>
	/// <remarks>
	///     如果存在IOServer冗余，使用该方法
	/// </remarks>
	KS_RET KSIOAPI KSIOAPIServerConnectEx(
		IN PKSIO_CONNECTION_OPTION					PrimaryIOConnOption ,
		IN PKSIO_CONNECTION_OPTION					SecondaryIOConnOption ,
		OUT KS_HANDLE&								Handle);


	/// <summary>
	/// 	获取连接状态
	/// </summary>
	/// <param name="Handle">
	///		IOServer连接句柄
	/// </param>
	/// <param name="bConnect">
	///		连接状态，true:连接 false连接断开
	/// </param>
	/// <returns>
	///		如果成功返回KS_OK,如果失败返回相应错误码
	/// </returns>
	/// <remarks>
	///     
	/// </remarks>
	KS_RET KSIOAPI  KSIOAPIServerIsConnected(
		IN KS_HANDLE								Handle,
		OUT KS_BOOL&								bConnect);

	/// <summary>
	/// 	断开连接
	/// </summary>
	/// <param name="Handle">
	///		IOServer连接句柄
	/// </param>
	/// <returns>
	///		如果成功返回KS_OK,如果失败返回相应错误码
	/// </returns>
	/// <remarks>
	///     用于断开DBHandle所标识的连接
	/// </remarks>
	KS_RET KSIOAPI KSIOAPIServerDisconnect(IN_OUT KS_HANDLE& Handle);

	//==============================================================================
	//
	//	七、用户管理接口函数
	//	
	//==============================================================================

	/// <summary>
	/// 	增加用户
	/// </summary>
	/// <param name="Handle">
	///		IOServer连接句柄
	/// </param>
	/// <param name="OperatorName">
	///		操作员名称
	/// </param>
	/// <param name="OperatorPassWord">
	///		操作员密码
	/// </param>
	/// <param name="User">
	///		用户信息
	/// </param>
	/// <returns>
	///   如果成功返回KS_OK,如果失败返回相应错误码
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
	/// 	删除用户
	/// </summary>
	/// <param name="Handle">
	///		IOServer连接句柄
	/// </param>
	/// <param name="OperatorName">
	///		操作员名称
	/// </param>
	/// <param name="OperatorPassWord">
	///		操作员密码
	/// </param>
	/// <param name="Name">
	///		用户名称
	/// </param>
	/// <returns>
	///   如果成功返回KS_OK,如果失败返回相应错误码
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
	/// 	枚举系统用户信息
	/// </summary>
	/// <param name="Handle">
	///		IOServer连接句柄
	/// </param>
	/// <param name="OperatorName">
	///		操作员名称
	/// </param>
	/// <param name="OperatorPassWord">
	///		操作员密码
	/// </param>
	/// <param name="UserArray">
	///		用户信息数组
	/// </param>
	/// <returns>
	///   如果成功返回KS_OK,如果失败返回相应错误码
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
	/// 	通过用户名称获取用户信息
	/// </summary>
	/// <param name="Handle">
	///		IOServer连接句柄
	/// <param name="Name">
	///		用户名称
	/// </param>
	/// <param name="OperatorName">
	///		操作员名称
	/// </param>
	/// <param name="OperatorPassWord">
	///		操作员密码
	/// </param>
	/// </param>
	/// <param name="User">
	///		用户信息
	/// </param>
	/// <returns>
	///   如果成功返回KS_OK,如果失败返回相应错误码
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
	//	八、开发配置与验证接口函数，兼容KS3.0工程用
	//	
	//==============================================================================

	/// <summary>
	/// 	枚举所有通道的名称
	/// </summary>
	/// <param name="Handle">
	///		IOServer连接句柄
	/// </param>
	/// <param name="ChannelNameArray">
	///		通道名称数组
	/// </param>
	/// <returns>
	///   如果成功返回KS_OK,如果失败返回相应错误码
	/// </returns>
	/// <remarks>
	///     
	/// </remarks>
	KS_RET KSIOAPI KSIOAPIEnumAllChannelNames(
		IN KS_HANDLE								Handle,
		OUT PKSIO_STRINGARRAY						ChannelNameArray) ;


	/// <summary>
	/// 	根据通道名称获取通道ID
	/// </summary>
	/// <param name="Handle">
	///		IOServer连接句柄
	/// </param>
	/// <param name="ChannelNameArray">
	///		通道名称数组
	/// </param>
	/// <param name="ChannelIDArray">
	///		通道ID数组
	/// </param>
	/// <param name="ErrorCodeArray">
	///		错误码数组
	/// </param>
	/// <returns>
	///   如果成功返回KS_OK,如果失败返回相应错误码
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
	/// 	枚举通道下的所有设备名称
	/// </summary>
	/// <param name="Handle">
	///		IOServer连接句柄
	/// </param>
	/// <param name="ChannelName">
	///		通道名称
	/// </param>
	/// <param name="DeviceNameArray">
	///		设备名称数组，指设备逻辑名
	/// </param>
	/// <returns>
	///   如果成功返回KS_OK,如果失败返回相应错误码
	/// </returns>
	/// <remarks>
	///     
	/// </remarks>
	KS_RET KSIOAPI KSIOAPIEnumChannelAllDeviceNames(
		IN KS_HANDLE								Handle,
		IN KS_WSTR									ChannelName, 
		OUT PKSIO_STRINGARRAY						DeviceNameArray) ;


	/// <summary>
	/// 	根据设备名获取设备ID
	/// </summary>
	/// <param name="Handle">
	///		IOServer连接句柄
	/// </param>
	/// <param name="ChannelNameArray">
	///		通道名数组
	/// </param>
	/// <param name="DeviceNameArray">
	///		设备名称数组，指设备逻辑名
	/// </param>
	/// <param name="DeviceIDArray">
	///		设备ID数组
	/// </param>
	/// <param name="ErrorCodeArray">
	///		错误码数组
	/// </param>
	/// <returns>
	///   如果成功返回KS_OK,如果失败返回相应错误码
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
	/// 	枚举设备下的所有数据块
	/// </summary>
	/// <param name="Handle">
	///		IOServer连接句柄
	/// </param>
	/// <param name="ChannelName">
	///		通道名称
	/// </param>
	/// <param name="DeviceName">
	///		设备名称，指设备逻辑名
	/// </param>
	/// <param name="DataBlockNameArray">
	///		数据块名称数组
	/// </param>
	/// <returns>
	///   如果成功返回KS_OK,如果失败返回相应错误码
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
	/// 	根据名称获取数据块ID
	/// </summary>
	/// <param name="Handle">
	///		IOServer连接句柄
	/// </param>
	/// <param name="ChannelNameArray">
	///		通道名称数组
	/// </param>
	/// <param name="DeviceNameArray">
	///		设备名称数组，指设备逻辑名
	/// </param>
	/// <param name="DataBlockNameArray">
	///		数据块名称数组
	/// </param>
	/// <param name="DataBlockIDArray">
	///		数据块ID数组
	/// </param>
	/// <param name="ErrorCodeArray">
	///		错误码数组
	/// </param>
	/// <returns>
	///   如果成功返回KS_OK,如果失败返回相应错误码
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
	/// 	获取设备的寄存器信息
	/// </summary>
	/// <param name="Handle">
	///		IOServer连接句柄
	/// </param>
	/// <param name="ChannelID">
	///		链路ID
	/// <param name="DeviceID">
	///		设备ID
	/// </param>
	/// <param name="RegInfoArray">
	///		寄存器信息集合
	/// </param>
	/// <returns>
	///   如果成功返回KS_OK,如果失败返回相应错误码
	/// </returns>
	/// <remarks>
	///      若产品定义确定KS3.5中继续兼容变量IO属性操作，使用该函数
	/// </remarks>
	KS_RET KSIOAPI KSIOAPIGetRegInfobyDeviceID(
		IN KS_HANDLE								Handle,
		IN KS_UINT32								ChannelID,
		IN KS_UINT32								DeviceID, 
		OUT PKSIO_REGISTER_ARRAY					RegInfoArray) ;


	/// <summary>
	/// 	获取数据块的寄存器信息
	/// </summary>
	/// <param name="Handle">
	///		IOServer连接句柄
	/// </param>
	/// <param name="ChannelID">
	///		链路ID
	/// <param name="DeviceID">
	///		设备ID
	/// <param name="DataBlockID">
	///		数据块ID
	/// </param>
	/// <param name="RegInfo">
	///		寄存器信息
	/// </param>
	/// <returns>
	///   如果成功返回KS_OK,如果失败返回相应错误码
	/// </returns>
	/// <remarks>
	///      若产品定义确定KS3.5中继续兼容变量IO属性操作，使用该函数
	/// </remarks>
	KS_RET KSIOAPI KSIOAPIGetRegInfobyDataBlockID(
		IN KS_HANDLE								Handle,
		IN KS_UINT32								ChannelID,
		IN KS_UINT32								DeviceID, 
		IN KS_UINT32								DataBlockID, 
		OUT PKSIO_REGISTER							RegInfo) ;

	/// <summary>
	/// 	获取数据块采集周期
	/// </summary>
	/// <param name="Handle">
	///		IOServer连接句柄
	/// </param>
	/// <param name="ChannelID">
	///		链路ID
	/// <param name="DeviceID">
	///		设备ID
	/// <param name="CollectInterval">
	///		数据块采集周期
	/// </param>
	/// <returns>
	///   如果成功返回KS_OK,如果失败返回相应错误码
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
	//	九、运行访问接口函数
	//
	//==============================================================================


		//==============================================================================
	//	2、IOServer控制与状态
	//==============================================================================

	/// <summary>
	/// 	控制对象运行状态
	/// </summary>
	/// <param name="Handle">
	///		IOServer连接句柄
	/// </param>
	/// <param name=" ItemType ">
	///		访问节点类型，参考
	/// </param>
	/// <param name=" ItemIDs ">
	///		访问节点ID数组
	/// </param>
	/// <param name=" ItemStates ">
	///		激活或者取消激活节点
	/// </param>
	/// <returns>
	///		如果成功返回KS_OK,如果失败返回相应错误码
	/// </returns>
	/// <remarks>
	///		如果已经激活也返回KS_OK
	/// </remarks>
	KS_RET KSIOAPI KSIOAPISetItemState(
		IN KS_HANDLE								Handle,
		IN KS_UINT16                     ItemType,
		IN KS_UINT32							ItemID,
		IN KS_UINT16						ItemState);


	/// <summary>
	/// 	控制对象运行状态
	/// </summary>
	/// <param name="Handle">
	///		IOServer连接句柄
	/// </param>
	/// <param name=" ItemType ">
	///		访问节点类型，参考
	/// </param>
	/// <param name=" ItemIDs ">
	///		访问节点ID数组
	/// </param>
	/// <param name=" ItemStates ">
	///		激活或者取消激活节点
	/// </param>
	/// <returns>
	///		如果成功返回KS_OK,如果失败返回相应错误码
	/// </returns>
	/// <remarks>
	///		如果已经激活也返回KS_OK
	/// </remarks>
	KS_RET KSIOAPI KSIOAPIGetItemState(
		IN KS_HANDLE								Handle,
		IN KS_UINT16                     ItemType,
		IN KS_UINT32							ItemID,
		OUT KS_UINT16						ItemState);
	//==============================================================================
	//	1、变量采集配置接口
	//==============================================================================

	/// <summary>
	/// 	添加变量配置信息
	/// </summary>
	/// <param name="Handle">
	///		IOServer连接句柄
	/// </param>
	/// <param name="TagPropertyArray">
	///		采集属性数组
	/// </param>
	/// <param name="ErrorCodeArray">
	///		错误码数组
	/// </param>
	/// <returns>
	///		如果成功返回KS_OK,如果失败返回相应错误码
	/// </returns>
	/// <remarks>
	///		兼容KS3.0工程用
	/// </remarks>
	KS_RET KSIOAPI KSIOAPIAddCollectTags(
		IN KS_HANDLE								Handle, 
		IN PKSIO_TAG_PROPERTY_ARRAY					TagPropertyArray,
		OUT PKSIO_LONGARRAY							ErrorCodeArray);

	/// <summary>
	/// 	删除采集变量
	/// </summary>
	/// <param name="Handle">
	///		IOServer连接句柄
	/// </param>
	/// <param name="TagAccessIDArray">
	///		采集变量访问ID数组
	/// </param>
	/// <param name="ErrorCodeArray">
	///		错误码数组
	/// </param>
	/// <returns>
	///		如果成功返回KS_OK,如果失败返回相应错误码
	/// </returns>
	/// <remarks>
	///		兼容KS3.0工程用
	/// </remarks>
	KS_RET KSIOAPI KSIOAPIRemoveCollectTags(
		IN KS_HANDLE								Handle, 
		IN PKSIO_LONGARRAY							TagAccessIDArray,
		OUT PKSIO_LONGARRAY							ErrorCodeArray);


	/// <summary> 
	///		层次化浏览采集变量
	/// </summary> 
	/// <param name="Handle">
	///		IOServer连接句柄
	/// </param>
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
	/// <param name="TagPropertyArray">
	///		变量属性数组
	/// </param>
	/// <returns>
	///		如果成功返回KS_OK,如果失败返回相应错误码
	/// </returns>
	/// <remarks>
	///		按通道-设备-变量的层次化
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
	/// 	根据变量全名获取变量访问ID
	/// </summary>
	/// <param name="Handle">
	///		IOServer连接句柄
	/// </param>
	/// <param name="TagFullNameArray">
	///		采集变量全名数组
	/// </param>
	/// <param name="TagAccessIDArray">
	///		采集变量访问ID数组
	/// </param>
	/// <param name="ErrorCodeArray">
	///		错误码数组
	/// </param>
	/// <returns>
	///		如果成功返回KS_OK,如果失败返回相应错误码
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
	//	2、IOServer控制与状态
	//==============================================================================

	/// <summary>
	/// 	启动IOServer运行
	/// </summary>
	/// <param name="Handle">
	///		IOServer连接句柄
	/// </param>
	/// <param name="OperatorName">
	///		操作员名称
	/// </param>
	/// <param name="OperatorPassWord">
	///		操作员密码
	/// </param>
	/// <returns>
	///		如果成功返回KS_OK,如果失败返回相应错误码
	/// </returns>
	/// <remarks>
	///		如果已经运行也返回KS_OK
	/// </remarks>
	KS_RET KSIOAPI KSIOAPIServerStart(
		IN KS_HANDLE								Handle,
		IN KS_WSTR									OperatorName,
		IN KS_WSTR									OperatorPassWord);

	/// <summary>
	/// 	停止IOServer运行
	/// </summary>
	/// <param name="Handle">
	///		IOServer连接句柄
	/// </param>
	/// <param name="OperatorName">
	///		操作员名称
	/// </param>
	/// <param name="OperatorPassWord">
	///		操作员密码
	/// </param>
	/// <returns>
	///		如果成功返回KS_OK,如果失败返回相应错误码
	/// </returns>
	/// <remarks>
	///		如果已经停止也返回KS_OK
	/// </remarks>
	KS_RET KSIOAPI KSIOAPIServerStop(
		IN KS_HANDLE								Handle,
		IN KS_WSTR									OperatorName,
		IN KS_WSTR									OperatorPassWord);

	/// <summary>
	/// 	获取IOServer工作状态
	/// </summary>
	/// <param name="Handle">
	///		IOServer连接句柄
	/// </param>
	/// <param name="Status">
	///		IOServer工作状态，具体定义参看KSIO_WORK_STATUS
	/// </param>
	/// <returns>
	///		如果成功返回KS_OK,如果失败返回相应错误码
	/// </returns>
	/// <remarks>
	///   
	/// </remarks>
	KS_RET KSIOAPI KSIOAPIGetWorkStatus(
		IN KS_HANDLE								Handle, 
		OUT KS_UINT16&								Status);

	/// <summary>
	/// 	获取IOServer运行状态
	/// </summary>
	/// <param name="Handle">
	///		IOServer连接句柄
	/// </param>
	/// <param name="Status">
	///		IOServer运行状态，具体定义参看KSIO_RUN_STATUS
	/// </param>
	/// <returns>
	///		如果成功返回KS_OK,如果失败返回相应错误码
	/// </returns>
	/// <remarks>
	///     
	/// </remarks>
	KS_RET KSIOAPI KSIOAPIGetRunningStatus(
		IN KS_HANDLE								Handle,
		OUT KS_UINT16&								Status);

	//==============================================================================
	//	3、IOServer同步读写接口
	//==============================================================================

	/// <summary>
	/// 	同步读操作
	/// </summary>
	/// <param name="Handle">
	///		IOServer连接句柄
	/// </param>
	/// <param name="DataSource">
	///		采集数据来源，见KSIO_DATASOUCE
	/// </param>
	/// <param name="TagAccessIDArray">
	///		采集变量访问ID数组
	/// </param>
	/// <param name="TagAccessFieldIDArray">
	///		采集变量访问域ID数组
	/// </param>
	/// <param name="TagValueArray">
	/// 	变量值数组
	/// </param>
	/// <param name="ErrorCodeArray">
	///		错误码数组
	/// </param>
	/// <param name="AcquairTime">
	///		读取操作时间，
	///		DataSource 为CHCHE时，AcquairTime =  双向网络传输时间
	///		DataSource 为DEVICE时，AcquairTime =  采集时间+双向网络传输时间
	/// </param>
	/// <returns>
	///		如果成功返回KS_OK,如果失败返回相应错误码
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
	/// 	同步写操作，指写入真正的设备
	/// </summary>
	/// <param name="Handle">
	///		IOServer连接句柄
	/// </param>
	/// <param name="bReadBack">
	/// 	是否回读
	/// </param>
	/// <param name="TagValueArray">
	/// 	变量值数组
	/// </param>
	/// <param name="ErrorCodeArray">
	///		错误码数组
	/// </param>
	/// <param name="TransferTime">
	///		写入操作时间
	///		当bReadBack 为TRUE时，TransferTime =写入时间+回读时间+双向网络时间。
	///		当bReadBack 为FALSE时，TransferTime =写入时间+双向网络时间。
	/// </param>
	/// <returns>
	///		如果成功返回KS_OK,如果失败返回相应错误码
	/// </returns>
	KS_RET KSIOAPI KSIOAPISyncWriteTags(
		IN KS_HANDLE								Handle,
		IN BOOL										bReadBack,
		IN PKSIO_TAG_VALUE_ARRAY					TagValueArray,
		OUT PKSIO_LONGARRAY							ErrorCodeArray,
		OUT PKSIO_TIMESTAMP							TransferTime);

	//==============================================================================
	//	4、IOServer变量异步读写接口
	//==============================================================================

	/// <summary>
	/// 	异步读操作
	/// </summary>
	/// <param name="Handle">
	///		IOServer连接句柄
	/// </param>
	/// <param name="DataSource">
	///		采集数据来源，见KSIO_DATASOUCE
	/// </param>
	/// <param name="TagAccessIDArray">
	///		采集变量访问ID数组
	/// </param>
	/// <param name="TagAccessFieldIDArray">
	///		采集变量访问域ID数组
	/// </param>
	/// <param name="ErrorCodeArray">
	///		错误码数组
	/// </param>
	/// <returns>
	///		如果成功返回KS_OK,如果失败返回相应错误码
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
	/// 	异步写操作
	/// </summary>
	/// <param name="Handle">
	///		IOServer连接句柄
	/// </param>
	/// <param name="TagValueArray">
	///		变量值数组
	/// </param>
	/// <param name="ErrorCodeArray">
	///		错误码数组
	/// </param>
	/// <returns>
	///		如果成功返回KS_OK,如果失败返回相应错误码
	/// </returns>
	/// <remarks>
	///     
	/// </remarks>
	KS_RET KSIOAPI KSIOAPIAsyncWriteTags(
		IN KS_HANDLE								Handle,
		IN PKSIO_TAG_VALUE_ARRAY					TagValueArray,
		OUT PKSIO_LONGARRAY							ErrorCodeArray);

	//==============================================================================
	//	5、IOServer订阅变量值变化接口
	//==============================================================================

	/// <summary>
	/// 	订阅采集变量值变化
	/// </summary>
	/// <param name="Handle">
	///		IOServer连接句柄
	/// </param>
	/// <param name="TagAccessIDArray">
	///		采集变量访问ID数组
	/// </param>
	/// <param name="TagAccessFieldIDArray">
	///		采集变量访问域ID数组
	/// </param>
	/// <param name="ErrorCodeArray">
	///		错误码数组
	/// </param>
	/// <returns>
	///		如果成功返回KS_OK,如果失败返回相应错误码
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
	/// 	取消订阅采集变量值变化
	/// </summary>
	/// <param name="Handle">
	///		IOServer连接句柄
	/// </param>
	/// <param name="TagAccessIDArray">
	///		采集变量访问ID数组
	/// </param>
	/// <param name="TagAccessFieldIDArray">
	///		采集变量访问域ID数组
	/// </param>
	/// <param name="ErrorCodeArray">
	///		错误码数组
	/// </param>
	/// <returns>
	///		如果成功返回KS_OK,如果失败返回相应错误码
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
	//	7、关于采集设备控制与状态方法说明
	//		3.5版IOServer中对此是通过通道、设备的控制系统变量与状态系统变量实现的
	//		因此没有了直接控制通道、设备的方法和获取通道、设备状态的方法
	//==============================================================================

	//==============================================================================
	//	8、IOServer的回调函数注册接口
	//==============================================================================

	/// <summary>
	/// 	注册与服务器连接状态变化回调
	/// </summary>
	/// <param name="Handle">
	///		IOServer连接句柄
	/// </param>
	/// <param name="pCallbackFunc">
	///		回调函数指针，如果该指针为NULL,将会取消注册
	/// </param>
	/// <returns>
	///		如果成功返回KS_OK,如果失败返回相应错误码
	/// </returns>
	/// <remarks>
	///     
	/// </remarks>
	KS_RET KSIOAPI KSIOAPIRegisterConnectStatusChangedCallbackFunc(
		IN KS_HANDLE								Handle,
		IN KSIO_CONNECT_STATUS_CHANGED_CALLBACKFUNC	pCallbackFunc);

	/// <summary>
	/// 	注册IOServer工作状态变化回调
	/// </summary>
	/// <param name="Handle">
	///		IOServer连接句柄
	/// </param>
	/// <param name="pCallbackFunc">
	///		回调函数指针，如果该指针为NULL,将会取消注册
	/// </param>
	/// <returns>
	///		如果成功返回KS_OK,如果失败返回相应错误码
	/// </returns>
	/// <remarks>
	///     
	/// </remarks>
	KS_RET KSIOAPI KSIOAPIRegisterWorkStatusChangedCallbackFunc(
		IN KS_HANDLE								Handle,
		IN KSIO_WORK_STATUS_CHANGED_CALLBACKFUNC	pCallbackFunc);

	/// <summary>
	/// 	注册IOServer运行状态变化回调
	/// </summary>
	/// <param name="Handle">
	///		IOServer连接句柄
	/// </param>
	/// <param name="pCallbackFunc">
	///		回调函数指针，如果该指针为NULL,将会取消注册
	/// </param>
	/// <returns>
	///		如果成功返回KS_OK,如果失败返回相应错误码
	/// </returns>
	/// <remarks>
	///     
	/// </remarks>
	KS_RET KSIOAPI KSIOAPIRegisterRunningStatusChangedCallbackFunc(
		IN KS_HANDLE								Handle,
		IN KSIO_RUNNING_STATUS_CHANGED_CALLBACKFUNC	pCallbackFunc);

	/// <summary>
	/// 	注册变量采集数据回调函数
	/// </summary>
	/// <param name="Handle">
	///		IOServer连接句柄
	/// </param>
	/// <param name="pCallbackFunc">
	///		回调函数指针，如果该指针为NULL,将会取消注册
	/// </param>
	/// <returns>
	///		如果成功返回KS_OK,如果失败返回相应错误码
	/// </returns>
	/// <remarks>
	///     
	/// </remarks>
	KS_RET KSIOAPI KSIOAPIRegisterCollectValueCallbackFunc(
		IN KS_HANDLE								Handle,
		IN KSIO_COLLECT_VALUE_CALLBACKFUNC			pCallbackFunc);

	/// <summary>
	/// 	注册异步写变量完成回调函数
	/// </summary>
	/// <param name="Handle">
	///		IOServer连接句柄
	/// </param>
	/// <param name="pCallbackFunc">
	///		回调函数指针，如果该指针为NULL,将会取消注册
	/// </param>
	/// <returns>
	///		如果成功返回KS_OK,如果失败返回相应错误码
	/// </returns>
	/// <remarks>
	///     
	/// </remarks>
	KS_RET KSIOAPI KSIOAPIRegisterWriteCompleteCallbackFunc(
		IN KS_HANDLE								Handle,
		IN KSIO_WRITE_COMPLTED_CALLBACKFUNC			pCallbackFunc);

	/// <summary>
	/// 	注册异步读变量完成回调函数
	/// </summary>
	/// <param name="Handle">
	///		IOServer连接句柄
	/// </param>
	/// <param name="pCallbackFunc">
	///		回调函数指针，如果该指针为NULL,将会取消注册
	/// </param>
	/// <returns>
	///		如果成功返回KS_OK,如果失败返回相应错误码
	/// </returns>
	/// <remarks>
	///     
	/// </remarks>
	KS_RET KSIOAPI KSIOAPIRegisterReadCompleteCallbackFunc(
		IN KS_HANDLE								Handle,
		IN KSIO_READ_COMPLTED_CALLBACKFUNC			pCallbackFunc);

	/// <summary>
	/// 	注册采集模块元素操作回调函数
	/// </summary>
	/// <param name="Handle">
	///		IOServer连接句柄
	/// </param>
	/// <param name="pCallbackFunc">
	///		回调函数指针，如果该指针为NULL,将会取消注册
	/// </param>
	/// <returns>
	///		如果成功返回KS_OK,如果失败返回相应错误码
	/// </returns>
	/// <remarks>
	///     
	/// </remarks>
	KS_RET KSIOAPI KSIOAPIRegisterCollectElementOperateChangeCallbackFunc(
		IN KS_HANDLE										Handle,
		IN KSIO_COLLECTELEMENT_OPERTATE_CHANGED_CALLBACKFUNC	pCallbackFunc);

	/// <summary>
	/// 	注册设备状态变化回调函数
	/// </summary>
	/// <param name="Handle">
	///		IOServer连接句柄
	/// </param>
	/// <param name="pCallbackFunc">
	///		回调函数指针，如果该指针为NULL,将会取消注册
	/// </param>
	/// <returns>
	///		如果成功返回KS_OK,如果失败返回相应错误码
	/// </returns>
	/// <remarks>
	///     
	/// </remarks>
	KS_RET KSIOAPI KSIOAPIRegisterDeviceStatusChangeCallbackFunc(
		IN KS_HANDLE								Handle,
		IN KSIO_DEVICE_STATUS_CHANGE_CALLBACKFUNC	pCallbackFunc);

	//==============================================================================
	//
	//	十、IOServer运行性能接口函数
	//
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
	///		第一次执行时，IOServer端初始化内部成员，返回KS_FAIL，第二次执行才返回正确值
	/// </remarks>
	KS_RET KSIOAPI KSIOAPIGetCurrentCPUConsume(
		IN KS_HANDLE							Handle,
		OUT KS_UINT32&							CurCPUUse,
		OUT KS_UINT16&							CurCPUPercentUse,
		OUT KS_UINT32&							Reserved1,
		OUT KS_UINT32&							Reserved2);

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
	KS_RET KSIOAPI KSIOAPIGetCurrentMemComsume(
		IN KS_HANDLE							Handle,
		OUT KS_UINT32&							CurMemUse,
		OUT KS_UINT16&							CurMemPercentUse,
		OUT KS_UINT32&							Reserved1,
		OUT KS_UINT32&							Reserved2) ;


	//增加比较易用的浏览配置函数 duyisu

	// 工程
	KS_RET KSIOAPI KSIOAPIBrowserProjects(
		IN KS_HANDLE								Handle, 
		IN KS_CWSTR									Mask, 
		OUT PSIO_SERVER_PROPERTY_ARRAY				ProjectPropertyArray);


    //通道
	KS_RET KSIOAPI KSIOAPIBrowserChannels(
		IN KS_HANDLE								Handle, 
		IN KS_CWSTR									Mask, 
		OUT PKSIO_CHANNEL_PROPERTY_ARRAY				ChannelPropertyArray);

   //设备
	KS_RET KSIOAPI KSIOAPIBrowserDevices(
		IN KS_HANDLE								Handle, 
		IN KS_CWSTR									Mask, 
		OUT PKSIO_DEVICE_PROPERTY_ARRAY				DevicePropertyArray);

	//变量
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

