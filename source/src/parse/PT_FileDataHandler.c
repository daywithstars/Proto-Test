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
	PT_FileDataList* list;
};



//===================================== PRIVATE Functions 

PT_FileDataList* PT_FileDataListCreate( PT_FileDataList* _this, const char* utf8_index, 
	PT_FileData value );

void PT_FileDataListDestroy( PT_FileDataList* _this );

PT_FileDataList* PT_FileDataListCat( PT_FileDataList* _this, const char* utf8_index, PT_FileData value );

PT_FileDataList* PT_FileDataListGet( PT_FileDataList* _this, const char* utf8_index );


//===================================== PUBLIC Functions

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
	
	PT_FileDataListDestroy(_this->list);
	
	free(_this);
}//PT_FileDataHandler_Destroy

SDL_bool PT_FileDataHandler_LoadBlock( PT_FileDataHandler* _this, const char* path ) {
	if ( !path || !_this )
	{
		return SDL_FALSE;
	}
	
	FILE* file = fopen(path, "rb");
	if ( !file )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, 
		"PT: PT_FileDataHandler_LoadBlock: Cannot open file: %s\n", path);
		return SDL_FALSE;
	}

	/*
		v0.1.0
		
		// Block head
		
		2 Byte = version major
		2 Byte = version minor
		2 Byte = version patch
		
		// Files section

		1 Byte = label
		4 Byte(unsigned) = extension name length
		n Byte = extension name
		4 Byte(unsigned) = filename length
		n Byte = filename
		4 Byte(unsigned) = data length
		n Byte = data
	*/
	
	uint16_t vMajor = 0;
	
	if ( fread(&vMajor, sizeof(uint16_t), 1, file) != 1 )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, 
		"PT: PT_FileDataHandler_LoadBlock: Wrong number of read items\n");
		
		fclose(file);
		return SDL_FALSE;
	}
	if ( fseek(file, 4, SEEK_CUR) < 0 )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, 
		"PT: PT_FileDataHandler_LoadBlock: Cannot jump with fseek\n");
		
		fclose(file);
		return SDL_FALSE;
	}
	
	if ( vMajor != 0 )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, 
		"PT: PT_FileDataHandler_LoadBlock: The DataFile block version is not supported\n");
		
		fclose(file);
		return SDL_FALSE;
	} 
	
	size_t totalBytes = 6;
	int i = 0;
	while ( SDL_TRUE )
	{
		PT_FileData fileData;
		SDL_memset(&fileData, 0, sizeof(PT_FileData));

		/* .label */
		size_t freadReturn =
		fread(&fileData.label, sizeof(uint8_t), 1, file);
		if ( freadReturn != 1 )
		{
			SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, 
			"PT: PT_FileDataHandler_LoadBlock: Wrong number of read items\n");
			break;
		}
		totalBytes += 1;
		
		/* .extension */
		uint32_t extensionLength = 0;
		freadReturn =
		fread(&extensionLength, sizeof(uint32_t), 1, file);
		if ( freadReturn != 1 )
		{
			SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, 
			"PT: PT_FileDataHandler_LoadBlock: Wrong number of read items\n");
			break;
		}
		totalBytes += 4;
		
		fileData.extension = (char*)malloc(extensionLength);
		if ( !fileData.extension )
		{
			SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, 
			"PT: PT_FileDataHandler_LoadBlock: Not enough memory\n");
			
			fclose(file);			
			return SDL_FALSE;		
		}
		
		freadReturn =
		fread(fileData.extension, sizeof(char), extensionLength, file);
		if ( freadReturn != extensionLength )
		{
			SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, 
			"PT: PT_FileDataHandler_LoadBlock: Wrong number of read items\n");
			
			free(fileData.extension);
			break;
		}
		totalBytes += extensionLength;
		
		/* Segment jump */
		uint32_t segmentJumpLength = 0;
		freadReturn =
		fread(&segmentJumpLength, sizeof(uint32_t), 1, file);
		if ( freadReturn != 1 )
		{
			SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, 
			"PT: PT_FileDataHandler_LoadBlock: Wrong number of read items\n");
			
			free(fileData.extension);
			break;
		}
		totalBytes += 4;
		if ( fseek(file, segmentJumpLength, SEEK_CUR) < 0 )
		{
			SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, 
			"PT: PT_FileDataHandler_LoadBlock: Cannot jump with fseek\n");
			
			fclose(file);
			free(fileData.extension);
			return SDL_FALSE;
		}
		totalBytes += segmentJumpLength;
		
		/* .data */
		uint32_t dataLength = 0;
		freadReturn =
		fread(&dataLength, sizeof(uint32_t), 1, file);
		if ( freadReturn != 1 )
		{
			SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, 
			"PT: PT_FileDataHandler_LoadBlock: Wrong number of read items\n");
			
			free(fileData.extension);
			break;
		}
		totalBytes += 4;

		fileData.data = (uint8_t*)malloc(dataLength);
		if ( !fileData.data )
		{
			SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, 
			"PT: PT_FileDataHandler_LoadBlock: Not enough memory\n");
			
			fclose(file);			
			free(fileData.extension);
			return SDL_FALSE;	
		}
		
		freadReturn =
		fread(fileData.data, sizeof(uint8_t), dataLength, file);
		if ( freadReturn != dataLength )
		{
			SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, 
			"PT: PT_FileDataHandler_LoadBlock: Wrong number of read items\n");
			
			free(fileData.extension);
			free(fileData.data);
			break;
		}
		totalBytes += dataLength;
		fileData._size = dataLength;
		
		_this->list = PT_FileDataListCat(_this->list, fileData.extension, fileData);
	
		
		/* 
			Test if, it's the end of the file 
		*/
		char c = '\0';
		//Somehow the fseek function do not work with the feof function. 
		//So we actually read the file. 
		fread(&c, sizeof(char), 1, file);
		
		if ( feof(file) )
		{
			break;
		}
		else {
			if ( fseek(file, totalBytes, SEEK_SET) < 0 )
			{
				SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, 
				"PT: PT_FileDataHandler_LoadBlock: Failed to reset file.\n");
				
				free(fileData.extension);
				free(fileData.data);
				break;
			}
		}
		
		i++;
	}
	if ( feof(file) == 0 )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, 
		"PT: PT_FileDataHandler_LoadBlock: The block file was not successfully entirely read\n");
		
		fclose(file);
		return SDL_FALSE;
	}


	fclose(file);
	return SDL_TRUE;
}//PT_FileDataHandler_LoadBlock

