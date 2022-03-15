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

#include <PT_LevelObjectLayer.h>
#include <PT_Parse.h>
#include <PT_SpriteFactory.h>
#include <PT_CollisionManager.h>
#include <PT_Camera.h>


//===================================== PRIVATE Functions

SDL_bool PT_LevelObjectLayerParse( void* layerData, json_value* jsonValue );

//===================================== PUBLIC Functions

PT_LevelLayer* PT_LevelObjectLayerCreate( json_value* jsonValue, const char* utf8_levelName ) {
	
	PT_LevelObjectLayer* _this = (PT_LevelObjectLayer*)malloc(sizeof(PT_LevelObjectLayer));
	if ( !_this )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_LevelObjectCreate: Not enough memory\n");
		return NULL;
	}
	SDL_memset(_this, 0, sizeof(PT_LevelObjectLayer));
	
	_this->nameID = PT_StringCreate();
	if ( !_this->nameID )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_LevelObjectCreate!\n");
		PT_LevelObjectLayerDestroy(_this);
		return NULL;
	}
	else {
		if ( !PT_StringInsert(&_this->nameID, utf8_levelName, 0) )
		{
			SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_LevelObjectCreate!\n");
			PT_LevelObjectLayerDestroy(_this);
			return NULL;
		}
	}
	
	_this->pLayer = PT_LevelLayerCreate((void*)_this, jsonValue, PT_LevelObjectLayerParse);
	if ( !_this->pLayer )
	{
		PT_LevelObjectLayerDestroy(_this);
		return NULL;
	}
	
	PT_LevelLayerAddUpdateCallback(_this->pLayer, PT_LevelObjectLayerUpdate);
	PT_LevelLayerAddDrawCallback(_this->pLayer, PT_LevelObjectLayerDraw);
	PT_LevelLayerAddDestroyCallback(_this->pLayer, PT_LevelObjectLayerDestroy);
	
	return _this->pLayer;
}//PT_LevelObjectLayerCreate

void PT_LevelObjectLayerDestroy( void* layerData ) {
	if ( !layerData )
	{
		return;
	}
	
	PT_LevelObjectLayer* _this = (PT_LevelObjectLayer*)layerData;
	
	PT_StringDestroy(_this->nameID);
	if ( _this->sprites )
	{
		for ( unsigned int i = 0; i < _this->numSprites; i++ )
		{
			PT_SpriteDestroy(_this->sprites[i]);
		}
		
		free(_this->sprites);
	}
	
	free(_this);
}//PT_LevelObjectLayerDestroy

void PT_LevelObjectLayerUpdate( void* layerData, Sint32 elapsedTime ) {
	PT_LevelObjectLayer* _this = (PT_LevelObjectLayer*)layerData;
	
	for ( unsigned int i = 0; i < _this->numSprites; i++ )
	{
		PT_SpriteUpdate(_this->sprites[i], elapsedTime);
		
		
		unsigned int cameraNumColliders = 0;
		PT_Collider* cameraColliders = NULL;
		
		PT_CameraGetColliders(&cameraColliders, &cameraNumColliders);
		
		for ( unsigned int j = 0; j < cameraNumColliders; j++ )
		{
			PT_CollisionHandlerTestSpriteAgainstCollider(_this->sprites[i], cameraColliders[j]);
		}
	}
}//PT_LevelObjectLayerUpdate

void PT_LevelObjectLayerDraw( void* layerData ) {
	PT_LevelObjectLayer* _this = (PT_LevelObjectLayer*)layerData;
	
	for ( unsigned int i = 0; i < _this->numSprites; i++ )
	{
		PT_SpriteDraw(_this->sprites[i]);
	}
	if ( _this->collisionHandler )
	{
		PT_CollisionHandlerUpdate(_this->collisionHandler);
	}
}//PT_LevelObjectLayerDraw

//===================================== PRIVATE Functions

