// AIVideoPlayer.cpp: implementation of the AIVideoPlayer class.
//
//////////////////////////////////////////////////////////////////////

#include "cssysdef.h"
#include "cssys/system.h"

#include "cssys/sysdriv.h"
#include "csws/csws.h"
#include "version.h"
#include "ivideo/fontserv.h"
#include <sys/stat.h>
#include "csgeom/csrect.h"

#include "csutil/schedule.h"

#include "AIVideoPlayer.h"
#include "AIVideoFrame.h"
#include "AIVideoFrameBuffer.h"
#include "ChimeSystemDriver.h"
#include "ChimeSector.h"
#include "WindowToolkit.h"
#include "ChimeApp.h"

#include "csengine/engine.h"
#include "csengine/texture.h"
#include "imap/parser.h"

#include "cstool/csview.h"
#include "cstool/initapp.h"
#include "csengine/sector.h"
#include "csengine/polygon.h"
#include "isound/loader.h"
#include "isound/handle.h"
#include "isound/source.h"
#include "isound/renderer.h"
#include "imesh/crossbld.h"

#include "imesh/sprite3d.h"
#include "ivideo/codec.h"
#include "ivideo/txtmgr.h"
#include "ivideo/natwin.h"
#include "ivaria/collider.h"
#include "ivaria/perfstat.h"
#include "ivaria/reporter.h"
#include "ivaria/stdrep.h"
#include "iutil/virtclk.h"
#include "ivideo/fontserv.h"
#include "iutil/plugin.h"
#include "cstool/mdldata.h"
#include "cstool/mdltool.h"
#include "imesh/mdlconv.h"
#include "imesh/crossbld.h"
#include "imap/parser.h"
#include "iengine/movable.h"
#include "iengine/motion.h"
#include "AISyncController.h"
#include "./comm_client/ClientComm.h"
#include "AIVideoPlayerWindow.h"
#include "AI2TVConstants.h"
#include "AICacheController.h"
#include "AIVideoFrameBuffer.h"
#include "AIVideoFrame.h"
#include "AICache.h"
#include <string.h>
#include "AICommController.h"
#include "AITimeController.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


AIVideoPlayer::AIVideoPlayer(ChimeSystemDriver *csd)
{
	sound=0;
	LOCKED=false;
	EXIT=false;
	// NEW: Create csSchedule
	schedule = new csSchedule();

	// Establish reference to ChimeSystem Driver
	Sys  = csd;	
	TimeController= new AITimeController(this);
	// set current frame number to -1; 
	current_frame_number=-1;

	// create new communication parser
	Comm = new AICommController();
	SyncController = new AISyncController(csd);
	// init some vars, that will be overwritten before used...
	screen=0;
	screen_position=csVector3(0,0,0); //start off at the origin
	
	// FrameDir will be the hardcoded directory, that the cache sets up for this vid.
	frameDir="";

	// FrameDirVFS is the dynamic VFS mapping we will use for accessing 
	// newly downloaded frames, for texture / material creation (crucial to 
	// displaying the frames in Crystalspace)
	frameDirVFS=new char[150];
	sprintf(frameDirVFS,"/lib/ai2tv/AI2TV_FRAMES_PLAYERINSTANCE_%d\0",NUM_INSTANCES);
	
	// Open the CrystalSpace control GUI window
	PlayerWindow = new AIVideoPlayerWindow(Sys->GetApp());
	
	// diagnostic stuff
	char message[200];
	sprintf(message,"Frames for this player will load into VFS://%s",frameDirVFS);
	debug(1,message);
	
	// establish "probes" to manage synchronization, and cache
	//SyncController = new AISyncController(csd); 
	CacheController = new AICacheController("AI2TV_CACHE"); // FIX, make inputtable on GUI

	// test out the synccontrollers server-client architecture
	//TestMusic();
}

AIVideoPlayer::~AIVideoPlayer()
{
	// kill framebuffer (FrameBuffer)
	// kill schedule, TimeController, Comm, 
	//  SyncController, CacheController
	// frameDirVFS
}

// AI2TV AUDIO FUNCTIONS

