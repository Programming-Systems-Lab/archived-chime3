// BaseCtl.h: interface for the BaseCtl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BASECTL_H__935F05C3_4F76_11D5_80AF_0050DAC00BBC__INCLUDED_)
#define AFX_BASECTL_H__935F05C3_4F76_11D5_80AF_0050DAC00BBC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <COMDEF.H>
#include "exports.h"
#include <list>
#include <Evtlogger.h>
#include <AString.h>
#include <DirData.h>
#include <fstream.h>

//supported input file types
const AString TEXTFILE_EXT_FILTER	= "\\*.txt";
const AString HTMLFILE_EXT_FILTER	= "\\*.htm?";
const AString XMLFILE_EXT_FILTER	= "\\*.xml";

const AString OUTPUTFILE_EXT		= ".imp";

//Thread struct used by services
typedef struct
{
  unsigned long threadId;
  void*         pMainObj;
} *PTHREADSTRUCT, THREADSTRUCT;

/*	Base class for import services provides most common functionality used by imports services.
	Services are usually assigned a set of directories/folders to monitor. The service reads
	each file in the current directory processes the data in the file and reports any errors
	encountered during processing. 

	Import services usually use a triple of directories when processing;
	an input directory (required)		- contains the files to be processed, the service 
										  continually monitors this directory processing files 
										  as they are put there
							  
	an archive directory (required)		- each file is copied over to the archive directory 
										  to be processed, original file in the input directory 
										  is then deleted so it is not processed again (the 
										  service will keep monitoring the input directory for 
										  files )
							  
	an error directory (required)		- Data errors are written to files stored in this directory.
										  not all services use an error directory. */

class CommonExport BaseCtl  
{
public:
	//constructor
	BaseCtl( std::list<DirData>& lstDirectories );
	//destructor
	virtual ~BaseCtl();
	
	//pure virtual functions
	//must be implemented by subclasses
	
	//pure virtual function which configures import service specific settings
	virtual void InitializeConfig() = 0;//pure virtual
	//process input file
	virtual BOOL ProcessLoad( AString& szInputFullQName ) = 0; //pure virtual function
	//start service processing
	virtual BOOL StartProcessThread() = 0; //pure virtual
	//determine action to be taken if an error log exists
	virtual BOOL HandleErrorFile( AString szErrorFullQName ) = 0; //pure virtual
		
	
	
	//Basic functionality
	virtual BOOL LogError( char* szFile, long lngLine, AString szMsg ) = 0;
	virtual BOOL LogInfo( char* szFile, long lngLine, AString szMsg ) = 0;
	virtual BOOL LogWarning( char* szFile, long lngLine, AString szMsg ) = 0;
	virtual BOOL LogTrace( char* szFile, long lngLine, AString szMsg) = 0;
	
	//copies the input file from input directory to archive directory with a unique file name
	//and opens archive file. On Success returns the name of Archive File
	//on error returns empty string ""
	virtual AString CopyInputToArchive(AString &szInputFullQName);
	//creates a unique filename with the extension ".imp"
	virtual AString CreateUniqueFileName();
	//determines if the current input directory contains files to be processed
	virtual BOOL CurrentInputDirectoryIsEmpty( void );
	
	virtual HANDLE FindFirstInputFile( LPCTSTR lpFilename, WIN32_FIND_DATA& ffd );

	//Since service can monitor multiple directories, the service needs to
	//be able to go on to service the next input directory
	virtual BOOL GetNextInputDirectory( void );
	//Get the next file in the input directory to be processed, this function then calls
	//processLoad() which effects file processing
	virtual BOOL ProcessInboundFiles();
	//starts up the service...calls initializeConfig() and processInboundFiles()
	virtual void ProcessThread();
	//stops the service thread
	virtual void StopProcessThread() { m_bContinueThread = false; }
	//When a file is being copied into the iput directory the service must
	//wait until the entire file is copied before attempting to process it
	virtual BOOL WaitUntilFileSizeDoesNotChange(WIN32_FIND_DATA & ffd);
		
protected:

	BOOL				m_bContinueThread;
	CWinThread			*m_pThread;
	BOOL				m_bLogging;
	std::list<DirData>	m_lstDirectories;	
	std::list<AString>	m_lstSupportedTypes;	
	AString				m_szInputFileLocation;
	AString				m_szArchiveFileLocation;
	AString				m_szErrorFileLocation;
	int					m_nMaxDir;
	int					m_nCurrentDir;
	ofstream			m_archiveFile;
	ofstream			m_errorFile;
	//must be assigned a value in/by derived classes
	//AString				m_szClassname; 
};

#endif // !defined(AFX_BASECTL_H__935F05C3_4F76_11D5_80AF_0050DAC00BBC__INCLUDED_)
