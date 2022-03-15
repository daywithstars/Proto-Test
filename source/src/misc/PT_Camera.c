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
#include <SDL_rect.h>

#include <PT_Camera.h>
#include <PT_Parse.h>
#include <PT_Graphics.h>


typedef struct pt_camera {
	Sint32 x;
	Sint32 y;
	Sint32 saveX;
	Sint32 saveY;
	
	Uint16 width;
	Uint16 height;
	
	PT_Collider* colliders;
	unsigned int numColliders;
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
		SDL_LogMessage(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_CRITICAL,
		"PT_CameraCreate: Not enough memory\n");
		SDL_LogMessage(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_CRITICAL,
		"PT_CameraCreate: FILE %s, LINE %d\n", __FILE__, __LINE__);
		
		return SDL_FALSE;
	}
	SDL_memset(ptCamera, 0, sizeof(PT_Camera));
	
	if ( !PT_CameraParse() )
	{
		SDL_LogMessage(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_ERROR,
		"PT_CameraCreate: Cannot parse\n");
		SDL_LogMessage(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_ERROR,
		"PT_CameraCreate: FILE %s, LINE %d\n", __FILE__, __LINE__);	
	
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
	
	if ( ptCamera->colliders )
	{
		for ( unsigned int i = 0; i < ptCamera->numColliders; i++ )
		{
			PT_ColliderDestroy(&ptCamera->colliders[i]);
		}
		
		free(ptCamera->colliders);
	}
	
	free(ptCamera);
}//PT_CameraDestroy

Uint16 PT_CameraGetWidth() {
	return ptCamera->width;
}
Uint16 PT_CameraGetHeight() {
	return ptCamera->height;
}

Sint32 PT_CameraGetX() {
	return ptCamera->x;
}
Sint32 PT_CameraGetY() {
	return ptCamera->y;
}

void PT_CameraSetPosition( Sint32 x, Sint32 y ) {
	
	ptCamera->x = x;
	ptCamera->y = y;
	
	const SDL_Rect rect = { -x, -y, ptCamera->width + x, ptCamera->height + y };
	
	PT_GraphicsSetViewport(&rect);
}

void PT_CameraGetRenderDistance( Uint32* startColumn, Uint32* startRow, Uint32* maxColumn, Uint32* maxRow,
	Uint32 tileLayerWidth, Uint32 tileLayerHeight, Uint16 tilewidth, Uint16 tileheight ) {
	
	
	//Horizontal: Logic
	if ( ptCamera->x != 0 )
	{
		*startColumn =  (ptCamera->x < 0 ? -(ptCamera->x / tilewidth) : ptCamera->x) / tilewidth; 
		*maxColumn = (ptCamera->width / tilewidth) + (*startColumn) + 1;
	}
	else {
		*startColumn = 0;
		*maxColumn = ptCamera->width / tilewidth;
	}
	
	//Horizontal: Limits
	if ( tilewidth * tileLayerWidth <= ptCamera->width )
	{
		*startColumn = 0;
		*maxColumn = tileLayerWidth;
	}
	else if ( ptCamera->x >= (Sint32)(tilewidth * tileLayerWidth) - ptCamera->width )
	{
		*startColumn = ((tilewidth * tileLayerWidth) - ptCamera->width) / tilewidth;
		*maxColumn = tileLayerWidth;
	}
	
	//Vertical: Logic
	if ( ptCamera->y != 0 )
	{
		*startRow = (ptCamera->y < 0 ? -(ptCamera->y / tileheight) : ptCamera->y) / tileheight;
		*maxRow = (ptCamera->height / tileheight) + (*startRow) + 1;		
	}
	else {
		*startRow = 0;
		*maxRow = ptCamera->height / tileheight;
	}
	
	//Vertical: Limits
	if ( tileheight * tileLayerHeight <= ptCamera->height )
	{
		*startRow = 0;
		*maxRow = tileLayerHeight;
	}
	else if ( ptCamera->y >= (Sint32)(tileheight * tileLayerHeight) - ptCamera->height )
	{
		*startRow = ((tileheight * tileLayerHeight) - ptCamera->height) / tileheight;
		*maxRow = tileLayerHeight;
	}
}//PT_CameraGetRenderDistance

