
#pragma warning( disable: 4049 )  /* more than 64k source lines */

/* this ALWAYS GENERATED file contains the proxy stub code */


 /* File created by MIDL compiler version 6.00.0347 */
/* at Tue Jan 29 00:25:48 2002
 */
/* Compiler settings for C:\Programming Projects\Columbia\VC\COMEVTLOGGER\COMEvtLogger.idl:
    Oicf, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

#if !defined(_M_IA64) && !defined(_M_AMD64)
#define USE_STUBLESS_PROXY


/* verify that the <rpcproxy.h> version is high enough to compile this file*/
#ifndef __REDQ_RPCPROXY_H_VERSION__
#define __REQUIRED_RPCPROXY_H_VERSION__ 440
#endif


#include "rpcproxy.h"
#ifndef __RPCPROXY_H_VERSION__
#error this stub requires an updated version of <rpcproxy.h>
#endif // __RPCPROXY_H_VERSION__


#include "COMEvtLogger.h"

#define TYPE_FORMAT_STRING_SIZE   41                                
#define PROC_FORMAT_STRING_SIZE   155                               
#define TRANSMIT_AS_TABLE_SIZE    0            
#define WIRE_MARSHAL_TABLE_SIZE   1            

typedef struct _MIDL_TYPE_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ TYPE_FORMAT_STRING_SIZE ];
    } MIDL_TYPE_FORMAT_STRING;

typedef struct _MIDL_PROC_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ PROC_FORMAT_STRING_SIZE ];
    } MIDL_PROC_FORMAT_STRING;


static RPC_SYNTAX_IDENTIFIER  _RpcTransferSyntax = 
{{0x8A885D04,0x1CEB,0x11C9,{0x9F,0xE8,0x08,0x00,0x2B,0x10,0x48,0x60}},{2,0}};


extern const MIDL_TYPE_FORMAT_STRING __MIDL_TypeFormatString;
extern const MIDL_PROC_FORMAT_STRING __MIDL_ProcFormatString;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IAMTSEvtLogger_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO IAMTSEvtLogger_ProxyInfo;


extern const USER_MARSHAL_ROUTINE_QUADRUPLE UserMarshalRoutines[ WIRE_MARSHAL_TABLE_SIZE ];

#if !defined(__RPC_WIN32__)
#error  Invalid build platform for this stub.
#endif

#if !(TARGET_IS_NT40_OR_LATER)
#error You need a Windows NT 4.0 or later to run this stub because it uses these features:
#error   -Oif or -Oicf, [wire_marshal] or [user_marshal] attribute.
#error However, your C/C++ compilation flags indicate you intend to run this app on earlier systems.
#error This app will die there with the RPC_X_WRONG_STUB_VERSION error.
#endif


