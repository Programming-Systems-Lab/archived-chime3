// AIVideoFrame.cpp: implementation of the AIVideoFrame class.
//
//////////////////////////////////////////////////////////////////////


#include "cssysdef.h"
#include "cssys/system.h"
#include "cssys/sysdriv.h"
#include "csws/csws.h"
#include "AIVideoFrame.h"

#include "version.h"
#include "ivideo/fontserv.h"
#include <sys/stat.h>
#include "csgeom/csrect.h"

#include "csengine/texture.h"
#include "imap/parser.h"

#include "isound/loader.h"
#include "isound/handle.h"
#include "isound/source.h"
#include "imesh/crossbld.h"
#include "imesh/sprite3d.h"
#include "ivideo/txtmgr.h"
#include "ivideo/natwin.h"
#include "ivideo/fontserv.h"
#include "iutil/plugin.h"
#include "imesh/mdlconv.h"
#include "imesh/crossbld.h"
#include "imap/parser.h"
#include "iengine/movable.h"
#include "iengine/motion.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

AIVideoFrame::AIVideoFrame()
{
	start=0;
	end=0;
	frame_number=0;
	filename="UNKNOWN.jpg";
}

AIVideoFrame::~AIVideoFrame()
{
	if (filename) delete filename;
	//if (materialWrapper) delete materialWrapper;
}

