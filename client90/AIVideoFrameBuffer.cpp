// AIVideoFrameBuffer.cpp: implementation of the AIVideoFrameBuffer class.
//
//////////////////////////////////////////////////////////////////////

#include "AIVideoFrameBuffer.h"
#include "cssysdef.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

AIVideoFrameBuffer::AIVideoFrameBuffer(int bsize)
{
	buffer_size = bsize;
	size=0;
	head=0;
	tail=0;
	frames= new AIVideoFrame*[bsize];

}

// if you do not specify size, lets use the default
AIVideoFrameBuffer::AIVideoFrameBuffer()
{
	buffer_size = DEFAULT_BUFFER_SIZE;
	head=-1;	// the front of the queue (the first element)
	size=0; // the back of the queue (add here)
	tail=0;

	frames= new AIVideoFrame*[buffer_size];
}

int AIVideoFrameBuffer::getHead(){return head%buffer_size;}
int AIVideoFrameBuffer::getTail(){return tail%buffer_size;}
AIVideoFrameBuffer::~AIVideoFrameBuffer()
{
	for (int i = 0; i < buffer_size;i++){
		if (frames[i] != 0) delete frames[i];
	}
	delete frames;
}

int AIVideoFrameBuffer::DEFAULT_BUFFER_SIZE = 20;


// Tells you if the buffer is full
int AIVideoFrameBuffer::isFull(){
	// if the next insertable slot is the front of the queue, we cannot insert
	return(size==buffer_size);
}


// putFrame puts a frame into the buffer (returns 0 if full)
int AIVideoFrameBuffer::putFrame(AIVideoFrame *frame){
	if (isFull()) {
		return 0;		
	} else {
		frames[tail++ % buffer_size]=frame;
		size++;
		return 1;
	}
}

// nextFrame returns and removes a frame from the buffer
// returns 0 if buffer is empty
AIVideoFrame* AIVideoFrameBuffer::nextFrame(){
	if (size > 0){	
		size--;
	return frames[++head % buffer_size];
	} else return 0;
}