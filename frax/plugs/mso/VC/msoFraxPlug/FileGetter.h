// FileGetter.h: interface for the FileGetter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILEGETTER_H__F38094F5_5E0E_4E5D_8A28_ADD947E0213E__INCLUDED_)
#define AFX_FILEGETTER_H__F38094F5_5E0E_4E5D_8A28_ADD947E0213E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <AString.h>
#include <EvtLogger.h>

class FileGetter  
{
public:
	FileGetter();
	virtual ~FileGetter();

	BOOL GetFile( AString szURL, AString szLocalDest );
	
	AString GetLastError( void ) const;
	void SetLastError( char* szFile, long lngLine, AString szError );

private:
	AString m_szError;
	EvtLogger m_EvtLogger;

};

#endif // !defined(AFX_FILEGETTER_H__F38094F5_5E0E_4E5D_8A28_ADD947E0213E__INCLUDED_)
