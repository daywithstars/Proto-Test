/*
Copyright 2022 daywithstars

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

#include <stdlib.h>
#include <string.h>
#include <malloc.h>

#include <file_data.h>


/* Supported formats: 
- Texts:
  .txt
-Images:
  .png
  .bmp
  .jpeg
  .jpg
  .gif
  .tiff
-Fonts
  .tff
-Sound
  .wav
  .mp3	
  .ogg
*/
FileLabel getFileLabel( const char* filename ) {
	const char* extension = strrchr(filename, '.');
	FileLabel label = FILE_LABEL_UNKNOWN;
	
	if ( !extension )
	{
		printf("Unknown extension\n");
		return label;
	}
	
	/* Texts */
	if ( !strcmp(extension, ".txt") )
	{
		label = FILE_LABEL_TEXT;
	}
	/* Images */
	else if ( !strcmp(extension, ".png") )
	{
		label = FILE_LABEL_IMAGE;
	}
	else if ( !strcmp(extension, ".bmp") )
	{
		label = FILE_LABEL_IMAGE;
	}
	else if ( !strcmp(extension, ".jpeg") )
	{
		label = FILE_LABEL_IMAGE;
	}
	else if ( !strcmp(extension, ".jpg") )
	{
		label = FILE_LABEL_IMAGE;
	}
	else if ( !strcmp(extension, ".gif") )
	{
		label = FILE_LABEL_IMAGE;
	}
	else if ( !strcmp(extension, ".tiff") )
	{
		label = FILE_LABEL_IMAGE;
	}
	
	/* Fonts */
	else if ( !strcmp(extension, ".ttf") )
	{
		label = FILE_LABEL_TRUE_TYPE_FONT;
	}
	
	/* Sounds */
	else if ( !strcmp(extension, ".wav") )
	{
		label = FILE_LABEL_SOUND;
	}
	else if ( !strcmp(extension, ".mp3") )
	{
		label = FILE_LABEL_SOUND;
	}
	else if ( !strcmp(extension, ".ogg") )
	{
		label = FILE_LABEL_SOUND;
	}
	else {
		printf("Unknown extension\n");
	}
	
	return label;
}//getFileLabel

FileData createFileData( const char* filename, int defaultPath ) {
	FileData fileData;
	memset(&fileData, 0, sizeof(FileData));
	
	/* .label */
	fileData.label = getFileLabel(filename);
	
	/* .extension */
	const char* extension = strrchr(filename, '.');
	if ( extension )
	{
		fileData.extensionName = (char*)malloc(strlen(extension) + 1);
		if ( !fileData.extensionName )
		{
			printf("Not enough memory\n");
			return fileData;
		}
		fileData.extensionName[0] = '\0';
		strcpy(fileData.extensionName, extension);
	}
	else {
		fileData.extensionName = (char*)malloc(strlen(".unknown") + 1);
		if ( !fileData.extensionName )
		{
			printf("Not enough memory\n");
			return fileData;
		}
		fileData.extensionName[0] = '\0';
		strcpy(fileData.extensionName, ".unknown");
	}
	
	/* .name */
	uint32_t filenameSize = strlen(filename) + 1;
	uint32_t pathLength = filenameSize;
	if ( defaultPath )
	{
		pathLength += strlen("../input/");
	}
	
	fileData.name = (char*)malloc(filenameSize);
	if ( !fileData.name )
	{
		printf("Not enough memory: FILE %s, LINE %d\n",
		 __FILE__, __LINE__);
		 
		return fileData;
	}
	fileData.name[0] = '\0';
	strcpy(fileData.name, filename);
	
	/* ._size & .data */
	char* path = (char*)malloc(pathLength);
	if ( !path )
	{
		printf("Not enough memory: FILE %s, LINE %d\n", 
		__FILE__, __LINE__);
		
		free(fileData.name);
		
		memset(&fileData, 0, sizeof(FileData));
		return fileData;
	}
	path[0] = '\0';
	if ( defaultPath )
	{
		strcat(path, "../input/");
	}
	strcat(path, filename);
	
	FILE* file = fopen(path, "rb");
	if ( !file )
	{
		printf("Cannot open %s, FILE %s, LINE %d\n", 
		filename, __FILE__, __LINE__);
		
		free(fileData.name);
		free(path);
		
		memset(&fileData, 0, sizeof(FileData));
		return fileData;
	}
	
	if ( fseek(file, 0L, SEEK_END) < 0 )
	{
		printf("Cannot find the size of file: %s, FILE %s, LINE %d\n", 
		filename, __FILE__, __LINE__);
		
		free(fileData.name);
		free(path);
		fclose(file);
		
		memset(&fileData, 0, sizeof(FileData));
		return fileData;
	}
	fileData._size = ftell(file);
	if ( fseek(file, 0L, SEEK_SET) < 0 )
	{
		printf("Cannot set the start of file: %s, FILE %s, LINE %d\n", 
		filename, __FILE__, __LINE__);
		
		free(fileData.name);
		free(path);
		fclose(file);
		
		memset(&fileData, 0, sizeof(FileData));
		return fileData;
	}
	
	fileData.data = (uint8_t*)malloc(fileData._size);
	if ( !fileData.data )
	{
		printf("Not enough memory: FILE %s, LINE %d\n", 
		__FILE__, __LINE__);
		
		free(fileData.name);
		free(path);
		fclose(file);
		
		memset(&fileData, 0, sizeof(FileData));
		return fileData;
	}
	if ( fread(fileData.data, sizeof(uint8_t), fileData._size, file) != fileData._size )
	{
		printf("File cannot be fully read: %s, FILE %s, LINE %d\n",
		filename, __FILE__, __LINE__);
		
		free(fileData.name);
		free(path);
		fclose(file);
		free(fileData.data);
		
		memset(&fileData, 0, sizeof(FileData));
		return fileData;
	}

	free(path);	
	fclose(file);
	return fileData;
}

