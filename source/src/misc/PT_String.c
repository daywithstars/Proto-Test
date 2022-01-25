/*
Copyright 2021 daywithstars

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

#include <SDL_log.h>

#include <PT_String.h>


#define PT_MOST_SIGNIFICANT_BIT 128

/*
Get the number of possible Bytes from the first head unicode UTF-8 encoded char by 'c'
* Return 1 if it is ASCII.
* Return 0 if is some other ASCII or not supported char
* Return 2 to 4 that means the number of Bytes this head UTF-8 have.
*/
Uint8 get_utf8_char_length( Uint8 c ) {
	const Uint8 tmpC = c;
	Uint8 head = PT_MOST_SIGNIFICANT_BIT + 64;
	
	if ( c <= 0x7F )
	{
		return 1;
	}
	
	c >>= 5;
	head >>= 5;
	if ( c == head )
	{
		return 2;
	}
	
	c = tmpC;
	head = PT_MOST_SIGNIFICANT_BIT + 64 + 32;
	
	c >>= 4;
	head >>= 4;
	if ( c == head )
	{
		return 3;
	}
	
	c = tmpC;
	head = PT_MOST_SIGNIFICANT_BIT + 64 + 32 + 16;
	
	c >>= 3;
	head >>= 3;
	if ( c == head )
	{
		return 4;
	}
	
	return 0;
}// get_utf8_char_length

/*
First you need to make sure you're dealing with UTF-8 char, used the 
above function @get_utf8_char_length and give the second Byte from the string to this function.

This function checks if the second head of possible UTF-8 char is present 
if it is not this means the previous function only detects the 192/b11000000 ASCII char.

Return 1 if the head is detected or 0 if not.
*/
Uint8 is_next_head_of_utf8_encode( Uint8 c ) {
	Uint8 head = PT_MOST_SIGNIFICANT_BIT;
	
	head >>= 6;
	c >>= 6;
	
	if ( head == c )
	{
		return 1;
	}
	
	return 0;
}//is_next_head_of_utf8_encode

//if a sequence of integer[n1 ... n2] is between n1 = [192 to 247] and n2 = [128 to 191]
//will not give warning. for example: char str[] = { 'a', (char)192, (char)128, '\0' };
//later function will be put into the string, if a warning is detected it will put U+FFFD into the string.
Uint8 is_char_supported( Uint8* c ) {
	static Uint8 _BytesCount = 0;
	
	if ( get_utf8_char_length(*c) )
	{
		_BytesCount = get_utf8_char_length(*c);
		
		if ( _BytesCount > 1 )
		{
			if ( is_next_head_of_utf8_encode( *(c + 1)) )
			{
				return 1;
			}
			else {
				return 0;
			}
		}
		
		return 1;
	}
	if ( is_next_head_of_utf8_encode(*c) && _BytesCount > 1 )
	{
		_BytesCount --;
		return 1;
	}
	
	return 0;
}//is_char_supported

Uint64 stringLength( const Uint8* str ) {
	

	Uint64 length = 0;
	Uint64 i = 1;
	while ( str[i - 1] != '\0' )
	{
		i++;
		length++;		
	}
	
	return length;
}//stringLength

void resizeString( PT_String* _this, Uint64 length ) {
	if ( _this->length == 0 )
	{
		_this->length = length;
		_this->_size = length + 1;
		_this->utf8_string = (Uint8*)SDL_malloc(_this->_size);
		_this->utf8_string[length] = '\0';
	}
	else if ( length != _this->length ) {
		
		Uint64 newSize = length + 1;
		Uint64 newLength = 0;
		Uint8* newString = (Uint8*)SDL_malloc(newSize);
		
		for ( Uint64 i = 0, j = 0; ; i++, j++ )
		{
			newString[i] = _this->utf8_string[j];
			
			if ( j >= _this->length - 1 )
			{
				newString[i + 1] = '\0';
				newLength = i + 1;
				break;
			}
			else if ( i >= length - 1 )
			{
				newString[length] = '\0';
				newLength = length;
				break;
			}
		}
		
		_this->length = newLength;
		_this->_size = newSize;
		free(_this->utf8_string);
		_this->utf8_string = newString;
	}
}//resizeString

//Return SDL_FALSE if some string is null or not equal
SDL_bool compareString( const char* str1, const char* str2 ) {
	
	if ( str1 == NULL || str2 == NULL )
	{
		return 0;
	}

	while ( *str1 == *str2 && (*str1) != '\0' && (*str2) != '\0' ) 
	{
		
		str1 ++;
		str2 ++;
	}
	
	return ( *str1 == *str2 ? SDL_TRUE : SDL_FALSE );
}//compareString

