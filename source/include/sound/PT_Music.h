/*
Copyright 2022 daywithstars

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

#ifndef _PT_MUSIC_H_
#define _PT_MUSIC_H_

#include <SDL_mixer.h>

//There's definitions are exclusively used into the PT_SoundManager


typedef struct {
	Mix_Music* music;
}PT_Music;



PT_Music* PT_MusicCreate( );

void PT_MusicDestroy( PT_Music* _this);

//utf8_fileName not path, like: bg.wav or bg.mp3 or bg.ogg
SDL_bool PT_MusicLoad( PT_Music* _this, const char* utf8_fileName );

void PT_MusicPlay( PT_Music* _this, int loop );

#endif /* _PT_MUSIC_H_ */



