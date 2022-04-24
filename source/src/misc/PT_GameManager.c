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

#include <SDL_log.h>

#include <PT_GameManager.h>
#include <PT_SpriteList.h>
#include <PT_InputButton.h>
#include <PT_Graphics.h>
#include <PT_ScreenManager.h>
#include <PT_Camera.h>
#include <PT_Parse.h>


typedef struct {
	PT_GameList* gameList;
	PT_Game currentGame;
	PT_SpriteList* buttonList;
	
	Uint32 srandCallCount;
}PT_GameManager;


extern PT_String* gRootDir;
extern PT_String* gDefaultRootDir;

PT_GameManager* ptGameManager = NULL;

//===================================== PRIVATE Functions

SDL_bool PT_GameManagerLoadGame( const char* utf8_gameName );

//===================================== PUBLIC Functions

SDL_bool PT_GameManagerCreate( ) {
	if ( ptGameManager )
	{
		return SDL_TRUE;
	}
	
	ptGameManager = (PT_GameManager*)malloc(sizeof(PT_GameManager));
	if ( !ptGameManager )
	{
		SDL_LogMessage(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_CRITICAL,
		"PT: PT_GameManagerCreate: Not enough memory\n");
	
		return SDL_FALSE;
	}
	SDL_memset(ptGameManager, 0, sizeof(PT_GameManager));


	return SDL_TRUE;
}
SDL_bool PT_GameManagerLoadButtons( ) {
	/*
		Load game buttons
	*/
	#include <PT_GameManager_default_buttonTemplate.h>
	PT_Parse* buttonsParse = PT_ParseCreate();
	if ( !buttonsParse )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_GameManagerLoadButtons!\n");
		
		PT_GameManagerDestroy();
		return SDL_FALSE;
	}
	
	if ( !PT_ParseLoadTemplate(buttonsParse, defaultButtonTemplate) )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_GameManagerLoadButtons!\n");
		
		PT_ParseDestroy(buttonsParse);
		PT_GameManagerDestroy();
		return SDL_FALSE;
	}
	else {
	
		PT_GameList* pGameList = ptGameManager->gameList;
		int i = 0;
		while ( pGameList )
		{
			/* Button position */
			json_object_entry entry = PT_ParseGetObjectEntry(buttonsParse, "dstRect");
			if ( entry.value )
			{
				PT_ParseChangeValue_Double(entry.value->u.array.values[0],
					150 * i
				);
			}
			
			PT_Sprite* button =
			PT_InputButtonCreateFromJsonValue(PT_ParseGetJsonValuePointer(buttonsParse));
			if ( button )
			{
				ptGameManager->buttonList = 
				PT_SpriteListAdd(ptGameManager->buttonList, (char*)pGameList->index->utf8_string,
					button, SDL_TRUE);
			}
			else {
				SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_GameManagerLoadButtons!\n");
				break;
			}
		
			pGameList = pGameList->next;
			i ++;
		}
	}
	PT_ParseDestroy(buttonsParse);
	
	return SDL_TRUE;
}//PT_GameManagerLoadButtons
void PT_GameManagerDestroy( ) {
	if ( !ptGameManager )
	{
		return;
	}
	
	if ( ptGameManager->currentGame.loaded )
	{
		PT_GameUnload(&ptGameManager->currentGame);
	}
		
	PT_GameListDestroy(ptGameManager->gameList);
	PT_SpriteListDestroy(ptGameManager->buttonList, SDL_TRUE);
	
	free(ptGameManager);
	ptGameManager = NULL;
}

void PT_GameManagerAddGame( const char* utf8_gameName, PT_Game game ) {
	ptGameManager->gameList =  PT_GameListAdd( ptGameManager->gameList, utf8_gameName, game );
}

void PT_GameManagerUpdate( Sint32 elapsedTime ) {

	if ( ptGameManager->currentGame.loaded )
	{
		if ( (ptGameManager->srandCallCount += elapsedTime) >= 5000 )
		{
			ptGameManager->srandCallCount = 0;
			time_t t;
			srand((unsigned) time(&t));
		}
		PT_ScreenManagerUpdate(elapsedTime);	
	}
	else {
		PT_SpriteList* list = ptGameManager->buttonList;
		while ( list )
		{
			for ( unsigned int i = 0; i < list->numValues; i++ )
			{
				PT_SpriteUpdate(list->values[i], elapsedTime);
				
				if ( PT_InputButtonGetEventPress((void*)list->values[i]->_data) )
				{
					PT_GameManagerLoadGame((char*)list->index->utf8_string);
					return;
				}
			}
			list = list->next;
		}
	}
}
void PT_GameManagerDraw( ) {
	if ( ptGameManager->currentGame.loaded )
	{
		PT_ScreenManagerDraw();
		PT_CameraDraw();
	}
	else {
		PT_SpriteList* pList = ptGameManager->buttonList;
	
		while ( pList )
		{
			for ( unsigned int i = 0; i < pList->numValues; i++ )
			{
				PT_SpriteDraw(pList->values[i]);
			}
			
			pList = pList->next;
		}
	}
}

//===================================== PRIVATE Functions

SDL_bool PT_GameManagerLoadGame( const char* utf8_gameName ) {
	
	PT_GameList* node = 
	PT_GameListGet(ptGameManager->gameList, utf8_gameName);
	
	if ( !node )
	{
		SDL_LogMessage(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_CRITICAL,
		"PT: PT_GameManagerLoadGame: Cannot load game: %s\n", utf8_gameName);
		SDL_LogMessage(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_CRITICAL,
		"PT: PT_GameManagerLoadGame: FILE %s, LINE %d\n", __FILE__, __LINE__);
		
		return SDL_FALSE;
	}
	if ( ptGameManager->currentGame.loaded  )
	{
		if ( PT_StringMatchFast((char*)ptGameManager->currentGame.folder->utf8_string, 
			(char*)node->value.folder->utf8_string) )
		{
			SDL_LogMessage(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_WARN,
			"PT: PT_GameManagerLoadGame: The game: %s is already loaded\n", utf8_gameName);
			SDL_LogMessage(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_WARN,
			"PT: PT_GameManagerLoadGame: FILE %s, LINE %d\n", __FILE__, __LINE__);
			
			return SDL_TRUE;
		}
		else {
			/* free previous game resources */
			PT_GameUnload(&ptGameManager->currentGame);
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
		"PT: PT_GameManagerLoadGame: Cannot successfully apply the game folder path to gRootDir\n");
		SDL_LogMessage(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_CRITICAL,
		"PT: PT_GameManagerLoadGame: FILE %s, LINE %d\n", __FILE__, __LINE__);
		
		return SDL_FALSE;
	}
	
	if ( !PT_ParseLegalDirectory((char*)gRootDir->utf8_string, SDL_FALSE) )
	{
		SDL_LogMessage(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_CRITICAL,
		"PT: PT_GameManagerLoadGame: FILE %s, LINE %d\n", __FILE__, __LINE__);
		
		PT_GraphicsShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Proto-Test",
		"Cannot find game folder or the current user cannot write and read in the directory.\n");
		
		return SDL_FALSE;
	} 
	
	if ( !PT_GameLoad(&node->value) )
	{
		SDL_LogMessage(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_CRITICAL,
		"PT: PT_GameManagerLoadGame: FILE %s, LINE %d\n", __FILE__, __LINE__);
		
		PT_GraphicsShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Proto-Test",
		"Cannot load game\n");
		
		return SDL_FALSE;
	}
	ptGameManager->currentGame = node->value;
	

	return SDL_TRUE;
}//PT_GameManagerLoadGame











