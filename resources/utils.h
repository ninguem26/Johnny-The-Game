#include "structs.h"

#ifndef UTILS_H
#define UTILS_H

int getScreenWidth();
int getScreenHeigth();

void createBullet(SPRITE *bullet, SPRITE *player, int *nBullets, ALLEGRO_DISPLAY *janela);
void moveToEnd(SPRITE *sprite, int index, int *length);

void initializeAllegro(ALLEGRO_DISPLAY **janela, ALLEGRO_EVENT_QUEUE **fila_eventos, ALLEGRO_TIMER **timer, ALLEGRO_FONT **font, int FPS);
void drawScreen(PLAYER *player, SPRITE *platform, SPRITE *enemy, SPRITE *bullet, ALLEGRO_FONT **font,
                int nBullets, int nEnemys, int nPlatforms, int curPlayerFrame);
void destroy(SPRITE *player, SPRITE *platform, SPRITE *enemy, SPRITE *bullet, ALLEGRO_DISPLAY **janela, ALLEGRO_EVENT_QUEUE **fila_eventos,
             ALLEGRO_FONT **font, int nBullets, int nEnemys, int nPlatforms);

#endif
