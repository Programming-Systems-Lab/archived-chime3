
#pragma warning( disable: 4049 )  /* more than 64k source lines */

/* this ALWAYS GENERATED file contains the proxy stub code */


 /* File created by MIDL compiler version 6.00.0347 */
/* at Tue Feb 05 21:43:06 2002
 */
/* Compiler settings for C:\Programming Projects\Columbia\VC\MSDocProp\MSDocProp.idl:
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


#include "MSDocProp.h"

#define TYPE_FORMAT_STRING_SIZE   55                                
#define PROC_FORMAT_STRING_SIZE   171                               
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


extern const MIDL_SERVER_INFO IAMTSSimpleProp_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO IAMTSSimpleProp_ProxyInfo;


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

	/* Procedure ViewSummaryInfo */

			0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/*  2 */	NdrFcLong( 0x0 ),	/* 0 */
/*  6 */	NdrFcShort( 0x7 ),	/* 7 */
/*  8 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 10 */	NdrFcShort( 0x0 ),	/* 0 */
/* 12 */	NdrFcShort( 0x8 ),	/* 8 */
/* 14 */	0x7,		/* Oi2 Flags:  srv must size, clt must size, has return, */
			0x3,		/* 3 */

	/* Parameter bstrFullQDocPath */

/* 16 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 18 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 20 */	NdrFcShort( 0x1a ),	/* Type Offset=26 */

	/* Parameter pbstrXMLPropString */

/* 22 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
/* 24 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 26 */	NdrFcShort( 0x2c ),	/* Type Offset=44 */

	/* Return value */

/* 28 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 30 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 32 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure ViewDocSummaryInfo */

/* 34 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 36 */	NdrFcLong( 0x0 ),	/* 0 */
/* 40 */	NdrFcShort( 0x8 ),	/* 8 */
/* 42 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 44 */	NdrFcShort( 0x0 ),	/* 0 */
/* 46 */	NdrFcShort( 0x8 ),	/* 8 */
/* 48 */	0x7,		/* Oi2 Flags:  srv must size, clt must size, has return, */
			0x3,		/* 3 */

	/* Parameter bstrFullQDocPath */

/* 50 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 52 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 54 */	NdrFcShort( 0x1a ),	/* Type Offset=26 */

	/* Parameter pbstrXMLPropString */

/* 56 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
/* 58 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 60 */	NdrFcShort( 0x2c ),	/* Type Offset=44 */

	/* Return value */

/* 62 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 64 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 66 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure ViewMFSummaryInfo */

/* 68 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 70 */	NdrFcLong( 0x0 ),	/* 0 */
/* 74 */	NdrFcShort( 0x9 ),	/* 9 */
/* 76 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 78 */	NdrFcShort( 0x0 ),	/* 0 */
/* 80 */	NdrFcShort( 0x8 ),	/* 8 */
/* 82 */	0x7,		/* Oi2 Flags:  srv must size, clt must size, has return, */
			0x3,		/* 3 */

	/* Parameter bstrFullQDocPath */

/* 84 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 86 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 88 */	NdrFcShort( 0x1a ),	/* Type Offset=26 */

	/* Parameter pbstrXMLPropString */

/* 90 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
/* 92 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 94 */	NdrFcShort( 0x2c ),	/* Type Offset=44 */

	/* Return value */

/* 96 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 98 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 100 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure ViewUserDefinedProperties */

/* 102 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 104 */	NdrFcLong( 0x0 ),	/* 0 */
/* 108 */	NdrFcShort( 0xa ),	/* 10 */
/* 110 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 112 */	NdrFcShort( 0x0 ),	/* 0 */
/* 114 */	NdrFcShort( 0x8 ),	/* 8 */
/* 116 */	0x7,		/* Oi2 Flags:  srv must size, clt must size, has return, */
			0x3,		/* 3 */

	/* Parameter bstrFullQDocPath */

/* 118 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 120 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 122 */	NdrFcShort( 0x1a ),	/* Type Offset=26 */

	/* Parameter pbstrXMLPropString */

