#include <PropConstants.h>
#include "FRAXFilter.h"


FRAXFilter::FRAXFilter():m_EvtLogger( "FRAXFilter" )
{}

FRAXFilter::~FRAXFilter()
{}

AString FRAXFilter::FilterDocSummaryInfoSection( AString szXML )
{
	AString szRetVal( "" );
	VARIANT_BOOL bStatus = VARIANT_FALSE;

	//create XMLDOMDocument
	MSXML2::IXMLDOMDocument* pXMLDOMDoc = NULL;
	
	HRESULT hr = CoCreateInstance( MSXML2::CLSID_DOMDocument,
									NULL,
									CLSCTX_ALL,
									MSXML2::IID_IXMLDOMDocument,
									reinterpret_cast<void**>(&pXMLDOMDoc) );

	if( FAILED( hr ) )
	{
		m_EvtLogger.ReportError( __FILE__, __LINE__, "CoCreateInstance failed" );;
		goto EXIT_FILTER_DOC_SUM_INFO;
	}

	try
	{
		_bstr_t bstrtData( szXML.AllocSysString() );
		bStatus = pXMLDOMDoc->loadXML( bstrtData );
		if( bStatus == VARIANT_FALSE )
		{
			//report error
			m_EvtLogger.ReportError( __FILE__, __LINE__, "Error loading XML" );
			goto EXIT_FILTER_DOC_SUM_INFO;
		}
		
		//start at root of document IP XML start tag
		MSXML2::IXMLDOMElementPtr spRoot = pXMLDOMDoc->firstChild;
		
		//get source address
		VARIANT vData;
		VariantInit( &vData );

		/*************************************************************/
		/* Extract the tag data we want */

		/* Get Company info */
		if ( !GetXMLData( COMPANY_XML_START_TAG, vData, spRoot ) || vData.vt != VT_BSTR )
		{
			m_EvtLogger.ReportError( __FILE__, __LINE__, "Error getting Company data" );
			goto EXIT_FILTER_DOC_SUM_INFO;
		}

		szRetVal += COMPANY_XML_START_TAG;
			szRetVal += (char*) _bstr_t ( vData.bstrVal );
		szRetVal += COMPANY_XML_END_TAG;
		
		if ( !GetXMLData( BYTE_COUNT_XML_START_TAG, vData, spRoot ) || vData.vt != VT_BSTR )
		{
			m_EvtLogger.ReportError( __FILE__, __LINE__, "Error getting Byte Count data" );
			goto EXIT_FILTER_DOC_SUM_INFO;
		}

		szRetVal += BYTE_COUNT_XML_START_TAG;
			szRetVal += (char*) _bstr_t ( vData.bstrVal );
		szRetVal += BYTE_COUNT_XML_END_TAG;
		

		/*************************************************************/
	}
	catch( _xml_error& e )
	{
		AString szError( e._pchBuf );
		m_EvtLogger.ReportError( __FILE__, __LINE__, szError );
		goto EXIT_FILTER_DOC_SUM_INFO;
	}
	catch( _com_error& e )
	{
		AString szError( (char*) e.Description() );
		m_EvtLogger.ReportError( __FILE__, __LINE__, szError );
		goto EXIT_FILTER_DOC_SUM_INFO;
	}
	catch(...)
	{
		m_EvtLogger.ReportError( __FILE__, __LINE__, "Unknown Error occurred" );
		goto EXIT_FILTER_DOC_SUM_INFO;
	}

EXIT_FILTER_DOC_SUM_INFO:

	if( pXMLDOMDoc )
		pXMLDOMDoc->Release();

	pXMLDOMDoc = NULL;

	return szRetVal;
}

