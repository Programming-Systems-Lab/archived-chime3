// ALogger.h: interface for the ALogger class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ALOGGER_H__CBAAE33C_E418_4127_AB30_B3E447408479__INCLUDED_)
#define AFX_ALOGGER_H__CBAAE33C_E418_4127_AB30_B3E447408479__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <exports.h>
#include <AString.h>
#include <afxmt.h>
#include <windows.h>

#define MSG_GENERIC_ERROR ((DWORD)0xC0000000L)

#define MSG_GENERIC_WARNING ((DWORD)0x80000001L) 

#define MSG_GENERIC_INFORMATION ((DWORD)0x40000002L)

const int YEAR_OFFSET	= 0;
const int YEAR_LENGTH	= 4;
const int MONTH_OFFSET	= 4;
const int MONTH_LENGTH	= 2;
const int DAY_OFFSET	= 6;
const int DAY_LENGTH	= 2;

const int SEPARATOR_OFFSET = 8;
const int SEPARATOR_LENGTH = 1;

const int HOUR_OFFSET	= 9;//8;  
const int HOUR_LENGTH	= 2;
const int MIN_OFFSET	= 11;//10;
const int MIN_LENGTH	= 2;
const int SEC_OFFSET	= 13;//12;
const int SEC_LENGTH	= 2;

//static methods used to build EvtLogger class

class CommonExport ALogger  
{
public:
	ALogger();
	virtual ~ALogger();

	/*	Function adds an event source to the registry. 

		Input parameters:	szSource - the name of the source that will show up
								in the event viewer
							szMsgDll - the name of the messaging DLL

		Return values:		On SUCCESS - returns TRUE
					On Error   - returns FALSE  */
	static BOOL AddEventSource( AString szSource, AString szMsgDll );
	/* Function logs an error event in the system event log .

		Input parameters:	szFile	- the source file where event occurred
							lngLine - the line of the source file where
									  event occurred
							szMsg	- the message to log
							szSource- the source which to associate with the
									  event (the source should have been
									  added to the registry along with an
									  associated messaging Dll using the
									  AddEventSource(...) function)

		Return values:		On Success returns TRUE
							On Error returns FALSE */
	static BOOL ReportError( char* szFile, long lngLine, AString szMsg, AString szSource );
	
	/* Function logs an error event in the system event log .

	Input parameters:	szFile	- the source file where event occurred
						lngLine - the line of the source file where
								  event occurred
						szMsg	- the message to log
						hSource - a handle to a registered event source
								  (the source should have been
								  added to the registry along with an
								  associated messaging Dll using the
								  AddEventSource(...) function)

	Return values:		On Success returns TRUE
						On Error returns FALSE */
	static BOOL ReportError( char* szFile, long lngLine, AString szMsg, HANDLE hSource );
	
	/* Function logs an info event in the system event log .

		Input parameters:	szFile	- the source file where event occurred
							lngLine - the line of the source file where
									  event occurred
							szMsg	- the message to log
							szSource- the source which to associate with the
									  event (the source should have been
									  added to the registry along with an
									  associated messaging Dll using the
									  AddEventSource(...) function)

		Return values:		On Success returns TRUE
							On Error returns FALSE */
	static BOOL ReportInfo( char* szFile, long lngLine, AString szMsg, AString szSource );

	/* Function logs an info event in the system event log .

	Input parameters:	szFile	- the source file where event occurred
						lngLine - the line of the source file where
								  event occurred
						szMsg	- the message to log
						hSource - a handle to a registered event source
								  (the source should have been
								  added to the registry along with an
								  associated messaging Dll using the
								  AddEventSource(...) function)

	Return values:		On Success returns TRUE
						On Error returns FALSE  */
	static BOOL ReportInfo( char* szFile, long lngLine, AString szMsg, HANDLE hSource );

		
	/* Function logs a warning event in the system event log .

	Input parameters:	szFile	- the source file where event occurred
						lngLine - the line of the source file where
								  event occurred
						szMsg	- the message to log
						szSource- the source which to associate with the
								  event (the source should have been
								  added to the registry along with an
								  associated messaging Dll using the
								  AddEventSource(...) function)

	Return values:		On Success returns TRUE
						On Error returns FALSE */	
	static BOOL ReportWarning( char* szFile, long lngLine, AString szMsg, AString szSource );
	
	/* Function logs a warning event in the system event log .

	Input parameters:	szFile	- the source file where event occurred
						lngLine - the line of the source file where
								  event occurred
						szMsg	- the message to log
						hSource - a handle to a registered event source
								  (the source should have been
								  added to the registry along with an
								  associated messaging Dll using the
								  AddEventSource(...) function)

	Return values:		On Success returns TRUE
						On Error returns FALSE  */
	static BOOL ReportWarning( char* szFile, long lngLine, AString szMsg, HANDLE hSource );

	//additional utility methods
		
	/*	Function gets the error message associated with the last system 
		error that occurred.

	Input parameters: szError - a reference to an AString to store the
								system error message.

	Return values:	  returns TRUE.  */
	static BOOL LastError( AString& szError );
	/*	Function creates a timestamp string using the local time on the
		system. The string contains formatting information

		Input parameters -	szTimestamp an refenece to an AString to store
							the timestamp

		Return values	- returns TRUE. 

		NOTE: timestamp string in the form: yyyy/mm/dd_hh:mm:ss   */
	static AString CreateFormattedLocalTimestamp( void );
		
	/*	Function creates a timestamp string using the local time on the
		system. The string contains no formatting information

	Input parameters -	szTimestamp an reference to an AString to store
						the timestamp

	Return values	- returns TRUE. 

	NOTE: timestamp string in the form: yyyymmdd_hhmmss   */
	static AString CreateLocalTimestamp( void );
	
		
	/* Function logs an error event to the system Application event log.

	Input parameters:	szError - the message to log
						szSource- the source name to associate with
								  the event. 

	Return values:		On Success returns TRUE
						On Error returns FALSE. */
	static BOOL ReportErrorEvent( AString szError, AString szSource );
		
	/* Function logs an error event to the system Application event log.

	Input parameters:	szError - the message to log
						hSource - HANDLE to an (open)existing event source

	Return values:		On Success returns TRUE
						On Error returns FALSE. */
	static BOOL ReportErrorEvent( AString szError, HANDLE hSource );
		
	/* Function logs an info event to the system Application event log.

	Input parameters:	szError - the message to log
						szSource- the source name to associate with
								  the event. 

	Return values:		On Success returns TRUE
						On Error returns FALSE. */
	static BOOL ReportInfoEvent( AString szError, AString szSource );
	
	/* Function logs an info event to the system Application event log.

	Input parameters:	szError - the message to log
						hSource - HANDLE to an (open)existing event source

	Return values:		On Success returns TRUE
						On Error returns FALSE. */
	static BOOL ReportInfoEvent( AString szError, HANDLE hSource );
	
	/* Function logs a warning event to the system Application event log.

	Input parameters:	szError - the message to log
						szSource- the source name to associate with
								  the event. 

	Return values:		On Success returns TRUE
						On Error returns FALSE. */
	static BOOL ReportWarningEvent( AString szError, AString szSource );
		
	/* Function logs an info event to the system Application event log.

	Input parameters:	szError - the message to log
						hSource - HANDLE to an (open)existing event source

	Return values:		On Success returns TRUE
						On Error returns FALSE. */
	static BOOL ReportWarningEvent( AString szError, HANDLE hSource );

private:
};

#endif // !defined(AFX_ALOGGER_H__CBAAE33C_E418_4127_AB30_B3E447408479__INCLUDED_)
