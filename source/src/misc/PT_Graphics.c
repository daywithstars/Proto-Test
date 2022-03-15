/*
Copyright 2022 daywithstars

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

#include <SDL_image.h>
#include <SDL_log.h>

#include <PT_Graphics.h>
#include <PT_TextureList.h>
#include <PT_FontList.h>
#include <PT_Parse.h>

extern PT_String* gRootDir;

struct pt_graphics {
	SDL_Window* window;
	int windowWidth;
	int windowHeight;
	
	SDL_Renderer* renderer;
	SDL_Color clearColor;
	
	PT_TextureList* textureList;
	PT_TextureList* fontTextureList;
	PT_FontList* fontList;
};

static PT_Graphics* ptGraphics = NULL;




SDL_bool PT_GraphicsParseSettings( ) {
	/*
		see the template: games/default-templates/game-folder-settings-template.json
	*/
	PT_Parse* parse = PT_ParseCreate();
	
	if ( !PT_ParseOpenFile(parse, "settings.json", SDL_TRUE) )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_GraphicsParseSettings!\n");
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "* Make sure your game folder is the same as the global-settings.json and try again\n");
		SDL_Log("(*)PT: PT_GraphicsParseSettings: Trying to create settings.json on your game folder\n");
		
		if ( !PT_ParseLoadTemplate(parse, 
"{\n \
	\"graphics\": {\n \
		\"screenWidth\": 640,\n\
		\"screenHeight\": 480,\n \
		\"renderDrawColor\": [0, 255, 0, 255]\n \
	}\n\
}") )
		{
			SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_GraphicsParseSettings!\n");
			return SDL_FALSE;
		}
		
		if ( !PT_ParseSaveFile(parse, "settings.json", SDL_TRUE) )
		{
			SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_GraphicsParseSettings!\n");
		}
	}
	
	//Loads screen size settings from settings.json
	//set window size
	json_object_entry entry = PT_ParseGetObjectEntry(parse, "graphics screenWidth");
	ptGraphics->windowWidth = entry.value->u.integer;
	entry = PT_ParseGetObjectEntry(parse, "graphics screenHeight");
	ptGraphics->windowHeight = entry.value->u.integer;
	
	//set render clear color
	entry = PT_ParseGetObjectEntry(parse, "graphics renderDrawColor");
	const SDL_Color color = {
		entry.value->u.array.values[0]->u.integer,
		entry.value->u.array.values[1]->u.integer,
		entry.value->u.array.values[2]->u.integer,
		entry.value->u.array.values[3]->u.integer
	};
	PT_GraphicsSetRenderClearColor(color);
	
	PT_ParseDestroy(parse);
	
	return SDL_TRUE;
}//PT_GraphicsParseSettings

void PT_GraphicsParseImages() {
	/*
		see the templates:
		games/default-templates/img-folder/
	*/
	PT_Parse* parseFolders = PT_ParseCreate();
	
	if ( !PT_ParseOpenFile(parseFolders, "assets/img/folders.json", SDL_TRUE) )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_GraphicsParseImages!\n");
		return;
	}
	
	//Search for folders
	json_object_entry entry = PT_ParseGetObjectEntry(parseFolders, "folders");
	if ( entry.value )
	{
		if ( entry.value->parent )
		{
			if ( entry.value->type == json_array )
			{
				for ( unsigned int i = 0; i < entry.value->u.array.length; i++ )
				{
					//Search for lists.json
					PT_Parse* parseList = PT_ParseCreate();
					
					PT_String* parseListPath = PT_StringCreate();
					PT_StringInsert(&parseListPath, "/img-list.json", 0);
					PT_StringInsert(&parseListPath, 
						entry.value->u.array.values[i]->u.string.ptr, 0);
					PT_StringInsert(&parseListPath, "assets/img/", 0);
					
					PT_ParseOpenFile(parseList, (char*)parseListPath->utf8_string, SDL_TRUE);
					
					const json_value* jsonValue = PT_ParseGetJsonValuePointer(parseList);
					if ( jsonValue )
					{
						for ( unsigned int j = 0; j < jsonValue->u.object.length; j++ )
						{
							//Loading the images with the name and path from list.json
							PT_String* imagePath = PT_StringCreate();
							
							PT_StringInsert(&imagePath, 
								jsonValue->u.object.values[j].value->u.string.ptr, 0);
							PT_StringInsert(&imagePath, "/", 0);
							PT_StringInsert(&imagePath, 
								entry.value->u.array.values[i]->u.string.ptr, 0);
							PT_StringInsert(&imagePath, "assets/img/", 0);
							
							PT_GraphicsLoadTexture((char*)imagePath->utf8_string, 
								jsonValue->u.object.values[j].name);
							
							PT_StringDestroy(imagePath);
							imagePath = NULL;
						}
					}
					
					PT_ParseDestroy(parseList);
					parseList = NULL;
					
					PT_StringDestroy(parseListPath);
					parseListPath = NULL;
				}
			}
		}
	}
	
	
	PT_ParseDestroy(parseFolders);
}//PT_GraphicsParseImages


