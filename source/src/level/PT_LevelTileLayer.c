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
#include <PT_Camera.h>
#include <PT_InputManager.h>

static float x=0.f, y=0.f;

//===================================== PRIVATE Functions

SDL_bool PT_LevelTileLayerParse( void* layerData, json_value* jsonValue );


//===================================== PUBLIC Functions


PT_LevelLayer* PT_LevelTileLayerCreate( json_value* jsonValue, Uint16 tilewidth, Uint16 tileheight,
	unsigned int numTilesets, PT_LevelTileset* pTilesets ) {

	PT_LevelTileLayer* _this = (PT_LevelTileLayer*)malloc(sizeof(PT_LevelTileLayer));
	if ( !_this )
	{
		return NULL;
	}
	SDL_memset(_this, 0, sizeof(PT_LevelTileLayer));
	
	_this->tilewidth = tilewidth;
	_this->tileheight = tileheight;
	
	_this->numTilesets = numTilesets;
	_this->pTilesets = pTilesets;
	
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
	
	if ( _this->tiles )
	{
		for ( unsigned int i = 0; i < _this->height; i++ )
		{
			free(_this->tiles[i]);
		}
		free(_this->tiles);
	}
	
	free(_this);
}//PT_LevelTileLayerDestroy

void PT_LevelTileLayerUpdate( void* layerData, Sint32 elapsedTime ) {
	PT_LevelTileLayer* _this = (PT_LevelTileLayer*)layerData;
	
	if ( PT_InputManagerKeyboardGetKeyHold(SDL_SCANCODE_LEFT) )
	{
		x -= 0.1 * elapsedTime;
	}
	else if ( PT_InputManagerKeyboardGetKeyHold(SDL_SCANCODE_RIGHT) )
	{
		x += 0.1 * elapsedTime;
	}
	else if ( PT_InputManagerKeyboardGetKeyHold(SDL_SCANCODE_UP) )
	{
		y -= 0.1 * elapsedTime;
	}
	else if ( PT_InputManagerKeyboardGetKeyHold(SDL_SCANCODE_DOWN) )
	{
		y += 0.1 * elapsedTime;
	}
	
	
	PT_CameraSetPosition((Sint32)x, (Sint32)y);
}
void PT_LevelTileLayerDraw( void* layerData ) {
	PT_LevelTileLayer* _this = (PT_LevelTileLayer*)layerData;

	Uint32 startColumn = 0;
	Uint32 startRow = 0;
	Uint32 maxColumn = 0;
	Uint32 maxRow = 0;
	
	PT_CameraGetRenderDistance(&startColumn, &startRow, &maxColumn, &maxRow, 
	_this->width, _this->height, _this->tilewidth, _this->tileheight);
	
	for ( Uint32 i = startRow; i < maxRow; i++ )
	{	
		for ( Uint32 j = startColumn; j < maxColumn; j++ )
		{
			PT_LevelTileDraw(_this->tiles[i][j]);
		}
	}
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
	
	_this->tiles = (PT_LevelTile**)malloc(sizeof(PT_LevelTile*) * _this->height);
	if ( !_this->tiles )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_LevelTileLayerParse: Not enough memory\n");
		return SDL_FALSE;
	}
	
	Uint32 k = 0;
	for ( Uint32 i = 0; i < _this->height; i++ )
	{
		_this->tiles[i] = (PT_LevelTile*)malloc(sizeof(PT_LevelTile) * _this->width);
		if ( !_this->tiles[i] )
		{
			SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_LevelTileLayerParse: Not enough memory\n");
			return SDL_FALSE;
		}
		
		for ( Uint32 j = 0; j < _this->width; j++ )
		{
			Uint32 data = entry.value->u.array.values[k]->u.integer;
			
			if ( data == 0 )
			{
				_this->tiles[i][j].visible = SDL_FALSE;
				k++;
				continue;
			}
			
			//Tile setup
			for ( Uint32 l = 0; l < _this->numTilesets; l++ )
			{	
				//visible
				if ( data > 0 )
				{
					PT_LevelTile tile;
					tile.visible = SDL_TRUE;
					
					//image
					if ( data >= _this->pTilesets[l].firstgid && 
						data < _this->pTilesets[l].firstgid + _this->pTilesets[l].tilecount )
					{
						tile.pImage = _this->pTilesets[l].image;
						Uint32 index = data - _this->pTilesets[l].firstgid;
			
						//srcRect
						if ( index == 1 )
						{
							tile.srcRect.x = 0;
							tile.srcRect.y = 0;
							tile.srcRect.w = _this->pTilesets[l].tilewidth;
							tile.srcRect.h = _this->pTilesets[l].tileheight;
						}
						else {
							int sx = 0;
							const int sw = _this->pTilesets[l].tilewidth;
							int sy_count = 0;
							const int sh = _this->pTilesets[l].tileheight;
							
							for ( Uint32 m = 0; m < _this->pTilesets[l].tilecount; m++ )
							{
								if ( m != 0 )
								{
									sx =
									(m % _this->pTilesets[l].columns) * _this->pTilesets[l].tilewidth;
									
									if ( m % _this->pTilesets[l].columns == 0 )
									{
										sy_count ++;
									}
								}
								
								int sy = sy_count * _this->pTilesets[l].tileheight;

								
								if ( index == m )
								{
									tile.srcRect.x = sx;
									tile.srcRect.y = sy;
									tile.srcRect.w = sw;
									tile.srcRect.h = sh;
									break;
								}
							}
						}
						
						//dstRect
						tile.dstRect.x = j * _this->pTilesets[l].tilewidth;
						tile.dstRect.y = i * _this->pTilesets[l].tileheight;
						tile.dstRect.w = _this->pTilesets[l].tilewidth;
						tile.dstRect.h = _this->pTilesets[l].tileheight;
						 
						_this->tiles[i][j] = tile;
						break;
					}
				}
				else {
					_this->tiles[i][j].visible = SDL_FALSE;
					break;
				}
			}
			
			
			k++;
		}
	}

	return SDL_TRUE;
}//PT_LevelTileLayerParse