Uint8 compareString_stopWhenSeeNullChar( Uint8* str1, Uint8* str2 ) {
	
	if ( str1 == NULL || str2 == NULL )
	{
		return 0;
	}
	
	Uint8 c1 = *str1, c2 = *str2;
	while ( (*str1) != '\0' && (*str2) != '\0' ) 
	{
		c1 = *str1;
		c2 = *str2;
		
		str1 ++;
		str2 ++;
	}
	
	return ( c1 == c2 ? 1 : 0 );
}//compareString_stopWhenSeeNullChar


//===================================== PUBLIC Functions

PT_String* PT_StringCreate( ) {
	PT_String* _this = (PT_String*)SDL_malloc(sizeof(PT_String));
	if ( !_this )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_StringCreate: Not enough memory\n");
		return NULL;
	}
	_this->utf8_string = NULL;
	_this->length = 0;
	_this->_size = 0;

	return _this;
}//PT_StringCreate

void PT_StringDestroy( PT_String* _this ) {
	if ( !_this )
	{
		return;
	}


	if ( _this->utf8_string )
	{
		free(_this->utf8_string);
		_this->length = 0;
		_this->_size = 0;
	}	
	free(_this);
	
}//PT_StringDestroy

SDL_bool PT_StringInsert( PT_String** _this, const char* _utf8_string, Uint64 insertPosition ) {
	
	if ( !_this )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_StringInsertString: Invalid PT_String\n");
		return SDL_FALSE;
	}
	if ( !_utf8_string )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_StringInsertString: Invalid _utf8_string\n");
		return SDL_FALSE;
	}

	Uint8* utf8_string = (Uint8*)_utf8_string;
	
	if ( (*_this)->length == 0 )
	{
		resizeString((*_this), stringLength(utf8_string));
		
		for ( Uint64 i = 0; i < (*_this)->length; i++ )
		{
			if ( is_char_supported(&utf8_string[i]) )
			{
				(*_this)->utf8_string[i] = utf8_string[i];
			}
			else {
				SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_StringInsertString case 1: this char: %d is not supported!\n", (int)utf8_string[i]);
					
				SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "* please format in UTF-8, the wrong char will be replaced by U+FFFD unicode code point into your string \n");
				
				
					
				PT_String* wrongStr = PT_StringCreate();
				resizeString(wrongStr, i + 3);
				
				Uint64 j = 0;
				while ( j < i )
				{
					wrongStr->utf8_string[j] = (*_this)->utf8_string[j];
					j++;
				}
				wrongStr->utf8_string[j] = 0xEF;
				wrongStr->utf8_string[j + 1] = 0xBF;
				wrongStr->utf8_string[j + 2] = 0xBD;
				wrongStr->utf8_string[j + 3] = '\0';
				
				PT_StringDestroy(*_this);
				(*_this) = wrongStr;
				return SDL_FALSE;
			}
		}
	}
	else {	
		
		if ( insertPosition == (*_this)->length )
		{
			const Uint64 newStringLength = stringLength(utf8_string);
			const Uint64 prevStringLength = (*_this)->length;
			const Uint64 newLength = newStringLength + (*_this)->length;
			
			PT_String* newString = PT_StringCreate();
			resizeString(newString, newLength);
			
			Uint64 i = 0;
			while ( i < prevStringLength )
			{
				newString->utf8_string[i] = (*_this)->utf8_string[i];
				i ++;
			} 
			Uint64 j = 0;
			while ( i < newLength )
			{
				if ( is_char_supported(&utf8_string[j]) )
				{
					newString->utf8_string[i] = utf8_string[j];
				}
				else {
					SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_StringInsertString case 2: this char: %d is not supported!\n", (int)utf8_string[j]);
						
					SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "* please format in UTF-8, the wrong char will be replaced by U+FFFD unicode code point into your string \n");
						
						
					PT_String* wrongStr = PT_StringCreate();
					resizeString(wrongStr, i + 3);
				
					Uint64 j = 0;
					while ( j < i )
					{
						wrongStr->utf8_string[j] = newString->utf8_string[j];
						j++;
					}
					wrongStr->utf8_string[j] = 0xEF;
					wrongStr->utf8_string[j + 1] = 0xBF;
					wrongStr->utf8_string[j + 2] = 0xBD;
					wrongStr->utf8_string[j + 3] = '\0';
					
					PT_StringDestroy(*_this);
					PT_StringDestroy(newString);
					(*_this) = wrongStr;
					
					return SDL_FALSE;
				}
				i++;
				j++;
			}
			newString->utf8_string[newLength] = '\0';
			
			PT_StringDestroy(*_this);
			(*_this) = newString;
		}
		else if ( insertPosition == 0 ) 
		{
			const Uint64 newStringLength = stringLength(utf8_string);
			const Uint64 prevStringLength = (*_this)->length;
			const Uint64 newLength = newStringLength + (*_this)->length;
			
			PT_String* newString = PT_StringCreate();
			resizeString(newString, newLength);
			
			Uint64 i = newStringLength;
			Uint64 j = 0;
			while ( i < newLength )
			{
				if ( j < prevStringLength )
				{
					newString->utf8_string[i] = (*_this)->utf8_string[j];
				}
				i ++;
				j ++;
			}
			i = 0;
			while ( i < newStringLength )
			{
				if (is_char_supported(&utf8_string[i]))
				{
					newString->utf8_string[i] = utf8_string[i];
				}
				else {
					SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_StringInsertString case 3: this char: %d is not supported!\n", (int)utf8_string[i]);
						
					SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "* please format in UTF-8, the wrong char will be replaced by U+FFFD unicode code point into your string \n");
					
						
					PT_String* wrongStr = PT_StringCreate();
					resizeString(wrongStr, j + i + 3);
				
					Uint64 k = 0;
					while ( k < i )
					{
						wrongStr->utf8_string[k] = newString->utf8_string[k];
						k ++;
					}
					wrongStr->utf8_string[k] = 0xEF;
					wrongStr->utf8_string[k + 1] = 0xBF;
					wrongStr->utf8_string[k + 2] = 0xBD;
					
					k += 3;
					Uint64 v = 0;
					while ( k < j + i + 3 )
					{
						wrongStr->utf8_string[k] = (*_this)->utf8_string[v];
						k++;
						v++;
					}
					wrongStr->utf8_string[k] = '\0';
					
					PT_StringDestroy(*_this);
					PT_StringDestroy(newString);
					(*_this) = wrongStr;
					
					return SDL_FALSE;
				}
				
				i++;
			}
			newString->utf8_string[newLength] = '\0';
			PT_StringDestroy(*_this);
			(*_this) = newString;
		}
		else 
		{	
			const Uint64 newStringLength = stringLength(utf8_string);
			const Uint64 prevStringLength = (*_this)->length;
			const Uint64 newLength = newStringLength + (*_this)->length;
		
			PT_String* newString = PT_StringCreate();
			resizeString(newString, newLength);
		
			Uint64 strCount = 0;
			Uint64 i = 0;
			Uint64 Bytes = 0;
			
			while ( i < prevStringLength )
			{
				if ( get_utf8_char_length((*_this)->utf8_string[i]) )
				{
					Bytes += get_utf8_char_length((*_this)->utf8_string[i]);
					
					strCount += 1;
					if ( strCount == insertPosition )
					{
						break;
					}
				}
				
				i++;
			}	
			
			Uint64 j = 0;
			while ( j < Bytes )
			{
				newString->utf8_string[j] = (*_this)->utf8_string[j];
				j++;
			}
			
			i = 0;
			while ( i < newStringLength )
			{
				if ( is_char_supported(&utf8_string[i]) )
				{
					newString->utf8_string[j] = utf8_string[i];
				}
				else {
					SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_StringInsertString case 5: this char: %d is not supported!\n", (int)utf8_string[i]);
						
					SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "* please format in UTF-8, the wrong char will be replaced by U+FFFD unicode code point into your string \n");
					
					
					PT_String* wrongStr = PT_StringCreate();
					resizeString(wrongStr, prevStringLength + i + 3);
					
					
					Uint64 k = 0;
					while ( k < Bytes )
					{
						wrongStr->utf8_string[k] = newString->utf8_string[k];
						k ++;
					}
					Uint64 v = 0;
					while ( v < i )
					{
						wrongStr->utf8_string[k] = utf8_string[v];
						k ++;
						v ++;
					}
					
					wrongStr->utf8_string[k] = 0xEF;
					wrongStr->utf8_string[k + 1] = 0xBF;
					wrongStr->utf8_string[k + 2] = 0xBD;
					
					k += 3;
					
					i = Bytes;
					while ( i < prevStringLength )
					{
						wrongStr->utf8_string[k] = (*_this)->utf8_string[i];
						
						k ++;
						i ++;
					}
					
					wrongStr->utf8_string[k] = '\0';
					
					PT_StringDestroy(newString);
					PT_StringDestroy(*_this);
					
					(*_this) = wrongStr;
					return SDL_FALSE;
				}
				
				j++;
				i++;
			}
			
			i = Bytes;
			while ( j < newLength ) 
			{
				newString->utf8_string[j] = (*_this)->utf8_string[i];
				j++;
				i++;
			}

			newString->utf8_string[newLength] = '\0';
			
			PT_StringDestroy(*_this);
			(*_this) = newString;
		}
	}
	
	return SDL_TRUE;
}//PT_StringInsert

