/*
Copyright 2022 daywithstars

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

#include <stdio.h>
#include <stdlib.h>

#include <PT_Animation.h>



PT_Animation PT_AnimationCreate( Uint16 frameDelay, Uint16 frameWidth, Uint16 frameHeight, 
	Uint16 frameColumnMax ) {
	
	PT_Animation _this = { SDL_FALSE, frameDelay, 0, frameWidth, frameHeight, 0, frameColumnMax };
	
	return _this;
}//PT_AnimationCreate

void PT_AnimationDestroy( PT_Animation* _this ) {
	if ( !_this )
	{
		return;
	}
	
	_this->frameDelayCount = 0;
}//PT_AnimationDestroy

void PT_AnimationUpdate( PT_Animation* _this, SDL_Rect* src, Sint32 elapsedTime ) {
	if ( _this->stop || !src )
	{
		return;
	}
	
	if ( (_this->frameDelayCount += elapsedTime) >= _this->frameDelay )
	{
		_this->frameDelayCount = 0;
		
		if ( ++_this->frameColumn >= _this->frameColumnMax )
		{
			_this->frameColumn = 0;
		}
	}
	
	src->x = _this->frameColumn * _this->frameWidth;
	src->y = 0;
	src->w = _this->frameWidth;
	src->h = _this->frameHeight;
}//PT_AnimationUpdate

void PT_AnimationStop( PT_Animation* _this, SDL_bool value ) {
	_this->stop = value;
}//PT_AnimationStop




