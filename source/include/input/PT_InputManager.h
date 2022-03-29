/*
Copyright 2022 daywithstars

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

/** 
* \file 
*
* The Proto Test include header for Input handling 
*/

#ifndef _PT_INPUTMANAGER_H_
#define _PT_INPUTMANAGER_H_

#include <SDL_stdinc.h>
#include <SDL_events.h>
#include <SDL_scancode.h>
#include <SDL_rect.h>

#include <PT_InputHandler.h>


SDL_bool PT_InputManagerCreateBasic( );

/**
* \brief Create all the types that @PT_InputManager handle with. 
*
* \returns SDL_TRUE on success or SDL_FALSE on failure.
*
* \sa PT_InputManagerDestroy 
*/
SDL_bool PT_InputManagerCreate( );

/** 
* \brief Clear all the memory used by the @PT_ScreenManager and its types.
*/
void PT_InputManagerDestroy( );

/**
* \brief Clear all internal states from the types that is in use.
*
* Call this function before the SDL_PollEvent on main loop. 
*
* \sa PT_KeyboardClearState
*/
void PT_InputManagerClearState( );

/**
* \brief Update all key states from the types that is in use.
*
* @param ev The pointer to the SDL_Event from your SDL_PollEvent on the main loop.
*
* Call this function after every return from SDL_PollEvent from the main loop.
*
* \sa PT_InputManagerClearState
*/
void PT_InputManagerUpdate( const SDL_Event* ev );

PT_InputHandler* PT_InputManagerGetInputHandler( const char* utf8_inputHandlerName );

/**
* \brief The same as PT_KeyboardGetKeyDown.
*
* \sa PT_KeyboardGetKeyDown
*/
SDL_bool PT_InputManagerKeyboardGetKeyDown( SDL_Scancode key );

/** 
* \brief The same as PT_KeyboardGetKeyUp.
*
* \sa PT_KeyboardGetKeyUp
*/
SDL_bool PT_InputManagerKeyboardGetKeyUp( SDL_Scancode key );

/**
* \brief The same as PT_KeyboardGetKeyHold.
*
* \sa PT_KeyboardGetKeyHold
*/
SDL_bool PT_InputManagerKeyboardGetKeyHold( SDL_Scancode key );

SDL_bool PT_InputManagerKeyboardGetKeyEvent( );

void PT_InputManagerMouseGetPosition( Sint32* x, Sint32* y );
void PT_InputManagerMouseGetWheelScroll( Sint32* wheelX, Sint32* wheelY );

SDL_bool PT_InputManagerMouseGetButtonDown( Uint8 button );
SDL_bool PT_InputManagerMouseGetButtonUp( Uint8 button );
SDL_bool PT_InputManagerMouseGetButtonHold( Uint8 button );

SDL_bool PT_InputManagerMouseGetButtonEvent( );

SDL_Rect PT_InputManagerMouseGetRect( );

#endif /* _PT_INPUTMANAGER_H_ */