SDL_bool PT_StringRemove( PT_String* _this, const char* utf8_string ) {
	
	PT_String* ignore = PT_StringCreate();
	if ( !ignore )
	{	
		return SDL_FALSE;
	}
	if ( !PT_StringInsert(&ignore, utf8_string, 0) )
	{
		PT_StringDestroy(ignore);
		return SDL_FALSE;
	}
	
	
	Uint64 numIgnore = 0;
	for ( Uint64 i = 0; i < ignore->length; i++ )
	{
		for ( Uint64 j = 0; j < _this->length; j++ )
		{
			if ( get_utf8_char_length(ignore->utf8_string[i]) != 
				get_utf8_char_length(_this->utf8_string[j]) )
			{
				j += get_utf8_char_length(_this->utf8_string[j]);
				i += get_utf8_char_length(ignore->utf8_string[i]);
				
				continue;
			}
			
			if ( ignore->utf8_string[i] == _this->utf8_string[j] )
			{
				if ( numIgnore + 1 < _this->length )
				{
					numIgnore ++;
				}
				else {
					PT_StringDestroy(ignore);
					return SDL_FALSE;
				}
			}
		}
	}
	
	
	Uint64 newStringSize = (_this->length - numIgnore) + 1;
	Uint8* newString = (Uint8*)malloc(sizeof(Uint8) * newStringSize);
	if ( !newString )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_StringRemove: Not enough memory\n");
		PT_StringDestroy(ignore);
		return SDL_FALSE;
	}
	Uint64 k = 0;
	for ( Uint64 i = 0; i < _this->length; i++ )
	{
		SDL_bool equals = SDL_FALSE;
		for ( Uint64 j = 0; j < ignore->length; j++ )
		{
			if ( get_utf8_char_length(ignore->utf8_string[j]) != 
				get_utf8_char_length(_this->utf8_string[i]) )
			{
				//i += get_utf8_char_length(_this->utf8_string[j]);
				j += get_utf8_char_length(ignore->utf8_string[i]);
				
				continue;
			}
			
			if ( _this->utf8_string[i] == ignore->utf8_string[j] )
			{
				equals = SDL_TRUE;
			}
		}
		
		if ( equals == SDL_FALSE )
		{
			if ( k < newStringSize )
			{
				newString[k] = _this->utf8_string[i];
				k ++;
			}
		}
	}
	newString[newStringSize - 1] = '\0';
	
	free(_this->utf8_string);
	_this->utf8_string = newString;
	_this->length = newStringSize - 1;
	_this->_size = newStringSize;
	

	PT_StringDestroy(ignore);
	return SDL_TRUE;
}//PT_StringRemove

