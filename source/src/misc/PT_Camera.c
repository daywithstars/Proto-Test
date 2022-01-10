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

#include <PT_Camera.h>
#include <PT_Parse.h>


typedef struct pt_camera {
	Sint32 x;
	Sint32 y;
	Uint16 width;
	Uint16 height;
}PT_Camera;

static PT_Camera* ptCamera = NULL;


//===================================== PRIVATE Functions

SDL_bool PT_CameraParse( );

//===================================== PUBLIC Functions

SDL_bool PT_CameraCreate() {
	if ( ptCamera )
	{
		return SDL_TRUE;
	}
	
	ptCamera = (PT_Camera*)malloc(sizeof(PT_Camera));
	if ( !ptCamera )
	{
		return SDL_FALSE;
	}
	SDL_memset(ptCamera, 0, sizeof(PT_Camera));
	
	if ( !PT_CameraParse() )
	{
		PT_CameraDestroy();
		return SDL_FALSE;
	}

	return SDL_TRUE;
}//PT_CameraCreate

void PT_CameraDestroy() {
	if ( !ptCamera )
	{
		return;
	}
	
	free(ptCamera);
}//PT_CameraDestroy

Uint16 PT_CameraGetWidth() {
	return ptCamera->width;
}
Uint16 PT_CameraGetHeight() {
	return ptCamera->height;
}

void PT_CameraSetPosition( Sint32 x, Sint32 y ) {
	ptCamera->x = x;
	ptCamera->y = y;
}

void PT_CameraGetRenderDistance( Uint32* startColumn, Uint32* startRow, Uint32* maxColumn, Uint32* maxRow,
	Uint16 tileSize ) {
	
	
	if ( ptCamera->x != 0 )
	{
		*startColumn =  tileSize / ptCamera->x; 
		*maxColumn = (tileSize + ptCamera->width) / ptCamera->x;
	}
	else {
		*startColumn = 0;
		*maxColumn = ptCamera->width / tileSize;
	}
	
	if ( ptCamera->y != 0 )
	{
		*startRow = tileSize / ptCamera->y;
		*maxRow = (tileSize + ptCamera->height) / ptCamera->y;		
	}
	else {
		*startRow = 0;
		*maxRow = ptCamera->height / tileSize;
	}
}//PT_CameraGetRenderDistance

//===================================== PRIVATE Functions

SDL_bool PT_CameraParse( ) {
	PT_Parse* parse = PT_ParseCreate();
	if ( !parse )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_CameraParse!\n");
		return SDL_FALSE;
	}
	if ( !PT_ParseOpenFile(parse, "settings.json", SDL_TRUE) )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_CameraParse!\n");
		PT_ParseDestroy(parse);
		return SDL_FALSE;
	}
	
	json_object_entry entry = PT_ParseGetObjectEntry(parse, "camera width");
	if ( !entry.name )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_CameraParse!\n");
		PT_ParseDestroy(parse);
		return SDL_FALSE;
	}
	ptCamera->width = entry.value->u.integer;
	
	entry = PT_ParseGetObjectEntry(parse, "camera height");
	if ( !entry.name )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_CameraParse!\n");
		PT_ParseDestroy(parse);
		return SDL_FALSE;
	}
	ptCamera->height = entry.value->u.integer;

	
	PT_ParseDestroy(parse);
	return SDL_TRUE;
}//PT_CameraParse





