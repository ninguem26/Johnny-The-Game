#include "structs.h"

#ifndef UTILS_H
#define UTILS_H

int getScreenWidth();
int getScreenHeigth();

void createBullet(SPRITE *bullet, SPRITE *player, int *nBullets, ALLEGRO_DISPLAY *janela);
void moveToEnd(SPRITE *sprite, int index, int *length);
void moveEnemyToEnd(ENEMY *enemy, int index, int *length);

void initializeAllegro(ALLEGRO_DISPLAY **janela, ALLEGRO_EVENT_QUEUE **fila_eventos, ALLEGRO_TIMER **timer, ALLEGRO_FONT **font,
                       ALLEGRO_FONT **fonte, ALLEGRO_SAMPLE **menuSong, ALLEGRO_SAMPLE **menuMusic, ALLEGRO_SAMPLE **gameMusic, int FPS);
void drawScreen(PLAYER *player, SPRITE *platform, ENEMY *enemy, SPRITE *bullet, ALLEGRO_FONT **font,
                int nBullets, int nEnemys, int nPlatforms, int curPlayerFrame, int curEnemyFrame, int actualLevel);
void destroy(SPRITE *player, SPRITE *platform, ENEMY *enemy, SPRITE *bullet, ALLEGRO_DISPLAY **janela, ALLEGRO_EVENT_QUEUE **fila_eventos,
             ALLEGRO_FONT **font, ALLEGRO_FONT **fonte, ALLEGRO_SAMPLE **menuSong, ALLEGRO_SAMPLE **menuMusic, ALLEGRO_SAMPLE **gameMusic,
             int nBullets, int nEnemys, int nPlatforms);

void initializeMenu(SPRITE *botao_start, SPRITE *botao_sair, SPRITE *area_central, ALLEGRO_DISPLAY **janela);
void drawMenu(SPRITE botao_start, SPRITE botao_sair, SPRITE area_central, ALLEGRO_FONT *font, ALLEGRO_FONT *fonte, int cabecalho, int startText);

bool interactButton(SPRITE button, int mouseX, int mouseY);
void drawCredits(int creditContent, ALLEGRO_FONT *font, ALLEGRO_FONT *fonte);
void setButtonCollor(ALLEGRO_BITMAP *button, int r, int g, int b, ALLEGRO_DISPLAY *janela);

#endif
