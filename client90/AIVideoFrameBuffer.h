// AIVideoFrameBuffer.h: interface for the AIVideoFrameBuffer class.
// This stores a circular queue of pre-built frames...
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AIVIDEOFRAMEBUFFER_H__C6FD9C33_C457_4DF8_9677_9A0BFC1504B1__INCLUDED_)
#define AFX_AIVIDEOFRAMEBUFFER_H__C6FD9C33_C457_4DF8_9677_9A0BFC1504B1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AIVideoFrame.h"

class AIVideoFrameBuffer  
{
public:
	AIVideoFrameBuffer();			// default constructor, uses DEFAULT_BUFFER_SIZE
	AIVideoFrameBuffer(int bsize);	// specify the size of the buffer
	AIVideoFrame** frames;			// an array of references to frames
	virtual ~AIVideoFrameBuffer();	// destructor
	int size;						// number of frames contained in buffer
	int buffer_size;				// size of the buffer
	int head,tail;					// used for circular queue
	int getHead();
	int getTail();
	int putFrame(AIVideoFrame* frame);
	AIVideoFrame* nextFrame();
	static int DEFAULT_BUFFER_SIZE;	
	int isFull();
};

#endif // !defined(AFX_AIVIDEOFRAMEBUFFER_H__C6FD9C33_C457_4DF8_9677_9A0BFC1504B1__INCLUDED_)
