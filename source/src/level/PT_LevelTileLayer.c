/*
Copyright 2022 daywithstars

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

#include <SDL_stdinc.h>
#include <SDL_log.h>

#include <PT_LevelTileLayer.h>
#include <PT_Parse.h>


//===================================== PRIVATE Functions

SDL_bool PT_LevelTileLayerParse( void* layerData, json_value* jsonValue );


//===================================== PUBLIC Functions


PT_LevelLayer* PT_LevelTileLayerCreate( json_value* jsonValue ) {

	PT_LevelTileLayer* _this = (PT_LevelTileLayer*)malloc(sizeof(PT_LevelTileLayer));
	if ( !_this )
	{
		return NULL;
	}
	SDL_memset(_this, 0, sizeof(PT_LevelTileLayer));
	
	_this->pLayer = PT_LevelLayerCreate((void*)_this, jsonValue, PT_LevelTileLayerParse);
	if ( !_this->pLayer )
	{
		PT_LevelTileLayerDestroy(_this);
		return NULL;
	}
	
	PT_LevelLayerAddUpdateCallback(_this->pLayer, PT_LevelTileLayerUpdate);
	PT_LevelLayerAddDrawCallback(_this->pLayer, PT_LevelTileLayerDraw);
	PT_LevelLayerAddDestroyCallback(_this->pLayer, PT_LevelTileLayerDestroy);
	
	return _this->pLayer;
}//PT_LevelTileLayerCreate

void PT_LevelTileLayerDestroy( void* layerData ) {

	if ( !layerData )
	{
		return;
	}
	
	PT_LevelTileLayer* _this = (PT_LevelTileLayer*)layerData;
	
	if ( _this->data )
	{
		free(_this->data);
	}
	
	free(_this);
}//PT_LevelTileLayerDestroy

void PT_LevelTileLayerUpdate( void* layerData, Sint32 elapsedTime ) {
	PT_LevelTileLayer* _this = (PT_LevelTileLayer*)layerData;
	
}
void PT_LevelTileLayerDraw( void* layerData ) {
	PT_LevelTileLayer* _this = (PT_LevelTileLayer*)layerData;
	
	Uint32 newLine = _this->width;
	printf("\n\n");
	for ( Uint32 i = 0; i < _this->dataLength; i++ )
	{	
		if ( i == newLine )
		{
			printf("\n");
			newLine += _this->width;
		}
		printf(" %d ", _this->data[i]);
	}
	printf("\n\n");
}

//===================================== PRIVATE Functions

SDL_bool PT_LevelTileLayerParse( void* layerData, json_value* jsonValue ) {
	if ( !layerData )
	{
		return SDL_FALSE;
	}

	PT_LevelTileLayer* _this = (PT_LevelTileLayer*)layerData;
	
	
	json_object_entry entry = PT_ParseGetObjectEntry_json_value(jsonValue, "width");
	
	//Property: map.layers[].width
	if ( !entry.name )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_LevelTileLayerParse!\n");
		return SDL_FALSE;
	}
	_this->width = entry.value->u.integer;
	
	
	//Property: map.layers[].height
	entry = PT_ParseGetObjectEntry_json_value(jsonValue, "height");
	if ( !entry.name )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_LevelTileLayerParse!\n");
		return SDL_FALSE;
	}
	_this->height = entry.value->u.integer;
	


	//Property: map.layers[].data
	entry = PT_ParseGetObjectEntry_json_value(jsonValue, "data");
	if ( !entry.name )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_LevelTileLayerParse!\n");
		return SDL_FALSE;
	}
	_this->dataLength = entry.value->u.array.length;
	
	_this->data = (Uint32*)malloc(sizeof(Uint32) * _this->dataLength);
	if ( !_this->data )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_LevelTileLayerParse: Not enough memory\n");
		return SDL_FALSE;
	}
	
	for ( Uint32 i = 0; i < _this->dataLength; i++ )
	{
		_this->data[i] = entry.value->u.array.values[i]->u.integer;
	}

	return SDL_TRUE;
}//PT_LevelTileLayerParse






