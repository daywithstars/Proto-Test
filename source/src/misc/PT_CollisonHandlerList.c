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

#include <PT_CollisionHandlerList.h>


PT_CollisionHandlerList* PT_CollisionHandlerListCreate( const char* utf8_index, 
	PT_CollisionHandler* value ) {
	
	PT_CollisionHandlerList* _this = (PT_CollisionHandlerList*)malloc(sizeof(PT_CollisionHandlerList));
	
	if ( !_this )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, 
		"PT: PT_CollisionHandlerListCreate: Not enough memory\n");
		return NULL;
	}
	
	_this->value = value;
	_this->index = PT_StringCreate();
	if ( !PT_StringInsert(&(_this->index), utf8_index, 0) )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_CollisionHandlerListCreate!\n");
	}
	_this->next = NULL;
	
	return _this;
}//PT_CollisionHandlerListCreate

void PT_CollisionHandlerListDestroy( PT_CollisionHandlerList* _this ) {
	if ( !_this )
	{
		return;
	}


	while ( _this )
	{
		PT_CollisionHandlerList* tmp = _this->next;
		
		if ( _this->value )
		{
			PT_CollisionHandlerDestroy(_this->value);
		}
		PT_StringDestroy(_this->index);
		
		free(_this);
		_this = tmp;
	}
}//PT_CollisionHandlerListDestroy

PT_CollisionHandlerList* PT_CollisionHandlerListAdd( PT_CollisionHandlerList* _this, 
	const char* utf8_index, PT_CollisionHandler* value ) {
	
	if ( !_this )
	{
		return PT_CollisionHandlerListCreate(utf8_index, value);
	}


	PT_CollisionHandlerList* pList = _this;
	
	while ( pList )
	{
		if ( PT_StringMatch(pList->index, utf8_index) )
		{
			if ( value )
			{
				PT_CollisionHandlerDestroy(value);
			}
			return _this;
		}
		pList = pList->next;
	}

	PT_CollisionHandlerList* newNode = PT_CollisionHandlerListCreate(utf8_index, value);
	newNode->next = _this;
	
	return newNode;
}//PT_CollisionHandlerListAdd

PT_CollisionHandlerList* PT_CollisionHandlerListGet( PT_CollisionHandlerList* _this, 
	const char* utf8_index ) {
	while ( _this )
	{
		if ( PT_StringMatch(_this->index, utf8_index) )
		{
			return _this;
		}
		
		_this = _this->next;
	}
	
	return NULL;
}//PT_CollisionHandlerListGet



