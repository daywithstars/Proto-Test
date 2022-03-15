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

#include <PT_Font.h>
#include <PT_Parse.h>
#include <PT_String.h>
#include <PT_Graphics.h>

//===================================== PRIVATE Functions 

SDL_bool PT_FontParse( PT_Font* _this, json_value* jsonValue );

//===================================== PUBLIC Functions

PT_Font* PT_FontCreate( json_value* jsonValue ) {
	PT_Font* _this = (PT_Font*)malloc(sizeof(PT_Font));
	if ( !_this )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_Font: Not enough memory\n");
		return NULL;
	}
	SDL_memset(_this, 0, sizeof(PT_Font));
	
	if ( !PT_FontParse(_this, jsonValue) )
	{
		PT_FontDestroy(_this);
		return NULL;
	}
	
	return _this;
}//PT_FontCreate

void PT_FontDestroy( PT_Font* _this ) {
	if ( !_this )
	{
		return;
	}
	
	if ( _this->font )
	{
		TTF_CloseFont(_this->font);
	}
	
	free(_this);
}//PT_FontDestroy


//===================================== PRIVATE Functions 

SDL_bool PT_FontParse( PT_Font* _this, json_value* jsonValue ) {
	json_object_entry entry = PT_ParseGetObjectEntry_json_value(jsonValue, "font-data");
	if ( !entry.value )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, 
		"PT: PT_FontParse: Cannot find \"font-data\" on template.\n");
		return SDL_FALSE;	
	}
	
	PT_String* fontPath = PT_StringCreate();
	if ( !fontPath )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_FontParse!\n");
		return SDL_FALSE;
	}
	if ( !PT_StringInsert(&fontPath, entry.value->u.string.ptr, 0) )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_FontParse!\n");
		PT_StringDestroy(fontPath);
		return SDL_FALSE;
	}
	if ( !PT_StringInsert(&fontPath, "assets/font/data/", 0) )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_FontParse!\n");
		PT_StringDestroy(fontPath);
		return SDL_FALSE;
	}
	
	int fontSize = 10;
	
	entry = PT_ParseGetObjectEntry_json_value(jsonValue, "size-px");
	if ( !entry.value )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, 
		"PT: PT_FontParse: Cannot find \"size-px\" on template.\n");
		PT_StringDestroy(fontPath);
		return SDL_FALSE;	
	} 
	fontSize = entry.value->u.integer;
	
	TTF_Font* font = PT_GraphicsLoadFont((char*)fontPath->utf8_string, fontSize, SDL_TRUE);
	if ( !font )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_FontParse!\n");
		PT_StringDestroy(fontPath);
		return SDL_FALSE;
	}
	
	_this->font = font;
	
	PT_StringDestroy(fontPath);
	
	
	entry = PT_ParseGetObjectEntry_json_value(jsonValue, "color");
	if ( entry.value )
	{
		_this->color.r = entry.value->u.array.values[0]->u.integer;
		_this->color.g = entry.value->u.array.values[1]->u.integer;
		_this->color.b = entry.value->u.array.values[2]->u.integer;
		_this->color.a = entry.value->u.array.values[3]->u.integer;
	}
	else {
		_this->color.r = 220;
		_this->color.g = 220;
		_this->color.b = 220;
		_this->color.a = 0xFF;
	}
	
	return SDL_TRUE;
}//PT_FontParse






