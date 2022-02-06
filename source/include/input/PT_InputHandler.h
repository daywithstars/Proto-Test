/*
Copyright 2022 daywithstars

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

/* This is the container for inputs from input dir. */
#ifndef _PT_INPUTHANDLER_H_
#define _PT_INPUTHANDLER_H_

#include <SDL_stdinc.h>
#include <SDL_rect.h>

#include <json.h>

#include <PT_StringList.h>


typedef struct { 
	PT_String* fileName;
	PT_String* name;
	PT_StringList* keyMap;
}PT_InputHandler;

typedef struct {
	SDL_bool returnValue;
	SDL_FPoint mousePosition;
}PT_InputHandlerGrab;


PT_InputHandler* PT_InputHandlerCreate( json_value* jsonValue );

void PT_InputHandlerDestroy( PT_InputHandler* _this );

SDL_bool PT_InputHandlerGetButtonState( PT_InputHandler* _this, const char* mapName );
PT_InputHandlerGrab PT_InputHandlerGetGrabPosition( PT_InputHandler* _this, const char* mapName );


#endif /* _PT_INPUTHANDLER_H_ */



