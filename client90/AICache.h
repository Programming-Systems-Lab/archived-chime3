// AICache.h: Interface for the AICache class.
//
/////////////////////////////////////////////////////////////////////

#ifndef AICACHE_H
#define AICACHE_H

#ifdef AICACHE_EXPORT
#define AICacheAPI _declspec(dllexport)
#else
#define AICacheAPI _declspec(dllimport)
#endif

//#include <afxtempl.h>
#include <SQLtypes.h>
//#include <afxdb.h>
#include <string>

//typedef basic_ofstream<char, char_traits<char> > ofstream;
//typedef CMap<SWORD,SWORD,CString,LPCSTR> CMapSQLTypeToSyntax;

class AICacheAPI AICache
{

protected:
	static int instanceCount;  // HOW OR WHERE DO I INITIALIZE THIS TO ZERO ????
	//char* strDataSource;
	char strDataSource[255];
	//std::string *cacheRootPath;


public:
	//std::string *cacheRootPath;

public:
	//AICache(string dataSource);
	//AICache();
	AICache(char* dataSource);
	~AICache();

	// Internal methods
	BOOL GetColumnSyntax();
	char* AICache::IntToStr(int nVal);
	//std::string IntToStr(int nVal)
	int MaxComprLevel(char* cLevels);
	void InitCache();

	// Cache setup methods
	BOOL setupCache();
	BOOL purgeCache();

	int createProject(int videoID, char* videoTitle, int compressionLevels[], int compLevelLength, int totalNumVideoFrames, int framesPerSecond);
	BOOL deleteProject(int projectID);
	char* getProjectPath(int projectID);
	
	// VIDEO API methods
	char* getNextN(int currentFrameNum, int count, int projectID);
	char* getFrame(int currentFrameNumber, int projectID);
	int putFrame(int projectID, int frameNumber, char* fileType, int compressionLevel, int startFrameId, int endFrameId, char* URL);
	
	// AUDIO API methods
	BOOL putAudio(int projectID, char* audioFilename, int startFrameId, int lengthInFrames, char* retrieveURL);
	int audioStatus(int projectID);
	char* getAudioURL(int projectID, int startFrameID);
	char* getAudioFilename(int projectID, int startFrameID);

	char* getSMILFile(int projectID);

	BOOL optimizeCache();

protected:
	// Internal cache methods
	BOOL AddTable(const char* strTableName, const char* strColumns);
	BOOL AddProjectTable(int projectID, int totalNumOfFrames,  int *compressionLevels, int numOfCompressionLevels);
	void ReportEventLog( LPCTSTR szEvent );
	//BOOL optimizeCache();
	BOOL generateSMIL(int projectID);
	BOOL mergeMPEG(int projectID);

};

class cache_err
{
public:
	char errMsg[1000];
	cache_err(char* errorMessage)
	{
		strcpy(errMsg, errorMessage);
	}
};

#endif
