// AIDownloader.h: interface for the AIDownloader class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AIDOWNLOADER_H__D9C8CA21_2B3B_4B25_961C_CB35E7B638D8__INCLUDED_)
#define AFX_AIDOWNLOADER_H__D9C8CA21_2B3B_4B25_961C_CB35E7B638D8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include <urlmon.h>
#include <windows.h>

class AIDownloader 
{
public:
	void addMetrics(int file_size, int download_speed);
	// this is the proxy server manager. Proxy servers here refer to different
	// compression levels
	int numMeasurements;
	int SpeedArraySize; // used by dov to clock downloads
	int* SpeedArray;
	int SizeArraySize; // used by dov to predict future download sizes
	int* SizeArray;

static void getProxies(int videoID);
static int currentProxyID;
static char* baseURL;
static DWORD WINAPI downloadAsThread(LPVOID param);
public:
	// Downloads any url to any specified place
	// this returns right away, even if downloading not complete
	static int downloadToDisk(char* url,char* filename,bool separate_thread);
	
	// this returns when finished downloading
	static int downloadToDisk(char* url,char* filename);

	// this returns when finished downloading, as above, but also sets metrics in the cache
	int downloadFile(char* url,char* filename);

	//Downloads URL into the cache using cacheRoot folders
	int download(char* url, char* filename);
	
	void setup();
	// loops on a list of things to download
	void runDownloads();
	AIDownloader();
	int download(char* url);
	virtual ~AIDownloader();

};

#endif // !defined(AFX_AIDOWNLOADER_H__D9C8CA21_2B3B_4B25_961C_CB35E7B638D8__INCLUDED_)
