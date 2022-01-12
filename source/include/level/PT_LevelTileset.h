/*
Copyright 2022 daywithstars

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

#ifndef _PT_LEVELTILESET_H_
#define _PT_LEVELTILESET_H_

#include <SDL_stdinc.h>

#include <json.h>

#include <PT_String.h>


typedef struct pt_level_tileset {

	PT_String* name;
	Uint8 columns;
	Uint8 firstgid;
	
	PT_String* image;
	Uint16 imagewidth;
	Uint16 imageheight;
	
	Uint16 tilecount;
	Uint16 tilewidth;
	Uint16 tileheight;
	
}PT_LevelTileset;


PT_LevelTileset PT_LevelTilesetCreate( json_value* jsonValue );
void PT_LevelTilesetDestroy( PT_LevelTileset* _this );

#endif /* _PT_LEVELTILESET_H_ */



