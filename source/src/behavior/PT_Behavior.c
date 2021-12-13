/*
Copyright 2021 daywithstars

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
#include <PT_BehaviorState.h>
#include <PT_BehaviorStateList.h>
#include <PT_InputManager.h>
#include <PT_CallbackList.h>
#include <PT_StringList.h>
#include <PT_Parse.h>



struct pt_behavior_state {

	PT_StringList* inputMap;
	PT_StringList* inputChangeStateMap;
	//PT_StringList* eventMap;
	
	unsigned int awaysNumIndex;
	char** aways;
};

struct pt_behavior {
	PT_InputHandler* inputHandler;
	PT_CallbackList* callbackList;
	PT_BehaviorStateList* behaviorStateList;
	PT_BehaviorState* currentState;
};


//===================================== PRIVATE Functions

SDL_bool PT_BehaviorParse( PT_Behavior* _this, json_value* jsonValue );

void PT_BehaviorChangeState( PT_Behavior* _this, const char* utf8_stateName );


//===================================== PUBLIC Functions

PT_Behavior* PT_BehaviorCreate( const char* utf8_behaviorTemplate ) {
	PT_Behavior* _this = (PT_Behavior*)malloc(sizeof(struct pt_behavior));
	if ( !_this )
	{	
		return NULL;
	}
	SDL_memset(_this, 0, sizeof(struct pt_behavior));
	
	PT_String* path = PT_StringCreate();
	PT_StringInsert(&path, ".json", 0);
	PT_StringInsert(&path, utf8_behaviorTemplate, 0);
	PT_StringInsert(&path, "assets/behavior/", 0);
	json_value* jsonValue = PT_ParseGetJsonValueFromFile((char*)path->utf8_string, SDL_TRUE);
	
	if ( !jsonValue )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_BehaviorCreate!\n");
		PT_BehaviorDestroy(_this);
		return NULL;
	}
	
	if ( !PT_BehaviorParse(_this, jsonValue) )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_BehaviorCreate: Cannot parse\n");
		PT_BehaviorDestroy(_this);
		json_value_free(jsonValue);
		PT_StringDestroy(path);
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
void PT_BehaviorAddSDL_FPointCallback( PT_Behavior* _this, const char* utf8_callbackName, 
	void (*callback)(void* _data, SDL_FPoint) ) {
	_this->callbackList = PT_CallbackListAddSDL_FPoint(_this->callbackList, utf8_callbackName, callback);
}

void PT_BehaviorUpdate( PT_Behavior* _this, void* target ) {
	if ( _this->currentState )
	{
		PT_BehaviorStateUpdate(_this->currentState, (void*)_this, target);
	}
}



//===================================== PRIVATE Functions

//================= PT_Behavior

SDL_bool PT_BehaviorParse( PT_Behavior* _this, json_value* jsonValue ) {
	/*
		see the template: games/default-templates/behavior-folder/behavior-template.json
	*/
	
	json_object_entry entry = PT_ParseGetObjectEntry_json_value(jsonValue, "settings keyMap");
	if ( entry.name )
	{
		_this->inputHandler = PT_InputManagerGetInputHandler(entry.value->u.string.ptr);
		if ( !_this->inputHandler )
		{
			SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_BehaviorParse: Invalid inputHandler\n");
			return SDL_FALSE;
		}
	}
	
	entry = PT_ParseGetObjectEntry_json_value(jsonValue, "states");
	if ( !entry.name )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_BehaviorParse: Can't find any state!\n");
		return SDL_FALSE;
	}
	
	for ( unsigned int i = 0; i < entry.value->u.array.length; i++ )
	{
		json_object_entry entry2 = 
		PT_ParseGetObjectEntry_json_value(entry.value->u.array.values[i], "name");
		
		PT_BehaviorState* newState = PT_BehaviorStateCreate(entry.value->u.array.values[i]);
		if ( !newState )
		{
			SDL_Log("(*)PT: PT_BehaviorStateParse: Not enough memory\n");
			return SDL_FALSE;
		}
		
		_this->behaviorStateList = 
		PT_BehaviorStateListAdd(_this->behaviorStateList, entry2.value->u.string.ptr, newState);
		
		if ( i == 0 )
		{
			//add the first state
			_this->currentState = newState;
		}
	}

	return SDL_TRUE;
}

void PT_BehaviorChangeState( PT_Behavior* _this, const char* utf8_stateName ) {
	PT_BehaviorStateList* node = PT_BehaviorStateListGet(_this->behaviorStateList, utf8_stateName);
	
	if ( node )
	{
		_this->currentState = node->value;
	}
}

//================= PT_BehaviorState implementation

