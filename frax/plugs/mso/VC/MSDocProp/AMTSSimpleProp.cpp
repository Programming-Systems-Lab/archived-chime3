// AMTSSimpleProp.cpp : Implementation of AMTSSimpleProp
#include "stdafx.h"
#include "MSDocProp.h"
#include "AMTSSimpleProp.h"
#include <PropConstants.h>

/////////////////////////////////////////////////////////////////////////////
// AMTSSimpleProp

STDMETHODIMP AMTSSimpleProp::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IAMTSSimpleProp
	};
	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (::InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}

HRESULT AMTSSimpleProp::DumpDocSummaryInfo( IPropertySetStorage* pPropSetStg, AString& szData )
{
	if( !pPropSetStg )
		return E_INVALIDARG;
	
	IPropertyStorage *pPropStg = NULL;
    HRESULT hr = E_FAIL;

    // Open Doc summary information, 
    hr = pPropSetStg->Open( FMTID_DocSummaryInformation,
							STGM_READ | STGM_SHARE_EXCLUSIVE, 
							&pPropStg );
    if( FAILED( hr ) ) 
	{
		m_EvtLogger.ReportError( __FILE__, __LINE__, "No Document Summary Info Available" );
        return E_FAIL;
    }
	
	// Initialize PROPSPEC for the properties you want.
    PROPSPEC *pPropSpec = new PROPSPEC [DOC_SUMMARY_PROP_COUNT];
    PROPVARIANT *pPropVar = new PROPVARIANT [DOC_SUMMARY_PROP_COUNT];

    for(int i = 0; i < DOC_SUMMARY_PROP_COUNT; i++) 
	{
        ZeroMemory( &pPropSpec[i], sizeof(PROPSPEC) );
        pPropSpec[i].ulKind = PRSPEC_PROPID;
        pPropSpec[i].propid = DocSummaryInfo[i].pidsi;
    }
	
	hr = pPropStg->ReadMultiple( DOC_SUMMARY_PROP_COUNT, pPropSpec, pPropVar);

    if( FAILED (hr ) ) 
	{
        m_EvtLogger.ReportError( __FILE__, __LINE__, "Error reading multiple properties" );
		goto EXIT_DUMP_DOC_SUMMARY_INFO;
    }
    else 
	{
        //build string to return
		szData = "";
		szData += DOC_SUMMARY_INFO_XML_START_TAG;

		// Dump properties.
		for(i = 0; i < DOC_SUMMARY_PROP_COUNT; i++) 
		{
			szData += DocSummaryInfo[i].m_szXMLStartTag;
				szData += DumpPropVariant(pPropVar + i);
			szData += DocSummaryInfo[i].m_szXMLEndTag;
		}
			
		szData += DOC_SUMMARY_INFO_XML_END_TAG;
    }
	
EXIT_DUMP_DOC_SUMMARY_INFO:
	
	// De-allocate memory.
	if( pPropVar )
		delete [] pPropVar;
	pPropVar = NULL;

	if( pPropSpec )
		delete [] pPropSpec;
	pPropSpec = NULL;

	// Release obtained interface.
	if( pPropStg )
		pPropStg->Release();
	pPropStg = NULL;

	return hr;
}

HRESULT AMTSSimpleProp::DumpHyperlinks( IPropertySetStorage* pPropSetStg, AString& szData )
{
	if( !pPropSetStg )
		return E_INVALIDARG;
	
	IPropertyStorage *pPropStg = NULL;
    HRESULT hr = E_FAIL;

    // Open Media summary information, 
    hr = pPropSetStg->Open( FMTID_UserDefinedProperties,
							STGM_READ | STGM_SHARE_EXCLUSIVE,
							&pPropStg );
    if( FAILED( hr ) ) 
	{
		m_EvtLogger.ReportError( __FILE__, __LINE__, "No User Defined Properties Available" );
        return E_FAIL;
    }
	
	// Initialize PROPSPEC for the properties you want.
    PROPSPEC *pPropSpec = new PROPSPEC [HYPERLINK_PROP_COUNT];
    PROPVARIANT *pPropVar = new PROPVARIANT [HYPERLINK_PROP_COUNT];

    for(int i = 0; i < HYPERLINK_PROP_COUNT; i++) 
	{
        ZeroMemory( &pPropSpec[i], sizeof(PROPSPEC) );
        pPropSpec[i].ulKind = PRSPEC_PROPID;
        pPropSpec[i].propid = HyperlinksInfo[i].pidsi;
    }
	
	hr = pPropStg->ReadMultiple( HYPERLINK_PROP_COUNT, pPropSpec, pPropVar);

    if( FAILED (hr ) ) 
	{
        m_EvtLogger.ReportError( __FILE__, __LINE__, "Error reading multiple properties" );
		goto EXIT_DUMP_HLINK_INFO;
    }
    else 
	{
        //build string to return
		szData = "";
		szData += HLINKS_INFO_XML_START_TAG;

		// Dump properties.
		for(i = 0; i < HYPERLINK_PROP_COUNT; i++) 
		{
			szData += HyperlinksInfo[i].m_szXMLStartTag;
			if( pPropVar[i].vt != VT_BLOB )
			{
				szData += DumpPropVariant( &pPropVar[i] );
			}
			else szData += GetDataFromBLOB( &pPropVar[i] );

			szData += HyperlinksInfo[i].m_szXMLEndTag;
		}
			
		szData += HLINKS_INFO_XML_END_TAG;
    }
	
EXIT_DUMP_HLINK_INFO:
	
	// De-allocate memory.
	if( pPropVar )
		delete [] pPropVar;
	pPropVar = NULL;

	if( pPropSpec )
		delete [] pPropSpec;
	pPropSpec = NULL;

	// Release obtained interface.
	if( pPropStg )
		pPropStg->Release();
	pPropStg = NULL;

	return hr;
}

