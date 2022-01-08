/*
Copyright 2022 daywithstars

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

#ifndef _PT_LEVEL_H_
#define _PT_LEVEL_H_


#include <json.h>

#include <SDL_stdinc.h>
#include <PT_LevelLayer.h>



typedef enum { 
	PT_LEVEL_ORIENTATION_NONE,
	PT_LEVEL_ORIENTATION_ORTHOGONAL
} PT_LevelOrientations;

typedef struct pt_level {
	PT_LevelOrientations orientation;
	Uint32 width;
	Uint32 height;
	Uint16 tilewidth;
	Uint16 tileheight;
	
	unsigned int numLayers;
	PT_LevelLayer** layers;
}PT_Level;



PT_Level* PT_LevelCreate( json_value* jsonValue );
void PT_LevelDestroy( PT_Level* _this );

void PT_LevelUpdate( PT_Level* _this, Sint32 elapsedTime );
void PT_LevelDraw( PT_Level* _this );


#endif /* _PT_LEVEL_H_ */



