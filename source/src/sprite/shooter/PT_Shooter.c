/*
Copyright 2021 daywithstars

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

#include <PT_Shooter.h>
#include <PT_Graphics.h>
#include <PT_InputManager.h>



struct pt_shooter {
	int something;//tmp
};



//===================================== PRIVATE Functions

SDL_bool PT_ShooterParse( void* _data, json_value* jsonValue );


//===================================== PUBLIC Functions

PT_Sprite* PT_ShooterCreate( const char* utf8_spriteTemplate ) {

	PT_Shooter* _this = (PT_Shooter*)malloc(sizeof(struct pt_shooter));
	if ( !_this )
	{
		return NULL;
	}
	SDL_memset(_this, 0, sizeof(struct pt_shooter));
	
	PT_Sprite* sprite = PT_SpriteCreate(utf8_spriteTemplate, (void*)_this, PT_ShooterParse);
	if ( !sprite )
	{
		free(_this);
		return NULL;
	}
	
	
	//add callbacks to sprite
	PT_SpriteAddUpdateCallback(sprite, PT_ShooterUpdate);
	PT_SpriteAddDrawCallback(sprite, PT_ShooterDraw);
	PT_SpriteAddDestroyCallback(sprite, PT_ShooterDestroy);
	
	return sprite;
}//PT_ShooterCreate

void PT_ShooterDestroy( void* _data ) {
	if ( !_data )
	{
		return;
	}

	PT_Shooter* _this = (PT_Shooter*)_data;
	
	free(_this);
}//PT_ShooterDestroy

void PT_ShooterUpdate( void* _data, Sint32 elapsedTime ) {
	PT_Shooter* _this = (PT_Shooter*)_data;
	

}//PT_ShooterUpdate

void PT_ShooterDraw( void* _data ) {
	PT_Shooter* _this = (PT_Shooter*)_data;

}//PT_ShooterDraw


//===================================== PRIVATE Functions

SDL_bool PT_ShooterParse( void* _data, json_value* jsonValue ) {
	/*
		see the template: games/default-templates/sprite-folder/sprite-template.json
	*/
	PT_Shooter* _this = (PT_Shooter*)_data;




	return SDL_TRUE;
}//PT_ShooterParse