HRESULT AMTSSimpleProp::DumpMediaFileSummaryInfo( IPropertySetStorage* pPropSetStg, AString& szData )
{
	if( !pPropSetStg )
		return E_INVALIDARG;
	
	IPropertyStorage *pPropStg = NULL;
    HRESULT hr = E_FAIL;

    // Open Media summary information, 
    hr = pPropSetStg->Open( FMTID_MediaFileSummaryInformation,
							STGM_READ | STGM_SHARE_EXCLUSIVE,
							&pPropStg );
    if( FAILED( hr ) ) 
	{
		m_EvtLogger.ReportError( __FILE__, __LINE__, "No Media File Summary Info Available" );
        return E_FAIL;
    }
	
	// Initialize PROPSPEC for the properties you want.
    PROPSPEC *pPropSpec = new PROPSPEC [MF_SUMMARY_PROP_COUNT];
    PROPVARIANT *pPropVar = new PROPVARIANT [MF_SUMMARY_PROP_COUNT];

    for(int i = 0; i < MF_SUMMARY_PROP_COUNT; i++) 
	{
        ZeroMemory( &pPropSpec[i], sizeof(PROPSPEC) );
        pPropSpec[i].ulKind = PRSPEC_PROPID;
        pPropSpec[i].propid = MediaFileSummaryInfo[i].pidsi;
    }
	
	hr = pPropStg->ReadMultiple( MF_SUMMARY_PROP_COUNT, pPropSpec, pPropVar);

    if( FAILED (hr ) ) 
	{
        m_EvtLogger.ReportError( __FILE__, __LINE__, "Error reading multiple properties" );
		goto EXIT_DUMP_MF_SUMMARY_INFO;
    }
    else 
	{
        //build string to return
		szData = "";
		szData += MF_SUMMARY_INFO_XML_START_TAG;

		// Dump properties.
		for(i = 0; i < MF_SUMMARY_PROP_COUNT; i++) 
		{
			szData += MediaFileSummaryInfo[i].m_szXMLStartTag;
				szData += DumpPropVariant(pPropVar + i);
			szData += MediaFileSummaryInfo[i].m_szXMLEndTag;
		}
			
		szData += MF_SUMMARY_INFO_XML_END_TAG;
    }
	
EXIT_DUMP_MF_SUMMARY_INFO:
	
	// De-allocate memory.
	if( pPropVar )
		delete [] pPropVar;
	pPropVar = NULL;

	if( pPropSpec )
		delete [] pPropSpec;
	pPropSpec = NULL;

	// Release obtained interface.
	if( pPropStg )
		pPropStg->Release();
	pPropStg = NULL;

	return hr;
}