void PT_StringClear( PT_String** _this ) {
	if ( !_this || !(*_this)) 
	{
		return;
	}

	if ( (*_this)->utf8_string )
	{
		free((*_this)->utf8_string);
		(*_this)->length = 0;
		(*_this)->_size = 0;
		(*_this)->utf8_string = NULL;
	}	
}//PT_StringClear

Uint64 PT_StringCount( const PT_String* _this ) {
	Uint64 strCount = 0;
	
	Uint64 i = 0;
	while ( i < _this->length )
	{
		if ( get_utf8_char_length(_this->utf8_string[i]) )
		{
			strCount += 1;
		}
		
		i++;
	}
	
	return strCount;
}//PT_StringCount

Uint64 PT_StringCountBasicString( const char* utf8_string ) {
	PT_String* tmp = PT_StringCreate();
	Uint64 strCount = 0;
	
	if ( !tmp )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_StringCountBasicString!\n");
		return strCount;
	}
	if ( !PT_StringInsert(&tmp, utf8_string, 0) )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_StringCountBasicString!\n");
		return strCount;
	}
	
	
	strCount = PT_StringCount(tmp);
	PT_StringDestroy(tmp);
	return strCount;
}//PT_StringCountBasicString

Uint64 PT_StringGetOccurrencePosition( const PT_String* _this, const char* utf8_string, 
	Sint32 numOccurrence ) {
	
	return PT_StringGetOccurrencePositionBasicString((const char*)_this->utf8_string, utf8_string,
		numOccurrence);
}//PT_StringGetOccurrencePosition

