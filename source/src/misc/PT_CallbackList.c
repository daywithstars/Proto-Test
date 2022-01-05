/*
Copyright 2021 daywithstars

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

#include <SDL_log.h>

#include <PT_CallbackList.h>


PT_CallbackList* PT_CallbackListCreate( const char* utf8_index ) {
	PT_CallbackList* _this = (PT_CallbackList*)malloc(sizeof(PT_CallbackList));
	
	if ( !_this )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_CallbackListCreate: Not enough memory\n");
		return NULL;
	}
	SDL_memset(_this, 0, sizeof(PT_CallbackList));

	_this->index = PT_StringCreate();
	if ( !PT_StringInsert(&(_this->index), utf8_index, 0) )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_CallbackListCreate!\n");
	}
	_this->next = NULL;
	
	return _this;
}//PT_CallbackListCreate

void PT_CallbackListDestroy( PT_CallbackList* _this ) {
	if ( !_this )
	{
		return;
	}


	while ( _this )
	{
		PT_CallbackList* tmp = _this->next;
		
		PT_StringDestroy(_this->index);
		
		free(_this);
		_this = tmp;
	}
}//PT_CallbackListDestroy

PT_CallbackList* PT_CallbackListAddSimple( PT_CallbackList* _this, const char* utf8_index, 
	void (*callback)(void* _data) ) {
	
	if ( !_this )
	{
		PT_CallbackList* head = PT_CallbackListCreate(utf8_index);
		head->simpleCallback = callback;
		return head;
	}
	
	PT_CallbackList* pList = _this;
	while ( pList )
	{
		if ( PT_StringMatch(pList->index, utf8_index) )
		{
			return _this;
		}
		pList = pList->next;
	}

	PT_CallbackList* newNode = PT_CallbackListCreate(utf8_index);
	newNode->simpleCallback = callback;
	newNode->next = _this;
	
	return newNode;
}//PT_CallbackListAddSimple

PT_CallbackList* PT_CallbackListAddString( PT_CallbackList* _this, const char* utf8_index, 
	SDL_bool (*callback)(void* _data, const char* utf8_string) ) {
	
	if ( !_this )
	{
		PT_CallbackList* head = PT_CallbackListCreate(utf8_index);
		head->stringCallback = callback;
		return head;
	}
	
	PT_CallbackList* pList = _this;
	while ( pList )
	{
		if ( PT_StringMatch(pList->index, utf8_index) )
		{
			return _this;
		}
		pList = pList->next;
	}

	PT_CallbackList* newNode = PT_CallbackListCreate(utf8_index);
	newNode->stringCallback = callback;
	newNode->next = _this;
	
	return newNode;
}//PT_CallbackListAddString

PT_CallbackList* PT_CallbackListAddSDL_FPoint( PT_CallbackList* _this, const char* utf8_index,
	void (*callback)(void* _data, SDL_FPoint) ) {
	
	if ( !_this )
	{
		PT_CallbackList* head = PT_CallbackListCreate(utf8_index);
		head->SDL_FPointCallback = callback;
		return head;
	}
	
	PT_CallbackList* pList = _this;
	while ( pList )
	{
		if ( PT_StringMatch(pList->index, utf8_index) )
		{
			return _this;
		}
		pList = pList->next;
	}

	PT_CallbackList* newNode = PT_CallbackListCreate(utf8_index);
	newNode->SDL_FPointCallback = callback;
	newNode->next = _this;
	
	return newNode;
}//PT_CallbackListAddSDL_FPoint

PT_CallbackList* PT_CallbackListGet( PT_CallbackList* _this, const char* utf8_index ) {
	while ( _this )
	{
		if ( PT_StringMatch(_this->index, utf8_index) )
		{
			return _this;
		}
		
		_this = _this->next;
	}
	
	return NULL;
}//PT_CallbackListGet



