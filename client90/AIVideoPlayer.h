// AIVideoPlayer.h: interface for the AIVideoPlayer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(_AIVIDEOPLAYER_H_)
#define _AIVIDEOPLAYER_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "AIVideoFrame.h"
#include "ivideo/fontserv.h"
#include "csgeom/csrect.h"
#include "imap/parser.h"
#include "isound/loader.h"
#include "isound/handle.h"
#include "isound/source.h"
#include "imesh/crossbld.h"
#include "imesh/sprite3d.h"
#include "ivideo/txtmgr.h"
#include "ivideo/natwin.h"
#include "ivaria/collider.h"
#include "ivaria/perfstat.h"
#include "ivaria/reporter.h"
#include "ivaria/stdrep.h"
#include "iutil/virtclk.h"
#include "ivideo/fontserv.h"
#include "iutil/plugin.h"
#include "imesh/mdlconv.h"
#include "imesh/crossbld.h"
#include "imap/parser.h"
#include "iengine/movable.h"
#include "iengine/motion.h"

class ChimeSystemDriver;
class AIVideoFrameBuffer;
class AIVideoFrame;
class AIVideoPlayerWindow;
class AISyncController;
class AICacheController;
class AICache;
class AICommController;
class AITimeController;
class csSchedule;


struct iPolygon3D;
struct iSoundSource;
class AIVideoPlayer  
{
public:
	static AIVideoPlayer* getInstance(ChimeSystemDriver *csd);
public:
	bool LOCKED;
	bool EXIT;
	void TestMusic();
	void ShutDown();
	static void TestMusicStatic();
	ChimeSystemDriver* Sys;
	AIVideoFrameBuffer* FrameBuffer;
	AICommController* Comm;
	AITimeController* TimeController;
	csSchedule* schedule;
	iSoundSource* sound;
	long int current_frame_number;
	void announceTime();
	void PlayAudio();
	void PauseAudio();
	void LoadAudio();

	char* frameDir; // the directory on disk where frames reside (absolute)
	char* frameDirVFS; // The Frame directory, as mapped to the VFS

	void Initialize();// test program to see if componenets work
	virtual ~AIVideoPlayer();
	csVector3 screen_position;
	int DrawScreen(csVector3 position);
	static int NUM_INSTANCES;	// limits concurrent execution...
	static int MAX_INSTANCES; 
	int DisplayFrame(iMaterialWrapper* frame_material);
	AIVideoFrame* createFrame( char* filename, int frame_number, int start, int end);
	void playThread(); // for testing
	iPolygon3D* screen; 
	AISyncController *SyncController;
	AICacheController *CacheController;
	void FrameManager(); // orchestrates downloading of frames....
	void runFrameManager();
// callback from timer that changes screen when possible
	void time_changed();

	// 2 debug functions to output to specified console/window/file,etc
	void debug(char* msg);
	void debug(int level,char* msg);
AIVideoPlayerWindow* PlayerWindow;
private :
	AIVideoPlayer(ChimeSystemDriver *csd);
	


};

#endif // !defined(_AIVIDEOPLAYER_H_)