void AIVideoPlayer::LoadAudio(){
	if (sound != 0) return; // music already loaded
	// static music audio file for now...
	const char* music_file = "/lib/ai2tv/track4.wav";
	iSoundWrapper* isw=0;
	iSoundData* isd =0;
	iSoundHandle* ish=0;
	ish= Sys->LevelLoader->LoadSound(music_file);
//	isw=	Sys->LevelLoader->LoadSound("dov_mp3","/lib/ai2tv/dov.mp3");
//	if (ish!=0){
	sound = ish->CreateSource(0);
	if (sound!=0) {
	    	debug(1,"Sound Loaded Successfully");
	}
		else
			debug(1,"Sound Load Failed");

}
void AIVideoPlayer::PlayAudio(){
	if (sound){
		debug(1,"Playing Audio"); 
		sound->Play(false);
	}
}
void AIVideoPlayer::PauseAudio(){
	if (sound) 
	{
		debug(1,"Pausing Audio");
		sound->Stop();
	}
}

// test loading music
void AIVideoPlayer::TestMusic(){
	if (sound != 0) return;
	const char* music_file = "/lib/ai2tv/track4.wav";
	iSoundWrapper* isw=0;
	iSoundData* isd =0;
	iSoundHandle* ish=0;
	iSoundSource* sound=0;
	ish= Sys->LevelLoader->LoadSound(music_file);
//	isw=	Sys->LevelLoader->LoadSound("dov_mp3","/lib/ai2tv/dov.mp3");
	if (ish!=0){
		sound = ish->Play(false);
	//	if (sound) sound->Stop();
		debug(1,"SOUND LOADED!!!");
	} else
			debug(1,"SOUND NOT LOADED!!!");

}

// test loading music
void AIVideoPlayer::TestMusicStatic(){
	extern ChimeSystemDriver *Sys;
	const char* music_file = "/lib/ai2tv/dov.mp3";
	iSoundWrapper* isw=0;
	iSoundData* isd =0;
	iSoundHandle* ish=0;
	iSoundSource* iss=0;
	ish= Sys->LevelLoader->LoadSound("/lib/ai2tv/track4.mp3");
//	isw=	Sys->LevelLoader->LoadSound("dov_mp3","/lib/ai2tv/dov.mp3");
	if (ish!=0){
		
	//ish->StartStream(false);
		ish->StartStream(false);
		iss=ish->Play(false);
		Sys->myVideoPlayer->debug(1,"SOUND LOADED!!!");
	} else
			Sys->myVideoPlayer->debug(1,"SOUND NOT LOADED!!!");

}
	/**************************************
	*
	*	AI2TV Implementation:
	*
	*	GetInstance, is the accessor to the protected constructor
	*	Currently, only 1 instance is allowed per client.
	*	
	***************************************/
	

	AIVideoPlayer* AIVideoPlayer::getInstance(ChimeSystemDriver *csd){
		if( NUM_INSTANCES < MAX_INSTANCES){
			++NUM_INSTANCES;
		
			// create player
			AIVideoPlayer* player = new AIVideoPlayer(csd);
		
			// give it a frame buffer
			player->FrameBuffer = new AIVideoFrameBuffer(20);
			player->Initialize(); // for debugging
			return player;
		} else return 0;
	}

/************************************
*
*	Draws the "screen" on the 
*
**************************************/
int  AIVideoPlayer::DrawScreen(csVector3 position){
	

	iTextureManager* txtmgr = Sys->myG3D->GetTextureManager ();
	iTextureWrapper *txt;
	iMaterialWrapper *mw= Sys->engine->GetMaterialList()->FindByName ("halldoor");
	char name[100] = "AI2TV_SCREEN";
	char file[100] ="/lib/ai2tv/AITV.jpg";

	if(txt=Sys->LevelLoader->LoadTexture (name,file,CS_TEXTURE_3D,txtmgr)){
	
		txt->GetTextureHandle()->Prepare();
		txt->Register(txtmgr);
		mw= Sys->engine->GetMaterialList()->NewMaterial(txtmgr->RegisterMaterial(txt->GetTextureHandle()));
		if(mw==0)Sys->Report (CS_REPORTER_SEVERITY_ERROR, "mw is null!!!");
	} else	mw= Sys->engine->GetMaterialList()->FindByName ("halldoor");
	
	debug(1,"Displaying Screen on wall...");

	/*
	iSoundHandle * sh =NULL;
	iSoundSource * ss = NULL;
	char mp3file[100] ="/lib/ai2tv/dov.mp3";
	sh =LevelLoader->LoadSound(mp3file);
	if (sh != 0)
		
		sh->StartStream(false);
	Report(CS_REPORTER_SEVERITY_ERROR, "music to my ears!");	
	*/
	//Prepare the whole room.

	csVector3 doorSize(0, 4, 3.5);
	csVector3 doorTxtSize(3.5,4,0);
	csVector3 meshPos(4.99, 0, 0);

	ChimeSector *sec2 = Sys->GetCurChimeSector();
	iSector* room;
	room = sec2->GetRoom(0);
	screen = sec2->BuildScreenOnWall(sec2->GetRoom(0), position, csVector3(0,0,0), doorSize, mw, doorTxtSize);
	screen_position=position;
	room->ShineLights ();
	return 1;

}

