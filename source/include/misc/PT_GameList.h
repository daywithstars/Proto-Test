/*
Copyright 2022 daywithstars

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

#ifndef _PT_GAMELIST_H_
#define _PT_GAMELIST_H_

#include <PT_String.h>
#include <PT_Game.h>


typedef struct pt_game_list {
	PT_String* index;
	PT_Game value;
	
	struct pt_game_list* next;
}PT_GameList;


void PT_GameListDestroy( PT_GameList* _this );

PT_GameList* PT_GameListAdd( PT_GameList* _this, const char* utf8_index, PT_Game value );

PT_GameList* PT_GameListGet( PT_GameList* _this, const char* utf8_index );

#endif /* _PT_GAMELIST_H_ */