PT_FileDataList* PT_FileDataHandler_GetFileDataList( PT_FileDataHandler* _this ) {
	if ( !_this )
	{
		return NULL;
	}
	
	return _this->list;
}//PT_FileDataHandler_GetFileDataList

PT_FileDataList* PT_FileDataHandler_GetFileDataListByExtension( PT_FileDataHandler* _this, 
	const char* ext ) {
	if ( !_this || !ext )
	{
		return NULL;
	}
	
	return PT_FileDataListGet(_this->list, ext);
}//PT_FileDataHandler_GetFilDataListByExtension


//===================================== PRIVATE Functions 

PT_FileDataList* PT_FileDataListCreate( PT_FileDataList* _this, const char* utf8_index, 
	PT_FileData value ) {

	if ( !_this )
	{
		_this = (PT_FileDataList*)malloc(sizeof(PT_FileDataList));
		if ( !_this )
		{
			SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_FileDataListCreate: Not enough memory\n");
			return NULL;
		}
		SDL_memset(_this, 0, sizeof(PT_FileDataList));
		
		_this->index = PT_StringCreate();
		if ( !PT_StringInsert(&(_this->index), utf8_index, 0) )
		{
			SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_FileDataListCreate!\n");
			PT_FileDataListDestroy(_this);
			return NULL;
		}
		
		_this->values = (PT_FileData*)malloc(sizeof(PT_FileData));
		if ( !_this->values )
		{
			SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_FileDataListCreate: Not enough memory\n");
			PT_FileDataListDestroy(_this);
			return NULL;
		}
		_this->numValues = 1;
		_this->values[0] = value;
	}
	else {	
	
		PT_FileData* newValues = (PT_FileData*)malloc(sizeof(PT_FileData) * (_this->numValues + 1));
		if ( !newValues )
		{
			SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_FileDataListCreate: Not enough memory\n");
			return _this;
		}
		
		unsigned int i = 0;
		while ( i < _this->numValues )
		{
			newValues[i] = _this->values[i];
			i ++;
		}
		_this->numValues ++;
		
		newValues[i] = value;
		
		free(_this->values);
		_this->values = newValues;
	}
	
	return _this;
}//PT_FileDataListCreate

void PT_FileDataListDestroy( PT_FileDataList* _this ) {
	if ( !_this )
	{
		return;
	}


	while ( _this )
	{
		PT_FileDataList* tmp = _this->next;
		
		if ( _this->values )
		{
			for ( unsigned int i = 0; i < _this->numValues; i++ )
			{
				if ( _this->values[i].extension )
				{
					free(_this->values[i].extension);
				}
				if ( _this->values[i].data )
				{
					free(_this->values[i].data);
				}
			}
			free(_this->values);
		}
		
		PT_StringDestroy(_this->index);
		
		free(_this);
		_this = tmp;
	}
}//PT_FileDataListDestroy

PT_FileDataList* PT_FileDataListCat( PT_FileDataList* _this, const char* utf8_index, PT_FileData value ) {
	if ( !_this )
	{
		return PT_FileDataListCreate(NULL, utf8_index, value);
	}
	
	PT_FileDataList* pList = _this;
	while ( pList )
	{
		if ( PT_StringMatch(pList->index, utf8_index) )
		{
			if ( value.extension && value.data )
			{
				_this = PT_FileDataListCreate(_this, NULL, value);
				return _this;
			}
		}
		pList = pList->next;
	}
	
	PT_FileDataList* newNode = PT_FileDataListCreate(NULL, utf8_index, value);
	newNode->next = _this;
	
	return newNode;
}//PT_FileDataListCat

PT_FileDataList* PT_FileDataListGet( PT_FileDataList* _this, const char* utf8_index ) {
	while ( _this )
	{
		if ( PT_StringMatch(_this->index, utf8_index) )
		{
			return _this;
		}
		
		_this = _this->next;
	}
	
	return NULL;
}//PT_FileDataListGet







