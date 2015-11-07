// Arquivos da Allegro 5
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

// Bibliotecas do C
#include <stdio.h>

// Arquivos do projeto
#include "utils.h"
#include "physics.h"
#include "player.h"

void initializePlayer(PLAYER *player){
    player->sprite.positionX = 10;
    player->sprite.positionY = 160;
    player->sprite.rotationY = 0;
    player->sprite.speedX = 3;
    player->sprite.speedY = 5;
    player->maxHealth = 10;
    player->actualHealth = player->maxHealth;
}

void damageCount(PLAYER *player, SPRITE *enemy, int nEnemys, bool *receiveDamage, bool *canMove){
    int i;
    for(i = 0; i < nEnemys; i++){
        if(collisionY(player->sprite, enemy[i], 0, 0) && collisionX(player->sprite, enemy[i]) && !*receiveDamage){
            *receiveDamage = true;
        }
    }

    if(*receiveDamage && *canMove){
        *canMove = false;
        player->actualHealth--;
    }
}
