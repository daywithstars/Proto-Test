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
#include <SDL_stdinc.h>
#include <SDL_events.h>

#include <PT_Application.h>
#include <PT_Graphics.h>
#include <PT_ScreenManager.h>
#include <PT_SoundManager.h>
#include <PT_Parse.h>
#include <PT_InputManager.h>
#include <PT_Mouse.h>



typedef struct {
	SDL_bool running;
	SDL_bool canExit;
	
	PT_String* gameName;
	PT_Mouse* mouse;
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
		SDL_Log("(*)PT: PT_ApplicationParseSettings: Creating default global-settings.json\n");
		
		if ( !PT_ParseLoadTemplate(parse, 
"{\n \
	\"game\": \"your-game-name\"\n\
}") )
		{
			SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_ApplicationParseSettings!\n");
			return SDL_FALSE;
		}
		
		if ( !PT_ParseSaveFile(parse, "../games/global-settings.json", SDL_FALSE) )
		{
			SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_ApplicationParseSettings!\n");
			return SDL_FALSE; 
		}

		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, 
		"* Create your game folder and put the name on: \"game\": \"your-game-name\" from global-settings.json then run again\n");
		
		return SDL_FALSE;
	}
	
	json_object_entry entry = PT_ParseGetObjectEntry(parse, "game");
	if ( entry.name )
	{
		PT_StringInsert(&gRootDir, "/", 0);
		PT_StringInsert(&gRootDir, entry.value->u.string.ptr, 0);
		PT_StringInsert(&gRootDir, "../games/", 0);
		
		PT_StringInsert(&ptApplication->gameName, entry.value->u.string.ptr, 0);
	}
	else {
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, 
		"PT: PT_ApplicationParseSettings: Cannot get game property\n");
		return SDL_FALSE;
	}
	
	PT_ParseDestroy(parse);
	return SDL_TRUE;
}//PT_ApplicationParseSettings

void PT_ApplicationUpdate( Sint32 elapsedTime ) {
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
		PT_MouseClearState(ptApplication->mouse);
		while ( SDL_PollEvent(&ev) ) {
		
			PT_InputManagerUpdate(&ev);
			PT_MouseUpdate(ptApplication->mouse, &ev);
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

	PT_ScreenManagerSetup();
	
	PT_ScreenManagerLoadScreen("Play Screen");
	PT_ScreenManagerLoadScreen("Settings Screen");
	PT_ScreenManagerLoadScreen("Menu Screen");
	PT_ScreenManagerSetFirstScreen("Play Screen");
	
	PT_SoundManagerCreate();
	if ( !PT_SoundManagerLoadSamples() )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_ApplicationCreate!\n");
	}
	if ( !PT_SoundManagerLoadMusics() )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_ApplicationCreate!\n");
	}
	
	PT_SoundManagerPlayMusic("SS2-intro", 1);
	
	SDL_Log("PT: PT_ApplicationCreate: Game: \"%s\", successful initialized\n", 
	(char*)ptApplication->gameName->utf8_string);
	
	ptApplication->mouse = PT_MouseCreate();
	
	ptApplication->running = SDL_TRUE;
	ptApplication->canExit = SDL_TRUE;
	return SDL_TRUE;
}//PT_ApplicationCreate

void PT_ApplicationDestroy( ) {
	
	if ( ptApplication )
	{
		PT_ScreenManagerDestroy();
		PT_SoundManagerDestroy();
		
		//To present sounds threads lose information before using it. 
		while ( !ptApplication->canExit ) { }
		SDL_Delay(100);//Give 100ms to any background processing returns. 
		
		PT_GraphicsDestroy();
		PT_InputManagerDestroy();
		
		PT_StringDestroy(ptApplication->gameName);
		PT_StringDestroy(gRootDir);
		gRootDir = NULL;
		
		PT_MouseDestroy(ptApplication->mouse);
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





