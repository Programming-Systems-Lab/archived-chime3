#include <stdio.h>
#include <windows.h> /* For the CreateThread prototype */
#include "test.h"
//long WINAPI Validate(long); /* Function prototype */

HANDLE hFreshFile;
unsigned long iBytesWritten, iCount, iDataValue;

void main(void)
{ unsigned long iID;
   HANDLE hThread;

/* Step 1: Let the user input some data. */

  //hFreshFile = CreateFile("datafile",...)
  for (iCount = 0; iCount<100; iCount++)
{ printf("Please enter next data item: ");
  scanf("%d",&iDataValue);
//  WriteFile(hFreshFile,&iDataValue,sizeof(int),&iBytesWritten,NULL);
}
//CloseHandle(hFreshFile);

/* Dispatch a thread that does Step 2 (Validate the data) for you. */
test *tst = new test();
hThread = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)tst->Validate,NULL,0,&iID);

/* Step 3: Let the user enter more data. */
}

long WINAPI test::Validate(long lParam)
{ HANDLE hOldFile;
  int iTemp;
//hFreshFile = CreateFile("datafile",...)
//hOldFile = CreateFile("revenues.dat",...);
	return(1);
}

