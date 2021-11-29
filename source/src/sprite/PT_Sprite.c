/*
Copyright 2021 daywithstars

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

#include <PT_Sprite.h>
#include <PT_String.h>
#include <PT_Parse.h>
#include <PT_Graphics.h>



struct pt_sprite {
	PT_String* imageName;	
	double x, y;
	int dirX, dirY;
	double speedX, speedY;
	SDL_Rect* srcRect;
	SDL_Rect* dstRect;
	PT_Behavior* behavior;
	
	
	void* _data;
	void (*update)(void* _data, Sint32);
	void (*draw)(void* _data);
	void (*destroy)(void* _data);
};


//===================================== PRIVATE Functions

SDL_bool PT_SpriteParse( PT_Sprite* _this, json_value* jsonValue );


//===================================== PUBLIC Functions

PT_Sprite* PT_SpriteCreate( const char* utf8_spriteTemplate, void* _data, 
	SDL_bool (*dataParse)(void* _data, json_value* jsonValue) ) {
	
	PT_Sprite* _this = (PT_Sprite*)malloc(sizeof(struct pt_sprite));
	if ( !_this )
	{
		return NULL;
	}
	SDL_memset(_this, 0, sizeof(struct pt_sprite));
	
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
		if ( !dataParse(_data, _jsonValue) )
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
	if ( _this->dstRect )
	{
		free(_this->dstRect);
	}
	PT_BehaviorDestroy(_this->behavior);
	
	if ( _this->destroy )
	{
		_this->destroy(_this->_data);
	}
	
	free(_this);
}//PT_SpriteDestroy

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

PT_Behavior* PT_SpriteGetBehavior( PT_Sprite* _this ) {
	return _this->behavior;
}

void PT_SpriteUpdate( PT_Sprite* _this, Sint32 elapsedTime ) {
	PT_BehaviorUpdate(_this->behavior, (void*)_this);

	_this->x += _this->speedX * elapsedTime * _this->dirX;
	_this->y += _this->speedY * elapsedTime * _this->dirY;
	
	if ( _this->update )
	{
		_this->update(_this->_data, elapsedTime);
	}
}//PT_SpriteUpdate

void PT_SpriteDraw( PT_Sprite* _this ) {
	if ( _this->imageName ) 
	{
		if ( _this->dstRect )
		{
			_this->dstRect->x = (int)_this->x;
			_this->dstRect->y = (int)_this->y;
		}
		PT_GraphicsDrawTexture((char*)_this->imageName->utf8_string, _this->srcRect, _this->dstRect,
			0.0, NULL, SDL_FLIP_NONE);
	}
	if ( _this->draw )
	{
		_this->draw(_this->_data);
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
			if ( !strcmp("image", jsonValue->u.object.values[i].name) )
			{
				_this->imageName = PT_StringCreate();
				PT_StringInsert(&_this->imageName, jsonValue->u.object.values[i].value->u.string.ptr, 0);
			}
			else if ( !strcmp("x", jsonValue->u.object.values[i].name) )
			{ 
				_this->x = jsonValue->u.object.values[i].value->u.dbl;
			}
			else if ( !strcmp("y", jsonValue->u.object.values[i].name) )
			{
				_this->y = jsonValue->u.object.values[i].value->u.dbl;
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
				_this->dstRect = (SDL_Rect*)malloc(sizeof(SDL_Rect));
				
				_this->dstRect->w = 
				jsonValue->u.object.values[i].value->u.array.values[0]->u.integer;
				_this->dstRect->h = 
				jsonValue->u.object.values[i].value->u.array.values[1]->u.integer;
			}
			else if ( !strcmp("behavior", jsonValue->u.object.values[i].name) )
			{
				
				_this->behavior = PT_BehaviorCreate(
					jsonValue->u.object.values[i].value->u.string.ptr
				);
				
				if ( !_this->behavior )
				{
					SDL_Log("(*)PT: PT_SpriteParse: Can't create behavior\n");
					continue;
				}
				
				PT_BehaviorAddSimpleCallback(_this->behavior, "sprite-move-left", PT_SpriteMoveLeft);
				PT_BehaviorAddSimpleCallback(_this->behavior, "sprite-move-right", PT_SpriteMoveRight);
				PT_BehaviorAddSimpleCallback(_this->behavior, "sprite-move-up", PT_SpriteMoveUp);
				PT_BehaviorAddSimpleCallback(_this->behavior, "sprite-move-down", PT_SpriteMoveDown);
				PT_BehaviorAddSimpleCallback(_this->behavior, 
					"sprite-stop-horizontal", PT_SpriteStopMoveHorizontal);
				PT_BehaviorAddSimpleCallback(_this->behavior, 
					"sprite-stop-vertical", PT_SpriteStopMoveVertical);
			}
			else {
				break;
			}
		}
	}
	
	return SDL_TRUE;
}



