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

#include <PT_ScreenManager.h>
#include <PT_StringList.h>
#include <PT_JsonList.h>
#include <PT_Parse.h>
#include <PT_Graphics.h>



typedef struct pt_screenList {
	PT_String* index;
	PT_Screen* value;

	struct pt_screenList* next;
}PT_ScreenList;

typedef struct {
	PT_Screen* firstScreen;
	PT_Screen* secondScreen;

	PT_ScreenList* screenList;
	PT_StringList* screenListToFree;
	PT_JsonList* jsonList;
}PT_ScreenManager;


static PT_ScreenManager* ptScreenManager = NULL;

//===================================== PRIVATE Functions declarations

//================= PT_ScreenList
PT_ScreenList* PT_ScreenListCreate( const char* utf8_index, PT_Screen* value );

PT_ScreenList* PT_ScreenListAdd( PT_ScreenList* _this, const char* utf8_index, PT_Screen* value );
PT_ScreenList* PT_ScreenListGet( PT_ScreenList* _this, const char* utf8_index );

PT_ScreenList* PT_ScreenListRemove( PT_ScreenList* _this, const char* utf8_index );

void PT_ScreenListDestroy( PT_ScreenList* _this );

//================= PT_ScreenManager

void PT_ScreenManagerFreeScreens( );


//===================================== END PRIVATE Funtions declarations

//===================================== PUBLIC Functions
//================= PT_ScreenManager
SDL_bool PT_ScreenManagerCreate( ) {
	if ( ptScreenManager )
	{
		return SDL_TRUE;
	}

	ptScreenManager = (PT_ScreenManager*)malloc(sizeof(PT_ScreenManager));
	if ( !ptScreenManager )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_ScreenManagerCreate: Not enough memory\n");
		return SDL_FALSE;
	}
	SDL_memset(ptScreenManager, 0, sizeof(PT_ScreenManager));
	
	ptScreenManager->firstScreen = NULL;
	ptScreenManager->secondScreen = NULL;

	ptScreenManager->screenList = NULL;
	ptScreenManager->screenListToFree = NULL;
	ptScreenManager->jsonList = NULL;

	return SDL_TRUE;
}//PT_ScreenManagerCreate

void PT_ScreenManagerDestroy( ) {
	if ( !ptScreenManager )
	{
		return;
	}
	
	ptScreenManager->firstScreen = NULL;
	ptScreenManager->secondScreen = NULL;
	
	PT_ScreenListDestroy(ptScreenManager->screenList);
	PT_StringListDestroy(ptScreenManager->screenListToFree);
	
	PT_JsonListDestroy(ptScreenManager->jsonList);

	free(ptScreenManager);
	ptScreenManager = NULL;
}//PT_ScreenManagerDestroy

