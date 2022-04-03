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

#include <PT_ScreenButton.h>
#include <PT_String.h>
#include <PT_Graphics.h>
#include <PT_InputManager.h>
#include <PT_Mouse.h>
#include <PT_ScreenManager.h>
#include <PT_Parse.h>
#include <PT_ObjectEntryConverter.h>




typedef enum {
	PT_SCREENBUTTON_MOUSE_OUT,
	PT_SCREENBUTTON_MOUSE_OVER,
	PT_SCREENBUTTON_MOUSE_CLICK
}PT_ScreenButtonMouseEventType;


typedef struct {
	PT_ScreenButtonMouseEventType type;
	SDL_bool loaded;

	PT_String* button;
	PT_String* changeScreen;
	PT_String* animationMouseOver;
	PT_String* animationMouseOut;
}PT_ScreenButtonEventDataBase_Mouse;

typedef struct  {
	PT_ScreenButtonEventDataBase_Mouse mouse;
}PT_ScreenButtonEventDataBase;





typedef struct {
	PT_ScreenButtonEventDataBase db;
}PT_ScreenButtonEvent;


struct pt_screen_button {
	PT_String* name;
	PT_ScreenButtonEvent event;

	PT_Sprite* pSprite;
};



//===================================== PRIVATE Functions

SDL_bool PT_ScreenButtonParse( PT_Sprite* sprite, void* _data, json_value* jsonValue );

void PT_ScreenButtonListenEvent( PT_ScreenButton* _this );


//===================================== PUBLIC Functions

PT_Sprite* PT_ScreenButtonCreate( const char* utf8_spriteTemplate ) {

	PT_ScreenButton* _this = (PT_ScreenButton*)malloc(sizeof(struct pt_screen_button));
	if ( !_this )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_ScreenButtonCreate: Not enough memory\n");
		return NULL;
	}
	SDL_memset(_this, 0, sizeof(struct pt_screen_button));
	
	PT_Sprite* sprite = PT_SpriteCreate(utf8_spriteTemplate, (void*)_this, PT_ScreenButtonParse);
	if ( !sprite )
	{
		PT_ScreenButtonDestroy(_this);
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, 
		"PT: PT_ScreenButtonCreate: Cannot create PT_ScreenButton\n");
		return NULL;
	}
	
	
	//add callbacks to sprite
	PT_SpriteAddUpdateCallback(sprite, PT_ScreenButtonUpdate);
	PT_SpriteAddDrawCallback(sprite, PT_ScreenButtonDraw);
	PT_SpriteAddDestroyCallback(sprite, PT_ScreenButtonDestroy);
	
	return sprite;
}//PT_ScreenButtonCreate

PT_Sprite* PT_ScreeenButtonCreateFromStringTemplate( const char* utf8_stringTemplate ) {
	
	PT_ScreenButton* _this = (PT_ScreenButton*)malloc(sizeof(struct pt_screen_button));
	if ( !_this )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, 
		"PT: PT_ScreeenButtonCreateFromStringTemplate: Not enough memory\n");
		return NULL;
	}
	SDL_memset(_this, 0, sizeof(struct pt_screen_button));
	
	PT_Sprite* sprite = PT_SpriteCreateFromStringTemplate(utf8_stringTemplate, 
		(void*)_this, PT_ScreenButtonParse);
	if ( !sprite )
	{
		PT_ScreenButtonDestroy(_this);
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, 
		"PT: PT_ScreeenButtonCreateFromStringTemplate: Cannot create PT_ScreenButton\n");
		return NULL;
	}
	
	
	//add callbacks to sprite
	PT_SpriteAddUpdateCallback(sprite, PT_ScreenButtonUpdate);
	PT_SpriteAddDrawCallback(sprite, PT_ScreenButtonDraw);
	PT_SpriteAddDestroyCallback(sprite, PT_ScreenButtonDestroy);
	
	return sprite;
}//PT_ScreeenButtonCreateFromStringTemplate

void PT_ScreenButtonDestroy( void* _data ) {
	if ( !_data )
	{
		return;
	}

	PT_ScreenButton* _this = (PT_ScreenButton*)_data;
	
	PT_StringDestroy(_this->name);
	
	if ( _this->event.db.mouse.loaded )
	{
		PT_StringDestroy(_this->event.db.mouse.changeScreen);
		PT_StringDestroy(_this->event.db.mouse.animationMouseOver);
		PT_StringDestroy(_this->event.db.mouse.animationMouseOut);
		PT_StringDestroy(_this->event.db.mouse.button);
	}
	
	free(_this);
}//PT_ScreenButtonDestroy

void PT_ScreenButtonUpdate( void* _data, Sint32 elapsedTime ) {
	PT_ScreenButton* _this = (PT_ScreenButton*)_data;
	
	PT_ScreenButtonListenEvent(_this);
	
	if ( _this->event.db.mouse.loaded )
	{
		//change animation
		if ( _this->event.db.mouse.type == PT_SCREENBUTTON_MOUSE_OVER )
		{
			if ( _this->event.db.mouse.animationMouseOver )
			{
				
				PT_SpriteChangeAnimation((void*)_this->pSprite, 
				(char*) _this->event.db.mouse.animationMouseOver->utf8_string);
			}
		}
		else if ( _this->event.db.mouse.type == PT_SCREENBUTTON_MOUSE_OUT )
		{
			if ( _this->event.db.mouse.animationMouseOut )
			{
				PT_SpriteChangeAnimation((void*)_this->pSprite, 
				(char*) _this->event.db.mouse.animationMouseOut->utf8_string);
			}
		}
		
		
		/*
		if ( _this->event.type & PT_SCREENBUTTON_EVENT_MOUSE_CLICK )
		{
			if ( _this->event.mouse.changeScreen )
			{
				//Change screen
				if ( PT_ScreenManagerLoadScreen((char*)_this->event.mouse.changeScreen->utf8_string) )
				{
					PT_ScreenManagerSetFirstScreen((char*)_this->event.mouse.changeScreen->utf8_string);
				}
			}
		}*/
	}

}//PT_ScreenButtonUpdate

