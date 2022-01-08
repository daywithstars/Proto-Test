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

#include <PT_Level.h>
#include <PT_Parse.h>
#include <PT_String.h>
#include <PT_LevelTileLayer.h>




//===================================== PRIVATE Functions

SDL_bool PT_LevelParse( PT_Level* _this, json_value* jsonValue );


//===================================== PUBLIC Functions

PT_Level* PT_LevelCreate( json_value* jsonValue ) {
	PT_Level *_this = (PT_Level*)malloc(sizeof(PT_Level));
	if ( !_this )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_LevelCreate: Not enough memory\n");
		return NULL;
	}
	SDL_memset(_this, 0, sizeof(PT_Level));
	
	if ( !PT_LevelParse(_this, jsonValue) )
	{
		PT_LevelDestroy(_this);
		return NULL;
	}
	
	return _this;
}
void PT_LevelDestroy( PT_Level* _this ) {
	if ( !_this )
	{
		return;
	}
	
	if ( _this->layers )
	{
		for ( unsigned int i = 0; i < _this->numLayers; i++ )
		{
			PT_LevelLayerDestroy(_this->layers[i]);
		}
		free(_this->layers);
	}
	
	
	free(_this);
}

void PT_LevelUpdate( PT_Level* _this, Sint32 elapsedTime ) {
	for ( unsigned int i = 0; i < _this->numLayers; i++ )
	{
		PT_LevelLayerUpdate(_this->layers[i], elapsedTime);
	}
}
void PT_LevelDraw( PT_Level* _this ) {
	for ( unsigned int i = 0; i < _this->numLayers; i++ )
	{
		PT_LevelLayerDraw(_this->layers[i]);
	}
} 

//===================================== PRIVATE Functions

SDL_bool PT_LevelParse( PT_Level* _this, json_value* jsonValue ) {
	
	//Property: map.orientation 
	json_object_entry entry = PT_ParseGetObjectEntry_json_value(jsonValue, "orientation");
	if ( !entry.name )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_LevelParse!\n");
		return SDL_FALSE;
	}
	
	if ( PT_StringMatchFast(entry.value->u.string.ptr, "orthogonal") )
	{
		_this->orientation = PT_LEVEL_ORIENTATION_ORTHOGONAL;
	}
	else {
		_this->orientation = PT_LEVEL_ORIENTATION_NONE;
	}
	
	//Property: map.width 
	entry = PT_ParseGetObjectEntry_json_value(jsonValue, "width");
	if ( !entry.name )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_LevelParse!\n");
		return SDL_FALSE;
	}
	_this->width = entry.value->u.integer;
	
	//Property: map.height
	entry = PT_ParseGetObjectEntry_json_value(jsonValue, "height");
	if ( !entry.name )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_LevelParse!\n");
		return SDL_FALSE;
	}
	_this->height = entry.value->u.integer;
	
	//Property: map.tilewidth
	entry = PT_ParseGetObjectEntry_json_value(jsonValue, "tilewidth");
	if ( !entry.name )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_LevelParse!\n");
		return SDL_FALSE;
	}
	_this->tilewidth = entry.value->u.integer;
	
	//Property: map.tileheight
	entry = PT_ParseGetObjectEntry_json_value(jsonValue, "tileheight");
	if ( !entry.name )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_LevelParse!\n");
		return SDL_FALSE;
	}
	_this->tileheight = entry.value->u.integer;
	
	//Property: map.layers
	entry = PT_ParseGetObjectEntry_json_value(jsonValue, "layers");
	if ( !entry.name )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_LevelParse!\n");
		return SDL_FALSE;
	}
	
	const unsigned int numLayers = entry.value->u.array.length;
	_this->layers = (PT_LevelLayer**)malloc(sizeof(PT_LevelLayer*));
	if ( !_this->layers )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_LevelParse: Not enough memory\n");
		return SDL_FALSE;
	}
	_this->numLayers = numLayers;
	
	for ( unsigned int i = 0; i < numLayers; i++ )
	{		
		//Property: map.layers[i].type
		json_object_entry entry2 = PT_ParseGetObjectEntry_json_value(
			entry.value->u.array.values[i], "type"
		);
		
		if ( !entry2.name )
		{
			SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_LevelParse!\n");
			return SDL_FALSE;
		}
		
		if ( PT_StringMatchFast(entry2.value->u.string.ptr, "tilelayer"))
		{
			_this->layers[i] = PT_LevelTileLayerCreate(entry.value->u.array.values[i]);
			if ( !_this->layers[i] )
			{
				SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_LevelParse!\n");
				return SDL_FALSE;
			}
		}
	}

	return SDL_TRUE;
}//PT_LevelParse



