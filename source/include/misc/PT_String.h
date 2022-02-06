/*
Copyright 2022 daywithstars

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

/** 
* \file 
*
* The Proto Test include header for Strings.
*/

#ifndef _PT_STRING_H_
#define _PT_STRING_H_

#include <SDL_stdinc.h>


typedef struct {
	Uint8* utf8_string;
	Uint64 length;//Not include the termination NULL character and Unicode(see PT_StringCount)
	Uint64 _size;//The number of bytes, including the NULL character
}PT_String;



/**
* \brief This function does the initialization and setup to use PT_String.
*	
* This function is required to properly use the PT_String. 
* 
* \returns PT_String pointer in successful or you will get a segment fault.   
*
* \sa PT_StringDestroy
*/
PT_String* PT_StringCreate( );

/** 
* \brief This function frees the memory used by the PT_String object.
*
*	In order to use the PT_String object again, you need to call PT_StringCreate.
*
* \sa PT_StringCreate
*/
void PT_StringDestroy( PT_String* _this );


/**
* \brief This function add UTF-8 text into any given position on your current string.
*	
* @param _this Is the pointer to previous created PT_String with PT_StringCreate function.
* @param utf8_string Is the UTF-8 string that will be added. 
* @param insertPosition Is the position where your utf8_string will be added.
*
*	case 1 If your PT_String object is empty, any given value is ignored and the utf8_string is added.
*
*	case 2 If the value is equal to your PT_String->length it will be added to the end
*	to insert into the end you do not need manual count, just put your string length property.
*
*	case 3 If the value is 0 it will added to the start of the current string.
*
*	case 4 If the value is great than your current string it will do nothing. 
*
*	case 5 If the value is between 1 and PT_StringCount - 1, it will add after the
*	position you set for example: you insert 'z' into position 1 
*	into this string: "ab議ね" the new string is: "azb議ね"
*	and if you insert intro position 3, your new string is: "ab議zね". 
*	To insert into the beginning go to case 3, and
*	to insert into the end go to case 2. Remember special char like '\n' counts even it is not visible.
*
*	Warning Do not use PT_String->utf8_string[n] thinking 
*	you're getting the correct character, instead use PT_StringCount. 
*
* \returns SDL_TRUE with no warning messages, SDL_FALSE with warning message. 
*
* \sa PT_StringRemove
* \sa PT_StringClear
* \sa PT_StringCount
* \sa PT_StringGetOccurrencePosition
* \sa PT_StringCopyFrom
* \sa PT_StringMatch
* \sa PT_StringPrintLine
*/
SDL_bool PT_StringInsert( PT_String** _this, const char* utf8_string, Uint64 insertPosition );

/**
* \brief Remove any char hold by _this from utf8_string. 
* 
* @param _this The pointer to the PT_String struct. 
*
* \sa PT_StringInsert
* \sa PT_StringClear
* \sa PT_StringCount
* \sa PT_StringGetOccurrencePosition
* \sa PT_StringCopyFrom
* \sa PT_StringMatch
* \sa PT_StringPrintLine
*/
SDL_bool PT_StringRemove( PT_String* _this, const char* utf8_string );

/**
* \brief Clear all the chars from the PT_String.
*
* Do not insert the same char twice in utf8_string. If you want remove all 'a' just put "a",
* if you want remove all 'a', 'z' put: "az". 
*
* @param _this The pointer to pointer an PT_String struct.
*
* \sa PT_StringInsert
* \sa PT_StringRemove
* \sa PT_StringCount
* \sa PT_StringGetOccurrencePosition
* \sa PT_StringCopyFrom
* \sa PT_StringMatch
* \sa PT_StringPrintLine
*/
void PT_StringClear( PT_String** _this );

/**
* \brief This function return the number of character unicode.
* 
* 	For example if you have "ab議ね" this function will returns 4.
* 	Remember special char like '\n' counts even it is not visible.
*
* @param _this The PT_String pointer.
*
* \returns The number of visible character like: "議ね" = 2.
*
*	This function don't count the termination Null char.
*
* \sa PT_StringInsert]
* \sa PT_StringRemove
* \sa PT_StringClear
* \sa PT_StringGetOccurrencePosition
* \sa PT_StringCopyFrom
* \sa PT_StringMatch
* \sa PT_StringPrintLine
*/ 
Uint64 PT_StringCount( const PT_String* _this );

