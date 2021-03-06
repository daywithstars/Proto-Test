/*
Copyright 2022 daywithstars

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

#ifndef _PT_SHOOTER_H_
#define _PT_SHOOTER_H_

#include <json.h>

#include <PT_Sprite.h>

typedef struct pt_shooter PT_Shooter;


PT_Sprite* PT_ShooterCreate( const char* utf8_spriteTemplate );

void PT_ShooterDestroy( void* _data ); 

void PT_ShooterCollisionWith( void* _data, PT_Collider own, PT_Collider target );

void PT_ShooterUpdate( void* _data, Sint32 elapsedTime );
void PT_ShooterDraw( void* _data );

#endif /* _PT_SHOOTER_H_ */