static const MIDL_PROC_FORMAT_STRING __MIDL_ProcFormatString =
    {
        0,
        {

	/* Procedure AddEventSource */

			0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/*  2 */	NdrFcLong( 0x0 ),	/* 0 */
/*  6 */	NdrFcShort( 0x7 ),	/* 7 */
/*  8 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 10 */	NdrFcShort( 0x0 ),	/* 0 */
/* 12 */	NdrFcShort( 0x22 ),	/* 34 */
/* 14 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x3,		/* 3 */

	/* Parameter bstrSource */

/* 16 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 18 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 20 */	NdrFcShort( 0x1a ),	/* Type Offset=26 */

	/* Parameter pStat */

/* 22 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 24 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 26 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 28 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 30 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 32 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure ReportErrorEvent */

/* 34 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 36 */	NdrFcLong( 0x0 ),	/* 0 */
/* 40 */	NdrFcShort( 0x8 ),	/* 8 */
/* 42 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 44 */	NdrFcShort( 0x0 ),	/* 0 */
/* 46 */	NdrFcShort( 0x22 ),	/* 34 */
/* 48 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x4,		/* 4 */

	/* Parameter bstrEventData */

/* 50 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 52 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 54 */	NdrFcShort( 0x1a ),	/* Type Offset=26 */

	/* Parameter bstrEventSource */

/* 56 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 58 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 60 */	NdrFcShort( 0x1a ),	/* Type Offset=26 */

	/* Parameter pStat */

/* 62 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 64 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 66 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 68 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 70 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 72 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure ReportInfoEvent */

/* 74 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 76 */	NdrFcLong( 0x0 ),	/* 0 */
/* 80 */	NdrFcShort( 0x9 ),	/* 9 */
/* 82 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 84 */	NdrFcShort( 0x0 ),	/* 0 */
/* 86 */	NdrFcShort( 0x22 ),	/* 34 */
/* 88 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x4,		/* 4 */

	/* Parameter bstrEventData */

/* 90 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 92 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 94 */	NdrFcShort( 0x1a ),	/* Type Offset=26 */

	/* Parameter bstrEventSource */

/* 96 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 98 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 100 */	NdrFcShort( 0x1a ),	/* Type Offset=26 */

	/* Parameter pStat */

/* 102 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 104 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 106 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 108 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 110 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 112 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure ReportWarningEvent */

/* 114 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 116 */	NdrFcLong( 0x0 ),	/* 0 */
/* 120 */	NdrFcShort( 0xa ),	/* 10 */
/* 122 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 124 */	NdrFcShort( 0x0 ),	/* 0 */
/* 126 */	NdrFcShort( 0x22 ),	/* 34 */
/* 128 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x4,		/* 4 */

	/* Parameter bstrEventData */

/* 130 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 132 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 134 */	NdrFcShort( 0x1a ),	/* Type Offset=26 */

	/* Parameter bstrEventSource */

/* 136 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 138 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 140 */	NdrFcShort( 0x1a ),	/* Type Offset=26 */

	/* Parameter pStat */

/* 142 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 144 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 146 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 148 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 150 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 152 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

			0x0
        }
    };

static const MIDL_TYPE_FORMAT_STRING __MIDL_TypeFormatString =
    {
        0,
        {
			NdrFcShort( 0x0 ),	/* 0 */
/*  2 */	
			0x12, 0x0,	/* FC_UP */
/*  4 */	NdrFcShort( 0xc ),	/* Offset= 12 (16) */
/*  6 */	
			0x1b,		/* FC_CARRAY */
			0x1,		/* 1 */
/*  8 */	NdrFcShort( 0x2 ),	/* 2 */
/* 10 */	0x9,		/* Corr desc: FC_ULONG */
			0x0,		/*  */
/* 12 */	NdrFcShort( 0xfffc ),	/* -4 */
/* 14 */	0x6,		/* FC_SHORT */
			0x5b,		/* FC_END */
/* 16 */	
			0x17,		/* FC_CSTRUCT */
			0x3,		/* 3 */
/* 18 */	NdrFcShort( 0x8 ),	/* 8 */
/* 20 */	NdrFcShort( 0xfffffff2 ),	/* Offset= -14 (6) */
/* 22 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 24 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 26 */	0xb4,		/* FC_USER_MARSHAL */
			0x83,		/* 131 */
/* 28 */	NdrFcShort( 0x0 ),	/* 0 */
/* 30 */	NdrFcShort( 0x4 ),	/* 4 */
/* 32 */	NdrFcShort( 0x0 ),	/* 0 */
/* 34 */	NdrFcShort( 0xffffffe0 ),	/* Offset= -32 (2) */
/* 36 */	
			0x11, 0xc,	/* FC_RP [alloced_on_stack] [simple_pointer] */
/* 38 */	0x6,		/* FC_SHORT */
			0x5c,		/* FC_PAD */

			0x0
        }
    };

static const USER_MARSHAL_ROUTINE_QUADRUPLE UserMarshalRoutines[ WIRE_MARSHAL_TABLE_SIZE ] = 
        {
            
            {
            BSTR_UserSize
            ,BSTR_UserMarshal
            ,BSTR_UserUnmarshal
            ,BSTR_UserFree
            }

        };



/* Object interface: IUnknown, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46}} */


/* Object interface: IDispatch, ver. 0.0,
   GUID={0x00020400,0x0000,0x0000,{0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46}} */


/* Object interface: IAMTSEvtLogger, ver. 0.0,
   GUID={0x2C4C8D6D,0xF7A1,0x472D,{0x94,0x2D,0xE2,0xD4,0xEF,0x0F,0x08,0x46}} */

