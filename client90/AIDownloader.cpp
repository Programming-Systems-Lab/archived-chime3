// AIDownloader.cpp: implementation of the AIDownloader class.
//  This is the worker class dispatched to download from a given
//  proxy server...
//////////////////////////////////////////////////////////////////////

#include "AIDownloader.h"
#include <windows.h>
#include <urlmon.h>
#include <stdio.h>
#include <time.h>


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////



AIDownloader::AIDownloader()
{

	AIDownloader::baseURL = "http://localhost:60200/";// NOT USED
	
	numMeasurements=0;
	
	SpeedArraySize = 3; // by default
	SpeedArray = new int[SpeedArraySize];
	SizeArraySize = 3 ; // by default
	SizeArray = new int[SizeArraySize];

	for (int i=0; i < SpeedArraySize;++i){
		SpeedArray[i]=-1;
	}
	for (int j=0; j < SizeArraySize;++j){
		SizeArray[j]=-1;
	}

}

char* AIDownloader::baseURL = "http://localhost:60200/";// NOT USED

AIDownloader::~AIDownloader()
{


}
// test stuff, never used	
void AIDownloader::runDownloads(){
// Dov's old code to test download functionality
/*	int files_length=166;
	for(int i=0; i < files_length; i++){ 
		char* finalURL = char*(AIDownloader::baseURL->c_str());
		finalURL += files[i];
		download(finalURL,files[i]);
	}*/

  }

int AIDownloader::download(char* url, char* filename)
{

printf("\ndownloading %s to %s\n",url,filename);
//char cachePath[300]; 
//sprintf(cachePath,"%s%s", AIDownloader::baseURL,filename);
CoInitialize(NULL);
int x= URLDownloadToFile(NULL, url, filename, 0, NULL);
return x;
}

DWORD WINAPI AIDownloader::downloadAsThread(LPVOID param){
	CoInitialize(NULL);
	printf("\n[NEW DOWNLOADER PLEASE*******] %s -> %s\n",(((char**)param)[0]),(((char**)param)[1]));
	int x= URLDownloadToFile(NULL, (((char**)param)[0]), (((char**)param)[1]), 0, NULL);
	return x;
}
int AIDownloader::downloadToDisk(char* url, char* filename, bool separate_thread)
{
	if(separate_thread){
		return downloadToDisk(url,filename);
	} else {
		DWORD threadId1;
		char** params = new char*[2];
		params[0] = url;
		params[1] = filename;
		CreateThread(NULL,0,downloadAsThread,params,0,&threadId1);
		return 0;
	}
}


// comment out the printfs
int AIDownloader::downloadToDisk(char* url, char* filename)
{

printf("\ndownloading %s to %s\n",url,filename);
CoInitialize(NULL);
int x= URLDownloadToFile(NULL, url, filename, 0, NULL);
if (x == 0){
printf("\n[DOWNLOAD_SUCCESS] %s -> %s\n",url,filename);

} else {
printf("\n[DOWNLOAD_FAILURE] %s -> %s\n",url,filename);

}
return x;
}


int AIDownloader::downloadFile(char* url, char* filename)
{

printf("\ndownloading %s to %s\n",url,filename);
CoInitialize(NULL);
time_t start = 0;
time_t end=0;
start=time(NULL);
int x= URLDownloadToFile(NULL, url, filename, 0, NULL);
end=time(NULL);
if (x!=0) return -1;

int totaltime = (int)(difftime(end,start));
//totaltime;
if (totaltime==0) totaltime=1;

FILE* fp = fopen(filename,"r");
long fsize = ftell(fp);
fclose(fp);
addMetrics((int)fsize,totaltime);
if (x == 0){
printf("\n[DOWNLOAD_SUCCESS] %s -> %s\n",url,filename);

} else {
printf("\n[DOWNLOAD_FAILURE] %s -> %s\n",url,filename);

}
return x;
}

// test stuff, never used
int AIDownloader::download(char* url){
	return download(url,"c:\\_ai2tv.ppm");
}
// test stuff, never used
void AIDownloader::setup(){
	
}
// test stuff, never used
void AIDownloader::getProxies(int videoID){
  // query global database for proxy list for this video
}


	
/*************************
/* addMetrics - callback method for measuring bandwidth
/* added by DOV
/*************************/

void AIDownloader::addMetrics(int file_size, int download_speed){
	SizeArray[numMeasurements % SizeArraySize]=file_size;
	SpeedArray[numMeasurements % SpeedArraySize]=download_speed;
	numMeasurements++;
}
