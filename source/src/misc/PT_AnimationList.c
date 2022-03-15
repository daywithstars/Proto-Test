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

#include <PT_AnimationList.h>


PT_AnimationList* PT_AnimationListCreate( const char* utf8_index, PT_Animation value ) {
	PT_AnimationList* _this = (PT_AnimationList*)malloc(sizeof(PT_AnimationList));
	
	if ( !_this )
	{
		SDL_LogMessage(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_CRITICAL,
		"PT: PT_AnimationListCreate: Not enough memory\n");
		SDL_LogMessage(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_CRITICAL,
		"PT: PT_AnimationListCreate: FILE %s, LINE %d\n", __FILE__, __LINE__);
		
		return NULL;
	}
	
	_this->value = value;
	_this->index = PT_StringCreate();
	if ( !PT_StringInsert(&(_this->index), utf8_index, 0) )
	{
		SDL_LogMessage(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_WARN,
		"PT: PT_AnimationListCreate!\n");
		SDL_LogMessage(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_WARN,
		"PT: PT_AnimationListCreate: FILE %s, LINE %d\n", __FILE__, __LINE__);
	}
	_this->next = NULL;
	
	return _this;
}//PT_AnimationListCreate

void PT_AnimationListDestroy( PT_AnimationList* _this ) {
	if ( !_this )
	{
		return;
	}


	while ( _this )
	{
		PT_AnimationList* tmp = _this->next;
		
		PT_AnimationDestroy(&_this->value);
		
		PT_StringDestroy(_this->index);
		
		free(_this);
		_this = tmp;
	}
}//PT_AnimationListDestroy

PT_AnimationList* PT_AnimationListAdd( PT_AnimationList* _this, const char* utf8_index, 
	PT_Animation value ) {
	
	if ( !_this )
	{
		return PT_AnimationListCreate(utf8_index, value);
	}
	
	PT_AnimationList* pList = _this;
	while ( pList )
	{
		if ( PT_StringMatch(pList->index, utf8_index) )
		{
			return _this;
		}
		pList = pList->next;
	}

	PT_AnimationList* newNode = PT_AnimationListCreate(utf8_index, value);
	newNode->next = _this;
	
	return newNode;
}//PT_AnimationListAdd

PT_AnimationList* PT_AnimationListGet( PT_AnimationList* _this, const char* utf8_index ) {
	while ( _this )
	{
		if ( PT_StringMatch(_this->index, utf8_index) )
		{
			return _this;
		}
		
		_this = _this->next;
	}
	
	return NULL;
}//PT_AnimationListGet