AString FRAXFilter::FilterHyperlinksInfoSection( AString szXML )
{
	AString szRetVal( "" );
	VARIANT_BOOL bStatus = VARIANT_FALSE;

	//create XMLDOMDocument
	MSXML2::IXMLDOMDocument* pXMLDOMDoc = NULL;
	
	HRESULT hr = CoCreateInstance( MSXML2::CLSID_DOMDocument,
									NULL,
									CLSCTX_ALL,
									MSXML2::IID_IXMLDOMDocument,
									reinterpret_cast<void**>(&pXMLDOMDoc) );

	if( FAILED( hr ) )
	{
		m_EvtLogger.ReportError( __FILE__, __LINE__, "CoCreateInstance failed" );;
		goto EXIT_FILTER_HLINKS_INFO;
	}

	try
	{
		_bstr_t bstrtData( szXML.AllocSysString() );
		bStatus = pXMLDOMDoc->loadXML( bstrtData );
		if( bStatus == VARIANT_FALSE )
		{
			//report error
			m_EvtLogger.ReportError( __FILE__, __LINE__, "Error loading XML" );
			goto EXIT_FILTER_HLINKS_INFO;
		}
		
		//start at root of document IP XML start tag
		MSXML2::IXMLDOMElementPtr spRoot = pXMLDOMDoc->firstChild;
		
		//get source address
		VARIANT vData;
		VariantInit( &vData );

		/*************************************************************/
		/* Extract the tag data we want */

		/* Get Hyperlinks info */
		if ( !GetXMLData( HYPERLINKS_XML_START_TAG, vData, spRoot ) || vData.vt != VT_BSTR )
		{
			m_EvtLogger.ReportError( __FILE__, __LINE__, "Error getting Hyperlinks data" );
			goto EXIT_FILTER_HLINKS_INFO;
		}

		szRetVal += HYPERLINKS_XML_START_TAG;
			szRetVal += (char*) _bstr_t ( vData.bstrVal );
		szRetVal += HYPERLINKS_XML_END_TAG;
		
		/*************************************************************/
	}
	catch( _xml_error& e )
	{
		AString szError( e._pchBuf );
		m_EvtLogger.ReportError( __FILE__, __LINE__, szError );
		goto EXIT_FILTER_HLINKS_INFO;
	}
	catch( _com_error& e )
	{
		AString szError( (char*) e.Description() );
		m_EvtLogger.ReportError( __FILE__, __LINE__, szError );
		goto EXIT_FILTER_HLINKS_INFO;
	}
	catch(...)
	{
		m_EvtLogger.ReportError( __FILE__, __LINE__, "Unknown Error occurred" );
		goto EXIT_FILTER_HLINKS_INFO;
	}

EXIT_FILTER_HLINKS_INFO:

	if( pXMLDOMDoc )
		pXMLDOMDoc->Release();

	pXMLDOMDoc = NULL;

	return szRetVal;
}

AString FRAXFilter::FilterSummaryInfoSection( AString szXML )
{
	AString szRetVal( "" );
	VARIANT_BOOL bStatus = VARIANT_FALSE;

	//create XMLDOMDocument
	MSXML2::IXMLDOMDocument* pXMLDOMDoc = NULL;
	
	HRESULT hr = CoCreateInstance( MSXML2::CLSID_DOMDocument,
									NULL,
									CLSCTX_ALL,
									MSXML2::IID_IXMLDOMDocument,
									reinterpret_cast<void**>(&pXMLDOMDoc) );

	if( FAILED( hr ) )
	{
		m_EvtLogger.ReportError( __FILE__, __LINE__, "CoCreateInstance failed" );;
		goto EXIT_FILTER_SUM_INFO;
	}

	try
	{
		_bstr_t bstrtData( szXML.AllocSysString() );
		bStatus = pXMLDOMDoc->loadXML( bstrtData );
		if( bStatus == VARIANT_FALSE )
		{
			//report error
			m_EvtLogger.ReportError( __FILE__, __LINE__, "Error loading XML" );
			goto EXIT_FILTER_SUM_INFO;
		}
		
		//start at root of document IP XML start tag
		MSXML2::IXMLDOMElementPtr spRoot = pXMLDOMDoc->firstChild;
		
		//get source address
		VARIANT vData;
		VariantInit( &vData );

		/*************************************************************/
		/* Extract the tag data we want */

		/* Get Application name info */
		if ( !GetXMLData( APPNAME_XML_START_TAG, vData, spRoot ) || vData.vt != VT_BSTR )
		{
			m_EvtLogger.ReportError( __FILE__, __LINE__, "Error getting Application name" );
			goto EXIT_FILTER_SUM_INFO;
		}

		szRetVal += APPNAME_XML_START_TAG;
			szRetVal += (char*) _bstr_t ( vData.bstrVal );
		szRetVal += APPNAME_XML_END_TAG;
		
		/* Get Last Saved info */
		if ( !GetXMLData( LAST_SAVED_XML_START_TAG, vData, spRoot ) || vData.vt != VT_BSTR )
		{
			m_EvtLogger.ReportError( __FILE__, __LINE__, "Error getting Last Saved data" );
			goto EXIT_FILTER_SUM_INFO;
		}

		szRetVal += LAST_SAVED_XML_START_TAG;
			szRetVal += (char*) _bstr_t ( vData.bstrVal );
		szRetVal += LAST_SAVED_XML_END_TAG;
		
		/* Get Created info */

		if ( !GetXMLData( CREATE_DATE_TIME_XML_START_TAG, vData, spRoot ) || vData.vt != VT_BSTR )
		{
			m_EvtLogger.ReportError( __FILE__, __LINE__, "Error getting Create date data" );
			goto EXIT_FILTER_SUM_INFO;
		}

		szRetVal += CREATE_DATE_TIME_XML_START_TAG;
			szRetVal += (char*) _bstr_t ( vData.bstrVal );
		szRetVal += CREATE_DATE_TIME_XML_END_TAG;
		

		/*************************************************************/
	}
	catch( _xml_error& e )
	{
		AString szError( e._pchBuf );
		m_EvtLogger.ReportError( __FILE__, __LINE__, szError );
		goto EXIT_FILTER_SUM_INFO;
	}
	catch( _com_error& e )
	{
		AString szError( (char*) e.Description() );
		m_EvtLogger.ReportError( __FILE__, __LINE__, szError );
		goto EXIT_FILTER_SUM_INFO;
	}
	catch(...)
	{
		m_EvtLogger.ReportError( __FILE__, __LINE__, "Unknown Error occurred" );
		goto EXIT_FILTER_SUM_INFO;
	}

EXIT_FILTER_SUM_INFO:

	if( pXMLDOMDoc )
		pXMLDOMDoc->Release();

	pXMLDOMDoc = NULL;

	return szRetVal;
}

