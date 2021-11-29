/*
Copyright 2021 daywithstars

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

#ifndef _PT_INPUTHANDLERLIST_H_
#define _PT_INPUTHANDLERLIST_H_


#include <PT_String.h>
#include <PT_InputHandler.h>


typedef struct pt_input_handler_list {
	PT_String* index;
	PT_InputHandler* value;
	
	struct pt_input_handler_list* next;
}PT_InputHandlerList;



/** 
* \brief Create the first PT_InputHandlerList Node.
*
* @param utf8_index The index node name.
* @param value An valid PT_String pointer or NULL.
*
* \returns An valid PT_InputHandlerList pointer or NULL with error.
*
* \sa PT_InputHandlerListDestroy
*/
PT_InputHandlerList* PT_InputHandlerListCreate( const char* utf8_index, PT_InputHandler* value );

/**
* \brief Free all the memory used by PT_InputHandlerList, this also frees the value if its not NULL.
*
* @param _this PT_InputHandlerList pointer.
*
* \sa PT_InputHandlerListCreate
*/
void PT_InputHandlerListDestroy( PT_InputHandlerList* _this );

/** 
* \brief Add an node to the top of PT_InputHandlerList list or create an head node. 
*
* @param _this This can be a NULL PT_InputHandlerList or a valid one.
*
*	case 1 NULL the function will works like: PT_InputHandlerListCreate, and will return the head node.
*
*	case 2 Valid PT_InputHandlerList, it will be used to add into the next node from the new created
*	PT_InputHandlerList and the new PT_InputHandlerList head will be returned.
*
* @param utf8_index The UTF-8 string to identify your PT_InputHandler.
*
*	case 1 The index have the same name from an previous node: the PT_InputHandler will be destroyed 
*	and nothing will occur.
*
*	case 2 The index have different names from previous created node: the function will works 
*	as spectated.
*
* @param value Pointer to your previous created PT_InputHandler.
*
* \returns The updated PT_InputHandlerList on success, the previous @_this when the utf8_index 
*	is already registered or NULL if fails.
*
* \sa PT_InputHandlerListCreate
* \sa PT_InputHandlerListGet
*/
PT_InputHandlerList* PT_InputHandlerListAdd( PT_InputHandlerList* _this, const char* utf8_index, PT_InputHandler* value );

/**
* \brief Get the pointer to an specific node, based on its index.
*
* @param _this An valid PT_InputHandlerList.
* @param utf8_index The index to be search on. 
*
* \returns The pointer to the node on success, or NULL if the node does not exist.
*
* \sa PT_InputHandlerListAdd
*/
PT_InputHandlerList* PT_InputHandlerListGet( PT_InputHandlerList* _this, const char* utf8_index );


#endif /* _PT_INPUTHANDLERLIST_H_ */



