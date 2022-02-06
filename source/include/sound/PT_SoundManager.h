/*
Copyright 2022 daywithstars

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

/** 
* \file 
*
* The Proto Test include header for handling sounds. 
*/


#ifndef _PT_SOUNDMANAGER_H_
#define _PT_SOUNDMANAGER_H_

#include <SDL_mixer.h>



/** 
* \brief Creates and load configurations from file.
*
* \sa PT_SoundManagerDestroy
*/
void PT_SoundManagerCreate( );

/** void PT_SoundManagerDestroy( )
*
* @Brief: Frees everything and close the PT_SoundManager.
*/
void PT_SoundManagerDestroy( );

/** 
* \brief Loads an music from /sound/music with the name from list.json.
*
* @param utf8_musicName Is the music property name from music-list.json file.
*
*
* \sa PT_SoundManagerPlayMusic
* \sa PT_SoundManagerPauseMusic
* \sa PT_SoundManagerStopMusic
* \sa PT_SoundManagerSetMusicVolume
* \sa PT_SoundManagerDestroyMusic
*/
SDL_bool PT_SoundManagerLoadMusics( ); 

/**
* \brief Loads all the samples from /sound/sample folder.
*
* \returns SDL_TRUE on success or SDL_FALSE with error.
*
*
* \sa PT_SoundManagerPlaySample
* \sa PT_SoundManagerPauseSample
* \sa PT_SoundManagerStopSample
* \sa PT_SoundManagerSetSamplesVolume
*/
SDL_bool PT_SoundManagerLoadSamples( );

/**
* \brief Play the loaded music from PT_SoundManagerLoadMusics.
*
* @param utf8_musicName the music name from music-list.json file.
* @param loop -1 to infinite playback, 0 will not play, n will play n times.
*
* Before play it, you need to load with PT_SoundManagerLoadMusicFile.
*
* \sa PT_SoundManagerLoadMusics
* \sa PT_SoundManagerPauseMusic
* \sa PT_SoundManagerStopMusic
* \sa PT_SoundManagerSetMusicVolume
* \sa PT_SoundManagerDestroyMusic
*/
void PT_SoundManagerPlayMusic( const char* utf8_musicName, int loop );

/**
* \brief Play any sample loaded from /sound/sample/list.json.
*
* @param utf8_sampleName The sample name from sample-list.json.
* @param loop -1 to infinite playback, 0 will not play, n will play n times.
*
* 
* \sa PT_SoundManagerLoadSamples
* \sa PT_SoundManagerPauseSample
* \sa PT_SoundManagerStopSample
* \sa PT_SoundManagerSetSamplesVolume
*/
void PT_SoundManagerPlaySample( const char* utf8_sampleName, int loop );

/** 
* \brief Use this function to pause the music playback.
*
*
* \sa PT_SoundManagerLoadMusics
* \sa PT_SoundManagerPlayMusic
* \sa PT_SoundManagerStopMusic
* \sa PT_SoundManagerSetMusicVolume
* \sa PT_SoundManagerDestroyMusic
*/
void PT_SoundManagerPauseMusic( );

/**
* \brief Use this function to pause the samples from playback.
*
* @param value true to pause, false to unpause.
*
* \sa PT_SoundManagerLoadSamples
* \sa PT_SoundManagerPlaySample
* \sa PT_SoundManagerStopSamples
* \sa PT_SoundManagerSetSamplesVolume
*/
void PT_SoundManagerPauseSamples( SDL_bool value );

void PT_SoundManagerStopMusic( );

void PT_SoundManagerStopSamples( );

//0 no sound, 128 max
void PT_SoundManagerSetMusicVolume( int volume );

void PT_SoundManagerSetSamplesVolume( int volume );

int PT_SoundManagerGetSampleTagGroup( );
int PT_SoundManagerGetMusicTagGroup( );

#endif /* _PT_SOUNDMANAGER_H_ */



