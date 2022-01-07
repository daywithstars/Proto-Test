/*
Copyright 2022 daywithstars

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

#ifndef _PT_LEVELTILELAYER_H_
#define _PT_LEVELTILELAYER_H_

#include <json.h>

#include <PT_String.h>

typedef struct pt_level_tile_layer {
	Uint32 width;
	Uint32 height;
	Uint32* data;
}PT_LevelTileLayer;


PT_LevelTileLayer* PT_LevelTileLayerCreate( json_value* jsonValue );
void PT_LevelTileLayerDestroy( PT_LevelTileLayer* _this );

void PT_LevelTileLayerUpdate( PT_LevelTileLayer* _this, Sint32 elapsedTime );
void PT_LevelTileLayerDraw( PT_LevelTileLayer* _this );

#endif /* _PT_LEVELTILELAYER_H_ */



