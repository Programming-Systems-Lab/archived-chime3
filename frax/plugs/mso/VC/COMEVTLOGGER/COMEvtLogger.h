
#pragma warning( disable: 4049 )  /* more than 64k source lines */

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


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


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 440
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __COMEvtLogger_h__
#define __COMEvtLogger_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IAMTSEvtLogger_FWD_DEFINED__
#define __IAMTSEvtLogger_FWD_DEFINED__
typedef interface IAMTSEvtLogger IAMTSEvtLogger;
#endif 	/* __IAMTSEvtLogger_FWD_DEFINED__ */


#ifndef __AMTSEvtLogger_FWD_DEFINED__
#define __AMTSEvtLogger_FWD_DEFINED__

#ifdef __cplusplus
typedef class AMTSEvtLogger AMTSEvtLogger;
#else
typedef struct AMTSEvtLogger AMTSEvtLogger;
#endif /* __cplusplus */

#endif 	/* __AMTSEvtLogger_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 

void * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void * ); 

#ifndef __IAMTSEvtLogger_INTERFACE_DEFINED__
#define __IAMTSEvtLogger_INTERFACE_DEFINED__

/* interface IAMTSEvtLogger */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IAMTSEvtLogger;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("2C4C8D6D-F7A1-472D-942D-E2D4EF0F0846")
    IAMTSEvtLogger : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE AddEventSource( 
            /* [in] */ BSTR bstrSource,
            /* [retval][out] */ VARIANT_BOOL *pStat) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ReportErrorEvent( 
            /* [in] */ BSTR bstrEventData,
            /* [in] */ BSTR bstrEventSource,
            /* [retval][out] */ VARIANT_BOOL *pStat) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ReportInfoEvent( 
            /* [in] */ BSTR bstrEventData,
            /* [in] */ BSTR bstrEventSource,
            /* [retval][out] */ VARIANT_BOOL *pStat) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ReportWarningEvent( 
            /* [in] */ BSTR bstrEventData,
            /* [in] */ BSTR bstrEventSource,
            /* [retval][out] */ VARIANT_BOOL *pStat) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IAMTSEvtLoggerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IAMTSEvtLogger * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IAMTSEvtLogger * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IAMTSEvtLogger * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IAMTSEvtLogger * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IAMTSEvtLogger * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IAMTSEvtLogger * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IAMTSEvtLogger * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *AddEventSource )( 
            IAMTSEvtLogger * This,
            /* [in] */ BSTR bstrSource,
            /* [retval][out] */ VARIANT_BOOL *pStat);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ReportErrorEvent )( 
            IAMTSEvtLogger * This,
            /* [in] */ BSTR bstrEventData,
            /* [in] */ BSTR bstrEventSource,
            /* [retval][out] */ VARIANT_BOOL *pStat);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ReportInfoEvent )( 
            IAMTSEvtLogger * This,
            /* [in] */ BSTR bstrEventData,
            /* [in] */ BSTR bstrEventSource,
            /* [retval][out] */ VARIANT_BOOL *pStat);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ReportWarningEvent )( 
            IAMTSEvtLogger * This,
            /* [in] */ BSTR bstrEventData,
            /* [in] */ BSTR bstrEventSource,
            /* [retval][out] */ VARIANT_BOOL *pStat);
        
        END_INTERFACE
    } IAMTSEvtLoggerVtbl;

    interface IAMTSEvtLogger
    {
        CONST_VTBL struct IAMTSEvtLoggerVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IAMTSEvtLogger_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IAMTSEvtLogger_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IAMTSEvtLogger_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IAMTSEvtLogger_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IAMTSEvtLogger_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IAMTSEvtLogger_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IAMTSEvtLogger_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IAMTSEvtLogger_AddEventSource(This,bstrSource,pStat)	\
    (This)->lpVtbl -> AddEventSource(This,bstrSource,pStat)

#define IAMTSEvtLogger_ReportErrorEvent(This,bstrEventData,bstrEventSource,pStat)	\
    (This)->lpVtbl -> ReportErrorEvent(This,bstrEventData,bstrEventSource,pStat)

#define IAMTSEvtLogger_ReportInfoEvent(This,bstrEventData,bstrEventSource,pStat)	\
    (This)->lpVtbl -> ReportInfoEvent(This,bstrEventData,bstrEventSource,pStat)

#define IAMTSEvtLogger_ReportWarningEvent(This,bstrEventData,bstrEventSource,pStat)	\
    (This)->lpVtbl -> ReportWarningEvent(This,bstrEventData,bstrEventSource,pStat)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IAMTSEvtLogger_AddEventSource_Proxy( 
    IAMTSEvtLogger * This,
    /* [in] */ BSTR bstrSource,
    /* [retval][out] */ VARIANT_BOOL *pStat);


void __RPC_STUB IAMTSEvtLogger_AddEventSource_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IAMTSEvtLogger_ReportErrorEvent_Proxy( 
    IAMTSEvtLogger * This,
    /* [in] */ BSTR bstrEventData,
    /* [in] */ BSTR bstrEventSource,
    /* [retval][out] */ VARIANT_BOOL *pStat);


void __RPC_STUB IAMTSEvtLogger_ReportErrorEvent_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IAMTSEvtLogger_ReportInfoEvent_Proxy( 
    IAMTSEvtLogger * This,
    /* [in] */ BSTR bstrEventData,
    /* [in] */ BSTR bstrEventSource,
    /* [retval][out] */ VARIANT_BOOL *pStat);


void __RPC_STUB IAMTSEvtLogger_ReportInfoEvent_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IAMTSEvtLogger_ReportWarningEvent_Proxy( 
    IAMTSEvtLogger * This,
    /* [in] */ BSTR bstrEventData,
    /* [in] */ BSTR bstrEventSource,
    /* [retval][out] */ VARIANT_BOOL *pStat);


void __RPC_STUB IAMTSEvtLogger_ReportWarningEvent_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IAMTSEvtLogger_INTERFACE_DEFINED__ */



#ifndef __COMEVTLOGGERLib_LIBRARY_DEFINED__
#define __COMEVTLOGGERLib_LIBRARY_DEFINED__

/* library COMEVTLOGGERLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_COMEVTLOGGERLib;

EXTERN_C const CLSID CLSID_AMTSEvtLogger;

#ifdef __cplusplus

class DECLSPEC_UUID("F8C74D83-9A9C-4BD5-9664-256E4836C05B")
AMTSEvtLogger;
#endif
#endif /* __COMEVTLOGGERLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long *, unsigned long            , BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserMarshal(  unsigned long *, unsigned char *, BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserUnmarshal(unsigned long *, unsigned char *, BSTR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long *, BSTR * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