/**
* \brief The same as PT_StringCount, but with basic string array of char's.
*/
Uint64 PT_StringCountBasicString( const char* utf8_string );

/**
* \brief Use this function to know the last position char where the occurrence happens. 
*
*	This function also can be used to get the specific position to be used into PT_StringInsert,
*	for example if you want to insert some string after 'a' from "zabc", if you want to insert 
*	after the "za" you can use the return from this function and add + 1 into PT_StringInsert.
*
* @param _this The PT_String pointer.
* @param utf8_string The string pattern to search for. 
* @param numOccurrence 1 means the first occurrence, 2 the second occurrence to n occurrence. A negative number means the last string occurrence.   
*
*	case 1 If this value is 0 the function will return 0.
*
*	case 2 If the value is above the number of occurrence, this function will return 0.  
*
* \returns The end UTF-8 char position wheres the utf8_strings occurs on _this or 0 in case 1 and when not found
*	the next occurrence. 
*
* \sa PT_StringInsert
* \sa PT_StringRemove
* \sa PT_StringClear
* \sa PT_StringCount
* \sa PT_StringCopyFrom
* \sa PT_StringMatch
* \sa PT_StringPrintLine
*/
Uint64 PT_StringGetOccurrencePosition( const PT_String* _this, const char* utf8_string, 
	Sint32 numOccurrence );

/**
* \brief The same as PT_StringGetOccurrencePosition, but with basic string type. 
*/
Uint64 PT_StringGetOccurrencePositionBasicString ( const char* utf8_string1, const char* utf8_string2, 
	Sint32 numOccurrence );

/**
* \brief Copy utf8_string to PT_String from a certain range. 
*
* For example let's copy only the file name "sound-space.wav" without the extersion: 
*
* (fileName, "sound-space.wav", 0, 
*	PT_StringGetOccurrencePositionBasicString("sound-space.wav", ".", 1) - 1, 0);
*
* @param _this The PT_String pointer that will receive the string from utf8_string.
* @param utf8_string The string to be copied into _this.
* @param sourceStart The start unicode char position from utf8_string.
* @param sourceEnd The end unicode char position or the last to be copied from utf8_string.
* @param tarStart The unicode char position that will be placed the copy of utf8_string. 
*
* \returns SDL_FALSE on error, or SDL_TRUE on success.
*
* \sa PT_StringInsert
* \sa PT_StringRemove
* \sa PT_StringClear
* \sa PT_StringCount
* \sa PT_StringMatch
* \sa PT_StringPrintLine
*/
SDL_bool PT_StringCopyFrom( PT_String* _this, const char* utf8_string, Uint64 sourceStart, 
	Uint64 sourceEnd, Uint64 tarStart );

/** 
* \brief Use this function to compare if both strings are equal.
*
* @param _this The PT_String pointer.
* @param utf8_string Some basic string to be create as PT_String and to be compared with _this.
*
* \returns SDL_TRUE if are equal, SDL_FALSE are not equal or NULL.
*
* \sa PT_StringInsert
* \sa PT_StringRemove
* \sa PT_StringClear
* \sa PT_StringCount
* \sa PT_StringGetOccurrencePosition
* \sa PT_StringCopyFrom
* \sa PT_StringPrintLine
*/
SDL_bool PT_StringMatch( const PT_String* _this, const char* utf8_string );

//simple c string comparison 
SDL_bool PT_StringMatchFast( const char* str1, const char* str2 );

/**
* \brief Print the current UTF-8 string into the standard output.
*
*	If your system and your current standard output do not support Unicode character 
*	they will not be displayed.
*
* @param _this The PT_String pointer, or NULL to print an blank line. 
*
* \sa PT_StringInsert
* \sa PT_StringRemove
* \sa PT_StringClear
* \sa PT_StringCount
* \sa PT_StringGetOccurrencePosition
* \sa PT_StringCopyFrom
* \sa PT_StringMatch
*/
void PT_StringPrintLine( const PT_String* _this );

#endif /* _PT_STRING_H_ */