#pragma code_seg(".orpc")
static const unsigned short IAMTSEvtLogger_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    0,
    34,
    74,
    114
    };

static const MIDL_STUBLESS_PROXY_INFO IAMTSEvtLogger_ProxyInfo =
    {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &IAMTSEvtLogger_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO IAMTSEvtLogger_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &IAMTSEvtLogger_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(11) _IAMTSEvtLoggerProxyVtbl = 
{
    &IAMTSEvtLogger_ProxyInfo,
    &IID_IAMTSEvtLogger,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfoCount */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfo */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    (void *) (INT_PTR) -1 /* IAMTSEvtLogger::AddEventSource */ ,
    (void *) (INT_PTR) -1 /* IAMTSEvtLogger::ReportErrorEvent */ ,
    (void *) (INT_PTR) -1 /* IAMTSEvtLogger::ReportInfoEvent */ ,
    (void *) (INT_PTR) -1 /* IAMTSEvtLogger::ReportWarningEvent */
};


static const PRPC_STUB_FUNCTION IAMTSEvtLogger_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl _IAMTSEvtLoggerStubVtbl =
{
    &IID_IAMTSEvtLogger,
    &IAMTSEvtLogger_ServerInfo,
    11,
    &IAMTSEvtLogger_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};

static const MIDL_STUB_DESC Object_StubDesc = 
    {
    0,
    NdrOleAllocate,
    NdrOleFree,
    0,
    0,
    0,
    0,
    0,
    __MIDL_TypeFormatString.Format,
    1, /* -error bounds_check flag */
    0x20000, /* Ndr library version */
    0,
    0x600015b, /* MIDL Version 6.0.347 */
    0,
    UserMarshalRoutines,
    0,  /* notify & notify_flag routine table */
    0x1, /* MIDL flag */
    0, /* cs routines */
    0,   /* proxy/server info */
    0   /* Reserved5 */
    };

const CInterfaceProxyVtbl * _COMEvtLogger_ProxyVtblList[] = 
{
    ( CInterfaceProxyVtbl *) &_IAMTSEvtLoggerProxyVtbl,
    0
};

const CInterfaceStubVtbl * _COMEvtLogger_StubVtblList[] = 
{
    ( CInterfaceStubVtbl *) &_IAMTSEvtLoggerStubVtbl,
    0
};

PCInterfaceName const _COMEvtLogger_InterfaceNamesList[] = 
{
    "IAMTSEvtLogger",
    0
};

const IID *  _COMEvtLogger_BaseIIDList[] = 
{
    &IID_IDispatch,
    0
};


#define _COMEvtLogger_CHECK_IID(n)	IID_GENERIC_CHECK_IID( _COMEvtLogger, pIID, n)

int __stdcall _COMEvtLogger_IID_Lookup( const IID * pIID, int * pIndex )
{
    
    if(!_COMEvtLogger_CHECK_IID(0))
        {
        *pIndex = 0;
        return 1;
        }

    return 0;
}

const ExtendedProxyFileInfo COMEvtLogger_ProxyFileInfo = 
{
    (PCInterfaceProxyVtblList *) & _COMEvtLogger_ProxyVtblList,
    (PCInterfaceStubVtblList *) & _COMEvtLogger_StubVtblList,
    (const PCInterfaceName * ) & _COMEvtLogger_InterfaceNamesList,
    (const IID ** ) & _COMEvtLogger_BaseIIDList,
    & _COMEvtLogger_IID_Lookup, 
    1,
    2,
    0, /* table of [async_uuid] interfaces */
    0, /* Filler1 */
    0, /* Filler2 */
    0  /* Filler3 */
};


#endif /* !defined(_M_IA64) && !defined(_M_AMD64)*/


#pragma warning( disable: 4049 )  /* more than 64k source lines */

/* this ALWAYS GENERATED file contains the proxy stub code */


 /* File created by MIDL compiler version 6.00.0347 */
/* at Tue Jan 29 00:25:48 2002
 */
/* Compiler settings for C:\Programming Projects\Columbia\VC\COMEVTLOGGER\COMEvtLogger.idl:
    Oicf, W1, Zp8, env=Win64 (32b run,appending)
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

#if defined(_M_IA64) || defined(_M_AMD64)
#define USE_STUBLESS_PROXY


/* verify that the <rpcproxy.h> version is high enough to compile this file*/
#ifndef __REDQ_RPCPROXY_H_VERSION__
#define __REQUIRED_RPCPROXY_H_VERSION__ 475
#endif


#include "rpcproxy.h"
#ifndef __RPCPROXY_H_VERSION__
#error this stub requires an updated version of <rpcproxy.h>
#endif // __RPCPROXY_H_VERSION__


#include "COMEvtLogger.h"

#define TYPE_FORMAT_STRING_SIZE   43                                
#define PROC_FORMAT_STRING_SIZE   195                               
#define TRANSMIT_AS_TABLE_SIZE    0            
#define WIRE_MARSHAL_TABLE_SIZE   1            

typedef struct _MIDL_TYPE_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ TYPE_FORMAT_STRING_SIZE ];
    } MIDL_TYPE_FORMAT_STRING;

