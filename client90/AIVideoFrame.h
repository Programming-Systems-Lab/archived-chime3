// AIVideoFrame.h: interface for the AIVideoFrame class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AIVIDEOFRAME_H__1F35658F_A838_47D5_BEC2_82B45E489052__INCLUDED_)
#define AFX_AIVIDEOFRAME_H__1F35658F_A838_47D5_BEC2_82B45E489052__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

struct iMaterialWrapper;

class AIVideoFrame  
{
public:
	iMaterialWrapper* materialWrapper; // the actual crystalspace usable material
	int frame_number; // the frameNumber - it's place in the video
	char* filename; // the filename of the Image file for this frame
	int start; // the frameID this starts being displayed at
	int end; // the frame number this stops being displayed at
	AIVideoFrame();
	virtual ~AIVideoFrame();

};

#endif // !defined(AFX_AIVIDEOFRAME_H__1F35658F_A838_47D5_BEC2_82B45E489052__INCLUDED_)
