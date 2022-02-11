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

#include <PT_Text.h>
#include <PT_Graphics.h>
#include <PT_Parse.h>


//===================================== PRIVATE Functions

SDL_bool PT_TextParse( PT_Text* _this, json_value* jsonValue );

//===================================== PUBLIC Functions

PT_Text* PT_TextCreate( json_value* jsonValue ) {
	PT_Text* _this = (PT_Text*)malloc(sizeof(PT_Text));
	if ( !_this )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_TextCreate: Not enough memory\n");
		return NULL;
	}
	SDL_memset(_this, 0, sizeof(PT_Text));
	
	_this->font = PT_StringCreate();
	if ( !_this->font )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_TextCreate!\n");
		PT_TextDestroy(_this);
		return NULL;
	}
	_this->textureName = PT_StringCreate();
	if ( !_this->textureName )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_TextCreate!\n");
		PT_TextDestroy(_this);
		return NULL;
	}
	_this->text = PT_StringCreate();
	if ( !_this->text )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_TextCreate!\n");
		PT_TextDestroy(_this);
		return NULL;
	}
	
	if ( !PT_TextParse(_this, jsonValue) )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_TextCreate!\n");
		PT_TextDestroy(_this);
		return NULL;
	}
	
	return _this;
}//PT_TextCreate

void PT_TextDestroy( PT_Text* _this ) {
	if ( !_this )
	{
		return;
	}
	
	PT_StringDestroy(_this->font);
	PT_StringDestroy(_this->textureName);
	PT_StringDestroy(_this->text);
	
	free(_this);
}//PT_TextDestroy

void PT_TextUpdateTexture( PT_Text* _this ) {
	if ( !_this )
	{
		return;
	}
	
	PT_GraphicsRenderTextSolid(
		(char*)_this->font->utf8_string, 
		(char*)_this->text->utf8_string,
		(char*)_this->textureName->utf8_string
	);
		
}//PT_TextUpdateTexture

void PT_TextDraw( PT_Text* _this ) {

	PT_GraphicsDrawFontTexture((char*)_this->textureName->utf8_string, NULL, _this->x, _this->y,
		0.0, NULL, SDL_FLIP_NONE);
		
}//PT_TextDraw


//===================================== PRIVATE Functions

SDL_bool PT_TextParse( PT_Text* _this, json_value* jsonValue ) {

	json_object_entry entry = PT_ParseGetObjectEntry_json_value(jsonValue, "font");
	if ( !entry.name )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_TextParse: Cannot find element: \"font\"\n");
		return SDL_FALSE;		
	}
	if ( !PT_StringInsert(&_this->font, entry.value->u.string.ptr, 0) )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_TextParse!\n");
		return SDL_FALSE;	
	}
	
	entry = PT_ParseGetObjectEntry_json_value(jsonValue, "texture-name");
	if ( !entry.name )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, 
		"PT: PT_TextParse: Cannot find element: \"texture-name\"\n");
		return SDL_FALSE;		
	}
	if ( !PT_StringInsert(&_this->textureName, entry.value->u.string.ptr, 0) )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_TextParse!\n");
		return SDL_FALSE;	
	}
	
	entry = PT_ParseGetObjectEntry_json_value(jsonValue, "position");
	if ( entry.name )
	{
		_this->x = entry.value->u.array.values[0]->u.integer;	
		_this->y = entry.value->u.array.values[1]->u.integer;	
	}
	
	entry = PT_ParseGetObjectEntry_json_value(jsonValue, "text");
	if ( !entry.name )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, 
		"PT: PT_TextParse: Cannot find element: \"text\"\n");
		return SDL_FALSE;		
	}
	if ( !PT_StringInsert(&_this->text, entry.value->u.string.ptr, 0) )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_TextParse!\n");
		return SDL_FALSE;	
	}
	

	PT_TextUpdateTexture(_this);
	return SDL_TRUE;
}//PT_TextParse