SDL_bool PT_GraphicsCreate( ) {
	if ( ptGraphics ) 
	{
		return SDL_TRUE;
	}
	
	if ( !IMG_Init(IMG_INIT_PNG) )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, 
		"PT: PT_GraphicsCreate: Cannot load support to PNG image, use the default .bmp\n");
	}
	if ( TTF_Init() < 0 )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION,
		"PT: PT_GraphicsCreate: Cannot load the ttf engine.\n");
	}
	
	ptGraphics = (PT_Graphics*)malloc(sizeof(struct pt_graphics));
	if ( !ptGraphics )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_GraphicsCreate: Not enough memory\n");
		return SDL_FALSE;
	}
	SDL_memset(ptGraphics, 0, sizeof(PT_Graphics));
	
	if ( !PT_GraphicsParseSettings() )
	{
		PT_GraphicsDestroy();
		return SDL_FALSE;
	}
	
	ptGraphics->window = SDL_CreateWindow("Proto-Test", 100, 100, ptGraphics->windowWidth,
		ptGraphics->windowHeight, SDL_WINDOW_SHOWN);
	if ( !ptGraphics->window )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_GraphcisCreate: %s\n", SDL_GetError());
		PT_GraphicsDestroy();
		return SDL_FALSE;
	}
	
	SDL_Surface* logo = IMG_Load("logo.png");
	if ( logo )
	{
		SDL_SetWindowIcon(ptGraphics->window, logo);
		SDL_FreeSurface(logo);
	}
	
	ptGraphics->renderer = SDL_CreateRenderer(ptGraphics->window, -1, SDL_RENDERER_SOFTWARE);
	if ( !ptGraphics->renderer )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_GraphcisCreate: %s\n", SDL_GetError());
		PT_GraphicsDestroy();
		return SDL_FALSE;
	}
	if ( SDL_SetRenderDrawBlendMode(ptGraphics->renderer, SDL_BLENDMODE_BLEND) )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_GraphcisCreate: %s\n", SDL_GetError());
	}

	
	PT_GraphicsParseImages();
	return SDL_TRUE;
}//PT_GraphicsCreate

void PT_GraphicsDestroy() {
	if ( !ptGraphics )
	{
		return;
	}
	
	PT_TextureListDestroy(ptGraphics->textureList);
	PT_TextureListDestroy(ptGraphics->fontTextureList);
	PT_FontListDestroy(ptGraphics->fontList);

	if ( ptGraphics->renderer )
	{
		SDL_DestroyRenderer(ptGraphics->renderer);
	}
	if ( ptGraphics->window )
	{
		SDL_DestroyWindow(ptGraphics->window);
	}

	free(ptGraphics);
	
	ptGraphics = NULL;
	
	IMG_Quit();
	TTF_Quit();
}//PT_GraphicsDestroy

int PT_GraphicsShowSimpleMessageBox( Uint32 flags, const char *utf8_title, const char *utf8_message ) {
	int returnStatus = 0;
	if ( ptGraphics )
	{
		returnStatus = SDL_ShowSimpleMessageBox(flags, utf8_title, utf8_message, ptGraphics->window);
	}
	else {
		returnStatus = SDL_ShowSimpleMessageBox(flags, utf8_title, utf8_message, NULL);
	}
	
	return returnStatus;
}//PT_GraphicsShowSimpleMessageBox

void PT_GraphicsLoadTexture( const char* utf8_filePath, const char* utf8_name ) {

	PT_String* path = PT_StringCreate();
	
	PT_StringInsert(&path, utf8_filePath, 0);
	PT_StringInsert(&path, (char*)gRootDir->utf8_string, 0);
	
	SDL_Surface* loadedSurface = IMG_Load((char*)path->utf8_string);
	if ( !loadedSurface )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, 
		"PT: PT_GraphicsLoadTexture: Unable to load texture: %s\n", (char*)path->utf8_string);
		
		PT_StringDestroy(path);
		return;
	}
	if ( SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 255, 0, 255)) < 0 )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_GraphicsLoadTexture: %s\n", SDL_GetError());
	}
	
	SDL_Texture* texture = SDL_CreateTextureFromSurface(ptGraphics->renderer, loadedSurface);
	if ( !texture )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_GraphicsLoadTexture: %s\n", SDL_GetError());
		
		PT_StringDestroy(path);
		SDL_FreeSurface(loadedSurface);
		return;
	}
	
	SDL_FreeSurface(loadedSurface);
	
	ptGraphics->textureList = PT_TextureListAdd(ptGraphics->textureList, utf8_name, texture);
	
	PT_StringDestroy(path);
}//PT_GraphicsLoadTexture