void PT_CameraGetColliders( PT_Collider** colliders, unsigned int* num ) {
	
	*colliders = ptCamera->colliders;
	*num = ptCamera->numColliders;
}

void PT_CameraSavePosition( ) {
	ptCamera->saveX = ptCamera->x;
	ptCamera->saveY = ptCamera->y;
}

void PT_CameraLoadPosition( ) {
	ptCamera->x = ptCamera->saveX;
	ptCamera->y = ptCamera->saveY;
}

void PT_CameraDraw( ) {
	for ( unsigned int i = 0; i < ptCamera->numColliders; i++ )
	{
		PT_ColliderDraw(ptCamera->colliders[i], ptCamera->x, ptCamera->y);
	}
}//PT_CameraDraw

//===================================== PRIVATE Functions

SDL_bool PT_CameraParse( ) {
	PT_Parse* parse = PT_ParseCreate();
	if ( !parse )
	{
		SDL_LogMessage(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_ERROR,
		"PT_CameraParse: Cannot create PT_Parse\n");
		SDL_LogMessage(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_ERROR,
		"PT_CameraParse: FILE %s, LINE %d\n", __FILE__, __LINE__);
		
		return SDL_FALSE;
	}
	if ( !PT_ParseOpenFile(parse, "settings.json", SDL_TRUE) )
	{
		SDL_LogMessage(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_ERROR,
		"PT_CameraParse: Cannot open file to parse\n");
		SDL_LogMessage(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_ERROR,
		"PT_CameraParse: FILE %s, LINE %d\n", __FILE__, __LINE__);
		
		PT_ParseDestroy(parse);
		return SDL_FALSE;
	}
	
	json_object_entry entry = PT_ParseGetObjectEntry(parse, "camera width");
	if ( !entry.name )
	{
		SDL_LogMessage(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_WARN,
		"PT_CameraParse: Cannot find \"camera\" \"width\" element\n\
		default value will be 640\n");
		SDL_LogMessage(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_WARN,
		"PT_CameraParse: FILE %s, LINE %d\n", __FILE__, __LINE__);
		
		ptCamera->width = 640;
	}
	else {
		ptCamera->width = entry.value->u.integer;
	}
	
	entry = PT_ParseGetObjectEntry(parse, "camera height");
	if ( !entry.name )
	{
		SDL_LogMessage(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_WARN,
		"PT_CameraParse: Cannot find \"camera\" \"height\" element\n\
		default value will be 480\n");
		SDL_LogMessage(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_WARN,
		"PT_CameraParse: FILE %s, LINE %d\n", __FILE__, __LINE__);
		
		ptCamera->height = 480;
	}
	else {
		ptCamera->height = entry.value->u.integer;
	}
	
	entry = PT_ParseGetObjectEntry(parse, "camera colliders");
	if ( entry.name )
	{
		ptCamera->colliders = (PT_Collider*)malloc(sizeof(PT_Collider) * entry.value->u.array.length);
		if ( !ptCamera->colliders )
		{
			SDL_LogMessage(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_CRITICAL,
			"PT_CameraParse: Not enough memory\n");
			SDL_LogMessage(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_CRITICAL,
			"PT_CameraParse: FILE %s, LINE %d\n", __FILE__, __LINE__);
		
			PT_ParseDestroy(parse);
			return SDL_FALSE;
		}
		ptCamera->numColliders = entry.value->u.array.length;
		
		
		for ( unsigned int i = 0; i < ptCamera->numColliders; i++ )
		{
			ptCamera->colliders[i] = PT_ColliderCreate(entry.value->u.array.values[i]);
			if ( !ptCamera->colliders[i].name )
			{
				SDL_LogMessage(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_CRITICAL,
				"PT_CameraParse: Cannot create PT_Collider\n");
				SDL_LogMessage(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_CRITICAL,
				"PT_CameraParse: FILE %s, LINE %d\n", __FILE__, __LINE__);
			
				continue;
			}
		}
	}

	
	PT_ParseDestroy(parse);
	return SDL_TRUE;
}//PT_CameraParse




