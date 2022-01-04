/*
Copyright 2022 daywithstars

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

#ifndef _PT_ANIMATION_H_
#define _PT_ANIMATION_H_

#include <SDL_stdinc.h>
#include <SDL_rect.h>


typedef struct pt_animation {
	int id;
	SDL_bool stop;

	Uint16 frameDelay;
	Uint16 frameDelayCount;
	
	Uint16 frameWidth;
	Uint16 frameHeight;
	
	Sint16 frameColumn;
	Sint8 frameColumnDir;
	Uint16 frameColumnMax;
	
	Sint16 frameRow;
	Sint8 frameRowDir;
	Uint16 frameRowMax;
}PT_Animation;



PT_Animation PT_AnimationCreate( 
	Uint16 frameDelay, Uint16 frameWidth, Uint16 frameHeight, 
	Sint16 frameColumn, Sint8 frameColumnDir, Uint16 frameColumnMax, 
	Sint16 frameRow, Sint8 frameRowDir, Uint16 frameRowMax );
void PT_AnimationDestroy( PT_Animation* _this );

void PT_AnimationUpdate( PT_Animation* _this, SDL_Rect* src, Sint32 elapsedTime );

void PT_AnimationStop( PT_Animation* _this, SDL_bool value );

#endif /* _PT_ANIMATION_H_ */



