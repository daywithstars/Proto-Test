/*
Copyright 2022 daywithstars

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

#ifndef _PT_ANIMATIONLIST_H_
#define _PT_ANIMATIONLIST_H_


#include <PT_String.h>
#include <PT_Animation.h>


typedef struct pt_animation_list {
	PT_String* index;
	PT_Animation value;
	
	struct pt_animation_list* next;
}PT_AnimationList;



/** 
* \brief Create the first PT_AnimationList Node.
*
* @param utf8_index The index node name.
* @param value PT_Animation.
*
* \returns An valid PT_AnimationList pointer or NULL with error.
*
* \sa PT_AnimationListDestroy
*/
PT_AnimationList* PT_AnimationListCreate( const char* utf8_index, PT_Animation value );

/**
* \brief Free all the memory used by PT_AnimationList, this also frees the value if its not NULL.
*
* @param _this PT_AnimationList pointer.
*
* \sa PT_AnimationListCreate
*/
void PT_AnimationListDestroy( PT_AnimationList* _this );

/** 
* \brief Add an node to the top of PT_AnimationList list or create an head node. 
*
* @param _this This can be a NULL PT_AnimationList or a valid one.
*
*	case 1 NULL the function will works like: PT_AnimationListCreate, and will return the head node.
*
*	case 2 Valid PT_AnimationList, it will be used to add into the next node from the new created
*	PT_AnimationList and the new PT_AnimationList head will be returned.
*
* @param utf8_index The UTF-8 string to identify your PT_Animation.
*
*
* @param value PT_Animation.
*
* \returns The updated PT_AnimationList on success, the previous @_this when the utf8_index 
*	is already registered or NULL if fails.
*
* \sa PT_AnimationListCreate
* \sa PT_AnimationListGet
*/
PT_AnimationList* PT_AnimationListAdd( PT_AnimationList* _this, const char* utf8_index, 
	PT_Animation value );

/**
* \brief Get the pointer to an specific node, based on its index.
*
* @param _this An valid PT_AnimationList.
* @param utf8_index The index to be search on. 
*
* \returns The pointer to the node on success, or NULL if the node does not exist.
*
* \sa PT_AnimationListAdd
*/
PT_AnimationList* PT_AnimationListGet( PT_AnimationList* _this, const char* utf8_index );


#endif /* _PT_ANIMATIONLIST_H_ */



