/*
Copyright 2022 daywithstars

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

#include <SDL_log.h>

#include <PT_InputManager.h>
#include <PT_Keyboard.h>
#include <PT_Mouse.h>
#include <PT_InputHandlerList.h>
#include <PT_Parse.h>


typedef struct {
	PT_Keyboard* keyboard;
	PT_Mouse* mouse;
	PT_InputHandlerList* inputHandlerList;
}PT_InputManager;


static PT_InputManager* ptInputManager = NULL;


//===================================== PRIVATE Functions

void PT_InputManagerParse( );

//===================================== PUBLIC Functions

SDL_bool PT_InputManagerCreate( ) {
	if ( ptInputManager )
	{
		return SDL_TRUE;
	}
	
	ptInputManager = (PT_InputManager*)malloc(sizeof(PT_InputManager));
	if ( !ptInputManager )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, 
		"PT: PT_InputManagerCreate: Not enough memory!\n");
		return SDL_FALSE;
	}
	SDL_memset(ptInputManager, 0, sizeof(PT_InputManager));
	
	ptInputManager->keyboard = PT_KeyboardCreate();
	if ( !ptInputManager->keyboard ) 
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, 
		"PT: PT_InputManagerCreate!\n");
		PT_InputManagerDestroy();
		return SDL_FALSE;
	}
	
	ptInputManager->mouse = PT_MouseCreate();
	if ( !ptInputManager->mouse )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, 
		"PT: PT_InputManagerCreate!\n");
		PT_InputManagerDestroy();
		return SDL_FALSE;
	}
	
	PT_InputManagerParse();
	
	return SDL_TRUE;
}//PT_InputManagerCreate

void PT_InputManagerDestroy( ) {
	if ( !ptInputManager )
	{
		return;
	}
	
	PT_KeyboardDestroy(ptInputManager->keyboard);
	ptInputManager->keyboard = NULL;
	
	PT_MouseDestroy(ptInputManager->mouse);
	ptInputManager->mouse = NULL;
	
	PT_InputHandlerListDestroy(ptInputManager->inputHandlerList);
	
	free(ptInputManager);
	ptInputManager = NULL;
}//PT_InputManagerDestroy

void PT_InputManagerClearState( ) {
	PT_KeyboardClearState(ptInputManager->keyboard);
	PT_MouseClearState(ptInputManager->mouse);
}//PT_InputManagerClearState

void PT_InputManagerUpdate( const SDL_Event* ev ) {
	PT_KeyboardUpdate(ptInputManager->keyboard, ev);
	PT_MouseUpdate(ptInputManager->mouse, ev);
}//PT_InputManagerUpdate

PT_InputHandler* PT_InputManagerGetInputHandler( const char* utf8_inputHandlerName ) {
	PT_InputHandlerList* node = 
	PT_InputHandlerListGet(ptInputManager->inputHandlerList, utf8_inputHandlerName);
	
	if ( !node )
	{
		SDL_Log("(*)PT: PT_InputManagerGetInputHandler: Cannot find \"%s\" inputHandler\n", 
		utf8_inputHandlerName);
		return NULL;
	}
	
	return node->value;
}//PT_InputManagerGetInputHandler

SDL_bool PT_InputManagerKeyboardGetKeyDown( SDL_Scancode key ) {
	return PT_KeyboardGetKeyDown(ptInputManager->keyboard, key);
}
SDL_bool PT_InputManagerKeyboardGetKeyUp( SDL_Scancode key ) {
	return PT_KeyboardGetKeyUp(ptInputManager->keyboard, key);
}
SDL_bool PT_InputManagerKeyboardGetKeyHold( SDL_Scancode key ) {
	return PT_KeyboardGetKeyHold(ptInputManager->keyboard, key);
}

SDL_bool PT_InputManagerKeyboardGetKeyEvent( ) {
	return PT_KeyboardGetKeyEvent(ptInputManager->keyboard);
}

void PT_InputManagerMouseGetPosition( Sint32* x, Sint32* y ) {
	PT_MouseGetPosition(ptInputManager->mouse, x, y);
}
void PT_InputManagerMouseGetWheelScroll( Sint32* wheelX, Sint32* wheelY ) {
	PT_MouseGetWheelScroll(ptInputManager->mouse, wheelX, wheelY);
}

SDL_bool PT_InputManagerMouseGetButtonDown( Uint8 button ) {
	return PT_MouseGetButtonDown(ptInputManager->mouse, button);
}
SDL_bool PT_InputManagerMouseGetButtonUp( Uint8 button ) {
	return PT_MouseGetButtonUp(ptInputManager->mouse, button);
}
SDL_bool PT_InputManagerMouseGetButtonHold( Uint8 button ) {
	return PT_MouseGetButtonHold(ptInputManager->mouse, button);
}

//===================================== PRIVATE Functions

void PT_InputManagerParse( ) {
	/*
		see the template: games/default-templates/input-folder/input-list.json
	*/
	
	PT_Parse* parse = PT_ParseCreate();
	
	if ( !PT_ParseOpenFile(parse, "assets/input/input-list.json", SDL_TRUE) )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_InputManagerParse!\n");
		PT_ParseDestroy(parse);
		return;
	}
	
	json_object_entry entry = PT_ParseGetObjectEntry(parse, "files");
	if ( !entry.name )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_InputManagerParse!\n");
		PT_ParseDestroy(parse);
		return;
	}
	
	if ( entry.value->type != json_array )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_InputManagerParse!\n");
		PT_ParseDestroy(parse);
		return;
	}
	
	for ( unsigned int i = 0; i < entry.value->u.array.length; i++ )
	{
		PT_String* inputHandlerPath = PT_StringCreate();
		
		if ( !PT_StringInsert(&inputHandlerPath, ".json", 0) )
		{
			SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_InputManagerParse!\n");
			continue;
		}
		if ( !PT_StringInsert(&inputHandlerPath, entry.value->u.array.values[i]->u.string.ptr, 0) )
		{
			SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_InputManagerParse!\n");
			continue;
		}
		if ( !PT_StringInsert(&inputHandlerPath, "assets/input/", 0) )
		{
			SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_InputManagerParse!\n");
			continue;
		}
		
		json_value* jsonValue = 
		PT_ParseGetJsonValueFromFile((char*)inputHandlerPath->utf8_string, SDL_TRUE);
		
		if ( !jsonValue )
		{
			SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, 
			"PT: PT_InputManagerParse: Cannot find %s jumping to next\n", 
			entry.value->u.array.values[i]->u.string.ptr);
			
			PT_StringDestroy(inputHandlerPath);
			continue;
		}
		
		json_object_entry entry = PT_ParseGetObjectEntry_json_value(jsonValue, "settings name");
		if ( !entry.name )
		{
			SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, 
			"PT: PT_InputManagerParse: Cannot find inputHandler name setting\n");
			
			json_value_free(jsonValue);
			PT_StringDestroy(inputHandlerPath);
			continue;
		}
		
		PT_InputHandler* inputHandler = PT_InputHandlerCreate(jsonValue);
		if ( !inputHandler )
		{
			SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, 
			"PT: PT_InputManagerParse!\n");
			
			json_value_free(jsonValue);
			PT_StringDestroy(inputHandlerPath);
			continue;
		}

		ptInputManager->inputHandlerList = 
		PT_InputHandlerListAdd(ptInputManager->inputHandlerList, entry.value->u.string.ptr,
		inputHandler);
		
		json_value_free(jsonValue);
		PT_StringDestroy(inputHandlerPath);
	}
	
	PT_ParseDestroy(parse);
}//PT_InputManagerParse



