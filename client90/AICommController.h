// AICommController.h: interface for the AICommController class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AICOMMCONTROLLER_H__14E066B9_1923_4D79_AAB8_DD0E6A91F953__INCLUDED_)
#define AFX_AICOMMCONTROLLER_H__14E066B9_1923_4D79_AAB8_DD0E6A91F953__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class AICommController  
{
public:
	AICommController();
	virtual ~AICommController();
	void broadcast(char* message);
	void broadcast(int code);
	void send(char* dest, char* message);
	int parseResponse(char* string);
	int current_state;
	int response_count;
	int max_time;

};

#endif // !defined(AFX_AICOMMCONTROLLER_H__14E066B9_1923_4D79_AAB8_DD0E6A91F953__INCLUDED_)
