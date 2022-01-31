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

#include <PT_BehaviorStateEventCollision.h>
#include <PT_BehaviorState.h>
#include <PT_Behavior.h>
#include <PT_Parse.h>




SDL_bool PT_BehaviorStateEventParseTrigger_Collision_ChangeDirection( 
	PT_BehaviorStateEvent* _this, json_value* jsonValue ) {
	
	PT_BehaviorStateEventCollisionChangeDirection* collisionChangeDirection =
		(PT_BehaviorStateEventCollisionChangeDirection*)malloc(
		sizeof(PT_BehaviorStateEventCollisionChangeDirection));
	
	if ( !collisionChangeDirection )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, 
		"PT: PT_BehaviorStateEventParseTrigger_Collision_ChangeDirection: Not enough memory\n");
		
		_this->trigger.collision.data = NULL;
		_this->flags = PT_BEHAVIOR_STATE_EVENT_TRIGGER_TYPE_NONE;
		return SDL_FALSE;
	}
	SDL_memset(collisionChangeDirection, 0, sizeof(PT_BehaviorStateEventCollisionChangeDirection));
	
	json_object_entry fieldsEntry = 
	PT_ParseGetObjectEntry_json_value(jsonValue, "trigger value fields");
	if ( fieldsEntry.name )
	{
		fieldsEntry = 
		PT_ParseGetObjectEntry_json_value(jsonValue, "trigger value fields this-collider-name");
		if ( fieldsEntry.name )
		{
			PT_String* collider_1_name = PT_StringCreate();
			if ( !collider_1_name )
			{
				SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, 
				"PT: PT_BehaviorStateEventParseTrigger_Collision_ChangeDirection!\n");
				
				return SDL_FALSE;	
			}

			if ( !PT_StringInsert(&collider_1_name, fieldsEntry.value->u.string.ptr, 0) )
			{
				SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, 
				"PT: PT_BehaviorStateEventParseTrigger_Collision_ChangeDirection!\n");
	
				return SDL_FALSE;	
			}
			
			collisionChangeDirection->collider_1_name = collider_1_name;
		}	
		else {
			SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, 
			"PT: PT_BehaviorStateEventParseTrigger_Collision_ChangeDirection: Cannot find in fields: this-collider-name\n");
			
			return SDL_FALSE;
		}
		
		fieldsEntry = 
		PT_ParseGetObjectEntry_json_value(jsonValue, "trigger value fields target-collider-name");
		if ( fieldsEntry.name )
		{
			PT_String* collider_2_name = PT_StringCreate();
			if ( !collider_2_name )
			{
				SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, 
				"PT: PT_BehaviorStateEventParseTrigger_Collision_ChangeDirection!\n");
				
				return SDL_FALSE;	
			}
			
			if ( !PT_StringInsert(&collider_2_name, fieldsEntry.value->u.string.ptr, 0) )
			{
				SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, 
				"PT: PT_BehaviorStateEventParseTrigger_Collision_ChangeDirection!\n");
	
				return SDL_FALSE;	
			}
			
			collisionChangeDirection->collider_2_name = collider_2_name;
		}	
		else {
			SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, 
			"PT: PT_BehaviorStateEventParseTrigger_Collision_ChangeDirection: Cannot find in fields: target-collider-name\n");
			
			return SDL_FALSE;
		}
		
		time_t t;
		srand((unsigned) time(&t));
		
		fieldsEntry = 
		PT_ParseGetObjectEntry_json_value(jsonValue, "trigger value fields dir-x");
		if ( fieldsEntry.name )
		{
			if ( fieldsEntry.value->type == json_string )
			{
				if ( PT_StringMatchFast(fieldsEntry.value->u.string.ptr, "rand") )
				{
					int value = rand() % 75;
					if ( value <= 25 )
					{
						collisionChangeDirection->dirX = 1;
					}
					else if ( value > 25 && value <= 50 )
					{
						collisionChangeDirection->dirX = -1;
					}
					else {
						collisionChangeDirection->dirX = 0;
					}
				}
			}
			else if ( fieldsEntry.value->type == json_integer )
			{
				collisionChangeDirection->dirX = fieldsEntry.value->u.integer;
			}
			else if ( fieldsEntry.value->type == json_double )
			{
				SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, 
			"PT: PT_BehaviorStateEventParseTrigger_Collision_ChangeDirection: dir-x: Do not use float values\n");	
			}
		}	
		else {
			SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, 
			"PT: PT_BehaviorStateEventParseTrigger_Collision_ChangeDirection: Cannot find in fields: dir-x\n");	
			return SDL_FALSE;
		}
		
		fieldsEntry = 
		PT_ParseGetObjectEntry_json_value(jsonValue, "trigger value fields dir-y");
		if ( fieldsEntry.name )
		{
			if ( fieldsEntry.value->type == json_string )
			{
				if ( PT_StringMatchFast(fieldsEntry.value->u.string.ptr, "rand") )
				{
					int value = rand() % 75;
					if ( value <= 25 )
					{
						collisionChangeDirection->dirY = 1;
					}
					else if ( value > 25 && value <= 50 )
					{
						collisionChangeDirection->dirY = -1;
					}
					else {
						collisionChangeDirection->dirY = 0;
					}
				}
			}
			else if ( fieldsEntry.value->type == json_integer )
			{
				collisionChangeDirection->dirY = fieldsEntry.value->u.integer;
			}
			else if ( fieldsEntry.value->type == json_double )
			{
				SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, 
			"PT: PT_BehaviorStateEventParseTrigger_Collision_ChangeDirection: dir-y: Do not use float values\n");	
			}
		}	
		else {
			SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, 
			"PT: PT_BehaviorStateEventParseTrigger_Collision_ChangeDirection: Cannot find in fields: dir-y\n");	
			return SDL_FALSE;
		}
	}
	else {
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, 
		"PT: PT_BehaviorStateEventParseTrigger_Collision_ChangeDirection: Cannot find fields\n");
		
		return SDL_FALSE;
	}
	
	if ( _this->pBehaviorState )
	{
		PT_BehaviorState* pBehaviorState = 
		(PT_BehaviorState*)_this->pBehaviorState;
		
		if ( pBehaviorState->pBehavior )
		{
			PT_Behavior* pBehavior =
			(PT_Behavior*)pBehaviorState->pBehavior;
			
			if ( pBehavior->pSprite )
			{
				collisionChangeDirection->pSprite = (PT_Sprite*)pBehavior->pSprite;
			}
			else {
				SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, 
				"PT: PT_BehaviorStateEventParseTrigger_Collision_ChangeDirection: Invalid pSprite\n");
				
				return SDL_FALSE;
			}
		}
		else {
			SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, 
			"PT: PT_BehaviorStateEventParseTrigger_Collision_ChangeDirection: Invalid pBehavior\n");
			
			return SDL_FALSE;
		}
	}
	else {
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, 
		"PT: PT_BehaviorStateEventParseTrigger_Collision_ChangeDirection: Invalid pBehaviorState\n");
		
		return SDL_FALSE;
	}
	
	
	_this->trigger.collision.data = (void*)collisionChangeDirection;

	return SDL_TRUE;
}//PT_BehaviorStateEventParseTrigger_Collision_ChangeDirection



