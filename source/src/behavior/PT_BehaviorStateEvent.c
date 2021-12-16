/*
Copyright 2021 daywithstars

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

#include <PT_BehaviorStateEvent.h>



PT_BehaviorStateEventQueue* PT_BehaviorStateEventQueueCreate( unsigned int capacity, 
	PT_BehaviorStateEvent** arrayValue ) {

	PT_BehaviorStateEventQueue* _this = 
	(PT_BehaviorStateEventQueue*)malloc(sizeof(PT_BehaviorStateEventQueue));
	
	if ( !_this )
	{
		return NULL;
	}
	
	_this->front = 0;
	_this->rear = capacity - 1;
	_this->capacity = capacity;
	_this->length = 0;
	_this->arrayValue = arrayValue;
	
	return _this;
}//PT_BehaviorStateEventQueueCreate

void PT_BehaviorStateEventQueueDestroy( PT_BehaviorStateEventQueue* _this ) {
	if ( !_this )
	{
		return;
	}
	
	free(_this);
}//PT_BehaviorStateEventQueueCreateDestroy

SDL_bool PT_BehaviorStateEventQueueIsFull( PT_BehaviorStateEventQueue* _this ) {
	return ( _this->length == _this->capacity );
}
SDL_bool PT_BehaviorStateEventQueueIsEmpty( PT_BehaviorStateEventQueue* _this ) {
	return ( _this->length == 0 );
}

void PT_BehaviorStateEventQueueEnQueue( PT_BehaviorStateEventQueue* _this, PT_BehaviorStateEvent* value ) {
	
	if ( PT_BehaviorStateEventQueueIsFull(_this) )
	{
		return;
	}
	
	_this->rear = ( _this->rear + 1 ) % _this->capacity;
	_this->arrayValue[_this->rear] = value;
	_this->length ++;
}//PT_BehaviorStateEventQueueEnQueue

PT_BehaviorStateEvent* PT_BehaviorStateEventQueueDeQueue( PT_BehaviorStateEventQueue* _this ) {
	if ( PT_BehaviorStateEventQueueIsEmpty(_this) )
	{
		return NULL;
	}
	
	PT_BehaviorStateEvent* value = _this->arrayValue[_this->front];
	_this->front = ( _this->front + 1 ) % _this->capacity;
	_this->length --;
	
	return value;
}//PT_BehaviorStateEventQueueDeQueue







