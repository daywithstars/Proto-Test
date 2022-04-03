/*
Copyright 2022 daywithstars

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <sys/stat.h>

#include <SDL_rwops.h>
#include <SDL_log.h>

#include <PT_String.h>
#include <PT_Parse.h>


#define PT_PARSE_MAX_ENTRY_NAME_SIZE 128


extern PT_String* gRootDir;

struct pt_parse {
	json_value* jsonValue;
	PT_String* jsonString;
};

//===================================== PRIVATE Functions

SDL_bool PT_ParseWriteJsonValue( PT_String** str, json_value* jsonValue, const char* elementName, 
	int shift );

//===================================== PUBLIC Functions

PT_Parse* PT_ParseCreate( ) {

	PT_Parse* _this = (PT_Parse*)malloc(sizeof(struct pt_parse));
	if ( !_this )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_ParseCreate: Not enough memory\n");
		return NULL;
	}

	_this->jsonString = PT_StringCreate();
	_this->jsonValue = NULL;
	
	return _this;
}//PT_ParseCreate

void PT_ParseDestroy( PT_Parse* _this ) {
	if ( !_this )
	{
		return;
	}
	
	if ( _this->jsonValue )
	{
		json_value_free(_this->jsonValue);
	}
	PT_StringDestroy(_this->jsonString);
	free(_this);
}//PT_ParseDestroy

SDL_bool PT_ParseLegalDirectory( const char* path, SDL_bool defaultPath ) {
	struct stat sb = {0};
	
	//Successfully opened the file
	if ( !defaultPath )
	{
		if ( stat(path, &sb) < 0 )
		{
			return SDL_FALSE;
		}
	}
	else {
		PT_String* strPath = PT_StringCreate();
		if ( !strPath )
		{
			return SDL_FALSE;
		}
		if ( !PT_StringInsert(&strPath, path, 0) )
		{
			PT_StringDestroy(strPath);
			return SDL_FALSE;
		}
		if ( !PT_StringInsert(&strPath, (char*)gRootDir->utf8_string, 0) )
		{
			PT_StringDestroy(strPath);
			return SDL_FALSE;
		}

		if ( stat((char*)strPath->utf8_string, &sb) < 0 )
		{
			return SDL_FALSE;
		}
	}
	//Check if it's not a directory
	if ( (sb.st_mode & S_IFMT) != S_IFDIR )
	{
		return SDL_FALSE;
	}
	
	SDL_bool stMode = (sb.st_mode & S_IRUSR) && ( sb.st_mode & S_IWUSR ) ? SDL_TRUE : SDL_FALSE;
	if ( !stMode )
	{
		return SDL_FALSE;
	}
	
	return SDL_TRUE;
}//PT_ParseLegalDirectory

SDL_bool PT_ParseOpenFile( PT_Parse* _this, const char* utf8_filePath, SDL_bool defaultPath ) {
	if ( !_this )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_ParseOpenFile: Invalid PT_Parse\n");
		return SDL_FALSE;
	}
	
	PT_String* strPath = PT_StringCreate();
	if ( !PT_StringInsert(&strPath, utf8_filePath, 0) )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_ParseOpenFile!\n");
	}
	if ( defaultPath )
	{
		if ( !PT_StringInsert(&strPath, (char*)gRootDir->utf8_string, 0) )
		{
			SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_ParseOpenFile!\n");
		}
	}
	
	SDL_RWops* file = SDL_RWFromFile((char*)strPath->utf8_string, "r");
	if ( !file )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_ParseOpenFile: Invalid filePath: %s\n",
		(char*)strPath->utf8_string);
		PT_StringDestroy(strPath);
		return SDL_FALSE;
	}
	
	Sint64 jsonStringSize = SDL_RWsize(file) + 1;
	json_char* jsonStr = (json_char*)malloc(jsonStringSize);
	
	if ( !jsonStr )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_ParseOpenFile: Not enough memory\n");
		PT_StringDestroy(strPath);
		SDL_RWclose(file);
		return SDL_FALSE;
	}
	
	SDL_RWread(file, jsonStr, sizeof(json_char), SDL_RWsize(file));
	jsonStr[jsonStringSize - 1] = '\0';
	
	if ( !PT_ParseLoadTemplate(_this, jsonStr) )
	{
		PT_StringDestroy(strPath);
		SDL_RWclose(file);
		free(jsonStr);
		return SDL_FALSE;
	}
	
	
	SDL_RWclose(file);
	PT_StringDestroy(strPath);
	free(jsonStr);
	return SDL_TRUE;
}//PT_ParseOpenFile

SDL_bool PT_ParseLoadTemplate( PT_Parse* _this, const json_char* jsonString ) {
	
	if ( _this->jsonValue )
	{
		json_value_free(_this->jsonValue);
	}
	
	_this->jsonValue = json_parse(
		jsonString,
		strlen(jsonString)
	);

	if ( !_this->jsonValue )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_ParseLoadTemplate: Unable to parse!");
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "* from: %s\n", jsonString);
		
		return SDL_FALSE;
	}
	
	PT_StringClear(&_this->jsonString);
	if ( !PT_StringInsert(&(_this->jsonString), (char*)jsonString, 0) )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_ParseLoadTemplate!\n");
	}
	
	return SDL_TRUE;
}//PT_ParseLoadTemplate

SDL_bool PT_ParseChangeValue_Integer( json_value* jsonValue, json_int_t value ) {

	if ( !jsonValue )
	{
		return SDL_FALSE;
	}
	
	if ( jsonValue->type == json_integer )
	{
		jsonValue->u.integer = value;
		
		return SDL_TRUE;
	}
	else {
		return SDL_FALSE;
	}
	
	return SDL_TRUE;
}//PT_ParseChangeValue_Integer

SDL_bool PT_ParseChangeValue_String( json_value* jsonValue, json_char* value ) {

	if ( !jsonValue || !value )
	{
		return SDL_FALSE;
	}
	
	if ( jsonValue->type == json_string )
	{
		
		free(jsonValue->u.string.ptr);
		jsonValue->u.string.ptr = (json_char*)malloc(sizeof(json_char) * (strlen(value) + 1));
		strcpy(jsonValue->u.string.ptr, value);
		jsonValue->u.string.length = strlen(value);
		
		return SDL_TRUE;
	}
	else {
		return SDL_FALSE;
	}
	
	return SDL_TRUE;
}//PT_ParseChangeValue_String

SDL_bool PT_ParseChangeValue_Double( json_value* jsonValue, double value ) {

	if ( !jsonValue )
	{
		return SDL_FALSE;
	}
	
	if ( jsonValue->type == json_double )
	{
		jsonValue->u.dbl = value;
		
		return SDL_TRUE;
	}
	else {
		return SDL_FALSE;
	}
	
	return SDL_TRUE;
}//PT_ParseChangeValue_Double

SDL_bool PT_ParseSaveOriginal( PT_Parse* _this, const char* utf8_filePath, SDL_bool defaultPath ) {
	if ( !_this )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_ParseSaveOriginal: Invalid PT_Parse\n");
		return SDL_FALSE;
	}
	
	PT_String* strPath = PT_StringCreate();
	if ( !PT_StringInsert(&strPath, utf8_filePath, 0) )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_ParseSaveOriginal!\n");
	}
	if ( defaultPath )
	{	
		if ( !PT_StringInsert(&strPath, (char*)gRootDir->utf8_string, 0) )
		{
			SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_ParseSaveOriginal!\n");
		}
	}
	
	SDL_RWops* file = SDL_RWFromFile((char*)strPath->utf8_string, "w");
	if ( !file )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_ParseSaveOriginal: Cannot create file: %s\n",
		(char*)strPath->utf8_string);
		PT_StringDestroy(strPath);
		return SDL_FALSE;
	}
	
	SDL_RWwrite(file, _this->jsonString->utf8_string, sizeof(Uint8), _this->jsonString->length);
	
	PT_StringDestroy(strPath);
	SDL_RWclose(file);
	return SDL_TRUE;
}//PT_ParseSaveOriginal

SDL_bool PT_ParseSaveJsonValue( json_value* jsonValue, const char* utf8_filePath, SDL_bool defaultPath ) {
	
	if ( !jsonValue )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_ParseSaveJsonValue: Invalid json_value\n");
		return SDL_FALSE;
	}
	
	PT_String* strPath = PT_StringCreate();
	if ( !PT_StringInsert(&strPath, utf8_filePath, 0) )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_ParseSaveJsonValue!\n");
	}
	if ( defaultPath )
	{	
		if ( !PT_StringInsert(&strPath, (char*)gRootDir->utf8_string, 0) )
		{
			SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_ParseSaveJsonValue!\n");
		}
	}
	
	SDL_RWops* file = SDL_RWFromFile((char*)strPath->utf8_string, "w");
	if ( !file )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_ParseSaveJsonValue: Cannot create file: %s\n",
		(char*)strPath->utf8_string);
		PT_StringDestroy(strPath);
		return SDL_FALSE;
	}
	PT_StringDestroy(strPath);


	PT_String* str = PT_StringCreate();
	if ( !str )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_ParseSaveJsonValue: Not enough memory\n");
		
		SDL_RWclose(file);
		return SDL_FALSE;
	}
	if ( !PT_ParseWriteJsonValue(&str, jsonValue, NULL, 0) )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_ParseSaveJsonValue!\n");
		
		SDL_RWclose(file);
		PT_StringDestroy(str);
		return SDL_FALSE;
	}
	else {
		SDL_RWwrite(file, str->utf8_string, sizeof(char), str->length);
		PT_StringDestroy(str);
	}

	SDL_RWclose(file);
	return SDL_TRUE;
}//PT_ParseSaveJsonValue

json_value* PT_ParseGetJsonValueFromFile( const char* utf8_filePath, SDL_bool defaultPath  ) {
	
	PT_String* strPath = PT_StringCreate();
	if ( !PT_StringInsert(&strPath, utf8_filePath, 0) )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_ParseGetJsonValueFromFile!\n");
	}
	if ( defaultPath )
	{
		if ( !PT_StringInsert(&strPath, (char*)gRootDir->utf8_string, 0) )
		{
			SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_ParseGetJsonValueFromFile!\n");
		}
	}
	
	SDL_RWops* file = SDL_RWFromFile((char*)strPath->utf8_string, "r");
	if ( !file )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_ParseGetJsonValueFromFile: Invalid filePath: %s\n",
		(char*)strPath->utf8_string);
		PT_StringDestroy(strPath);
		return NULL;
	}
	
	Sint64 jsonStringSize = SDL_RWsize(file);
	json_char* jsonStr = (json_char*)malloc(jsonStringSize + 1);
	
	if ( !jsonStr )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_ParseGetJsonValueFromFile: Not enough memory\n");
		PT_StringDestroy(strPath);
		SDL_RWclose(file);
		return NULL;
	}
	
	SDL_RWread(file, jsonStr, sizeof(json_char), SDL_RWsize(file));
	jsonStr[jsonStringSize] = '\0';
	
	json_value* jsonValue = json_parse(
		jsonStr,
		SDL_RWsize(file)
	);

	if ( !jsonValue )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_ParseGetJsonValueFromFile: Unable to parse!");
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "* from: %s\n", jsonStr);
		free(jsonStr);
		return NULL;
	}
	
	
	SDL_RWclose(file);
	PT_StringDestroy(strPath);
	free(jsonStr);
	
	return jsonValue;
}//PT_ParseGetJsonValueFromFile

json_value* PT_ParseGetJsonValuePointer( PT_Parse* _this ) {
	return _this->jsonValue;
}//PT_ParseGetJsonValuePointer

json_object_entry PT_ParseGetObjectEntry( PT_Parse* _this, const char* nameSequence ) {

	json_object_entry entry = { 0 };

	if ( !_this )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_ParseGetObjectEntry: Invalid PT_Parse\n");
		return entry;
	}
	if ( !_this->jsonValue )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_ParseGetObjectEntry: Invalid jsonValue\n");
		return entry;
	}
	json_value* jv = _this->jsonValue;
	
	entry = PT_ParseGetObjectEntry_json_value(jv, nameSequence);
	
	return entry;
}//PT_ParseGetObjectEntry

json_object_entry PT_ParseGetObjectEntry_json_value( json_value* _json_value, const char* nameSequence ) {

	json_object_entry entry = { 0 };
	PT_String* sequenceFound = PT_StringCreate();
	if ( !sequenceFound )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, 
		"PT: PT_ParseGetObjectEntry_json_value!\n");
		return entry;
	}	

	if ( !_json_value )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, 
		"PT: PT_ParseGetObjectEntry_json_value: Invalid _json_value\n");
		PT_StringDestroy(sequenceFound);
		return entry;
	}
	
	json_value* pJsonValue = _json_value;
	char entryName[PT_PARSE_MAX_ENTRY_NAME_SIZE];
	int nextArrayIt = 0;
		
	for ( Uint64 i = 0, j = 0; ( i < strlen(nameSequence) + 1 ) && pJsonValue; i++, j++ )
	{
		if ( nameSequence[i] == ' ' || nameSequence[i] == '\0' )
		{
			entryName[j] = '\0';
			j = 0;
			i += 1;//jump the ' ' char
			
			if ( pJsonValue )
			{
				if ( pJsonValue->type == json_object )
				{
					for ( json_int_t k = 0; k < pJsonValue->u.object.length; k++ )
					{
						if ( !strcmp(entryName, pJsonValue->u.object.values[k].name) )
						{
							if ( sequenceFound->length != 0 )
							{
								if ( !PT_StringInsert(&sequenceFound, " ", sequenceFound->length) )
								{
									SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, 
									"PT: PT_ParseGetObjectEntry_json_value\n");
								}
							}
							if ( !PT_StringInsert(&sequenceFound, entryName, sequenceFound->length) )
							{
								SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, 
								"PT: PT_ParseGetObjectEntry_json_value\n");
							}
							entry = pJsonValue->u.object.values[k];
							pJsonValue = pJsonValue->u.object.values[k].value;
							break;
						}
					}
				}
				else if ( pJsonValue->type == json_array )
				{	
					for ( json_int_t k = nextArrayIt; k < pJsonValue->u.array.length; k++ )
					{
						if ( pJsonValue->u.array.values[k]->type == json_object )
						{
							pJsonValue = pJsonValue->u.array.values[k];
							nextArrayIt ++;
							i = j = 0;
							break;
						}
					}
				}
			}
		}
		entryName[j] = nameSequence[i];
	}
	
	if ( !PT_StringMatchFast((char*)sequenceFound->utf8_string, nameSequence) )
	{
		PT_StringDestroy(sequenceFound);
		SDL_memset(&entry, 0, sizeof(json_object_entry));
		
		return entry;
	}
	
	PT_StringDestroy(sequenceFound);
	return entry;
}//PT_ParseGetObjectEntry_json_value


//===================================== PRIVATE Functions

SDL_bool PT_ParseWriteJsonValue( PT_String** str, json_value* jsonValue, 
	const char* elementName, int shift ) {
	
	if ( !str || !jsonValue || jsonValue->type == json_none )
	{
		return SDL_FALSE;
	}
	
	const char tab[] = { 0x09, 0x0 };
	
	if ( jsonValue->type == json_object )
	{
		if ( jsonValue->parent != NULL )
		{
			if ( elementName )
			{
				/*
					\t"elementName": {\n
				*/
				for ( int i = 0; i < shift; i++ )
				{
					PT_StringInsert(str, tab, (*str)->length);
				}
				PT_StringInsert(str, "\"", (*str)->length);
				PT_StringInsert(str, elementName, (*str)->length);
				PT_StringInsert(str, "\"", (*str)->length);
				PT_StringInsert(str, ": {\n", (*str)->length);
				
				for ( unsigned int i = 0; i < jsonValue->u.object.length; i++ )
				{	
					if ( !PT_ParseWriteJsonValue(str, jsonValue->u.object.values[i].value, 
						jsonValue->u.object.values[i].name, shift + 1))
					{
						return SDL_FALSE;
					}
				}
			
				/*
					\t},\n
				*/
				for ( int i = 0; i < shift; i++ )
				{
					PT_StringInsert(str, tab, (*str)->length);
				}
				PT_StringInsert(str, "},\n", (*str)->length);
			}
			else {
				/*
					\t+1{\n
				*/
				for ( int i = 0; i < shift + 1; i++ )
				{
					PT_StringInsert(str, tab, (*str)->length);
				}
				PT_StringInsert(str, "{\n", (*str)->length);

				for ( unsigned int i = 0; i < jsonValue->u.object.length; i++ )
				{	
					if ( !PT_ParseWriteJsonValue(str, jsonValue->u.object.values[i].value, 
						jsonValue->u.object.values[i].name, shift + 2))
					{
						return SDL_FALSE;
					}
				}
				
				/*
					\t+1},\n
				*/
				for ( int i = 0; i < shift + 1; i++ )
				{
					PT_StringInsert(str, tab, (*str)->length);
				}
				PT_StringInsert(str, "},\n", (*str)->length);
			}
		}
		else {
			/*
				{\n
			*/
			PT_StringInsert(str, "{\n", (*str)->length);
			
			for ( unsigned int i = 0; i < jsonValue->u.object.length; i++ )
			{	
				if ( !PT_ParseWriteJsonValue(str, jsonValue->u.object.values[i].value, 
					jsonValue->u.object.values[i].name, shift + 1))
				{
					return SDL_FALSE;
				}
			}
			
			/*
				}
			*/
			PT_StringInsert(str, "}", (*str)->length);
		}
	}
	else if ( jsonValue->type == json_array )
	{
		if ( elementName )
		{
			/*
				\t"elementName": [\n
			*/
			for ( int i = 0; i < shift; i++ )
			{
				PT_StringInsert(str, tab, (*str)->length);
			}
			PT_StringInsert(str, "\"", (*str)->length);
			PT_StringInsert(str, elementName, (*str)->length);
			PT_StringInsert(str, "\"", (*str)->length);
			PT_StringInsert(str, ": [\n", (*str)->length);
			
	
			for ( unsigned int i = 0; i < jsonValue->u.array.length; i++ )
			{	
				if ( jsonValue->u.array.values[i]->type == json_object || 
					jsonValue->u.array.values[i]->type == json_array )
				{
					if ( !PT_ParseWriteJsonValue(str, jsonValue->u.array.values[i], 
						NULL, shift))
					{
						return SDL_FALSE;
					}
				}
				else {
					if ( !PT_ParseWriteJsonValue(str, jsonValue->u.array.values[i], 
						NULL, shift + 1))
					{
						return SDL_FALSE;
					}
				}
			}
			
			/*
				\t],\n
			*/
			for ( int i = 0; i < shift; i++ )
			{
				PT_StringInsert(str, tab, (*str)->length);
			}
			PT_StringInsert(str, "],\n", (*str)->length);
		}
		else {
		
			/*
				\t+1[\n
			*/
			for ( int i = 0; i < shift + 1; i++ )
			{
				PT_StringInsert(str, tab, (*str)->length);
			}
			PT_StringInsert(str, "[\n", (*str)->length);
			
			for ( unsigned int i = 0; i < jsonValue->u.array.length; i++ )
			{	
				if ( jsonValue->u.array.values[i]->type == json_object || 
					jsonValue->u.array.values[i]->type == json_array )
				{
					if ( !PT_ParseWriteJsonValue(str, jsonValue->u.array.values[i], 
						NULL, shift))
					{
						return SDL_FALSE;
					}
				}
				else {
					if ( !PT_ParseWriteJsonValue(str, jsonValue->u.array.values[i], 
						NULL, shift + 1))
					{
						return SDL_FALSE;
					}
				}
			}
			
			/*
				\t+1],\n
			*/
			for ( int i = 0; i < shift + 1; i++ )
			{
				PT_StringInsert(str, tab, (*str)->length);
			}
			PT_StringInsert(str, "],\n", (*str)->length);
		}
	}
	else 
	{	
		if ( elementName )
		{
			/*
				\t"elementName":<space>
			*/
			for ( int i = 0; i < shift; i++ )
			{
				PT_StringInsert(str, tab, (*str)->length);
			}
			PT_StringInsert(str, "\"", (*str)->length);
			PT_StringInsert(str, elementName, (*str)->length);
			PT_StringInsert(str, "\"", (*str)->length);
			PT_StringInsert(str, ": ", (*str)->length);
		}
		else {
			/*
				\t
			*/
			for ( int i = 0; i < shift; i++ )
			{
				PT_StringInsert(str, tab, (*str)->length);
			}
		}
		
		
		
		if ( jsonValue->type == json_integer )
		{	
			char number[32];
			sprintf(number, "%ld", jsonValue->u.integer);
			PT_StringInsert(str, number, (*str)->length);
			PT_StringInsert(str, ",\n", (*str)->length);
		}
		else if ( jsonValue->type == json_double )
		{
			char number[32];
			sprintf(number, "%f", jsonValue->u.dbl);
			char* comma = strchr(number, ',');
			if ( comma )
			{
				comma[0] = '.';
			}
			PT_StringInsert(str, number, (*str)->length);
			PT_StringInsert(str, ",\n", (*str)->length);
		}
		else if ( jsonValue->type == json_string )
		{
			PT_StringInsert(str, "\"", (*str)->length);
			PT_StringInsert(str, jsonValue->u.string.ptr, (*str)->length);
			PT_StringInsert(str, "\"", (*str)->length);
			PT_StringInsert(str, ",\n", (*str)->length);
		}
		else if ( jsonValue->type == json_boolean )
		{
			if ( jsonValue->u.boolean )
			{
				PT_StringInsert(str, "true", (*str)->length);
			}
			else {
				PT_StringInsert(str, "false", (*str)->length);
			}

			PT_StringInsert(str, ",\n", (*str)->length);
		}
		else if ( jsonValue->type == json_null )
		{
			PT_StringInsert(str, "null", (*str)->length);
			PT_StringInsert(str, ",\n", (*str)->length);
		}
	}
	
	return SDL_TRUE;
}//PT_ParseWriteJsonValue





