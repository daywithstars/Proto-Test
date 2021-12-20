/*
Copyright 2021 daywithstars

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

#ifndef _PT_BEHAVIOR_STATE_LIST_H_
#define _PT_BEHAVIOR_STATE_LIST_H_


#include <PT_String.h>

#include <PT_BehaviorState.h>

typedef struct pt_behavior_state_list {
	PT_String* index;
	PT_BehaviorState* value;
	
	struct pt_behavior_state_list* next;
}PT_BehaviorStateList;




PT_BehaviorStateList* PT_BehaviorStateListCreate( const char* utf8_index, PT_BehaviorState* value );

void PT_BehaviorStateListDestroy( PT_BehaviorStateList* _this );

PT_BehaviorStateList* PT_BehaviorStateListAdd( PT_BehaviorStateList* _this, const char* utf8_index,
	PT_BehaviorState* value );
PT_BehaviorStateList* PT_BehaviorStateListGet( PT_BehaviorStateList* _this, const char* utf8_index );


#endif /* _PT_BEHAVIOR_STATE_LIST_H_ */