// Dumps simple PROPVARIANT values.
AString AMTSSimpleProp::DumpPropVariant( PROPVARIANT *pPropVar ) 
{
	AString szMsg( "" );
	// Don't iterate arrays, just inform as an array.
    if(pPropVar->vt & VT_ARRAY) 
		return AString( "Array of values" );
		  
	// Don't handle byref for simplicity, just inform byref.
	if(pPropVar->vt & VT_BYREF) 
		return AString( "ByRef" );
     

	  // Switch types.
	  switch(pPropVar->vt) 
	  {
		case VT_EMPTY: 	return AString( "" );
						break;
						
		case VT_NULL: return AString( "" );
						break;
		
		case VT_BLOB: return AString( "(BLOB)" );
						break;
		
		case VT_BOOL:	if( pPropVar->boolVal == VARIANT_TRUE )
							szMsg = AString( "TRUE" );
						else szMsg = AString( "FALSE" );
			
						return szMsg;
						break;

		case VT_I2: // 2-byte signed int.
					return AString( (int) pPropVar->iVal );
					break;

		case VT_I4: // 4-byte signed int.
					return AString( (int) pPropVar->lVal );
					break;

		case VT_R4: // 4-byte real.
					return AString( (double) pPropVar->fltVal );
					break;
		
		case VT_R8: // 8-byte real.
					return AString( (double) pPropVar->dblVal );
					break;

		 case VT_BSTR: // OLE Automation string.
						return AString( (char*) _bstr_t( pPropVar->bstrVal  ) ); 
						break;

		case VT_LPSTR: // Null-terminated string.
						return AString( pPropVar->pszVal );
						break;
    
		case VT_LPWSTR: // Null terminated wide string
						return AString( (char*) _bstr_t(pPropVar->pwszVal) );
						break;

		case VT_FILETIME:
		{
			
			// return low 32 bits of date time
			AString szMsg( pPropVar->filetime.dwLowDateTime );
			
			/*char *dayPre[] =
						 {"Sun","Mon","Tue","Wed","Thu","Fri","Sat"};

			FILETIME lft;
			FileTimeToLocalFileTime(&pPropVar->filetime, &lft);                SYSTEMTIME lst;
			FileTimeToSystemTime(&lft, &lst);

			szMsg = AString( 1 + (lst.wHour-1) % 12 );
			szMsg += ":";
			if ( lst.wMinute < 10 )
				szMsg += "0";
			szMsg += AString( lst.wMinute );
			szMsg += ":";
			if ( lst.wSecond < 10 )
				szMsg += "0";
			szMsg += AString( lst.wSecond );
			szMsg += " ";
			(lst.wHour>=12) ? szMsg += AString( "pm" ) : szMsg += AString( "am" );
			szMsg += " ";

			szMsg += AString( dayPre[lst.wDayOfWeek%7] );
			szMsg += " ";

			if ( lst.wMonth< 10 )
				szMsg += "0";
			szMsg += AString( lst.wMonth );
			szMsg += "-";
			if ( lst.wDay< 10 )
				szMsg += "0";
			szMsg += AString( lst.wDay );
			szMsg += "-";
			szMsg += AString( lst.wYear );
			*/
			return szMsg;
		}
		break;
		
		case VT_CF: // Clipboard format.
					return AString( "(Clipboard format)" );
					break;

		default: // Unhandled type, consult wtypes.h's VARENUM structure.
					return AString( "(Unhandled type)" );
					break;
		}

	return szMsg;
}

HRESULT AMTSSimpleProp::DumpSummaryInfo( IPropertySetStorage *pPropSetStg, AString& szData )
{
	if( !pPropSetStg )
		return E_INVALIDARG;
	
	IPropertyStorage *pPropStg = NULL;
    HRESULT hr = E_FAIL;

    // Open summary information, 
    hr = pPropSetStg->Open( FMTID_SummaryInformation,
							STGM_READ | STGM_SHARE_EXCLUSIVE, 
							&pPropStg );
    if( FAILED( hr ) ) 
	{
		m_EvtLogger.ReportError( __FILE__, __LINE__, "No Summary Info Available" );
        return E_FAIL;
    }
	
	// Initialize PROPSPEC for the properties you want.
    PROPSPEC *pPropSpec = new PROPSPEC [SUMMARY_PROP_COUNT];
    PROPVARIANT *pPropVar = new PROPVARIANT [SUMMARY_PROP_COUNT];

    for(int i = 0; i < SUMMARY_PROP_COUNT; i++) 
	{
        ZeroMemory( &pPropSpec[i], sizeof(PROPSPEC) );
        pPropSpec[i].ulKind = PRSPEC_PROPID;
        pPropSpec[i].propid = SummaryInfo[i].pidsi;
    }
	
	hr = pPropStg->ReadMultiple( SUMMARY_PROP_COUNT, pPropSpec, pPropVar);

    if( FAILED (hr ) ) 
	{
        m_EvtLogger.ReportError( __FILE__, __LINE__, "Error reading multiple properties" );
		goto EXIT_DUMP_SUMMARY_INFO;
    }
    else 
	{
        //build string to return
		szData = "";
		szData += SUMMARY_INFO_XML_START_TAG;

		// Dump properties.
		for(i = 0; i < SUMMARY_PROP_COUNT; i++) 
		{
			szData += SummaryInfo[i].m_szXMLStartTag;
				szData += DumpPropVariant(pPropVar + i);
			szData += SummaryInfo[i].m_szXMLEndTag;
		}
			
		szData += SUMMARY_INFO_XML_END_TAG;
    }
	
EXIT_DUMP_SUMMARY_INFO:
	
	// De-allocate memory.
	if( pPropVar )
		delete [] pPropVar;
	pPropVar = NULL;

	if( pPropSpec )
		delete [] pPropSpec;
	pPropSpec = NULL;

	// Release obtained interface.
	if( pPropStg )
		pPropStg->Release();
	pPropStg = NULL;

	return hr;
}

