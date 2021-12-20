/*
Copyright 2021 daywithstars

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

#ifndef _PT_BEHAVIOR_STATE_H_
#define _PT_BEHAVIOR_STATE_H_

#include <json.h>

#include <SDL_stdinc.h>

#include <PT_StringList.h>
#include <PT_BehaviorStateEvent.h>



typedef struct pt_behavior_state {
	void* pBehavior;

	PT_StringList* inputMap;
	PT_StringList* inputChangeStateMap;
	
	unsigned int eventsNum;
	PT_BehaviorStateEvent** events;
	
	unsigned int awaysNum;
	char** aways;
}PT_BehaviorState;


PT_BehaviorState* PT_BehaviorStateCreate( json_value* jsonValue, void* pBehavior );
void PT_BehaviorStateDestroy( PT_BehaviorState* _this );

void PT_BehaviorStateUpdate( PT_BehaviorState* _this, void* target, Sint32 elapsedTime );

#endif /* _PT_BEHAVIOR_STATE_H_ */



