// AMTSSimpleProp.h : Declaration of the AMTSSimpleProp

#ifndef __AMTSSIMPLEPROP_H_
#define __AMTSSIMPLEPROP_H_

#include "resource.h"       // main symbols
#include <AString.h>
#include <EvtLogger.h>
#include <comsvcs.h>

/////////////////////////////////////////////////////////////////////////////
// AMTSSimpleProp
class ATL_NO_VTABLE AMTSSimpleProp : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<AMTSSimpleProp, &CLSID_AMTSSimpleProp>,
	public ISupportErrorInfo,
	public IDispatchImpl<IAMTSSimpleProp, &IID_IAMTSSimpleProp, &LIBID_MSDOCPROPLib>
{
public:
	AMTSSimpleProp():m_EvtLogger( "AMTSSimpleProp" )
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_AMTSSIMPLEPROP)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(AMTSSimpleProp)
	COM_INTERFACE_ENTRY(IAMTSSimpleProp)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()

// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// IAMTSSimpleProp
public:
	STDMETHOD(ViewHyperlinks)(/*[in]*/ BSTR bstrFullQDocPath, /*[out,retval]*/ BSTR* pbstrXMLPropString);
	STDMETHOD(ViewUserDefinedProperties)(/*[in]*/ BSTR bstrFullQDocPath, /*[out,retval]*/ BSTR* pbstrXMLPropString);
	STDMETHOD(ViewMFSummaryInfo)(/*[in]*/ BSTR bstrFullQDocPath, /*[out,retval]*/ BSTR* pbstrXMLPropString);
	STDMETHOD(ViewDocSummaryInfo)(/*[in]*/ BSTR bstrFullQDocPath, /*[out,retval]*/ BSTR* pbstrXMLPropString);
	STDMETHOD(ViewSummaryInfo)(/*[in]*/ BSTR bstrFullQDocPath, /*[out,retval]*/ BSTR* pbstrXMLPropString);

private:
	EvtLogger m_EvtLogger;
	HRESULT DumpDocSummaryInfo( IPropertySetStorage *pPropSetStg, AString& szData );
	HRESULT DumpMediaFileSummaryInfo( IPropertySetStorage *pPropSetStg, AString& szData );
	HRESULT DumpSummaryInfo( IPropertySetStorage *pPropSetStg, AString& szData );
	HRESULT DumpUserDefinedProperties( IPropertySetStorage *pPropSetStg, AString& szData );
	HRESULT DumpHyperlinks( IPropertySetStorage *pPropSetStg, AString& szData );
	
	AString DumpPropVariant( PROPVARIANT *pPropVar );
	AString GetDataFromBLOB( PROPVARIANT *pPropVar );
};

#endif //__AMTSSIMPLEPROP_H_