typedef struct _MIDL_PROC_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ PROC_FORMAT_STRING_SIZE ];
    } MIDL_PROC_FORMAT_STRING;


static RPC_SYNTAX_IDENTIFIER  _RpcTransferSyntax = 
{{0x8A885D04,0x1CEB,0x11C9,{0x9F,0xE8,0x08,0x00,0x2B,0x10,0x48,0x60}},{2,0}};


extern const MIDL_TYPE_FORMAT_STRING __MIDL_TypeFormatString;
extern const MIDL_PROC_FORMAT_STRING __MIDL_ProcFormatString;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IAMTSEvtLogger_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO IAMTSEvtLogger_ProxyInfo;


extern const USER_MARSHAL_ROUTINE_QUADRUPLE UserMarshalRoutines[ WIRE_MARSHAL_TABLE_SIZE ];

#if !defined(__RPC_WIN64__)
#error  Invalid build platform for this stub.
#endif

static const MIDL_PROC_FORMAT_STRING __MIDL_ProcFormatString =
    {
        0,
        {

	/* Procedure AddEventSource */

			0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/*  2 */	NdrFcLong( 0x0 ),	/* 0 */
/*  6 */	NdrFcShort( 0x7 ),	/* 7 */
/*  8 */	NdrFcShort( 0x20 ),	/* ia64 Stack size/offset = 32 */
/* 10 */	NdrFcShort( 0x0 ),	/* 0 */
/* 12 */	NdrFcShort( 0x22 ),	/* 34 */
/* 14 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 16 */	0xa,		/* 10 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 18 */	NdrFcShort( 0x0 ),	/* 0 */
/* 20 */	NdrFcShort( 0x1 ),	/* 1 */
/* 22 */	NdrFcShort( 0x0 ),	/* 0 */
/* 24 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter bstrSource */

/* 26 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 28 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 30 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter pStat */

/* 32 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 34 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 36 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 38 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 40 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 42 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure ReportErrorEvent */

/* 44 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 46 */	NdrFcLong( 0x0 ),	/* 0 */
/* 50 */	NdrFcShort( 0x8 ),	/* 8 */
/* 52 */	NdrFcShort( 0x28 ),	/* ia64 Stack size/offset = 40 */
/* 54 */	NdrFcShort( 0x0 ),	/* 0 */
/* 56 */	NdrFcShort( 0x22 ),	/* 34 */
/* 58 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x4,		/* 4 */
/* 60 */	0xa,		/* 10 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 62 */	NdrFcShort( 0x0 ),	/* 0 */
/* 64 */	NdrFcShort( 0x2 ),	/* 2 */
/* 66 */	NdrFcShort( 0x0 ),	/* 0 */
/* 68 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter bstrEventData */

/* 70 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 72 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 74 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter bstrEventSource */

/* 76 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 78 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 80 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter pStat */

/* 82 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 84 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 86 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 88 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 90 */	NdrFcShort( 0x20 ),	/* ia64 Stack size/offset = 32 */
/* 92 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure ReportInfoEvent */

/* 94 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 96 */	NdrFcLong( 0x0 ),	/* 0 */
/* 100 */	NdrFcShort( 0x9 ),	/* 9 */
/* 102 */	NdrFcShort( 0x28 ),	/* ia64 Stack size/offset = 40 */
/* 104 */	NdrFcShort( 0x0 ),	/* 0 */
/* 106 */	NdrFcShort( 0x22 ),	/* 34 */
/* 108 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x4,		/* 4 */
/* 110 */	0xa,		/* 10 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 112 */	NdrFcShort( 0x0 ),	/* 0 */
/* 114 */	NdrFcShort( 0x2 ),	/* 2 */
/* 116 */	NdrFcShort( 0x0 ),	/* 0 */
/* 118 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter bstrEventData */

/* 120 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 122 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 124 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter bstrEventSource */

/* 126 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 128 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 130 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter pStat */

/* 132 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 134 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 136 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 138 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 140 */	NdrFcShort( 0x20 ),	/* ia64 Stack size/offset = 32 */
/* 142 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure ReportWarningEvent */

/* 144 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 146 */	NdrFcLong( 0x0 ),	/* 0 */
/* 150 */	NdrFcShort( 0xa ),	/* 10 */
/* 152 */	NdrFcShort( 0x28 ),	/* ia64 Stack size/offset = 40 */
/* 154 */	NdrFcShort( 0x0 ),	/* 0 */
/* 156 */	NdrFcShort( 0x22 ),	/* 34 */
/* 158 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x4,		/* 4 */
/* 160 */	0xa,		/* 10 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 162 */	NdrFcShort( 0x0 ),	/* 0 */
/* 164 */	NdrFcShort( 0x2 ),	/* 2 */
/* 166 */	NdrFcShort( 0x0 ),	/* 0 */
/* 168 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter bstrEventData */

/* 170 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 172 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 174 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter bstrEventSource */

/* 176 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 178 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 180 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter pStat */

/* 182 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 184 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 186 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 188 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 190 */	NdrFcShort( 0x20 ),	/* ia64 Stack size/offset = 32 */
/* 192 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

			0x0
        }
    };

static const MIDL_TYPE_FORMAT_STRING __MIDL_TypeFormatString =
    {
        0,
        {
			NdrFcShort( 0x0 ),	/* 0 */
/*  2 */	
			0x12, 0x0,	/* FC_UP */
/*  4 */	NdrFcShort( 0xe ),	/* Offset= 14 (18) */
/*  6 */	
			0x1b,		/* FC_CARRAY */
			0x1,		/* 1 */
/*  8 */	NdrFcShort( 0x2 ),	/* 2 */
/* 10 */	0x9,		/* Corr desc: FC_ULONG */
			0x0,		/*  */
/* 12 */	NdrFcShort( 0xfffc ),	/* -4 */
/* 14 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 16 */	0x6,		/* FC_SHORT */
			0x5b,		/* FC_END */
/* 18 */	
			0x17,		/* FC_CSTRUCT */
			0x3,		/* 3 */
/* 20 */	NdrFcShort( 0x8 ),	/* 8 */
/* 22 */	NdrFcShort( 0xfffffff0 ),	/* Offset= -16 (6) */
/* 24 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 26 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 28 */	0xb4,		/* FC_USER_MARSHAL */
			0x83,		/* 131 */
/* 30 */	NdrFcShort( 0x0 ),	/* 0 */
/* 32 */	NdrFcShort( 0x8 ),	/* 8 */
/* 34 */	NdrFcShort( 0x0 ),	/* 0 */
/* 36 */	NdrFcShort( 0xffffffde ),	/* Offset= -34 (2) */
/* 38 */	
			0x11, 0xc,	/* FC_RP [alloced_on_stack] [simple_pointer] */
/* 40 */	0x6,		/* FC_SHORT */
			0x5c,		/* FC_PAD */

			0x0
        }
    };