void PT_ScreenButtonDraw( void* _data ) {
	PT_ScreenButton* _this = (PT_ScreenButton*)_data;

}//PT_ScreenButtonDraw


//===================================== PRIVATE Functions

SDL_bool PT_ScreenButtonParse( PT_Sprite* sprite, void* _data, json_value* jsonValue ) {
	/*
		see the template: games/default-templates/sprite-folder/sprite-template.json
	*/
	PT_ScreenButton* _this = (PT_ScreenButton*)_data;
	_this->pSprite = sprite;
	
	
	/*
		"misc": {
			"button-name": "[name]",
	*/
	json_object_entry entry = PT_ParseGetObjectEntry_json_value(jsonValue, "misc button-name");
	if ( !entry.value )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, 
		"PT: PT_ScreenButtonParse: Unable to find \"misc button-name\" element\n");
		return SDL_FALSE;
	}
	
	_this->name = PT_ObjectEntryConverter_PT_String(entry);
	if ( !_this->name )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_ScreenButtonParse!\n");
		return SDL_FALSE;
	}
	
	/*
		"actions": {
			"mouse-over": {
				"animation": "[anim-name]",
			},
	*/
	entry = PT_ParseGetObjectEntry_json_value(jsonValue, "misc actions mouse-over animation");
	if ( entry.value )
	{
		_this->event.db.mouse.animationMouseOver = PT_ObjectEntryConverter_PT_String(entry);
		if ( !_this->event.db.mouse.animationMouseOver )
		{
			SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_ScreenButtonParse!\n");
			return SDL_FALSE;
		}
		_this->event.db.mouse.loaded = SDL_TRUE;
	}
	
	/*
		"actions": {
			"mouse-out": {
				"animation": "[anim-name]",
			},
	*/
	entry = PT_ParseGetObjectEntry_json_value(jsonValue, "misc actions mouse-out animation");
	if ( entry.value )
	{
		_this->event.db.mouse.animationMouseOut = PT_ObjectEntryConverter_PT_String(entry);
		if ( !_this->event.db.mouse.animationMouseOut )
		{
			SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_ScreenButtonParse!\n");
			return SDL_FALSE;
		}
		_this->event.db.mouse.loaded = SDL_TRUE;
	}
	
	/*
		"actions": {
			"mouse-click": {
				"change-screen": "[screen-name]",
			},
	*/
	entry = PT_ParseGetObjectEntry_json_value(jsonValue, "misc actions mouse-click change-screen");
	if ( entry.value )
	{
		_this->event.db.mouse.changeScreen = PT_ObjectEntryConverter_PT_String(entry);
		if ( !_this->event.db.mouse.changeScreen )
		{
			SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_ScreenButtonParse!\n");
			return SDL_FALSE;
		}
		_this->event.db.mouse.loaded = SDL_TRUE;
	}
	
	/*
		"actions": {
			"mouse-click": {
				"button": "[button-name]",
			},
	*/
	entry = PT_ParseGetObjectEntry_json_value(jsonValue, "misc actions mouse-click button");
	if ( entry.value )
	{
		_this->event.db.mouse.button = PT_ObjectEntryConverter_PT_String(entry);
		if ( !_this->event.db.mouse.button )
		{
			SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_ScreenButtonParse!\n");
			return SDL_FALSE;
		}
		_this->event.db.mouse.loaded = SDL_TRUE;
	}


	return SDL_TRUE;
}//PT_ScreenButtonParse

void PT_ScreenButtonListenEvent( PT_ScreenButton* _this ) {
	
	if ( _this->event.db.mouse.loaded )
	{	
		Sint32 mx, my;
		PT_InputManagerMouseGetPosition(&mx, &my);
		
		SDL_Rect mouseRect = PT_InputManagerMouseGetRect();
		mouseRect.x += mx;
		mouseRect.y += my;
		
		_this->event.db.mouse.type = PT_SCREENBUTTON_MOUSE_OUT;
		
		for ( unsigned int i = 0; i < _this->pSprite->numColliders; i++ )
		{
			if ( PT_ColliderTestCollisionAgainstRectangle(_this->pSprite->colliders[i],  
				_this->pSprite->dstRect.x, _this->pSprite->dstRect.y, mouseRect) )
			{
				_this->event.db.mouse.type = PT_SCREENBUTTON_MOUSE_OVER;
				
				
				if ( PT_InputManagerMouseGetButtonDown(
					PT_MouseGetButtonByString((char*)_this->event.db.mouse.button->utf8_string)) )
				{
					_this->event.db.mouse.type = PT_SCREENBUTTON_MOUSE_CLICK;
				}
			}
		}
	}
	
}//PT_ScreenButtonListenEvent






