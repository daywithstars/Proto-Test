/*
Copyright 2022 daywithstars

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

/** 
* \file 
*
* The Proto Test include header for Graphics handling. 
*/


#ifndef _PT_GRAPHICS_H_
#define _PT_GRAPHICS_H_

#include <SDL.h>
#include <SDL_ttf.h>


typedef struct pt_graphics PT_Graphics;



/**
* \brief Creates an load the default settings from games/settings.json. 
*
* \returns SDL_TRUE on success or SDL_FALSE on error.
*
* \sa PT_GraphicsDestroy
*/
SDL_bool PT_GraphicsCreate( );

/**
* \brief Frees PT_Graphics memory and all members variables. 
* 
* \sa PT_GraphicsCreate
*/
void PT_GraphicsDestroy( );

SDL_bool PT_GraphicsParseSettings( );

int PT_GraphicsShowSimpleMessageBox( Uint32 flags, const char *utf8_title, const char *utf8_message );

/**
* \brief Store one texture to the internal list of textures. 
*
* @param utf8_filePath The file path relative to gRootDir from global.c.
* @param utf8_name The unique texture name to the texture that will be loaded. 
*
* \sa PT_GraphicsDrawTexture
* \sa PT_GraphicsRenderPresent
* \sa PT_GraphicsRenderClear
*/
void PT_GraphicsLoadTexture( const char* utf8_filePath, const char* utf8_name );
void PT_GraphicsLoadTextureFromBuffer( void* buffer, int bufferSize, const char* type,
	const char* utf8_textureName );

TTF_Font* PT_GraphicsLoadFont( const char* utf8_filePath, int fontSize, SDL_bool defaultPath );
void PT_GraphicsLoadFontFromBuffer( void* buffer, int bufferSize, const char* utf8_fontName, int ptsize );


SDL_bool PT_GraphicsLoadImages( );
void PT_GraphicsUnloadImages( );

SDL_bool PT_GraphicsLoadFonts( );
void PT_GraphicsUnloadFonts( );

void PT_GraphicsSetViewport( const SDL_Rect* rect );

/**
* \brief Set the default color, that color will be the clear color. 
*
* @param color The SDL_Color structure. 
*
* \sa PT_GraphicsSetRenderDrawColor
* \sa PT_GraphicsRenderFillRect
* \sa PT_GraphicsRenderClear
* \sa PT_GraphicsRenderPresent
*/
void PT_GraphicsSetRenderClearColor( SDL_Color color );

/**
* \brief Set the color used for drawing operations like:
*
*	PT_GraphicsRenderClear, PT_GraphicsRenderFillRect.
*
* @param color The SDL_Color structure. 
*
* \sa PT_GraphicsSetRenderClearColor
* \sa PT_GraphicsRenderFillRect
* \sa PT_GraphicsRenderClear
* \sa PT_GraphicsRenderPresent
*/
void PT_GraphicsSetRenderDrawColor( SDL_Color color );

/** 
* \brief Fill a rectangle on the current rendering target with the drawing color.
*
* @param rect The SDL_Rect structure representing the rectangle to fill, or NULL for the entire 
*	rendering target.
*
* \sa PT_GraphicsSetRenderClearColor
* \sa PT_GraphicsSetRenderDrawColor
* \sa PT_GraphicsRenderClear
* \sa PT_GraphicsRenderPresent
*/ 
void PT_GraphicsRenderFillRect( const SDL_Rect* rect );

void PT_GraphicsRenderFillRectF( const SDL_FRect* rect );

void PT_GraphicsRenderTextSolid( const char* utf8_font, const char* utf8_text, 
	const char* utf8_fontTextureName );

/**
* \brief Draws an previous loaded texture to the current rendering target.
*
* @param utf8_name The name of previous loaded texture with PT_GraphicsLoadTexture.
* @param srcRect The source SDL_Rect structure or NULL for the entire texture. 
* @param dstRect The destination SDL_Rect structure or NULL for the entire rendering target.
* @param angle An angle in degrees that indicates the rotation that will be applied to dstRect,
*	rotating it in a clockwise direction.
* @param center A pointer to a point indicating the point around which dstRect will be rotated (if NULL,
*	rotation will be done around dstRect.w / 2, dstRect.h / 2).
* @param flip A SDL_RendererFlip value stating which flipping action should be performed on the texture.
*
* \sa PT_GraphicsLoadTexture
* \sa PT_GraphicsRenderPresent
* \sa PT_GraphicsRenderClear
*/
void PT_GraphicsDrawTexture( const char* utf8_name, const SDL_Rect* srcRect, const SDL_Rect* dstRect,
	const double angle, const SDL_Point* center, const SDL_RendererFlip flip );
	
void PT_GraphicsDrawTextureF( const char* utf8_name, const SDL_Rect* srcRect, const SDL_FRect* dstRect,
	const double angle, const SDL_FPoint* center, const SDL_RendererFlip flip );

void PT_GraphicsDrawFontTexture( const char* utf8_name, const SDL_Rect* srcRect, int x, int y,
	const double angle, const SDL_Point* center, const SDL_RendererFlip flip );

/**
* \brief Clear the current rendering target with the drawing color. 
*
* \sa PT_GraphicsSetRenderClearColor
* \sa PT_GraphicsSetRenderDrawColor
* \sa PT_GraphicsRenderFillRect
* \sa PT_GraphicsRenderPresent
*/
void PT_GraphicsRenderClear( );

/**
* \brief Update the screen with any rendering performed since the previous call.
*
* \sa PT_GraphicsSetRenderClearColor
* \sa PT_GraphicsSetRenderDrawColor
* \sa PT_GraphicsRenderFillRect
* \sa PT_GraphicsRenderClear
*/
void PT_GraphicsRenderPresent( );

#endif /* _PT_GRAPHICS_H_ */



