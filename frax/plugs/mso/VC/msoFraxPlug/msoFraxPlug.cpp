// msoFraxPlug.cpp : Defines the entry point for the console application.
//
/*******************/

/* Ignore VC++ 6 compiler warning for truncated symbol names when debugging */

#ifdef USING_VC6
#pragma warning( disable : 4786 )
#endif

/********************/

#include "stdafx.h"
#include "msoFraxPlug.h"

#include <comdef.h>

#include <string.h>
#include <stdlib.h>
#include <sienaconf.h>

#include <iostream>
#include <exception>
						// include Siena headers
#include <siena/Siena.h>
#include <siena/ThinClient.h>

#include <AString.h>
#include <EvtLogger.h>
#include "FRAXFilter.h"
#include "FileGetter.h"
#import <MSDocProp.tlb> no_namespace named_guids raw_dispinterfaces
#import <msoProp.dll> no_namespace named_guids raw_dispinterfaces

static void print_usage(const char * progname) {
    cerr << "usage: " << progname 
	 << " [-udp] [-port <num>] [-host <host>] <uri-master>" << endl;
}

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// The one and only application object

CWinApp theApp;

using namespace std;

/*
	Function extracts the properties of a specified document.

	Parameters: szFileURL	- URL of document
				szMaster	- Siena master server

	Return Values: TRUE if all goes OK else FASLE returned

	NOTE: All errors written to NT Event log

	Basic Process Steps:
	
	1) Download document to local store from szFileURI
	2) Extract document properties
	3) Publish results as Siena event
	4) Remove local copy of document
	5) Cleanup resources and Exit
*/
BOOL ProcessDocument( AString szFileURL, AString szMaster )
{
	BOOL bRetVal = FALSE;
	EvtLogger evtLog( APPNAME );

	IAMTSSimpleProp* pSimpleProp = NULL;
	_IMsoDocProp* pExtProp = NULL;
		
	// Quick error checks, URL must not be empty string, neither (Siena) szMaster
	if( szFileURL.GetLength() == 0 || szMaster.GetLength() == 0 )
		return FALSE;

	//	set up temporary storage location, try %Temp% dir
	//  otherwise use root dir
	char szBuffer[MAX_PATH];
	AString szTempPath( "" );

	memset( szBuffer, 0, sizeof(szBuffer) );

	if( GetEnvironmentVariable( "Temp", szBuffer, sizeof(szBuffer) ) )
		szTempPath = AString( szBuffer );
	else szTempPath = "C:";

	// Request necessary interfaces
	// Get Standard properties
	HRESULT hr =  CoCreateInstance( CLSID_AMTSSimpleProp,
										NULL,
										CLSCTX_ALL,
										IID_IAMTSSimpleProp,
										reinterpret_cast<void**>( &pSimpleProp ) );
	if( FAILED( hr ) )
		goto EXIT_PROCESS_DOCUMENT;

	// Get extended info e.g Hyperlinks
	hr = CoCreateInstance( CLSID_IMsoDocProp,
							NULL,
							CLSCTX_ALL,
							IID__IMsoDocProp,
							reinterpret_cast<void**>(&pExtProp) );
	
	if( FAILED( hr ) )
		goto EXIT_PROCESS_DOCUMENT;

	try
	{
		// Download to local storage, then process
		szFileURL = szFileURL.FindReplaceNoCase( "\\", "/" );
		int nIndex = szFileURL.ReverseFind( '/' );
		szTempPath += "\\";
		szTempPath += szFileURL.Mid( nIndex + 1 );

		FileGetter objFileGet;
		if( !objFileGet.GetFile( szFileURL, szTempPath ) )
			goto EXIT_PROCESS_DOCUMENT;
	
		BSTR bstrArg = szTempPath.AllocSysString();

		// Format string for FRAX
		
		// Get protocol
		nIndex = szFileURL.IndexOfNoCase( ":" );
		
		AString szProtocol( PROTOCOL_XML_START_TAG );
			if( nIndex == -1 )
				szProtocol += "unknown";
			else szProtocol += szFileURL.Mid( 0, nIndex );
		szProtocol += PROTOCOL_XML_END_TAG;

		// Get URL 
		AString szURL( URL_XML_START_TAG );
			szURL += szFileURL;
		szURL += URL_XML_END_TAG;

		AString szType( TYPE_XML_START_TAG );
			szType += "MS Office Document";
		szType += TYPE_XML_END_TAG;

		nIndex = szFileURL.ReverseFind( '/' );
		
		AString szSource( FRAX_SOURCE_XML_START_TAG );
			if( nIndex == -1 )
				szSource += szFileURL;
			else szSource += szFileURL.Mid( 0, nIndex + 1 );
		szSource += FRAX_SOURCE_XML_END_TAG;

		AString szSummaryInfo = (char*) _bstr_t( pSimpleProp->ViewSummaryInfo( bstrArg ) );
		FRAXFilter f;
		szSummaryInfo = f.FilterSummaryInfoSection( szSummaryInfo );
		AString szDocSummaryInfo = (char*) _bstr_t( pSimpleProp->ViewDocSummaryInfo( bstrArg ) );
		szDocSummaryInfo = f.FilterDocSummaryInfoSection( szDocSummaryInfo );
		AString szHyperlinks = (char*) _bstr_t( pExtProp->ViewHyperlinks( bstrArg  ) );
		szHyperlinks = f.FilterHyperlinksInfoSection( szHyperlinks );
		
		// Build data string to publish
		AString szData( OFFICE_DOC_PROP_XML_START_TAG ) ;
		szData += szProtocol;
		szData += szURL;
		szData += szType;
		szData += szSource;
		szData += szSummaryInfo;
		szData += szDocSummaryInfo;
		szData += szHyperlinks;
		szData += OFFICE_DOC_PROP_XML_END_TAG;

		// Free string memory
		SysFreeString( bstrArg );
		
		// Delete temporary file
		if( szTempPath.GetLength() > MIN_PATH_LENGTH )
			DeleteFile( szTempPath );

		// Publish document properties via Siena
		ThinClient siena( (LPCTSTR) szMaster );

		// publish directly to frax
		
		Notification n;
		n[OFFICE_DOC_PROP_RESULTS_NAME] = OFFICE_DOC_PROP_RESULTS_VALUE;
 
		n[OFFICE_DOC_PROPERTIES] = (LPCTSTR) szData;

		siena.publish(n);
				
		siena.shutdown();			// closes Siena interface
	}
	catch( _com_error& e ) // COM error handling
	{
		
		AString szError( (char*) e.Description() );
		if( szError.GetLength() > 0 )
		{
			evtLog.ReportError( __FILE__, __LINE__, szError );
		}

		goto EXIT_PROCESS_DOCUMENT;
	}
	catch( exception &ex ) // Siena error handling
	{
		AString szError( ex.what() );
		if( szError.GetLength() > 0 )
		{
			evtLog.ReportError( __FILE__, __LINE__, szError );
		}

		goto EXIT_PROCESS_DOCUMENT;
    }
	catch( ... ) // General error handling
	{
		evtLog.ReportError( __FILE__, __LINE__, "Unexpected Error Occurred" );
		goto EXIT_PROCESS_DOCUMENT;
	}
	
	bRetVal = TRUE;

// Cleanup
EXIT_PROCESS_DOCUMENT:

	if( pSimpleProp )
		pSimpleProp->Release();

	pSimpleProp = NULL;

	if( pExtProp )
		pExtProp->Release();

	pExtProp = NULL;

	return bRetVal;
}

