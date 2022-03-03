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

#include <PT_StringStack.h>



PT_StringStack* PT_StringStackPush( PT_StringStack* _this, PT_String* value ) {

	PT_StringStack* newStack = (PT_StringStack*)malloc(sizeof(PT_StringStack));
	if ( !newStack )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_StringStackPush: Not enough memory\n");
		return NULL;
	}
	SDL_memset(newStack, 0, sizeof(PT_StringStack));
	newStack->pValue = value;

	if ( _this )
	{
		newStack->next = _this;
	}
	
	return newStack;
}//PT_StringStackPush

void PT_StringStackDestroy( PT_StringStack** _this ) {
	if ( !_this || !(*_this) )
	{
		return;
	}
	
	while ( (*_this) )
	{
		(*_this) = PT_StringStackPop((*_this));
	}
	(*_this) = NULL;
}//PT_StringStackDestroy

PT_StringStack* PT_StringStackPop( PT_StringStack* _this ) {
	if ( PT_StringStackIsEmpty(_this) )
	{
		return NULL;
	}
	
	PT_StringStack* next = _this->next;
	free(_this);

	return next;
}//PT_StringStackPop

SDL_bool PT_StringStackIsEmpty( PT_StringStack* _this ) {
	if ( !_this )
	{
		return SDL_TRUE;
	} 
	
	return SDL_FALSE;
}//PT_StringStackIsEmpty




