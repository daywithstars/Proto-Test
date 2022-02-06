/*
Copyright 2022 daywithstars

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

/** 
* \file 
*
* The Proto Test include header for List textures. 
*/

#ifndef _PT_TEXTURELIST_
#define _PT_TEXTURELIST_

#include <SDL.h>
#include <SDL_stdinc.h>

#include <PT_String.h>

typedef struct pt_texture_list {
	SDL_Texture* value;
	PT_String* index;
	
	struct pt_texture_list* next;
}PT_TextureList;


/**
* \brief Create the first PT_TextureList Node.
*
* @param utf8_index The index node name.
* @param value An valid SDL_Texture pointer or NULL.
*
* \returns An valid PT_TextureList pointer or NULL with error.
*
* \sa PT_TextureListDestroy
*/
PT_TextureList* PT_TextureListCreate( const char* utf8_index, SDL_Texture* value );

/** 
* \brief Free all the memory used by PT_TextureList, this also frees the value if its not NULL.
*
* @param _this PT_TextureList pointer.
*
* \sa PT_TextureListCreate
*/
void PT_TextureListDestroy( PT_TextureList* _this );

/** 
* \brief Add an node to the top of PT_TextureList list or create an head node.
*
* @param _this This can be a NULL PT_TextureList or a valid one.
*
*	case 1 NULL the function will works like: PT_TextureListCreate, and will return the head node.
*
*	case 2 Valid PT_TextureList, it will be used to add into the next node from the new created
*	PT_TextureList and the new PT_TextureList head will be returned.
*
* @param utf8_index The UTF-8 string to identify your texture.
*
*	case 1 The index have the same name from an previous node: the texture will be destroyed 
*	and nothing will occur.
*
*	case 2 The index have different names from previous created node: the function will works 
*	as spectated.
*
* @param value Pointer to your previous created SDL_Texture.
*
* \returns The updated PT_TextureList on success, the previous _this when the utf8_index 
*	is already registered or NULL if fails.
*
* \sa PT_TextureListGet
*/
PT_TextureList* PT_TextureListAdd( PT_TextureList* _this, const char* utf8_index, SDL_Texture* value );

/**
* \brief Get the pointer to an specific node, based on its index.
*
* @param _this PT_TextureList pointer.
* @param utf8_index The index to be search on. 
*
* \returns The pointer to the node on success, or NULL if the node does not exist.
*
* \sa PT_TextureListAdd
*/
PT_TextureList* PT_TextureListGet( PT_TextureList* _this, const char* utf8_index );

#endif /* _PT_TEXTURELIST_ */



