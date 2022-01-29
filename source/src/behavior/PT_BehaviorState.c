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

#include <PT_BehaviorState.h>
#include <PT_Behavior.h>
#include <PT_Sprite.h>
#include <PT_Parse.h>




//===================================== PRIVATE Functions
SDL_bool PT_BehaviorStateParse( PT_BehaviorState* _this, json_value* jsonValue );
SDL_bool PT_BehaviorStateParseInputSimpleCallback( PT_BehaviorState* _this, 
	const char* utf8_index, const char* utf8_value );
SDL_bool PT_BehaviorStateParseInputChangeAnimation( PT_BehaviorState* _this,
	const char* utf8_index, const char* utf8_value, Uint64 charPos );
SDL_bool PT_BehaviorStateParseInputChangeState( PT_BehaviorState* _this,
	const char* utf8_index, const char* utf8_value, Uint64 charPos );

void PT_BehaviorStateUpdateInput( PT_BehaviorState* _this, void* target );
void PT_BehaviorStateUpdateAlways( PT_BehaviorState* _this, void* target );

//===================================== PUBLIC Functions
PT_BehaviorState* PT_BehaviorStateCreate( json_value* jsonValue, void* pBehavior ) {
	PT_BehaviorState* _this = (PT_BehaviorState*)malloc(sizeof(PT_BehaviorState));
	if ( !_this )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_BehaviorStateCreate: Not enough memory\n");
		return NULL;
	}
	SDL_memset(_this, 0, sizeof(PT_BehaviorState));
	
	_this->pBehavior = pBehavior;
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

	PT_StringListDestroy(_this->inputSimpleCallbacks);
	PT_StringListDestroy(_this->inputChangeStateList);
	PT_StringListDestroy(_this->inputChangeAnimationList);
	
	PT_StringListDestroy(_this->always);
	
	if ( _this->events )
	{
		for ( unsigned int i = 0; i < _this->eventsNum; i++ )
		{
			PT_BehaviorStateEventDestroy(_this->events[i]);
		}
		free(_this->events);
	}
	
	free(_this);
}//PT_BehaviorStateDestroy

void PT_BehaviorStateUpdate( PT_BehaviorState* _this, void* target, Sint32 elapsedTime ) {
	/*
		the "input" need to be an action from any input type.
		the "always" need to accour before any other and need to be always executed in each state.
	*/
	
	PT_BehaviorStateUpdateAlways(_this, target);
	PT_BehaviorStateUpdateInput(_this, target);
	
	for ( unsigned int i = 0; i < _this->eventsNum; i++ )
	{
		if ( _this->events[i] )
		{
			PT_BehaviorStateEventUpdate(_this->events[i], elapsedTime);
		}
	}
}//PT_BehaviorStateUpdate


//===================================== PRIVATE Functions

