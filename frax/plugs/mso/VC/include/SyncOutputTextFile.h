// SyncOutputFile.h: interface for the SyncOutputFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SYNCOUTPUTFILE_H__8BA7A1FA_8A33_4A92_8D1C_410FA29FFC6F__INCLUDED_)
#define AFX_SYNCOUTPUTFILE_H__8BA7A1FA_8A33_4A92_8D1C_410FA29FFC6F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <AString.h>
#include <afxmt.h>
#include <fstream.h>
#include <exports.h>

/* NOTE:	If the file is to be accessed by multiple threads the method
			calls of the class should be used with the default 
			arguments to avoid concurrency issues such as one thread
			trying to delete a file in use by another thread. The OS
			should prevent this but it is not guaranteed.			*/

//Thread safe output file stream, file protected by a mutex 
class CommonExport SyncOutputTextFile  
{
public:
	SyncOutputTextFile( AString szFilename, BOOL bDeleteExisting = FALSE );
	virtual ~SyncOutputTextFile();

	//methods

	/* Function writes to the file. 

	Input parameters:	szData		- data to write to file.
						bTruncate	- whether or not to truncate file
									  before writing to file.    

	Return values:		On Success	- returns TRUE
						On Error	- returns FALSE */
	BOOL WriteToFile( AString szData, BOOL bTruncate = FALSE );

private:
	AString m_szFilename;
	CMutex* m_pMutex;
	ofstream m_fOut;
};

#endif // !defined(AFX_SYNCOUTPUTFILE_H__8BA7A1FA_8A33_4A92_8D1C_410FA29FFC6F__INCLUDED_)
