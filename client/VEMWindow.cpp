#define SYSDEF_PATH
#include "cssysdef.h"
#include "cssys/sysdriv.h"
#include "csws/csws.h"
#include "version.h"
#include "ifontsrv.h"
#include "icfgnew.h"
#include <string.h>
#include <winsock.h>
#include "ChimeWindow.h"

#define ADM 0
#define PWR 1
#define USR 2

typedef struct { char file [255], type [255], sub [255], object [255]; } FileData;

// Helper VeM class that handles notebook events
class VeMnb : public csDialog
{
	FileData *files;

  public:
	VeMnb (csComponent *iParent) : csDialog (iParent) {GetDefaults ();}

	virtual int GetDefaults ();

	// Handles all events that occur inside the notebook pages
	virtual bool HandleEvent (iEvent &Event)
	{
		if (Event.Type == csevCommand)
		{
			switch (Event.Command.Code)
			{

				case cscmdListBoxItemSelected:
				case cscmdListBoxItemClicked:
				{
					csListBoxItem *srcitem = (csListBoxItem *) Event.Command.Info;
					csListBox *srcbox = (csListBox *) srcitem -> parent;

					if (srcbox == GetChild (2000))
					{
						this -> parent -> parent -> GetChild (1000) -> SetText (files [srcitem -> id].file);
						this -> parent -> parent -> GetChild (1001) -> SetText (files [srcitem -> id].type);
						this -> parent -> parent -> GetChild (1002) -> SetText (files [srcitem -> id].sub);
						this -> parent -> parent -> GetChild (1003) -> SetText (files [srcitem -> id].object);
					}

					return true;
				}
				case 70002:
					if (this -> GetChild (3000) -> SendCommand (cscmdRadioButtonQuery))
						this -> parent -> parent -> GetChild (1000) -> SetText (
							this -> GetChild (2000) -> focused -> GetText ());
					else if (this -> GetChild (3001) -> SendCommand (cscmdRadioButtonQuery))
					{
						int i = '.';
						char *pdest = "";

						pdest = strrchr (this -> GetChild (2000) -> focused -> GetText (), i);
						this -> parent -> parent -> GetChild (1000) -> SetText (pdest);
					}
					return true;
					break;
				case 70003:
					this -> parent -> parent -> GetChild (1001) -> SetText (
						this -> GetChild (2001) -> focused -> GetText ());
					return true;
					break;
				case 70004:
					this -> parent -> parent -> GetChild (1002) -> SetText (
						this -> GetChild (2002) -> focused -> GetText ());
					return true;
					break;
				case 70005:
					this -> parent -> parent -> GetChild (1003) -> SetText (
						this -> GetChild (2003) -> focused -> GetText ());
					return true;
					break;
			}
		}

		return csDialog::HandleEvent (Event);
	}
};

// Gets all of the default settings for the files 
int VeMnb::GetDefaults ()
{
	int i, count = 0, flag = 0;
	char  x [255], y [255], w [255], z [255];

	FILE *fileData;

	if ((fileData = fopen ("VeM_Default.dat", "r")) == NULL)
		printf ("\n\nERROR: The file \"VeM_Default.dat\" cannot be found.\n\n");
	else
	{
		while (flag == 0)
		{
			if ((fscanf (fileData, "%s %s %s %s", &x, &y, &w, &z)) != EOF)
				count++;
			else
				flag = 1;
		}

		files = (FileData *) calloc (count, sizeof (FileData));

		fileData = fopen ("VeM_Default.dat", "r");

		for (i = 0; i < count; i++)
			fscanf (fileData, "%s %s %s %s\n", &files [i].file, &files [i].type, &files [i].sub, &files [i].object); 

		fclose (fileData);
	}

	return count;
}

