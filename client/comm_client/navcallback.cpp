#include <windows.h>
#include <stdio.h>
#include "udp.h"

NavCallback::NavCallback()
{
	//create the Mutex object the first time we create this class
	hMutex=CreateMutex(NULL,FALSE,NULL); // create a mutex object with no name
}

void NavCallback::GetFunction(int method, char *received) 
{
	//keep everyone waiting
	WaitForSingleObject(hMutex,INFINITE); 

	//this is the lucky thread
	printf("\n\nIn Navdeep's Receiver\n");
	printf("\nThe method was: %d\n", method);
	printf("\nThe parameters are: %s\n", received);
	ReleaseMutex(hMutex);
}

