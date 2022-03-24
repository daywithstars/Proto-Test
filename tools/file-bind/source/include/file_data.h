/*
Copyright 2022 daywithstars

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

#ifndef _FILE_DATA_H_
#define _FILE_DATA_H_

#include <stdio.h>
#include <stdint.h>


typedef enum {
	FILE_LABEL_UNKNOWN=0,
	FILE_LABEL_TEXT,
	FILE_LABEL_IMAGE,
	FILE_LABEL_TRUE_TYPE_FONT,
	FILE_LABEL_SOUND
}FileLabel;

typedef struct {
	uint8_t label;
	char* extensionName;
	char* name;
	uint32_t _size;
	uint8_t* data;
}FileData;


FileData createFileData( const char* filename );
void destroyFileData( FileData* fileData );

int8_t bindFileData( FileData* fileData, FILE* file );

#endif /* _FILE_DATA_H_ */