HRESULT AMTSSimpleProp::DumpUserDefinedProperties( IPropertySetStorage* pPropSetStg, AString& szData )
{
	if( !pPropSetStg )
		return E_INVALIDARG;
	
	IPropertyStorage *pPropStg = NULL;
    IEnumSTATPROPSTG *pEnumProp = NULL;
	HRESULT hr = E_FAIL;
	STATPROPSTG sps;
	ULONG uFetched = 0;
	PROPSPEC propSpec[1];
	PROPVARIANT propVar[1];

	// Open User Defined properties 
    hr = pPropSetStg->Open( FMTID_UserDefinedProperties,
							STGM_READ | STGM_SHARE_EXCLUSIVE, 
							&pPropStg );
    if( FAILED( hr ) ) 
	{
		m_EvtLogger.ReportError( __FILE__, __LINE__, "No User Defined Properties Available" );
        return E_FAIL;
    }
		
	// Get property enumerator
	hr = pPropStg->Enum( &pEnumProp );
	if( FAILED( hr ) )
	{
		m_EvtLogger.ReportError( __FILE__, __LINE__, "Error Enumerating Properties" );
        goto EXIT_DUMP_USER_DEF_PROP;
    }

	szData = USED_DEF_PROPS_XML_START_TAG; //open XML tag

	// while we can still fetch properties
	while( pEnumProp->Next( 1, &sps, &uFetched ) == S_OK )
	{
		// Build a PROPSPEC for this property
		ZeroMemory( &propSpec[0], sizeof(PROPSPEC) );
		propSpec[0].ulKind = PRSPEC_PROPID;
		propSpec[0].propid = sps.propid;

		// read this property
		hr = pPropStg->ReadMultiple( 1, &propSpec[0], &propVar[0] );
		if( SUCCEEDED( hr ) )
		{
			AString szPropName( "" );
			AString szXMLStartTag( "" );
			AString szXMLEndTag( "" );
			szPropName = (char*) _bstr_t( sps.lpwstrName );

			if( szPropName.GetLength() > 0 )
			{
				// build XML tags and get value
				szXMLStartTag += "<";
				szXMLStartTag += szPropName;
				szXMLStartTag += ">";

				szXMLEndTag += "</";
				szXMLEndTag += szPropName;
				szXMLEndTag += ">";

				szData += szXMLStartTag;
					szData += DumpPropVariant( &propVar[0] );
				szData += szXMLEndTag;
			}
		}
		else
		{
			// exit
			m_EvtLogger.ReportError( __FILE__, __LINE__, "Error Reading Properties" );
			goto EXIT_DUMP_USER_DEF_PROP;
		}
	}

EXIT_DUMP_USER_DEF_PROP:
	
	szData += USED_DEF_PROPS_XML_END_TAG; // close XML tag

	// Release obtained interface.
	if( pEnumProp )
		pEnumProp->Release();
	pEnumProp = NULL;

	if( pPropStg )
		pPropStg->Release();
	pPropStg = NULL;
	
	return hr;
}

AString AMTSSimpleProp::GetDataFromBLOB( PROPVARIANT* pPropVar )
{
	/*CAPROPVARIANT v;
	BLOB b;

	int nBytesToCopy = pPropVar->blob.cbSize;
	int nOffsetBytes = sizeof(ULONG);
	v.cElems = 0;
	v.pElems = NULL;

	// find out how many propvariants we need
	memcpy( &v.cElems, pPropVar->blob.pBlobData, sizeof(ULONG) );
	v.pElems = new PROPVARIANT[v.cElems];

	//copy byte array over to propvariant array pElems
	memcpy( v.pElems, pPropVar->blob.pBlobData + sizeof(ULONG), ( nBytesToCopy - sizeof(ULONG) ) );

	/*if( v.pElems[0].vt == VT_I4 )
	{
		int x = 0;
		x++;
	}*/

	return AString( "" );
}

