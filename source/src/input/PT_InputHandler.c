/*
Copyright 2021 daywithstars

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

#include <PT_InputHandler.h>
#include <PT_Sprite.h>
#include <PT_Parse.h>
#include <PT_InputManager.h>
#include <PT_Keyboard.h>
#include <PT_Mouse.h>


typedef enum {
	PT_INPUT_HANDLER_INPUT_TYPE_NONE,
	PT_INPUT_HANDLER_INPUT_TYPE_KEYBOARD,
	PT_INPUT_HANDLER_INPUT_TYPE_MOUSE
}PT_InputHandlerInputTypes;


//===================================== PRIVATE Functions

void PT_InputHandlerParse( PT_InputHandler* _this, json_value* jsonValue );


//===================================== PUBLIC Functions

PT_InputHandler* PT_InputHandlerCreate( json_value* jsonValue ) {
	PT_InputHandler* _this = (PT_InputHandler*)malloc(sizeof(PT_InputHandler));
	if ( !_this )
	{
		return NULL;
	}
	SDL_memset(_this, 0, sizeof(PT_InputHandler));
	
	_this->fileName = PT_StringCreate();
	_this->name = PT_StringCreate();
	
	PT_InputHandlerParse(_this, jsonValue);
	
	return _this;
}//PT_InputHandlerCreate

void PT_InputHandlerDestroy( PT_InputHandler* _this ) {
	if ( !_this )
	{
		return;
	}
	
	PT_StringDestroy(_this->fileName);
	PT_StringDestroy(_this->name);
	PT_StringListDestroy(_this->keyMap);
	
	free(_this);
}//PT_InputHandlerDestroy

SDL_bool PT_InputHandlerGetButtonState( PT_InputHandler* _this, const char* mapName ) {
	PT_StringList* node = PT_StringListGet(_this->keyMap, mapName);
	
	if ( !node )
	{
		return SDL_FALSE;
	}
	
	const PT_InputHandlerInputTypes inputType = 
	node->value->utf8_string[0] == 'k' && node->value->utf8_string[1] == 'y' && 
	node->value->utf8_string[2] == 'd' ? PT_INPUT_HANDLER_INPUT_TYPE_KEYBOARD :
	
	node->value->utf8_string[0] == 'm' && node->value->utf8_string[1] == 's' &&
	node->value->utf8_string[2] == 'e' ? PT_INPUT_HANDLER_INPUT_TYPE_MOUSE : 
	PT_INPUT_HANDLER_INPUT_TYPE_NONE;
	
	if ( inputType == PT_INPUT_HANDLER_INPUT_TYPE_NONE )
	{
		return SDL_FALSE;
	}
	
	//button pressed
	if ( node->value->utf8_string[4] == 'p' )
	{
		if ( inputType == PT_INPUT_HANDLER_INPUT_TYPE_KEYBOARD )
		{
			return PT_InputManagerKeyboardGetKeyDown(
				PT_KeyboardGetScancodeByString( (char*)(&node->value->utf8_string[6]) )
			);
		}
		else if ( inputType == PT_INPUT_HANDLER_INPUT_TYPE_MOUSE )
		{
			return PT_InputManagerMouseGetButtonDown(
				PT_MouseGetButtonByString( (char*)(&node->value->utf8_string[6]) )
			);
		}
	}
	//button release
	else if ( node->value->utf8_string[4] == 'r' )
	{
		if ( inputType == PT_INPUT_HANDLER_INPUT_TYPE_KEYBOARD )
		{
			return PT_InputManagerKeyboardGetKeyUp(
				PT_KeyboardGetScancodeByString( (char*)(&node->value->utf8_string[6]) )
			);
		}
		else if ( inputType == PT_INPUT_HANDLER_INPUT_TYPE_MOUSE )
		{
			return PT_InputManagerMouseGetButtonUp(
				PT_MouseGetButtonByString( (char*)(&node->value->utf8_string[6]) )
			);
		}
	}
	//button hold
	else if ( node->value->utf8_string[4] == 'h' )
	{
		if ( inputType == PT_INPUT_HANDLER_INPUT_TYPE_KEYBOARD )
		{
			return PT_InputManagerKeyboardGetKeyHold(
				PT_KeyboardGetScancodeByString( (char*)(&node->value->utf8_string[6]) )
			);
		}
		else if ( inputType == PT_INPUT_HANDLER_INPUT_TYPE_MOUSE )
		{
			return PT_InputManagerMouseGetButtonHold(
				PT_MouseGetButtonByString( (char*)(&node->value->utf8_string[6]) )
			);
		}
	}
	
	
	return SDL_FALSE;
}//PT_InputHandlerGetButtonState

PT_InputHandlerGrab PT_InputHandlerGetGrabPosition( PT_InputHandler* _this, const char* mapName ) {
	PT_StringList* node = PT_StringListGet(_this->keyMap, mapName);
	
	PT_InputHandlerGrab inputHandlerGrab;
	SDL_memset(&inputHandlerGrab, 0, sizeof(PT_InputHandlerGrab));
	
	if ( !node )
	{
		return inputHandlerGrab;
	}
	
	const PT_InputHandlerInputTypes inputType = 
	node->value->utf8_string[0] == 'm' && node->value->utf8_string[1] == 's' &&
	node->value->utf8_string[2] == 'e' ? PT_INPUT_HANDLER_INPUT_TYPE_MOUSE : 
	PT_INPUT_HANDLER_INPUT_TYPE_NONE;
	
	if ( inputType == PT_INPUT_HANDLER_INPUT_TYPE_NONE )
	{
		return inputHandlerGrab;
	}
	
	
	//button pressed and grab
	if ( node->value->utf8_string[4] == 'p' &&  node->value->utf8_string[5] == 'g' )
	{
		if ( 
			PT_InputManagerMouseGetButtonDown(
			PT_MouseGetButtonByString((char*)(&node->value->utf8_string[7])))
			)
		{
			Sint32 mx=0, my=0;
			PT_InputManagerMouseGetPosition(&mx, &my);
			
			inputHandlerGrab.returnValue = SDL_TRUE;
			inputHandlerGrab.mousePosition.x = mx;
			inputHandlerGrab.mousePosition.y = my;
		}	
	}
	//button release and grab
	else if ( node->value->utf8_string[4] == 'r' &&  node->value->utf8_string[5] == 'g' )
	{
		if ( 
			PT_InputManagerMouseGetButtonUp(
			PT_MouseGetButtonByString((char*)(&node->value->utf8_string[7])))
			)
		{
			Sint32 mx=0, my=0;
			
			PT_InputManagerMouseGetPosition(&mx, &my);
			
			inputHandlerGrab.returnValue = SDL_TRUE;
			inputHandlerGrab.mousePosition.x = mx;
			inputHandlerGrab.mousePosition.y = my;
		}	
	}
	//button hold and grab
	else if ( node->value->utf8_string[4] == 'h' &&  node->value->utf8_string[5] == 'g' )
	{
		if ( 
			PT_InputManagerMouseGetButtonHold(
			PT_MouseGetButtonByString((char*)(&node->value->utf8_string[7])))
			)
		{
			Sint32 mx=0, my=0;
			
			PT_InputManagerMouseGetPosition(&mx, &my);
			
			inputHandlerGrab.returnValue = SDL_TRUE;
			inputHandlerGrab.mousePosition.x = mx;
			inputHandlerGrab.mousePosition.y = my;
		}
	}
	
	return inputHandlerGrab;
}//PT_InputHandlerGrab

//===================================== PRIVATE Functions

void PT_InputHandlerParse( PT_InputHandler* _this, json_value* jsonValue ) {

	/*
		see the template: games/default-templates/input-folder/input-template.json
	*/

	json_object_entry entry = PT_ParseGetObjectEntry_json_value(jsonValue, "file");
	PT_StringInsert(&_this->fileName, entry.value->u.string.ptr, 0);
	
	entry = PT_ParseGetObjectEntry_json_value(jsonValue, "settings name");
	PT_StringInsert(&_this->name, entry.value->u.string.ptr, 0);
	
	entry = PT_ParseGetObjectEntry_json_value(jsonValue, "buttons-map");
	
	if ( entry.value->type != json_array )
	{
		return;
	}
	
	for ( unsigned int i = 0; i < entry.value->u.array.length; i++ )
	{
		PT_String* mapString = PT_StringCreate();
		PT_StringInsert(
			&mapString, 
			entry.value->u.array.values[i]->u.object.values[0].value->u.string.ptr,0
		);
			
		_this->keyMap = PT_StringListAdd(
			_this->keyMap, 
			entry.value->u.array.values[i]->u.object.values[0].name, 
			mapString
		);
	}
}//PT_InputHandlerParse