void PT_ScreenManagerSetup( ) {
	if ( !ptScreenManager )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_ScreenManagerSetup: Invalid ScreenManager!\n");
		return;
	}
	
	if ( !PT_ParseLegalDirectory("assets/screen/", SDL_TRUE) )
	{
		return;
	}

	PT_JsonListDestroy(ptScreenManager->jsonList);
	
	PT_Parse* parseList = PT_ParseCreate();
	if ( !parseList )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_ScreenManagerSetup!\n");
		return;
	}
	PT_String* screenListPath = PT_StringCreate();
	if ( !screenListPath )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_ScreenManagerSetup!\n");
		PT_ParseDestroy(parseList);
		return;
	}
	
	if ( !PT_StringInsert(&screenListPath, "assets/screen/screen-list.json", 0) )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_ScreenManagerSetup!\n");
	}
	if ( !PT_ParseOpenFile(parseList, (json_char*)screenListPath->utf8_string, SDL_TRUE) )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_ScreenManagerSetup!\n");
		PT_GraphicsShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Proto-Test",
		"* Cannot find screen-list.json in folder assets/screen/\n\
		A default screen-list.json will be created.");
		
		
		#include <PT_ScreenManager_default_screenList.h>
		if ( !PT_ParseLoadTemplate(parseList, defaultScreenListTemplate) )
		{
			SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_ScreenManagerSetup!\n");
			PT_ParseDestroy(parseList);
			return;
		}
		
		if ( !PT_ParseSaveOriginal(parseList, "assets/screen/screen-list.json", SDL_TRUE) )
		{
			SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_ScreenManagerSetup!\n");
		}
	}
	
	json_object_entry entry = PT_ParseGetObjectEntry(parseList, "screens");
	if ( entry.value )
	{
		if ( entry.value->type == json_array )
		{
			for ( unsigned int i = 0; i < entry.value->u.array.length; i++)
			{
				if ( entry.value->u.array.values[i]->type == json_string )
				{
					json_char* templatePath = entry.value->u.array.values[i]->u.string.ptr;
					
					PT_String* jsonScreenPath = PT_StringCreate();
					
					if ( !PT_StringInsert(&jsonScreenPath, ".json", 0) )
					{
						SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_ScreenManagerSetup!\n");
					}
					if ( !PT_StringInsert(&jsonScreenPath, templatePath, 0) )
					{
						SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_ScreenManagerSetup!\n");
					}
					if ( !PT_StringInsert(&jsonScreenPath, "assets/screen/", 0) )
					{
						SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_ScreenManagerSetup!\n");
					}
					
					
					json_value* jsonValue = 
					PT_ParseGetJsonValueFromFile((char*)jsonScreenPath->utf8_string, SDL_TRUE);
					
					if ( jsonValue->type == json_object )
					{
						json_object_entry entry = 
						PT_ParseGetObjectEntry_json_value(jsonValue, "settings name");

						ptScreenManager->jsonList = PT_JsonListAdd(ptScreenManager->jsonList, 
							entry.value->u.string.ptr, jsonValue);	
							
						entry =
						PT_ParseGetObjectEntry_json_value(jsonValue, "settings tag");
						if ( entry.value )
						{
							if ( PT_StringMatchFast((char*)entry.value->u.string.ptr, "first-screen") )
							{
								entry = 
								PT_ParseGetObjectEntry_json_value(jsonValue, "settings name");
							
								if ( !PT_ScreenManagerLoadScreen(entry.value->u.string.ptr) )
								{
									SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, 
									"PT: PT_ScreenManagerSetup!\n");
								}
								else {
									PT_ScreenManagerSetFirstScreen(entry.value->u.string.ptr);
								}
							}
						}
					}

					
					PT_StringDestroy(jsonScreenPath);
					jsonScreenPath = NULL;
				}
				else {
					SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, 
					"PT: PT_ScreenManagerSetup: The array index: %u is not an json_string\n", i);
				}
			}
		}
		else {
			SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, 
			"PT: PT_ScreenManagerSetup: The property type is not an json_array\n");
		}
	}
	else {
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, 
		"PT: PT_ScreenManagerSetup: Cannot find property\n");
	}
	
	PT_StringDestroy(screenListPath);
	screenListPath = NULL;
	
	PT_ParseDestroy(parseList);
	parseList = NULL;
}//PT_ScreenManagerSetup

void PT_ScreenManagerSetFirstScreen( const char* utf8_screenName ) {
	if ( !ptScreenManager )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, 
		"PT: PT_ScreenManagerSetFirstScreen: Invalid ScreenManager!\n");
		return;
	}

	PT_ScreenList* node = PT_ScreenListGet(ptScreenManager->screenList, utf8_screenName);
	
	if ( node )
	{
		if ( !ptScreenManager->firstScreen )
		{
			if ( ptScreenManager->secondScreen == node->value )
			{
				SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, 
				"PT: PT_ScreenManagerSetFirstScreen: The same screen is running on second slot\n");
				return;
			}
			
			ptScreenManager->firstScreen = node->value;
			return;
		}
		
		if ( ptScreenManager->secondScreen == ptScreenManager->firstScreen )
		{
			SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, 
			"PT: PT_ScreenManagerSetFirstScreen: The same screen is running on second slot\n");
			return;
		}
		
		ptScreenManager->firstScreen = node->value;
	}
	else {
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, 
		"PT: PT_ScreenManagerSetFirstScreen: Can't find loaded screen: %s\n", utf8_screenName);
	}
}//PT_ScreenManagerSetFirstScreen

