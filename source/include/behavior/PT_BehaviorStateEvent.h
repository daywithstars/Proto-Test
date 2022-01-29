/*
Copyright 2022 daywithstars

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

#ifndef _PT_BEHAVIORSTATEEVENT_H_
#define _PT_BEHAVIORSTATEEVENT_H_


#include <SDL_stdinc.h>

#include <json.h>

#include <PT_String.h>
#include <PT_Sprite.h>

//Use "mask" with bitwise to bind TYPE with VALUE
typedef enum {
	PT_BEHAVIOR_STATE_EVENT_TRIGGER_TYPE_NONE                  =1,
	PT_BEHAVIOR_STATE_EVENT_TRIGGER_TYPE_ALWAYS                =2,
	PT_BEHAVIOR_STATE_EVENT_TRIGGER_TYPE_TIMES                 =4,
	PT_BEHAVIOR_STATE_EVENT_TRIGGER_TYPE_CONDITION             =8,
	PT_BEHAVIOR_STATE_EVENT_TRIGGER_TYPE_TIMER                 =16,
	PT_BEHAVIOR_STATE_EVENT_TRIGGER_TYPE_INPUT                 =32,
	PT_BEHAVIOR_STATE_EVENT_TRIGGER_TYPE_COLLISION             =64,
	
	PT_BEHAVIOR_STATE_EVENT_TRIGGER_VALUE_PLAY_SAMPLE          =128,
	PT_BEHAVIOR_STATE_EVENT_TRIGGER_VALUE_PLAY_MUSIC           =256,
	PT_BEHAVIOR_STATE_EVENT_TRIGGER_VALUE_CHANGE_DIRECTION     =512,
	PT_BEHAVIOR_STATE_EVENT_TRIGGER_VALUE_CHANGE_SCREEN        =1024
}PT_BehaviorStateEventTriggerFlags;




typedef struct {
	PT_String* name;
	int loop;
}PT_BehaviorStateEventPlaySound;


/*
	The mask result will be cast(from PT_BehaviorStateEventTriggerField) into one of these:
*/
typedef struct {
	PT_String* keyMap;
	PT_BehaviorStateEventPlaySound sound;
}PT_BehaviorStateEventInputPlaySound;

typedef struct {
	//In construction
	PT_BehaviorStateEventPlaySound sound;
}PT_BehaviorStateEventCollisionPlaySound;

typedef struct {
	PT_String* collider_1_name;
	PT_String* collider_2_name;
	
	int dirX;
	int dirY;
	
	PT_Sprite* pSprite;
}PT_BehaviorStateEventCollisionChangeDirection;



typedef struct {
	void* data;
}PT_BehaviorStateEventTriggerField;

typedef union {
	PT_BehaviorStateEventTriggerField always;
	PT_BehaviorStateEventTriggerField collision;
	PT_BehaviorStateEventTriggerField input;
}PT_BehaviorStateEventTrigger;



typedef struct {
	int flags;
	
	void* pBehaviorState;
	PT_BehaviorStateEventTrigger trigger;
}PT_BehaviorStateEvent;


PT_BehaviorStateEvent* PT_BehaviorStateEventCreate( void* pBehaviorState, 
	json_value* jsonValue );
void PT_BehaviorStateEventDestroy( PT_BehaviorStateEvent* _this );

void PT_BehaviorStateEventUpdate( PT_BehaviorStateEvent* _this, Sint32 elapsedTime );

#endif /* _PT_BEHAVIORSTATEEVENT_H_ */



