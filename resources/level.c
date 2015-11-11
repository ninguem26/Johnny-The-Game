#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#include <stdio.h>

#include "utils.h"
#include "level.h"
#include "platform.h"
#include "enemy.h"

void loadLevel(int levelNumber, SPRITE *platform, ENEMY *enemy, int *nPlatforms, int *nEnemys, ALLEGRO_DISPLAY **janela){
    if(levelNumber == 1){
        initializePlatform(platform, &nPlatforms, &janela, 0, 0, 1, 1);
        initializeEnemy(enemy, &nEnemys, &janela, 0, 0, 0);

        //Definindo plataformas
        platform[0].positionX = 0*32;
        platform[0].positionY = 0*32;
        platform[0].rotationY = 0;

        platform[1].positionX = 0*32;
        platform[1].positionY = 568;
        platform[1].rotationY = 0;
    }else if(levelNumber == 2){
        initializePlatform(platform, &nPlatforms, &janela, 2, 2, 0, 1);
        initializeEnemy(enemy, &nEnemys, &janela, 2, 0, 0);

        //Definindo plataformas
        platform[0].positionX = 5*32;
        platform[0].positionY = 537;
        platform[0].rotationY = 0;

        platform[1].positionX = 12*32;
        platform[1].positionY = 536;
        platform[1].rotationY = 0;

        platform[2].positionX = 13*32;
        platform[2].positionY = 503;
        platform[2].rotationY = 0;

        platform[3].positionX = 15*32;
        platform[3].positionY = 535;
        platform[3].rotationY = 0;

        platform[4].positionX = 0;
        platform[4].positionY = 568;
        platform[4].rotationY = 0;

        //Definindo inimigos
        enemy[0].sprite.positionX = 8*32;
        enemy[0].sprite.positionY = 536;
        enemy[0].sprite.rotationY = 0;
        enemy[0].sprite.speedX = -2;
        enemy[0].health = 1;
        enemy[0].damage = 1;

        enemy[1].sprite.positionX = 18*32;
        enemy[1].sprite.positionY = 536;
        enemy[1].sprite.rotationY = 0;
        enemy[1].sprite.speedX = -2;
        enemy[1].health = 1;
        enemy[1].damage = 1;
    }else if(levelNumber == 3){
        initializePlatform(platform, &nPlatforms, &janela, 0, 3, 2, 4);
        initializeEnemy(enemy, &nEnemys, &janela, 4, 2, 0);

        //Definindo plataformas
        platform[0].positionX = 1*32;
        platform[0].positionY = 11*32;
        platform[0].rotationY = 0;

        platform[1].positionX = 1*32;
        platform[1].positionY = 7*32;
        platform[1].rotationY = 0;

        platform[2].positionX = 5*32;
        platform[2].positionY = 9*32;
        platform[2].rotationY = 0;

        platform[3].positionX = 0*32;
        platform[3].positionY = -5*32;
        platform[3].rotationY = 0;

        platform[4].positionX = 24*32;
        platform[4].positionY = -9*32;
        platform[4].rotationY = 0;

        platform[5].positionX = 0*32;
        platform[5].positionY = 568;
        platform[5].rotationY = 0;

        platform[6].positionX = 0*32;
        platform[6].positionY = 13*32;
        platform[6].rotationY = 0;

        platform[7].positionX = -5*32;
        platform[7].positionY = 0;
        platform[7].rotationY = 0;

        platform[8].positionX = 5*32;
        platform[8].positionY = 5*32;
        platform[8].rotationY = 0;

        //Definindo inimigos
        enemy[0].sprite.positionX = 7*32;
        enemy[0].sprite.positionY = 536;
        enemy[0].sprite.rotationY = 0;
        enemy[0].sprite.speedX = -2;
        enemy[0].health = 1;
        enemy[0].damage = 1;

        enemy[1].sprite.positionX = 14*32;
        enemy[1].sprite.positionY = 536;
        enemy[1].sprite.rotationY = 0;
        enemy[1].sprite.speedX = -2;
        enemy[1].health = 1;
        enemy[1].damage = 1;

        enemy[2].sprite.positionX = 14*32;
        enemy[2].sprite.positionY = 14*32;
        enemy[2].sprite.rotationY = 20;
        enemy[2].sprite.speedX = -2;
        enemy[2].health = 1;
        enemy[2].damage = 1;

        enemy[3].sprite.positionX = 1*32;
        enemy[3].sprite.positionY = 8*32;
        enemy[3].sprite.rotationY = 20;
        enemy[3].sprite.speedX = -2;
        enemy[3].health = 1;
        enemy[3].damage = 1;

        enemy[4].sprite.positionX = 14*32;
        enemy[4].sprite.positionY = 12*32;
        enemy[4].sprite.rotationY = 0;
        enemy[4].sprite.speedX = -6;
        enemy[4].health = 4;
        enemy[4].damage = 2;

        enemy[5].sprite.positionX = 14*32;
        enemy[5].sprite.positionY = 4*32;
        enemy[5].sprite.rotationY = 0;
        enemy[5].sprite.speedX = -6;
        enemy[5].health = 4;
        enemy[5].damage = 2;
    }else if(levelNumber == 4){
        initializePlatform(platform, &nPlatforms, &janela, 0, 2, 1, 2);
        initializeEnemy(enemy, &nEnemys, &janela, 3, 1, 0);

        //Definindo plataformas
        platform[0].positionX = 704;
        platform[0].positionY = 16*32;
        platform[0].rotationY = 0;

        platform[1].positionX = 18*32;
        platform[1].positionY = 15*32;
        platform[1].rotationY = 0;

        platform[2].positionX = 768;
        platform[2].positionY = 0;
        platform[2].rotationY = 0;

        platform[3].positionX = 0*32;
        platform[3].positionY = 568;
        platform[3].rotationY = 0;

        platform[4].positionX = -6*32;
        platform[4].positionY = 13*32;
        platform[4].rotationY = 0;

        //Definindo inimigos
        enemy[0].sprite.positionX = 7*32;
        enemy[0].sprite.positionY = 536;
        enemy[0].sprite.rotationY = 0;
        enemy[0].sprite.speedX = -2;
        enemy[0].health = 1;
        enemy[0].damage = 1;

        enemy[1].sprite.positionX = 14*32;
        enemy[1].sprite.positionY = 536;
        enemy[1].sprite.rotationY = 0;
        enemy[1].sprite.speedX = -2;
        enemy[1].health = 1;
        enemy[1].damage = 1;

        enemy[2].sprite.positionX = 14*32;
        enemy[2].sprite.positionY = 14*32;
        enemy[2].sprite.rotationY = 20;
        enemy[2].sprite.speedX = -2;
        enemy[2].health = 1;
        enemy[2].damage = 1;

        enemy[3].sprite.positionX = 14*32;
        enemy[3].sprite.positionY = 12*32;
        enemy[3].sprite.rotationY = 20;
        enemy[3].sprite.speedX = -6;
        enemy[3].health = 4;
        enemy[3].damage = 2;
    }
}
