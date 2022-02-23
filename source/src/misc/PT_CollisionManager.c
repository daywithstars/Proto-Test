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

#include <PT_CollisionManager.h>
#include <PT_CollisionHandlerList.h>


typedef struct {	
	PT_CollisionHandlerList* handlerList;
	PT_CollisionHandler* currentHandler;
}PT_CollisionManager;


static PT_CollisionManager* ptCollisionManager = NULL;






//===================================== PUBLIC Functions


SDL_bool PT_CollisionManagerCreate( ) {
	if ( ptCollisionManager )
	{
		return SDL_TRUE;
	}
	ptCollisionManager = (PT_CollisionManager*)malloc(sizeof(PT_CollisionManager));
	if ( !ptCollisionManager )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_CollisionManagerCreate: Not enough memory!\n");
		return SDL_FALSE;
	}
	SDL_memset(ptCollisionManager, 0, sizeof(PT_CollisionManager));

	return SDL_TRUE;
}//PT_CollisionManagerCreate

void PT_CollisionManagerDestroy( ) {
	if ( !ptCollisionManager )
	{
		return;
	}
	
	PT_CollisionHandlerListDestroy(ptCollisionManager->handlerList);
		

	free(ptCollisionManager);
	ptCollisionManager = NULL;
}//PT_CollisionManagerDestroy

SDL_bool PT_CollisionManagerAddHandler( const char* utf8_handlerName, SDL_bool cameraLimits ) {
	PT_CollisionHandler* handler = PT_CollisionHandlerCreate(cameraLimits);
	if ( !handler )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION,"PT: PT_CollisionManagerAddHandler!\n");
		return SDL_FALSE;
	}
	
	ptCollisionManager->handlerList = 
	PT_CollisionHandlerListAdd(ptCollisionManager->handlerList, utf8_handlerName, handler);
	return SDL_TRUE;
}//PT_CollisionManagerAddHandler

PT_CollisionHandler* PT_CollisionManagerGetHandler( const char* utf8_handlerName ) {
	PT_CollisionHandlerList* node = 
	PT_CollisionHandlerListGet(ptCollisionManager->handlerList, utf8_handlerName);
	
	if ( !node )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, 
		"PT: PT_CollisionManagerGetHandler: Cannot find: %s CollisionHandler\n", utf8_handlerName);
		return NULL;
	}
	
	return node->value;
}//PT_CollisionManagerGetHandler

void PT_CollisionManagerSetCurrentHandler( PT_CollisionHandler* handler ) {
	ptCollisionManager->currentHandler = handler;
}
PT_CollisionHandler* PT_CollisionManagerGetCurrentHandler( ) {	
	return ptCollisionManager->currentHandler;
}
void PT_CollisionManagerSetCurrentHandlerByName( const char* utf8_handlerName ) {
	PT_CollisionHandler* handler = PT_CollisionManagerGetHandler(utf8_handlerName);
	if ( handler )
	{
		ptCollisionManager->currentHandler = handler;
	}
}



