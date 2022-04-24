/*
Copyright 2022 daywithstars

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <sys/stat.h>

#include <SDL_log.h>
#include <SDL_stdinc.h>
#include <SDL_events.h>

#include <PT_InputManager.h>
#include <PT_Application.h>
#include <PT_Graphics.h>
#include <PT_ScreenManager.h>
#include <PT_LevelManager.h>
#include <PT_CollisionManager.h>
#include <PT_Parse.h>
#include <PT_Camera.h>
#include <PT_GameManager.h>



typedef struct {
	SDL_bool running;
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
		
		PT_GraphicsShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Proto-Test", 
		"Cannot find or successfully read settings on global-settings in the executable folder\n");
		
		PT_ParseDestroy(parse);
		return SDL_FALSE;
	}
	
	json_object_entry entry = PT_ParseGetObjectEntry(parse, "root-folder");
	if ( !entry.value )
	{
		SDL_LogMessage(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_CRITICAL,
		"PT: PT_ApplicationParseSettings: FILE %s, LINE %d\n", __FILE__, __LINE__);
		
		PT_GraphicsShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Proto-Test", 
		"Cannot find \"root-folder\" element in global-settings.json");
		
		PT_ParseDestroy(parse);
		return SDL_FALSE;
	}
	
	if ( !PT_ParseLegalDirectory(entry.value->u.string.ptr, SDL_FALSE) )
	{
		SDL_LogMessage(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_CRITICAL,
		"PT: PT_ApplicationParseSettings: FILE %s, LINE %d\n", __FILE__, __LINE__);
		
		PT_GraphicsShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Proto-Test", 
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
		
		PT_GraphicsShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Proto-Test",
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
			
			PT_GraphicsShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Proto-Test",
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
			
			PT_GraphicsShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Proto-Test",
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
			
			PT_GraphicsShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Proto-Test",
			"Cannot setup game, trying next game...");
			
			continue;
		}	
		
		if ( !PT_GameManagerCreate() )
		{
			SDL_LogMessage(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_CRITICAL,
			"PT: PT_ApplicationParseSettings: FILE %s, LINE %d\n", __FILE__, __LINE__);
			
			PT_GraphicsShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Proto-Test",
			"Cannot create Game Manager\n");
			
			PT_ParseDestroy(parse);
		
			return SDL_FALSE;
		}
		PT_GameManagerAddGame((char*)gameName, game);
	}
	
	PT_ParseDestroy(parse);
	return SDL_TRUE;
}//PT_ApplicationParseSettings

void PT_ApplicationUpdate( Sint32 elapsedTime ) {

	PT_GameManagerUpdate(elapsedTime);
	
}//PT_ApplicationUpdate

void PT_ApplicationDraw( ) {
	PT_GraphicsRenderClear();
	PT_GameManagerDraw();
	PT_GraphicsRenderPresent();
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
	if ( !PT_CollisionManagerCreate() )
	{
		SDL_LogMessage(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_WARN,
		"PT: PT_GameLoad: Cannot create PT_CollisionManager\n");
		SDL_LogMessage(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_WARN,
		"PT: PT_GameLoad: FILE %s, LINE %d\n", __FILE__, __LINE__);
		
		PT_GraphicsShowSimpleMessageBox(SDL_MESSAGEBOX_WARNING, "Proto-Test",
		"Collision detection system couldn't be initialized.\n\
		Check the console output");
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

	if ( !PT_GameManagerLoadButtons() )
	{
		SDL_LogMessage(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_ERROR,
		"PT: PT_ApplicationCreate: FILE %s, LINE %d\n", __FILE__, __LINE__);

		SDL_LogMessage(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_ERROR,
		"PT: PT_ApplicationCreate!\n");
		
		PT_ApplicationDestroy();
		return SDL_FALSE;
	}
	
	if ( !PT_InputManagerCreateBasic() )
	{
		SDL_LogMessage(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_CRITICAL,
		"PT: PT_ApplicationCreate: Cannot create basic PT_InputManager\n");
		SDL_LogMessage(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_CRITICAL,
		"PT: PT_ApplicationCreate: FILE %s, LINE %d\n", __FILE__, __LINE__);
	
		PT_ApplicationDestroy();
		return SDL_FALSE;
	}
	
	ptApplication->running = SDL_TRUE;
	
	return SDL_TRUE;
}//PT_ApplicationCreate

void PT_ApplicationDestroy( ) {
	
	if ( ptApplication )
	{
		PT_CollisionManagerDestroy();
		PT_GameManagerDestroy();
	
		PT_GraphicsDestroy();
		PT_InputManagerDestroy();
		
		PT_StringDestroy(gDefaultRootDir);
		gDefaultRootDir = NULL;
		PT_StringDestroy(gRootDir);
		gRootDir = NULL;
		
		free(ptApplication);
	}
	ptApplication = NULL;
	
	SDL_Log("Last SDL_Error|json: %s\n", SDL_GetError());
}//PT_ApplicationDestroy

void PT_ApplicationRun( ) {
	PT_ApplicationMainLoop();
}//PT_ApplicationRun






