// AITimeController.cpp: implementation of the AITimeController class.
//
//////////////////////////////////////////////////////////////////////

#include "AITimeController.h"
#include <windows.h>

#ifndef _AIVIDEOPLAYER_H_
class AIVideoPlayer{
public:
	int current_frame_number;
		void time_changed();

};
#endif // _AIVIDEOPLAYER_H_

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

AITimeController::AITimeController(AIVideoPlayer* p)
{
EXIT=false;
stopflag=-1;
player=p;
stopped=true;
current_frame_number=-1;
current_time=0;
runTimerLoop();
}

AITimeController::~AITimeController()
{

}

void AITimeController::start(){stopped=false;}
void AITimeController::stop(){stopped=true;}



DWORD WINAPI RunAITimeController(LPVOID lParam){
	AITimeController* tc = (AITimeController*)lParam;
		int fps = 33;
	while(true){
		if (tc->EXIT) return 0;
		if (tc->stopflag>0&&tc->current_frame_number>=tc->stopflag){
				tc->stopped=true; tc->stopflag=0;
		}
		while (!tc->stopped){
			/// sleep until next frame
			int adj = ((tc->current_frame_number % 3 ==2)?1:0);
			tc->current_frame_number = tc->current_frame_number+1;
			Sleep(adj+fps);
			tc->current_time+=(adj+fps);
			tc->player->time_changed();
		}
	}
}



void AITimeController::runTimerLoop(){
	CoInitialize(NULL);
	DWORD threadId1;
	CreateThread(NULL,0,RunAITimeController,this,0,&threadId1);
}