// Main entry point
int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	// initialize MFC and print and error on failure
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: change error code to suit your needs
		cerr << _T("Fatal Error: MFC initialization failed") << endl;
		nRetCode = 1;
	}
	else
	{
		bool udp = false;
		unsigned short port = 1969;
		const char * master = NULL;
		const char * thishost = NULL;

		try 
		{
			int i;
			for(i=1; i < argc; ++i) 
			{		// parse cmd-line params
				if(strcmp(argv[i], "-udp")==0) 
				{
					udp = true;
				} 
				else if (strcmp(argv[i], "-port")==0) 
				{
					if (++i < argc) 
					{
						port = atoi(argv[i]);
					} 
					else 
					{
						print_usage(argv[0]);
						return 1;
					}
				} 
				else if (strcmp(argv[i], "-host")==0) 
				{
					if (++i < argc) 
					{
						thishost = argv[i];
					} 
					else 
					{
						print_usage(argv[0]);
						return 1;
					}
				} 
				else 
				{
					master = argv[i];
				}
			}
			if (master == NULL) 
			{
				print_usage(argv[0]);
				return 1;
			}

			ThinClient siena(master);		// create interface to
			Receiver * r;				// given master server
			if (udp) 
			{
				r = new UDPReceiver(port, thishost);// create receiver for 
			} 
			else 
			{				// this interface
				r = new TCPReceiver(port, thishost);
			}
			siena.set_receiver(r);			// set receiver

			Filter f;				// create subscription filter
			
			f.add_constraint( CONSTRAINT_NAME, SX_eq, CONSTRAINT_VALUE );
			
			siena.subscribe(f);			// subscribe
			EvtLogger evtLog( APPNAME );
			AString szMaster( master );

			Notification * n;
			bool bShutdown = false;

			while( !bShutdown )
			{	// read incoming notifications
				n = siena.get_notification();
				if (n != NULL)  
				{
					// init
					CoInitialize( NULL );
					// process document and publish results
					AString szFileURI( (*n)[TARGET_DOC].string_value().c_str() );
					
					if( !ProcessDocument( szFileURI, szMaster ) )
					{
						AString szError( "Error Processing: " );
						szError += szFileURI;
						evtLog.ReportError( __FILE__, __LINE__, szError );
					}

					// un-init
					CoUninitialize();
				}
			}
			
			siena.unsubscribe();			// unsubscribe and shutdown 
			siena.shutdown();			// interface
			delete(r);
		} 
		catch (exception &ex) 
		{
			cout << "error: " << ex.what() << endl;
		}
	}
	return nRetCode;
}


