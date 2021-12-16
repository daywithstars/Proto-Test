/*
Copyright 2021 daywithstars

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

#include <SDL_log.h>

#include <PT_BehaviorState.h>
#include <PT_BehaviorStateList.h>
#include <PT_StringList.h>
#include <PT_Parse.h>
#include <PT_InputHandler.h>
#include <PT_CallbackList.h>


struct pt_behavior_state {

	PT_StringList* inputMap;
	PT_StringList* inputChangeStateMap;
	
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
SDL_bool PT_BehaviorStateParse( PT_BehaviorState* _this, json_value* jsonValue );
void PT_BehaviorStateUpdateInput( PT_BehaviorState* _this, struct pt_behavior* behavior, void* target );
void PT_BehaviorStateUpdateAways( PT_BehaviorState* _this, struct pt_behavior* behavior, void* target );

void PT_BehaviorChangeState( PT_Behavior* _this, const char* utf8_stateName );

//===================================== PUBLIC Functions
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
}//PT_BehaviorStateCreate

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
}//PT_BehaviorStateDestroy

void PT_BehaviorStateUpdate( PT_BehaviorState* _this, void* behaviorData, void* target ) {
	/*
		the "input" need to be an action from any input type.
		the "aways" need to accour before any other and need to be aways executed in each state.
		the "event" need to be something happend that is out of control from the *target "input".
	*/
	struct pt_behavior* behavior = (struct pt_behavior*)behaviorData;

	
	PT_BehaviorStateUpdateAways(_this, behavior, target);
	PT_BehaviorStateUpdateInput(_this, behavior, target);
}//PT_BehaviorStateUpdate



//===================================== PRIVATE Functions

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
}//PT_BehaviorStateParse

void PT_BehaviorStateUpdateInput( PT_BehaviorState* _this, struct pt_behavior* behavior, void* target ) {
	SDL_PumpEvents();

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
}//PT_BehaviorStateUpdateInput

void PT_BehaviorStateUpdateAways( PT_BehaviorState* _this, struct pt_behavior* behavior, void* target ) {

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
}//PT_BehaviorStateUpdateAways





