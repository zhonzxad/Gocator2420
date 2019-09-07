
//==============================================================================
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __kxIOAPI_inc_H_INCLUDED__
#define __kxIOAPI_inc_H_INCLUDED__
//==============================================================================

//==============================================================================
/// 导入导出定义宏
#ifdef _AFXDLL

#ifdef _KXIOAPI_DLL_FLAG
#define XKX_IOAPI_CLASSEXT	_declspec(dllexport)
#define XKX_IOAPI_STDEXT		_declspec(dllexport)
#else
#define XKX_IOAPI_CLASSEXT	_declspec(dllimport)
#define XKX_IOAPI_STDEXT	_declspec(dllimport)
#endif
#else
#define XKX_IOAPI_CLASSEXT
#define XKX_IOAPI_STDEXT
#endif

//==============================================================================

//#include "kxObject/kxObject_inc.h"


//==============================================================================
//包含头文件
#include "kxIOClient/IOServerAPI.h"
#include "kxIOServerCommon/kxIOServerCommon_inc.h"
#include "kxIOClient/IkxIOServerCallBack.h"
#include "kxIOClient/IkxIOServerHandle.h"
#include "kxIOClient/IkxIOServerConnect.h"


//==============================================================================
//引用模块命名空间
//using namespace NkxIOServerCommon;
//using namespace NkxIOAPI;

XKX_IOAPI_STDEXT IkxIOServerConnect* kxf_GetIOServerConnectManager();

//==============================================================================

#endif #define __kxIOAPI_inc_H_INCLUDED__