STDMETHODIMP AMTSSimpleProp::ViewDocSummaryInfo(BSTR bstrFullQDocPath, BSTR *pbstrXMLPropString)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	
	//store doc path and name
	_bstr_t bstrtDoc( bstrFullQDocPath );
	
	if( bstrtDoc.length() == 0 )
		return E_INVALIDARG;

	//COM classes used to get property info
	IStorage* pStorage = NULL;
	IPropertySetStorage *pPropSetStg = NULL;
	
	HRESULT hr = E_FAIL;
	AString szProperties( "" );

	/*hr = ::StgOpenStorage( bstrtDoc, //path to file
							NULL,  
							STGM_READ | STGM_SHARE_EXCLUSIVE, //access mode
							NULL,
							0, //reserved must be 0
							&pStorage ); //pointer to storage object
	*/

	hr = StgOpenStorageEx( bstrtDoc,
						   STGM_READ | STGM_SHARE_EXCLUSIVE, //access mode
						   STGFMT_ANY,
						   0,
						   0,
						   0,
						   IID_IPropertySetStorage,
						   reinterpret_cast<void**>(&pStorage) );
							 
							 

	if( FAILED( hr ) )
	{
		//report error
		AString szError( "Error opening Storage Object: " );
		
		switch( hr )
		{
			case STG_E_FILENOTFOUND: szError += "File not found";
										break;

			case STG_E_ACCESSDENIED: szError += "Access Denied"; 
										break;
			
			case STG_E_LOCKVIOLATION: szError += "File locked";
										break;
	
			case STG_E_SHAREVIOLATION: szError += "Sharing Violoation";
										break;
			
			case STG_E_FILEALREADYEXISTS: szError += "File exists but is not a compound document";
											break;

			case STG_E_PATHNOTFOUND: szError += "Path not found";
										break;

			case STG_E_INVALIDNAME: szError += "Invalid filename";
										break;

			default: szError += "Unexpected Error";
								break;
		}

		szError += " filename: ";
		szError += (char*) bstrtDoc;
		m_EvtLogger.ReportError( __FILE__, __LINE__, szError );
		
		goto EXIT_VIEW_DOC_SUMMARY_INFO;
	}

	// Obtain the IPropertySetStorage interface.
    hr = pStorage->QueryInterface( IID_IPropertySetStorage, reinterpret_cast<void**>(&pPropSetStg) );
    if( FAILED( hr ) ) 
	{
        //report error
		m_EvtLogger.ReportError( __FILE__, __LINE__, "Error obtaining IPropertyStorage interface" );
		goto EXIT_VIEW_DOC_SUMMARY_INFO;
    }

	try
	{
			hr = DumpDocSummaryInfo( pPropSetStg, szProperties );

			if( SUCCEEDED( hr ) )
				*pbstrXMLPropString = szProperties.AllocSysString();
	}
	catch( CMemoryException& e )
	{
		TCHAR szMsg [255];
		e.GetErrorMessage( szMsg, 255 );
		AString szError( szMsg );
		m_EvtLogger.ReportError( __FILE__, __LINE__, szError );
		hr = E_OUTOFMEMORY;
		goto EXIT_VIEW_DOC_SUMMARY_INFO;
	}
	catch( ... )
	{
		m_EvtLogger.ReportError( __FILE__, __LINE__, "Unknown Error" );
		hr = E_FAIL;
		goto EXIT_VIEW_DOC_SUMMARY_INFO;
	}
	

EXIT_VIEW_DOC_SUMMARY_INFO:
	if( pPropSetStg )
		pPropSetStg->Release();

	pPropSetStg = NULL;
	 
	if( pStorage )
		pStorage->Release();

	pStorage = NULL;

	return hr;
}

