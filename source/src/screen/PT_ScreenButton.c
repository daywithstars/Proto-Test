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
#include <PT_Graphics.h>
#include <PT_InputManager.h>



struct pt_screen_button {
	int something;//tmp
};



//===================================== PRIVATE Functions

SDL_bool PT_ScreenButtonParse( PT_Sprite* sprite, void* _data, json_value* jsonValue );


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
		free(_this);
		return NULL;
	}
	
	
	//add callbacks to sprite
	PT_SpriteAddCollisionCallback(sprite, PT_ScreenButtonCollisionWith);
	PT_SpriteAddUpdateCallback(sprite, PT_ScreenButtonUpdate);
	PT_SpriteAddDrawCallback(sprite, PT_ScreenButtonDraw);
	PT_SpriteAddDestroyCallback(sprite, PT_ScreenButtonDestroy);
	
	return sprite;
}//PT_ScreenButtonCreate

void PT_ScreenButtonDestroy( void* _data ) {
	if ( !_data )
	{
		return;
	}

	PT_ScreenButton* _this = (PT_ScreenButton*)_data;
	
	free(_this);
}//PT_ScreenButtonDestroy

void PT_ScreenButtonCollisionWith( void* _data, PT_Collider own, PT_Collider target ) {
	PT_ScreenButton* _this = (PT_ScreenButton*)_data;
}//PT_ScreenButtonCollisionWith

void PT_ScreenButtonUpdate( void* _data, Sint32 elapsedTime ) {
	PT_ScreenButton* _this = (PT_ScreenButton*)_data;
	

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




	return SDL_TRUE;
}//PT_ScreenButtonParse




