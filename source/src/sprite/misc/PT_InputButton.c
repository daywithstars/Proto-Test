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

#include <PT_InputButton.h>
#include <PT_String.h>
#include <PT_Graphics.h>
#include <PT_InputManager.h>
#include <PT_Mouse.h>
#include <PT_ScreenManager.h>
#include <PT_Parse.h>
#include <PT_ObjectEntryConverter.h>




typedef enum {
	PT_INPUTBUTTON_MOUSE_OUT,
	PT_INPUTBUTTON_MOUSE_OVER,
	PT_INPUTBUTTON_MOUSE_CLICK
}PT_InputButtonMouseEventType;


typedef struct {
	PT_InputButtonMouseEventType type;
	SDL_bool loaded;

	PT_String* button;
	PT_String* changeScreen;
	PT_String* animationMouseOver;
	PT_String* animationMouseOut;
}PT_InputButtonEventDataBase_Mouse;

typedef struct  {
	PT_InputButtonEventDataBase_Mouse mouse;
}PT_InputButtonEventDataBase;





typedef struct {
	PT_InputButtonEventDataBase db;
}PT_InputButtonEvent;

struct pt_input_button {
	PT_String* name;
	PT_InputButtonEvent event;

	PT_Sprite* pSprite;
};



//===================================== PRIVATE Functions

PT_InputButton* PT_InputButtonAlloc( );
void PT_InputButtonAddSpriteCallbacks( PT_Sprite* pSprite );

SDL_bool PT_InputButtonParse( PT_Sprite* sprite, void* _data, json_value* jsonValue );

void PT_InputButtonListenEvent( PT_InputButton* _this );


//===================================== PUBLIC Functions

PT_Sprite* PT_InputButtonCreate( const char* utf8_spriteTemplate ) {

	PT_InputButton* _this = PT_InputButtonAlloc();
	if ( !_this )
	{
		return NULL;
	}
	
	PT_Sprite* sprite = PT_SpriteCreate(utf8_spriteTemplate, (void*)_this, PT_InputButtonParse);
	if ( !sprite )
	{
		PT_InputButtonDestroy(_this);
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, 
		"PT: PT_InputButtonCreate: Cannot create PT_InputButton\n");
		return NULL;
	}
	
	PT_InputButtonAddSpriteCallbacks(sprite);
	return sprite;
}//PT_InputButtonCreate

PT_Sprite* PT_InputButtonCreateFromJsonValue( json_value* jsonValue ) {

	PT_InputButton* _this = PT_InputButtonAlloc();
	if ( !_this )
	{
		return NULL;
	}
	
	PT_Sprite* sprite = PT_SpriteCreateFromJsonValue(jsonValue, (void*)_this, PT_InputButtonParse);
	if ( !sprite )
	{
		PT_InputButtonDestroy(_this);
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, 
		"PT: PT_InputButtonCreate: Cannot create PT_InputButton\n");
		return NULL;
	}
	
	PT_InputButtonAddSpriteCallbacks(sprite);
	return sprite;
}//PT_InputButtonCreateFromJsonValue

PT_Sprite* PT_ScreeenButtonCreateFromStringTemplate( const char* utf8_stringTemplate ) {
	
	PT_InputButton* _this = PT_InputButtonAlloc();
	if ( !_this )
	{
		return NULL;
	}
	
	
	PT_Sprite* sprite = PT_SpriteCreateFromStringTemplate(utf8_stringTemplate, 
		(void*)_this, PT_InputButtonParse);
		
	if ( !sprite )
	{
		PT_InputButtonDestroy(_this);
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, 
		"PT: PT_ScreeenButtonCreateFromStringTemplate: Cannot create PT_InputButton\n");
		return NULL;
	}

	PT_InputButtonAddSpriteCallbacks(sprite);
	return sprite;
}//PT_ScreeenButtonCreateFromStringTemplate

void PT_InputButtonDestroy( void* _data ) {
	if ( !_data )
	{
		return;
	}

	PT_InputButton* _this = (PT_InputButton*)_data;
	
	PT_StringDestroy(_this->name);
	
	if ( _this->event.db.mouse.loaded )
	{
		PT_StringDestroy(_this->event.db.mouse.changeScreen);
		PT_StringDestroy(_this->event.db.mouse.animationMouseOver);
		PT_StringDestroy(_this->event.db.mouse.animationMouseOut);
		PT_StringDestroy(_this->event.db.mouse.button);
	}
	
	free(_this);
}//PT_InputButtonDestroy

SDL_bool PT_InputButtonGetEventPress( void* _data ) {
	PT_InputButton* _this = (PT_InputButton*)_data;

	if ( _this->event.db.mouse.type == PT_INPUTBUTTON_MOUSE_CLICK )
	{
		return SDL_TRUE;
	}

	return SDL_FALSE;
}//PT_InputButtonGetEventPress

