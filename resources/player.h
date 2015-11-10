#include "structs.h"

#ifndef PLAYER_H
#define PLAYER_H

void initializePlayer(PLAYER *player);
void damageCount(PLAYER *player, ENEMY *enemy, int nEnemys, bool *receiveDamage, bool *canMove, int *curPlayerFrame);

#endif
