/*
Copyright 2021 daywithstars

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

#ifndef _PT_SPRITE_H_
#define _PT_SPRITE_H_

#include <SDL.h>

#include <json.h>

#include <PT_Behavior.h>


typedef struct pt_sprite PT_Sprite;


PT_Sprite* PT_SpriteCreate( const char* utf8_spriteTemplate, void* _data, 
	SDL_bool (*dataParse)(void* _data, json_value* jsonValue) );

void PT_SpriteDestroy( PT_Sprite* _this );


void PT_SpriteAddUpdateCallback( PT_Sprite* _this, void (*callback)(void* _data, Sint32 elapsedTime) );
void PT_SpriteAddDrawCallback( PT_Sprite* _this, void (*callback)(void* _data) );
void PT_SpriteAddDestroyCallback( PT_Sprite* _this, void (*callback)(void* _data) );

void PT_SpriteMoveLeft( void* _data ); 
void PT_SpriteMoveRight( void* _data ); 
void PT_SpriteMoveUp( void* _data ); 
void PT_SpriteMoveDown( void* _data ); 
void PT_SpriteStopMoveHorizontal( void* _data );
void PT_SpriteStopMoveVertical( void* _data );

PT_Behavior* PT_SpriteGetBehavior( PT_Sprite* _this );

void PT_SpriteUpdate( PT_Sprite* _this, Sint32 elapsedTime );
void PT_SpriteDraw( PT_Sprite* _this );

#endif /* _PT_SPRITE_H_ */