FileData loadFileData( FILE* file ) {
	FileData fileData = {0};
	
	if ( !file )
	{	
		printf("Invalid file: FILE %s, LINE %d\n", __FILE__, __LINE__);
		return fileData;
	}
	
	
	
	return fileData;
}//loadFileData

int saveFileData( FileData fileData, const char* path ) {
	return 1;
}//saveFileData

void destroyFileData( FileData* fileData ) {
	if ( !fileData )
	{
		return;
	}

	if ( fileData->name )
	{
		free(fileData->name);
	}
	if ( fileData->data )
	{
		free(fileData->data);
	}
}

int bindFileData( FileData* fileData, FILE* file ) {
	/*
		File header
		
		1 Byte = label
		4 Byte(unsigned) = extension name length
		n Byte = extension name
		4 Byte(unsigned) = filename length
		n Byte = filename
		4 Byte(unsigned) = data length
		n Byte = data
	*/
	
	if ( !fileData || !file )
	{
		return 0;
	}
	
	if ( fwrite(&fileData->label, sizeof(uint8_t), 1, file) != 1 )
	{
		printf("Number of items does not match: FILE %s, LINE %d\n", 
		__FILE__, __LINE__);
	}
	
	uint32_t fileExtensionNameLength = strlen(fileData->extensionName) + 1;
	if ( fwrite(&fileExtensionNameLength, sizeof(uint32_t), 1, file) != 1 )
	{
		printf("Number of items does not match: FILE %s, LINE %d\n", 
		__FILE__, __LINE__);
	}
	
	if ( fwrite(fileData->extensionName, sizeof(char), fileExtensionNameLength, file) != fileExtensionNameLength )
	{
		printf("Number of items does not match: FILE %s, LINE %d\n", 
		__FILE__, __LINE__);
	}
	
	uint32_t filenameLength = strlen(fileData->name) + 1;
	if ( fwrite(&filenameLength, sizeof(uint32_t), 1, file) != 1 )
	{
		printf("Number of items does not match: FILE %s, LINE %d\n", 
		__FILE__, __LINE__);
	}
	
	if ( fwrite(fileData->name, sizeof(char), filenameLength, file) != filenameLength )
	{
		printf("Number of items does not match: FILE %s, LINE %d\n", 
		__FILE__, __LINE__);
	}
	
	if ( fwrite(&fileData->_size, sizeof(uint32_t), 1, file) != 1 )
	{
		printf("Number of items does not match: FILE %s, LINE %d\n", 
		__FILE__, __LINE__);
	}
	
	if ( fwrite(fileData->data, sizeof(uint8_t), fileData->_size, file) != fileData->_size )
	{
		printf("Number of items does not match: FILE %s, LINE %d\n", 
		__FILE__, __LINE__);
	}
	
	return 1;
}






