/*
Copyright 2022 daywithstars

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

#include <PT_BehaviorStateEventInput.h>
#include <PT_Parse.h>



SDL_bool PT_BehaviorStateEventParseTrigger_Input_PlaySample( 
	PT_BehaviorStateEvent* _this, json_value* jsonValue ) {
	
	PT_BehaviorStateEventInputPlaySound* inputPlaySound = 
	(PT_BehaviorStateEventInputPlaySound*)malloc(sizeof(PT_BehaviorStateEventInputPlaySound));
	if ( !inputPlaySound )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, 
		"PT: PT_BehaviorStateEventParseTrigger_Input_PlaySample: Not enough memory\n");
		
		_this->trigger.input.data = NULL;
		return SDL_FALSE;
	}
	SDL_memset(inputPlaySound, 0, sizeof(PT_BehaviorStateEventInputPlaySound));
	
	json_object_entry fieldsEntry = 
	PT_ParseGetObjectEntry_json_value(jsonValue, "trigger value fields");
	if ( fieldsEntry.name )
	{
		fieldsEntry = 
		PT_ParseGetObjectEntry_json_value(jsonValue, "trigger value fields key-map");
		if ( fieldsEntry.name )
		{
			PT_String* keyMap = PT_StringCreate();
			if ( !keyMap )
			{
				SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, 
				"PT: PT_BehaviorStateEventParseTrigger_Input_PlaySample!\n");
				
				return SDL_FALSE;	
			}
			
			if ( !PT_StringInsert(&keyMap, fieldsEntry.value->u.string.ptr, 0) )
			{
				SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, 
				"PT: PT_BehaviorStateEventParseTrigger_Input_PlaySample!\n");
	
				return SDL_FALSE;	
			}
			
			inputPlaySound->keyMap = keyMap;
		}
		else {
			SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, 
			"PT: PT_BehaviorStateEventParseTrigger_Input_PlaySample: Cannot find in fields: key-map\n");
			
			return SDL_FALSE;
		}
		
		fieldsEntry = 
		PT_ParseGetObjectEntry_json_value(jsonValue, "trigger value fields sample-name");
		if ( fieldsEntry.name )
		{
			PT_String* sampleName = PT_StringCreate();
			if ( !sampleName )
			{
				SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, 
				"PT: PT_BehaviorStateEventParseTrigger_Input_PlaySample!\n");
				
				return SDL_FALSE;	
			}
			
			if ( !PT_StringInsert(&sampleName, fieldsEntry.value->u.string.ptr, 0) )
			{
				SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, 
				"PT: PT_BehaviorStateEventParseTrigger_Input_PlaySample!\n");
	
				return SDL_FALSE;	
			}
			
			inputPlaySound->sound.name = sampleName;
		}
		else {
			SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, 
			"PT: PT_BehaviorStateEventParseTrigger_Input_PlaySample: Cannot find in fields: sample-name\n");
			
			return SDL_FALSE;
		}
		
		fieldsEntry = 
		PT_ParseGetObjectEntry_json_value(jsonValue, "trigger value fields loop");
		if ( fieldsEntry.name )
		{
			inputPlaySound->sound.loop = fieldsEntry.value->u.integer;
		}
		else {
			SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, 
			"PT: PT_BehaviorStateEventParseTrigger_Input_PlaySample: Cannot find in fields: loop\n");
			
			return SDL_FALSE;
		}
	}
	else {
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, 
		"PT: PT_BehaviorStateEventParseTrigger_Input_PlaySample: Cannot find fields\n");
		
		return SDL_FALSE;
	}
	
	
	_this->trigger.input.data = (void*)inputPlaySound;
	
	return SDL_TRUE;
}//PT_BehaviorStateEventParseTrigger_Input_PlaySample

SDL_bool PT_BehaviorStateEventParseTrigger_Input_PlayMusic( 
	PT_BehaviorStateEvent* _this, json_value* jsonValue ) {
	
	PT_BehaviorStateEventInputPlaySound* inputPlaySound = 
	(PT_BehaviorStateEventInputPlaySound*)malloc(sizeof(PT_BehaviorStateEventInputPlaySound));
	if ( !inputPlaySound )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, 
		"PT: PT_BehaviorStateEventParseTrigger_Input_PlayMusic: Not enough memory\n");
		
		_this->trigger.input.data = NULL;
		return SDL_FALSE;
	}
	SDL_memset(inputPlaySound, 0, sizeof(PT_BehaviorStateEventInputPlaySound));
	
	json_object_entry fieldsEntry = 
	PT_ParseGetObjectEntry_json_value(jsonValue, "trigger value fields");
	if ( fieldsEntry.name )
	{
		fieldsEntry = 
		PT_ParseGetObjectEntry_json_value(jsonValue, "trigger value fields key-map");
		if ( fieldsEntry.name )
		{
			PT_String* keyMap = PT_StringCreate();
			if ( !keyMap )
			{
				SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, 
				"PT: PT_BehaviorStateEventParseTrigger_Input_PlayMusic!\n");
				
				return SDL_FALSE;	
			}
			
			if ( !PT_StringInsert(&keyMap, fieldsEntry.value->u.string.ptr, 0) )
			{
				SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, 
				"PT: PT_BehaviorStateEventParseTrigger_Input_PlayMusic!\n");
	
				return SDL_FALSE;	
			}
			
			inputPlaySound->keyMap = keyMap;
		}
		else {
			SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, 
			"PT: PT_BehaviorStateEventParseTrigger_Input_PlayMusic: Cannot find in fields: key-map\n");
			
			return SDL_FALSE;
		}
		
		fieldsEntry = 
		PT_ParseGetObjectEntry_json_value(jsonValue, "trigger value fields music-name");
		if ( fieldsEntry.name )
		{
			PT_String* musicName = PT_StringCreate();
			if ( !musicName )
			{
				SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, 
				"PT: PT_BehaviorStateEventParseTrigger_Input_PlayMusic!\n");
				
				return SDL_FALSE;	
			}
			
			if ( !PT_StringInsert(&musicName, fieldsEntry.value->u.string.ptr, 0) )
			{
				SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, 
				"PT: PT_BehaviorStateEventParseTrigger_Input_PlayMusic!\n");
	
				return SDL_FALSE;	
			}
			
			inputPlaySound->sound.name = musicName;
		}
		else {
			SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, 
			"PT: PT_BehaviorStateEventParseTrigger_Input_PlayMusic: Cannot find in fields: music-name\n");
			
			return SDL_FALSE;
		}
		
		fieldsEntry = 
		PT_ParseGetObjectEntry_json_value(jsonValue, "trigger value fields loop");
		if ( fieldsEntry.name )
		{
			inputPlaySound->sound.loop = fieldsEntry.value->u.integer;
		}
		else {
			SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, 
			"PT: PT_BehaviorStateEventParseTrigger_Input_PlayMusic: Cannot find in fields: loop\n");
			
			return SDL_FALSE;
		}
	}
	else {
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, 
		"PT: PT_BehaviorStateEventParseTrigger_Input_PlayMusic: Cannot find fields\n");
		
		return SDL_FALSE;
	}
	
	
	_this->trigger.input.data = (void*)inputPlaySound;
	
	return SDL_TRUE;
}//PT_BehaviorStateEventParseTrigger_Input_PlayMusic

SDL_bool PT_BehaviorStateEventParseTrigger_Input_ChangeScreen( 
	PT_BehaviorStateEvent* _this, json_value* jsonValue ) {
	
	//In construction
	
	_this->flags = PT_BEHAVIOR_STATE_EVENT_TRIGGER_TYPE_NONE;
	if ( jsonValue)
	{
		jsonValue = NULL;
	}
	
	return SDL_TRUE;
}//PT_BehaviorStateEventParseTrigger_Input_ChangeScreen
	
	
	
	
	
