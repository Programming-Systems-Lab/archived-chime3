// AICacheController.h: interface for the AICacheController class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AICACHECONTROLLER_H__4738645D_CBBE_45F8_B437_E94FBF1F1B70__INCLUDED_)
#define AFX_AICACHECONTROLLER_H__4738645D_CBBE_45F8_B437_E94FBF1F1B70__INCLUDED_
#include "AICache.h"
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class AIDownloader;
class AIVideoPlayer;
class AICacheController  
{
public:
	AICache* cache;    // reference to the real cache API implementation
	AIDownloader* Downloader; // downloading engine to use instead of cache API temporarily
	AICacheController(char* dsn);
	int setupProject(int videoID, char* videoTitle, int compressionLevels[], int compLevelLength, int totalNumVideoFrames, int framesPerSecond);
	virtual ~AICacheController();
	int testDownload(char* url);
	int getAvgNetSpeed();
	int getAvgFileSize();
	int currentProjectID;
	void processNextFrame(AIVideoPlayer* player);
	void checkNetwork();

	void BetterCLevel(); // move up one comp. level
	void WorseCLevel(); // move down one comp. level

	// for bypass implementation of cache
	char* cacheRootPathHDD;  // on disk
	char* cacheRootPathVFS;	 // on VFS
	char* cacheRootURL;		 // up to the videoID

	// implementation of keeping track of files to display/download
	int setupFrameList(char* filename);
	int maxFrameNumber; // the total number of frames in the video
	int clevel_count;
	int* clevels; // array of levels, inherently also the cframe[]sizes
	int** cframes; // array of frame numbers for each level
	int** cframes_start;  
	int** cframes_end;

	int currentCLevelIndex;		// in the clevels[] array
	int currentFrameIndex; // in the cframes[] array
	
	bool EXIT;
};

#endif // !defined(AFX_AICACHECONTROLLER_H__4738645D_CBBE_45F8_B437_E94FBF1F1B70__INCLUDED_)