// Creates the VeM interface
void VeM::Gui (int user)
{
  csComponent *v = this;

  ///////////////////////  Top Begin ///////////////////////////////////////////////////////////////
  
  csStatic *st = new csStatic (v, NULL, "", csscsFrameLabel);
  st->SetRect (3, 5, 489, 71);	

  st = new csStatic (v, NULL, "File: ");
  st->SetPos (11, 21);

  ilFile = new csInputLine (v, 1000, csifsThinRect);
  ilFile -> SetRect (48, 17, 148, 33);
  ilFile -> id = 1000;
  
  st = new csStatic (v, NULL, "Type: ");
  st->SetPos (8, 47);

  ilType = new csInputLine (v, 40, csifsThinRect);
  ilType -> SetRect (48, 43, 148, 59);
  ilType -> id = 1001;

  st = new csStatic (v, NULL, "Sub-Type: ");
  st->SetPos (164, 21);

  ilSub = new csInputLine (v, 40, csifsThinRect);
  ilSub -> SetRect (236, 17, 336, 33);
  ilSub -> id = 1002;
  
  st = new csStatic (v, NULL, "3D Object: ");
  st->SetPos (161, 47);

  ilObject = new csInputLine (v, 40, csifsThinRect);
  ilObject->SetRect (236, 43, 336, 59);
  ilObject -> id = 1003;

  csButton *butAccept = new csButton (v, 70000);
  butAccept->SetText ("Apply"); 
  butAccept->SetRect (376, 15, 456, 35);

  csButton *butReset = new csButton (v, 70001);
  butReset->SetText ("Clear"); 
  butReset->SetRect (376, 41, 456, 61);

  ///////////////////////  Top End ///////////////////////////////////////////////////////////////////////


  ///////////////////////  Notebook Begin  ///////////////////////////////////////////////////////////////

  csNotebook *nb = new csNotebook (v, CSNBS_TABPOS_LEFT | CSNBS_PAGEFRAME | CSNBS_PAGEINFO);
  v -> SendCommand (cscmdWindowSetClient, (void *) nb);

  VeMnb *page1 = new VeMnb (nb);
  nb->AddPrimaryTab (page1, "~Files", "Choose the file");
  nb->SetRect (3, 78, 489, 175);

  lbFile = new csListBox (page1, CSLBS_VSCROLL, cslfsThickRect);
  lbFile -> SetRect (155, 5, 375, 152);
  lbFile -> id = 2000;

  FillFiles ();

  if (user == ADM)
  {
	csRadioButton *rbOne = new csRadioButton (page1, cscmdNothing);
	rbOne -> SetPos (5, 5); 
	rbOne -> SetState (CSS_GROUP, true);
	rbOne -> SendCommand (cscmdRadioButtonSet, (void *) true);
	rbOne -> id = 3000;

	st = new csStatic (page1, rbOne, "This File");
	st -> SetPos (23, 7);

	csRadioButton *rbAll = new csRadioButton (page1, cscmdNothing);
	rbAll -> SetPos (5, 25);
	rbAll -> id = 3001;

	st = new csStatic (page1, rbAll, "This File Type");
	st -> SetPos (23, 27);
  }

  csButton *butAcceptFile = new csButton (page1, 70002);
  butAcceptFile -> id = 4002;
  butAcceptFile -> SetText ("Accept"); 
  butAcceptFile -> SetRect (37, 132, 117, 152);

  VeMnb *page2 = new VeMnb (nb);

  if (user != USR)
	nb -> AddPrimaryTab (page2, "~Type", "Choose the type of file");

  lbType = new csListBox (page2, CSLBS_VSCROLL, cslfsThickRect);
  lbType -> SetRect (155, 5, 375, 152);
  lbType -> id = 2001;

  FillListBox ("VeM_Types.dat", lbType);

  csButton *butAcceptType = new csButton (page2, 70003);
  butAcceptType -> id = 4003;
  butAcceptType -> SetText ("Accept"); 
  butAcceptType -> SetRect (37, 132, 117, 152);

  VeMnb *page3 = new VeMnb (nb);
  nb -> AddPrimaryTab (page3, "~Sub-Type", "Choose the Sub-Type of file");

  lbSub = new csListBox (page3, CSLBS_VSCROLL, cslfsThickRect);
  lbSub -> SetRect (155, 5, 375, 152);
  lbSub -> id = 2002;

  FillListBox ("VeM_Subs.dat", lbSub);

  csButton *butAcceptSub = new csButton (page3, 70004);
  butAcceptSub -> id = 4004;
  butAcceptSub -> SetText ("Accept"); 
  butAcceptSub -> SetRect (37, 132, 117, 152);

  VeMnb *page4 = new VeMnb (nb);

  if (user == ADM)
	  nb -> AddPrimaryTab (page4, "3D ~Object", "Choose the 3D object representing the file");

  lbObject = new csListBox (page4, CSLBS_VSCROLL, cslfsThickRect);
  lbObject -> SetRect (155, 5, 375, 152);
  lbObject -> id = 2003;

  st = new csStatic (page4, NULL, "Preview");
  st->SetPos (50, 17);

  FillListBox ("VeM_Objects.dat", lbObject);

  csButton *butAcceptObject = new csButton (page4, 70005);
  butAcceptObject -> id = 4005;
  butAcceptObject -> SetText ("Accept"); 
  butAcceptObject -> SetRect (37, 132, 117, 152);

  ///////////////////////  Notebook End  /////////////////////////////////////////////////////////////////
}

