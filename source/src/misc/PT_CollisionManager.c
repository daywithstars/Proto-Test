/*
Copyright 2022 daywithstars

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

#include <SDL_log.h>

#include <PT_CollisionManager.h>
#include <PT_SpriteList.h>
#include <PT_Collider.h>
#include <PT_Camera.h>



typedef struct {
	PT_SpriteList* list;
}PT_CollisionManager;


static PT_CollisionManager* ptCollisionManager = NULL;


//===================================== PRIVATE Functions

void PT_CollisionManagerCheckGroupCollision( unsigned int num, PT_Sprite** values );
void PT_CollisionManagerCheckColliderCollision( PT_Sprite* sprite_1, PT_Sprite* sprite_2,
	unsigned int num_1, PT_Collider* values_1,
	unsigned int num_2, PT_Collider* values_2 );

//===================================== PUBLIC Functions


SDL_bool PT_CollisionManagerCreate() {
	if ( ptCollisionManager )
	{
		return SDL_TRUE;
	}
	ptCollisionManager = (PT_CollisionManager*)malloc(sizeof(PT_CollisionManager));
	if ( !ptCollisionManager )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_CollisionManagerCreate: Not enough memory\n");
		return SDL_FALSE;
	}
	SDL_memset(ptCollisionManager, 0, sizeof(PT_CollisionManager));
	

	return SDL_TRUE;
}//PT_CollisionManagerCreate

void PT_CollisionManagerDestroy() {
	if ( !ptCollisionManager )
	{
		return;
	}
	
	PT_SpriteListDestroy(ptCollisionManager->list, SDL_FALSE);
	
	free(ptCollisionManager);
	ptCollisionManager = NULL;
}//PT_CollisionManagerDestroy

void PT_CollisionManagerAdd( const char* utf8_group, PT_Sprite* sprite ) {

	ptCollisionManager->list = PT_SpriteListCat(ptCollisionManager->list, utf8_group, sprite);
}//PT_CollisionManagerAdd

void PT_CollisionManagerUpdate( ) {
	PT_SpriteList* pList = ptCollisionManager->list;
	while ( pList )
	{
		PT_CollisionManagerCheckGroupCollision(pList->numValues, pList->values);
		pList = pList->next;
	}
}//PT_CollisionManagerUpdate

//===================================== PRIVATE Functions

void PT_CollisionManagerCheckGroupCollision( unsigned int num, PT_Sprite** values ) {
	if ( num < 2 )
	{
		return;
	}
	
	for ( unsigned int i = 0; i < num; i++ )
	{
		for ( unsigned int j = 0; j < num; j++ )
		{
			if ( i == j )
			{
				continue;
			}
			
			//Only for sprites on the screen.
			if ( values[i]->dstRect.x  <= PT_CameraGetX() + PT_CameraGetWidth() && 
			values[i]->dstRect.x + values[i]->dstRect.w >= PT_CameraGetX() &&
			values[i]->dstRect.y <= PT_CameraGetY() + PT_CameraGetHeight() &&
			values[i]->dstRect.y + values[i]->dstRect.h >= PT_CameraGetY() )
			{
				
				if ( values[j]->dstRect.x  <= PT_CameraGetX() + PT_CameraGetWidth() && 
				values[j]->dstRect.x + values[j]->dstRect.w >= PT_CameraGetX() &&
				values[j]->dstRect.y <= PT_CameraGetY() + PT_CameraGetHeight() &&
				values[j]->dstRect.y + values[j]->dstRect.h >= PT_CameraGetY() )
				{
					PT_CollisionManagerCheckColliderCollision(values[i], values[j], 
						values[i]->numColliders, values[i]->colliders,
						values[j]->numColliders, values[j]->colliders);
				}
				else {
					continue;
				}
			}
			else {
				break;
			}
		}
	}
}//PT_CollisionManagerCheckGroupCollision

void PT_CollisionManagerCheckColliderCollision( PT_Sprite* sprite_1, PT_Sprite* sprite_2,
	unsigned int num_1, PT_Collider* values_1,
	unsigned int num_2, PT_Collider* values_2 ) {
	
	if ( num_1 == 0 || num_2 == 0 )
	{
		return;
	}
	
	for ( unsigned int i = 0; i < num_1; i++ )
	{
		for ( unsigned int j = 0; j < num_2; j++ )
		{
			if ( PT_ColliderTestCollision(values_1[i], sprite_1->dstRect.x, sprite_1->dstRect.y,
				values_2[j], sprite_2->dstRect.x, sprite_2->dstRect.y) )
			{
				PT_SpriteCollisionWith(sprite_1, values_1[i], values_2[j]);
				PT_SpriteCollisionWith(sprite_2, values_2[j], values_1[i]);
				
				sprite_1->collisionColliderName = values_1[i].name;
				sprite_1->collisionTargetColliderName = values_2[j].name;
				
				sprite_2->collisionColliderName = values_2[j].name;
				sprite_2->collisionTargetColliderName = values_1[i].name;
			}
		}
	}
}//PT_CollisionManagerCheckColliderCollision






