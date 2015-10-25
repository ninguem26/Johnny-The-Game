#include "structs.h"

#ifndef PHYSICS_H
#define PHYSICS_H

bool collisionX(SPRITE collider1, SPRITE collider2);
bool collisionY(SPRITE collider1, SPRITE collider2, int i);
bool collisionRight(SPRITE collider1, SPRITE collider2);
bool collisionLeft(SPRITE collider1, SPRITE collider2);
bool collisionTop(SPRITE collider1, SPRITE collider2);

#endif