static const USER_MARSHAL_ROUTINE_QUADRUPLE UserMarshalRoutines[ WIRE_MARSHAL_TABLE_SIZE ] = 
        {
            
            {
            BSTR_UserSize
            ,BSTR_UserMarshal
            ,BSTR_UserUnmarshal
            ,BSTR_UserFree
            }

        };



/* Object interface: IUnknown, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46}} */


/* Object interface: IDispatch, ver. 0.0,
   GUID={0x00020400,0x0000,0x0000,{0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46}} */


/* Object interface: IAMTSEvtLogger, ver. 0.0,
   GUID={0x2C4C8D6D,0xF7A1,0x472D,{0x94,0x2D,0xE2,0xD4,0xEF,0x0F,0x08,0x46}} */

#pragma code_seg(".orpc")
static const unsigned short IAMTSEvtLogger_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    0,
    44,
    94,
    144
    };

static const MIDL_STUBLESS_PROXY_INFO IAMTSEvtLogger_ProxyInfo =
    {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &IAMTSEvtLogger_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO IAMTSEvtLogger_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &IAMTSEvtLogger_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(11) _IAMTSEvtLoggerProxyVtbl = 
{
    &IAMTSEvtLogger_ProxyInfo,
    &IID_IAMTSEvtLogger,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfoCount */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfo */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    (void *) (INT_PTR) -1 /* IAMTSEvtLogger::AddEventSource */ ,
    (void *) (INT_PTR) -1 /* IAMTSEvtLogger::ReportErrorEvent */ ,
    (void *) (INT_PTR) -1 /* IAMTSEvtLogger::ReportInfoEvent */ ,
    (void *) (INT_PTR) -1 /* IAMTSEvtLogger::ReportWarningEvent */
};


static const PRPC_STUB_FUNCTION IAMTSEvtLogger_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl _IAMTSEvtLoggerStubVtbl =
{
    &IID_IAMTSEvtLogger,
    &IAMTSEvtLogger_ServerInfo,
    11,
    &IAMTSEvtLogger_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};

static const MIDL_STUB_DESC Object_StubDesc = 
    {
    0,
    NdrOleAllocate,
    NdrOleFree,
    0,
    0,
    0,
    0,
    0,
    __MIDL_TypeFormatString.Format,
    1, /* -error bounds_check flag */
    0x50002, /* Ndr library version */
    0,
    0x600015b, /* MIDL Version 6.0.347 */
    0,
    UserMarshalRoutines,
    0,  /* notify & notify_flag routine table */
    0x1, /* MIDL flag */
    0, /* cs routines */
    0,   /* proxy/server info */
    0   /* Reserved5 */
    };

const CInterfaceProxyVtbl * _COMEvtLogger_ProxyVtblList[] = 
{
    ( CInterfaceProxyVtbl *) &_IAMTSEvtLoggerProxyVtbl,
    0
};

const CInterfaceStubVtbl * _COMEvtLogger_StubVtblList[] = 
{
    ( CInterfaceStubVtbl *) &_IAMTSEvtLoggerStubVtbl,
    0
};

PCInterfaceName const _COMEvtLogger_InterfaceNamesList[] = 
{
    "IAMTSEvtLogger",
    0
};

const IID *  _COMEvtLogger_BaseIIDList[] = 
{
    &IID_IDispatch,
    0
};


#define _COMEvtLogger_CHECK_IID(n)	IID_GENERIC_CHECK_IID( _COMEvtLogger, pIID, n)

int __stdcall _COMEvtLogger_IID_Lookup( const IID * pIID, int * pIndex )
{
    
    if(!_COMEvtLogger_CHECK_IID(0))
        {
        *pIndex = 0;
        return 1;
        }

    return 0;
}

const ExtendedProxyFileInfo COMEvtLogger_ProxyFileInfo = 
{
    (PCInterfaceProxyVtblList *) & _COMEvtLogger_ProxyVtblList,
    (PCInterfaceStubVtblList *) & _COMEvtLogger_StubVtblList,
    (const PCInterfaceName * ) & _COMEvtLogger_InterfaceNamesList,
    (const IID ** ) & _COMEvtLogger_BaseIIDList,
    & _COMEvtLogger_IID_Lookup, 
    1,
    2,
    0, /* table of [async_uuid] interfaces */
    0, /* Filler1 */
    0, /* Filler2 */
    0  /* Filler3 */
};


#endif /* defined(_M_IA64) || defined(_M_AMD64)*/

