/*
Copyright 2021 daywithstars

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

#ifndef _PT_BEHAVIORSTATEEVENT_H_
#define _PT_BEHAVIORSTATEEVENT_H_

#include <SDL_stdinc.h>

#include <PT_String.h>


typedef enum  {
	
	PT_BEHAVIORSTATE_EVENT_TYPE_NONE,
	PT_BEHAVIORSTATE_EVENT_TYPE_PLAYSAMPLE,
	PT_BEHAVIORSTATE_EVENT_TYPE_PLAYMUSIC,
	
}PT_BehaviorStateEventType;



typedef struct {
	PT_String* audioName;
	int audioLoop;
}PT_BehaviorStateEvent_audioInfo;

typedef union {
	PT_BehaviorStateEvent_audioInfo playSample;
	PT_BehaviorStateEvent_audioInfo playMusic;
}PT_BehaviorStateEvent_Sound;



typedef struct {
	PT_BehaviorStateEventType type;
	
	PT_BehaviorStateEvent_Sound sound;
}PT_BehaviorStateEvent;



typedef struct {
	unsigned int front, rear, length, capacity;

	PT_BehaviorStateEvent** arrayValue;
}PT_BehaviorStateEventQueue;


PT_BehaviorStateEventQueue* PT_BehaviorStateEventQueueCreate( unsigned int capacity, 
	PT_BehaviorStateEvent** arrayValue );
void PT_BehaviorStateEventQueueDestroy( PT_BehaviorStateEventQueue* _this );

void PT_BehaviorStateEventQueueEnQueue( PT_BehaviorStateEventQueue* _this, PT_BehaviorStateEvent* value );
PT_BehaviorStateEvent* PT_BehaviorStateEventQueueDeQueue( PT_BehaviorStateEventQueue* _this );

#endif /* _PT_BEHAVIORSTATEEVENT_H_ */



