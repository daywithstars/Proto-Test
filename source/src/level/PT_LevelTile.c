/*
Copyright 2022 daywithstars

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

#include <stdlib.h>

#include <PT_LevelTile.h>
#include <PT_Graphics.h>


PT_LevelTile PT_LevelTileCreate( SDL_bool visible, SDL_Rect srcRect, SDL_FRect dstRect, 
	PT_String* pImage ) {
	
	PT_LevelTile _this = { visible, srcRect, dstRect, pImage };
	return _this;
}

void PT_LevelTileDraw( PT_LevelTile _this ) {
	if ( _this.visible )
	{
		PT_GraphicsDrawTextureF((char*)_this.pImage->utf8_string, &_this.srcRect, &_this.dstRect,
			0.0, NULL, SDL_FLIP_NONE);
	}
}








