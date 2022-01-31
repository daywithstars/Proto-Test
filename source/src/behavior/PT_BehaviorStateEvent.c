/*
Copyright 2022 daywithstars

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/


#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <time.h>

#include <SDL_stdinc.h>
#include <SDL_log.h>

#include <PT_BehaviorStateEvent.h>
#include <PT_BehaviorStateEventInput.h>
#include <PT_BehaviorStateEventCollision.h>
#include <PT_Behavior.h>
#include <PT_BehaviorState.h>
#include <PT_Parse.h>
#include <PT_InputHandler.h>
#include <PT_SoundManager.h>




//===================================== PRIVATE Functions

SDL_bool PT_BehaviorStateEventParse( PT_BehaviorStateEvent* _this, json_value* jsonValue );

	
	

//===================================== PUBLIC Functions

PT_BehaviorStateEvent* PT_BehaviorStateEventCreate( void* pBehaviorState,
	json_value* jsonValue  ) {
	
	PT_BehaviorStateEvent* _this = (PT_BehaviorStateEvent*)malloc(sizeof(PT_BehaviorStateEvent));
	if ( !_this )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_BehaviorStateEventCreate: Not enough memory!\n");
		return NULL;
	}
	SDL_memset(_this, 0, sizeof(PT_BehaviorStateEvent));
	
	_this->pBehaviorState = pBehaviorState;
	if ( !PT_BehaviorStateEventParse(_this, jsonValue) )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_BehaviorStateEventCreate: Cannot parse!\n");
		PT_BehaviorStateEventDestroy(_this);
		return NULL;	
	}
	
	
	return _this;
}//PT_BehaviorStateEventCreate

void PT_BehaviorStateEventDestroy( PT_BehaviorStateEvent* _this ) {
	if ( !_this )
	{
		return;
	}

	//Input
	if ( _this->flags & PT_BEHAVIOR_STATE_EVENT_TRIGGER_TYPE_INPUT )
	{
		//Input play-sample
		if ( _this->flags & PT_BEHAVIOR_STATE_EVENT_TRIGGER_VALUE_PLAY_SAMPLE )
		{
			if ( _this->trigger.input.data )
			{
				PT_BehaviorStateEventInputPlaySound* inputPlaySound = 
				(PT_BehaviorStateEventInputPlaySound*)_this->trigger.input.data;
				
				PT_StringDestroy(inputPlaySound->keyMap);
				PT_StringDestroy(inputPlaySound->sound.name);
				
				free(inputPlaySound);
			}
		}
		//Input play-music
		else if ( _this->flags & PT_BEHAVIOR_STATE_EVENT_TRIGGER_VALUE_PLAY_MUSIC )
		{
			if ( _this->trigger.input.data )
			{
				PT_BehaviorStateEventInputPlaySound* inputPlaySound = 
				(PT_BehaviorStateEventInputPlaySound*)_this->trigger.input.data;
				
				PT_StringDestroy(inputPlaySound->keyMap);
				PT_StringDestroy(inputPlaySound->sound.name);
				
				free(inputPlaySound);
			}
		}
		//Input change-screen
		else if ( _this->flags & PT_BEHAVIOR_STATE_EVENT_TRIGGER_VALUE_CHANGE_SCREEN )
		{
			if ( _this->trigger.input.data )
			{
				printf("destroy change-screen to input\n");
			}
		}
	}
	//Collision
	if ( _this->flags & PT_BEHAVIOR_STATE_EVENT_TRIGGER_TYPE_COLLISION )
	{
		//Collision ChangeDirection
		if ( _this->flags & PT_BEHAVIOR_STATE_EVENT_TRIGGER_VALUE_CHANGE_DIRECTION )
		{
			if ( _this->trigger.collision.data )
			{
				PT_BehaviorStateEventCollisionChangeDirection* collisionChangeDirection =
				(PT_BehaviorStateEventCollisionChangeDirection*)_this->trigger.input.data;
				
				PT_StringDestroy(collisionChangeDirection->collider_1_name);
				PT_StringDestroy(collisionChangeDirection->collider_2_name);
				
				free(collisionChangeDirection);
			}
		}
	}

	free(_this);
}//PT_BehaviorStateEventDestroy

void PT_BehaviorStateEventUpdate( PT_BehaviorStateEvent* _this, Sint32 elapsedTime ) {

	//Input
	if ( _this->flags & PT_BEHAVIOR_STATE_EVENT_TRIGGER_TYPE_INPUT )
	{	
		PT_BehaviorState* pBehaviorState = (PT_BehaviorState*)_this->pBehaviorState;
		PT_Behavior* pBehavior = (PT_Behavior*)pBehaviorState->pBehavior;
		PT_InputHandler* inputHandler = pBehavior->inputHandler;

		
		if ( _this->flags & PT_BEHAVIOR_STATE_EVENT_TRIGGER_VALUE_PLAY_SAMPLE )
		{
			PT_BehaviorStateEventInputPlaySound* inputPlaySound = 
			(PT_BehaviorStateEventInputPlaySound*)_this->trigger.input.data;
			
			if ( 
				PT_InputHandlerGetButtonState(inputHandler, (char*)inputPlaySound->keyMap->utf8_string) 
			   )
			{	
				PT_SoundManagerPlaySample((char*)inputPlaySound->sound.name->utf8_string,
				inputPlaySound->sound.loop);
			}
		}
		else if ( _this->flags & PT_BEHAVIOR_STATE_EVENT_TRIGGER_VALUE_PLAY_MUSIC )
		{
			printf("music input\n");
		}
	}
	//Collision
	else if ( _this->flags & PT_BEHAVIOR_STATE_EVENT_TRIGGER_TYPE_COLLISION )
	{
		//Collision ChangeDirection
		if ( _this->flags & PT_BEHAVIOR_STATE_EVENT_TRIGGER_VALUE_CHANGE_DIRECTION )
		{
			PT_BehaviorStateEventCollisionChangeDirection* collisionChangeDirection =
			(PT_BehaviorStateEventCollisionChangeDirection*)_this->trigger.collision.data;
			
			if ( collisionChangeDirection->pSprite->collisionColliderName )
			{
				if ( PT_StringMatchFast(
					(char*)collisionChangeDirection->pSprite->collisionColliderName->utf8_string,
					(char*)collisionChangeDirection->collider_1_name->utf8_string) 
						&&
					PT_StringMatchFast(
					(char*)collisionChangeDirection->pSprite->collisionTargetColliderName->utf8_string,
					(char*)collisionChangeDirection->collider_2_name->utf8_string)
					)
				{
					collisionChangeDirection->pSprite->dirX = collisionChangeDirection->dirX;
					collisionChangeDirection->pSprite->dirY = collisionChangeDirection->dirY;
				}
			}
		}
	}
}//PT_BehaviorStateEventUpdate


//===================================== PRIVATE Functions

SDL_bool PT_BehaviorStateEventParse( PT_BehaviorStateEvent* _this, json_value* jsonValue ) {
	if ( !_this || !jsonValue)
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, 
		"PT: PT_BehaviorStateEventParse: Invalid StateEvent or jsonValue\n");
		return SDL_FALSE;
	}
	
	//Find the event trigger type
	json_object_entry triggerEntry = 
	PT_ParseGetObjectEntry_json_value(jsonValue, "trigger type");
	if ( triggerEntry.name )
	{	
		if ( PT_StringMatchFast(triggerEntry.value->u.string.ptr, "none") )
		{
			_this->flags |= PT_BEHAVIOR_STATE_EVENT_TRIGGER_TYPE_NONE;
		}
		else if ( PT_StringMatchFast(triggerEntry.value->u.string.ptr, "always") )
		{
			_this->flags |= PT_BEHAVIOR_STATE_EVENT_TRIGGER_TYPE_ALWAYS;
		}
		else if ( PT_StringMatchFast(triggerEntry.value->u.string.ptr, "times") )
		{
			_this->flags |= PT_BEHAVIOR_STATE_EVENT_TRIGGER_TYPE_TIMES;
		}
		else if ( PT_StringMatchFast(triggerEntry.value->u.string.ptr, "condition") )
		{
			_this->flags |= PT_BEHAVIOR_STATE_EVENT_TRIGGER_TYPE_CONDITION;
		}
		else if ( PT_StringMatchFast(triggerEntry.value->u.string.ptr, "timer") )
		{
			_this->flags |= PT_BEHAVIOR_STATE_EVENT_TRIGGER_TYPE_TIMER;
		}
		else if ( PT_StringMatchFast(triggerEntry.value->u.string.ptr, "input") )
		{
			_this->flags |= PT_BEHAVIOR_STATE_EVENT_TRIGGER_TYPE_INPUT;
		}
		else if ( PT_StringMatchFast(triggerEntry.value->u.string.ptr, "collision") )
		{
			_this->flags |= PT_BEHAVIOR_STATE_EVENT_TRIGGER_TYPE_COLLISION;
		}
		else {
			SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, 
			"PT: PT_BehaviorStateEventParse: invalid <TriggerType>\n");
		}
	}
	else {
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, 
		"PT: PT_BehaviorStateEventParse: Cannot find states events trigger type\n");
		return SDL_FALSE;
	}
		
	//Find the event type
	triggerEntry = 
	PT_ParseGetObjectEntry_json_value(jsonValue, "trigger value type");
	if ( triggerEntry.name )
	{
		if ( PT_StringMatchFast(triggerEntry.value->u.string.ptr, "play-sample") )
		{
			_this->flags |= PT_BEHAVIOR_STATE_EVENT_TRIGGER_VALUE_PLAY_SAMPLE;
		}
		else if ( PT_StringMatchFast(triggerEntry.value->u.string.ptr, "play-music") )
		{
			_this->flags |= PT_BEHAVIOR_STATE_EVENT_TRIGGER_VALUE_PLAY_MUSIC;
		}
		else if ( PT_StringMatchFast(triggerEntry.value->u.string.ptr, "change-direction") )
		{
			_this->flags |= PT_BEHAVIOR_STATE_EVENT_TRIGGER_VALUE_CHANGE_DIRECTION;
		}
		else if ( PT_StringMatchFast(triggerEntry.value->u.string.ptr, "change-screen") )
		{
			_this->flags |= PT_BEHAVIOR_STATE_EVENT_TRIGGER_VALUE_CHANGE_SCREEN;
		}
		else {
			SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, 
			"PT: PT_BehaviorStateEventParse: invalid <ValueType>\n");
		}
	}
	else {
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, 
		"PT: PT_BehaviorStateEventParse: Cannot find states events trigger value type\n");
		return SDL_FALSE;
	}
	
	
	//set the correct parse to read the "fields".
	//None
	/*if ( _this->flag & PT_BEHAVIOR_STATE_EVENT_TRIGGER_TYPE_NONE )
	{
		
	}*/
	//Input
	if ( _this->flags & PT_BEHAVIOR_STATE_EVENT_TRIGGER_TYPE_INPUT )
	{
		//Input play-sample
		if ( _this->flags & PT_BEHAVIOR_STATE_EVENT_TRIGGER_VALUE_PLAY_SAMPLE )
		{
			if ( !PT_BehaviorStateEventParseTrigger_Input_PlaySample(_this, jsonValue) )
			{
				SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_BehaviorStateEventParse!\n");
				return SDL_FALSE;
			}
		}
		//Input play-music
		else if ( _this->flags & PT_BEHAVIOR_STATE_EVENT_TRIGGER_VALUE_PLAY_MUSIC )
		{
			if ( !PT_BehaviorStateEventParseTrigger_Input_PlayMusic(_this, jsonValue) )
			{
				SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_BehaviorStateEventParse!\n");
				return SDL_FALSE;
			}
		}
		//Input change-screen
		else if ( _this->flags & PT_BEHAVIOR_STATE_EVENT_TRIGGER_VALUE_CHANGE_SCREEN )
		{
			if ( !PT_BehaviorStateEventParseTrigger_Input_ChangeScreen(_this, jsonValue) )
			{
				SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_BehaviorStateEventParse!\n");
				return SDL_FALSE;
			}
		}
		else {
			SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, 
			"PT: PT_BehaviorStateEventParse: input has been bind with some invalid flag\n");
			return SDL_FALSE;
		}
	}
	//Collision
	else if ( _this->flags & PT_BEHAVIOR_STATE_EVENT_TRIGGER_TYPE_COLLISION )
	{
		//Collision ChangeDirection
		if ( _this->flags & PT_BEHAVIOR_STATE_EVENT_TRIGGER_VALUE_CHANGE_DIRECTION )
		{
			if ( !PT_BehaviorStateEventParseTrigger_Collision_ChangeDirection(_this, jsonValue) )
			{
				SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_BehaviorStateEventParse!\n");
				return SDL_FALSE;
			}
		}
		else {
			SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, 
			"PT: PT_BehaviorStateEventParse: input has been bind with some invalid flag\n");
			return SDL_FALSE;
		}
	}

	return SDL_TRUE;
}//PT_BehaviorStateEventParse



