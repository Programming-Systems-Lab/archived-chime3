// FRAXFilterUnitTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "FRAXFilterUnitTest.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// The one and only application object

CWinApp theApp;

using namespace std;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	// initialize MFC and print and error on failure
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: change error code to suit your needs
		cerr << _T("Fatal Error: MFC initialization failed") << endl;
		nRetCode = 1;
	}
	else
	{
		CoInitialize( NULL );
/*Barbados Tourism Authority*/
		FRAXFilter f;
		
		f.FilterDocSummaryInfoSection( "<DocSummaryInfo><Category></Category><PresFormat></PresFormat><ByteCount>37888</ByteCount><LineCount>93</LineCount><ParagraphCount>29</ParagraphCount><SlideCount></SlideCount><NoteCount></NoteCount><HiddenCount></HiddenCount><MultiMediaClipCount></MultiMediaClipCount><Scale>FALSE</Scale><Manager></Manager><Company>Barbados Tourism Authority</Company><LinksDirty>FALSE</LinksDirty><HyperLinksChanged>FALSE</HyperLinksChanged></DocSummaryInfo>" );
		f.FilterHyperlinksInfoSection( "<HLinksInfo><HLINK_DATA>http://www.abc.com/;http://www.msn.com/</HLINK_DATA></HLinksInfo>" );
		f.FilterSummaryInfoSection( "<SummaryInfo><Title>August 8, 2001</Title><Subject></Subject><Author>TyroneCl</Author><Keywords></Keywords><Comments></Comments><Template>Normal</Template><LastAuthor>Rean Griffith</LastAuthor><RevisionNumber>5</RevisionNumber><EditTime>12:52428:52428 pm Fri 52428-52428-52428</EditTime><LastPrinted>11:30:00 am Fri 07-13-2001</LastPrinted><Created>6:49:00 pm Sat 01-19-2002</Created><LastSaved>9:07:00 am Thu 01-24-2002</LastSaved><PageCount>3</PageCount><WordCount>615</WordCount><CharCount>3368</CharCount><Thumbnail></Thumbnail><AppName>Microsoft Word 9.0</AppName><DocSecurity>0</DocSecurity></SummaryInfo>" );

		CoUninitialize();

	}

	return nRetCode;
}


