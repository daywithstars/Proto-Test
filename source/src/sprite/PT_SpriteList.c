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

#include <PT_SpriteList.h>


PT_SpriteList* PT_SpriteListCreate( PT_SpriteList* _this, const char* utf8_index, PT_Sprite* value ) {

	if ( !_this )
	{
		_this = (PT_SpriteList*)malloc(sizeof(PT_SpriteList));
		if ( !_this )
		{
			SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_SpriteListCreate: Not enough memory\n");
			return NULL;
		}
		SDL_memset(_this, 0, sizeof(PT_SpriteList));
		
		_this->index = PT_StringCreate();
		if ( !PT_StringInsert(&(_this->index), utf8_index, 0) )
		{
			SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_SpriteListCreate!\n");
			PT_SpriteListDestroy(_this, SDL_FALSE);
			return NULL;
		}
		
		_this->values = (PT_Sprite**)malloc(sizeof(PT_Sprite*));
		if ( !_this->values )
		{
			SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_SpriteListCreate: Not enough memory\n");
			PT_SpriteListDestroy(_this, SDL_FALSE);
			return NULL;
		}
		_this->numValues = 1;
		_this->values[0] = value;
	}
	else {	
	
		PT_Sprite** newValues = (PT_Sprite**)malloc(sizeof(PT_Sprite*) * (_this->numValues + 1));
		if ( !newValues )
		{
			SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_SpriteListCreate: Not enough memory\n");
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
}//PT_SpriteListCreate

void PT_SpriteListDestroy( PT_SpriteList* _this, SDL_bool freeSprites ) {
	if ( !_this )
	{
		return;
	}


	while ( _this )
	{
		PT_SpriteList* tmp = _this->next;
		
		if ( _this->values )
		{
			if ( freeSprites )
			{
				for ( unsigned int i = 0; i < _this->numValues; i++ )
				{
					PT_SpriteDestroy(_this->values[i]);
				}
			}
			free(_this->values);
		}
		
		PT_StringDestroy(_this->index);
		
		free(_this);
		_this = tmp;
	}
}//PT_SpriteListDestroy 

PT_SpriteList* PT_SpriteListAdd( PT_SpriteList* _this, const char* utf8_index, PT_Sprite* value, SDL_bool freeRepeated ) {
	
	if ( !_this )
	{
		return PT_SpriteListCreate(NULL, utf8_index, value);
	}
	
	PT_SpriteList* pList = _this;
	while ( pList )
	{
		if ( PT_StringMatch(pList->index, utf8_index) )
		{
			if ( value && freeRepeated )
			{
				PT_SpriteDestroy(value);
			}
			return _this;
		}
		pList = pList->next;
	}

	PT_SpriteList* newNode = PT_SpriteListCreate(NULL, utf8_index, value);
	newNode->next = _this;
	
	return newNode;
}//PT_SpriteListAdd

PT_SpriteList* PT_SpriteListCat( PT_SpriteList* _this, const char* utf8_index, PT_Sprite* value ) {
	if ( !_this )
	{
		return PT_SpriteListCreate(NULL, utf8_index, value);
	}
	
	PT_SpriteList* pList = _this;
	while ( pList )
	{
		if ( PT_StringMatch(pList->index, utf8_index) )
		{
			if ( value )
			{
				_this = PT_SpriteListCreate(_this, NULL, value);
				return _this;
			}
		}
		pList = pList->next;
	}
	
	PT_SpriteList* newNode = PT_SpriteListCreate(NULL, utf8_index, value);
	newNode->next = _this;
	
	return newNode;
}//PT_SpriteListCat

PT_SpriteList* PT_SpriteListGet( PT_SpriteList* _this, const char* utf8_index ) {
	while ( _this )
	{
		if ( PT_StringMatch(_this->index, utf8_index) )
		{
			return _this;
		}
		
		_this = _this->next;
	}
	
	return NULL;
}//PT_SpriteListGet






