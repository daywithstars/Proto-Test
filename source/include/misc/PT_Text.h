/*
Copyright 2022 daywithstars

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

#ifndef _PT_TEXT_H_
#define _PT_TEXT_H_


#include <SDL_stdinc.h>

#include <json.h>

#include <PT_String.h>


typedef struct pt_text {
	PT_String* font;
	PT_String* textureName;
	PT_String* text;
	
	int x, y;
}PT_Text;


PT_Text* PT_TextCreate( json_value* jsonValue );

void PT_TextDestroy( PT_Text* _this );

void PT_TextUpdateTexture( PT_Text* _this );

void PT_TextDraw( PT_Text* _this );

#endif /* _PT_TEXT_H_ */



