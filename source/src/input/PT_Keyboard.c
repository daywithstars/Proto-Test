/*
Copyright 2021 daywithstars

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

#include <PT_Keyboard.h>
#include <PT_String.h>


struct pt_keyboard {
	SDL_bool keyDown[SDL_NUM_SCANCODES];
	SDL_bool keyUp[SDL_NUM_SCANCODES];
	SDL_bool keyHold[SDL_NUM_SCANCODES];
};




PT_Keyboard* PT_KeyboardCreate( ) {
	PT_Keyboard* _this = (PT_Keyboard*)malloc(sizeof(struct pt_keyboard));
	if ( !_this )
	{
		return NULL;
	}
	SDL_memset(_this->keyDown, SDL_FALSE, sizeof(SDL_bool) * SDL_NUM_SCANCODES);
	SDL_memset(_this->keyUp, SDL_FALSE, sizeof(SDL_bool) * SDL_NUM_SCANCODES);
	SDL_memset(_this->keyHold, SDL_FALSE, sizeof(SDL_bool) * SDL_NUM_SCANCODES);
		
	return _this;
}//PT_KeyboardCreate

void PT_KeyboardDestroy( PT_Keyboard* _this ) {
	if ( !_this )
	{
		return;
	}
	
	free(_this);
}//PT_KeyboardDestroy

void PT_KeyboardClearState( PT_Keyboard* _this ) {
	SDL_memset(_this->keyDown, SDL_FALSE, sizeof(SDL_bool) * SDL_NUM_SCANCODES);
	SDL_memset(_this->keyUp, SDL_FALSE, sizeof(SDL_bool) * SDL_NUM_SCANCODES);
}

void PT_KeyboardUpdate( PT_Keyboard* _this, const SDL_Event* ev ) {
	if ( ev->type == SDL_KEYDOWN )
	{
		if ( ev->key.repeat == 0 )
		{
			_this->keyDown[ev->key.keysym.scancode] = SDL_TRUE;
			_this->keyUp[ev->key.keysym.scancode] = SDL_FALSE;
		}
		_this->keyHold[ev->key.keysym.scancode] = SDL_TRUE;
	}
	else if ( ev->type == SDL_KEYUP )
	{
		_this->keyDown[ev->key.keysym.scancode] = SDL_FALSE;
		_this->keyUp[ev->key.keysym.scancode] = SDL_TRUE;
		_this->keyHold[ev->key.keysym.scancode] = SDL_FALSE;	
	}
}//PT_KeyboardUpdate

SDL_bool PT_KeyboardGetKeyDown( PT_Keyboard* _this, SDL_Scancode key ) {
	return _this->keyDown[key];
}
SDL_bool PT_KeyboardGetKeyUp( PT_Keyboard* _this, SDL_Scancode key ) {
	return _this->keyUp[key];
}
SDL_bool PT_KeyboardGetKeyHold( PT_Keyboard* _this, SDL_Scancode key ) {
	return _this->keyHold[key];
}

SDL_Scancode PT_KeyboardGetScancodeByString( const char* string ) {

	if ( !string )
	{
		return SDL_SCANCODE_UNKNOWN;
	}
	
	if ( PT_StringMatchFast(string, "a") )
	{
		return SDL_SCANCODE_A;
	}
	if ( PT_StringMatchFast(string, "b") )
	{
		return SDL_SCANCODE_B;
	}
	if ( PT_StringMatchFast(string, "c") )
	{
		return SDL_SCANCODE_C;
	}
	if ( PT_StringMatchFast(string, "d") )
	{
		return SDL_SCANCODE_D;
	}
	if ( PT_StringMatchFast(string, "e") )
	{
		return SDL_SCANCODE_E;
	}
	if ( PT_StringMatchFast(string, "f") )
	{
		return SDL_SCANCODE_F;
	}
	if ( PT_StringMatchFast(string, "g") )
	{
		return SDL_SCANCODE_G;
	}
	if ( PT_StringMatchFast(string, "h") )
	{
		return SDL_SCANCODE_H;
	}
	if ( PT_StringMatchFast(string, "i") )
	{
		return SDL_SCANCODE_I;
	}
	if ( PT_StringMatchFast(string, "j") )
	{
		return SDL_SCANCODE_J;
	}
	if ( PT_StringMatchFast(string, "k") )
	{
		return SDL_SCANCODE_K;
	}
	if ( PT_StringMatchFast(string, "l") )
	{
		return SDL_SCANCODE_L;
	}
	if ( PT_StringMatchFast(string, "M") )
	{
		return SDL_SCANCODE_M;
	}
	if ( PT_StringMatchFast(string, "n") )
	{
		return SDL_SCANCODE_N;
	}
	if ( PT_StringMatchFast(string, "o") )
	{
		return SDL_SCANCODE_O;
	}
	if ( PT_StringMatchFast(string, "p") )
	{
		return SDL_SCANCODE_P;
	}
	if ( PT_StringMatchFast(string, "q") )
	{
		return SDL_SCANCODE_Q;
	}
	if ( PT_StringMatchFast(string, "r") )
	{
		return SDL_SCANCODE_R;
	}
	if ( PT_StringMatchFast(string, "s") )
	{
		return SDL_SCANCODE_S;
	}
	if ( PT_StringMatchFast(string, "t") )
	{
		return SDL_SCANCODE_T;
	}
	if ( PT_StringMatchFast(string, "u") )
	{
		return SDL_SCANCODE_U;
	}
	if ( PT_StringMatchFast(string, "v") )
	{
		return SDL_SCANCODE_V;
	}
	if ( PT_StringMatchFast(string, "w") )
	{
		return SDL_SCANCODE_W;
	}
	if ( PT_StringMatchFast(string, "x") )
	{
		return SDL_SCANCODE_X;
	}
	if ( PT_StringMatchFast(string, "y") )
	{
		return SDL_SCANCODE_Y;
	}
	if ( PT_StringMatchFast(string, "z") )
	{
		return SDL_SCANCODE_Z;
	}
	
	if ( PT_StringMatchFast(string, "1") )
	{
		return SDL_SCANCODE_1;
	}
	if ( PT_StringMatchFast(string, "2") )
	{
		return SDL_SCANCODE_2;
	}
	if ( PT_StringMatchFast(string, "3") )
	{
		return SDL_SCANCODE_3;
	}
	if ( PT_StringMatchFast(string, "4") )
	{
		return SDL_SCANCODE_4;
	}
	if ( PT_StringMatchFast(string, "5") )
	{
		return SDL_SCANCODE_5;
	}
	if ( PT_StringMatchFast(string, "6") )
	{
		return SDL_SCANCODE_6;
	}
	if ( PT_StringMatchFast(string, "7") )
	{
		return SDL_SCANCODE_7;
	}
	if ( PT_StringMatchFast(string, "8") )
	{
		return SDL_SCANCODE_8;
	}
	if ( PT_StringMatchFast(string, "9") )
	{
		return SDL_SCANCODE_9;
	}
	if ( PT_StringMatchFast(string, "0") )
	{
		return SDL_SCANCODE_0;
	}
	
	
	if ( PT_StringMatchFast(string, "return") )
	{
		return SDL_SCANCODE_RETURN;
	}	
	if ( PT_StringMatchFast(string, "escape") )
	{
		return SDL_SCANCODE_ESCAPE;
	}
	if ( PT_StringMatchFast(string, "backspace") )
	{
		return SDL_SCANCODE_BACKSPACE;
	}	
	if ( PT_StringMatchFast(string, "tab") )
	{
		return SDL_SCANCODE_TAB;
	}
	if ( PT_StringMatchFast(string, "space") )
	{
		return SDL_SCANCODE_SPACE;
	}
	
	
	if ( PT_StringMatchFast(string, "minus") )
	{
		return SDL_SCANCODE_MINUS;
	}	
	if ( PT_StringMatchFast(string, "equals") )
	{
		return SDL_SCANCODE_EQUALS;
	}
	if ( PT_StringMatchFast(string, "leftbracket") )
	{
		return SDL_SCANCODE_LEFTBRACKET;
	}
	if ( PT_StringMatchFast(string, "rightbracket") )
	{
		return SDL_SCANCODE_RIGHTBRACKET;
	}
	if ( PT_StringMatchFast(string, "backslash") )
	{
		return SDL_SCANCODE_BACKSLASH;
	}
	if ( PT_StringMatchFast(string, "nonushash") )
	{
		return SDL_SCANCODE_NONUSHASH;
	}
	if ( PT_StringMatchFast(string, "semicolon") )
	{
		return SDL_SCANCODE_SEMICOLON;
	}
	if ( PT_StringMatchFast(string, "apostrophe") )
	{
		return SDL_SCANCODE_APOSTROPHE;
	}
	if ( PT_StringMatchFast(string, "grave") )
	{
		return SDL_SCANCODE_GRAVE;
	}
	if ( PT_StringMatchFast(string, "comma") )
	{
		return SDL_SCANCODE_COMMA;
	}
	if ( PT_StringMatchFast(string, "period") )
	{
		return SDL_SCANCODE_PERIOD;
	}
	if ( PT_StringMatchFast(string, "slash") )
	{
		return SDL_SCANCODE_SLASH;
	}
	

	if ( PT_StringMatchFast(string, "capslock") )
	{
		return SDL_SCANCODE_CAPSLOCK;
	}
	
	if ( PT_StringMatchFast(string, "f1") )
	{
		return SDL_SCANCODE_F1;
	}
	if ( PT_StringMatchFast(string, "f2") )
	{
		return SDL_SCANCODE_F2;
	}
	if ( PT_StringMatchFast(string, "f3") )
	{
		return SDL_SCANCODE_F3;
	}
	if ( PT_StringMatchFast(string, "f4") )
	{
		return SDL_SCANCODE_F4;
	}
	if ( PT_StringMatchFast(string, "f5") )
	{
		return SDL_SCANCODE_F5;
	}
	if ( PT_StringMatchFast(string, "f6") )
	{
		return SDL_SCANCODE_F6;
	}
	if ( PT_StringMatchFast(string, "f7") )
	{
		return SDL_SCANCODE_F7;
	}
	if ( PT_StringMatchFast(string, "f8") )
	{
		return SDL_SCANCODE_F8;
	}
	if ( PT_StringMatchFast(string, "f9") )
	{
		return SDL_SCANCODE_F9;
	}
	if ( PT_StringMatchFast(string, "f10") )
	{
		return SDL_SCANCODE_F10;
	}
	if ( PT_StringMatchFast(string, "f11") )
	{
		return SDL_SCANCODE_F11;
	}
	if ( PT_StringMatchFast(string, "f12") )
	{
		return SDL_SCANCODE_F12;
	}
	
	
	if ( PT_StringMatchFast(string, "printscreen") )
	{
		return SDL_SCANCODE_PRINTSCREEN;
	}
	if ( PT_StringMatchFast(string, "scrolllock") )
	{
		return SDL_SCANCODE_SCROLLLOCK;
	}
	if ( PT_StringMatchFast(string, "pause") )
	{
		return SDL_SCANCODE_PAUSE;
	}
	if ( PT_StringMatchFast(string, "insert") )
	{
		return SDL_SCANCODE_INSERT;
	}
	
	
	if ( PT_StringMatchFast(string, "home") )
	{
		return SDL_SCANCODE_HOME;
	}
	if ( PT_StringMatchFast(string, "pageup") )
	{
		return SDL_SCANCODE_PAGEUP;
	}
	if ( PT_StringMatchFast(string, "delete") )
	{
		return SDL_SCANCODE_DELETE;
	}
	if ( PT_StringMatchFast(string, "end") )
	{
		return SDL_SCANCODE_END;
	}
	if ( PT_StringMatchFast(string, "pagedown") )
	{
		return SDL_SCANCODE_PAGEDOWN;
	}
	if ( PT_StringMatchFast(string, "right") )
	{
		return SDL_SCANCODE_RIGHT;
	}
	if ( PT_StringMatchFast(string, "left") )
	{
		return SDL_SCANCODE_LEFT;
	}
	if ( PT_StringMatchFast(string, "down") )
	{
		return SDL_SCANCODE_DOWN;
	}
	if ( PT_StringMatchFast(string, "up") )
	{
		return SDL_SCANCODE_UP;
	}
	
	
	if ( PT_StringMatchFast(string, "numlockclear") )
	{
		return SDL_SCANCODE_NUMLOCKCLEAR;
	}	
	
	return SDL_SCANCODE_UNKNOWN;
}//PT_KeyboardGetScancodeByString