// Fills in all of the files from the default data file 
void VeM::FillFiles ()
{
	int count = 0;
	char  x [255], y [255], w [255], z [255];

	FILE *fileData;

	if ((fileData = fopen ("VeM_Default.dat", "r")) == NULL)
		printf ("\n\nERROR: The file \"VeM_Default.dat\" cannot be found.\n\n");
	else
	{
		while ((fscanf (fileData, "%s %s %s %s", &x, &y, &w, &z)) != EOF)
		{
			csListBoxItem *lstbxitm = new csListBoxItem (lbFile, x, 0, cslisEmphasized);
			lstbxitm -> id = count;
			count++;
		}

		fclose (fileData);
	}
}

// Fills in the list boxes using the data files
void VeM::FillListBox (char *which, csListBox *lstbx)
{
	int count = 0;
	char  x [255];

	FILE *fileData;

	if ((fileData = fopen (which, "r")) == NULL)
		printf ("\n\nERROR: The file \"%s\" cannot be found.\n\n", which);
	else
	{
		while ((fscanf (fileData, "%s", &x)) != EOF)
		{
			csListBoxItem *lstbxitm = new csListBoxItem (lstbx, x, 0, cslisEmphasized);
			lstbxitm -> id = count;
			count++;
		}

		fclose (fileData);
	}
}

// Sends an event to the Siena server 
void VeM::Publish (char *host, short port)
{
	WORD wVersionRequested = MAKEWORD (2,2); // Version of Winsock that is used <2.2>
	WSADATA wsaData;
	int nRet;

	nRet = WSAStartup (wVersionRequested, &wsaData); // Initializes Winsock

	// Checks that all fields are filled or nothing happens
	if ((strlen (ilFile -> GetText ()) != 0) && (strlen (ilType -> GetText ()) != 0) &&
		(strlen (ilSub -> GetText ()) != 0) && (strlen (ilObject -> GetText ()) != 0))
	{
		// Checks for the correct version of winsock
		if (wsaData.wVersion != wVersionRequested)
			printf ("\n\nError: Winsock did not Initialize Properly\n\n");
		else
		{
			LPHOSTENT lpht;
			SOCKET	s;
			SOCKADDR_IN saServer;

			lpht = gethostbyname (host);

			if (lpht == NULL)
				fprintf (stderr, "\n\nWinsock Error: Host Not Found\n\n");

			s = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP);
			
			if (s == INVALID_SOCKET)
				fprintf (stderr, "\n\nWinsock Error: Invalid Socket\n\n");

			saServer.sin_family = AF_INET;
			saServer.sin_addr = *((LPIN_ADDR)*lpht -> h_addr_list);
			saServer.sin_port = htons (port);

			if (connect (s, (LPSOCKADDR) &saServer, sizeof (struct sockaddr)) == SOCKET_ERROR)
			{
				fprintf (stderr, "\n\nWinsock Error: Unable to Connect\n\n");
				closesocket (s);
			}

			char publishString [1000]; 

			// Create Request 
			sprintf (publishString, "senp{method=\"PUB\" ttl=30 version=1.1 ");
			sprintf (publishString, "%sto=\"senp://", publishString);
			sprintf (publishString, "%s%s", publishString, host);
			sprintf (publishString, "%s:", publishString);
			sprintf (publishString, "%s%d\"}", publishString, port);

			// Create Event
			sprintf (publishString, "%s event{", publishString);
			sprintf (publishString, "%scomponent=\"VEM\"", publishString);
			sprintf (publishString, "%s file=\"%s\"", publishString, ilFile -> GetText ());
			sprintf (publishString, "%s type=\"%s\"", publishString, ilType -> GetText ());
			sprintf (publishString, "%s sub=\"%s\"", publishString, ilSub -> GetText ());
			sprintf (publishString, "%s object=\"%s\"}", publishString, ilObject -> GetText ());
			
			if (send (s, publishString, 1000, 0) == SOCKET_ERROR)
			{
				fprintf (stderr, "\n\nWinsock Error: Unable to Send\n\n");
				closesocket (s);
			}

			closesocket (s);
		}

		WSACleanup ();
	}
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////


VEMWindow::VEMWindow(csComponent *iParent)
  : AlwaysVisibleWindow(iParent, "VEM Console", CSWS_TITLEBAR | CSWS_BUTCLOSE |
    CSWS_BUTMAXIMIZE)
  {

  SetRect (app->bound.Width() / 4, app->bound.Height() / 4 * 3, app->bound.Width() / 2.25 + app->bound.Width() / 4, app->bound.Height());
  //SetState(CSS_DISABLED, true);
  
  //this -> SetSize (500, 285);
  //this -> Center ();
  
  VeM *v = new VeM (this);
  this -> SendCommand (cscmdWindowSetClient, (void *) v);

  v -> Gui (ADM);
}