/* 124 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
/* 126 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 128 */	NdrFcShort( 0x2c ),	/* Type Offset=44 */

	/* Return value */

/* 130 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 132 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 134 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure ViewHyperlinks */

/* 136 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 138 */	NdrFcLong( 0x0 ),	/* 0 */
/* 142 */	NdrFcShort( 0xb ),	/* 11 */
/* 144 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 146 */	NdrFcShort( 0x0 ),	/* 0 */
/* 148 */	NdrFcShort( 0x8 ),	/* 8 */
/* 150 */	0x7,		/* Oi2 Flags:  srv must size, clt must size, has return, */
			0x3,		/* 3 */

	/* Parameter bstrFullQDocPath */

/* 152 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 154 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 156 */	NdrFcShort( 0x1a ),	/* Type Offset=26 */

	/* Parameter pbstrXMLPropString */

/* 158 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
/* 160 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 162 */	NdrFcShort( 0x2c ),	/* Type Offset=44 */

	/* Return value */

/* 164 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 166 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 168 */	0x8,		/* FC_LONG */
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
			0x11, 0x4,	/* FC_RP [alloced_on_stack] */
/* 38 */	NdrFcShort( 0x6 ),	/* Offset= 6 (44) */
/* 40 */	
			0x13, 0x0,	/* FC_OP */
/* 42 */	NdrFcShort( 0xffffffe6 ),	/* Offset= -26 (16) */
/* 44 */	0xb4,		/* FC_USER_MARSHAL */
			0x83,		/* 131 */
/* 46 */	NdrFcShort( 0x0 ),	/* 0 */
/* 48 */	NdrFcShort( 0x4 ),	/* 4 */
/* 50 */	NdrFcShort( 0x0 ),	/* 0 */
/* 52 */	NdrFcShort( 0xfffffff4 ),	/* Offset= -12 (40) */

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


/* Object interface: IAMTSSimpleProp, ver. 0.0,
   GUID={0xE4CA377E,0x7A32,0x49C5,{0xB0,0xCB,0x00,0x3A,0xBE,0xF4,0x24,0x39}} */

#pragma code_seg(".orpc")
static const unsigned short IAMTSSimpleProp_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    0,
    34,
    68,
    102,
    136
    };

