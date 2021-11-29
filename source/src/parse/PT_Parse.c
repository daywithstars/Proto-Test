/*
Copyright 2021 daywithstars

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

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



PT_Parse* PT_ParseCreate( ) {

	PT_Parse* _this = (PT_Parse*)malloc(sizeof(struct pt_parse));
	if ( !_this )
	{
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
	
	PT_StringDestroy(_this->jsonString);
	_this->jsonString = PT_StringCreate();
	if ( !PT_StringInsert(&(_this->jsonString), (char*)jsonString, 0) )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_ParseLoadTemplate!\n");
	}
	
	return SDL_TRUE;
}//PT_ParseLoadTemplate

SDL_bool PT_ParseSaveFile( PT_Parse* _this, const char* utf8_filePath, SDL_bool defaultPath ) {
	if ( !_this )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_ParseSaveFile: Invalid PT_Parse\n");
		return SDL_FALSE;
	}
	
	PT_String* strPath = PT_StringCreate();
	if ( !PT_StringInsert(&strPath, utf8_filePath, 0) )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_ParseSaveFile!\n");
	}
	if ( defaultPath )
	{	
		if ( !PT_StringInsert(&strPath, (char*)gRootDir->utf8_string, 0) )
		{
			SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_ParseSaveFile!\n");
		}
	}
	
	SDL_RWops* file = SDL_RWFromFile((char*)strPath->utf8_string, "w");
	if ( !file )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_ParseSaveFile: Cannot create file: %s\n",
		(char*)strPath->utf8_string);
		PT_StringDestroy(strPath);
		return SDL_FALSE;
	}
	
	SDL_RWwrite(file, _this->jsonString->utf8_string, sizeof(Uint8), _this->jsonString->length);
	
	PT_StringDestroy(strPath);
	SDL_RWclose(file);
	return SDL_TRUE;
}//PT_ParseSaveFile

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
	json_char* jsonStr = (json_char*)malloc(jsonStringSize);
	
	if ( !jsonStr )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_ParseGetJsonValueFromFile: Not enough memory\n");
		PT_StringDestroy(strPath);
		SDL_RWclose(file);
		return NULL;
	}
	
	SDL_RWread(file, jsonStr, sizeof(json_char), SDL_RWsize(file));
	jsonStr[jsonStringSize - 1] = '\0';
	
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

const json_value* PT_ParseGetJsonValuePointer( PT_Parse* _this ) {
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

	if ( !_json_value )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_ParseGetObjectEntry_json_value: Invalid _json_value\n");
		return entry;
	}
	
	if ( _json_value->type == json_object )
	{
		char entryName[PT_PARSE_MAX_ENTRY_NAME_SIZE];
		
		for ( Uint64 i = 0, j = 0; ( i < strlen(nameSequence) + 1 ) && _json_value; i++, j++ )
		{
			if ( nameSequence[i] == ' ' || nameSequence[i] == '\0' )
			{
				
				entryName[j] = '\0';
				j = 0;
				i += 1;//jump the ' ' char
				
				if ( _json_value )
				{
					for ( json_int_t k = 0; k < _json_value->u.object.length; k++ )
					{
						if ( !strcmp(entryName, _json_value->u.object.values[k].name) )
						{
							entry = _json_value->u.object.values[k];
							_json_value = _json_value->u.object.values[k].value;
							break;
						}
					}
				}
			}
			entryName[j] = nameSequence[i];
		}
	}
	else {
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_ParseGetObjectEntry_json_value: This is not an json_object: %d\n", _json_value->type);
	}
	
	return entry;
}//PT_ParseGetObjectEntry_json_value