STDMETHODIMP AMTSSimpleProp::ViewHyperlinks(BSTR bstrFullQDocPath, BSTR *pbstrXMLPropString)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	
		//store doc path and name
	_bstr_t bstrtDoc( bstrFullQDocPath );
	
	if( bstrtDoc.length() == 0 )
		return E_INVALIDARG;

	//COM classes used to get property info
	IStorage* pStorage = NULL;
	IPropertySetStorage *pPropSetStg = NULL;
	
	HRESULT hr = E_FAIL;
	AString szProperties( "" );

	/*hr = ::StgOpenStorage( bstrtDoc, //path to file
							NULL,  
							STGM_READ | STGM_SHARE_EXCLUSIVE, //access mode
							NULL,
							0, //reserved must be 0
							&pStorage ); //pointer to storage object
	*/

	hr = StgOpenStorageEx( bstrtDoc,
						   STGM_READ | STGM_SHARE_EXCLUSIVE, //access mode
						   STGFMT_ANY,
						   0,
						   0,
						   0,
						   IID_IPropertySetStorage,
						   reinterpret_cast<void**>(&pStorage) );
							 
							 

	if( FAILED( hr ) )
	{
		//report error
		AString szError( "Error opening Storage Object: " );
		
		switch( hr )
		{
			case STG_E_FILENOTFOUND: szError += "File not found";
										break;

			case STG_E_ACCESSDENIED: szError += "Access Denied"; 
										break;
			
			case STG_E_LOCKVIOLATION: szError += "File locked";
										break;
	
			case STG_E_SHAREVIOLATION: szError += "Sharing Violoation";
										break;
			
			case STG_E_FILEALREADYEXISTS: szError += "File exists but is not a compound document";
											break;

			case STG_E_PATHNOTFOUND: szError += "Path not found";
										break;

			case STG_E_INVALIDNAME: szError += "Invalid filename";
										break;

			default: szError += "Unexpected Error";
								break;
		}

		szError += " filename: ";
		szError += (char*) bstrtDoc;
		m_EvtLogger.ReportError( __FILE__, __LINE__, szError );
		
		goto EXIT_VIEW_HLINKS;
	}

	// Obtain the IPropertySetStorage interface.
    hr = pStorage->QueryInterface( IID_IPropertySetStorage, reinterpret_cast<void**>(&pPropSetStg) );
    if( FAILED( hr ) ) 
	{
        //report error
		m_EvtLogger.ReportError( __FILE__, __LINE__, "Error obtaining IPropertyStorage interface" );
		goto EXIT_VIEW_HLINKS;
    }

	try
	{
			hr = DumpHyperlinks( pPropSetStg, szProperties );

			if( SUCCEEDED( hr ) )
				*pbstrXMLPropString = szProperties.AllocSysString();
	}
	catch( CMemoryException& e )
	{
		TCHAR szMsg [255];
		e.GetErrorMessage( szMsg, 255 );
		AString szError( szMsg );
		m_EvtLogger.ReportError( __FILE__, __LINE__, szError );
		hr = E_OUTOFMEMORY;
		goto EXIT_VIEW_HLINKS;
	}
	catch( ... )
	{
		m_EvtLogger.ReportError( __FILE__, __LINE__, "Unknown Error" );
		hr = E_FAIL;
		goto EXIT_VIEW_HLINKS;
	}

EXIT_VIEW_HLINKS:
	if( pPropSetStg )
		pPropSetStg->Release();

	pPropSetStg = NULL;
	 
	if( pStorage )
		pStorage->Release();

	pStorage = NULL;

	return hr;
}


// Fails to open storage
STDMETHODIMP AMTSSimpleProp::ViewMFSummaryInfo(BSTR bstrFullQDocPath, BSTR *pbstrXMLPropString)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	
	return S_FALSE; //just exit, throw no exception

		
	//store doc path and name
	_bstr_t bstrtDoc( bstrFullQDocPath );

	if( bstrtDoc.length() == 0 )
		return E_INVALIDARG;

	//COM classes used to get property info
	IStorage* pStorage = NULL;
	IPropertySetStorage *pPropSetStg = NULL;
    HRESULT hr = E_FAIL;
	AString szProperties( "" );

	/*hr = ::StgOpenStorage( bstrtDoc, //path to file
							NULL,  
							STGM_READ | STGM_SHARE_EXCLUSIVE, //access mode
							NULL,
							0, //reserved must be 0
							&pStorage ); //pointer to storage object
	*/

	hr = StgOpenStorageEx( bstrtDoc,
						   STGM_READ | STGM_SHARE_EXCLUSIVE, //access mode
						   STGFMT_ANY,
						   0,
						   0,
						   0,
						   IID_IPropertySetStorage,
						   reinterpret_cast<void**>(&pStorage) );
	if( FAILED( hr ) )
	{
		//report error
		AString szError( "Error opening Storage Object: " );
		
		switch( hr )
		{
			case STG_E_FILENOTFOUND: szError += "File not found";
										break;

			case STG_E_ACCESSDENIED: szError += "Access Denied"; 
										break;
			
			case STG_E_LOCKVIOLATION: szError += "File locked";
										break;
	
			case STG_E_SHAREVIOLATION: szError += "Sharing Violoation";
										break;
			
			case STG_E_FILEALREADYEXISTS: szError += "File exists but is not a compound document";
											break;

			case STG_E_PATHNOTFOUND: szError += "Path not found";
										break;

			case STG_E_INVALIDNAME: szError += "Invalid filename";
										break;

			default: szError += "Unexpected Error";
								break;
		}

		szError += " filename: ";
		szError += (char*) bstrtDoc;
		m_EvtLogger.ReportError( __FILE__, __LINE__, szError );
		
		goto EXIT_VIEW_MF_SUMMARY_INFO;
	}

	// Obtain the IPropertySetStorage interface.
    hr = pStorage->QueryInterface( IID_IPropertySetStorage, reinterpret_cast<void**>(&pPropSetStg) );
    if( FAILED( hr ) ) 
	{
        //report error
		m_EvtLogger.ReportError( __FILE__, __LINE__, "Error obtaining IPropertyStorage interface" );
		goto EXIT_VIEW_MF_SUMMARY_INFO;
    }

	try
	{
			hr = DumpMediaFileSummaryInfo( pPropSetStg, szProperties );

			if( SUCCEEDED( hr ) )
				*pbstrXMLPropString = szProperties.AllocSysString();
	}
	catch( CMemoryException& e )
	{
		TCHAR szMsg [255];
		e.GetErrorMessage( szMsg, 255 );
		AString szError( szMsg );
		m_EvtLogger.ReportError( __FILE__, __LINE__, szError );
		hr = E_OUTOFMEMORY;
		goto EXIT_VIEW_MF_SUMMARY_INFO;
	}
	catch( ... )
	{
		m_EvtLogger.ReportError( __FILE__, __LINE__, "Unknown Error" );
		hr = E_FAIL;
		goto EXIT_VIEW_MF_SUMMARY_INFO;
	}
	