void PT_InputButtonUpdate( void* _data, Sint32 elapsedTime ) {
	PT_InputButton* _this = (PT_InputButton*)_data;
	
	PT_InputButtonListenEvent(_this);
	
	if ( _this->event.db.mouse.loaded )
	{
		if ( _this->event.db.mouse.type == PT_INPUTBUTTON_MOUSE_OVER )
		{
			//change animation
			if ( _this->event.db.mouse.animationMouseOver )
			{
				PT_SpriteChangeAnimation((void*)_this->pSprite, 
				(char*) _this->event.db.mouse.animationMouseOver->utf8_string);
			}
		}
		else if ( _this->event.db.mouse.type == PT_INPUTBUTTON_MOUSE_OUT )
		{
			//change animation
			if ( _this->event.db.mouse.animationMouseOut )
			{
				PT_SpriteChangeAnimation((void*)_this->pSprite, 
				(char*) _this->event.db.mouse.animationMouseOut->utf8_string);
			}
		}
	}

}//PT_InputButtonUpdate

void PT_InputButtonDraw( void* _data ) {
	PT_InputButton* _this = (PT_InputButton*)_data;

	if ( _this->pSprite->imageName == NULL )
	{
		if ( _this->event.db.mouse.type == PT_INPUTBUTTON_MOUSE_OVER )
		{
			PT_GraphicsSetRenderDrawColor((SDL_Color){0, 120, 230, 0XFF});
		}
		else {
			PT_GraphicsSetRenderDrawColor((SDL_Color){0, 0, 230, 0XFF});
		}
		
		
		const SDL_FRect rect = { 
			_this->pSprite->dstRect.x,
			_this->pSprite->dstRect.y,
			48,
			32
		};
		
		PT_GraphicsRenderFillRectF(&rect);
		
		PT_GraphicsDrawFontTexture(
			(char*)_this->name->utf8_string, NULL, 
			_this->pSprite->dstRect.x + 4, _this->pSprite->dstRect.y + 6,
			0.0, NULL, SDL_FLIP_NONE
		);
	}
}//PT_InputButtonDraw


//===================================== PRIVATE Functions

PT_InputButton* PT_InputButtonAlloc( ) {

	PT_InputButton* _this = (PT_InputButton*)malloc(sizeof(struct pt_input_button));
	if ( !_this )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_InputButtonAlloc: Not enough memory\n");
		return NULL;
	}
	SDL_memset(_this, 0, sizeof(struct pt_input_button));
	
	return _this;
}//PT_InputButtonAlloc

void PT_InputButtonAddSpriteCallbacks( PT_Sprite* pSprite ) {
	if ( !pSprite )
	{
		return;
	}
	
	PT_SpriteAddUpdateCallback(pSprite, PT_InputButtonUpdate);
	PT_SpriteAddDrawCallback(pSprite, PT_InputButtonDraw);
	PT_SpriteAddDestroyCallback(pSprite, PT_InputButtonDestroy);
}//PT_InputButtonAddSpriteCallbacks

SDL_bool PT_InputButtonParse( PT_Sprite* sprite, void* _data, json_value* jsonValue ) {
	/*
		see the template: games/default-templates/sprite-folder/sprite-template.json
	*/
	PT_InputButton* _this = (PT_InputButton*)_data;
	_this->pSprite = sprite;
	
	
	/*
		"misc": {
			"button-name": "[name]",
	*/
	json_object_entry entry = PT_ParseGetObjectEntry_json_value(jsonValue, "misc button-name");
	if ( !entry.value )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, 
		"PT: PT_InputButtonParse: Unable to find \"misc button-name\" element\n");
		return SDL_FALSE;
	}
	
	_this->name = PT_ObjectEntryConverter_PT_String(entry);
	if ( !_this->name )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_InputButtonParse!\n");
		return SDL_FALSE;
	}
	PT_GraphicsRenderTextSolid("Rowdies-14", "Select", (char*)_this->name->utf8_string);
	
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
			SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_InputButtonParse!\n");
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
			SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_InputButtonParse!\n");
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
			SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_InputButtonParse!\n");
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
			SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_InputButtonParse!\n");
			return SDL_FALSE;
		}
		_this->event.db.mouse.loaded = SDL_TRUE;
	}


	return SDL_TRUE;
}//PT_InputButtonParse

void PT_InputButtonListenEvent( PT_InputButton* _this ) {
	
	if ( _this->event.db.mouse.loaded )
	{	
		Sint32 mx, my;
		PT_InputManagerMouseGetPosition(&mx, &my);
		
		SDL_Rect mouseRect = PT_InputManagerMouseGetRect();
		mouseRect.x += mx;
		mouseRect.y += my;
		
		_this->event.db.mouse.type = PT_INPUTBUTTON_MOUSE_OUT;
		
		for ( unsigned int i = 0; i < _this->pSprite->numColliders; i++ )
		{
			if ( PT_ColliderTestCollisionAgainstRectangle(_this->pSprite->colliders[i],  
				_this->pSprite->dstRect.x, _this->pSprite->dstRect.y, mouseRect) )
			{
				_this->event.db.mouse.type = PT_INPUTBUTTON_MOUSE_OVER;
				
				
				if ( PT_InputManagerMouseGetButtonDown(
					PT_MouseGetButtonByString((char*)_this->event.db.mouse.button->utf8_string)) )
				{
					_this->event.db.mouse.type = PT_INPUTBUTTON_MOUSE_CLICK;
					break;
				}
			}
		}
	}
	
}//PT_InputButtonListenEvent






