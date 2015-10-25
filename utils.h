#include "structs.h"

#ifndef UTILS_H
#define UTILS_H

int getScreenWidth();
int getScreenHeigth();

void createBullet(SPRITE *bullet, SPRITE *player, int *nBullets, ALLEGRO_DISPLAY *janela);

#endif
