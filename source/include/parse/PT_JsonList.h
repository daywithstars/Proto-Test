/*
Copyright 2021 daywithstars

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

/** 
* \file 
*
* The Proto Test include header for List json_value's. 
*/


#ifndef _PT_JSONLIST_H_
#define _PT_JSONLIST_H_

#include <json.h>

#include <PT_String.h>


typedef struct pt_json_list {
	PT_String* index;
	json_value* value;

	struct pt_json_list* next;
}PT_JsonList;


/**
* \brief Create the first PT_JsonList Node.
*
* @param utf8_index The index node name.
* @param value An valid json_value pointer or NULL.
*
* \returns An valid json_value pointer or NULL with error.
*
* \sa PT_JsonListDestroy
*/
PT_JsonList* PT_JsonListCreate( const char* utf8_index, json_value* value );

/**
* \brief Free all the memory used by PT_JsonList, this also frees the value if its not NULL.
*
* @param _this PT_JsonList pointer.
*
* \sa PT_JsonListCreate
*/
void PT_JsonListDestroy( PT_JsonList* _this );

/** 
* \brief Add an node to the top of PT_JsonList list or create an head node.
*
* @param _this This can be a NULL PT_JsonList or a valid one.
*
*	case 1 NULL the function will works like: PT_JsonListCreate, and will return the head node.
*
*	case 2 Valid PT_JsonList, it will be used to add into the next node from the new created
*	PT_JsonList and the new PT_JsonList head will be returned.
*
* @param utf8_index The UTF-8 string to identify your json_value.
*
*	case 1 The index have the same name from an previous node: the json_value will be destroyed 
*	and nothing will occur.
*
*	case 2 The index have different names from previous created node: the function will works 
*	as spectated.
*
* @param value Pointer to your previous created json_value.
*
* \returns The updated PT_JsonList on success, the previous _this when the utf8_index 
*	is already registered or NULL if fails.
*
* * \sa PT_JsonListGet
*/
PT_JsonList* PT_JsonListAdd( PT_JsonList* _this, const char* utf8_index, json_value* value );

/**
* \brief Get the pointer to an specific node, based on its index.
*
* @param _this An valid PT_JsonList.
* @param utf8_index The index to be search on. 
*
* \returns The pointer to the node on success, or NULL if the node does not exist.
*
* \sa PT_JsonListAdd
*/
PT_JsonList* PT_JsonListGet( PT_JsonList* _this, const char* utf8_index );

#endif /* _PT_JSONLIST_H_ */