/*******************
**
**	static vars
*********************/
int  AIVideoPlayer::NUM_INSTANCES =0; // # of instances running
int AIVideoPlayer::MAX_INSTANCES =1; // currently allowing only 1

/************************************************************
**
**  Debugging function, used to pipe output to the
**  requested console/file/etc... A level, as defined here, 
**  is a place for you to add different outputs, and add cases
**  to the switch statement to handle debugger calls to those
**  outputs.
**
************************************************************/

// default debug output mode
void AIVideoPlayer::debug(char* msg){
	// cause to get handled by default case
	debug(-9999999,msg);
}

// specify the debug output mode... 
void AIVideoPlayer::debug(int level,char* msg){
	try{
	switch(level){
	case 1:
			if (Sys && Sys->GetApp() && Sys->GetApp()->historyWindow)
			Sys->GetApp()->historyWindow->AddItem(msg);
			break;
	case 2:
			if (Sys && Sys->GetApp() &&  Sys->GetApp()->chatWindow)
			Sys->GetApp()->chatWindow->ShowMessage(msg);
			break;
	case 3:
			printf("%s\n",msg);
			break;

	// By default pipe to stdout
	default:
		printf("%s\n",msg);
		break;
	}
	} catch(...){
		//error. no way to debug
			printf("DEBUG ERROR ATTEMPTED:%s\n",msg);
	}
}



/***********************************************
*	
*	AIVideoFrame*  AIVideoPlayer::createFrame()
*
*  Param: char* filename - the grapic file - in VFS  
*  Param: int frame_number - the position in the video
*  Param: int start, end - the duration in frame numbers
*		when this frame is to be displayed
*
*
* creates a frame (instance of AIVideoFrame) and puts 
* it on the heap. The reference returned can then be stored
* in a frame buffer or immediately, you can just use the
* texturehandle/wrapper with the crystalspace system..
* 
*************************************************/
AIVideoFrame*  AIVideoPlayer::createFrame(char* filename, int frame_number, int start, int end){
	
	AIVideoFrame* fr = new AIVideoFrame();
	iTextureManager* txtmgr = Sys->myG3D->GetTextureManager ();
	iTextureWrapper *txt;
	iMaterialWrapper *mw =0;
	char name[100];
	char vfs_filename[1000];
	// chance to "/lib/ai2tv%s" below if not calling exclusivelyfrom cache controller
	sprintf(vfs_filename,"%s",filename);
	sprintf(name,"fr_%d",frame_number);
	if(txt=Sys->LevelLoader->LoadTexture (name,vfs_filename,CS_TEXTURE_3D,txtmgr)){
		txt->GetTextureHandle()->Prepare();
		txt->Register(txtmgr);
		mw= Sys->engine->GetMaterialList()->NewMaterial(txtmgr->RegisterMaterial(txt->GetTextureHandle()));
		if(mw==0)Sys->Report (CS_REPORTER_SEVERITY_ERROR, "mw is null!!!");
		char msg[1000];
		sprintf(msg,"TRIED TO CREATE FRAME ON %S with name %s",filename,name);
		Sys->Report (CS_REPORTER_SEVERITY_ERROR,msg);
	} else	mw= Sys->engine->GetMaterialList()->FindByName ("halldoor");
	fr->filename=filename;
	fr->start=start;
	fr->frame_number=frame_number;
	fr->end=end;
	fr->materialWrapper=mw;
	return fr;
}

