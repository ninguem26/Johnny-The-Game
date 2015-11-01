#include "structs.h"

#ifndef PHYSICS_H
#define PHYSICS_H

bool collisionX(SPRITE collider1, SPRITE collider2);
bool collisionY(SPRITE collider1, SPRITE collider2, int i);
bool collisionRight(SPRITE collider1, SPRITE collider2);
bool collisionLeft(SPRITE collider1, SPRITE collider2);
bool collisionTop(SPRITE collider1, SPRITE collider2);
bool collisionDown(SPRITE collider1, SPRITE collider2);

void bulletCollision(SPRITE *collider1, SPRITE *collider2, int *nBullets, int nPlatforms);
void playerCollision(SPRITE *collider1, SPRITE *collider2, float *rightSpeed, float *leftSpeed, float *jumpSpeed,
                        bool *isGrounded, bool *isJumping, bool *isFalling, int nPlatforms);
void enemyCollision(SPRITE *collider1, SPRITE *collider2, int nPlatforms, int nEnemys);

#endif
