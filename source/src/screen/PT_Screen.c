/*
Copyright 2022 daywithstars

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

#include <SDL_log.h>

#include <PT_Screen.h>
#include <PT_ScreenManager.h>
#include <PT_LevelManager.h>
#include <PT_String.h>
#include <PT_Parse.h>
#include <PT_Graphics.h>
#include <PT_InputManager.h>
#include <PT_CollisionManager.h>
#include <PT_SpriteFactory.h>
#include <PT_Camera.h>



struct pt_screen {
	PT_String* name;
	PT_String* fileName;
	
	PT_Sprite** sprites;
	unsigned int numSprites;
	
	PT_CollisionHandler* collisionHandler;
};

SDL_bool PT_ScreenParseSettings( PT_Screen* _this, json_value* jsonValue ) {
	/*
		see the template: games/default-templates/screen-folder/screen-template.json
	*/
	//Search for settings

	if ( jsonValue->type == json_object )
	{
		json_object_entry entry = PT_ParseGetObjectEntry_json_value(jsonValue, "settings name");
		
		//settings->name
		if ( entry.value )
		{
			if ( !PT_StringInsert(&_this->name, entry.value->u.string.ptr, 0) )
			{
				SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_ScreenParseSettings!\n");
			}
		}
		else {
			SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, 
			"PT: PT_ScreenParseSettings: Cannot find the property: settings name\n");
		}
		
		//file
		
		entry = PT_ParseGetObjectEntry_json_value(jsonValue, "file");
		if ( entry.value )
		{
			if ( !PT_StringInsert(&_this->fileName, entry.value->u.string.ptr, 0) )
			{
				SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_ScreenParseSettings!\n");
			}
		}
		else {
			SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, 
			"PT: PT_ScreenParseSettings: Cannot find the property: file\n");
		}
		
		//setup collison handler
		PT_String* collisionHandlerName = PT_StringCreate();
		if ( !collisionHandlerName )
		{
			SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, 
			"PT: PT_ScreenParseSettings!\n");
		}
		else {
			if ( !PT_StringInsert(&collisionHandlerName, (char*)_this->name->utf8_string, 0) )
			{
				SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, 
				"PT: PT_ScreenParseSettings!\n");
				PT_StringDestroy(collisionHandlerName);
			}
			else {
				if ( !PT_StringInsert(&collisionHandlerName, "screen: ", 0) )
				{
					SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, 
					"PT: PT_ScreenParseSettings!\n");
					PT_StringDestroy(collisionHandlerName);
				}
				else {
					if ( 
					!PT_CollisionManagerAddHandler((char*)collisionHandlerName->utf8_string, SDL_FALSE) 
					)
					{
						SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, 
						"PT: PT_ScreenParseSettings!\n");
						PT_StringDestroy(collisionHandlerName);
					}
					else {
						_this->collisionHandler =
						PT_CollisionManagerGetHandler((char*)collisionHandlerName->utf8_string);
						if ( !_this->collisionHandler )
						{
							SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, 
							"PT: PT_ScreenParseSettings: Cannot get collision handler\n");
						}
						
						PT_StringDestroy(collisionHandlerName);
					}
				}
			}
		}
	}
	
	return SDL_TRUE;
}//PT_ScreenParseSettings

SDL_bool PT_ScreenLoadSprites( PT_Screen* _this, json_value* jsonValue ) {
	if ( !_this || !jsonValue )
	{
		return SDL_FALSE;
	}
	
	json_object_entry entry = PT_ParseGetObjectEntry_json_value(jsonValue, "sprites");
	if ( entry.value )
	{
		_this->sprites = (PT_Sprite**)malloc(sizeof(PT_Sprite*) * entry.value->u.array.length);
		if ( !_this->sprites )
		{
			SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_ScreenLoadSprites: Not enough memory\n");
			return SDL_FALSE;
		}
		_this->numSprites = entry.value->u.array.length;
		
		PT_Parse *parse = PT_ParseCreate();
		PT_String* spriteTemplatePath = PT_StringCreate();
		
		PT_CollisionManagerSetCurrentHandler(_this->collisionHandler);
		for ( unsigned int i = 0; i < entry.value->u.array.length; i++ )
		{
			_this->sprites[i] = NULL;
			
			//Reads sprite template to see it's type.
			if ( !PT_StringInsert(&spriteTemplatePath, ".json", 0) )
			{
				SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_ScreenLoadSprites!\n");
			}
			if ( !PT_StringInsert(&spriteTemplatePath, entry.value->u.array.values[i]->u.string.ptr, 0) )
			{
				SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_ScreenLoadSprites!\n");
			}
			if ( !PT_StringInsert(&spriteTemplatePath, "assets/sprite/", 0) )
			{
				SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_ScreenLoadSprites!\n");
			}
			
			if ( !PT_ParseOpenFile(parse, (char*)spriteTemplatePath->utf8_string, SDL_TRUE) )
			{
				SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_ScreenLoadSprites!\n");
			}
			else {
				json_object_entry spriteTemplateEntry = PT_ParseGetObjectEntry(parse, "type");
				if ( spriteTemplateEntry.value )
				{
					_this->sprites[i] = PT_SpriteFactoryGet(
						entry.value->u.array.values[i]->u.string.ptr,
						spriteTemplateEntry.value->u.string.ptr
					);
				}
			}
			
			PT_StringClear(&spriteTemplatePath);
		}
		
		PT_StringDestroy(spriteTemplatePath);
		PT_ParseDestroy(parse);
	}
	
	return SDL_TRUE;
}//PT_ScreenLoadSprites