void PT_ScreenManagerSetSecondScreen( const char* utf8_screenName ) {
	if ( !ptScreenManager )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, 
		"PT: PT_ScreenManagerSetSecondScreen: Invalid ScreenManager!\n");
		return;
	}

	PT_ScreenList* node = PT_ScreenListGet(ptScreenManager->screenList, utf8_screenName);
	
	if ( node )
	{
		if ( !ptScreenManager->secondScreen )
		{
			if ( ptScreenManager->firstScreen == node->value )
			{
				SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, 
				"PT: PT_ScreenManagerSetSecondScreen: The same screen is running on first slot\n");
				return;
			}
			
			ptScreenManager->secondScreen = node->value;
			return;
		}
		
		if ( ptScreenManager->firstScreen == ptScreenManager->secondScreen )
		{
			SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, 
			"PT: PT_ScreenManagerSetSecondScreen: The same screen is running on first slot\n");
			return;
		}
		
		ptScreenManager->secondScreen = node->value;
	}
	else {
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, 
		"PT: PT_ScreenManagerSetSecondScreen: Can't find screen: %s\n", utf8_screenName);
	}

}//PT_ScreenManagerSetSecondScreen

SDL_bool PT_ScreenManagerLoadScreen( const char* utf8_screenName ) {
	if ( !ptScreenManager )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, 
		"PT: PT_ScreenManagerLoadScreen: Invalid ScreenManager!\n");
		return SDL_FALSE;
	}

	PT_JsonList* node = PT_JsonListGet(ptScreenManager->jsonList, utf8_screenName);

	if ( !node )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_ScreenManagerLoadScreen: Cannot find screen: %s to be loaded\n", utf8_screenName);
		return SDL_FALSE;
	}

	Uint8* screenName = node->index->utf8_string;
	PT_Screen* screen = PT_ScreenCreate(node->value);
	
	ptScreenManager->screenList = PT_ScreenListAdd(ptScreenManager->screenList, 
		(char*)screenName, screen);

	return SDL_TRUE;
}//PT_ScreenManagerLoadScreen

void PT_ScreenManagerUnloadScreen( const char* utf8_screenName ) {
	if ( !ptScreenManager )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, 
		"PT: PT_ScreenManagerUnloadScreen: Invalid ScreenManager!\n");
		return;
	}

	ptScreenManager->screenListToFree = PT_StringListAdd(ptScreenManager->screenListToFree, 
		utf8_screenName, NULL);	
	
}//PT_ScreenManagerUnloadScreen

void PT_ScreenManagerUpdate( Sint32 elapsedTime ) {

	if ( ptScreenManager->firstScreen )
	{
		PT_ScreenUpdate(ptScreenManager->firstScreen, elapsedTime);
	}
	if ( ptScreenManager->secondScreen )
	{
		PT_ScreenUpdate(ptScreenManager->secondScreen, elapsedTime);
	}
	
	PT_ScreenManagerFreeScreens();
}//PT_ScreenManagerUpdate

void PT_ScreenManagerDraw( ) {

	if ( ptScreenManager->firstScreen )
	{
		PT_ScreenDraw(ptScreenManager->firstScreen);
	}
	if ( ptScreenManager->secondScreen )
	{
		PT_ScreenDraw(ptScreenManager->secondScreen);
	}
}//PT_ScreenManagerDraw

