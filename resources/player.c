// Arquivos da Allegro 5
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

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

    player->sprite.image[0] = al_load_bitmap("sprites/Johnny/johnny_frame-1 42x54.png");
    player->sprite.image[1] = al_load_bitmap("sprites/Johnny/johnny_frame-2 42x54.png");
    player->sprite.image[2] = al_load_bitmap("sprites/Johnny/johnny_frame-3 42x54.png");
    player->sprite.image[3] = al_load_bitmap("sprites/Johnny/johnny_frame-4 42x54.png");
    player->sprite.image[4] = al_load_bitmap("sprites/Johnny/johnny_walk_frame-1 42x54.png");
    player->sprite.image[5] = al_load_bitmap("sprites/Johnny/johnny_walk_frame-2 42x54.png");
    player->sprite.image[6] = al_load_bitmap("sprites/Johnny/johnny_walk_frame-3 42x54.png");
    player->sprite.image[7] = al_load_bitmap("sprites/Johnny/johnny_walk_frame-4 42x54.png");
    player->sprite.image[8] = al_load_bitmap("sprites/Johnny/johnny_jump 42x54.png");
    player->sprite.image[9] = al_load_bitmap("sprites/Johnny/johnny_fall 42x54.png");
}

void damageCount(PLAYER *player, SPRITE *enemy, int nEnemys, bool *receiveDamage, bool *canMove, int *curPlayerFrame){
    int i;
    for(i = 0; i < nEnemys; i++){
        if(collisionY(player->sprite, enemy[i], 0, 0) && collisionX(player->sprite, enemy[i]) && !*receiveDamage){
            *receiveDamage = true;
        }
    }

    if(*receiveDamage && *canMove){
        *curPlayerFrame = 9;
        *canMove = false;
        player->actualHealth--;
    }
}