static const MIDL_STUBLESS_PROXY_INFO IAMTSSimpleProp_ProxyInfo =
    {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &IAMTSSimpleProp_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO IAMTSSimpleProp_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &IAMTSSimpleProp_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(12) _IAMTSSimplePropProxyVtbl = 
{
    &IAMTSSimpleProp_ProxyInfo,
    &IID_IAMTSSimpleProp,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfoCount */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfo */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    (void *) (INT_PTR) -1 /* IAMTSSimpleProp::ViewSummaryInfo */ ,
    (void *) (INT_PTR) -1 /* IAMTSSimpleProp::ViewDocSummaryInfo */ ,
    (void *) (INT_PTR) -1 /* IAMTSSimpleProp::ViewMFSummaryInfo */ ,
    (void *) (INT_PTR) -1 /* IAMTSSimpleProp::ViewUserDefinedProperties */ ,
    (void *) (INT_PTR) -1 /* IAMTSSimpleProp::ViewHyperlinks */
};


static const PRPC_STUB_FUNCTION IAMTSSimpleProp_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl _IAMTSSimplePropStubVtbl =
{
    &IID_IAMTSSimpleProp,
    &IAMTSSimpleProp_ServerInfo,
    12,
    &IAMTSSimpleProp_table[-3],
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

const CInterfaceProxyVtbl * _MSDocProp_ProxyVtblList[] = 
{
    ( CInterfaceProxyVtbl *) &_IAMTSSimplePropProxyVtbl,
    0
};

const CInterfaceStubVtbl * _MSDocProp_StubVtblList[] = 
{
    ( CInterfaceStubVtbl *) &_IAMTSSimplePropStubVtbl,
    0
};

PCInterfaceName const _MSDocProp_InterfaceNamesList[] = 
{
    "IAMTSSimpleProp",
    0
};

const IID *  _MSDocProp_BaseIIDList[] = 
{
    &IID_IDispatch,
    0
};


#define _MSDocProp_CHECK_IID(n)	IID_GENERIC_CHECK_IID( _MSDocProp, pIID, n)

int __stdcall _MSDocProp_IID_Lookup( const IID * pIID, int * pIndex )
{
    
    if(!_MSDocProp_CHECK_IID(0))
        {
        *pIndex = 0;
        return 1;
        }

    return 0;
}

const ExtendedProxyFileInfo MSDocProp_ProxyFileInfo = 
{
    (PCInterfaceProxyVtblList *) & _MSDocProp_ProxyVtblList,
    (PCInterfaceStubVtblList *) & _MSDocProp_StubVtblList,
    (const PCInterfaceName * ) & _MSDocProp_InterfaceNamesList,
    (const IID ** ) & _MSDocProp_BaseIIDList,
    & _MSDocProp_IID_Lookup, 
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
/* at Tue Feb 05 21:43:06 2002
 */
/* Compiler settings for C:\Programming Projects\Columbia\VC\MSDocProp\MSDocProp.idl:
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


#include "MSDocProp.h"

#define TYPE_FORMAT_STRING_SIZE   57                                
#define PROC_FORMAT_STRING_SIZE   221                               
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


extern const MIDL_SERVER_INFO IAMTSSimpleProp_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO IAMTSSimpleProp_ProxyInfo;


extern const USER_MARSHAL_ROUTINE_QUADRUPLE UserMarshalRoutines[ WIRE_MARSHAL_TABLE_SIZE ];

#if !defined(__RPC_WIN64__)
#error  Invalid build platform for this stub.
#endif

static const MIDL_PROC_FORMAT_STRING __MIDL_ProcFormatString =
    {
        0,
        {

	/* Procedure ViewSummaryInfo */

			0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/*  2 */	NdrFcLong( 0x0 ),	/* 0 */
/*  6 */	NdrFcShort( 0x7 ),	/* 7 */
/*  8 */	NdrFcShort( 0x20 ),	/* ia64 Stack size/offset = 32 */
/* 10 */	NdrFcShort( 0x0 ),	/* 0 */
/* 12 */	NdrFcShort( 0x8 ),	/* 8 */
/* 14 */	0x47,		/* Oi2 Flags:  srv must size, clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 16 */	0xa,		/* 10 */
			0x7,		/* Ext Flags:  new corr desc, clt corr check, srv corr check, */
/* 18 */	NdrFcShort( 0x1 ),	/* 1 */
/* 20 */	NdrFcShort( 0x1 ),	/* 1 */
/* 22 */	NdrFcShort( 0x0 ),	/* 0 */
/* 24 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter bstrFullQDocPath */

/* 26 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 28 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 30 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter pbstrXMLPropString */

/* 32 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
/* 34 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 36 */	NdrFcShort( 0x2e ),	/* Type Offset=46 */

	/* Return value */

/* 38 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 40 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 42 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure ViewDocSummaryInfo */

/* 44 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 46 */	NdrFcLong( 0x0 ),	/* 0 */
/* 50 */	NdrFcShort( 0x8 ),	/* 8 */
/* 52 */	NdrFcShort( 0x20 ),	/* ia64 Stack size/offset = 32 */
/* 54 */	NdrFcShort( 0x0 ),	/* 0 */
/* 56 */	NdrFcShort( 0x8 ),	/* 8 */
/* 58 */	0x47,		/* Oi2 Flags:  srv must size, clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 60 */	0xa,		/* 10 */
			0x7,		/* Ext Flags:  new corr desc, clt corr check, srv corr check, */
/* 62 */	NdrFcShort( 0x1 ),	/* 1 */
/* 64 */	NdrFcShort( 0x1 ),	/* 1 */
/* 66 */	NdrFcShort( 0x0 ),	/* 0 */
/* 68 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter bstrFullQDocPath */

/* 70 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 72 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 74 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter pbstrXMLPropString */

/* 76 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
/* 78 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 80 */	NdrFcShort( 0x2e ),	/* Type Offset=46 */

	/* Return value */

/* 82 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 84 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 86 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure ViewMFSummaryInfo */

/* 88 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 90 */	NdrFcLong( 0x0 ),	/* 0 */
/* 94 */	NdrFcShort( 0x9 ),	/* 9 */
/* 96 */	NdrFcShort( 0x20 ),	/* ia64 Stack size/offset = 32 */
/* 98 */	NdrFcShort( 0x0 ),	/* 0 */
/* 100 */	NdrFcShort( 0x8 ),	/* 8 */
/* 102 */	0x47,		/* Oi2 Flags:  srv must size, clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 104 */	0xa,		/* 10 */
			0x7,		/* Ext Flags:  new corr desc, clt corr check, srv corr check, */
/* 106 */	NdrFcShort( 0x1 ),	/* 1 */
/* 108 */	NdrFcShort( 0x1 ),	/* 1 */
/* 110 */	NdrFcShort( 0x0 ),	/* 0 */
/* 112 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter bstrFullQDocPath */

/* 114 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 116 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 118 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter pbstrXMLPropString */

/* 120 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
/* 122 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 124 */	NdrFcShort( 0x2e ),	/* Type Offset=46 */

	/* Return value */

/* 126 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 128 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 130 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure ViewUserDefinedProperties */

/* 132 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 134 */	NdrFcLong( 0x0 ),	/* 0 */
/* 138 */	NdrFcShort( 0xa ),	/* 10 */
/* 140 */	NdrFcShort( 0x20 ),	/* ia64 Stack size/offset = 32 */
/* 142 */	NdrFcShort( 0x0 ),	/* 0 */
/* 144 */	NdrFcShort( 0x8 ),	/* 8 */
/* 146 */	0x47,		/* Oi2 Flags:  srv must size, clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 148 */	0xa,		/* 10 */
			0x7,		/* Ext Flags:  new corr desc, clt corr check, srv corr check, */
/* 150 */	NdrFcShort( 0x1 ),	/* 1 */
/* 152 */	NdrFcShort( 0x1 ),	/* 1 */
/* 154 */	NdrFcShort( 0x0 ),	/* 0 */
/* 156 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter bstrFullQDocPath */

/* 158 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 160 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 162 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter pbstrXMLPropString */

/* 164 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
/* 166 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 168 */	NdrFcShort( 0x2e ),	/* Type Offset=46 */

	/* Return value */

/* 170 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 172 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 174 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure ViewHyperlinks */

/* 176 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 178 */	NdrFcLong( 0x0 ),	/* 0 */
/* 182 */	NdrFcShort( 0xb ),	/* 11 */
/* 184 */	NdrFcShort( 0x20 ),	/* ia64 Stack size/offset = 32 */
/* 186 */	NdrFcShort( 0x0 ),	/* 0 */
/* 188 */	NdrFcShort( 0x8 ),	/* 8 */
/* 190 */	0x47,		/* Oi2 Flags:  srv must size, clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 192 */	0xa,		/* 10 */
			0x7,		/* Ext Flags:  new corr desc, clt corr check, srv corr check, */
/* 194 */	NdrFcShort( 0x1 ),	/* 1 */
/* 196 */	NdrFcShort( 0x1 ),	/* 1 */
/* 198 */	NdrFcShort( 0x0 ),	/* 0 */
/* 200 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter bstrFullQDocPath */

/* 202 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 204 */	NdrFcShort( 0x8 ),	/* ia64 Stack size/offset = 8 */
/* 206 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter pbstrXMLPropString */

/* 208 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
/* 210 */	NdrFcShort( 0x10 ),	/* ia64 Stack size/offset = 16 */
/* 212 */	NdrFcShort( 0x2e ),	/* Type Offset=46 */

	/* Return value */

/* 214 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 216 */	NdrFcShort( 0x18 ),	/* ia64 Stack size/offset = 24 */
/* 218 */	0x8,		/* FC_LONG */
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
			0x11, 0x4,	/* FC_RP [alloced_on_stack] */
/* 40 */	NdrFcShort( 0x6 ),	/* Offset= 6 (46) */
/* 42 */	
			0x13, 0x0,	/* FC_OP */
/* 44 */	NdrFcShort( 0xffffffe6 ),	/* Offset= -26 (18) */
/* 46 */	0xb4,		/* FC_USER_MARSHAL */
			0x83,		/* 131 */
/* 48 */	NdrFcShort( 0x0 ),	/* 0 */
/* 50 */	NdrFcShort( 0x8 ),	/* 8 */
/* 52 */	NdrFcShort( 0x0 ),	/* 0 */
/* 54 */	NdrFcShort( 0xfffffff4 ),	/* Offset= -12 (42) */

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


/* Object interface: IAMTSSimpleProp, ver. 0.0,
   GUID={0xE4CA377E,0x7A32,0x49C5,{0xB0,0xCB,0x00,0x3A,0xBE,0xF4,0x24,0x39}} */

#pragma code_seg(".orpc")
static const unsigned short IAMTSSimpleProp_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    0,
    44,
    88,
    132,
    176
    };

static const MIDL_STUBLESS_PROXY_INFO IAMTSSimpleProp_ProxyInfo =
    {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &IAMTSSimpleProp_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO IAMTSSimpleProp_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &IAMTSSimpleProp_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(12) _IAMTSSimplePropProxyVtbl = 
{
    &IAMTSSimpleProp_ProxyInfo,
    &IID_IAMTSSimpleProp,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfoCount */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfo */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    (void *) (INT_PTR) -1 /* IAMTSSimpleProp::ViewSummaryInfo */ ,
    (void *) (INT_PTR) -1 /* IAMTSSimpleProp::ViewDocSummaryInfo */ ,
    (void *) (INT_PTR) -1 /* IAMTSSimpleProp::ViewMFSummaryInfo */ ,
    (void *) (INT_PTR) -1 /* IAMTSSimpleProp::ViewUserDefinedProperties */ ,
    (void *) (INT_PTR) -1 /* IAMTSSimpleProp::ViewHyperlinks */
};


static const PRPC_STUB_FUNCTION IAMTSSimpleProp_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl _IAMTSSimplePropStubVtbl =
{
    &IID_IAMTSSimpleProp,
    &IAMTSSimpleProp_ServerInfo,
    12,
    &IAMTSSimpleProp_table[-3],
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

const CInterfaceProxyVtbl * _MSDocProp_ProxyVtblList[] = 
{
    ( CInterfaceProxyVtbl *) &_IAMTSSimplePropProxyVtbl,
    0
};

const CInterfaceStubVtbl * _MSDocProp_StubVtblList[] = 
{
    ( CInterfaceStubVtbl *) &_IAMTSSimplePropStubVtbl,
    0
};

PCInterfaceName const _MSDocProp_InterfaceNamesList[] = 
{
    "IAMTSSimpleProp",
    0
};

const IID *  _MSDocProp_BaseIIDList[] = 
{
    &IID_IDispatch,
    0
};


#define _MSDocProp_CHECK_IID(n)	IID_GENERIC_CHECK_IID( _MSDocProp, pIID, n)

int __stdcall _MSDocProp_IID_Lookup( const IID * pIID, int * pIndex )
{
    
    if(!_MSDocProp_CHECK_IID(0))
        {
        *pIndex = 0;
        return 1;
        }

    return 0;
}

const ExtendedProxyFileInfo MSDocProp_ProxyFileInfo = 
{
    (PCInterfaceProxyVtblList *) & _MSDocProp_ProxyVtblList,
    (PCInterfaceStubVtblList *) & _MSDocProp_StubVtblList,
    (const PCInterfaceName * ) & _MSDocProp_InterfaceNamesList,
    (const IID ** ) & _MSDocProp_BaseIIDList,
    & _MSDocProp_IID_Lookup, 
    1,
    2,
    0, /* table of [async_uuid] interfaces */
    0, /* Filler1 */
    0, /* Filler2 */
    0  /* Filler3 */
};


#endif /* defined(_M_IA64) || defined(_M_AMD64)*/

