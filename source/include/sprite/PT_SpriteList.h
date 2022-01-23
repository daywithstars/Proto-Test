/*
Copyright 2022 daywithstars

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

#ifndef _PT_SPRITELIST_H_
#define _PT_SPRITELIST_H_


#include <PT_String.h>
#include <PT_Sprite.h>


typedef struct pt_sprite_list {
	PT_String* index;
	
	unsigned int numValues;
	PT_Sprite** values;
	
	struct pt_sprite_list* next;
}PT_SpriteList;



/** 
* \brief Create the first PT_SpriteList Node or add more values to the node
*
* You can use this function to concatenate more values, but be sure to parse the correct _this/node(not recommended, instead use PT_SpriteListCat). 
*
* @param _this The PT_SpriteList pointer or NULL. 
* @param utf8_index The index node name.
* @param value An valid PT_Sprite pointer or NULL.
*
* \returns An valid PT_SpriteList pointer or NULL with error.
*
* \sa PT_SpriteListDestroy
*/
PT_SpriteList* PT_SpriteListCreate( PT_SpriteList* _this, const char* utf8_index, PT_Sprite* value );

/**
* \brief Free all the memory used by PT_SpriteList, this also frees the value if its not NULL.
*
* @param _this PT_SpriteList pointer.
* @param freeSprites Set to SDL_FALSE if you want this function to not free its sprites. 
*
* \sa PT_SpriteListCreate
*/
void PT_SpriteListDestroy( PT_SpriteList* _this, SDL_bool freeSprites );

/** 
* \brief Add an node to the top of PT_SpriteList list or create an head node. 
*
* @param _this This can be a NULL PT_SpriteList or a valid one.
*
*	case 1 NULL the function will works like: PT_SpriteListCreate, and will return the head node.
*
*	case 2 Valid PT_SpriteList, it will be used to add into the next node from the new created
*	PT_SpriteList and the new PT_SpriteList head will be returned.
*
* @param utf8_index The UTF-8 string to identify your PT_Sprite.
*
*	case 1 The index have the same name from an previous node: the PT_Sprite will be destroyed or not(depending on last param). 
*
*	case 2 The index have different names from previous created node: the function will works 
*	as spectated.
*
* @param value Pointer to your previous created PT_Sprite.
*
* @param freeRepeated if the sprite is already indexed, it will destroy this sprite. 
*
* \returns The updated PT_SpriteList on success, the previous @_this when the utf8_index 
*	is already registered or NULL if fails.
*
* \sa PT_SpriteListCreate
* \sa PT_SpriteListCat
* \sa PT_SpriteListGet
*/
PT_SpriteList* PT_SpriteListAdd( PT_SpriteList* _this, const char* utf8_index, PT_Sprite* value, SDL_bool freeRepeated );

/** 
* \brief Concatenate an PT_Sprite to a node that already have the same index. 
*
* @param _this This can be a NULL PT_SpriteList or a valid one.
*
*	case 1 NULL the function will works like: PT_SpriteListCreate, and will return the head node.
*
*	case 2 Valid PT_SpriteList, it will search for the utf8_index and will concatenate value to that node->index. If the node->index cannot be founded, it will work like: PT_SpriteListAdd
*
* @param value Pointer to your previous created PT_Sprite.
*
* \returns The updated PT_SpriteList on success.
*
* \sa PT_SpriteListCreate
* \sa PT_SpriteListAdd
* \sa PT_SpriteListGet
*/
PT_SpriteList* PT_SpriteListCat( PT_SpriteList* _this, const char* utf8_index, PT_Sprite* value );

/**
* \brief Get the pointer to an specific node, based on its index.
*
* @param _this An valid PT_SpriteList.
* @param utf8_index The index to be search on. 
*
* \returns The pointer to the node on success, or NULL if the node does not exist.
*
* \sa PT_SpriteListAdd
* \sa PT_SpriteListCat
*/
PT_SpriteList* PT_SpriteListGet( PT_SpriteList* _this, const char* utf8_index );


#endif /* _PT_SPRITELIST_H_ */



