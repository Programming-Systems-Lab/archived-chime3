// msoFRAXPlugUnitTest.cpp : Defines the entry point for the console application.
//
/*******************/

/* Ignore VC++ 6 compiler warning for truncated symbol names when debugging */

#ifdef USING_VC6
#pragma warning( disable : 4786 )
#endif

/********************/

#include "stdafx.h"
#include "msoFRAXPlugUnitTest.h"

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

// TODO: move constants to one shared common include file
static const char* CONSTRAINT_NAME					= "OfficeDoc";
static const char* CONSTRAINT_VALUE					= "FRAXOfficeDoc";
static const char* TARGET_DOC						= "TargetDoc";
static const char* OFFICE_DOC_PROP_RESULTS_NAME		= "OfficeDocPropResults";
static const char* OFFICE_DOC_PROP_RESULTS_VALUE	= "msoFRAXPlugResults";
static const char* OFFICE_DOC_PROPERTIES			= "DocProperties";

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// The one and only application object

CWinApp theApp;

using namespace std;

static void print_usage(const char * progname) {
    cerr << "usage: " << progname 
	 << " [-udp] [-port <num>] [-host <host>] <uri-master>" << endl;
}

// Application entry point.
// Simple driver program, publishes a request for Office document 
// properties on the Siena Bus and waits for a response by a 
// capable FRAX plug
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

			// Notification 
			Notification n;
			n[CONSTRAINT_NAME] = CONSTRAINT_VALUE;
			// Target Document
			n[TARGET_DOC] = "http://localhost/testweb/images/testDoc.doc";
 
			// publish then wait for results
			siena.publish(n);

			Filter f;				// create subscription filter
			f.add_constraint( OFFICE_DOC_PROP_RESULTS_NAME, SX_eq, OFFICE_DOC_PROP_RESULTS_VALUE );
			
			siena.subscribe(f);			// subscribe

			// Wait on results
			Notification * res;
			res = siena.get_notification();
			if (res != NULL)  
			{
				AString szFileURI( (*res)[OFFICE_DOC_PROPERTIES].string_value().c_str() );

				cout << (LPCTSTR) szFileURI << endl;
			}

				siena.shutdown();			// closes Siena interface
			
		}		
		catch (exception &ex) 
		{
			cout << "error: " << ex.what() << endl;
		}
    
	}

	return nRetCode;
}


