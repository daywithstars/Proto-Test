/*
Copyright 2022 daywithstars

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

#ifndef _PT_FILEDATAHANDLER_H_
#define _PT_FILEDATAHANDLER_H_

#include <stdint.h>

#include <SDL_stdinc.h>

#include <PT_String.h>



typedef enum {
	PT_FILEDATA_LABEL_UNKNOWN=0,
	PT_FILEDATA_LABEL_TEXT,
	PT_FILEDATA_LABEL_IMAGE,
	PT_FILEDATA_LABEL_TRUETYPE_FONT,
	PT_FILEDATA_LABEL_SOUND
}PT_FileDataLabel;

typedef struct {
	PT_FileDataLabel label;
	
	char* extension;
	char* name;
	uint8_t* data;
	uint32_t _size;
}PT_FileData;

typedef struct pt_file_data_list {
	PT_String* index;
	
	unsigned int numValues;
	PT_FileData* values;
	
	struct pt_file_data_list* next;
}PT_FileDataList;

typedef struct pt_file_data_handler PT_FileDataHandler;


PT_FileDataHandler* PT_FileDataHandler_Create( );
void PT_FileDataHandler_Destroy( PT_FileDataHandler* _this );

SDL_bool PT_FileDataHandler_LoadBlock( PT_FileDataHandler* _this, const char* path );

PT_FileDataList* PT_FileDataHandler_GetFileDataList( PT_FileDataHandler* _this );
PT_FileDataList* PT_FileDataHandler_GetFileDataListByExtension( PT_FileDataHandler* _this, 
	const char* ext );

#endif /* _PT_FILEDATAHANDLER_H_ */



