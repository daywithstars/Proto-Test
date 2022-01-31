/*
Copyright 2022 daywithstars

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

#include <SDL_stdinc.h>
#include <SDL_log.h>

#include <PT_Collider.h>
#include <PT_Graphics.h>



PT_Collider PT_ColliderCreate() {
	PT_Collider _this;
	SDL_memset(&_this, 0, sizeof(PT_Collider));
	
	return _this;
}//PT_ColliderCreate

void PT_ColliderDestroy( PT_Collider* _this ) {
	if ( !_this )
	{
		return;
	}
	
	PT_StringDestroy(_this->name);
	_this->name = NULL;
	
	switch ( _this->type )
	{
		case PT_COLLIDER_TYPE_RECTANGLE:	
		free(_this->rect);
		_this->rect = NULL;
		break;
	}
}//PT_ColliderDestroy

SDL_bool PT_ColliderSetName( PT_Collider* _this, const char* utf8_name ) {
	
	if ( !_this->name )
	{
		_this->name = PT_StringCreate();
		if ( !_this->name )
		{
			return SDL_FALSE;
		}
	}
	
	PT_StringClear(&_this->name);
	if ( !PT_StringInsert(&_this->name, utf8_name, 0) )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_ColliderSetName!\n");
		return SDL_FALSE;
	}
	
	return SDL_TRUE;
}//PT_ColliderSetName

void PT_ColliderSetVisible( PT_Collider* _this, SDL_bool value ) {
	_this->visible = value;
}//PT_ColliderSetVisible

void PT_ColliderSetColor( PT_Collider* _this, const SDL_Color color ) {
	_this->color = color;
}//PT_ColliderSetColor

SDL_bool PT_ColliderSetRect( PT_Collider* _this, float x, float y, float w, float h ) {
	if ( _this->rect )
	{
		free(_this->rect);
		_this->rect = NULL;
	}	
	
	_this->rect = (SDL_FRect*)malloc(sizeof(SDL_FRect));
	if ( !_this->rect )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_ColliderSetRect: Not enough memory!\n");
		return SDL_FALSE;
	}
	_this->type = PT_COLLIDER_TYPE_RECTANGLE;

	_this->rect->x = x;
	_this->rect->y = y;
	_this->rect->w = w;
	_this->rect->h = h;
	
	return SDL_TRUE;
}//PT_ColliderSetRect

SDL_bool PT_ColliderTestCollision( PT_Collider _this, float _thisRelativeX, float _thisRelativeY, PT_Collider other, float otherRelativeX, float otherRelativeY ) {
	if ( _this.type == PT_COLLIDER_TYPE_RECTANGLE )
	{
		if ( other.type == PT_COLLIDER_TYPE_RECTANGLE )
		{
			return !( (_this.rect->x + _thisRelativeX) + _this.rect->w < (other.rect->x + otherRelativeX) 								||
				(_this.rect->x + _thisRelativeX) > (other.rect->x + otherRelativeX) + other.rect->w 
							||
				(_this.rect->y + _thisRelativeY) + _this.rect->h < (other.rect->y + otherRelativeY)
							||
				(_this.rect->y + _thisRelativeY) > (other.rect->y + otherRelativeY) + other.rect->h 
			 );
		}
	}
	
	return SDL_FALSE;
}//PT_ColliderTestCollision

void PT_ColliderDraw( PT_Collider _this, float relativeX, float relativeY ) {
	if ( !_this.visible )
	{
		return;
	}
	
	if ( _this.type == PT_COLLIDER_TYPE_RECTANGLE )
	{
		PT_GraphicsSetRenderDrawColor(_this.color);
		
		const SDL_FRect rect = { _this.rect->x + relativeX, _this.rect->y + relativeY, 
			_this.rect->w, _this.rect->h };
			
		PT_GraphicsRenderFillRectF(&rect);
	}
}//PT_ColliderDraw







