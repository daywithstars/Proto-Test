/*
Copyright 2022 daywithstars

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

#include <SDL.h>

#include <PT_SoundManager.h>
#include <PT_SoundInformation.h>
#include <PT_Music.h>
#include <PT_SampleList.h>
#include <PT_MusicList.h>
#include <PT_Parse.h>
#include <PT_String.h>
#include <PT_Graphics.h>


typedef enum {
	PT_SOUNDMANAGER_FORMAT_NONE,
	PT_SOUNDMANAGER_FORMAT_MP3,
	PT_SOUNDMANAGER_FORMAT_OGG,
	PT_SOUNDMANAGER_FORMAT_MP3_OGG,
}PT_SoundManagerLoadedFormats;

typedef struct {
	PT_SoundManagerLoadedFormats loadedFormats;

	json_value* sampleListJsonValue;
	PT_SampleList* sampleList;
	
	json_value* musicListJsonValue;
	PT_MusicList* musicList;
}PT_SoundManager;


extern PT_String* gRootDir;

static PT_SoundManager* ptSoundManager = NULL;


//===================================== PRIVATE PT_SoundManager Functions

SDL_bool PT_SoundManagerParse( );

//===================================== PUBLIC Functions

void PT_SoundManagerCreate( ) {
	if ( ptSoundManager )
	{
		return;
	}
	
	int flags = MIX_INIT_MP3 | MIX_INIT_OGG | MIX_INIT_MOD;
	int returnFlags = Mix_Init(flags);
	if ( returnFlags == 0 )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, 
		"PT: PT_SoundManagerCreate: Cannot load any other format, just wave.\n");
	}
	if ( Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) == -1 )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "SS: SoundManager-initialize: %s\n", Mix_GetError());
		return;
	}
	
	
	ptSoundManager = (PT_SoundManager*)malloc(sizeof(PT_SoundManager));
	if ( !ptSoundManager )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_SoundMangerCreate: Not enough memory\n");
		return;
	}
	SDL_memset(ptSoundManager, 0, sizeof(PT_SoundManager));
	
	
	if ( returnFlags == flags )
	{
		ptSoundManager->loadedFormats = PT_SOUNDMANAGER_FORMAT_MP3_OGG;
	}
	else if ( returnFlags == MIX_INIT_MP3 )
	{
		ptSoundManager->loadedFormats = PT_SOUNDMANAGER_FORMAT_MP3;
	}
	else if ( returnFlags == MIX_INIT_OGG )
	{
		ptSoundManager->loadedFormats = PT_SOUNDMANAGER_FORMAT_OGG;
	}
	else if ( returnFlags == 0 )
	{
		ptSoundManager->loadedFormats = PT_SOUNDMANAGER_FORMAT_NONE;
	}

	SDL_Log("~~~~~ PT_SoundManagerCreate ~~~~~\n");
	int i, count = SDL_GetNumAudioDevices(0);
	for ( i = 0; i < count; ++i)
	{
		SDL_Log("* Audio device %d: %s\n", i, SDL_GetAudioDeviceName(i, 0));
	}
	if ( ptSoundManager->loadedFormats == PT_SOUNDMANAGER_FORMAT_NONE )
	{
		SDL_Log("* Formats loaded: only wave\n");
	}
	else if ( ptSoundManager->loadedFormats == PT_SOUNDMANAGER_FORMAT_MP3 )
	{
		SDL_Log("* Formats loaded: Wave, Mp3\n");
	}
	else if ( ptSoundManager->loadedFormats == PT_SOUNDMANAGER_FORMAT_OGG )
	{
		SDL_Log("* Formats loaded: Wave, Ogg\n");
	}
	else if ( ptSoundManager->loadedFormats == PT_SOUNDMANAGER_FORMAT_MP3_OGG )
	{
		SDL_Log("* Formats loaded: Wave, Mp3, Ogg\n");
	}
	
	
	if ( !PT_SoundManagerParse() )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_SoundManagerCreate!\n");
		return;
	}
	
	SDL_Log("~~~~~\n");
}//PT_SoundManagerCreate

void PT_SoundManagerDestroy( ) {
	if ( !ptSoundManager )
	{
		return;
	}
	
	if ( ptSoundManager->sampleListJsonValue )
	{
		json_value_free(ptSoundManager->sampleListJsonValue);
	}
	if ( ptSoundManager->musicListJsonValue )
	{
		json_value_free(ptSoundManager->musicListJsonValue);
	}
	
	PT_SampleListDestroy(ptSoundManager->sampleList);
	PT_MusicListDestroy(ptSoundManager->musicList);
	

	Mix_CloseAudio();
	Mix_Quit();
	
	free(ptSoundManager);
	ptSoundManager = NULL;
}//PT_SoundManagerDestroy

SDL_bool PT_SoundManagerLoadMusics( ) {
	if ( !ptSoundManager )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, 
		"PT: PT_SoundManagerLoadMusicFile: Invalid ptSoundManager!\n");
		return SDL_FALSE;
	}
	if ( !ptSoundManager->musicListJsonValue )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, 
		"PT: PT_SoundManagerLoadMusicFile: Invalid musicListJsonValue!\n");
		return SDL_FALSE;	
	}
	
	json_object_entry entry = 
		PT_ParseGetObjectEntry_json_value(ptSoundManager->musicListJsonValue, "musics");
		
	if ( !entry.value )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, 
		"PT: PT_SoundManagerLoadMusicFile: Cannot find music array from music-list.json!\n");
		return SDL_FALSE;
	}
	
	for ( unsigned int i = 0; i < entry.value->u.array.length; i++ )
	{
		//Load the music and list into the list, without the extension. 
		//Test if the format is supported. 
		
		PT_String* musicName = PT_StringCreate();
		PT_String* musicExtension = PT_StringCreate();
		
		PT_StringCopyFrom(
			musicName, 
			entry.value->u.array.values[i]->u.string.ptr,
			0,
PT_StringGetOccurrencePositionBasicString(entry.value->u.array.values[i]->u.string.ptr, ".", 1) - 1,
			0
		);
		PT_StringCopyFrom(
			musicExtension, 
			entry.value->u.array.values[i]->u.string.ptr,
PT_StringGetOccurrencePositionBasicString(entry.value->u.array.values[i]->u.string.ptr, ".", 1),
			PT_StringCountBasicString(entry.value->u.array.values[i]->u.string.ptr),
			0
		);
		
		//Supported format tests.
		if ( PT_StringMatch(musicExtension, "mp3") )
		{
			if ( ptSoundManager->loadedFormats != PT_SOUNDMANAGER_FORMAT_MP3 &&
				PT_SOUNDMANAGER_FORMAT_MP3_OGG )
			{
				PT_GraphicsShowSimpleMessageBox(
					SDL_MESSAGEBOX_WARNING,
					"PT: PT_SoundManagerLoadMusics",
					"The system does not support mp3. Alternative: try ogg or wav."
				);
			}
		}
		if ( PT_StringMatch(musicExtension, "ogg") )
		{
			if ( ptSoundManager->loadedFormats != PT_SOUNDMANAGER_FORMAT_OGG &&
				PT_SOUNDMANAGER_FORMAT_MP3_OGG )
			{
				PT_GraphicsShowSimpleMessageBox(
					SDL_MESSAGEBOX_WARNING,
					"PT: PT_SoundManagerLoadMusics",
					"The system does not support ogg. Alternative: try mp3 or wav."
				);
			}
		}
		
		
		PT_Music* music = PT_MusicCreate();
		
		if ( !PT_MusicLoad(music, entry.value->u.array.values[i]->u.string.ptr) )
		{
			PT_MusicDestroy(music);
			PT_StringDestroy(musicName);
			PT_StringDestroy(musicExtension);
			continue;
		}
		
		ptSoundManager->musicList = 
		PT_MusicListAdd(ptSoundManager->musicList, (char*)musicName->utf8_string, music);
		
		PT_StringDestroy(musicName);
		PT_StringDestroy(musicExtension);
	}
	
	return SDL_TRUE;
}//PT_SoundManagerLoadMusicFile

SDL_bool PT_SoundManagerLoadSamples( ) {
	if ( !ptSoundManager )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, 
		"PT: PT_SoundManagerLoadSamples: Invalid ptSoundManager!\n");
		return SDL_FALSE;
	}
	if ( !ptSoundManager->sampleListJsonValue )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, 
		"PT: PT_SoundManagerLoadMusicFile: Invalid sampleListJsonValue!\n");
		return SDL_FALSE;	
	}
	
	json_object_entry entry = 
		PT_ParseGetObjectEntry_json_value(ptSoundManager->sampleListJsonValue, "samples");
		
	if ( !entry.value )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, 
		"PT: PT_SoundManagerLoadMusicFile: Cannot find sample array from sample-list.json!\n");
		return SDL_FALSE;
	}
	
	for ( unsigned int i = 0; i < entry.value->u.array.length; i++ )
	{
		//Loads the sample and list into the list, without the extension. 
		//Test if the format is supported. 
		
		PT_String* sampleName = PT_StringCreate();
		PT_String* musicExtension = PT_StringCreate();
		
		PT_StringCopyFrom(
			sampleName, 
			entry.value->u.array.values[i]->u.string.ptr,
			0,
PT_StringGetOccurrencePositionBasicString(entry.value->u.array.values[i]->u.string.ptr, ".", 1) - 1,
			0
		);
		PT_StringCopyFrom(
			musicExtension, 
			entry.value->u.array.values[i]->u.string.ptr,
PT_StringGetOccurrencePositionBasicString(entry.value->u.array.values[i]->u.string.ptr, ".", 1),
			PT_StringCountBasicString(entry.value->u.array.values[i]->u.string.ptr),
			0
		);
		
		//Supported format tests.
		if ( !PT_StringMatch(musicExtension, "wav") )
		{
			if ( ptSoundManager->loadedFormats != PT_SOUNDMANAGER_FORMAT_MP3 &&
				PT_SOUNDMANAGER_FORMAT_MP3_OGG )
			{
				PT_GraphicsShowSimpleMessageBox(
					SDL_MESSAGEBOX_WARNING,
					"PT: PT_SoundManagerLoadSamples",
					"The System does not support sample in any other format instead wave"
				);
				PT_GraphicsShowSimpleMessageBox(
					SDL_MESSAGEBOX_WARNING,
					"PT: PT_SoundManagerLoadSamples",
					"If your sample is an wav audio and are using other file extension, change to \".wav\" and this message will not appear."
				);
			}
		}
		
		PT_Sample* sample = PT_SampleCreate();
		
		if ( !PT_SampleLoad(sample, entry.value->u.array.values[i]->u.string.ptr) )
		{
			PT_SampleDestroy(sample);
			PT_StringDestroy(sampleName);
			PT_StringDestroy(musicExtension);
			continue;
		}
		
		ptSoundManager->sampleList = 
		PT_SampleListAdd(ptSoundManager->sampleList, (char*)sampleName->utf8_string, sample);
		
		PT_StringDestroy(sampleName);
		PT_StringDestroy(musicExtension);
	}
	
	return SDL_TRUE;
}//PT_SoundManagerLoadSamples

void PT_SoundManagerPlayMusic( const char* utf8_musicName, int loop ) {
	if ( !ptSoundManager )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, 
		"PT: PT_SoundManagerPlayMusic: Invalid ptSoundManager\n");
		return;
	}
	
	PT_MusicList* node = PT_MusicListGet(ptSoundManager->musicList, utf8_musicName);
	if ( !node )
	{
		SDL_Log("(*)PT: PT_SoundManagerPlayMusic: Cannot fined loaded %s music\n", utf8_musicName);
		return;
	}
	
	PT_MusicPlay(node->value, loop);

}//PT_SoundManagerPlayMusic

void PT_SoundManagerPlaySample( const char* utf8_sampleName, int loop ) {
	if ( !ptSoundManager )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, 
		"PT: PT_SoundManagerPlaySample: Invalid ptSoundManager\n");
		return;
	}
	
	PT_SampleList* node = PT_SampleListGet(ptSoundManager->sampleList, utf8_sampleName);
	if ( !node )
	{
		SDL_Log("(*)PT: PT_SoundManagerPlaySample: Cannot fined loaded %s sample\n", utf8_sampleName);
		return;
	}
	
	PT_SamplePlay(node->value, loop);
}//PT_SoundManagerPlaySample

void PT_SoundManagerPauseMusic( ) {
	if ( !ptSoundManager )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, 
		"PT: PT_SoundManagerPauseMusic: Invalid ptSoundManager\n");
		return;
	}
	
	Mix_PauseMusic();
	
}//PT_SoundManagerPauseMusic

void PT_SoundManagerPauseSamples( SDL_bool value ) {
	int totalChannels = Mix_GroupCount(PT_SoundManagerGetSampleTagGroup());
	
	while ( totalChannels >= 1 )
	{
		const int channel = Mix_GroupAvailable(PT_SoundManagerGetSampleTagGroup());
		if ( value && channel != -1 )
		{
			Mix_Pause(channel);
		}
		else if ( value == SDL_FALSE && channel != -1 ) 
		{	
			Mix_Resume(channel);
		}
		totalChannels --;
	}
}//PT_SoundManagerPauseSample

void PT_SoundManagerStopMusic( ) {
	Mix_HaltMusic();
}//PT_SoundManagerStopMusic

void PT_SoundManagerStopSamples( ) {
	Mix_HaltGroup(PT_SoundManagerGetSampleTagGroup());
}//PT_SoundManagerStopSamples

void PT_SoundManagerSetMusicVolume( int volume ) {
	Mix_VolumeMusic(volume);
}//PT_SoundManagerSetMusicVolume

void PT_SoundManagerSetSamplesVolume( int volume ) {
	int totalChannels = Mix_GroupCount(PT_SoundManagerGetSampleTagGroup());
	
	while ( totalChannels >= 1 )
	{
		const int channel = Mix_GroupAvailable(PT_SoundManagerGetSampleTagGroup());
	
		if ( channel != -1 )
		{
			printf("prev sample volume: %d new volume: %d\n", Mix_Volume(channel, volume), volume);
		}
		
		totalChannels --;
	}
}//PT_SoundManagerSetSamplesVolume

int PT_SoundManagerGetSampleTagGroup( ) {
	return 3;
}
int PT_SoundManagerGetMusicTagGroup( ) {
	return 7;
}

//===================================== PRIVATE PT_SoundManager Functions

SDL_bool PT_SoundManagerParse( ) {
	/*
		see the templates:
		games/default-templates/sound-folder/
	*/

	if ( !ptSoundManager )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_SoundManagerParse: Invalid PT_SoundManager\n");
		return SDL_FALSE;
	}
	
	ptSoundManager->sampleListJsonValue = 
	PT_ParseGetJsonValueFromFile("assets/sound/sample/sample-list.json", SDL_TRUE);
	if ( !ptSoundManager->sampleListJsonValue )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_SoundManagerParse!\n");
		return SDL_FALSE;
	}


	ptSoundManager->musicListJsonValue = 
	PT_ParseGetJsonValueFromFile("assets/sound/music/music-list.json", SDL_TRUE);
	if ( !ptSoundManager->musicListJsonValue )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_SoundManagerParse!\n");
		return SDL_FALSE;
	}

	return SDL_TRUE;
}//PT_SoundManagerParse



