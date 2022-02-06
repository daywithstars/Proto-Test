/*
Copyright 2022 daywithstars

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

#ifndef _PT_BEHAVIOR_H_
#define _PT_BEHAVIOR_H_

#include <SDL.h>


#include <PT_InputHandler.h>
#include <PT_CallbackList.h>
#include <PT_BehaviorStateList.h>


typedef struct pt_behavior {
	PT_InputHandler* inputHandler;
	PT_CallbackList* callbackList;
	PT_BehaviorStateList* behaviorStateList;
	PT_BehaviorState* currentState;
	
	void* pSprite;
}PT_Behavior;


PT_Behavior* PT_BehaviorCreate( void* pSprite, const char* utf8_behaviorTemplate );
void PT_BehaviorDestroy( PT_Behavior* _this );

void PT_BehaviorAddSimpleCallback( PT_Behavior* _this, const char* utf8_callbackName, 
	void (*callback)(void* _data) );
void PT_BehaviorAddStringCallback( PT_Behavior* _this, const char* utf8_callbackName, 
	SDL_bool (*callback)(void* _data, const char* utf8_string) );
void PT_BehaviorAddSDL_FPointCallback( PT_Behavior* _this, const char* utf8_callbackName, 
	void (*callback)(void* _data, SDL_FPoint) );

void PT_BehaviorUpdate( PT_Behavior* _this, void* target, Sint32 elapsedTime );

void PT_BehaviorChangeState( PT_Behavior* _this, const char* utf8_stateName );

#endif /* _PT_BEHAVIOR_H_ */



