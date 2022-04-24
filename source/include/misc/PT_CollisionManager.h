/*
Copyright 2022 daywithstars

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

#ifndef _PT_COLLISIONMANAGER_H_
#define _PT_COLLISIONMANAGER_H_

#include <SDL_stdinc.h>

#include <PT_CollisionHandler.h>


SDL_bool PT_CollisionManagerCreate( );
void PT_CollisionManagerDestroy( );

//This function add layers that are used to make containers with collision handlers.
SDL_bool PT_CollisionManagerAddHandler( const char* utf8_handlerName, SDL_bool cameraLimits );

void PT_CollisionManagerClearHandlers( );

/*
* This function return a pointer to a previous handler added, in order to update only the handler you need.
* Use this function right after add the handler, doing this on the initialization struct holder. 
*/
PT_CollisionHandler* PT_CollisionManagerGetHandler( const char* utf8_handlerName );

//This function is used to set the current handler right before load/create sprites.
void PT_CollisionManagerSetCurrentHandler( PT_CollisionHandler* handler );
void PT_CollisionManagerSetCurrentHandlerByName( const char* utf8_handlerName );

//This function is used inside the sprite parse to get the current right handler.
PT_CollisionHandler* PT_CollisionManagerGetCurrentHandler( );

#endif /* _PT_COLLISIONMANAGER_H_ */