SDL_bool PT_BehaviorStateParse( PT_BehaviorState* _this, json_value* jsonValue ) {

	json_object_entry entry = PT_ParseGetObjectEntry_json_value(jsonValue, "input");
	if ( entry.name )
	{
		for ( unsigned int i = 0; i < entry.value->u.object.length; i++ )
		{
			if ( entry.value->u.object.values[i].value->type == json_array )
			{
				for ( unsigned int j = 0; j < entry.value->u.object.values[i].value->u.array.length; j++ )
				{
					const Uint64 changeStateLastCharPosition = 
					PT_StringGetOccurrencePositionBasicString(
						entry.value->u.object.values[i].value->u.array.values[j]->u.string.ptr,
						"change-state-",
						1
					);
					const Uint64 changeAnimationLastCharPosition =
					PT_StringGetOccurrencePositionBasicString(
						entry.value->u.object.values[i].value->u.array.values[j]->u.string.ptr,
						"change-animation-",
						1
					);
					
					if ( changeStateLastCharPosition != 0 )
					{
						if ( 
							!PT_BehaviorStateParseInputChangeState(
								_this,
								entry.value->u.object.values[i].name,
								entry.value->u.object.values[i].value->u.array.values[j]->u.string.ptr,
								changeStateLastCharPosition
							)
						)
						{
							continue;
						}	
					}	
					else if ( changeAnimationLastCharPosition != 0 )
					{
						if ( 
							!PT_BehaviorStateParseInputChangeAnimation(
								_this,
								entry.value->u.object.values[i].name,
								entry.value->u.object.values[i].value->u.array.values[j]->u.string.ptr,
								changeAnimationLastCharPosition
							)
						)
						{
							continue;
						}
					}
					else {
						if ( 
							!PT_BehaviorStateParseInputSimpleCallback(
								_this, entry.value->u.object.values[i].name,
								entry.value->u.object.values[i].value->u.array.values[j]->u.string.ptr) )
						{
							continue;
						}
					}
				}
			}
			else {
				const Uint64 changeStateLastCharPosition = PT_StringGetOccurrencePositionBasicString(
					entry.value->u.object.values[i].value->u.string.ptr,
					"change-state-",
					1
				);
				const Uint64 changeAnimationLastCharPosition = PT_StringGetOccurrencePositionBasicString(
					entry.value->u.object.values[i].value->u.string.ptr,
					"change-animation-",
					1
				);
				
				if ( changeStateLastCharPosition != 0 )
				{
					if ( 
						!PT_BehaviorStateParseInputChangeState(
							_this,
							entry.value->u.object.values[i].name,
							entry.value->u.object.values[i].value->u.string.ptr,
							changeStateLastCharPosition
						)
					)
					{
						continue;
					}				
				}
				else if ( changeAnimationLastCharPosition != 0 )
				{
					if ( 
						!PT_BehaviorStateParseInputChangeAnimation(
							_this,
							entry.value->u.object.values[i].name,
							entry.value->u.object.values[i].value->u.string.ptr,
							changeAnimationLastCharPosition
						)
					)
					{
						continue;
					}
				}
				else {
					if ( 
						!PT_BehaviorStateParseInputSimpleCallback(
							_this, entry.value->u.object.values[i].name,
							entry.value->u.object.values[i].value->u.string.ptr) )
					{
						continue;
					}
				}
			}
		}
	}
	
	entry = PT_ParseGetObjectEntry_json_value(jsonValue, "events");
	if ( entry.name )
	{
		if ( !(entry.value->type == json_array) )
		{
			SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, 
			"PT: PT_BehaviorStateEventParse: Invalid \"events\", see: behavior-template.json \n");
		}
		else {
			const unsigned int arrayLength = entry.value->u.array.length;
			_this->eventsNum = arrayLength;
			
			_this->events = (PT_BehaviorStateEvent**)malloc(sizeof(PT_BehaviorStateEvent*) * arrayLength);
			if ( !_this->events )
			{
				SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, 
				"PT: PT_BehaviorStateParse: Not enough memory for events!\n");
			}
			else {
				
				for ( unsigned int i = 0; i < arrayLength; i++ )
				{
					/*
						we parse the array position json_object to the create function, 
						to not restart the internal search from PT_BehaviorStateEventParse and get 
						always the first array element. 
					*/
					_this->events[i] = PT_BehaviorStateEventCreate(_this,
						entry.value->u.array.values[i]);
					if ( !_this->events[i] )
					{
						SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION,
						"PT: PT_BehaviorStateParse: Event[%d]!\n", i);
						break;
					}
				}
			}
		}
	}
	
	entry = PT_ParseGetObjectEntry_json_value(jsonValue, "always");
	if ( entry.name )
	{	
		for ( unsigned int i = 0; i < entry.value->u.array.length; i++ )
		{	
			PT_String* value = PT_StringCreate();
			PT_StringInsert(&value, entry.value->u.array.values[i]->u.string.ptr, 0);
			_this->always = PT_StringListCat(_this->always, "always", value);
		}
	}
	

	return SDL_TRUE;
}//PT_BehaviorStateParse

SDL_bool PT_BehaviorStateParseInputSimpleCallback( PT_BehaviorState* _this, 
	const char* utf8_index, const char* utf8_value ) {
	
	PT_String* callbackName = PT_StringCreate();
	if ( !PT_StringInsert(&callbackName, utf8_value, 0) )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_BehaviorStateParseInputSimpleCallback\n");
		PT_StringDestroy(callbackName);
		return SDL_FALSE;
	}
	
	_this->inputSimpleCallbacks = 
	PT_StringListCat(
		_this->inputSimpleCallbacks,
		utf8_index,
		callbackName
	);
	
	return SDL_TRUE;
}//PT_BehaviorStateParseInputSimpleCallback

SDL_bool PT_BehaviorStateParseInputChangeAnimation( PT_BehaviorState* _this,
	const char* utf8_index, const char* utf8_value, Uint64 charPos ) {
	
	PT_String* animName = PT_StringCreate();
	
	SDL_bool _return = PT_StringCopyFrom(
		animName,
		utf8_value,
		charPos,
		PT_StringCountBasicString(utf8_value),
		0
	);
	
	if ( _return == SDL_FALSE )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_BehaviorStateParseInputChangeAnimation!\n");
		PT_StringDestroy(animName);
		return SDL_FALSE;
	}
	
	_this->inputChangeAnimationList = PT_StringListCat(_this->inputChangeAnimationList,
		utf8_index, animName);
		
	return SDL_TRUE;
}//PT_BehaviorStateParseInputChangeAnimation