/*************************
*
*	displAY a frame on screen
*
***************************/
int AIVideoPlayer::DisplayFrame(iMaterialWrapper *frame_material){
	while(LOCKED){}
	LOCKED=true;
	if (screen != 0 && 0) 
	{
		screen->SetMaterial(frame_material);
		ChimeSector *sec2 = Sys->GetCurChimeSector();
		iSector* room;
		room = sec2->GetRoom(0); 
	//	Sys->GetApp()->historyWindow->AddItem("Just displayed a frame on iPolygon3d");
		room->ShineLights();
		//Sys->GetApp()->Draw();
	} else {
		if (frame_material==0){
				//Sys->GetApp()->historyWindow->AddItem("I was given an empty material! Not drawing frame");
			LOCKED=false;
			return 0;

		}

	// hardcoded scaling and tiling matrices for throwing the frame/texture
	// onto the wall
	csVector3 frameSize(0.01, 2.25, 2.9);
	csVector3 frameTxtSize(2.9, 2.25, 0);
	
	// ChimeSector is the actual room we are in. We use this object's methods
	// to actually map the texture to the screen/wall, etc.
	ChimeSector *sec2 = Sys->GetCurChimeSector();
	iSector* room;
	room = sec2->GetRoom(0);

	// This chimeSector method is used to actually put the frame on the wall...
	iMeshWrapper *doormesh = sec2->BuildSideDoor(sec2->GetRoom(0), screen_position, csVector3(-0.5,1.6,0.5), frameSize, frame_material, frameTxtSize);
	room->ShineLights ();
	}
LOCKED=false;
return 1;
} 

/*******************
**
**  global function for testing threaded display
**
**/
int curframe = 0;
bool frames_loaded=false;
AIVideoFrame* framies[100];

void loadFrames(AIVideoPlayer *player){
	player->debug(1,"Video Player: Loading Frames");
	frames_loaded=true;
	while(curframe < 21 && !player->FrameBuffer->isFull()){
	char buf[50];

	sprintf(buf,"%d.jpg\0",curframe);
	AIVideoFrame* fr = player->createFrame(buf,curframe++,0,0);
	player->FrameBuffer->putFrame(fr);
	char msg2[1000];
	player->Sys->Report(CS_REPORTER_SEVERITY_ERROR,buf);
	sprintf(msg2,"Frame buffer Size: %d, head=%d.tail=%d",	player->FrameBuffer->size,	player->FrameBuffer->getHead(),	player->FrameBuffer->getTail());
	player->Sys->Report(CS_REPORTER_SEVERITY_ERROR,msg2);
//	framies[curframe++]=fr;
	}
	curframe=0;
	frames_loaded=true;
	player->Sys->Report(CS_REPORTER_SEVERITY_ERROR,"done loading the frames");
}


DWORD WINAPI runShowFrame(LPVOID lParam){

	AIVideoPlayer *player = (AIVideoPlayer*)lParam;
		try {
/*	if (!frames_loaded){
		player->Sys->GetApp()->historyWindow->AddItem("Frames Not loaded. Loading frames");
		player->Sys->Report(CS_REPORTER_SEVERITY_ERROR,"FRAMES NOT LOADED. LETS LOAD THEM");
	//	loadFrames(player);
	} */

	AIVideoFrame* myframe=0;

	if((myframe==0) && 	player->FrameBuffer->size >0){
		myframe=player->FrameBuffer->nextFrame();
		player->Sys->Report(CS_REPORTER_SEVERITY_ERROR,"skipped a frame");
		char msg2[1000];
	sprintf(msg2,"Frame buffer Size: %d, head=%d.tail=%d",	player->FrameBuffer->size,	player->FrameBuffer->getHead(),	player->FrameBuffer->tail);
	player->Sys->Report(CS_REPORTER_SEVERITY_ERROR,msg2);
	player->debug(1,msg2);
	}

	if (myframe!=0) {
		if (myframe->materialWrapper !=0){
			player->DisplayFrame(myframe->materialWrapper);
			player->Sys->Report(CS_REPORTER_SEVERITY_ERROR,"displayed frame...");
		}
	}

 
	//if(player->FrameBuffer->size > 0){
//		AIVideoFrame *f = player->FrameBuffer->nextFrame();
//		if (f!=0){
//		player->DisplayFrame(f->materialWrapper);
//		}
	//Sleep(2000);
	//}
		} catch (...) {
	player->Sys->Report(CS_REPORTER_SEVERITY_ERROR,"EXCEPTION :(...");
	}
	return(0);

}

void AIVideoPlayer::playThread(){
	DWORD threadId;	
	CoInitialize(NULL);
 	CreateThread(NULL,0,runShowFrame,this,0,&threadId);
}

/*****************************
*	attempt at shutting down player bug free
*
*******************************/

void AIVideoPlayer::ShutDown(){
	debug(1,"Shutting Down....");
	EXIT=true;
	TimeController->EXIT=true;
//	Sleep(10000);
//	delete TimeController;
//	delete SyncController;
//	delete CacheController;
	
//	delete Comm;
//	delete frameDirVFS;
//	delete FrameBuffer;
}