EXIT_VIEW_MF_SUMMARY_INFO:
	if( pPropSetStg )
		pPropSetStg->Release();

	pPropSetStg = NULL;
	 
	if( pStorage )
		pStorage->Release();

	pStorage = NULL;

	return hr;
}


STDMETHODIMP AMTSSimpleProp::ViewSummaryInfo( BSTR bstrFullQDocPath, BSTR* pbstrXMLPropString )
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	//store doc path and name
	_bstr_t bstrtDoc( bstrFullQDocPath );

	if( bstrtDoc.length() == 0 )
		return E_INVALIDARG;

	//COM classes used to get property info
	IStorage* pStorage = NULL;
	IPropertySetStorage *pPropSetStg = NULL;
    HRESULT hr = E_FAIL;
	AString szProperties( "" );

	/*hr = ::StgOpenStorage( bstrtDoc, //path to file
							NULL,  
							STGM_READ | STGM_SHARE_EXCLUSIVE, //access mode
							NULL,
							0, //reserved must be 0
							&pStorage ); //pointer to storage object
	*/

	hr = StgOpenStorageEx( bstrtDoc,
						   STGM_READ | STGM_SHARE_EXCLUSIVE, //access mode
						   STGFMT_ANY,
						   0,
						   0,
						   0,
						   IID_IPropertySetStorage,
						   reinterpret_cast<void**>(&pStorage) );
	if( FAILED( hr ) )
	{
		//report error
		AString szError( "Error opening Storage Object: " );
		
		switch( hr )
		{
			case STG_E_FILENOTFOUND: szError += "File not found";
										break;

			case STG_E_ACCESSDENIED: szError += "Access Denied"; 
										break;
			
			case STG_E_LOCKVIOLATION: szError += "File locked";
										break;
	
			case STG_E_SHAREVIOLATION: szError += "Sharing Violoation";
										break;
			
			case STG_E_FILEALREADYEXISTS: szError += "File exists but is not a compound document";
											break;

			case STG_E_PATHNOTFOUND: szError += "Path not found";
										break;

			case STG_E_INVALIDNAME: szError += "Invalid filename";
										break;

			default: szError += "Unexpected Error";
								break;
		}

		szError += " filename: ";
		szError += (char*) bstrtDoc;
		m_EvtLogger.ReportError( __FILE__, __LINE__, szError );
		
		goto EXIT_VIEW_SUMMARY_INFO;
	}

	// Obtain the IPropertySetStorage interface.
    hr = pStorage->QueryInterface( IID_IPropertySetStorage, reinterpret_cast<void**>(&pPropSetStg) );
    if( FAILED( hr ) ) 
	{
        //report error
		m_EvtLogger.ReportError( __FILE__, __LINE__, "Error obtaining IPropertyStorage interface" );
		goto EXIT_VIEW_SUMMARY_INFO;
    }

	try
	{
			hr = DumpSummaryInfo( pPropSetStg, szProperties );

			if( SUCCEEDED( hr ) )
				*pbstrXMLPropString = szProperties.AllocSysString();
	}
	catch( CMemoryException& e )
	{
		TCHAR szMsg [255];
		e.GetErrorMessage( szMsg, 255 );
		AString szError( szMsg );
		m_EvtLogger.ReportError( __FILE__, __LINE__, szError );
		hr = E_OUTOFMEMORY;
		goto EXIT_VIEW_SUMMARY_INFO;
	}
	catch( ... )
	{
		m_EvtLogger.ReportError( __FILE__, __LINE__, "Unknown Error" );
		hr = E_FAIL;
		goto EXIT_VIEW_SUMMARY_INFO;
	}
	
