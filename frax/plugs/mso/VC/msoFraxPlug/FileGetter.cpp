// FileGetter.cpp: implementation of the FileGetter class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FileGetter.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

FileGetter::FileGetter():m_szError( "" ),m_EvtLogger( "FileGetter" )
{}

FileGetter::~FileGetter()
{}

AString FileGetter::GetLastError( void ) const
{
	return m_szError;
}

BOOL FileGetter::GetFile( AString szURL, AString szLocalDest )
{
	// quick error checks

	if( szURL.GetLength() == 0 )
	{
		SetLastError( __FILE__, __LINE__, "Empty URL...no file to get" );
		return FALSE;
	}

	if( szLocalDest.GetLength() == 0 )
	{
		SetLastError( __FILE__, __LINE__, "Empty URL...no location to store file" );
		return FALSE;
	}

	HRESULT hr = URLDownloadToFile( NULL, (LPCTSTR) szURL, (LPCTSTR) szLocalDest, 0, NULL );
	
	if( FAILED( hr ) )
	{
		AString szError( "" );
		// Try to use GetLastError to determine what went wrong
		EvtLogger::LastError( szError );

		// build our own error msg if no info returned from GetLastError
		if( szError.GetLength() == 0 )
		{
			szError = "Error downloading from URL: ";
			szError += szURL;
			szError += " and saving to: ";
			szError += szLocalDest;
		}
		
		// Set the last error property of FileGetter
		SetLastError( __FILE__, __LINE__, szError );

		return FALSE; // flag error
	}
	else return TRUE;
}

void FileGetter::SetLastError( char* szFile, long lngLine, AString szError )
{
	if( szError.GetLength() > 0 )
	{
		m_EvtLogger.ReportError( szFile, lngLine, szError );
		m_szError = szError;
	}
}


