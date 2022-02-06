/*
Copyright 2022 daywithstars

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

#include <SDL_log.h>

#include <PT_Music.h>
#include <PT_String.h>
#include <PT_Application.h>


extern PT_String* gRootDir;


PT_Music* PT_MusicCreate( ) {
	PT_Music* _this = (PT_Music*)malloc(sizeof(PT_Music));
	if ( !_this )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_MusicCreate: Not enough memory\n");
		return NULL;
	}
	SDL_memset(_this, 0, sizeof(PT_Music));
	
	return _this;
}//PT_MusicCreate

void PT_MusicDestroy( PT_Music* _this) {
	
	if ( !_this )
	{
		return;
	}
	
	if ( _this->music )
	{
		Mix_FreeMusic(_this->music);
	}
	
	free(_this);
}//PT_MusicDestroy

SDL_bool PT_MusicLoad( PT_Music* _this, const char* utf8_fileName ) {
	if ( !_this )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_MusicLoad: Invalid PT_Music\n");
		return SDL_FALSE;
	}
	
	PT_String* filePath = PT_StringCreate();
	
	PT_StringInsert(&filePath, utf8_fileName, 0);
	PT_StringInsert(&filePath, "assets/sound/music/", 0);
	PT_StringInsert(&filePath, (char*)gRootDir->utf8_string, 0);
	
	_this->music = Mix_LoadMUS((char*)filePath->utf8_string);
	if ( !_this->music )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_MusicLoad: %s\n", Mix_GetError());
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, 
		"PT: PT_MusicLoad: Cannot load: %s, Check if the filename with extension is the same in the music-list.json\n", 
		utf8_fileName);
		PT_StringDestroy(filePath);
		
		return SDL_FALSE;
	}
	
	PT_StringDestroy(filePath);

	return SDL_TRUE;
}//PT_MusicLoad

void PT_MusicPlay( PT_Music* _this, int loop ) {
	if ( !_this )
	{
		return;
	}
	
	Mix_PlayMusic(_this->music, loop);
}//PT_MusicPlay