TTF_Font* PT_GraphicsLoadFont( const char* utf8_filePath, int fontSize, SDL_bool defaultPath ) {
	PT_String* path = PT_StringCreate();
	
	if ( !PT_StringInsert(&path, utf8_filePath, 0) )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_GraphicsLoadFont!\n");
		PT_StringDestroy(path);
		return NULL;
	}
	if ( defaultPath )
	{
		if ( !PT_StringInsert(&path, (char*)gRootDir->utf8_string, 0) )
		{
			SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_GraphicsLoadFont!\n");
			PT_StringDestroy(path);
			return NULL;
		}
	}
	
	TTF_Font* font = TTF_OpenFont((char*)path->utf8_string, fontSize);
	if ( !font )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_GraphicsLoadFont: %s\n", TTF_GetError());
		PT_StringDestroy(path);
		return NULL;
	}
	
	return font;
}//PT_GraphicsLoadFont

SDL_bool PT_GraphicsLoadFonts( ) {
	PT_Parse* parse = PT_ParseCreate();
	if ( !parse )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_GraphicsLoadFonts!\n");
		return SDL_FALSE;
	}
	if ( !PT_ParseOpenFile(parse, "assets/font/font-list.json", SDL_TRUE) )
	{
		PT_ParseDestroy(parse);
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_GraphicsLoadFonts!\n");
		return SDL_FALSE;
	}
	
	json_object_entry entry = PT_ParseGetObjectEntry(parse, "font-list");
	if ( !entry.value )
	{
		PT_ParseDestroy(parse);
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_GraphicsLoadFonts!\n");
		return SDL_FALSE;
	}
	
	for ( unsigned int i = 0; i < entry.value->u.array.length; i++ )
	{
		PT_String* fontPath = PT_StringCreate();
		if ( !fontPath )
		{
			PT_ParseDestroy(parse);
			SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_GraphicsLoadFonts!\n");
			return SDL_FALSE;
		}
		if ( !PT_StringInsert(&fontPath, ".json", 0) )
		{
			PT_StringDestroy(fontPath);
			PT_ParseDestroy(parse);
			SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_GraphicsLoadFonts!\n");
			return SDL_FALSE;
		}
		if ( !PT_StringInsert(&fontPath, entry.value->u.array.values[i]->u.string.ptr, 0) )
		{
			PT_StringDestroy(fontPath);
			PT_ParseDestroy(parse);
			SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_GraphicsLoadFonts!\n");
			return SDL_FALSE;
		}
		if ( !PT_StringInsert(&fontPath, "assets/font/", 0) )
		{
			PT_StringDestroy(fontPath);
			PT_ParseDestroy(parse);
			SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_GraphicsLoadFonts!\n");
			return SDL_FALSE;	
		}
		
		
		PT_Parse* fontParse = PT_ParseCreate();
		if ( !fontParse )
		{
			PT_StringDestroy(fontPath);
			PT_ParseDestroy(parse);
			SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_GraphicsLoadFonts!\n");
			return SDL_FALSE;
		}
		if ( !PT_ParseOpenFile(fontParse, (char*)fontPath->utf8_string, SDL_TRUE) )
		{
			PT_StringDestroy(fontPath);
			PT_ParseDestroy(parse);
			PT_ParseDestroy(fontParse);
			SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_GraphicsLoadFonts!\n");
			return SDL_FALSE;
		}
		
		PT_Font* font = PT_FontCreate(PT_ParseGetJsonValuePointer(fontParse));
		if ( !font )
		{
			PT_StringDestroy(fontPath);
			PT_ParseDestroy(parse);
			PT_ParseDestroy(fontParse);
			SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_GraphicsLoadFonts!\n");
			return SDL_FALSE;
		}
		
		json_object_entry fontEntry = PT_ParseGetObjectEntry(fontParse, "name");
		if ( !fontEntry.value )
		{
			PT_StringDestroy(fontPath);
			PT_ParseDestroy(parse);
			PT_ParseDestroy(fontParse);
			PT_FontDestroy(font);
			SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, 
			"PT: PT_GraphicsLoadFonts: Cannot find \"name\" on font-template.json\n");
			return SDL_FALSE;
		}
		
		ptGraphics->fontList = PT_FontListAdd(ptGraphics->fontList, fontEntry.value->u.string.ptr, font);
		
		PT_StringDestroy(fontPath);
		PT_ParseDestroy(fontParse);
	}
	
	PT_ParseDestroy(parse);
	
	return SDL_TRUE;
}//PT_GraphicsLoadFonts

