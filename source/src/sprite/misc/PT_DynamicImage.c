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

#include <SDL.h>

#include <PT_DynamicImage.h>
#include <PT_InputManager.h>
#include <PT_String.h>
#include <PT_Graphics.h>


struct pt_dynamic_image {
	int tmp;
};

//===================================== PRIVATE Functions

SDL_bool PT_DynamicImageParse( PT_Sprite* sprite, void* _data, json_value* jsonValue );


PT_Sprite* PT_DynamicImageCreate( const char* utf8_spriteTemplate ) {
	
	PT_DynamicImage* _this = (PT_DynamicImage*)malloc(sizeof(struct pt_dynamic_image));
	if ( !_this )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_DynamicImageCreate: Not enough memory\n");
		return NULL;
	}
	SDL_memset(_this, 0, sizeof(struct pt_dynamic_image));
	
	PT_Sprite* sprite = PT_SpriteCreate(utf8_spriteTemplate, (void*)_this, PT_DynamicImageParse);
	if ( !sprite )
	{
		free(_this);
		return NULL;
	}
	
	//add callbacks to sprite
	PT_SpriteAddDestroyCallback(sprite, PT_DynamicImageDestroy);
	PT_SpriteAddUpdateCallback(sprite, PT_DynamicImageUpdate);
	PT_SpriteAddDrawCallback(sprite, PT_DynamicImageDraw);
	
	return sprite;
}//PT_DynamicImageCreate

void PT_DynamicImageDestroy( void* _data ) {
	if ( !_data )
	{
		return;
	}

	PT_DynamicImage* _this = (PT_DynamicImage*)_data;
	
	free(_this);
}//PT_DynamicImageDestroy

void PT_DynamicImageUpdate( void* _data, Sint32 elapsedTime ) {
	PT_DynamicImage* _this = (PT_DynamicImage*)_data;
	
}//PT_DynamicImageUpdate

void PT_DynamicImageDraw( void* _data ) {
	PT_DynamicImage* _this = (PT_DynamicImage*)_data;

}//PT_DynamicImageDraw

//===================================== PRIVATE Functions

SDL_bool PT_DynamicImageParse( PT_Sprite* sprite, void* _data, json_value* jsonValue ) {
	/*
		see the template: games/default-templates/sprite-folder/dynamicImage-template.json
	*/
	PT_DynamicImage* _this = (PT_DynamicImage*)_data;
	
	if ( !sprite->behavior )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_DynamicImageParse: Please set an behavior into template!\n");
		
		return SDL_FALSE;
	}
	
	//prints "image"
	//printf("%s\n", jsonValue->u.object.values[0].name);

	
	
	return SDL_TRUE;
}//PT_DynamicImageParse



