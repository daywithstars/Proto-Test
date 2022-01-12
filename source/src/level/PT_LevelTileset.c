/*
Copyright 2022 daywithstars

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

#include <stdio.h>
#include <stdlib.h>

#include <SDL_log.h>

#include <PT_LevelTileset.h>
#include <PT_Parse.h>


//===================================== PRIVATE Functions

SDL_bool PT_LevelTilesetParse( PT_LevelTileset* _this, json_value* jsonValue );

//===================================== PUBLIC Functions

PT_LevelTileset PT_LevelTilesetCreate( json_value* jsonValue ) {
	
	PT_LevelTileset _this;
	SDL_memset(&_this, 0, sizeof(PT_LevelTileset));
	
	if ( !PT_LevelTilesetParse(&_this, jsonValue) )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_LevelTilesetCreate!\n");
	}
	
	return _this;
}
void PT_LevelTilesetDestroy( PT_LevelTileset* _this ) {
	if ( !_this )
	{
		return;
	}
	
	PT_StringDestroy(_this->name);
	PT_StringDestroy(_this->image);
}

//===================================== PRIVATE Functions

SDL_bool PT_LevelTilesetParse( PT_LevelTileset* _this, json_value* jsonValue ) {
	
	//Property: tilesets[].name
	_this->name = PT_StringCreate();
	if ( !_this->name )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_LevelTilesetParse!\n");
		return SDL_FALSE;
	}
	json_object_entry entry = PT_ParseGetObjectEntry_json_value(jsonValue, "name");
	if ( !entry.name )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_LevelTilesetParse!\n");
		return SDL_FALSE;
	}
	if ( !PT_StringInsert(&_this->name, entry.value->u.string.ptr, 0) )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_LevelTilesetParse!\n");
		return SDL_FALSE;
	}
	
	//Property: tilesets[].columns
	entry = PT_ParseGetObjectEntry_json_value(jsonValue, "columns");
	if ( !entry.name )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_LevelTilesetParse!\n");
		return SDL_FALSE;
	}
	_this->columns = entry.value->u.integer;
	
	//Property: tilesets[].firstgid
	entry = PT_ParseGetObjectEntry_json_value(jsonValue, "firstgid");
	if ( !entry.name )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_LevelTilesetParse!\n");
		return SDL_FALSE;
	}
	_this->firstgid = entry.value->u.integer;
	
	//Property: tilesets[].image
	_this->image = PT_StringCreate();
	if ( !_this->image )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_LevelTilesetParse!\n");
		return SDL_FALSE;
	}
	entry = PT_ParseGetObjectEntry_json_value(jsonValue, "image");
	if ( !entry.name )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_LevelTilesetParse!\n");
		return SDL_FALSE;
	}

	Uint64 copyStart = PT_StringGetOccurrencePositionBasicString(entry.value->u.string.ptr, "/", -1);
	Uint64 copyEnd = PT_StringGetOccurrencePositionBasicString(entry.value->u.string.ptr, ".", -1);
	if ( !PT_StringCopyFrom(_this->image, entry.value->u.string.ptr, copyStart, copyEnd - 1, 0) )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_LevelTilesetParse!\n");
		return SDL_FALSE;
	}
	
	//Property: tilesets[].imagewidth
	entry = PT_ParseGetObjectEntry_json_value(jsonValue, "imagewidth");
	if ( !entry.name )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_LevelTilesetParse!\n");
		return SDL_FALSE;
	}
	_this->imagewidth = entry.value->u.integer;
	
	//Property: tilesets[].imageheight
	entry = PT_ParseGetObjectEntry_json_value(jsonValue, "imageheight");
	if ( !entry.name )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_LevelTilesetParse!\n");
		return SDL_FALSE;
	}
	_this->imageheight = entry.value->u.integer;
	
	
	//Property: tilesets[].tilecount
	entry = PT_ParseGetObjectEntry_json_value(jsonValue, "tilecount");
	if ( !entry.name )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_LevelTilesetParse!\n");
		return SDL_FALSE;
	}
	_this->tilecount = entry.value->u.integer;
	
	//Property: tilesets[].tilewidth
	entry = PT_ParseGetObjectEntry_json_value(jsonValue, "tilewidth");
	if ( !entry.name )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_LevelTilesetParse!\n");
		return SDL_FALSE;
	}
	_this->tilewidth = entry.value->u.integer;
	
	//Property: tilesets[].tileheight
	entry = PT_ParseGetObjectEntry_json_value(jsonValue, "tileheight");
	if ( !entry.name )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_LevelTilesetParse!\n");
		return SDL_FALSE;
	}
	_this->tileheight = entry.value->u.integer;

	return SDL_TRUE;
}