EXIT_VIEW_SUMMARY_INFO:
	if( pPropSetStg )
		pPropSetStg->Release();

	pPropSetStg = NULL;
	 
	if( pStorage )
		pStorage->Release();

	pStorage = NULL;

	return hr;
}

STDMETHODIMP AMTSSimpleProp::ViewUserDefinedProperties(BSTR bstrFullQDocPath, BSTR *pbstrXMLPropString)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	
		//store doc path and name
	_bstr_t bstrtDoc( bstrFullQDocPath );

	if( bstrtDoc.length() == 0 )
		return E_INVALIDARG;

	//COM classes used to get property info
	IStorage* pStorage = NULL;
	IPropertySetStorage *pPropSetStg = NULL;
    HRESULT hr = E_FAIL;
	AString szProperties( "" );

	/*hr = ::StgOpenStorage( bstrtDoc, //path to file
							NULL,  
							STGM_READ | STGM_SHARE_EXCLUSIVE, //access mode
							NULL,
							0, //reserved must be 0
							&pStorage ); //pointer to storage object
	*/

	hr = StgOpenStorageEx( bstrtDoc,
						   STGM_READ | STGM_SHARE_EXCLUSIVE, //access mode
						   STGFMT_ANY,
						   0,
						   0,
						   0,
						   IID_IPropertySetStorage,
						   reinterpret_cast<void**>(&pStorage) );
	if( FAILED( hr ) )
	{
		//report error
		AString szError( "Error opening Storage Object: " );
		
		switch( hr )
		{
			case STG_E_FILENOTFOUND: szError += "File not found";
										break;

			case STG_E_ACCESSDENIED: szError += "Access Denied"; 
										break;
			
			case STG_E_LOCKVIOLATION: szError += "File locked";
										break;
	
			case STG_E_SHAREVIOLATION: szError += "Sharing Violoation";
										break;
			
			case STG_E_FILEALREADYEXISTS: szError += "File exists but is not a compound document";
											break;

			case STG_E_PATHNOTFOUND: szError += "Path not found";
										break;

			case STG_E_INVALIDNAME: szError += "Invalid filename";
										break;

			default: szError += "Unexpected Error";
								break;
		}

		szError += " filename: ";
		szError += (char*) bstrtDoc;
		m_EvtLogger.ReportError( __FILE__, __LINE__, szError );
		
		goto EXIT_USER_DEF_PROPS;
	}

	// Obtain the IPropertySetStorage interface.
    hr = pStorage->QueryInterface( IID_IPropertySetStorage, reinterpret_cast<void**>(&pPropSetStg) );
    if( FAILED( hr ) ) 
	{
        //report error
		m_EvtLogger.ReportError( __FILE__, __LINE__, "Error obtaining IPropertyStorage interface" );
		goto EXIT_USER_DEF_PROPS;
    }

	try
	{
			hr = DumpUserDefinedProperties( pPropSetStg, szProperties );

			if( SUCCEEDED( hr ) )
				*pbstrXMLPropString = szProperties.AllocSysString();
	}
	catch( CMemoryException& e )
	{
		TCHAR szMsg [255];
		e.GetErrorMessage( szMsg, 255 );
		AString szError( szMsg );
		m_EvtLogger.ReportError( __FILE__, __LINE__, szError );
		hr = E_OUTOFMEMORY;
		goto EXIT_USER_DEF_PROPS;
	}
	catch( ... )
	{
		m_EvtLogger.ReportError( __FILE__, __LINE__, "Unknown Error" );
		hr = E_FAIL;
		goto EXIT_USER_DEF_PROPS;
	}
	
EXIT_USER_DEF_PROPS:

	if( pPropSetStg )
		pPropSetStg->Release();

	pPropSetStg = NULL;
	 
	if( pStorage )
		pStorage->Release();

	pStorage = NULL;

	return hr;
}

