/*
Copyright 2022 daywithstars

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
	SDL_bool keyEvent;

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
	_this->keyEvent = SDL_FALSE;
	
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
	_this->keyEvent = SDL_FALSE;
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
		_this->keyEvent = SDL_TRUE;
	}
	else if ( ev->type == SDL_KEYUP )
	{
		_this->keyDown[ev->key.keysym.scancode] = SDL_FALSE;
		_this->keyUp[ev->key.keysym.scancode] = SDL_TRUE;
		_this->keyHold[ev->key.keysym.scancode] = SDL_FALSE;	
		_this->keyEvent = SDL_TRUE;
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

SDL_bool PT_KeyboardGetKeyEvent( PT_Keyboard* _this ) {
	return _this->keyEvent;
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
	
	
	if ( PT_StringMatchFast(string, "kpdivide") )
	{
		return SDL_SCANCODE_KP_DIVIDE;
	}
	if ( PT_StringMatchFast(string, "kpmultiply") )
	{
		return SDL_SCANCODE_KP_MULTIPLY;
	}
	if ( PT_StringMatchFast(string, "kpminus") )
	{
		return SDL_SCANCODE_KP_MINUS;
	}
	if ( PT_StringMatchFast(string, "kpplus") )
	{
		return SDL_SCANCODE_KP_PLUS;
	}
	if ( PT_StringMatchFast(string, "kpenter") )
	{
		return SDL_SCANCODE_KP_ENTER;
	}
	if ( PT_StringMatchFast(string, "kp1") )
	{
		return SDL_SCANCODE_KP_1;
	}
	if ( PT_StringMatchFast(string, "kp2") )
	{
		return SDL_SCANCODE_KP_2;
	}
	if ( PT_StringMatchFast(string, "kp3") )
	{
		return SDL_SCANCODE_KP_3;
	}
	if ( PT_StringMatchFast(string, "kp4") )
	{
		return SDL_SCANCODE_KP_4;
	}
	if ( PT_StringMatchFast(string, "kp5") )
	{
		return SDL_SCANCODE_KP_5;
	}
	if ( PT_StringMatchFast(string, "kp6") )
	{
		return SDL_SCANCODE_KP_6;
	}
	if ( PT_StringMatchFast(string, "kp7") )
	{
		return SDL_SCANCODE_KP_7;
	}
	if ( PT_StringMatchFast(string, "kp8") )
	{
		return SDL_SCANCODE_KP_8;
	}
	if ( PT_StringMatchFast(string, "kp9") )
	{
		return SDL_SCANCODE_KP_9;
	}	
	if ( PT_StringMatchFast(string, "kp0") )
	{
		return SDL_SCANCODE_KP_0;
	}
	if ( PT_StringMatchFast(string, "kpperiod") )
	{
		return SDL_SCANCODE_KP_PERIOD;
	}
	if ( PT_StringMatchFast(string, "nonusbackslash") )
	{
		return SDL_SCANCODE_NONUSBACKSLASH;
	}
	if ( PT_StringMatchFast(string, "application") )
	{
		return SDL_SCANCODE_APPLICATION;
	}
	if ( PT_StringMatchFast(string, "power") )
	{
		return SDL_SCANCODE_POWER;
	}
	if ( PT_StringMatchFast(string, "kpequals") )
	{
		return SDL_SCANCODE_KP_EQUALS;
	}
	
	
	if ( PT_StringMatchFast(string, "f13") )
	{
		return SDL_SCANCODE_F13;
	}
	if ( PT_StringMatchFast(string, "f14") )
	{
		return SDL_SCANCODE_F14;
	}
	if ( PT_StringMatchFast(string, "f15") )
	{
		return SDL_SCANCODE_F15;
	}
	if ( PT_StringMatchFast(string, "f16") )
	{
		return SDL_SCANCODE_F16;
	}
	if ( PT_StringMatchFast(string, "f17") )
	{
		return SDL_SCANCODE_F17;
	}
	if ( PT_StringMatchFast(string, "f18") )
	{
		return SDL_SCANCODE_F18;
	}
	if ( PT_StringMatchFast(string, "f19") )
	{
		return SDL_SCANCODE_F19;
	}
	if ( PT_StringMatchFast(string, "f20") )
	{
		return SDL_SCANCODE_F20;
	}
	if ( PT_StringMatchFast(string, "f21") )
	{
		return SDL_SCANCODE_F21;
	}
	if ( PT_StringMatchFast(string, "f22") )
	{
		return SDL_SCANCODE_F22;
	}
	if ( PT_StringMatchFast(string, "f23") )
	{
		return SDL_SCANCODE_F23;
	}
	if ( PT_StringMatchFast(string, "f24") )
	{
		return SDL_SCANCODE_F24;
	}
	if ( PT_StringMatchFast(string, "execute") )
	{
		return SDL_SCANCODE_EXECUTE;
	}
	if ( PT_StringMatchFast(string, "help") )
	{
		return SDL_SCANCODE_HELP;
	}
	if ( PT_StringMatchFast(string, "menu") )
	{
		return SDL_SCANCODE_MENU;
	}
	if ( PT_StringMatchFast(string, "select") )
	{
		return SDL_SCANCODE_SELECT;
	}
	if ( PT_StringMatchFast(string, "stop") )
	{
		return SDL_SCANCODE_STOP;
	}
	if ( PT_StringMatchFast(string, "again") )
	{
		return SDL_SCANCODE_AGAIN;
	}
	if ( PT_StringMatchFast(string, "undo") )
	{
		return SDL_SCANCODE_UNDO;
	}
	if ( PT_StringMatchFast(string, "cut") )
	{
		return SDL_SCANCODE_CUT;
	}
	if ( PT_StringMatchFast(string, "copy") )
	{
		return SDL_SCANCODE_COPY;
	}
	if ( PT_StringMatchFast(string, "paste") )
	{
		return SDL_SCANCODE_PASTE;
	}
	if ( PT_StringMatchFast(string, "find") )
	{
		return SDL_SCANCODE_FIND;
	}
	if ( PT_StringMatchFast(string, "mute") )
	{
		return SDL_SCANCODE_MUTE;
	}
	if ( PT_StringMatchFast(string, "volumeup") )
	{
		return SDL_SCANCODE_VOLUMEUP;
	}
	if ( PT_StringMatchFast(string, "volumedown") )
	{
		return SDL_SCANCODE_VOLUMEDOWN;
	}
	
	if ( PT_StringMatchFast(string, "kpcomma") )
	{
		return SDL_SCANCODE_KP_COMMA;
	}
	if ( PT_StringMatchFast(string, "kpequalsas400") )
	{
		return SDL_SCANCODE_KP_EQUALSAS400;
	}
	
	return SDL_SCANCODE_UNKNOWN;
}//PT_KeyboardGetScancodeByString











