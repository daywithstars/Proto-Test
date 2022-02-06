/*
Copyright 2022 daywithstars

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/


#ifndef _PT_MOUSE_H_
#define _PT_MOUSE_H_

#include <SDL_stdinc.h>
#include <SDL_events.h>


typedef struct pt_mouse PT_Mouse;


PT_Mouse* PT_MouseCreate( );

void PT_MouseDestroy( PT_Mouse* _this );

void PT_MouseClearState( PT_Mouse* _this );

void PT_MouseUpdate( PT_Mouse* _this, const SDL_Event* ev );

void PT_MouseGetPosition( PT_Mouse* _this, Sint32* x, Sint32* y );
void PT_MouseGetWheelScroll( PT_Mouse* _this, Sint32* wheelX, Sint32* wheelY );

SDL_bool PT_MouseGetButtonDown( PT_Mouse* _this, Uint8 button );
SDL_bool PT_MouseGetButtonUp( PT_Mouse* _this, Uint8 button );
SDL_bool PT_MouseGetButtonHold( PT_Mouse* _this, Uint8 button );

SDL_bool PT_MouseGetButtonEvent( PT_Mouse* _this );

Uint8 PT_MouseGetButtonByString( const char* string );

#endif /* _PT_MOUSE_H_ */