SDL_bool PT_ScreenLoadLevels( PT_Screen* _this, json_value* jsonValue ) {
	if ( !_this || !jsonValue )
	{
		return SDL_FALSE;
	}
	
	json_object_entry entry = PT_ParseGetObjectEntry_json_value(jsonValue, "levels");
	if ( entry.value )
	{
		for ( unsigned int i = 0; i < entry.value->u.array.length; i++ )
		{
			if ( !PT_LevelManagerLoadLevel(entry.value->u.array.values[i]->u.string.ptr) )
			{
				SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_ScreenLoadLevels!\n");
				return SDL_FALSE;
			}
		}	
	}

	return SDL_TRUE;
}//PT_ScreenLoadLevels

//===================================== PUBLIC Functions

PT_Screen* PT_ScreenCreate( json_value* jsonValue ) {
	PT_Screen* _this = (PT_Screen*)malloc(sizeof(struct pt_screen));
	
	if ( !_this )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_ScreeCreate: Not enough memory\n");
		return NULL;
	}
	SDL_memset(_this, 0, sizeof(struct pt_screen));
	_this->name = PT_StringCreate();
	_this->fileName = PT_StringCreate();
	
	PT_ScreenParseSettings(_this, jsonValue);
	if ( !PT_ScreenLoadSprites(_this, jsonValue) )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_ScreenCreate: Cannot load sprites\n");
	}
	if ( !PT_ScreenLoadLevels(_this, jsonValue) )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_ScreenCreate: Cannot load levels\n");
	}
	
	SDL_Log("===== PT: PT_ScreenCreate =====\n");
	SDL_Log("* Screen: %s: created\n", (char*)_this->name->utf8_string);
	SDL_Log("* --File: %s\n", (char*)_this->fileName->utf8_string);
	SDL_Log("=====\n");
	
	return _this;
}//PT_ScreenCreate

void PT_ScreenDestroy( PT_Screen* _this ) {
	if ( !_this )
	{
		return;
	}
	
	SDL_Log("===== PT: PT_ScreenDestroy =====\n");
	SDL_Log("* Screen: %s: destroyed\n", (char*)_this->name->utf8_string);
	SDL_Log("* --File: %s\n", (char*)_this->fileName->utf8_string);
	SDL_Log("=====\n");
	
	PT_StringDestroy(_this->name);
	PT_StringDestroy(_this->fileName);
	
	if ( _this->sprites )
	{
		for ( unsigned int i = 0; i < _this->numSprites; i++ )
		{
			if ( _this->sprites[i] )
			{
				PT_SpriteDestroy(_this->sprites[i]);
			}
		} 
		
		free(_this->sprites);
	}
	
	free(_this);
}//PT_ScreenDestroy

void PT_ScreenUpdate( PT_Screen* _this, Sint32 elapsedTime ) {
	PT_LevelManagerUpdate(elapsedTime);
	
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
	if ( _this->collisionHandler )
	{
		PT_CollisionHandlerUpdate(_this->collisionHandler);
	}
}//PT_ScreenUpdate

void PT_ScreenDraw( PT_Screen* _this ) {

	PT_LevelManagerDraw();
	
	for ( unsigned int i = 0; i < _this->numSprites; i++ )
	{
		PT_SpriteDraw(_this->sprites[i]);
	}
}//PT_ScreenDraw



