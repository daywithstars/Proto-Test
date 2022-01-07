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

#include <PT_LevelTileLayer.h>


//===================================== PRIVATE Functions

SDL_bool PT_LevelTileLayerParse( PT_LevelTileLayer* _this, json_value* jsonValue );


//===================================== PUBLIC Functions


PT_LevelTileLayer* PT_LevelTileLayerCreate( json_value* jsonValue ) {

	PT_LevelTileLayer* _this = (PT_LevelTileLayer*)malloc(sizeof(PT_LevelTileLayer));
	if ( !_this )
	{
		return NULL;
	}
	SDL_memset(_this, 0, sizeof(PT_LevelTileLayer));
	
	if ( !PT_LevelTileLayerParse(_this, jsonValue) )
	{
		free(_this);
		return NULL;
	}
	
	return _this;
}
void PT_LevelTileLayerDestroy( PT_LevelTileLayer* _this ) {
	if ( !_this )
	{
		return;
	}
	
	
	free(_this);
}

void PT_LevelTileLayerUpdate( PT_LevelTileLayer* _this, Sint32 elapsedTime ) {
	
}
void PT_LevelTileLayerDraw( PT_LevelTileLayer* _this ) {
}

//===================================== PRIVATE Functions

SDL_bool PT_LevelTileLayerParse( PT_LevelTileLayer* _this, json_value* jsonValue ) {
	if ( !_this )
	{
		return SDL_FALSE;
	}
	
	
	
	return SDL_TRUE;
}//PT_LevelTileLayerParse