SDL_bool PT_LevelObjectLayerParse( void* layerData, json_value* jsonValue ) {

	PT_LevelObjectLayer* _this = (PT_LevelObjectLayer*)layerData;
	
	//Default properties.id 
	json_object_entry entry = PT_ParseGetObjectEntry_json_value(jsonValue, "id");
	if ( entry.value )
	{
		int id = entry.value->u.integer;
		char idstr[6];
		sprintf(idstr, "%d", id);
		
		if ( !PT_StringInsert(&_this->nameID, idstr, 0) )
		{
			SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, 
			"PT: PT_LevelObjectLayerParse: Cannot add id to nameID\n");
		}
		else {
			if ( !PT_CollisionManagerAddHandler((char*)_this->nameID->utf8_string, SDL_TRUE) )
			{
				SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, 
				"PT: PT_LevelObjectLayerParse: Cannot add CollisionHandler\n");
			}
		}
	}
	else {
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, 
		"PT: PT_LevelObjectLayerParse: Cannot find element \"id\"\n");
	}
	
	//Custom properties.limit-collision
	entry = PT_ParseGetObjectEntry_json_value(jsonValue, "properties");
	if ( entry.value )
	{
		for ( unsigned int i = 0; i < entry.value->u.array.length; i++ )
		{
			json_value* arrayJsonValue = entry.value->u.array.values[i];
			
			json_object_entry entry2 = PT_ParseGetObjectEntry_json_value(arrayJsonValue, "name");
			if ( entry2.value )
			{
				if ( !strcmp(entry2.value->u.string.ptr, "limit-collision") )
				{
					entry2 = PT_ParseGetObjectEntry_json_value(arrayJsonValue, "value");
					if ( entry2.value->type == json_boolean && entry2.value->u.boolean )
					{
						PT_CollisionManagerSetCurrentHandlerByName((char*)_this->nameID->utf8_string);
						_this->collisionHandler = PT_CollisionManagerGetCurrentHandler();
					}
				}
			}
		}
	}
	
	//Default properties.objects
	entry = PT_ParseGetObjectEntry_json_value(jsonValue, "objects");
	if ( entry.value )
	{
		if ( entry.value->u.array.length > 0 )
		{
			_this->sprites = (PT_Sprite**)malloc(sizeof(PT_Sprite*) * entry.value->u.array.length);
			if ( !_this->sprites )
			{
				SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, 
				"PT: PT_LevelObjectLayerParse: Not enough memory\n");
				return SDL_FALSE;
			}
			_this->numSprites = entry.value->u.array.length;
			
			for ( unsigned int i = 0; i < entry.value->u.array.length; i++ )
			{
				PT_Sprite* sprite = NULL;
				char* spriteType = NULL;
				char* spriteTemplate = NULL;
				
			
				//Object.type
				json_object_entry entry2 = 
				PT_ParseGetObjectEntry_json_value(entry.value->u.array.values[i], "type");
				
				if ( entry2.value )
				{
					if ( entry2.value->u.string.length > 0 )
					{
						spriteType = entry2.value->u.string.ptr;
					}
					else {
						SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, 
						"PT: PT_LevelObjectLayerParse: Object.type empty string!\n");
					}
				}
				
				//Object.properties[]
				entry2 =
				PT_ParseGetObjectEntry_json_value(entry.value->u.array.values[i], "properties");
				
				if ( entry2.value )
				{
					for ( unsigned int j = 0; j < entry2.value->u.array.length; j++ )
					{
						//Object.properties[].name
						json_object_entry entry3 = 
						PT_ParseGetObjectEntry_json_value(entry2.value->u.array.values[j], "name");
						
						if ( entry3.value )
						{
							if ( PT_StringMatchFast(entry3.value->u.string.ptr, "template") )
							{
								entry3 =
								PT_ParseGetObjectEntry_json_value(entry2.value->u.array.values[j], 
									"type");
								
								if ( PT_StringMatchFast(entry3.value->u.string.ptr, "string") )
								{
									entry3 =
									PT_ParseGetObjectEntry_json_value(entry2.value->u.array.values[j], 
									"value");
									
									if ( entry3.value )
									{
										spriteTemplate = entry3.value->u.string.ptr;
										continue;
									}
								}
							}
						}
					}
				}
				else {
					SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, 
					"PT: PT_LevelObjectLayerParse: Object must have a custom property of type string, called \"template\"\n");
				}
				
				sprite = PT_SpriteFactoryGet(spriteTemplate, spriteType);
				if ( !sprite )
				{
					SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, 
					"PT: PT_LevelObjectLayerParse: Cannot properly create sprite!\n");
				}
				
				//Object.x and .y
				entry2 =
				PT_ParseGetObjectEntry_json_value(entry.value->u.array.values[i], "x");
				
				if ( entry2.value )
				{
					if ( entry2.value->type == json_double )
					{
						sprite->dstRect.x = entry2.value->u.dbl;
					}
					else if ( entry2.value->type == json_integer )
					{
						sprite->dstRect.x = entry2.value->u.integer;
					}
				}
				
				entry2 =
				PT_ParseGetObjectEntry_json_value(entry.value->u.array.values[i], "y");
				
				if ( entry2.value )
				{
					if ( entry2.value->type == json_double )
					{
						sprite->dstRect.y = entry2.value->u.dbl;
					}
					else if ( entry2.value->type == json_integer )
					{
						sprite->dstRect.y = entry2.value->u.integer;
					}
				}
				
				_this->sprites[i] = sprite;
			}
		}
	}
	
	return SDL_TRUE;
}//PT_LevelObjectLayerParse



