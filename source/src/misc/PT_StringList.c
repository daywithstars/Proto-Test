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

#include <PT_StringList.h>


PT_StringList* PT_StringListCreate( PT_StringList* _this, const char* utf8_index, PT_String* value ) {

	if ( !_this )
	{
		_this = (PT_StringList*)malloc(sizeof(PT_StringList));
		if ( !_this )
		{
			SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_StringListCreate: Not enough memory\n");
			return NULL;
		}
		SDL_memset(_this, 0, sizeof(PT_StringList));
		
		_this->index = PT_StringCreate();
		if ( !PT_StringInsert(&(_this->index), utf8_index, 0) )
		{
			SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_StringListCreate!\n");
			PT_StringListDestroy(_this);
			return NULL;
		}
		
		_this->values = (PT_String**)malloc(sizeof(PT_String*));
		if ( !_this->values )
		{
			SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_StringListCreate: Not enough memory\n");
			PT_StringListDestroy(_this);
			return NULL;
		}
		_this->numValues = 1;
		_this->values[0] = value;
	}
	else {	
	
		PT_String** newValues = (PT_String**)malloc(sizeof(PT_String*) * (_this->numValues + 1));
		if ( !newValues )
		{
			SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_StringListCreate: Not enough memory\n");
			return _this;
		}
		
		unsigned int i = 0;
		while ( i < _this->numValues )
		{
			newValues[i] = _this->values[i];
			i ++;
		}
		_this->numValues ++;
		
		newValues[i] = value;
		
		free(_this->values);
		_this->values = newValues;
	}
	
	return _this;
}//PT_StringListCreate

void PT_StringListDestroy( PT_StringList* _this ) {
	if ( !_this )
	{
		return;
	}


	while ( _this )
	{
		PT_StringList* tmp = _this->next;
		
		for ( unsigned int i = 0; i < _this->numValues; i++ )
		{
			PT_StringDestroy(_this->values[i]);
		}
		free(_this->values);
		
		PT_StringDestroy(_this->index);
		
		free(_this);
		_this = tmp;
	}
}//PT_StringListDestroy

PT_StringList* PT_StringListAdd( PT_StringList* _this, const char* utf8_index, PT_String* value ) {
	
	if ( !_this )
	{
		return PT_StringListCreate(NULL, utf8_index, value);
	}
	
	PT_StringList* pList = _this;
	while ( pList )
	{
		if ( PT_StringMatch(pList->index, utf8_index) )
		{
			if ( value )
			{
				PT_StringDestroy(value);
			}
			return _this;
		}
		pList = pList->next;
	}

	PT_StringList* newNode = PT_StringListCreate(NULL, utf8_index, value);
	newNode->next = _this;
	
	return newNode;
}//PT_StringListAdd

PT_StringList* PT_StringListCat( PT_StringList* _this, const char* utf8_index, PT_String* value ) {
	if ( !_this )
	{
		return PT_StringListCreate(NULL, utf8_index, value);
	}
	
	PT_StringList* pList = _this;
	while ( pList )
	{
		if ( PT_StringMatch(pList->index, utf8_index) )
		{
			if ( value )
			{
				_this = PT_StringListCreate(_this, NULL, value);
				return _this;
			}
		}
		pList = pList->next;
	}
	
	PT_StringList* newNode = PT_StringListCreate(NULL, utf8_index, value);
	newNode->next = _this;
	
	return newNode;
}//PT_StringListCat

PT_StringList* PT_StringListGet( PT_StringList* _this, const char* utf8_index ) {
	while ( _this )
	{
		if ( PT_StringMatch(_this->index, utf8_index) )
		{
			return _this;
		}
		
		_this = _this->next;
	}
	
	return NULL;
}//PT_StringListGet



