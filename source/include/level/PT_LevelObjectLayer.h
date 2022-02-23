/*
Copyright 2022 daywithstars

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

#ifndef _PT_LEVELOBJECTLAYER_H_
#define _PT_LEVELOBJECTLAYER_H_

#include <json.h>

#include <SDL_stdinc.h>

#include <PT_String.h>
#include <PT_LevelLayer.h>
#include <PT_Sprite.h>
#include <PT_CollisionHandler.h>


typedef struct pt_level_object_layer {
	PT_String* nameID;

	PT_LevelLayer* pLayer;

	unsigned int numSprites;
	PT_Sprite** sprites;
	
	PT_CollisionHandler* collisionHandler;
	
}PT_LevelObjectLayer;


PT_LevelLayer* PT_LevelObjectLayerCreate( json_value* jsonValue, const char* utf8_levelName );
void PT_LevelObjectLayerDestroy( void* layerData );

void PT_LevelObjectLayerUpdate( void* layerData, Sint32 elapsedTime );
void PT_LevelObjectLayerDraw( void* layerData );

#endif /* _PT_LEVELOBJECTLAYER_H_ */



