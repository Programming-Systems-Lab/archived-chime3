#include "cssysdef.h"
#include "cssys/sysdriv.h"
#include "csws/csws.h"
#include "version.h"
#include "ifontsrv.h"
#include "icfgnew.h"
#include "ChimeApp.h"
#include "ChimeWindow.h"

//stop 3D and open this window
ChimeWindow::ChimeWindow(csComponent *iParent, const char *iTitle, int iWindowStyle, 
						 csWindowFrameStyle iFrameStyle) 
	: csWindow(iParent, iTitle, iWindowStyle=CSWS_TITLEBAR | CSWS_BUTCLOSE | CSWS_BUTMAXIMIZE , 
				iFrameStyle=cswfs3D) {

		((ChimeApp*) parent)->Stop3D();
		csWindow::csWindow(iParent, iTitle, iWindowStyle, iFrameStyle);

}

//close this window and start animation
void ChimeWindow::Close() {	
	((ChimeApp *) parent)->Start3D();
	csWindow::Close();
}