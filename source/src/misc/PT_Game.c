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

#include <PT_Game.h>
#include <PT_Graphics.h>
#include <PT_InputManager.h>
#include <PT_LevelManager.h>
#include <PT_ScreenManager.h>
#include <PT_CollisionManager.h>
#include <PT_SoundManager.h>
#include <PT_Camera.h>


PT_Game PT_GameCreate( const char* utf8_gameFolder ) {
	PT_Game game;
	SDL_memset(&game, 0, sizeof(PT_Game));
	
	game.folder = PT_StringCreate();
	if ( !game.folder )
	{
		SDL_LogMessage(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_CRITICAL,
		"PT: PT_GameCreate: Cannot create game basic variable(s)\n");
		SDL_LogMessage(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_CRITICAL,
		"PT: PT_GameCreate: FILE %s, LINE %d\n", __FILE__, __LINE__);
		
		PT_StringDestroy(game.folder);
		
		SDL_memset(&game, 0, sizeof(PT_Game));
	}
	if ( !PT_StringInsert(&game.folder, utf8_gameFolder, 0) )
	{
		SDL_LogMessage(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_CRITICAL,
		"PT: PT_GameCreate: Cannot create game basic variable(s)\n");
		SDL_LogMessage(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_CRITICAL,
		"PT: PT_GameCreate: FILE %s, LINE %d\n", __FILE__, __LINE__);
		
		PT_StringDestroy(game.folder);
		
		SDL_memset(&game, 0, sizeof(PT_Game));
	}
	
	return game;
}//PT_GameCreate

SDL_bool PT_GameLoad( PT_Game* _this ) {
	if ( _this->loaded )
	{
		SDL_LogMessage(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_WARN,
		"PT: PT_GameLoad: Game already loaded!\n");
		SDL_LogMessage(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_WARN,
		"PT: PT_GameLoad: FILE %s, LINE %d\n", __FILE__, __LINE__);
		
		return SDL_TRUE;
	}

	/*
		Graphics
	*/	
	if ( !PT_GraphicsParseSettings() )
	{
		SDL_LogMessage(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_CRITICAL,
		"PT: PT_GameLoad: Cannot parse graphics settings\n");
		SDL_LogMessage(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_CRITICAL,
		"PT: PT_GameLoad: FILE %s, LINE %d\n", __FILE__, __LINE__);
	
		return SDL_FALSE;
	}
	if ( !PT_GraphicsLoadImages() )
	{
		SDL_LogMessage(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_WARN,
		"PT: PT_GameLoad: Cannot load fonts\n");
		SDL_LogMessage(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_WARN,
		"PT: PT_GameLoad: FILE %s, LINE %d\n", __FILE__, __LINE__);
		PT_GraphicsShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Proto-Test",
		"Cannot load any image file, see the console output.");	
	}
	if ( !PT_GraphicsLoadFonts() )
	{
		SDL_LogMessage(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_WARN,
		"PT: PT_GameLoad: Cannot load fonts\n");
		SDL_LogMessage(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_WARN,
		"PT: PT_GameLoad: FILE %s, LINE %d\n", __FILE__, __LINE__);
	}
	/*
		Input
	*/
	if ( !PT_InputManagerCreate() )
	{
		SDL_LogMessage(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_CRITICAL,
		"PT: PT_GameLoad: Cannot create PT_InputManager\n");
		SDL_LogMessage(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_CRITICAL,
		"PT: PT_GameLoad: FILE %s, LINE %d\n", __FILE__, __LINE__);
	
		PT_GraphicsShowSimpleMessageBox(SDL_MESSAGEBOX_WARNING, "Proto-Test",
		"Input Manager system couldn't be initialized.\n Check the console output");
	
		return SDL_FALSE;
	}
	/*
		Camera
	*/
	if ( !PT_CameraCreate() )
	{
		SDL_LogMessage(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_WARN,
		"PT: PT_GameLoad: Cannot create the camera\n");
		SDL_LogMessage(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_WARN,
		"PT: PT_GameLoad: FILE %s, LINE %d\n", __FILE__, __LINE__);
		
		PT_GraphicsShowSimpleMessageBox(SDL_MESSAGEBOX_WARNING, "Proto-Test",
		"Camera couldn't be initialized.\n Check the console output");
		
		return SDL_FALSE;
	}
	/*
		Sound
	*/
	PT_SoundManagerCreate();
	
	if ( !PT_SoundManagerLoadSamples() )
	{
		SDL_LogMessage(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_WARN,
		"PT: PT_GameLoad: Cannot load sound/sample(s)\n");
		SDL_LogMessage(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_WARN,
		"PT: PT_GameLoad: FILE %s, LINE %d\n", __FILE__, __LINE__);
		
		PT_GraphicsShowSimpleMessageBox(SDL_MESSAGEBOX_WARNING, "Proto-Test",
		"Failed on loading sample(s)");
	}
	if ( !PT_SoundManagerLoadMusics() )
	{
		SDL_LogMessage(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_WARN,
		"PT: PT_GameLoad: Cannot load sound/music(s)\n");
		SDL_LogMessage(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_WARN,
		"PT: PT_GameLoad: FILE %s, LINE %d\n", __FILE__, __LINE__);
		
		PT_GraphicsShowSimpleMessageBox(SDL_MESSAGEBOX_WARNING, "Proto-Test",
		"Failed on loading music(s)");
	}
	/*
		Screen
	*/
	if ( !PT_ScreenManagerCreate() )
	{
		SDL_LogMessage(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_WARN,
		"PT: PT_GameLoad: Cannot create PT_ScreenManager\n");
		SDL_LogMessage(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_WARN,
		"PT: PT_GameLoad: FILE %s, LINE %d\n", __FILE__, __LINE__);
	}
	PT_ScreenManagerSetup();
	/*
		Level
	*/
	if ( !PT_LevelManagerCreate() )
	{
		SDL_LogMessage(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_WARN,
		"PT: PT_GameLoad: Cannot create the level manager\n");
		SDL_LogMessage(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_WARN,
		"PT: PT_GameLoad: FILE %s, LINE %d\n", __FILE__, __LINE__);
	}
	else {
		if ( !PT_LevelManagerSetup() )
		{	
			SDL_LogMessage(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_WARN,
			"PT: PT_GameLoad: Cannot setup level manager\n");
			SDL_LogMessage(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_WARN,
			"PT: PT_GameLoad: FILE %s, LINE %d\n", __FILE__, __LINE__);
			
			PT_LevelManagerDestroy();
		}
	}
	
	
	_this->loaded = SDL_TRUE;
	return SDL_TRUE;
}//PT_GameLoad

void PT_GameUnload( PT_Game *_this ) {
	if ( _this->loaded == SDL_FALSE )
	{
		SDL_LogMessage(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_WARN,
		"PT: PT_GameLoad: Game already unloaded!\n");
		SDL_LogMessage(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_WARN,
		"PT: PT_GameLoad: FILE %s, LINE %d\n", __FILE__, __LINE__);
		
		return;
	}
	
	PT_CollisionManagerClearHandlers();
	
	PT_GraphicsUnloadImages();
	PT_GraphicsUnloadFonts();
	
	PT_InputManagerDestroy();
	
	PT_LevelManagerDestroy();
	
	PT_CameraDestroy();
	
	PT_ScreenManagerDestroy();
	
	PT_SoundManagerDestroy();
	
	_this->loaded = SDL_FALSE;
}//PT_GameUnload

void PT_GameDestroy( PT_Game* _this ) {
	if ( !_this )
	{
		return;
	}

	PT_StringDestroy(_this->folder);
}//PT_GameDestroy






