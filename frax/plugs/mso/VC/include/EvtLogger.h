// EvtLogger.h: interface for the EvtLogger class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EVTLOGGER_H__3398113C_542A_11D5_80B8_0050DAC00BBC__INCLUDED_)
#define AFX_EVTLOGGER_H__3398113C_542A_11D5_80B8_0050DAC00BBC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <exports.h>
#include <AString.h>
#include <afxmt.h>
#include <windows.h>

const AString SZ_MESSAGE_DLL = "genmsg.dll";

class CommonExport EvtLogger  
{
public:
	EvtLogger( AString szSource );
	virtual ~EvtLogger();
	
	/* Function cleans up an EvtLogger Instance.

		Return values:	returns TRUE */
	BOOL CleanupInstance( void );
	/* Function initializes and EvtLogger instance.

		Input parameters:	szSource - name of source to initialize an 
									   instance with. 

		Return values:		On Success returns TRUE
							On Error returns FALSE */
	BOOL InitInstance( AString szSource );
	
	/* Function logs an error event in the system event log .

		Input parameters:	szFile	- the source file where event occurred
							lngLine - the line of the source file where
									  event occurred
							szMsg	- the message to log
							
		Return values:		On Success returns TRUE
							On Error returns FALSE */
	BOOL ReportError( char* szFile, long lngLine, AString szMsg );
	
	/* Function logs an info event in the system event log .

		Input parameters:	szFile	- the source file where event occurred
							lngLine - the line of the source file where
									  event occurred
							szMsg	- the message to log
							
		Return values:		On Success returns TRUE
							On Error returns FALSE */
	BOOL ReportInfo( char* szFile, long lngLine, AString szMsg );
	
	/* Function logs a warning event in the system event log .

	Input parameters:	szFile	- the source file where event occurred
						lngLine - the line of the source file where
								  event occurred
						szMsg	- the message to log
						
	Return values:		On Success returns TRUE
						On Error returns FALSE */	
	BOOL ReportWarning( char* szFile, long lngLine, AString szMsg );

	/*	Function gets the error message associated with the last system 
		error that occurred.

	Input parameters: szError - a reference to an AString to store the
								system error message.

	Return values:	  returns TRUE.  */
	static BOOL LastError( AString& szError );
	/*	Write data to Tracefile. Tracefile protected by named mutex 
		created when evtLogger instance created. 
	
		Input Parameters:	char* szFile	- source file originating trace
							long lngLine	- line in source file of trace
							AString szData	- string data to write to file
	
	Return values:			returns 0  */
	long Trace( char* szFile, long lngLine, AString szData );	
	
	
	BOOL WriteInfoEvent( AString szData );
	BOOL WriteErrorEvent( AString szData );
	BOOL WriteWarningEvent( AString szData );

	AString GetTraceFileFullQName( void );
	AString GetSourceName( void );

private:
	AString m_szSource;
	HANDLE m_hSource;
	AString m_szTraceFile;
};

#endif // !defined(AFX_EVTLOGGER_H__3398113C_542A_11D5_80B8_0050DAC00BBC__INCLUDED_)
