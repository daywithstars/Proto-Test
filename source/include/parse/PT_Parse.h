/*
Copyright 2022 daywithstars

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

/** 
* \file 
*
* The Proto Test include header for operating json.h.
*/

#ifndef _PT_PARSE_H_
#define _PT_PARSE_H_

#include <SDL_stdinc.h>

#include <json.h>

#include <PT_String.h>


typedef struct pt_parse PT_Parse;

typedef enum {	
	PT_PARSE_CAT_TYPE_START,
	PT_PARSE_CAT_TYPE_MIDDLE,
	PT_PARSE_CAT_TYPE_END,
}PT_ParseCatType;



/**
* \brief This function does the initialization and setup to use PT_Parse.
*
*	This function is required to properly use the PT_Parse.
*
* \returns PT_String pointer in successful or NULL with error.  
*
* \sa PT_ParseDestroy
*/ 
PT_Parse* PT_ParseCreate( );

/**
* \brief frees the memory used by PT_Parse and they properties. 
*
* @param _this The PT_Parse pointer.
*
* \sa PT_ParseCreate
*/
void PT_ParseDestroy( PT_Parse* _this );  

SDL_bool PT_ParseLegalDirectory( const char* path, SDL_bool defaultPath );

/**
* \brief Use this function to open a json file starting at gRootDir
*
* @param _this Is the pointer to previous created PT_Parse with PT_ParseCreate function.
* @param utf8_filePath Is the path to the file to be loaded, starts at the gRootDir.
*
*	Do not use full path, use relative path based on the gRootDir position.
*
* @param defaultPath SDL_TRUE to use the gobal.c path, or SDL_FALSE to not use it. 
*
* \returns SDL_TRUE on success, or SDL_FALSE on error.
*
* \sa PT_ParseLoadTemplate
* \sa PT_ParseSaveFile
*/
SDL_bool PT_ParseOpenFile( PT_Parse* _this, const char* utf8_filePath, SDL_bool defaultPath );

/**
* \brief This function loads an json string into the json_value that is inside PT_Parse.
*
* @param _this Is the pointer to previous created PT_Parse with PT_ParseCreate function.
* @param jsonString The json string that will be used to parse. 
*
*	This clear the previous jsonString and set this as a new jsonString if it succeeded.
*
* \returns SDL_TRUE on success or SDL_FALSE on error.
*
* \sa PT_ParseOpenFile
* \sa PT_ParseSaveFile
*/
SDL_bool PT_ParseLoadTemplate( PT_Parse* _this, const json_char* jsonString );

//Alloc a new string based on @string
void PT_ParseSetOriginalString( PT_Parse* _this, PT_String* string );

SDL_bool PT_ParseChangeValue_Integer( json_value* jsonValue, json_int_t value );
SDL_bool PT_ParseChangeValue_String( json_value* jsonValue, json_char* value );
SDL_bool PT_ParseChangeValue_Double( json_value* jsonValue, double value );

/**
* \brief Use this function to save the original opened json file, starting at gRootDir.
*
* @param _this Is the pointer to previous created PT_Parse with PT_ParseCreate function.
* @param utf8_filePath Is the path to the file to be saved, starts at the gRootDir.
*
*	Do not use full path, use relative path based on the gRootDir position.
*
* @param defaultPath SDL_TRUE to use the gobal.c path, or SDL_FALSE to not use it.
*
* \returns SDL_TRUE on success, or SDL_FALSE on error.
*
* \sa PT_ParseOpenFile
* \sa PT_ParseLoadTemplate
*/
SDL_bool PT_ParseSaveOriginal( PT_Parse* _this, const char* utf8_filePath, SDL_bool defaultPath );

//The same as above, but it saves the tree of jsonValue pointer. 
SDL_bool PT_ParseSaveJsonValue( json_value* jsonValue, const char* utf8_filePath, SDL_bool defaultPath );

