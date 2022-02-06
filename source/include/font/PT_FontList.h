/*
Copyright 2022 daywithstars

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

#ifndef _PT_FONTLIST_H_
#define _PT_FONTLIST_H_

#include <SDL_stdinc.h>

#include <PT_Font.h>
#include <PT_String.h>

typedef struct pt_font_list {
	PT_Font* value;
	PT_String* index;
	
	struct pt_font_list* next;
}PT_FontList;


/**
* \brief Create the first PT_FontList Node.
*
* @param utf8_index The index node name.
* @param value An valid PT_Font pointer or NULL.
*
* \returns An valid PT_FontList pointer or NULL with error.
*
* \sa PT_FontListDestroy
*/
PT_FontList* PT_FontListCreate( const char* utf8_index, PT_Font* value );

/** 
* \brief Free all the memory used by PT_FontList, this also frees the value if its not NULL.
*
* @param _this PT_FontList pointer.
*
* \sa PT_FontListCreate
*/
void PT_FontListDestroy( PT_FontList* _this );

/** 
* \brief Add an node to the top of PT_FontList list or create an head node.
*
* @param _this This can be a NULL PT_FontList or a valid one.
*
*	case 1 NULL the function will works like: PT_FontListCreate, and will return the head node.
*
*	case 2 Valid PT_FontList, it will be used to add into the next node from the new created
*	PT_FontList and the new PT_FontList head will be returned.
*
* @param utf8_index The UTF-8 string to identify your font.
*
*	case 1 The index have the same name from an previous node: the font will be destroyed 
*	and nothing will occur.
*
*	case 2 The index have different names from previous created node: the function will works 
*	as spectated.
*
* @param value Pointer to your previous created PT_Font.
*
* \returns The updated PT_FontList on success, the previous _this when the utf8_index 
*	is already registered or NULL if fails.
*
* \sa PT_FontListGet
*/
PT_FontList* PT_FontListAdd( PT_FontList* _this, const char* utf8_index, PT_Font* value );

/**
* \brief Get the pointer to an specific node, based on its index.
*
* @param _this PT_FontList pointer.
* @param utf8_index The index to be search on. 
*
* \returns The pointer to the node on success, or NULL if the node does not exist.
*
* \sa PT_FontListAdd
*/
PT_FontList* PT_FontListGet( PT_FontList* _this, const char* utf8_index );

#endif /* _PT_FONTLIST_H_ */



