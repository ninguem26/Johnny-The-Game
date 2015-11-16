#include "structs.h"

#ifndef LEVEL_H
#define LEVEL_H

void loadLevel(int levelNumber, SPRITE *platform, ENEMY *enemy, SPRITE *bullet, int *nPlatforms, int *nEnemys, int *nBullets,
               ALLEGRO_DISPLAY **janela);

#endif
