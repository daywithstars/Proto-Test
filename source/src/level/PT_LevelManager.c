/*
Copyright 2022 daywithstars

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

#include <PT_LevelManager.h>
#include <PT_LevelList.h>
#include <PT_Parse.h>


typedef struct {
	PT_LevelList* levelList;
}PT_LevelManager;

static PT_LevelManager* ptLevelManager = NULL;

//===================================== PRIVATE Functions

SDL_bool PT_LevelManagerParse( );


//===================================== PUBLIC Functions

SDL_bool PT_LevelManagerCreate( ) {
	if ( ptLevelManager )
	{
		return SDL_TRUE;
	}
	
	ptLevelManager = (PT_LevelManager*)malloc(sizeof(PT_LevelManager));
	if ( !ptLevelManager )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_LevelManagerCreate: Not enough memory\n");
		return SDL_FALSE;
	}	
	SDL_memset(ptLevelManager, 0, sizeof(PT_LevelManager));
	
	if ( !PT_LevelManagerParse() )
	{
		PT_LevelManagerDestroy();
		return SDL_FALSE;
	}
	
	return SDL_TRUE;
}//PT_LevelManagerCreate
void PT_LevelManagerDestroy( ) {
	if ( !ptLevelManager )
	{
		return;
	}
	
	PT_LevelListDestroy(ptLevelManager->levelList);
	
	free(ptLevelManager);
	ptLevelManager = NULL;
}//PT_LevelManagerDestroy

//===================================== PRIVATE Functions

SDL_bool PT_LevelManagerParse( ) {
	if ( !ptLevelManager )
	{
		return SDL_FALSE;
	}
	
	PT_Parse* parse = PT_ParseCreate();
	if ( !parse )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_LevelManagerParse: Not enough memory\n");
		return SDL_FALSE;
	}
	
	if ( !PT_ParseOpenFile(parse, "assets/level/level-list.json", SDL_TRUE) )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_LevelManagerParse!\n");
		PT_ParseDestroy(parse);
		return SDL_FALSE;
	}
	
	PT_ParseDestroy(parse);
	
	return SDL_TRUE;
}//PT_LevelManagerParse





