/*
Copyright 2022 daywithstars

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

#ifndef _PT_LEVELLAYER_H_
#define _PT_LEVELLAYER_H_

#include <SDL_stdinc.h>

#include <json.h>

#include <PT_String.h>


typedef enum {
	PT_LEVEL_LAYER_TYPE_TILE,
	PT_LEVEL_LAYER_TYPE_OBJECT
}PT_LevelLayerType;


typedef struct pt_level_layer {
	int id;
	SDL_bool visible;
	PT_LevelLayerType type;
	PT_String* name;
	
	void* layerData;
	void (*update)(void* layerData, Sint32 elapsedTime);
	void (*draw)(void* layerData);
	void (*destroy)(void* layerData);
}PT_LevelLayer;


PT_LevelLayer* PT_LevelLayerCreate( void* layerData, json_value* jsonValue,
	SDL_bool (*layerParse)(void* layerData, json_value* jsonValue) );
void PT_LevelLayerDestroy(PT_LevelLayer* _this);


void PT_LevelLayerAddUpdateCallback( PT_LevelLayer* _this, 
	void (*callback)(void* layerData, Sint32 elapsedTime) );
void PT_LevelLayerAddDrawCallback( PT_LevelLayer* _this, void (*callback)(void* layerData) );
void PT_LevelLayerAddDestroyCallback( PT_LevelLayer* _this, void (*callback)(void* layerData) );

void PT_LevelLayerUpdate(PT_LevelLayer* _this, Sint32 elapsedTime);
void PT_LevelLayerDraw(PT_LevelLayer* _this);



#endif /* _PT_LEVELLAYER_H_ */




