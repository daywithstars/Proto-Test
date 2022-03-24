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

#include <PT_FileDataHandler.h>


struct pt_file_data_handler {
	int tmp;
};


PT_FileDataHandler* PT_FileDataHandler_Create( ) {
	PT_FileDataHandler* _this = (PT_FileDataHandler*)malloc(sizeof(struct pt_file_data_handler));
	if ( !_this )
	{	
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, 
		"PT: PT_FileDataHandler_Create: Not enough memory\n");
		
		return NULL;
	}
	SDL_memset(_this, 0, sizeof(struct pt_file_data_handler));
	
	return _this;
}//PT_FileDataHandler_Create

void PT_FileDataHandler_Destroy( PT_FileDataHandler* _this ) {
	if ( !_this )
	{
		return;
	}
	
	free(_this);
}//PT_FileDataHandler_Destroy

SDL_bool PT_FileDataHandler_LoadBlock( const char* path ) {

	return SDL_TRUE;
}//PT_FileDataHandler_LoadBlock

PT_FileDataList* PT_FileDataHandler_GetFilDataListByExtension( PT_FileDataHandler* _this, 
	const char* ext ) {
	
	PT_FileDataList* pList = NULL;
	
	
	return pList;
}//PT_FileDataHandler_GetFilDataListByExtension





