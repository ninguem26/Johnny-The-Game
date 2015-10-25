// Arquivos da Allegro 5
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

// Bibliotecas do C
#include <stdio.h>

// Arquivos do projeto
#include "player.h"

void initializePlayer(SPRITE *player){
    player->positionX = 10;
    player->positionY = 160;
    player->rotationY = 0;
    player->speedX = 3;
    player->speedY = 5;
}