/************************
/*
/*	this will run in a separate thread
/*	utilizing the frames_loaded, the network measurement
/*	and the file buffer, to trudge forward, make and display frames;
/*
/*************************/

void AIVideoPlayer::FrameManager(){
	while(1 && !EXIT){
		// while	i have space in the buffer AND
		//			I have not reached the end of the film AND
		//			i have not reached the end of this CLevel
			if (!FrameBuffer ||  !CacheController ) return;
		 
			while(FrameBuffer && !EXIT && CacheController &&	!FrameBuffer->isFull() && CacheController->currentFrameIndex < CacheController->clevels[CacheController->currentCLevelIndex])
			CacheController->processNextFrame(this);
		
	}
}

void AIVideoPlayer::announceTime(){
	Comm->broadcast(AI2TV_NOTE);
}

// static function to thread above funct
DWORD WINAPI callRunFrameManager(LPVOID lParam){
	AIVideoPlayer* player = (AIVideoPlayer*)lParam;
	player->FrameManager();
	return 0;
}


// used to call the global thread calling function...
void AIVideoPlayer::runFrameManager(){
	DWORD threadId;
	CoInitialize(NULL);
	CreateThread(NULL,0,callRunFrameManager,this,0,&threadId);
}



/************************8
/*
/*	 init - get settings from file
/*************************/

void AIVideoPlayer::Initialize(){
	char* my_msg= new char[200];

	FILE* session;
	int num_clients = 0;
	int*	clientIDs =0;
	char** ipaddresses;
	char* frame_list_file= new char[200];
	char*url= new char[255];
		
	if (session=fopen("./data/ai2tv/session.txt","r")){
		// get the # of clients
		fscanf(session,"%d",&num_clients);
		ipaddresses = new char*[num_clients];
		clientIDs =new int[num_clients];

		// for each client get info
		for (int x = 0; x < num_clients; ++x){
			ipaddresses[x] = new char[20];
			fscanf(session,"%d %s",&(clientIDs[x]),ipaddresses[x]);	
			}
		// now get the Base URL for the JPEGS, and the name of the list file
			fscanf(session,"%s",url);
			CacheController->cacheRootURL = url;
			fscanf(session,"%s",frame_list_file);
		// ok, in theory if no errors, we are done here...
			fclose(session);
			SyncController->setMultiCastGroup(num_clients,ipaddresses,clientIDs);
			debug(1,"Loaded Session File");
	
	} else {
		debug(1,"ERROR! CANNOT OPEN SESSION FILE! PLEASE RESTART");
		delete this;
		return;
	};
	
	//setting roots for vfs and harddisk paths
	CacheController->cacheRootPathHDD="./data/ai2tv/proj25";
	CacheController->cacheRootPathVFS="/lib/ai2tv/proj25";
	//CacheController->cacheRootURL="http://www.cs.columbia.edu/~dk315/ai2tv/1";

	if (CacheController->setupFrameList(frame_list_file)){
	sprintf(my_msg,"AIFL: CL%dMF%d\0",CacheController->clevel_count,CacheController->maxFrameNumber);
 	debug(1,my_msg);

	}	else {
	debug(1,"FRAME LIST SETUP FAILED");
	}
	LoadAudio();

	runFrameManager();
		return;
//	sprintf(my_msg,"AvgFileSize:%d AvgNetSpeed:%d\0",CacheController->getAvgFileSize(),CacheController->getAvgNetSpeed());
 
//	debug(1,my_msg);
//	LoadAudio();
	return; 
	//set up some vars to make a new project
	int videoID = 1234567;
    char* videoTitle="Testing video player...:)";
	int CompressionLevels[3]={33,50,100};
	int compLevelLength=3;
	int totalNumVideoFrames = 200;
	int framesPerSecond = 30;
	// test cachecontroller 

	//	char* message="\0";
	int projID=0;

	//	projID = CacheController->cache->setupProject(videoID, videoTitle, CompressionLevels,  compLevelLength, totalNumVideoFrames,  framesPerSecond);	
	projID=25;
	
	//	return;

	sprintf(my_msg,"loaded project %d\0",projID);
	if (projID < 0 ) projID *=-1;
	debug(1,my_msg);

	///const char *projPath=CacheController->cache->getProjectPath(abs(projID));
	///frameDir = "\0";
	frameDir = new char[200];  //   
	// const char* test;  //   
	try{
	//CacheController->cache->getProjectPath(projID);
	sprintf(frameDir,"./data/proj25");// 
	debug(1,frameDir);
	} catch(...){}

	//	CacheController->cache->putFrame(projID,1,1,3,1,5,"http://photos.onlysimchas.com/pix/g288/tn_OSPGEN_10.jpg");
	//	CacheController->cache->putFrame(projID,2,2,6,1,3,"http://photos.onlysimchas.com/pix/g288/tn_OSPGEN_11.jpg");
	//	CacheController->cache->putFrame(projID,4,4,6,4,6,"http://photos.onlysimchas.com/pix/g288/tn_OSPGEN_12.jpg");
	//	CacheController->cache->putFrame(projID,6,6,9,1,6,"http://photos.onlysimchas.com/pix/g288/tn_OSPGEN_13.jpg");
	//	CacheController->cache->putFrame(projID,7,7,9,7,7,"http://photos.onlysimchas.com/pix/g288/tn_OSPGEN_14.jpg");
	try{
		//CacheController->cache->putFrame(projID,11,11,33,11,33,"http://photos.onlysimchas.com/pix/g288/tn_OSPGEN_15.jpg");
	} catch(...){}

	sprintf(my_msg,"created project %d path=%s:\0",projID, frameDir);
	debug(1,my_msg);
	//	this->Sys->GetApp()->historyWindow->AddItem("test");
	//	this->Sys->GetApp()->historyWindow->AddItem("Test");	
	//	} catch (...){Sys->GetApp()->historyWindow->AddItem("OH FUCK");}
	// mount the vfs path
	try{
     
	}
	catch(...){//debug(1,"VFS MOUNT BAD");
	}	

	frameDirVFS = "/lib/ai2tv/proj22";
	// Since lib/ai2tv is mounted, we should not need to mount
	// subdirectories, but rather, we can access directly
	//Sys->myVFS->Mount(frameDirVFS,"./data/ai2tv/proj22");
	
	int i=1;
	
	sprintf(my_msg,"%s/%d.jpg\0",frameDirVFS,i);
	FrameBuffer->putFrame(createFrame("proj22/1.jpg",1,1,5));
	FrameBuffer->putFrame(createFrame("proj22/11.jpg",2,1,5));
	FrameBuffer->putFrame(createFrame("proj22/2.jpg",4,1,5));
	FrameBuffer->putFrame(createFrame("proj22/4.jpg",8,1,5));
	sprintf(my_msg,"%s->%s/%d.jpg\0",frameDir,frameDirVFS,i);
 	debug(1,my_msg);

	sprintf(my_msg,"AvgFileSize:%d AvgNetSpeed:%d\0",CacheController->getAvgFileSize(),CacheController->getAvgNetSpeed());
	//CacheController->cache->putFrame(25,1,1,33,11,33,"http://photos.onlysimchas.com/pix/g288/OSPGEN_15.jpg");

	sprintf(my_msg,"AvgFileSize:%d AvgNetSpeed:%d\0",CacheController->getAvgFileSize(),CacheController->getAvgNetSpeed());
 
	debug(1,my_msg);
	debug(2,my_msg);


}

