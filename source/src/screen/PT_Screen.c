/*
Copyright 2021 daywithstars

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
#include <PT_SoundManager.h>
#include <PT_String.h>
#include <PT_Parse.h>
#include <PT_Graphics.h>
#include <PT_InputManager.h>
#include <PT_SpriteFabric.h>


struct pt_screen {
	PT_String* name;
	PT_String* fileName;
	
	PT_Sprite** sprites;
	unsigned int numSprites;
};


Uint8 colorTar = 1;
Uint8 colorValue = 0;

SDL_bool PT_ScreenParseSettings( PT_Screen* _this, json_value* jsonValue ) {
	/*
		see the template: games/default-templates/screen-folder/screen-template.json
	*/
	//Search for settings

	if ( jsonValue->type == json_object )
	{
		json_object_entry entry = PT_ParseGetObjectEntry_json_value(jsonValue, "settings name");
		
		//settings->name
		if ( entry.name )
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
		entry.name = NULL;
		
		entry = PT_ParseGetObjectEntry_json_value(jsonValue, "file");
		if ( entry.name )
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
	}
	
	return SDL_TRUE;
}//PT_ScreenParseSettings

SDL_bool PT_ScreenLoadSprites( PT_Screen* _this, json_value* jsonValue ) {
	if ( !_this || !jsonValue )
	{
		return SDL_FALSE;
	}
	
	json_object_entry entry = PT_ParseGetObjectEntry_json_value(jsonValue, "sprites");
	if ( entry.name )
	{
		_this->sprites = (PT_Sprite**)malloc(sizeof(PT_Sprite*) * entry.value->u.array.length);
		_this->numSprites = entry.value->u.array.length;
		
		PT_Parse *parse = PT_ParseCreate();
		PT_String* spriteTemplatePath = PT_StringCreate();
		
		for ( unsigned int i = 0; i < entry.value->u.array.length; i++ )
		{
			_this->sprites[i] = NULL;
			
			//Reads sprite template to see it's type.
			PT_StringInsert(&spriteTemplatePath, ".json", 0);
			PT_StringInsert(&spriteTemplatePath, entry.value->u.array.values[i]->u.string.ptr, 0);
			PT_StringInsert(&spriteTemplatePath, "assets/sprite/", 0);
			
			if ( !PT_ParseOpenFile(parse, (char*)spriteTemplatePath->utf8_string, SDL_TRUE) )
			{
				SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_ScreenLoadSprites!\n");
			}
			else {
				json_object_entry spriteTemplateEntry = PT_ParseGetObjectEntry(parse, "type");
				if ( spriteTemplateEntry.name )
				{
					_this->sprites[i] = PT_SpriteFabricGet(
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
	
	for ( unsigned int i = 0; i < _this->numSprites; i++ )
	{
		PT_SpriteUpdate(_this->sprites[i], elapsedTime);
	}
	if ( PT_InputManagerKeyboardGetKeyDown(SDL_SCANCODE_S) )
	{
		PT_SoundManagerPlaySample("jump", 0);
	}
}//PT_ScreenUpdate

void PT_ScreenDraw( PT_Screen* _this ) {
	
	for ( unsigned int i = 0; i < _this->numSprites; i++ )
	{
		PT_SpriteDraw(_this->sprites[i]);
	}
	
	if ( PT_InputManagerKeyboardGetKeyDown(SDL_SCANCODE_R) )
	{
		colorTar = 1;
	}
	if ( PT_InputManagerKeyboardGetKeyDown(SDL_SCANCODE_G) )
	{
		colorTar = 2;
	}
	if ( PT_InputManagerKeyboardGetKeyDown(SDL_SCANCODE_B) )
	{
		colorTar = 3;
	}
	if ( PT_InputManagerKeyboardGetKeyDown(SDL_SCANCODE_A) )
	{
		colorTar = 4;
	}
	if ( PT_InputManagerKeyboardGetKeyDown(SDL_SCANCODE_D) )
	{
		colorTar = 5;
	}
	
	static SDL_Color color = { 0, 0, 0, 100 };
	static Uint8 gDir = 1;
	PT_GraphicsSetRenderDrawColor(color);
	PT_GraphicsRenderFillRect(NULL);
	
	switch ( colorTar )
	{
		case 1:
		color.r = colorValue;
		break;
		
		case 2:
		color.g = colorValue;
		break;
		
		case 3:
		color.b = colorValue;
		break;
		
		case 4:
		color.a = colorValue;
		break;
		
		case 5:
		color.r = 0;
		color.g = 0;
		color.b = 0;
		color.a = 100;
		break;
	}
	
	colorValue += gDir;
	if ( colorValue > 200 )
	{
		gDir = -1;
	}
	else if ( colorValue <= 0 )
	{
		gDir = 1;
	}
}//PT_ScreenDraw



