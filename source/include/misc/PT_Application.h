/*
Copyright 2021 daywithstars

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

/** 
* \file 
*
* The Proto Test the structure that holds everything together. 
*/ 


#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#include <SDL_stdinc.h>

#include <PT_String.h>


/**
* \brief Allocate all basic objects that will be used from the application.
*
* \returns SDL_TRUE on success, or SDL_FALSE on error.
*
* \sa PT_ApplicationDestroy
*/
SDL_bool PT_ApplicationCreate( );

/** 
* \brief After the PT_ApplicationRun and quit, this function will clear all remains objects.
*
* \sa PT_ApplicationCreate
*/
void PT_ApplicationDestroy( ); 

/**
* \brief Get the current game name.
*
* \returns The game name.
*/
const PT_String* PT_ApplicationGetGameName( );

/**
* \brief Set to SDL_TRUE if still running some background processing.
*
* @param value SDL_TRUE meaning the application can exits at any time.
*
* This function is required if you need some module that is already running for
* that background processing before it exits.
*
* Do not use this function to processing that is called in iterations like in every 3 seconds
* you create an thread and call this function with SDL_FALSE, or your application never exits if the
* thread consumes more time than 3 seconds. 
*/
void PT_ApplicationCanExit( SDL_bool value );

/**
* \brief Run the application, this will remains until the application is running.
*/
void PT_ApplicationRun( ); 

#endif /* _APPLICATION_H_ */



