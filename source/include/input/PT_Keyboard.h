/*
Copyright 2022 daywithstars

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

/** 
* \file 
*
* The Proto Test include header for keyboard handling 
*/


#ifndef _PT_KEYBOARD_H_
#define _PT_KEYBOARD_H_

#include <SDL_stdinc.h>
#include <SDL_events.h>
#include <SDL_keyboard.h>



typedef struct pt_keyboard PT_Keyboard;


/** 
* \brief Create the PT_Keyboard and return the pointer.
* 
* * \sa PT_KeyboardDestroy
*/
PT_Keyboard* PT_KeyboardCreate( );

/**
* \brief This function clear the memory used by the PT_Keyboard pointer.
*
* @param _this The PT_Keyboard pointer. Remember to set your pointer to NULL after destroy it.
*
* \sa PT_KeyboardCreate
*/
void PT_KeyboardDestroy( PT_Keyboard* _this );

/** 
* \brief Clear internal states from PT_Keyboard for handle with single key press event.
*
* @param _this The PT_Keyboard pointer.
*
* This function needs to be called before SDL_PollEvent in the main loop.
*
* \sa PT_KeyboardUpdate
*/
void PT_KeyboardClearState( PT_Keyboard* _this );

/**
* \brief Updates the keyboard key lists from PT_Keyboard.
*
* @param _this The PT_Keyboard pointer.
* @param ev The pointer to SDL_Event from main loop.
*
* \sa PT_KeyboardClearState
*/
void PT_KeyboardUpdate( PT_Keyboard* _this, const SDL_Event* ev );

/**
* \brief Get SDL_TRUE if 'key' has been pressed.
*
* @param _this The PT_Keyboard pointer.
* @param key The SDL_Scancode value to be tested.
*
* \returns SDL_TRUE on success, or SDL_FALSE if the key haven't been pressed. 
*
* The function will return SDL_TRUE once per key pressed, if you hold the key 
* after pressed it, this will return SDL_FALSE until you release an press again.
*
* \sa PT_KeyboardGetKeyUp
* \sa PT_KeyboardGetKeyHold
*/
SDL_bool PT_KeyboardGetKeyDown( PT_Keyboard* _this, SDL_Scancode key );

/** 
* \brief The same as PT_KeyboardGetKeyDown but with Key up/release events.
*
* @param _this The PT_Keyboard pointer.
* @param key The SDL_Scancode value to be tested.
*
* \returns SDL_TRUE on success, or SDL_FALSE if the key haven't been released. 
*
* \sa PT_KeyboardGetKeyDown
* \sa PT_KeyboardGetKeyHold
*/
SDL_bool PT_KeyboardGetKeyUp( PT_Keyboard* _this, SDL_Scancode key );

/**
* \brief Use this function to know if a key have been pressed until it is released. 
*
* @param _this The PT_Keyboard pointer.
* @param key The SDL_Scancode value to be tested.
*
* \returns SDL_TRUE on success, or SDL_FALSE if the key haven't been holding.  
*
* \sa PT_KeyboardGetKeyDown
* \sa PT_KeyboardGetKeyUp
*/
SDL_bool PT_KeyboardGetKeyHold( PT_Keyboard* _this, SDL_Scancode key );

SDL_bool PT_KeyboardGetKeyEvent( PT_Keyboard* _this );

//give only the last part of the string: keyboard-hold-up in this case "up". All in lower case.
SDL_Scancode PT_KeyboardGetScancodeByString( const char* string );

#endif /* _PT_KEYBOARD_H_ */



