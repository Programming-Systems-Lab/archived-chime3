// AMTSEvtLogger.h : Declaration of the AMTSEvtLogger

#ifndef __AMTSEVTLOGGER_H_
#define __AMTSEVTLOGGER_H_

#include "resource.h"       // main symbols
#include <comsvcs.h>
#include <Alogger.h>
#include <AString.h>
#include <EvtLogger.h>
#include <COMDEF.h>

const AString DEFAULT_MESSAGING_DLL = "genmsg.dll";

/////////////////////////////////////////////////////////////////////////////
// AMTSEvtLogger
class ATL_NO_VTABLE AMTSEvtLogger : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<AMTSEvtLogger, &CLSID_AMTSEvtLogger>,
	public ISupportErrorInfo,
	public IDispatchImpl<IAMTSEvtLogger, &IID_IAMTSEvtLogger, &LIBID_COMEVTLOGGERLib>,
	public IObjectControl
{
public:
	AMTSEvtLogger()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_AMTSEVTLOGGER)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(AMTSEvtLogger)
	COM_INTERFACE_ENTRY(IAMTSEvtLogger)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()

// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// IObjectControl
public:
	STDMETHOD(Activate)();
	STDMETHOD_(BOOL, CanBePooled)();
	STDMETHOD_(void, Deactivate)();

	CComPtr<IObjectContext> m_spObjectContext;

// IAMTSEvtLogger
public:
	STDMETHOD(ReportWarningEvent)(/*[in]*/ BSTR bstrEventData, /*[in]*/ BSTR bstrEventSource, /*[out,retval]*/ VARIANT_BOOL* pStat);
	STDMETHOD(ReportInfoEvent)(/*[in]*/ BSTR bstrEventData, /*[in]*/ BSTR bstrEventSource, /*[out,retval]*/ VARIANT_BOOL* pStat);
	STDMETHOD(ReportErrorEvent)(/*[in]*/ BSTR bstrEventData, /*[in]*/ BSTR bstrEventSource, /*[out,retval]*/ VARIANT_BOOL* pStat);
	STDMETHOD(AddEventSource)(/*[in]*/ BSTR bstrSource, /*[out,retval]*/ VARIANT_BOOL* pStat);

private:
		
};

#endif //__AMTSEVTLOGGER_H_
