// AICommController.cpp: implementation of the AICommController class.
//
//////////////////////////////////////////////////////////////////////

#include "AICommController.h"
#include "cssysdef.h"
#include "cssys/system.h"

#include "cssys/sysdriv.h"
#include "csws/csws.h"
#include "version.h"
#include "ivideo/fontserv.h"
#include <sys/stat.h>
#include "csgeom/csrect.h"


#include "AIVideoPlayer.h"
#include "WindowToolkit.h"
#include "AIVideoPlayerWindow.h"
#include "AIVideoFrameBuffer.h"
#include "ChimeSystemDriver.h"
#include "ChimeApp.h"
#include "AI2TVConstants.h"
#include "AISyncController.h"
#include "AITimeController.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
extern ChimeSystemDriver *Sys;
AICommController::AICommController()
{
current_state = AI2TV_WAITING;
response_count=0;
max_time = -1;
}

AICommController::~AICommController()
{

}

// send methods, calls the udpclient
void AICommController::send(char* dest, char* message){
	Sys->myVideoPlayer->SyncController->client->sendMess(dest,message);
}

void AICommController::broadcast(int code){
	// if i pressed pause, announce it
	if (code == AI2TV_PAUSE_PRESSED){
		current_state= AI2TV_PAUSE_PRESSED;
		code = AI2TV_PAUSE_ANNOUNCE;
	}

	// if i pressed play announce it
	if (code == AI2TV_PLAY_PRESSED){
		current_state= AI2TV_PLAY_PRESSED;
		code = AI2TV_PLAY_ANNOUNCE;
	}

	char ev_msg[100];
	sprintf(ev_msg,"%d.%d\0",code,Sys->myVideoPlayer->TimeController->current_frame_number);
	broadcast(ev_msg);
			
}
void AICommController::broadcast(char* message){
	for(int i = 0; i < Sys->myVideoPlayer->SyncController->num_clients; i++){
		send(Sys->myVideoPlayer->SyncController->multicast_group[i],message);
	}
}

// parse response. called by UDPServer
int AICommController::parseResponse(char* string)
{
if (Sys->myVideoPlayer) {
	
	
	int code;
	int current_time=0; // used for resetting clock
	int frame_number;
	sscanf(string,"%d.%d",&code,&frame_number);

	if (code < 0) return 0;
		char* resultant = new char[300];
	switch(code){
		// if someone announced, confirm with him
		case AI2TV_PAUSE_ANNOUNCE:
			broadcast(AI2TV_PAUSE_CONFIRM);
			break;

		case AI2TV_PLAY_ANNOUNCE:
			broadcast(AI2TV_PLAY_CONFIRM);
			break;

		case AI2TV_PAUSE_CONFIRM:
			// if i announced, i should gather the info
			if ( current_state == AI2TV_PAUSE_PRESSED){
				response_count++;
				if (max_time < frame_number) max_time = frame_number;
				current_state =0;
				max_time +=2;
				sprintf(resultant,"%d.%d \0",AI2TV_PAUSE_FINAL,max_time);
				for(int j = 0; j < Sys->myVideoPlayer->SyncController->num_clients; j++){
						broadcast(resultant);
					}
				sprintf(resultant,"CALL STOP@%d\0",max_time);
				Sys->myVideoPlayer->debug(2,resultant);
				
			}
			break;


		case AI2TV_PLAY_CONFIRM:
			// if i announced, i should gather the info
			if ( current_state == AI2TV_PLAY_PRESSED){
				response_count++;
				if (max_time < frame_number) max_time = frame_number;
 			//	if (response_count == Sys->myVideoPlayer->SyncController->num_clients)
					max_time +=2;
					sprintf(resultant,"%d.%d \0",AI2TV_PLAY_FINAL,max_time);
					for(int j = 0; j < Sys->myVideoPlayer->SyncController->num_clients; j++){
						broadcast(resultant);
					}
					response_count=0;
					current_state=0;
					sprintf(resultant,"CALL PLAY%d\0",max_time);
					Sys->myVideoPlayer->debug(2,resultant);
					max_time=-1;
			
			//	}
			}
			break;
		/*case AI2TV_NOTE:
			// sync the time
			if (Sys->myVideoPlayer->TimeController->current_frame_number < frame_number - 50){
				Sys->myVideoPlayer->TimeController->stopflag=frame_number;
				Sys->myVideoPlayer->TimeController->current_frame_number=frame_number;
				current_time= frame_number *33 + (frame_number/3);
				Sys->myVideoPlayer->TimeController->current_time=current_time;
				sprintf(resultant,"SYNC'ed%d\0",frame_number);
				Sys->GetApp()->chatWindow->ShowMessage(resultant);
			}
			//	Sys->myVideoPlayer->TimeController->stop();
		*/
			break;
		case AI2TV_PAUSE_FINAL:
			// final announcement all stop here!
				//Sys->myVideoPlayer->TimeController->stopflag=frame_number;
				Sys->myVideoPlayer->TimeController->stop();
				Sys->myVideoPlayer->PauseAudio();
				Sys->myVideoPlayer->TimeController->current_frame_number=frame_number;
				current_time= frame_number *33 + (frame_number/3);
				Sys->myVideoPlayer->TimeController->current_time=current_time;
				sprintf(resultant,"STOPPING@%d\0",frame_number);
				Sys->myVideoPlayer->debug(2,resultant);
				if (Sys->myVideoPlayer && Sys->myVideoPlayer->PlayerWindow){
				Sys->myVideoPlayer->PlayerWindow->SetText(resultant);
				}
				current_state=0;
			//	Sys->myVideoPlayer->TimeController->stop();
		
			break;
		
		case AI2TV_PLAY_FINAL:
			// final announcement all start here!
				Sys->myVideoPlayer->TimeController->stopflag=-1;
				Sys->myVideoPlayer->TimeController->start();
				Sys->myVideoPlayer->PlayAudio();
				sprintf(resultant,"PLAYING%d\0",frame_number);
				Sys->myVideoPlayer->debug(2,resultant);
				if (Sys->myVideoPlayer && Sys->myVideoPlayer->PlayerWindow){
				Sys->myVideoPlayer->PlayerWindow->SetText(resultant);
				}
				current_state=0;
				
			break;

	
	}


	if (sprintf(resultant,"%d@%d\0",code,frame_number)&& code > 0 ){
			Sys->myVideoPlayer->debug(2,resultant);
		return 1;
	} else return 0;
		
	}	else return 0;
}