/**
* \brief This function will concatenate all the tree from json_value from entry value, to the @string
*
* @type: PT_PARSE_CAT_TYPE_START to add the basic JSON notation to the start of @string
* @type: PT_PARSE_CAT_TYPE_MIDDLE to only add the entry to the @string
* @type: PT_PARSE_CAT_TYPE_END to add the basic JSON notation to the end of @string
*
* example:
*
* PT_ParseCatObjectEntryToString(entry, &str, PT_PARSE_CAT_TYPE_START, SDL_FALSE);
* PT_ParseCatObjectEntryToString(entry, &str, PT_PARSE_CAT_TYPE_MIDDLE, SDL_FALSE);
* PT_ParseCatObjectEntryToString(entry, &str, PT_PARSE_CAT_TYPE_MIDDLE, SDL_TRUE);
* PT_ParseCatObjectEntryToString(entry, &str, PT_PARSE_CAT_TYPE_END, SDL_FALSE);
*
* In the above example, entry can be modified to update its values, or it can be another entry.  
*
* @param entry the JSON object tree to be inserted into @string
* @param string the pointer to pointer of PT_String, must be a valid created string with PT_StringCreate.
* @param type one of the PT_ParseCatType.
* @param resetShift when you want to reset the shifting from the last call to this function. 
*
* @return SDL_TRUE on success, or SDL_FALSE with error. 
*/
SDL_bool PT_ParseCatObjectEntryToString( json_object_entry entry, PT_String** string, 
	PT_ParseCatType type, int shiftStartLevel );

/**
* \brief Use this function to load your own json_value.
*
* You're response to free the json_value memory.
*
* @param utf8_filePath Is the path to the file to be loaded, starts at the RootDir.
* @param defaultPath SDL_TRUE to use the global.c path, or SDL_FALSE to not use it.
*
* \returns An new json_value pointer on success, or NULL if not. 
*/
json_value* PT_ParseGetJsonValueFromFile( const char* utf8_filePath, SDL_bool defaultPath );

/**
* \brief Use this function to get the first read-only internal json_value.
*
* Do not free the pointer, it will be free by PT_ParseDestroy. 
*
* @param _this PT_Parse pointer.
*
* \returns The ponter to the internal loaded json_value.
*
* \sa PT_ParseLoadTemplate
* \sa PT_ParseOpenFile
*/
json_value* PT_ParseGetJsonValuePointer( PT_Parse* _this );

/**
* \brief This function returns the json_object_entry. 
*
*With the json_object_entry you can be use to get 
*name and values from json files. You do not need free manually any kind of memory used by
*json_object_entry, just call PT_ParseDestroy to free the PT_Parse pointer.
*
* @param _this PT_Parse pointer.
* @param nameSequence The sequence of property names for example, you have this json file:
*	
*{ "name1": "value", "name2": { "other": "value" } }
*and you want to reach the "other" and get the json_object_entry, set @nameSequence to:
*"name2 other" this means name2(from first object) ->(to) other(that is the "other" depth/shifted object)
*use space char between each property name you want to search for.  
*
* \returns The json_object_entry structure, that have some attributes like:
* 
*json_oject_entry.value that can be used into the next function: PT_ParseGetObjectEntry_json_value.
*Use entry.value->parent to scroll between an object properties like:
*unsigned int i = entry.value->parent->u.object.length and use the property values from object. 
*The reason for that is because with parent property you have access to the previous/top parent.
*
* \sa PT_ParseLoadTemplate
* \sa PT_ParseOpenFile
*/
json_object_entry PT_ParseGetObjectEntry( PT_Parse* _this, const char* nameSequence );

/**
* \brief When you have an json_value you can use this function to advance into the sequence. 
*
*This function is more useful when you have array of objects or 
*array with normal data types like: integer, strings and objects together inside the array.
*Let's suppose you get the json_object_entry from: { "array": [ 100, "obj": { "name": "value" }] }
*search for: PT_ParseGetObjectEntry( parse, "array" ); now you have an json_object_entry with
*the entry.value property that is an json_value, and you can use this json_value to continue search on.
*This is because now you know where to skip the 100 integer value knowing 
*the json_value structures without knowing the origin from that json_value. 
*
* @param nameSequence The same as the previous PT_ParseGetObjectEntry function.	
*
* \returns The same as the previous PT_ParseGetObjectEntry function.	
*/
json_object_entry PT_ParseGetObjectEntry_json_value( json_value* _json_value, const char* nameSequence ); 

#endif /* _PT_PARSE_H_ */



