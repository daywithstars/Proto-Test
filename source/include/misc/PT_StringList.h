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


#ifndef _PT_STRING_LIST_H_
#define _PT_STRING_LIST_H_

#include <PT_String.h>


typedef struct pt_string_list {
	PT_String* index;
	PT_String* value;
	
	struct pt_string_list* next;
}PT_StringList;



/** 
* \brief Create the first PT_StringList Node.
*
* @param utf8_index The index node name.
* @param value An valid PT_String pointer or NULL.
*
* \returns An valid PT_StringList pointer or NULL with error.
*
* \sa PT_StringListDestroy
*/
PT_StringList* PT_StringListCreate( const char* utf8_index, PT_String* value );

/**
* \brief Free all the memory used by PT_StringList, this also frees the value if its not NULL.
*
* @param _this PT_StringList pointer.
*
* \sa PT_StringListCreate
*/
void PT_StringListDestroy( PT_StringList* _this );

/** 
* \brief Add an node to the top of PT_StringList list or create an head node. 
*
* @param _this This can be a NULL PT_StringList or a valid one.
*
*	case 1 NULL the function will works like: PT_StringListCreate, and will return the head node.
*
*	case 2 Valid PT_StringList, it will be used to add into the next node from the new created
*	PT_StringList and the new PT_StringList head will be returned.
*
* @param utf8_index The UTF-8 string to identify your PT_String.
*
*	case 1 The index have the same name from an previous node: the PT_String will be destroyed 
*	and nothing will occur.
*
*	case 2 The index have different names from previous created node: the function will works 
*	as spectated.
*
* @param value Pointer to your previous created PT_String.
*
* \returns The updated PT_StringList on success, the previous @_this when the utf8_index 
*	is already registered or NULL if fails.
*
* \sa PT_StringListCreate
* \sa PT_StringListGet
*/
PT_StringList* PT_StringListAdd( PT_StringList* _this, const char* utf8_index, PT_String* value );

/**
* \brief Get the pointer to an specific node, based on its index.
*
* @param _this An valid PT_StringList.
* @param utf8_index The index to be search on. 
*
* \returns The pointer to the node on success, or NULL if the node does not exist.
*
* \sa PT_StringListAdd
*/
PT_StringList* PT_StringListGet( PT_StringList* _this, const char* utf8_index );

#endif /* _PT_STRING_LIST_H_ */



