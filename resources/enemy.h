#include "structs.h"

#ifndef ENEMY_H
#define ENEMY_H

void killEnemy(ENEMY *enemy, SPRITE *bullet, int *nBullets, int *nEnemys);
void initializeEnemy(ENEMY *enemy, int **nEnemys, ALLEGRO_DISPLAY ***janela, int type1, int type2, int type3);

#endif
