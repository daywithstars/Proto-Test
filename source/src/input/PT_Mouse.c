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

#include <SDL_log.h>

#include <PT_Mouse.h>

#define PT_MOUSE_NUM_BUTTONS 5


struct pt_mouse {
	SDL_bool buttonEvent;

	Sint32 x, y;
	Sint32 wheelX, wheelY;
	SDL_bool buttonDown[PT_MOUSE_NUM_BUTTONS];
	SDL_bool buttonUp[PT_MOUSE_NUM_BUTTONS];
	SDL_bool buttonHold[PT_MOUSE_NUM_BUTTONS];
};


PT_Mouse* PT_MouseCreate( ) {
	PT_Mouse* _this = (PT_Mouse*)malloc(sizeof(struct pt_mouse));
	if ( !_this )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_MouseCreate: Not enough memory\n");
		return NULL;
	}
	SDL_memset(_this, 0, sizeof(struct pt_mouse));
	
	return _this;
}//PT_MouseCreate

void PT_MouseDestroy( PT_Mouse* _this ) {
	if ( !_this )
	{
		return;
	}
	
	free(_this);
}//PT_MouseDestroy

void PT_MouseClearState( PT_Mouse* _this ) {
	_this->buttonEvent = SDL_FALSE;
	_this->wheelX = _this->wheelY = 0;
	SDL_memset(_this->buttonDown, SDL_FALSE, sizeof(SDL_bool) * PT_MOUSE_NUM_BUTTONS);
	SDL_memset(_this->buttonUp, SDL_FALSE, sizeof(SDL_bool) * PT_MOUSE_NUM_BUTTONS);
}//PT_MouseClearState

void PT_MouseUpdate( PT_Mouse* _this, const SDL_Event* ev ) {

	if ( ev->type == SDL_MOUSEMOTION )
	{
		_this->x = ev->motion.x;
		_this->y = ev->motion.y;
	}
	else if ( ev->type == SDL_MOUSEWHEEL )
	{
		_this->wheelX = ev->wheel.x;
		_this->wheelY = ev->wheel.y;
	}
	
	if ( ev->type == SDL_MOUSEBUTTONDOWN )
	{
		_this->buttonDown[ev->button.button] = SDL_TRUE;
		_this->buttonUp[ev->button.button] = SDL_FALSE;
		_this->buttonHold[ev->button.button] = SDL_TRUE;
		_this->buttonEvent = SDL_TRUE;
	}
	else if ( ev->type == SDL_MOUSEBUTTONUP )
	{
		_this->buttonDown[ev->button.button] = SDL_FALSE;
		_this->buttonUp[ev->button.button] = SDL_TRUE;
		_this->buttonHold[ev->button.button] = SDL_FALSE;
		_this->buttonEvent = SDL_TRUE;
	}
}//PT_MouseUpdate

void PT_MouseGetPosition( PT_Mouse* _this, Sint32* x, Sint32* y ) {
	*x = _this->x;
	*y = _this->y;
}

void PT_MouseGetWheelScroll( PT_Mouse* _this, Sint32* wheelX, Sint32* wheelY ) {
	*wheelX = _this->wheelX;
	*wheelY = _this->wheelY;
}

SDL_bool PT_MouseGetButtonDown( PT_Mouse* _this, Uint8 button ) {
	if ( button < 1 || button > PT_MOUSE_NUM_BUTTONS )
	{
		return SDL_FALSE;
	}
	return _this->buttonDown[button];
}
SDL_bool PT_MouseGetButtonUp( PT_Mouse* _this, Uint8 button ) {
	if ( button < 1 || button > PT_MOUSE_NUM_BUTTONS )
	{
		return SDL_FALSE;
	}
	return _this->buttonUp[button];
}
SDL_bool PT_MouseGetButtonHold( PT_Mouse* _this, Uint8 button ) {
	if ( button < 1 || button > PT_MOUSE_NUM_BUTTONS )
	{
		return SDL_FALSE;
	}
	return _this->buttonHold[button];
}

SDL_bool PT_MouseGetButtonEvent( PT_Mouse* _this ) {
	return _this->buttonEvent;
}//PT_MouseGetButtonEvent

Uint8 PT_MouseGetButtonByString( const char* string ) {

	if ( !string )
	{
		return 0;
	}
	
	if ( !strcmp(string, "left") )
	{
		return SDL_BUTTON_LEFT;
	}
	if ( !strcmp(string, "middle") )
	{
		return SDL_BUTTON_MIDDLE;
	}	
	if ( !strcmp(string, "right") )
	{
		return SDL_BUTTON_RIGHT;
	}
	if ( !strcmp(string, "x1") )
	{
		return SDL_BUTTON_X1;
	}
	if ( !strcmp(string, "x2") )
	{
		return SDL_BUTTON_X2;
	}

	return 0;
}//PT_MouseGetButtonByString