/*
	Function extracts the XML data associated with an XML tag.

	Parameters:	szXMLTag	- tag we want the data for
				vData		- variant to hold tag data
				spRoot		- XML element pointer

	Return values:	On Success returns TRUE (data extracted)
					otherwise returns FALSE
*/
BOOL FRAXFilter::GetXMLData( AString szXMLTag, VARIANT& vData, MSXML2::IXMLDOMElementPtr spRoot )
{
	AString szTemp = szXMLTag;
	szTemp.Strip();
	szTemp.TrimLeft( "<" );
	szTemp.TrimRight( ">" );

	if( spRoot == NULL )
		return FALSE;
	
	BOOL bReturn = FALSE;
	try
	{
		VariantClear( &vData ); //clear contents of variant

		// Find child node with specific XML tag
		MSXML2::IXMLDOMElementPtr spFirstChild = spRoot->selectSingleNode( _bstr_t( (LPCTSTR) szTemp ) );
		
		if( spFirstChild == NULL )
		{
			AString szError( "No Child Node found with name: " );
			szError += szXMLTag;

			szError += " Current Node is: ";

			BSTR bstrTemp;

			spRoot->get_nodeName( &bstrTemp );

			szError += (char*) _bstr_t( bstrTemp );

			SysFreeString( bstrTemp );

			m_EvtLogger.ReportError( __FILE__, __LINE__, szError );
			return bReturn;
		}
		
		//	Get node Value
		HRESULT hr = spFirstChild->get_nodeTypedValue( &vData );
		if( FAILED( hr ) )
		{
			AString szError( "Error retrieving value for: " );
			szError += szTemp;
			m_EvtLogger.ReportError( __FILE__, __LINE__, szError );
		}
		else bReturn = TRUE;
	}
	catch( _xml_error& e )
	{
		AString szError( e._pchBuf );
		m_EvtLogger.ReportError( __FILE__, __LINE__, szError );
		return bReturn;
	}
	catch( _com_error& e )
	{
		AString szError( (char*) e.Description() );
		m_EvtLogger.ReportError( __FILE__, __LINE__, szError );
		return bReturn;
	}
	catch(...)
	{
		m_EvtLogger.ReportError( __FILE__, __LINE__, "Unknown Error occurred" );
		return bReturn;
	}
	
	return bReturn;
}