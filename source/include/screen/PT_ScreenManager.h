/*
Copyright 2021 daywithstars

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

/** 
* \file 
*
* The Proto Test include header for Screen operations.
*/


#ifndef _PT_SCREENMANAGER_H_
#define _PT_SCREENMANAGER_H_

#include <SDL_stdinc.h>

#include <PT_Screen.h>


/**
* \brief Creates the PT_ScreenManager if it not exist.
*
* \returns SDL_TRUE on success or SDL_FALSE on error.
*
* \sa PT_ScreenManagerDestroy
*/
SDL_bool PT_ScreenManagerCreate( );

/**
* \brief Frees all the memory used, including the loaded screens.
*
* \sa PT_ScreenManagerCreate
*/
void PT_ScreenManagerDestroy( );

/**
* \brief Use this function to update the PT_ScreenManager with assets/screen/screen-list.json.
*
* \sa PT_ScreenManagerLoadScreen
* \sa PT_ScreenManagerSetFirstScreen
* \sa PT_ScreenManagerSetSecondScreen
*/
void PT_ScreenManagerSetup(  );

/**
* \brief Use this function to set and previous loaded screen to be on slot 1. 
*
* @param utf8_screenName The screen name from screen-list.json file.
*
* After the PT_ScreenManagerSetup function you can use PT_ScreenManagerLoadScreen
* and load an screen from screen-list.json to the application, and with this function
* you can set the screen to populate the first slot. 
*
* The same screen cannot be set to an second slot or vice-versa.
*
* \sa PT_ScreenManagerSetup
* \sa PT_ScreenManagerLoadScreen
* \sa PT_ScreenManagerSetSecondScreen
*/
void PT_ScreenManagerSetFirstScreen( const char* utf8_screenName );

/** 
* \brief  Use this function to set and previous loaded screen to be on slot 2.
*
* @param utf8_screenName The screen name from screen-list.json file.
*
* Read the description from PT_ScreenManagerSetFirstScreen.
*
* \sa PT_ScreenManagerSetup
* \sa PT_ScreenManagerLoadScreen
* \sa PT_ScreenManagerSetFirstScreen
*/
void PT_ScreenManagerSetSecondScreen( const char* utf8_screenName );

/** 
* \brief Loads any screen from screen-list.json to the application.
*
* @param utf8_screenName The screen name from screen-list.json file.
*
* \returns SDL_TRUE on success or SDL_FALSE on error.
*
* This function do not display the loaded screen you'll need to attach to 
* an slot with PT_ScreenManagerSetFirstScreen or PT_ScreenManagerSetSecondScreen.
*
* \sa PT_ScreenManagerSetup
* \sa PT_ScreenManagerSetFirstScreen
* \sa PT_ScreenManagerSetSecondScreen
* \sa PT_ScreenManagerUnloadScreen
*/
SDL_bool PT_ScreenManagerLoadScreen( const char* utf8_screenName );

/**
* \brief This function free any loaded screen from the memory.
*
* @param utf8_screenName Previous loaded screen with PT_ScreenManagerLoadScreen.
*
* \sa PT_ScreenManagerLoadScreen
*/
void PT_ScreenManagerUnloadScreen( const char* utf8_screenName );

/**
* \brief Update's screen slot's if they are in use. 
*
* @param elapsedTime The number of milliseconds elapsed between each iteration.
*
* The elapsedTime is given by mainLoop the internal function from PT_Application.
*
* \sa PT_ScreenManagerDraw
*/
void PT_ScreenManagerUpdate( Sint32 elapsedTime );

/** 
* \brief Draw's screen slot's if they are in use.
*
* \sa PT_ScreenManagerUpdate
*/
void PT_ScreenManagerDraw( );

#endif /* _PT_SCREENMANAGER_H_ */



