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

#include <PT_LevelManager.h>
#include <PT_LevelList.h>
#include <PT_Parse.h>
#include <PT_JsonList.h>
#include <PT_Graphics.h>


typedef struct {
	PT_LevelList* levelList;
	PT_JsonList* jsonList;
	
	PT_Level* currentLevel;
	
}PT_LevelManager;

static PT_LevelManager* ptLevelManager = NULL;


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
	
	return SDL_TRUE;
}//PT_LevelManagerCreate
void PT_LevelManagerDestroy( ) {
	if ( !ptLevelManager )
	{
		return;
	}
	
	PT_LevelListDestroy(ptLevelManager->levelList);
	PT_JsonListDestroy(ptLevelManager->jsonList);
	
	free(ptLevelManager);
	ptLevelManager = NULL;
}//PT_LevelManagerDestroy

SDL_bool PT_LevelManagerSetup() {
	if ( !ptLevelManager )
	{
		return SDL_FALSE;
	}
	
	if ( !PT_ParseLegalDirectory("assets/level/", SDL_TRUE) )
	{
		return SDL_TRUE;
	}
	
	//To refresh the level list.
	PT_JsonListDestroy(ptLevelManager->jsonList);
	
	
	PT_Parse* parse = PT_ParseCreate();
	if ( !parse )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_LevelManagerSetup!\n");
		return SDL_FALSE;
	}
	
	if ( !PT_ParseOpenFile(parse, "assets/level/level-list.json", SDL_TRUE) )
	{
		SDL_LogMessage(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_WARN,
		"PT: PT_LevelManagerSetup: Cannot find list-list.json, creating a default level-list.json\n");
		SDL_LogMessage(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_WARN,
		"PT: PT_LevelManagerSetup: FILE %s, LINE %d\n", __FILE__, __LINE__);
		
		PT_GraphicsShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Proto-Test",
		"* Cannot find level-list.json in assets/level/ directory\n\
		* A default level-list.json will be created in that directory");
		
		#include <PT_LevelManager_default_levelList.h>
		if ( !PT_ParseLoadTemplate(parse, defaultLevelListTemplate) )
		{
			SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_LevelManagerSetup!\n");
			PT_ParseDestroy(parse);
			return SDL_FALSE;
		}
		
		if ( !PT_ParseSaveOriginal(parse, "assets/level/level-list.json", SDL_TRUE) )
		{
			SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_LevelManagerSetup!\n");
		}
	}
	
	const json_value* jsonValue = PT_ParseGetJsonValuePointer(parse);
	
	for ( unsigned int i = 0; i < jsonValue->u.object.length; i++ )
	{
		if ( PT_StringMatchFast(jsonValue->u.object.values[i].name, "none") )
		{
			continue;
		}
		
		json_char* levelName = jsonValue->u.object.values[i].name;
		json_char* levelFileName = jsonValue->u.object.values[i].value->u.string.ptr;
		
		PT_String* jsonLevelPath = PT_StringCreate();
		if ( !jsonLevelPath )
		{
			SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_LevelManagerSetup!\n");
			break;
		}
		
		if ( !PT_StringInsert(&jsonLevelPath, ".json", 0) )
		{
			SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_LevelManagerSetup!\n");
			PT_StringDestroy(jsonLevelPath);
			continue;
		}
		if ( !PT_StringInsert(&jsonLevelPath, levelFileName, 0) )
		{
			SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_LevelManagerSetup!\n");
			PT_StringDestroy(jsonLevelPath);
			continue;
		}
		if ( !PT_StringInsert(&jsonLevelPath, "assets/level/", 0) )
		{
			SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_LevelManagerSetup!\n");
			PT_StringDestroy(jsonLevelPath);
			continue;
		}
		
		
		json_value* value = PT_ParseGetJsonValueFromFile((char*)jsonLevelPath->utf8_string, SDL_TRUE);
		if ( !value )
		{
			SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_LevelManagerSetup!\n");
			PT_StringDestroy(jsonLevelPath);
			continue;
		}
		
		ptLevelManager->jsonList = PT_JsonListAdd(ptLevelManager->jsonList, levelName, value);
		
		PT_StringDestroy(jsonLevelPath);
	}
	
	PT_ParseDestroy(parse);
	
	return SDL_TRUE;
}//PT_LevelManagerSetup

SDL_bool PT_LevelManagerLoadLevel( const char* utf8_levelName ) {
	if ( !ptLevelManager )
	{
		return SDL_FALSE;
	}
	
	PT_JsonList* node = PT_JsonListGet(ptLevelManager->jsonList, utf8_levelName);
	if ( !node )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, 
		"PT: PT_LevelManagerLoadLevel: Cannot find level: %s\n", utf8_levelName);
		return SDL_FALSE;
	}
	
	PT_Level* level = PT_LevelCreate(node->value, utf8_levelName);
	if ( !level )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION,
		"PT: PT_LevelManagerLoadLevel\n");
		return SDL_FALSE;
	}
	
	ptLevelManager->levelList = PT_LevelListAdd(ptLevelManager->levelList, utf8_levelName, level);
	if ( !ptLevelManager->currentLevel )
	{
		if ( ptLevelManager->levelList )
		{
			ptLevelManager->currentLevel = ptLevelManager->levelList->value;
		}
	}
	
	return SDL_TRUE;
}//PT_LevelManagerLoadLevel

void PT_LevelManagerUpdate( Sint32 elapsedTime ) {
	if ( ptLevelManager->currentLevel )
	{
		PT_LevelUpdate(ptLevelManager->currentLevel, elapsedTime);
	}
}
void PT_LevelManagerDraw( ) {
	if ( ptLevelManager->currentLevel )
	{
		PT_LevelDraw(ptLevelManager->currentLevel);
	}
}





