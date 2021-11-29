/*
Copyright 2021 daywithstars

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

#include <SDL_log.h>

#include <PT_SoundInformation.h>

void PT_SoundInformationPrintAudioFormat( SDL_AudioFormat format ) {	
	SDL_Log("~~~~~ PT_SoundInformation: PrintAudioFormat ~~~~~\n");
	switch ( format )
	{
		case AUDIO_U8:
		SDL_Log("* AUDIO_U8 Unsigned 8-bit samples\n");
		break;
		
		case AUDIO_S8:
		SDL_Log("* AUDIO_S8 Signed 8-bit samples\n");
		break;
		
		case AUDIO_U16LSB:
		SDL_Log("* AUDIO_U16LSB Unsigned 16-bit samples\n");
		break;
		
		case AUDIO_S16LSB:
		SDL_Log("* AUDIO_S16LSB Signed 16-bit samples\n");
		break;
		
		case AUDIO_U16MSB:
		SDL_Log("* AUDIO_U16MSB Unsigned 16-bit samples big-endian Byte order\n");
		break;
		
		case AUDIO_S16MSB:
		SDL_Log("* AUDIO_S16MSB Signed 16-bit samples big-endian Byte order\n");
		break;
		
		case AUDIO_S32LSB:
		SDL_Log("* AUDIO_S32LSB 32-bit integer samples\n");
		break;
		
		case AUDIO_S32MSB:
		SDL_Log("* AUDIO_S32MSB 32-bit integer samples big-endian Byte order\n");
		break;
		
		default:
		SDL_Log("* Unknown Audio Format\n");
		break;
	}
	SDL_Log("~~~~~ End PT_SoundInformation: PrintAudioFormat\n");
	SDL_Log("\n");
}//PT_SoundInformationPrintAudioFormat

const char* PT_SoundInformationGetAudioFormatName( SDL_AudioFormat format ) {
	switch ( format )
	{
		case AUDIO_U8:
		return("AUDIO_U8");
		
		case AUDIO_S8:
		return("AUDIO_S8");
		
		case AUDIO_U16LSB:
		return("AUDIO_U16LSB");
		
		case AUDIO_S16LSB:
		return("AUDIO_S16LSB");
		
		case AUDIO_U16MSB:
		return("AUDIO_U16MSB");
		
		case AUDIO_S16MSB:
		return("AUDIO_S16MSB");
		
		case AUDIO_S32LSB:
		return("AUDIO_S32LSB");
		
		case AUDIO_S32MSB:
		return ("AUDIO_S32MSB");
		
		default:
		return ("Unknown Audio Format");
	}
}//PT_SoundInformationGetAudioFormatName

const char* PT_SoundInformationGetAudioFormatNameExtended( SDL_AudioFormat format ) {
	switch ( format )
	{
		case AUDIO_U8:
		return("Unsigned 8-bit samples");
		
		case AUDIO_S8:
		return("Signed 8-bit samples");
		
		case AUDIO_U16LSB:
		return("Unsigned 16-bit samples little-endian Byte order");
		
		case AUDIO_S16LSB:
		return("Signed 16-bit samples little-endian Byte order");
		
		case AUDIO_U16MSB:
		return("Unsigned 16-bit samples big-endian Byte order");
		
		case AUDIO_S16MSB:
		return("Signed 16-bit samples big-endian Byte order");
		
		case AUDIO_S32LSB:
		return("Signed 32-bit integer samples little-endian Byte order");
		
		case AUDIO_S32MSB:
		return ("Signed 32-bit integer samples big-endian Byte order");
		
		default:
		return ("Unknown Audio Format");
	}
}//PT_SoundInformationGetAudioFormatNameExtended



