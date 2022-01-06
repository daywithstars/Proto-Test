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

#include <PT_Level.h>


struct pt_level {
	int tmp;
};




PT_Level* PT_LevelCreate( json_value* jsonValue ) {
	PT_Level *_this = (PT_Level*)malloc(sizeof(struct pt_level));
	if ( !_this )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_LevelCreate: Not enough memory\n");
		return NULL;
	}
	SDL_memset(_this, 0, sizeof(struct pt_level));
	
	
	return _this;
}
void PT_LevelDestroy( PT_Level* _this ) {
	if ( !_this )
	{
		return;
	}
	
	free(_this);
}






