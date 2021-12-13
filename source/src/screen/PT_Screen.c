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
#include <PT_DynamicImage.h>


struct pt_screen {
	PT_String* name;
	PT_String* fileName;
	PT_Sprite* dynamicImage;
};

static SDL_Rect dst = { 0, 0, 270, 480 };
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
	
	_this->dynamicImage = PT_DynamicImageCreate("sprite-dynamic-image-1");
	
	SDL_Log("===== PT: PT_ScreenCreate =====\n");
	SDL_Log("* Screen: %s: created\n", (char*)_this->name->utf8_string);
	SDL_Log("=====\n");
	
	return _this;
}//PT_ScreenCreate

void PT_ScreenDestroy( PT_Screen* _this ) {
	if ( !_this )
	{
		return;
	}
	
	PT_SpriteDestroy(_this->dynamicImage);
	
	SDL_Log("===== PT: PT_ScreenDestroy =====\n");
	SDL_Log("* Screen: %s: destroyed\n", (char*)_this->name->utf8_string);
	SDL_Log("=====\n");
	
	PT_StringDestroy(_this->name);
	PT_StringDestroy(_this->fileName);
	free(_this);
}//PT_ScreenDestroy

void PT_ScreenUpdate( PT_Screen* _this, Sint32 elapsedTime ) {
	PT_SpriteUpdate(_this->dynamicImage, elapsedTime);

	static float dir = 0.020;
	static float x = 0.f;
	x += elapsedTime * dir;
	
	if ( PT_InputManagerKeyboardGetKeyDown(SDL_SCANCODE_S) )
	{
		PT_SoundManagerPlaySample("jump", 0);
	}
	
	if ( x <= 0 )
	{
		dir = 0.020;
	}
	else if ( x + dst.w >= 640 )
	{
		dir = -0.020;
	}
	
	dst.x = (int)x;
}//PT_ScreenUpdate

void PT_ScreenDraw( PT_Screen* _this ) {
	
	PT_GraphicsDrawTexture("background-proto-test", NULL, NULL, 0.0, NULL, SDL_FLIP_NONE);
	PT_GraphicsDrawTexture("background-proto-test", NULL, &dst, 0.0, NULL, SDL_FLIP_NONE);
	
	PT_SpriteDraw(_this->dynamicImage);
	
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