SDL_bool PT_BehaviorStateParse( PT_BehaviorState* _this, json_value* jsonValue ) {

	json_object_entry entry = PT_ParseGetObjectEntry_json_value(jsonValue, "input");
	if ( entry.name )
	{
		for ( unsigned int i = 0; i < entry.value->u.object.length; i++ )
		{
			const Uint64 changeStateLastCharPosition = PT_StringGetOccurrencePositionBasicString(
				entry.value->u.object.values[i].value->u.string.ptr,
				"change-state-",
				1
			);
			
			if ( changeStateLastCharPosition == 0 )
			{
				PT_String* callbackName = PT_StringCreate();
				PT_StringInsert(&callbackName, entry.value->u.object.values[i].value->u.string.ptr, 0);
				
				_this->inputMap = PT_StringListAdd(_this->inputMap, entry.value->u.object.values[i].name,
				callbackName);
			}
			else {
				PT_String* stateName = PT_StringCreate();
				
				SDL_bool _return = PT_StringCopyFrom(
					stateName,
					entry.value->u.object.values[i].value->u.string.ptr,
					changeStateLastCharPosition,
					PT_StringCountBasicString(entry.value->u.object.values[i].value->u.string.ptr),
					0
				);
				
				if ( _return == SDL_FALSE )
				{
					SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_BehaviorStateParse!");
					PT_StringDestroy(stateName);
					continue;
				}
				
				_this->inputChangeStateMap = PT_StringListAdd(_this->inputChangeStateMap, 
					entry.value->u.object.values[i].name, stateName);
			}
		}
	}
	
	entry = PT_ParseGetObjectEntry_json_value(jsonValue, "event");
	if ( entry.name )
	{
		//printf("Yes we have events!\n");
		//printf("\t so, implement it\n");
	}
	
	entry = PT_ParseGetObjectEntry_json_value(jsonValue, "aways");
	if ( entry.name )
	{
		_this->awaysNumIndex = entry.value->u.array.length;
		_this->aways = (char**)malloc(_this->awaysNumIndex);
		if ( !_this->aways )
		{
			SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_BehaviorStateParse: Not enough memory\n");
		}
		else {
		
			for ( unsigned int i = 0; i < entry.value->u.array.length; i++ )
			{	
				_this->aways[i] = (char*)malloc(entry.value->u.array.values[i]->u.string.length);
				strcpy(_this->aways[i], entry.value->u.array.values[i]->u.string.ptr);
			}
		}
	}
	

	return SDL_TRUE;
}

PT_BehaviorState* PT_BehaviorStateCreate( json_value* jsonValue ) {
	PT_BehaviorState* _this = (PT_BehaviorState*)malloc(sizeof(PT_BehaviorState));
	if ( !_this )
	{
		return NULL;
	}
	SDL_memset(_this, 0, sizeof(PT_BehaviorState));
	
	if ( !PT_BehaviorStateParse(_this, jsonValue) )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_BehaviorStateCreate: Cannot parse!\n");
		PT_BehaviorStateDestroy(_this);
		return NULL;
	}
	
	return _this;
}
void PT_BehaviorStateDestroy( PT_BehaviorState* _this ) {
	if ( !_this )
	{
		return;
	}

	PT_StringListDestroy(_this->inputMap);
	PT_StringListDestroy(_this->inputChangeStateMap);
	if ( _this->aways )
	{
		for ( unsigned int i = 0; i < _this->awaysNumIndex; i++ )
		{
			free(_this->aways[i]);
		}
		free(_this->aways);
	}
	
	free(_this);
}

void PT_BehaviorStateUpdateInput( PT_BehaviorState* _this, PT_Behavior* behavior, void* target ) {
	PT_InputHandler* inputHandler = behavior->inputHandler;
	
	PT_StringList* pList = _this->inputMap;
	while ( pList )
	{
		if ( PT_InputHandlerGetButtonState(inputHandler, (char*)pList->index->utf8_string) )
		{
			//callback call
			PT_CallbackList* node = PT_CallbackListGet(behavior->callbackList, 
				(char*)pList->value->utf8_string);
			if ( node )
			{
				if ( node->simpleCallback )
				{
					node->simpleCallback(target);
				}
			}
		}
		else if ( 
				PT_InputHandlerGetGrabPosition(inputHandler, (char*)pList->index->utf8_string) 
				.returnValue
			)
		{
			//callback
			PT_InputHandlerGrab grab = 
			PT_InputHandlerGetGrabPosition(inputHandler, (char*)pList->index->utf8_string);
			
			PT_CallbackList* node = PT_CallbackListGet(behavior->callbackList, 
				(char*)pList->value->utf8_string);
			if ( node )
			{
				if ( node->SDL_FPointCallback )
				{
					node->SDL_FPointCallback(target, grab.mousePosition);
				}
			}
		}
		
		pList = pList->next;
	}
	
	/*
		we treat in separated loop, to not process any previous state input. 
	*/
	pList = _this->inputChangeStateMap;
	while ( pList )
	{
		if ( PT_InputHandlerGetButtonState(inputHandler, (char*)pList->index->utf8_string) )
		{
			//callback call
			PT_BehaviorChangeState(behavior, (char*)pList->value->utf8_string);
		}
		
		pList = pList->next;
	}
}
void PT_BehaviorStateUpdateAways( PT_BehaviorState* _this, PT_Behavior* behavior, void* target ) {

	for ( unsigned int i = 0; i < _this->awaysNumIndex; i++ )
	{
		//callback call
		PT_CallbackList* node = PT_CallbackListGet(behavior->callbackList, _this->aways[i]);
		if ( node )
		{
			if ( node->simpleCallback )
			{
				node->simpleCallback(target);
			}
		}
	}
}
void PT_BehaviorStateUpdate( PT_BehaviorState* _this, void* behaviorData, void* target ) {
	/*
		the "input" need to be an action from any input type.
		the "aways" need to accour before any other and need to be aways executed in each state.
		the "event" need to be something happend that is out of control from the *target "input".
	*/
	PT_Behavior* behavior = (PT_Behavior*)behaviorData;

	
	PT_BehaviorStateUpdateAways(_this, behavior, target);
	PT_BehaviorStateUpdateInput(_this, behavior, target);
}



