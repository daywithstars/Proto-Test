/*
Copyright 2021 daywithstars

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

#include <stdio.h>
#include <stdlib.h>

#include <SDL.h>
#include <locale.h>
#include <PT_Application.h>

SDL_bool SDL_InitializeSystems( );
void SDL_FinalizeSystems( );


int main(int argc, char** argv) {
	setlocale(LC_ALL, "");
	
	printf("number arguments: %d first argument value: %s\n", argc, argv[0]);
	
	if ( !SDL_InitializeSystems() )
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "PT: main.c: %s\n", SDL_GetError());
		return 1;
	}
	
	
	if ( PT_ApplicationCreate() )
	{
		PT_ApplicationRun();
		
		PT_ApplicationDestroy();
	}
	
	SDL_FinalizeSystems();
	return 0;
}

SDL_bool SDL_InitializeSystems( ) {
	if ( SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0 )
	{
		return SDL_FALSE;
	}
	
	return SDL_TRUE;
}
void SDL_FinalizeSystems( ) {
	SDL_Quit();
}