SDL_bool PT_BehaviorStateParseInputChangeState( PT_BehaviorState* _this,
	const char* utf8_index, const char* utf8_value, Uint64 charPos ) {
	
	PT_String* stateName = PT_StringCreate();
	
	SDL_bool _return = PT_StringCopyFrom(
		stateName,
		utf8_value,
		charPos,
		PT_StringCountBasicString(utf8_value),
		0
	);
	
	if ( _return == SDL_FALSE )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_BehaviorStateParseInputChangeState!\n");
		PT_StringDestroy(stateName);
		return SDL_FALSE;
	}
	
	_this->inputChangeStateList = PT_StringListCat(_this->inputChangeStateList, 
		utf8_index, stateName);
	
	return SDL_TRUE;
}//PT_BehaviorStateParseInputChangeState

void PT_BehaviorStateUpdateInput( PT_BehaviorState* _this, void* target ) {
	if ( !_this || !_this->pBehavior )
	{
		return;
	}

	PT_InputHandler* inputHandler = ((PT_Behavior*)_this->pBehavior)->inputHandler;
	
	PT_StringList* pList = _this->inputSimpleCallbacks;
	while ( pList )
	{
		if ( PT_InputHandlerGetButtonState(inputHandler, (char*)pList->index->utf8_string) )
		{
			//use the callback
			for ( unsigned int i = 0; i < pList->numValues; i++ )
			{
				PT_CallbackList* node = PT_CallbackListGet(
				((PT_Behavior*)_this->pBehavior)->callbackList, 
					(char*)pList->values[i]->utf8_string);
				if ( node )
				{
					if ( node->simpleCallback )
					{
						node->simpleCallback(target);
					}
				}
			}
		}
		else if ( 
				PT_InputHandlerGetGrabPosition(inputHandler, (char*)pList->index->utf8_string) 
				.returnValue
			)
		{
			//use the callback
			for ( unsigned int i = 0; i < pList->numValues; i++ )
			{
				PT_InputHandlerGrab grab = 
				PT_InputHandlerGetGrabPosition(inputHandler, (char*)pList->index->utf8_string);
				
				PT_CallbackList* node = PT_CallbackListGet(
				((PT_Behavior*)_this->pBehavior)->callbackList, 
					(char*)pList->values[i]->utf8_string);
				if ( node )
				{
					if ( node->SDL_FPointCallback )
					{
						node->SDL_FPointCallback(target, grab.mousePosition);
					}
				}
			}
		}
		
		pList = pList->next;
	}
	
	pList = _this->inputChangeAnimationList;
	SDL_bool animChanged = SDL_FALSE;
	while ( pList )
	{
		if ( PT_InputHandlerGetButtonState(inputHandler, (char*)pList->index->utf8_string) )
		{
			//use the change animation from PT_Sprite
			for ( unsigned int i = 0; i < pList->numValues; i++ )
			{
				if ( PT_SpriteChangeAnimation(target, (char*)pList->values[i]->utf8_string) )
				{
					animChanged = SDL_TRUE;
					break;
				}
			}
		}
		
		if ( animChanged )
		{
			break;
		}
		pList = pList->next;
	}
	
	/*
		we treat in separated loop, to not process any previous state input. 
	*/
	pList = _this->inputChangeStateList;
	while ( pList )
	{

		if ( PT_InputHandlerGetButtonState(inputHandler, (char*)pList->index->utf8_string) )
		{
			//use the callback 
			for ( unsigned int i = 0; i < pList->numValues; i++ )
			{
				PT_BehaviorChangeState((PT_Behavior*)_this->pBehavior, 
					(char*)pList->values[i]->utf8_string);
			}
		}
		
		pList = pList->next;

	}
}//PT_BehaviorStateUpdateInput

void PT_BehaviorStateUpdateAlways( PT_BehaviorState* _this, void* target ) {
	if ( !_this || !_this->pBehavior )
	{
		return;
	}

	PT_StringList* pList = _this->always;
	while ( pList )
	{
		//use the callback
		for ( unsigned int i = 0; i < pList->numValues; i++ )
		{
			PT_CallbackList* node = PT_CallbackListGet(
			((PT_Behavior*)_this->pBehavior)->callbackList, (char*)pList->values[i]->utf8_string);
			if ( node )
			{
				if ( node->simpleCallback )
				{
					node->simpleCallback(target);
				}
			}
		}
		
		pList = pList->next;
	}
}//PT_BehaviorStateUpdateAlways





