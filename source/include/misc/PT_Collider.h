/*
Copyright 2022 daywithstars

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

#ifndef _PT_COLLIDER_H_
#define _PT_COLLIDER_H_

#include <SDL_rect.h>
#include <SDL_pixels.h>

#include <PT_String.h>

typedef enum {
	PT_COLLIDER_TYPE_RECTANGLE=1
}PT_ColliderType;


typedef struct {
	PT_ColliderType type;
	PT_String* name;
	
	SDL_FRect* rect;
}PT_Collider;


PT_Collider PT_ColliderCreate();
void PT_ColliderDestroy( PT_Collider* _this );

SDL_bool PT_ColliderSetName( PT_Collider* _this, const char* utf8_name );
SDL_bool PT_ColliderSetRect( PT_Collider* _this, float x, float y, float w, float h );

void PT_ColliderDraw( PT_Collider* _this, SDL_Color color, float relativeX, float relativeY );


#endif /* _PT_COLLIDIR_H_ */



