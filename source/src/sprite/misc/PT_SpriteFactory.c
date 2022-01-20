/*
Copyright 2022 daywithstars

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

#include <PT_SpriteFactory.h>
#include <PT_String.h>
#include <PT_DynamicImage.h>



PT_Sprite* PT_SpriteFactoryGet( const char* utf8_spriteTemplate, const char* spriteType ) {
	PT_Sprite* sprite = NULL;

	if ( PT_StringMatchFast(spriteType, "PT_Sprite") )
	{
		sprite = PT_SpriteCreate(utf8_spriteTemplate, NULL, NULL);
	}
	else if ( PT_StringMatchFast(spriteType, "PT_DynamicImage") )
	{
		sprite = PT_DynamicImageCreate(utf8_spriteTemplate);
	}
	else {
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, 
		"PT: PT_SpriteFactoryGet: Unrecognized type: %s\n", spriteType);
	}
	
	return sprite;
}



