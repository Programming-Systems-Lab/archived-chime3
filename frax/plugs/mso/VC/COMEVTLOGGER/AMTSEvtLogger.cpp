// AMTSEvtLogger.cpp : Implementation of AMTSEvtLogger
#include "stdafx.h"
#include "COMEvtLogger.h"
#include "AMTSEvtLogger.h"
#include <comdef.h>

/////////////////////////////////////////////////////////////////////////////
// AMTSEvtLogger

STDMETHODIMP AMTSEvtLogger::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IAMTSEvtLogger
	};
	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (::InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}

HRESULT AMTSEvtLogger::Activate()
{
	HRESULT hr = GetObjectContext(&m_spObjectContext);
	if (SUCCEEDED(hr))
		return S_OK;
	return hr;
} 

BOOL AMTSEvtLogger::CanBePooled()
{
	return TRUE;
} 

void AMTSEvtLogger::Deactivate()
{
	m_spObjectContext.Release();
}

STDMETHODIMP AMTSEvtLogger::AddEventSource( BSTR bstrSource, VARIANT_BOOL *pStat )
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if( pStat )
		*pStat = VARIANT_FALSE;


	HRESULT hr = E_FAIL;
	AString szSource("");
	szSource = (char*) _bstr_t( bstrSource );

	//if empty source name, return invalid argument E_INVALIDARG
	if( szSource.CompareNoCase( "" ) == 0 )
		return Error( "Invalid Argument for Event Source", __uuidof(AMTSEvtLogger), E_INVALIDARG );

	if( !ALogger::AddEventSource( szSource, DEFAULT_MESSAGING_DLL ) )
		return Error( "Error Adding Event Source", __uuidof(AMTSEvtLogger), E_FAIL );
	else
	{
		if( pStat )
			*pStat = VARIANT_TRUE;
		
		hr = S_OK;
	}

	return hr;
}

STDMETHODIMP AMTSEvtLogger::ReportErrorEvent(BSTR bstrEventData, BSTR bstrEventSource, VARIANT_BOOL *pStat)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if( pStat )
		*pStat = VARIANT_FALSE;

	AString szError( "" );
	AString szSource( "" );
	HRESULT hr = E_FAIL;

	szError = (char*) _bstr_t( bstrEventData );
	szSource = (char*) _bstr_t( bstrEventSource );

	//empty event data and or empty event source not allowed
	if( szError.CompareNoCase( "" ) == 0 || szSource.CompareNoCase( "" ) == 0 )
		return Error( "Invalid Argument: Event Data and/or Source missing", __uuidof(AMTSEvtLogger), E_INVALIDARG );

	if( !ALogger::ReportErrorEvent( szError, szSource ) )
		return Error( "Error Reporting Event", __uuidof(AMTSEvtLogger), E_FAIL );
	else
	{
		if( pStat )
			*pStat = VARIANT_TRUE;

		hr = S_OK;
	}

	return hr;
}

STDMETHODIMP AMTSEvtLogger::ReportInfoEvent(BSTR bstrEventData, BSTR bstrEventSource, VARIANT_BOOL *pStat)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if( pStat )
		*pStat = VARIANT_FALSE;

	AString szInfo( "" );
	AString szSource( "" );
	HRESULT hr = E_FAIL;

	szInfo = (char*) _bstr_t( bstrEventData );
	szSource = (char*) _bstr_t( bstrEventSource );

	//empty event data and or empty event source not allowed
	if( szInfo.CompareNoCase( "" ) == 0 || szSource.CompareNoCase( "" ) == 0 )
		return Error( "Invalid Argument: Event Data and/or Source missing", __uuidof(AMTSEvtLogger), E_INVALIDARG );

	if( !ALogger::ReportInfoEvent( szInfo, szSource ) )
		return Error( "Error Reporting Event", __uuidof(AMTSEvtLogger), E_FAIL );
	else
	{
		if( pStat )
			*pStat = VARIANT_TRUE;

		hr = S_OK;
	}

	return hr;
}

STDMETHODIMP AMTSEvtLogger::ReportWarningEvent(BSTR bstrEventData, BSTR bstrEventSource, VARIANT_BOOL *pStat)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if( pStat )
		*pStat = VARIANT_FALSE;

	AString szWarning( "" );
	AString szSource( "" );
	HRESULT hr = E_FAIL;

	szWarning = (char*) _bstr_t( bstrEventData );
	szSource = (char*) _bstr_t( bstrEventSource );

	//empty event data and or empty event source not allowed
	if( szWarning.CompareNoCase( "" ) == 0 || szSource.CompareNoCase( "" ) == 0 )
		return Error( "Invalid Argument: Event Data and/or Source missing", __uuidof(AMTSEvtLogger), E_INVALIDARG );

	if( !ALogger::ReportWarningEvent( szWarning, szSource ) )
		return Error( "Error Reporting Event", __uuidof(AMTSEvtLogger), E_FAIL );
	else
	{
		if( pStat )
			*pStat = VARIANT_TRUE;

		hr = S_OK;
	}

	return hr;
}