void PT_GraphicsSetViewport( const SDL_Rect* rect ) {
	SDL_RenderSetViewport(ptGraphics->renderer, rect);
}//PT_GraphicsSetViewport

void PT_GraphicsSetRenderClearColor( SDL_Color color ) {
	ptGraphics->clearColor = color;
}//PT_GraphicsSetRenderClearColor

void PT_GraphicsSetRenderDrawColor( SDL_Color color ) {
	SDL_SetRenderDrawColor(ptGraphics->renderer, color.r, color.g, color.b, color.a);
}//PT_GraphicsSetRenderDrawColor

void PT_GraphicsRenderFillRect( const SDL_Rect* rect ) {
	SDL_RenderFillRect(ptGraphics->renderer, rect);
}//PT_GraphicsRenderFillRect

void PT_GraphicsRenderFillRectF( const SDL_FRect* rect ) {
	SDL_RenderFillRectF(ptGraphics->renderer, rect);
}//PT_GraphicsRenderFillRectF

void PT_GraphicsRenderTextSolid( const char* utf8_font, const char* utf8_text, 
	const char* utf8_fontTextureName ) {
	
	PT_FontList* node = PT_FontListGet(ptGraphics->fontList, utf8_font);
	if ( !node )
	{
		SDL_Log("(*)PT: PT_GraphicsRenderTextSolid: Cannot find font: %s\n", utf8_font);
		return;
	}
	SDL_Surface* surface = TTF_RenderUTF8_Solid(node->value->font, utf8_text, node->value->color);
	if ( !surface )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_GraphicsRenderTextSolid: %s\n", TTF_GetError());
		return;
	}
	
	SDL_Texture* texture = SDL_CreateTextureFromSurface(ptGraphics->renderer, surface);
	if ( !texture )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_GraphicsRenderTextSolid: %s\n", TTF_GetError());
		SDL_FreeSurface(surface);
		return;
	}
	
	ptGraphics->fontTextureList = 
		PT_TextureListAdd(ptGraphics->fontTextureList, utf8_fontTextureName, texture);
	
	SDL_FreeSurface(surface);
}//PT_GraphicsRenderTextSolid

void PT_GraphicsDrawTexture( const char* utf8_name, const SDL_Rect* srcRect, const SDL_Rect* dstRect,
	const double angle, const SDL_Point* center, const SDL_RendererFlip flip ) {
	
	PT_TextureList* node = PT_TextureListGet(ptGraphics->textureList, utf8_name);
	if ( !node )
	{
		return;
	}
	
	SDL_RenderCopyEx(ptGraphics->renderer, node->value, srcRect, dstRect, angle, center, flip);
}//PT_GraphicsDrawTexture

void PT_GraphicsDrawTextureF( const char* utf8_name, const SDL_Rect* srcRect, const SDL_FRect* dstRect,
	const double angle, const SDL_FPoint* center, const SDL_RendererFlip flip ) {
	
	PT_TextureList* node = PT_TextureListGet(ptGraphics->textureList, utf8_name);
	if ( !node )
	{
		return;
	}
	
	SDL_RenderCopyExF(ptGraphics->renderer, node->value, srcRect, dstRect, angle, center, flip);
}//PT_GraphicsDrawTextureF

void PT_GraphicsDrawFontTexture( const char* utf8_name, const SDL_Rect* srcRect, int x, int y,
	const double angle, const SDL_Point* center, const SDL_RendererFlip flip ) {

	PT_TextureList* node = PT_TextureListGet(ptGraphics->fontTextureList, utf8_name);
	if ( !node )
	{
		return;
	}

	int w, h;
	SDL_QueryTexture(node->value, NULL, NULL, &w, &h);
	const SDL_Rect dstRect = { x, y, w, h };
	SDL_RenderCopyEx(ptGraphics->renderer, node->value, srcRect, &dstRect, angle, center, flip);
}//PT_GraphicsDrawFontTexture

void PT_GraphicsRenderClear() {
	SDL_SetRenderDrawColor(ptGraphics->renderer, 
		ptGraphics->clearColor.r,
		ptGraphics->clearColor.g,
		ptGraphics->clearColor.b,
		ptGraphics->clearColor.a
	);
	
	SDL_RenderClear(ptGraphics->renderer);
}//PT_GraphicsRenderClear

void PT_GraphicsRenderPresent() {
	SDL_RenderPresent(ptGraphics->renderer);
}//PT_GraphicsRenderPresent




