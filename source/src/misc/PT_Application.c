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
#include <SDL_stdinc.h>
#include <SDL_events.h>
#include <PT_InputManager.h>
#include <PT_Application.h>
#include <PT_Graphics.h>
#include <PT_ScreenManager.h>
#include <PT_SoundManager.h>
#include <PT_LevelManager.h>
#include <PT_CollisionManager.h>
#include <PT_Parse.h>
#include <PT_Camera.h>



typedef struct {
	SDL_bool running;
	SDL_bool canExit;
	
	PT_String* gameName;
}PT_Application;

extern PT_String* gRootDir;

static PT_Application* ptApplication = NULL;



SDL_bool PT_ApplicationParseSettings( ) {
	/*
		see the template: games/default-templates/global-settings-template.json
	*/
	PT_Parse* parse = PT_ParseCreate();
	
	if ( !PT_ParseOpenFile(parse, "../games/global-settings.json", SDL_FALSE) )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, 
		"PT: PT_Application: Please use the global-settings-template.json and create a new global-settings.json into game directory\n");
		
		PT_ParseDestroy(parse);
		return SDL_FALSE;
	}
	
	json_object_entry entry = PT_ParseGetObjectEntry(parse, "games");
	if ( !entry.name )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, 
		"PT: PT_Application: Cannot find games element into global-settings.json\n");
		
		PT_ParseDestroy(parse);
		return SDL_FALSE;
	}
	
	for ( unsigned int i = 0; i < entry.value->u.array.length; i++ )
	{
		if ( i != 0 )//cap to read only the first array.
		{
			break;
		}
		
		for ( unsigned int j = 0; j < entry.value->u.array.values[i]->u.object.length; j++ )
		{
			
			if ( PT_StringMatchFast(entry.value->u.array.values[i]->u.object.values[j].name, "name") )
			{				
				PT_StringInsert(&ptApplication->gameName, 
				entry.value->u.array.values[i]->u.object.values[j].value->u.string.ptr, 0);
			}
			else if ( PT_StringMatchFast(entry.value->u.array.values[i]->u.object.values[j].name, "folder-name") )
			{
				PT_StringInsert(&gRootDir, "/", 0);
				PT_StringInsert(&gRootDir, 
				entry.value->u.array.values[i]->u.object.values[j].value->u.string.ptr, 0);
				PT_StringInsert(&gRootDir, "../games/", 0);
			}
		}
	}
	
	PT_ParseDestroy(parse);
	return SDL_TRUE;
}//PT_ApplicationParseSettings

void PT_ApplicationUpdate( Sint32 elapsedTime ) {
	PT_CollisionManagerUpdate();
	PT_ScreenManagerUpdate(elapsedTime);
}//PT_ApplicationUpdate

void PT_ApplicationDraw( ) {
	PT_GraphicsRenderClear();
	PT_ScreenManagerDraw();
	PT_GraphicsRenderPresent();
}//PT_ApplicationDraw

void PT_ApplicationMainLoop( ) {
	if ( !ptApplication )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_ApplicationMainLoop: Invalid PT_Application\n");
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
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_ApplicationCreate: Not enough memory\n");
		return SDL_FALSE;
	}
	
	ptApplication->gameName = PT_StringCreate();
	if ( !PT_ScreenManagerCreate() )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, 
		"PT: PT_ApplicationCreate: Cannot create PT_ScreenManager\n");
		PT_ApplicationDestroy();
		return SDL_FALSE;	
	}
	gRootDir = PT_StringCreate();
	
	if ( !PT_ApplicationParseSettings() )
	{
		PT_ApplicationDestroy();
		return SDL_FALSE;
	}
	if ( !PT_GraphicsCreate() )
	{
		PT_ApplicationDestroy();
		return SDL_FALSE;
	}
	if ( !PT_InputManagerCreate() )
	{
		return SDL_FALSE;
	}
	if ( !PT_CollisionManagerCreate() )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_ApplicationCreate!\n");
	}
	if ( !PT_LevelManagerCreate() )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_ApplicationCreate: Cannot create PT_LevelManager!\n");
	}
	else {
		if ( !PT_LevelManagerSetup() )
		{
			SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, 
			"PT: PT_ApplicationCreate: Cannot create PT_LevelManager!\n");
			PT_LevelManagerDestroy();
		}
	}

	if ( !PT_CameraCreate() )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_ApplicationCreate: Cannot initilize Camera\n");
	}
	PT_ScreenManagerSetup();
	
	PT_SoundManagerCreate();
	if ( !PT_SoundManagerLoadSamples() )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_ApplicationCreate!\n");
	}
	if ( !PT_SoundManagerLoadMusics() )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_ApplicationCreate!\n");
	}
	
	SDL_Log("PT: PT_ApplicationCreate: Game: \"%s\", successful initialized\n", 
	(char*)ptApplication->gameName->utf8_string);
	
	ptApplication->running = SDL_TRUE;
	ptApplication->canExit = SDL_TRUE;
	return SDL_TRUE;
}//PT_ApplicationCreate

void PT_ApplicationDestroy( ) {
	
	if ( ptApplication )
	{
		PT_LevelManagerDestroy();
		PT_ScreenManagerDestroy();
		PT_SoundManagerDestroy();
		PT_CollisionManagerDestroy();
		
		//To present sounds threads lose information before using it. 
		while ( !ptApplication->canExit ) { }
		
		PT_GraphicsDestroy();
		PT_InputManagerDestroy();
		PT_CameraDestroy();
		
		PT_StringDestroy(ptApplication->gameName);
		PT_StringDestroy(gRootDir);
		gRootDir = NULL;
		
		free(ptApplication);
	}
	ptApplication = NULL;
	
	SDL_Log("Last SDL_Error|json: %s\n", SDL_GetError());
}//PT_ApplicationDestroy

const PT_String* PT_ApplicationGetGameName( ) {
	return ptApplication->gameName;
}//PT_ApplicationGetGameName

void PT_ApplicationCanExit( SDL_bool value ) {
	ptApplication->canExit = value;
}//PT_ApplicationCanExit

void PT_ApplicationRun( ) {
	PT_ApplicationMainLoop();
}//PT_ApplicationRun






