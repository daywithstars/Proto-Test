/*
Copyright 2022 daywithstars

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

#ifndef _PT_SPRITE_H_
#define _PT_SPRITE_H_

#include <SDL.h>

#include <json.h>

#include <PT_String.h>
#include <PT_AnimationList.h>
#include <PT_Collider.h>


struct pt_behavior;

typedef struct {
	SDL_bool tagStopHorizontal;
	SDL_bool tagStopVertical;

	PT_String* imageName;	
	
	int dirX, dirY;
	float speedX, speedY;
	SDL_Rect* srcRect;
	SDL_FRect dstRect;
	SDL_bool cameraTarget;
	
	PT_AnimationList* animationList;
	PT_Animation currentAnimation;
	
	unsigned int numColliders;
	PT_Collider* colliders;
	PT_String* collisionColliderName;
	PT_String* collisionTargetColliderName;
	
	struct pt_behavior* behavior;
	
	
	void* _data;
	void (*collision)(void* _data, PT_Collider, PT_Collider);
	void (*update)(void* _data, Sint32);
	void (*draw)(void* _data);
	void (*destroy)(void* _data);
} PT_Sprite;


PT_Sprite* PT_SpriteCreate( const char* utf8_spriteTemplate, void* _data, 
	SDL_bool (*dataParse)(PT_Sprite* sprite, void* _data, json_value* jsonValue) );

PT_Sprite* PT_SpriteCreateFromJsonValue( json_value* jsonValue, void* _data, 
	SDL_bool (*dataParse)(PT_Sprite* sprite, void* _data, json_value* jsonValue) );

PT_Sprite* PT_SpriteCreateFromStringTemplate( const char* utf8_stringTemplate, void* _data, 
	SDL_bool (*dataParse)(PT_Sprite* sprite, void* _data, json_value* jsonValue) );

void PT_SpriteDestroy( PT_Sprite* _this );


void PT_SpriteAddCollisionCallback( PT_Sprite* _this, 
	void (*callback)(void* _data, PT_Collider, PT_Collider) );
void PT_SpriteAddUpdateCallback( PT_Sprite* _this, void (*callback)(void* _data, Sint32 elapsedTime) );
void PT_SpriteAddDrawCallback( PT_Sprite* _this, void (*callback)(void* _data) );
void PT_SpriteAddDestroyCallback( PT_Sprite* _this, void (*callback)(void* _data) );

//Simple callbacks declarations.
void PT_SpriteMoveLeft( void* _data ); 
void PT_SpriteMoveRight( void* _data ); 
void PT_SpriteMoveUp( void* _data ); 
void PT_SpriteMoveDown( void* _data ); 
void PT_SpriteStopMoveHorizontal( void* _data );
void PT_SpriteStopMoveVertical( void* _data );

//String callbacks declarations.
//return true if it changes. 
SDL_bool PT_SpriteChangeAnimation( void* _data, const char* utf8_string );

//Misc callbacks declarations.
void PT_SpriteGrab( void* _data, SDL_FPoint mousePosition );

void PT_SpriteCollisionWith( PT_Sprite* _this, PT_Collider own, PT_Collider target );

void PT_SpriteUpdate( PT_Sprite* _this, Sint32 elapsedTime );
void PT_SpriteDraw( PT_Sprite* _this );

#endif /* _PT_SPRITE_H_ */



