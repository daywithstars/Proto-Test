/*
Copyright 2021 daywithstars

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

/** 
* \file 
*
* The Proto Test include header for List strings.
*/


#ifndef _PT_CALLBACK_LIST_H_
#define _PT_CALLBACK_LIST_H_


#include <SDL.h>


#include <PT_String.h>



typedef struct pt_callback_list {
	PT_String* index;
	void (*simpleCallback)(void* _data);
	void (*SDL_FPointCallback)(void* _data, SDL_FPoint );
	
	struct pt_callback_list* next;
}PT_CallbackList;


/**
* \brief Free all the memory used by PT_CallbackList, this also frees the value if its not NULL.
*
* @param _this PT_CallbackList pointer.
*
* \sa PT_CallbackListCreate
*/
void PT_CallbackListDestroy( PT_CallbackList* _this );

/** 
* \brief Add an simple callback to node from top of PT_CallbackList list or create an head node. 
*
* @param _this This can be a NULL PT_CallbackList or a valid one.
*
*	case 1 NULL the function will works creating the first node and will return the node.
*
*	case 2 Valid PT_CallbackList, it will be used to add into the next node from the new created
*	PT_CallbackList and the new PT_CallbackList head will be returned.
*
* @param utf8_index The UTF-8 string to identify your callback.
*
*	case 1 The index have the same name from an previous node: the callback will not be added.
*
*	case 2 The index have different names from previous created node: the function will works 
*	as spectated.
*
* @param callback Pointer to your simple callback function: void callbackName( void* _data );
*
* \returns The updated PT_CallbackList on success, the previous @_this when the utf8_index 
*	is already registered or NULL if fails.
*
* \sa PT_CallbackListCreate
* \sa PT_CallbackListGet
*/
PT_CallbackList* PT_CallbackListAddSimple( PT_CallbackList* _this, const char* utf8_index, 
	void (*callback)(void* _data) );
	
PT_CallbackList* PT_CallbackListAddSDL_FPoint( PT_CallbackList* _this, const char* utf8_index,
	void (*callback)(void* _data, SDL_FPoint) );

/**
* \brief Get the pointer to an specific node, based on its index.
*
* @param _this An valid PT_CallbackList.
* @param utf8_index The index to be search on. 
*
* \returns The pointer to the node on success, or NULL if the node does not exist.
*
* \sa PT_CallbackListAddSimple
*/
PT_CallbackList* PT_CallbackListGet( PT_CallbackList* _this, const char* utf8_index );

#endif /* _PT_CALLBACK_LIST_H_ */



