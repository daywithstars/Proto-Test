/*
Copyright 2022 daywithstars

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <time.h>
#include <sys/stat.h>

#include <SDL_log.h>
#include <SDL_stdinc.h>
#include <SDL_events.h>

#include <PT_InputManager.h>
#include <PT_Application.h>
#include <PT_Graphics.h>
#include <PT_ScreenManager.h>
#include <PT_LevelManager.h>
#include <PT_SoundManager.h>
#include <PT_CollisionManager.h>
#include <PT_Parse.h>
#include <PT_Camera.h>
#include <PT_GameList.h>



typedef struct {
	SDL_bool running;
	
	PT_Game currentGame;
	PT_GameList* gameList;
	Uint32 srandCallCount;
}PT_Application;

extern PT_String* gRootDir;
extern PT_String* gDefaultRootDir;

static PT_Application* ptApplication = NULL;



SDL_bool PT_ApplicationParseSettings( ) {
	PT_Parse* parse = PT_ParseCreate();
	
	if ( !PT_ParseOpenFile(parse, "global-settings.json", SDL_FALSE) )
	{
		SDL_LogMessage(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_CRITICAL,
		"PT: PT_ApplicationParseSettings: FILE %s, LINE %d\n", __FILE__, __LINE__);
		
		PT_GraphicsShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Application", 
		"Cannot find or successfully read settings on global-settings in the executable folder\n");
		
		PT_ParseDestroy(parse);
		return SDL_FALSE;
	}
	
	json_object_entry entry = PT_ParseGetObjectEntry(parse, "root-folder");
	if ( !entry.value )
	{
		SDL_LogMessage(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_CRITICAL,
		"PT: PT_ApplicationParseSettings: FILE %s, LINE %d\n", __FILE__, __LINE__);
		
		PT_GraphicsShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Application", 
		"Cannot find \"root-folder\" element in global-settings.json");
		
		PT_ParseDestroy(parse);
		return SDL_FALSE;
	}
	
	if ( !PT_ParseLegalDirectory(entry.value->u.string.ptr, SDL_FALSE) )
	{
		SDL_LogMessage(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_CRITICAL,
		"PT: PT_ApplicationParseSettings: FILE %s, LINE %d\n", __FILE__, __LINE__);
		
		PT_GraphicsShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Application", 
		"Unable to find the directory from \"root-folder\" element in global-settings.json.\n\
		* It cannot be a file, but an directory.\n\
		* It need to be a relative path starting from the executable location.");
		
		PT_ParseDestroy(parse);
		return SDL_FALSE;
	}
	
	if ( !PT_StringInsert(&gDefaultRootDir, entry.value->u.string.ptr, 0) )
	{
		SDL_LogMessage(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_CRITICAL,
		"PT: PT_ApplicationParseSettings: FILE %s, LINE %d\n", __FILE__, __LINE__);
		
		PT_ParseDestroy(parse);
		return SDL_FALSE;
	}
	
	entry = PT_ParseGetObjectEntry(parse, "games");
	if ( !entry.value || entry.value->type != json_array )
	{
		SDL_LogMessage(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_CRITICAL,
		"PT: PT_ApplicationParseSettings: FILE %s, LINE %d\n", __FILE__, __LINE__);
		
		PT_GraphicsShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Application",
		"Cannot find \"games\" element into global-settings.json or\
		the \"games\" element is not of type json_array");
		
		PT_ParseDestroy(parse);
		return SDL_FALSE;
	}
	
	for ( unsigned int i = 0; i < entry.value->u.array.length; i++ )
	{
		json_object_entry entry2 = 
		PT_ParseGetObjectEntry_json_value(entry.value->u.array.values[i], "name");
		if ( !entry2.value )
		{
			SDL_LogMessage(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_CRITICAL,
			"PT: PT_ApplicationParseSettings: FILE %s, LINE %d\n", __FILE__, __LINE__);
			
			PT_GraphicsShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Application",
			"Cannot find \"games\" \"name\" element into global-settings.json\n\
			trying next game...");
			
			continue;
		}
		
		json_char* gameName = entry2.value->u.string.ptr;
		
		entry2 = 
		PT_ParseGetObjectEntry_json_value(entry.value->u.array.values[i], "folder-name");
		if ( !entry2.value )
		{
			SDL_LogMessage(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_CRITICAL,
			"PT: PT_ApplicationParseSettings: FILE %s, LINE %d\n", __FILE__, __LINE__);
			
			PT_GraphicsShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Application",
			"Cannot find \"games\" \"folder-name\" element into global-settings.json\n\
			trying next game...");
			
			continue;
		}
		
		json_char* gameFolder = entry2.value->u.string.ptr;
		
		PT_Game game = PT_GameCreate(gameFolder);
		if ( !game.folder )
		{
			SDL_LogMessage(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_CRITICAL,
			"PT: PT_ApplicationParseSettings: FILE %s, LINE %d\n", __FILE__, __LINE__);
			
			PT_GraphicsShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Application",
			"Cannot setup game, trying next game...");
			
			continue;
		}	
		ptApplication->gameList = PT_GameListAdd(ptApplication->gameList, gameName, game);
	}
	
	PT_ParseDestroy(parse);
	return SDL_TRUE;
}//PT_ApplicationParseSettings

void PT_ApplicationUpdate( Sint32 elapsedTime ) {
	if ( ptApplication->currentGame.loaded )
	{
		if ( (ptApplication->srandCallCount += elapsedTime) >= 5000 )
		{
			ptApplication->srandCallCount = 0;
			time_t t;
			srand((unsigned) time(&t));
		}
		PT_ScreenManagerUpdate(elapsedTime);	
	}
}//PT_ApplicationUpdate

void PT_ApplicationDraw( ) {
	if ( ptApplication->currentGame.loaded )
	{
		
		PT_GraphicsRenderClear();
		PT_ScreenManagerDraw();
		PT_CameraDraw();
		PT_GraphicsRenderPresent();
		
	}
}//PT_ApplicationDraw

void PT_ApplicationMainLoop( ) {
	if ( !ptApplication )
	{
		SDL_LogMessage(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_CRITICAL,
		"PT: PT_ApplicationMainLoop: Invalid PT_Application\n");
		SDL_LogMessage(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_CRITICAL,
		"PT: PT_ApplicationMainLoop: FILE %s, LINE %d\n", __FILE__, __LINE__);
		return;
	}
	
	Sint32 prevElapsedTime = SDL_GetTicks();
	SDL_Event ev;
	while ( ptApplication->running )
	{
		const Sint32 currentElapsedTime = SDL_GetTicks();
		const Sint32 elapsedTime = currentElapsedTime - prevElapsedTime;
		prevElapsedTime = currentElapsedTime;
	
		PT_InputManagerClearState();
		while ( SDL_PollEvent(&ev) ) {
		
			PT_InputManagerUpdate(&ev);
			if ( ev.type == SDL_QUIT )
			{
				ptApplication->running = SDL_FALSE;
			}
		}
		PT_ApplicationUpdate(elapsedTime);
		PT_ApplicationDraw();
	}
}//PT_ApplicationMainLoop

//===================================== PUBLIC Functions
SDL_bool PT_ApplicationCreate( ) {
	if ( ptApplication )
	{
		return SDL_TRUE;
	}
	
	ptApplication = (PT_Application*)malloc(sizeof(PT_Application));
	if ( !ptApplication )
	{
		SDL_LogMessage(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_CRITICAL,
		"PT: PT_ApplicationCreate: Not enough memory\n");
		SDL_LogMessage(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_CRITICAL,
		"PT: PT_ApplicationCreate: FILE %s, LINE %d\n", __FILE__, __LINE__);
		
		return SDL_FALSE;
	}
	SDL_memset(ptApplication, 0, sizeof(PT_Application));
	
	gRootDir = PT_StringCreate();
	gDefaultRootDir = PT_StringCreate();
	
	if ( !gRootDir || !gDefaultRootDir )
	{
		SDL_LogMessage(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_CRITICAL,
		"PT: PT_ApplicationCreate: Cannot create application basic variables\n");
		SDL_LogMessage(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_CRITICAL,
		"PT: PT_ApplicationCreate: FILE %s, LINE %d\n", __FILE__, __LINE__);
		
		PT_ApplicationDestroy();
		return SDL_FALSE;
	}
	
	if ( !PT_ApplicationParseSettings() )
	{
		SDL_LogMessage(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_CRITICAL,
		"PT: PT_ApplicationCreate: Cannot parse settings\n");
		SDL_LogMessage(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_CRITICAL,
		"PT: PT_ApplicationCreate: FILE %s, LINE %d\n", __FILE__, __LINE__);
	
		PT_ApplicationDestroy();
		return SDL_FALSE;
	}
	
	if ( !PT_GraphicsCreate() )
	{
		SDL_LogMessage(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_CRITICAL,
		"PT: PT_ApplicationCreate: Cannot create PT_Graphics\n");
		SDL_LogMessage(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_CRITICAL,
		"PT: PT_ApplicationCreate: FILE %s, LINE %d\n", __FILE__, __LINE__);
	
		PT_ApplicationDestroy();
		return SDL_FALSE;
	}
	
	if ( !PT_ApplicationLoadGame("Shooter") )
	{
		SDL_LogMessage(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_CRITICAL,
		"PT: PT_ApplicationCreate: Cannot load game\n");
		SDL_LogMessage(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_CRITICAL,
		"PT: PT_ApplicationCreate: FILE %s, LINE %d\n", __FILE__, __LINE__);
	
		PT_ApplicationDestroy();
		return SDL_FALSE;
	}
	
	PT_SoundManagerCreate();
	
	PT_SoundManagerLoadSamples();
	
	PT_SoundManagerLoadMusics();
	
	ptApplication->running = SDL_TRUE;
	ptApplication->srandCallCount = 0;
	
	return SDL_TRUE;
}//PT_ApplicationCreate

void PT_ApplicationDestroy( ) {
	
	if ( ptApplication )
	{
		PT_GameUnload(&ptApplication->currentGame);
		
		PT_SoundManagerDestroy();
	
		PT_GraphicsDestroy();
		
		PT_GameListDestroy(ptApplication->gameList);
		ptApplication->gameList = NULL;
		
		PT_StringDestroy(gDefaultRootDir);
		gDefaultRootDir = NULL;
		PT_StringDestroy(gRootDir);
		gRootDir = NULL;
		
		free(ptApplication);
	}
	ptApplication = NULL;
	
	SDL_Log("Last SDL_Error|json: %s\n", SDL_GetError());
}//PT_ApplicationDestroy

SDL_bool PT_ApplicationLoadGame( const char* utf8_gameName ) {
	if ( !ptApplication )
	{
		SDL_LogMessage(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_CRITICAL,
		"PT: PT_ApplicationLoadGame: Invalid ptApplication\n");
		SDL_LogMessage(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_CRITICAL,
		"PT: PT_ApplicationLoadGame: FILE %s, LINE %d\n", __FILE__, __LINE__);
		
		return SDL_FALSE;
	}
	
	PT_GameList* node = 
	PT_GameListGet(ptApplication->gameList, utf8_gameName);
	
	if ( !node )
	{
		SDL_LogMessage(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_CRITICAL,
		"PT: PT_ApplicationLoadGame: Cannot load game: %s\n", utf8_gameName);
		SDL_LogMessage(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_CRITICAL,
		"PT: PT_ApplicationLoadGame: FILE %s, LINE %d\n", __FILE__, __LINE__);
		
		return SDL_FALSE;
	}
	if ( ptApplication->currentGame.loaded  )
	{
		if ( PT_StringMatchFast((char*)ptApplication->currentGame.folder->utf8_string, 
			(char*)node->value.folder->utf8_string) )
		{
			SDL_LogMessage(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_WARN,
			"PT: PT_ApplicationLoadGame: The game: %s is already loaded\n", utf8_gameName);
			SDL_LogMessage(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_WARN,
			"PT: PT_ApplicationLoadGame: FILE %s, LINE %d\n", __FILE__, __LINE__);
			
			return SDL_TRUE;
		}
		else {
			/* free previous game resources */
			PT_GameUnload(&ptApplication->currentGame);
		}
	}

	SDL_bool gamePathSuccess = SDL_TRUE;	
	PT_StringClear(&gRootDir);
	
	gamePathSuccess = PT_StringInsert(&gRootDir, "/", 0);
	gamePathSuccess = PT_StringInsert(&gRootDir, (char*)node->value.folder->utf8_string, 0);
	gamePathSuccess = PT_StringInsert(&gRootDir, (char*)gDefaultRootDir->utf8_string, 0);
	
	if ( !gamePathSuccess )
	{
		SDL_LogMessage(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_CRITICAL,
		"PT: PT_ApplicationLoadGame: Cannot successfully apply the game folder path to gRootDir\n");
		SDL_LogMessage(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_CRITICAL,
		"PT: PT_ApplicationLoadGame: FILE %s, LINE %d\n", __FILE__, __LINE__);
		
		return SDL_FALSE;
	}
	
	if ( !PT_ParseLegalDirectory((char*)gRootDir->utf8_string, SDL_FALSE) )
	{
		SDL_LogMessage(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_CRITICAL,
		"PT: PT_ApplicationLoadGame: FILE %s, LINE %d\n", __FILE__, __LINE__);
		
		PT_GraphicsShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Application",
		"Cannot find game folder or the current user cannot write and read in the directory.\n\
		Trying next game...");
		
		return SDL_FALSE;
	} 
	
	PT_GameLoad(&node->value);
	
	if ( node->value.loaded )
	{
		ptApplication->currentGame = node->value;
	}
	else {
		SDL_LogMessage(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_CRITICAL,
		"PT: PT_ApplicationLoadGame: Unable to load game: %s\n", utf8_gameName);
		SDL_LogMessage(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_CRITICAL,
		"PT: PT_ApplicationLoadGame: FILE %s, LINE %d\n", __FILE__, __LINE__);
		
		return SDL_FALSE;
	}
	
	return SDL_TRUE;
}//PT_ApplicationLoadGame

void PT_ApplicationRun( ) {
	PT_ApplicationMainLoop();
}//PT_ApplicationRun