Uint64 PT_StringGetOccurrencePositionBasicString ( const char* utf8_string1, const char* utf8_string2, 
	Sint32 numOccurrence ) {
	
	Uint64 value = 0;

	if ( numOccurrence == 0 )
	{
		return value;
	}
	
	PT_String* str1 = PT_StringCreate();
	if ( !PT_StringInsert(&str1, utf8_string1, 0) )
	{
		PT_StringDestroy(str1);
		return value;
	}
	PT_String* str2 = PT_StringCreate();
	if ( !PT_StringInsert(&str2, utf8_string2, 0) )
	{
		PT_StringDestroy(str2);
		PT_StringDestroy(str1);
		return value;
	}

	
	Uint64 strCount = 0;
	for ( Uint64 i = 0; i < str1->length; i++ )
	{
		if ( str2->utf8_string[0] == str1->utf8_string[i] )
		{
			Uint8* tmp = &str1->utf8_string[i];
			
			if ( compareString_stopWhenSeeNullChar(str2->utf8_string, tmp) )
			{
				if ( numOccurrence > 0 )
				{
					numOccurrence --;
					
					if ( numOccurrence == 0 )
					{
						value = strCount + PT_StringCount(str2);
						PT_StringDestroy(str1);
						PT_StringDestroy(str2);
						return value;
					}
				}
				else {
					value = strCount + PT_StringCount(str2);
				}
			}
		}
		
		if ( get_utf8_char_length(str1->utf8_string[i]) )
		{
			strCount += get_utf8_char_length(str1->utf8_string[i]);
		}
	}

	PT_StringDestroy(str1);
	PT_StringDestroy(str2);
	return value;
}//PT_StringGetOccurrencePositionBasicString

SDL_bool PT_StringCopyFrom( PT_String* _this, const char* utf8_string, Uint64 sourceStart, 
	Uint64 sourceEnd, Uint64 tarStart ) {
	
	if ( !_this || !utf8_string || sourceStart > sourceEnd )
	{
		return SDL_FALSE;
	}
	
	PT_String* sourceStr = PT_StringCreate();
	if ( !PT_StringInsert(&sourceStr, utf8_string, 0) )
	{
		PT_StringDestroy(sourceStr);
		return SDL_FALSE;
	}
	
	Uint64 sourceStrCount = PT_StringCount(sourceStr);
	if ( sourceStart > sourceStrCount )
	{
		PT_StringDestroy(sourceStr);
		return SDL_FALSE;
	}
	if ( tarStart > PT_StringCount(_this) )
	{
		PT_StringDestroy(sourceStr);
		return SDL_FALSE;
	}
	
	
	Uint64 charCount = 0;
	Uint8 startByte = 0;
	Uint64 i = 0;
	while ( i < sourceStr->length )
	{
		if ( charCount == sourceStart )
		{
			break;
		}
		if ( get_utf8_char_length(sourceStr->utf8_string[i]) )
		{
			charCount += 1;
			startByte += get_utf8_char_length(sourceStr->utf8_string[i]);
		}
		
		i ++;
	}
	
	Uint64 endByte = startByte;
	i = startByte;
	while ( i < sourceStr->length )
	{
		if ( charCount == sourceEnd )
		{
			break;
		}
		if ( get_utf8_char_length(sourceStr->utf8_string[i]) )
		{
			charCount += 1;
			endByte += get_utf8_char_length(sourceStr->utf8_string[i]);
		}
		
		i ++;
	}
	
	i = startByte;
	Uint64 j = 0;
	Uint8* newString = (Uint8*)malloc(sizeof(Uint8) * sourceStr->length + 1);
	do
	{
		newString[j] = sourceStr->utf8_string[i];
		i ++;
		j ++;
	}while(i < endByte);

	newString[j] = '\0';
	
	PT_StringInsert(&_this, (char*)newString, tarStart);
	
	PT_StringDestroy(sourceStr);
	free(newString);

	return SDL_TRUE;
}//PT_StringCopyFrom

SDL_bool PT_StringMatch( const PT_String* _this, const char* utf8_string ) {
	PT_String* str2 = PT_StringCreate();
	PT_StringInsert(&str2, utf8_string, 0);

	SDL_bool value = compareString((char*)_this->utf8_string, (char*)str2->utf8_string);
	
	PT_StringDestroy(str2);
	return value;
}//PT_StringMatch

SDL_bool PT_StringMatchFast( const char* str1, const char* str2 ) {
	return compareString(str1, str2);
}//PT_StringMatchFast

void PT_StringPrintLine( const PT_String* _this ) {
	if ( !_this )
	{
		SDL_Log("\n");
		return;
	}
	
	SDL_Log("PT: PT_StringPrint:%s", (char*)_this->utf8_string);
}//PT_StringPrint



