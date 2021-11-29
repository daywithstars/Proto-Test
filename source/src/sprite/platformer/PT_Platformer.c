/*
Copyright 2021 daywithstars

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

#include <PT_Platformer.h>
#include <PT_Graphics.h>
#include <PT_InputManager.h>



struct pt_platformer {
	double gravity;
};



//===================================== PRIVATE Functions

SDL_bool PT_PlatformerParse( void* _data, json_value* jsonValue );


//===================================== PUBLIC Functions

PT_Sprite* PT_PlatformerCreate( const char* utf8_spriteTemplate ) {

	PT_Platformer* _this = (PT_Platformer*)malloc(sizeof(struct pt_platformer));
	if ( !_this )
	{
		return NULL;
	}
	SDL_memset(_this, 0, sizeof(struct pt_platformer));
	
	PT_Sprite* sprite = PT_SpriteCreate(utf8_spriteTemplate, (void*)_this, PT_PlatformerParse);
	if ( !sprite )
	{
		free(_this);
		return NULL;
	}
	
	
	//add callbacks to sprite
	PT_SpriteAddUpdateCallback(sprite, PT_PlatformerUpdate);
	PT_SpriteAddDrawCallback(sprite, PT_PlatformerDraw);
	PT_SpriteAddDestroyCallback(sprite, PT_PlatformerDestroy);
	
	return sprite;
}//PT_PlatformerCreate

void PT_PlatformerDestroy( void* _data ) {
	if ( !_data )
	{
		return;
	}

	PT_Platformer* _this = (PT_Platformer*)_data;
	
	free(_this);
}//PT_PlatformerDestroy

void PT_PlatformerUpdate( void* _data, Sint32 elapsedTime ) {
	PT_Platformer* _this = (PT_Platformer*)_data;
	

}//PT_PlatformerUpdate

void PT_PlatformerDraw( void* _data ) {
	PT_Platformer* _this = (PT_Platformer*)_data;

}//PT_PlatformerDraw


//===================================== PRIVATE Functions

SDL_bool PT_PlatformerParse( void* _data, json_value* jsonValue ) {
	/*
		see the template: games/default-templates/sprite-folder/sprite-template.json
	*/
	PT_Platformer* _this = (PT_Platformer*)_data;




	return SDL_TRUE;
}//PT_PlatformerParse



