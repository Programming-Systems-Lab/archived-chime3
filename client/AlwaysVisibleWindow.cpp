#include "cssysdef.h"
#include "cssys/sysdriv.h"
#include "csws/csws.h"
#include "version.h"
#include "ifontsrv.h"
#include "icfgnew.h"
#include "ChimeWindow.h"


//this is the window which is always visible in chime
AlwaysVisibleWindow::AlwaysVisibleWindow(csComponent *iParent, const char *iTitle, int iWindowStyle, csWindowFrameStyle iFrameStyle) 
	: csWindow(iParent, iTitle, iWindowStyle=CSWS_TITLEBAR | CSWS_BUTCLOSE | CSWS_BUTMAXIMIZE , iFrameStyle=cswfs3D) {
	csWindow::csWindow(iParent, iTitle, iWindowStyle, iFrameStyle);
}

bool AlwaysVisibleWindow::HandleEvent(iEvent &Event) {
	
	if (Event.Type == csevMouseMove || Event.Type == csevMouseDown) {
			int x = Event.Mouse.x;
			int y = Event.Mouse.y;
			if ((app->bound.xmin < x && x < app->bound.xmax) && (app->bound.ymin < y && y < app->bound.ymax)) {
				Select();
			}

			return csWindow::HandleEvent(Event);
	}

	else
		return csWindow::HandleEvent(Event);
}