void PT_ScreenManagerFreeScreens( ) {
	if ( !ptScreenManager->screenListToFree )
	{
		return;
	}
	
	PT_StringList* pList = ptScreenManager->screenListToFree;
	while ( pList )
	{
		PT_ScreenList* node = PT_ScreenListGet(ptScreenManager->screenList, 
			(char*)pList->index->utf8_string);
		if ( node )
		{
			if ( node->value == ptScreenManager->firstScreen )
			{
				ptScreenManager->firstScreen = NULL;
			}
			if ( node->value == ptScreenManager->secondScreen )
			{
				ptScreenManager->secondScreen = NULL;
			}
			
			ptScreenManager->screenList = PT_ScreenListRemove(ptScreenManager->screenList, 
				(char*)node->index->utf8_string);
		}
		else {
			SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, 
			"PT: PT_ScreenManagerFreeScreens: Cannot find screen: %s\n", 
			(char*)pList->index->utf8_string);
		}
		
		pList = pList->next;
	}
	
	PT_StringListDestroy(ptScreenManager->screenListToFree);
	ptScreenManager->screenListToFree = NULL;	
}//PT_ScreenManagerFreeScreens

//===================================== PRIVATE Functions implementations 
//================= PT_ScreenList
PT_ScreenList* PT_ScreenListCreate( const char* utf8_index, PT_Screen* value ) {
	PT_ScreenList* ptScreenList = (PT_ScreenList*)malloc(sizeof(struct pt_screenList));
	
	ptScreenList->index = PT_StringCreate();
	if ( !PT_StringInsert(&(ptScreenList->index), utf8_index, 0) )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_ScreenListCreate!\n");
	}
	
	ptScreenList->value = value;
	ptScreenList->next = NULL;
	
	return ptScreenList;
}//PT_ScreenListCreate

void PT_ScreenListDestroy( PT_ScreenList* _this ) {
	if ( !_this )
	{
		return;
	}


	while ( _this )
	{
		PT_ScreenList* tmp = _this->next;
		
		if ( _this->value )
		{
			PT_ScreenDestroy(_this->value);
		}
		PT_StringDestroy(_this->index);
		
		free(_this);
		_this = tmp;
	}
}//PT_ScreenListDestroy

PT_ScreenList* PT_ScreenListAdd( PT_ScreenList* _this, const char* utf8_index, PT_Screen* value ) {
	
	if ( !_this )
	{
		return PT_ScreenListCreate(utf8_index, value);
	}


	PT_ScreenList* pList = _this;
	
	while ( pList )
	{
		if ( PT_StringMatch(pList->index, utf8_index) )
		{
			if ( value )
			{
				SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, 
				"PT: PT_ScreenListAdd: Screen already added: %s\n", utf8_index);
				PT_ScreenDestroy(value);
			}
			return _this;
		}
		pList = pList->next;
	}

	PT_ScreenList* newNode = PT_ScreenListCreate(utf8_index, value);
	newNode->next = _this;
	
	return newNode;
}//PT_ScreenListAdd

PT_ScreenList* PT_ScreenListGet( PT_ScreenList* _this, const char* utf8_index ) {
	while ( _this )
	{
		if ( PT_StringMatch(_this->index, utf8_index) )
		{
			return _this;
		}
		
		_this = _this->next;
	}
	
	return NULL;
}//PT_ScreenListGet

PT_ScreenList* PT_ScreenListRemove( PT_ScreenList* _this, const char* utf8_index ) {
	PT_ScreenList* pList = _this;
	PT_ScreenList* previous = NULL;
	
	while ( pList )
	{
		PT_ScreenList* next = pList->next;
		
		if ( PT_StringMatch(pList->index, utf8_index) ) 
		{
			if ( pList->value )
			{
				PT_ScreenDestroy(pList->value);
			}
			PT_StringDestroy(pList->index);
			free(pList);
			
			if ( previous )
			{
				previous->next = next;
				return _this;
			}
			else {
				pList = next;
				return pList;
			}
		}
		
		previous = pList;
		pList = next;
	}
	
	return _this;
}//PT_ScreenListRemove