void AIVideoPlayer::time_changed(){
//		FrameBuffer->frames[FrameBuffer->head]->start <=TimeController->current_frame_number-1
	/*if ( TimeController->current_frame_number % 30 == 0){
			int tot_sec = TimeController->current_frame_number / 30;
			int min = tot_sec/60;
			int sec = tot_sec % 60;
			char play[20];
			sprintf(play,"Playing (%d:%2d)",min,sec);
			PlayerWindow->SetText(play);
		}*/
	if (FrameBuffer->size > 0&&
		FrameBuffer->frames[(FrameBuffer->getHead()+1)%FrameBuffer->buffer_size] &&
		FrameBuffer->frames[(FrameBuffer->getHead()+1)%FrameBuffer->buffer_size]->start &&
		FrameBuffer->frames[(FrameBuffer->getHead()+1)%FrameBuffer->buffer_size]->start <= TimeController->current_frame_number-1
// use above line for realtime
		// use this for "Every 1 second iteration" TimeController->current_frame_number % 30 == 0
		) {
		playThread();	
	}
	
		if ( TimeController->current_frame_number % 30 == 0){
			int tot_sec = TimeController->current_frame_number / 30;
			int min = tot_sec/60;
			int sec = tot_sec % 60;
			char* play= new char[25];
			sprintf(play,"Playing (%d:%2d)",min,sec);
			PlayerWindow->SetText(play);
		}
	
}