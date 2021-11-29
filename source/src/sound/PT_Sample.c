/*
Copyright 2021 daywithstars

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

#include <SDL_log.h>

#include <PT_Sample.h>
#include <PT_Application.h>
#include <PT_SoundManager.h>
#include <PT_String.h>


extern PT_String* gRootDir;


PT_Sample* PT_SampleCreate( ) {
	PT_Sample* _this = (PT_Sample*)malloc(sizeof(PT_Sample));
	if ( !_this )
	{
		return NULL;
	}
	SDL_memset(_this, 0, sizeof(PT_Sample));
	
	return _this;
}//PT_SampleCreate

void PT_SampleDestroy( PT_Sample* _this ) {

	if ( !_this )
	{
		return;
	}
	
	if ( _this->chunk )
	{
		Mix_FreeChunk(_this->chunk);
	}
	
	free(_this);
}//PT_SampleDestroy

SDL_bool PT_SampleLoad( PT_Sample* _this, const char* utf8_fileName ) {
	if ( !_this )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_SampleLoad: Invalid PT_Sample\n");
		return SDL_FALSE;
	}
	
	PT_String* filePath = PT_StringCreate();
	
	PT_StringInsert(&filePath, utf8_fileName, 0);
	PT_StringInsert(&filePath, "assets/sound/sample/", 0);
	PT_StringInsert(&filePath, (char*)gRootDir->utf8_string, 0);
	
	_this->chunk = Mix_LoadWAV((char*)filePath->utf8_string);
	if ( !_this->chunk )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_SampleLoad: %s\n", SDL_GetError());
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, 
		"PT: PT_SampleLoad: Cannot load: %s, Check if the filename with extension is the same in the sample-list.json\n", 
		utf8_fileName);
		PT_StringDestroy(filePath);
		
		return SDL_FALSE;
	}
	
	
	PT_StringDestroy(filePath);
	
	return SDL_TRUE;
}//PT_SampleLoad

void PT_SamplePlay( PT_Sample* _this, int loop ) {
	if ( !_this )
	{
		return;
	}
	
	int channel = Mix_PlayChannel(-1, _this->chunk, loop);
	Mix_GroupChannel(channel, PT_SoundManagerGetSampleTagGroup());
}//PT_SamplePlay



