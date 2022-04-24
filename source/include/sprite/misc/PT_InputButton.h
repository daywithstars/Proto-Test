/*
Copyright 2022 daywithstars

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

#ifndef _PT_INPUTBUTTON_H_
#define _PT_INPUTBUTTON_H_

#include <json.h>

#include <PT_Sprite.h>



typedef struct pt_input_button PT_InputButton;


PT_Sprite* PT_InputButtonCreate( const char* utf8_spriteTemplate );
PT_Sprite* PT_InputButtonCreateFromJsonValue( json_value* jsonValue );
PT_Sprite* PT_InputButtonCreateFromStringTemplate( const char* utf8_stringTemplate );

void PT_InputButtonDestroy( void* _data ); 

SDL_bool PT_InputButtonGetEventPress( void* _data );

void PT_InputButtonUpdate( void* _data, Sint32 elapsedTime );
void PT_InputButtonDraw( void* _data );


#endif /* _PT_INPUTBUTTON_H_ */



