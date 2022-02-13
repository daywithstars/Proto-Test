/*
Copyright 2022 daywithstars

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

#include <SDL_log.h>

#include <PT_Sprite.h>
#include <PT_Parse.h>
#include <PT_Graphics.h>
#include <PT_CollisionManager.h>
#include <PT_Behavior.h>
#include <PT_Camera.h>




//===================================== PRIVATE Functions

SDL_bool PT_SpriteParse( PT_Sprite* _this, json_value* jsonValue );


//===================================== PUBLIC Functions

PT_Sprite* PT_SpriteCreate( const char* utf8_spriteTemplate, void* _data, 
	SDL_bool (*dataParse)(PT_Sprite* sprite, void* _data, json_value* jsonValue) ) {
	
	PT_Sprite* _this = (PT_Sprite*)malloc(sizeof(PT_Sprite));
	if ( !_this )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_SpriteCreate: Not enough memory\n");
		return NULL;
	}
	SDL_memset(_this, 0, sizeof(PT_Sprite));
	
	PT_String* path = PT_StringCreate();
	PT_StringInsert(&path, ".json", 0);
	PT_StringInsert(&path, utf8_spriteTemplate, 0);
	PT_StringInsert(&path, "assets/sprite/", 0);
	json_value* _jsonValue = PT_ParseGetJsonValueFromFile((char*)path->utf8_string, SDL_TRUE);
	
	if ( !_jsonValue )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_SpriteCreate!\n");
		PT_SpriteDestroy(_this);
		return NULL;
	}
	
	if ( !PT_SpriteParse(_this, _jsonValue) )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_SpriteCreate!\n");
		PT_SpriteDestroy(_this);
		json_value_free(_jsonValue);
		PT_StringDestroy(path);
		return NULL;
	}
	
	if ( dataParse )
	{
		if ( !dataParse(_this, _data, _jsonValue) )
		{
			SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_SpriteCreate!\n");
			PT_SpriteDestroy(_this);
			json_value_free(_jsonValue);
			PT_StringDestroy(path);
			return NULL;
		}
	}
	
	_this->_data = _data;
	
	json_value_free(_jsonValue);
	PT_StringDestroy(path);
	return _this;
}//PT_SpriteCreate

void PT_SpriteDestroy( PT_Sprite* _this ) {
	if ( !_this )
	{
		return;
	}
	
	PT_StringDestroy(_this->imageName);
	if ( _this->srcRect )
	{
		free(_this->srcRect);
	}
	PT_AnimationListDestroy(_this->animationList);
	if ( _this->colliders )
	{
		for ( unsigned int i = 0; i < _this->numColliders; i++ )
		{
			PT_ColliderDestroy(&_this->colliders[i]);
		}
		free(_this->colliders);
	}
	PT_BehaviorDestroy(_this->behavior);
	
	if ( _this->destroy )
	{
		_this->destroy(_this->_data);
	}
	
	free(_this);
}//PT_SpriteDestroy

void PT_SpriteAddCollisionCallback( PT_Sprite* _this, 
	void (*callback)(void* _data, PT_Collider, PT_Collider) ) {
	_this->collision = callback;
}
void PT_SpriteAddUpdateCallback( PT_Sprite* _this, void (*callback)(void* _data, Sint32 elapsedTime) ) {
	_this->update = callback;
}
void PT_SpriteAddDrawCallback( PT_Sprite* _this, void (*callback)(void* _data) ) {
	_this->draw = callback;
}
void PT_SpriteAddDestroyCallback( PT_Sprite* _this, void (*callback)(void* _data) ) {
	_this->destroy = callback;
}

void PT_SpriteMoveLeft( void* _data ) {
	PT_Sprite* _this = (PT_Sprite*)_data;
	_this->dirX = -1;
}
void PT_SpriteMoveRight( void* _data ) {
	PT_Sprite* _this = (PT_Sprite*)_data;
	_this->dirX = 1;
}
void PT_SpriteMoveUp( void* _data ) {
	PT_Sprite* _this = (PT_Sprite*)_data;
	_this->dirY = -1;
}
void PT_SpriteMoveDown( void* _data ) {
	PT_Sprite* _this = (PT_Sprite*)_data;
	_this->dirY = 1;
}
void PT_SpriteStopMoveHorizontal( void* _data ) {
	PT_Sprite* _this = (PT_Sprite*)_data;
	_this->dirX = 0;
}
void PT_SpriteStopMoveVertical( void* _data ) {
	PT_Sprite* _this = (PT_Sprite*)_data;
	_this->dirY = 0;
}

SDL_bool PT_SpriteChangeAnimation( void* _data, const char* utf8_string ) {
	PT_Sprite* _this = (PT_Sprite*)_data;
	
	if ( _this->animationList )
	{
		PT_AnimationList* node = PT_AnimationListGet(_this->animationList, utf8_string);
		if ( node )
		{
			if ( _this->currentAnimation.id != node->value.id )
			{
				_this->currentAnimation = node->value;
				return SDL_TRUE;
			}
		}
	}
	
	return SDL_FALSE;
}

void PT_SpriteGrab( void* _data, SDL_FPoint mousePosition ) {
	PT_Sprite* _this = (PT_Sprite*)_data;
	
	//Test collision then set positions
	const SDL_Rect mouseRect = { (int)mousePosition.x, (int)mousePosition.y, 4, 4 };
	const SDL_Rect spriteRect = { (int)_this->dstRect.x, (int)_this->dstRect.y,
		(int)_this->dstRect.w, (int)_this->dstRect.h };
	
	if ( SDL_HasIntersection(&mouseRect, &spriteRect) )
	{	
		_this->dstRect.x = mousePosition.x - spriteRect.w / 2;
		_this->dstRect.y = mousePosition.y - spriteRect.h / 2;
	}
}//PT_SpriteGrab

void PT_SpriteCollisionWith( PT_Sprite* _this, PT_Collider own, PT_Collider target ) {
	if ( _this->collision )
	{
		_this->collision(_this->_data, own, target);
	}
}//PT_SpriteCollisionWith

void PT_SpriteUpdate( PT_Sprite* _this, Sint32 elapsedTime ) {
	if ( _this->tagStopHorizontal )
	{
		PT_SpriteStopMoveHorizontal((void*)_this);
	}
	if ( _this->tagStopVertical )
	{
		PT_SpriteStopMoveVertical((void*)_this);
	}

	if ( _this->animationList )
	{
		PT_AnimationUpdate(&_this->currentAnimation, _this->srcRect, elapsedTime);
	}
	if ( _this->behavior )
	{
		PT_BehaviorUpdate(_this->behavior, (void*)_this, elapsedTime);
	}
	_this->collisionColliderName = NULL;
	_this->collisionTargetColliderName = NULL;

	_this->dstRect.x += _this->speedX * elapsedTime * _this->dirX;
	_this->dstRect.y += _this->speedY * elapsedTime * _this->dirY;
	
	if ( _this->update )
	{
		_this->update(_this->_data, elapsedTime);
	}
}//PT_SpriteUpdate

void PT_SpriteDraw( PT_Sprite* _this ) {
	if ( _this->imageName ) 
	{
		if ( _this->cameraTarget )
		{
			const SDL_FRect dst = { _this->dstRect.x + PT_CameraGetX(), 
				_this->dstRect.y + PT_CameraGetY(), _this->dstRect.w, _this->dstRect.h };
				
			PT_GraphicsDrawTextureF((char*)_this->imageName->utf8_string, _this->srcRect, &dst,
				0.0, NULL, SDL_FLIP_NONE);
		}
		else {
			PT_GraphicsDrawTextureF((char*)_this->imageName->utf8_string, _this->srcRect, &_this->dstRect,
							0.0, NULL, SDL_FLIP_NONE);
		}
	}
	if ( _this->draw )
	{
		_this->draw(_this->_data);
	}
	for ( unsigned int i = 0; i < _this->numColliders; i++ )
	{	
		if ( _this->cameraTarget )
		{
			PT_ColliderDraw(_this->colliders[i], 
			_this->dstRect.x + PT_CameraGetX(), _this->dstRect.y + PT_CameraGetY());
		}
		else
		{
			PT_ColliderDraw(_this->colliders[i], _this->dstRect.x, _this->dstRect.y);
		}
	}
}//PT_SpriteDraw


//===================================== PRIVATE Functions

SDL_bool PT_SpriteParse( PT_Sprite* _this, json_value* jsonValue ) {	
	/*
		see the template: games/default-templates/sprite-folder/sprite-template.json
	*/
	if ( jsonValue->type == json_object )
	{
		for ( unsigned int i = 0; i < jsonValue->u.object.length; i++ )
		{
			if ( !strcmp("tags", jsonValue->u.object.values[i].name) )
			{
				for ( unsigned int j = 0; j < jsonValue->u.object.values[i].value->u.object.length; j++ )
				{
					if ( !strcmp("collider", 
						jsonValue->u.object.values[i].value->u.object.values[j].name) )
					{
						PT_CollisionManagerAdd(
							jsonValue->u.object.values[i].value->u.object.values[j].value->u.string.ptr, 
							_this);
					}	
					else if ( !strcmp("movement", 
						jsonValue->u.object.values[i].value->u.object.values[j].name) )
					{
						for ( unsigned int k = 0; 
						k < jsonValue->u.object.values[i].value->u.object.values[j].value->u.array.length;
						k++ )
						{
							if ( 
								!strcmp("constant-stop-vertical",
								jsonValue->u.object.values[i]
								.value->u.object.values[j]
								.value->u.array.values[k]->u.string.ptr
								) )
							{
								_this->tagStopVertical = SDL_TRUE;
							}
							if ( 
								!strcmp("constant-stop-horizontal",
								jsonValue->u.object.values[i]
								.value->u.object.values[j]
								.value->u.array.values[k]->u.string.ptr
								) )
							{
								_this->tagStopHorizontal = SDL_TRUE;
							}
						}
					}
				}
			}
			else if ( !strcmp("image", jsonValue->u.object.values[i].name) )
			{
				_this->imageName = PT_StringCreate();
				PT_StringInsert(&_this->imageName, jsonValue->u.object.values[i].value->u.string.ptr, 0);
			}
			else if ( !strcmp("speedX", jsonValue->u.object.values[i].name) )
			{ 
				_this->speedX = jsonValue->u.object.values[i].value->u.dbl;
			}
			else if ( !strcmp("speedY", jsonValue->u.object.values[i].name) )
			{
				_this->speedY = jsonValue->u.object.values[i].value->u.dbl;
			}
			else if ( !strcmp("srcRect", jsonValue->u.object.values[i].name) )
			{
				_this->srcRect = (SDL_Rect*)malloc(sizeof(SDL_Rect));
				
				_this->srcRect->x = 
				jsonValue->u.object.values[i].value->u.array.values[0]->u.integer;
				_this->srcRect->y = 
				jsonValue->u.object.values[i].value->u.array.values[1]->u.integer;
				_this->srcRect->w = 
				jsonValue->u.object.values[i].value->u.array.values[2]->u.integer;
				_this->srcRect->h = 
				jsonValue->u.object.values[i].value->u.array.values[3]->u.integer;
			}
			else if ( !strcmp("dstRect", jsonValue->u.object.values[i].name) )
			{	
				_this->dstRect.x = 
				jsonValue->u.object.values[i].value->u.array.values[0]->u.dbl;
				_this->dstRect.y =
				jsonValue->u.object.values[i].value->u.array.values[1]->u.dbl;
				_this->dstRect.w = 
				jsonValue->u.object.values[i].value->u.array.values[2]->u.dbl;
				_this->dstRect.h = 
				jsonValue->u.object.values[i].value->u.array.values[3]->u.dbl;
			}
			else if ( !strcmp("animations", jsonValue->u.object.values[i].name) )
			{
				
				for ( unsigned int j = 0; j < jsonValue->u.object.values[i].value->u.array.length; j++ )
				{
					json_value* animObj = jsonValue->u.object.values[i].value->u.array.values[j];
					
					char* name = "default";
					Uint16 frameDelay = 100;
					Uint16 frameWidth = 32;
					Uint16 frameHeight = 32;
					Sint16 frameColumn = 0;
					Sint8 frameColumnDir = 1;
					Uint16 frameColumnMax = 2;
					Sint16 frameRow = 0;
					Sint16 frameRowDir = 0;
					Uint16 frameRowMax = 2;
				
					json_object_entry entry = PT_ParseGetObjectEntry_json_value(animObj, "name");
					if ( entry.name )
					{
						name = entry.value->u.string.ptr;
					}
					entry = PT_ParseGetObjectEntry_json_value(animObj, "frame-delay");
					if ( entry.name )
					{
						frameDelay = entry.value->u.integer;
					}
					entry = PT_ParseGetObjectEntry_json_value(animObj, "frame-width");
					if ( entry.name )
					{
						frameWidth = entry.value->u.integer;
					}
					entry = PT_ParseGetObjectEntry_json_value(animObj, "frame-height");
					if ( entry.name )
					{
						frameHeight = entry.value->u.integer;
					}
					entry = PT_ParseGetObjectEntry_json_value(animObj, "frame-column");
					if ( entry.name )
					{
						frameColumn = entry.value->u.integer;
					}
					entry = PT_ParseGetObjectEntry_json_value(animObj, "frame-column-dir");
					if ( entry.name )
					{
						frameColumnDir = entry.value->u.integer;
					}
					entry = PT_ParseGetObjectEntry_json_value(animObj, "frame-column-max");
					if ( entry.name )
					{
						frameColumnMax = entry.value->u.integer;
					}
					entry = PT_ParseGetObjectEntry_json_value(animObj, "frame-row");
					if ( entry.name )
					{
						frameRow = entry.value->u.integer;
					}
					entry = PT_ParseGetObjectEntry_json_value(animObj, "frame-row-dir");
					if ( entry.name )
					{
						frameRowDir = entry.value->u.integer;
					}
					entry = PT_ParseGetObjectEntry_json_value(animObj, "frame-row-max");
					if ( entry.name )
					{
						frameRowMax = entry.value->u.integer;
					}
					
					
					PT_Animation anim = 
					PT_AnimationCreate(
						frameDelay,
						frameWidth,
						frameHeight,
						frameColumn,
						frameColumnDir,
						frameColumnMax,
						frameRow,
						frameRowDir,
						frameRowMax
					);
					
					_this->animationList = 
					PT_AnimationListAdd(
						_this->animationList, 
						name,
						anim
					);
					
					if ( j == 0 )
					{
						_this->currentAnimation = anim;
					}
				}
			}
			else if ( !strcmp("colliders", jsonValue->u.object.values[i].name) )
			{
				_this->colliders = (PT_Collider*)malloc(
					sizeof(PT_Collider) * jsonValue->u.object.values[i].value->u.array.length );
				if ( !_this->colliders )
				{
					SDL_Log("PT: PT_SpriteParse: Not enough memory!\n");
					continue;
				}	
				_this->numColliders = jsonValue->u.object.values[i].value->u.array.length;
				
				for ( unsigned int j = 0; j < _this->numColliders; j++ )
				{
					_this->colliders[j] = 
					PT_ColliderCreate(jsonValue->u.object.values[i].value->u.array.values[j]);
					
					if ( !_this->colliders[j].name )
					{
						SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_SpriteParse!\n");
						continue;
					}
				}
			}
			else if ( !strcmp("behavior", jsonValue->u.object.values[i].name) )
			{
				
				_this->behavior = PT_BehaviorCreate( 
					(void*)_this,
					jsonValue->u.object.values[i].value->u.string.ptr
				);
				
				if ( !_this->behavior )
				{
					SDL_Log("(*)PT: PT_SpriteParse: Can't create behavior\n");
					continue;
				}
				
				PT_BehaviorAddSimpleCallback(_this->behavior, "PT_SpriteMoveLeft", PT_SpriteMoveLeft);
				PT_BehaviorAddSimpleCallback(_this->behavior, "PT_SpriteMoveRight", PT_SpriteMoveRight);
				PT_BehaviorAddSimpleCallback(_this->behavior, "PT_SpriteMoveUp", PT_SpriteMoveUp);
				PT_BehaviorAddSimpleCallback(_this->behavior, "PT_SpriteMoveDown", PT_SpriteMoveDown);
				PT_BehaviorAddSimpleCallback(_this->behavior, 
					"PT_SpriteStopMoveHorizontal", PT_SpriteStopMoveHorizontal);
				PT_BehaviorAddSimpleCallback(_this->behavior, 
					"PT_SpriteStopMoveVertical", PT_SpriteStopMoveVertical);
	
				PT_BehaviorAddStringCallback(_this->behavior, 
					"PT_SpriteChangeAnimation", PT_SpriteChangeAnimation);				
					
				PT_BehaviorAddSDL_FPointCallback(_this->behavior, "PT_SpriteGrab", PT_SpriteGrab);
			}
		}
	}
	
	return SDL_TRUE;
}



