/*
Copyright 2022 daywithstars

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

/** 
* \file 
*
* The Proto Test include header for using on PT_ScreenManager.
*/


#ifndef _PT_SCREEN_H_
#define _PT_SCREEN_H_

#include <SDL_stdinc.h>

#include <json.h>


typedef struct pt_screen PT_Screen;

/** 
* \brief Loads screen objects based on json_value given by PT_ScreenManagerLoadScreen
*
* @param jsonValue The json_value that will be read to instantiate screen objects. 
*
* \returns The PT_Screen pointer on success or NULL on error.
*
* \sa PT_ScreenDestroy
*/
PT_Screen* PT_ScreenCreate( json_value* jsonValue );

/**
* \brief Frees all the memory used by PT_Screen and its objects.
*
* @param _this The PT_Screen pointer.
*
* \sa PT_ScreenCreate
*/
void PT_ScreenDestroy( PT_Screen* _this );

/** 
* \brief Updates all objects inside the PT_Screen.
*
* @param _this The PT_Screen pointer.
* @param elapsedTime is given by mainLoop the internal function from PT_Application.
*
* \sa PT_ScreenDraw
*/
void PT_ScreenUpdate( PT_Screen* _this, Sint32 elapsedTime );

/**
* \brief Draws all the screen contents.
*
* @param _this The PT_Screen pointer.
*
* \sa PT_ScreenUpdate
*/
void PT_ScreenDraw( PT_Screen* _this );

#endif /* _PT_SCREEN_H_ */



