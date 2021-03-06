/*
Copyright 2022 daywithstars

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

#include <json.h>

#include <SDL_log.h>

#include <PT_Behavior.h>
#include <PT_InputManager.h>
#include <PT_Parse.h>






//===================================== PRIVATE Functions

SDL_bool PT_BehaviorParse( PT_Behavior* _this, json_value* jsonValue );


//===================================== PUBLIC Functions

PT_Behavior* PT_BehaviorCreate( void* pSprite, const char* utf8_behaviorTemplate ) {

	/* Creation task */
	PT_Behavior* _this = (PT_Behavior*)malloc(sizeof(PT_Behavior));
	if ( !_this )
	{	
		SDL_LogMessage(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_WARN,
		"PT: PT_BehaviorCreate: Not enough memory\n");
		SDL_LogMessage(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_WARN,
		"PT: PT_BehaviorCreate: FILE %s, LINE %d\n", __FILE__, __LINE__);
		
		return NULL;
	}
	SDL_memset(_this, 0, sizeof(PT_Behavior));
	
	/* Template loading task */
	SDL_bool templateLoaded = SDL_TRUE;
	PT_String* path = PT_StringCreate();
	if ( !path )
	{
		templateLoaded = SDL_FALSE;
	}
	
	templateLoaded = PT_StringInsert(&path, ".json", 0);
	templateLoaded = PT_StringInsert(&path, utf8_behaviorTemplate, 0);
	templateLoaded = PT_StringInsert(&path, "assets/behavior/", 0);
	json_value* jsonValue = PT_ParseGetJsonValueFromFile((char*)path->utf8_string, SDL_TRUE);
	
	if ( !jsonValue )
	{
		templateLoaded = SDL_FALSE;
		json_value_free(jsonValue);
	}
	
	if ( !templateLoaded )
	{
		SDL_LogMessage(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_WARN,
		"PT: PT_BehaviorCreate: Cannot load template\n");
		SDL_LogMessage(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_WARN,
		"PT: PT_BehaviorCreate: FILE %s, LINE %d\n", __FILE__, __LINE__);
		
		PT_StringDestroy(path);
		PT_BehaviorDestroy(_this);
		return NULL;
	}
	
	/* Parse task */
	_this->pSprite = pSprite;
	if ( !PT_BehaviorParse(_this, jsonValue) )
	{
		SDL_LogMessage(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_WARN,
		"PT: PT_BehaviorCreate: Cannot parse\n");
		SDL_LogMessage(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_WARN,
		"PT: PT_BehaviorCreate: FILE %s, LINE %d\n", __FILE__, __LINE__);
		
		json_value_free(jsonValue);
		PT_StringDestroy(path);
		PT_BehaviorDestroy(_this);
		return NULL;
	}
	
	
	json_value_free(jsonValue);
	PT_StringDestroy(path);
	
	return _this;
}
void PT_BehaviorDestroy( PT_Behavior* _this ) {
	if ( !_this )
	{
		return;
	}
	
	_this->inputHandler = NULL;
	_this->currentState = NULL;
	PT_CallbackListDestroy(_this->callbackList);
	PT_BehaviorStateListDestroy(_this->behaviorStateList);
	
	free(_this);
}

void PT_BehaviorAddSimpleCallback( PT_Behavior* _this, const char* utf8_callbackName, 
	void (*callback)(void* _data) ) {
	_this->callbackList = PT_CallbackListAddSimple(_this->callbackList, utf8_callbackName, callback);
}
void PT_BehaviorAddStringCallback( PT_Behavior* _this, const char* utf8_callbackName, 
	SDL_bool (*callback)(void* _data, const char* utf8_string) ) {
	_this->callbackList = PT_CallbackListAddString(_this->callbackList, utf8_callbackName, callback);
}
void PT_BehaviorAddSDL_FPointCallback( PT_Behavior* _this, const char* utf8_callbackName, 
	void (*callback)(void* _data, SDL_FPoint) ) {
	_this->callbackList = PT_CallbackListAddSDL_FPoint(_this->callbackList, utf8_callbackName, callback);
}

void PT_BehaviorUpdate( PT_Behavior* _this, void* target, Sint32 elapsedTime ) {
	if ( _this->currentState )
	{
		PT_BehaviorStateUpdate(_this->currentState, target, elapsedTime);
	}
}

void PT_BehaviorChangeState( PT_Behavior* _this, const char* utf8_stateName ) {
	PT_BehaviorStateList* node = PT_BehaviorStateListGet(_this->behaviorStateList, utf8_stateName);
	
	if ( node )
	{
		_this->currentState = node->value;
	}
}

//===================================== PRIVATE Functions

//================= PT_Behavior

SDL_bool PT_BehaviorParse( PT_Behavior* _this, json_value* jsonValue ) {
	
	json_object_entry entry = PT_ParseGetObjectEntry_json_value(jsonValue, "settings input-template");
	if ( entry.value )
	{
		if ( strcmp(entry.value->u.string.ptr, "none") )
		{
			_this->inputHandler = PT_InputManagerGetInputHandler(entry.value->u.string.ptr);
		}
	}
	
	entry = PT_ParseGetObjectEntry_json_value(jsonValue, "states");
	if ( entry.value )
	{
		if ( entry.value->type == json_array )
		{
			for ( unsigned int i = 0; i < entry.value->u.array.length; i++ )
			{
				json_object_entry entry2 = 
				PT_ParseGetObjectEntry_json_value(entry.value->u.array.values[i], "name");
				if ( !entry2.value )
				{
					continue;
				}
				
				PT_BehaviorState* newState = PT_BehaviorStateCreate(
						entry.value->u.array.values[i],	
						(void*)_this		
					);
				if ( !newState )
				{
					continue;
				}
				
				_this->behaviorStateList = 
				PT_BehaviorStateListAdd(_this->behaviorStateList, entry2.value->u.string.ptr, newState);
				
				if ( i == 0 )
				{
					//add the first state
					_this->currentState = newState;
				}
			}
		}
	}

	return SDL_TRUE;
}





