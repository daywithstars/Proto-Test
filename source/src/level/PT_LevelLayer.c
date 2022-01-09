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

#include <PT_LevelLayer.h>
#include <PT_Parse.h>


//===================================== PRIVATE Functions

SDL_bool PT_LevelLayerParse( PT_LevelLayer* _this, json_value* jsonValue );

//===================================== PUBLIC Functions

PT_LevelLayer* PT_LevelLayerCreate( void* layerData, json_value* jsonValue,
	SDL_bool (*layerParse)(void* layerData, json_value* jsonValue) ) {
	
	PT_LevelLayer* _this = (PT_LevelLayer*)malloc(sizeof(PT_LevelLayer));
	if ( !_this )
	{
		return NULL;
	}
	SDL_memset(_this, 0, sizeof(PT_LevelLayer));
	
	if ( !PT_LevelLayerParse(_this, jsonValue) )
	{
		free(_this);
		return NULL;
	}
	
	if ( layerParse )
	{
		if ( !layerParse(layerData, jsonValue) )
		{
			PT_LevelLayerDestroy(_this);
			return NULL;
		}
	}
	_this->layerData = layerData;
	
	return _this;
}
void PT_LevelLayerDestroy(PT_LevelLayer* _this) {
	if ( !_this )
	{
		return;
	}
	
	PT_StringDestroy(_this->name);
	
	if ( _this->destroy )
	{
		_this->destroy(_this->layerData);
	}
	
	free(_this);
}


void PT_LevelLayerAddUpdateCallback( PT_LevelLayer* _this, 
	void (*callback)(void* layerData, Sint32 elapsedTime) ) {
	
	_this->update = callback;
}
void PT_LevelLayerAddDrawCallback( PT_LevelLayer* _this, void (*callback)(void* layerData) ) {
	_this->draw = callback;
}
void PT_LevelLayerAddDestroyCallback( PT_LevelLayer* _this, void (*callback)(void* layerData) ) {
	_this->destroy = callback;
}

void PT_LevelLayerUpdate(PT_LevelLayer* _this, Sint32 elapsedTime) {
	if ( _this->visible )
	{
		_this->update(_this->layerData, elapsedTime);
	}
}
void PT_LevelLayerDraw(PT_LevelLayer* _this) {
	if ( _this->visible )
	{
		_this->draw(_this->layerData);
	}
}


//===================================== PRIVATE Functions

SDL_bool PT_LevelLayerParse( PT_LevelLayer* _this, json_value* jsonValue ) {
	
	json_object_entry entry = PT_ParseGetObjectEntry_json_value(jsonValue, "id");
	
	//Property: map.layers[].id
	if ( !entry.name )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_LevelLayerParse!\n");
		return SDL_FALSE;
	}
	_this->id = entry.value->u.integer;
	
	//Property: map.layers[].visible
	entry = PT_ParseGetObjectEntry_json_value(jsonValue, "visible");
	if ( !entry.name )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_LevelLayerParse!\n");
		return SDL_FALSE;
	}
	_this->visible = entry.value->u.boolean;
	
	//Property: map.layers[].type
	entry = PT_ParseGetObjectEntry_json_value(jsonValue, "type");
	if ( !entry.name )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_LevelLayerParse!\n");
		return SDL_FALSE;
	}
	if ( PT_StringMatchFast(entry.value->u.string.ptr, "tilelayer") )
	{
		_this->type = PT_LEVEL_LAYER_TYPE_TILE;
	}
	
	//Property: map.layers[].name
	entry = PT_ParseGetObjectEntry_json_value(jsonValue, "name");
	if ( !entry.name )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_LevelLayerParse!\n");
		return SDL_FALSE;
	}
	_this->name = PT_StringCreate();
	if ( !_this->name )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_LevelLayerParse: Not enough memory\n");
		return SDL_FALSE;
	}
	if ( !PT_StringInsert(&_this->name, entry.value->u.string.ptr, 0) )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_LevelLayerParse!\n");
		return SDL_FALSE;
	}

	return SDL_TRUE;
}











