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

#include <PT_CollisionHandler.h>
#include <PT_SpriteList.h>
#include <PT_Collider.h>
#include <PT_Camera.h>



struct pt_collision_handler {
	SDL_bool cameraLimits;
	PT_SpriteList* list;
};




//===================================== PRIVATE Functions

void PT_CollisionHandlerCheckGroupCollision( unsigned int num, PT_Sprite** values, SDL_bool cameraLimits );
void PT_CollisionHandlerCheckColliderCollision( PT_Sprite* sprite_1, PT_Sprite* sprite_2,
	unsigned int num_1, PT_Collider* values_1,
	unsigned int num_2, PT_Collider* values_2 );

//===================================== PUBLIC Functions


PT_CollisionHandler* PT_CollisionHandlerCreate( SDL_bool cameraLimits ) {
	
	PT_CollisionHandler* _this = (PT_CollisionHandler*)malloc(sizeof(struct pt_collision_handler));
	if ( !_this )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_CollisionHandlerCreate: Not enough memory\n");
		return NULL;
	}
	SDL_memset(_this, 0, sizeof(struct pt_collision_handler));
	_this->cameraLimits = cameraLimits;

	return _this;
}//PT_CollisionHandlerCreate

void PT_CollisionHandlerDestroy( PT_CollisionHandler* _this ) {
	if ( !_this )
	{
		return;
	}
	
	PT_SpriteListDestroy(_this->list, SDL_FALSE);
	
	free(_this);
}//PT_CollisionHandlerDestroy

void PT_CollisionHandlerAdd( PT_CollisionHandler* _this, const char* utf8_group, PT_Sprite* sprite ) {
	if ( !_this )
	{
		SDL_Log("(*)PT: PT_CollisionHandlerAdd: Invalid pointer\n");
		return;
	}
	_this->list = PT_SpriteListCat(_this->list, utf8_group, sprite);
}//PT_CollisionHandlerAdd

void PT_CollisionHandlerUpdate( PT_CollisionHandler* _this ) {
	PT_SpriteList* pList = _this->list;
	while ( pList )
	{
		PT_CollisionHandlerCheckGroupCollision(pList->numValues, pList->values, _this->cameraLimits);
		pList = pList->next;
	}
}//PT_CollisionHandlerUpdate  

void PT_CollisionHandlerTestSpriteAgainstCollider( PT_Sprite* sprite, PT_Collider collider ) {

	if ( sprite->dstRect.x  <= PT_CameraGetX() + PT_CameraGetWidth() && 
		sprite->dstRect.x + sprite->dstRect.w >= PT_CameraGetX() &&
		sprite->dstRect.y <= PT_CameraGetY() + PT_CameraGetHeight() &&
		sprite->dstRect.y + sprite->dstRect.h >= PT_CameraGetY() )
	{
		for ( unsigned int i = 0; i < sprite->numColliders; i++ )
		{
			if ( PT_ColliderTestCollision(sprite->colliders[i], sprite->dstRect.x, sprite->dstRect.y,
				collider, PT_CameraGetX(), PT_CameraGetY()) )
			{
				PT_SpriteCollisionWith(sprite, sprite->colliders[i], collider);
			}
		}
	}
}//PT_CollisionHandlerTestSpriteToCollider

//===================================== PRIVATE Functions

void PT_CollisionHandlerCheckGroupCollision( unsigned int num, PT_Sprite** values, SDL_bool cameraLimits ) {
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
			
			//Only for sprites on the camera.
			if ( cameraLimits )
			{
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
						PT_CollisionHandlerCheckColliderCollision(values[i], values[j], 
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
			//Every sprite(This can uses a lot of CPU time). 
			else {	

				PT_CollisionHandlerCheckColliderCollision(values[i], values[j], 
					values[i]->numColliders, values[i]->colliders,
					values[j]->numColliders, values[j]->colliders);
			}
		}
	}
}//PT_CollisionHandlerCheckGroupCollision

void PT_CollisionHandlerCheckColliderCollision( PT_Sprite* sprite_1, PT_Sprite* sprite_2,
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
}//PT_CollisionHandlerCheckColliderCollision






