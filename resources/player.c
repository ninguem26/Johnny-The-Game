// Arquivos da Allegro 5
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

// Bibliotecas do C
#include <stdio.h>

// Arquivos do projeto
#include "player.h"

void initializePlayer(PLAYER *player){
    player->sprite.positionX = 10;
    player->sprite.positionY = 160;
    player->sprite.rotationY = 0;
    player->sprite.speedX = 3;
    player->sprite.speedY = 5;
    player->maxHealth = 3;
    player->actualHealth = player->maxHealth;
}
