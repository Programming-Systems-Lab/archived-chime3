// AITimeController.h: interface for the AITimeController class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AITIMECONTROLLER_H__2E21EA5E_0BD6_4102_9BE6_65F4FD64750B__INCLUDED_)
#define AFX_AITIMECONTROLLER_H__2E21EA5E_0BD6_4102_9BE6_65F4FD64750B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
class AIVideoPlayer;
class AITimeController {  

public:
	int stopflag;
	AIVideoPlayer* player;
	bool stopped;
	bool EXIT;
	AITimeController(AIVideoPlayer* p);
	virtual ~AITimeController();
	void runTimerLoop();
	int current_frame_number;
	int current_time;
	void start();
	void stop();

};

#endif // !defined(AFX_AITIMECONTROLLER_H__2E21EA5E_0BD6_4102_9BE6_65F4FD64750B__INCLUDED_)
