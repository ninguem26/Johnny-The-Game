#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <stdio.h>

#include "enemy.h"
#include "utils.h"
#include "physics.h"
void killEnemy(SPRITE *enemy, SPRITE *bullet, int *nBullets, int *nEnemys){
    int i = 0, j;
    while(i < *nBullets){
        j = 0;
        while(j < *nEnemys){
            if(collisionY(bullet[i], enemy[j], 0, 0) && collisionX(bullet[i], enemy[j])){
                al_destroy_bitmap(bullet[i].image[0]); //Destruindo bitmap do projétil que colidiu
                al_destroy_bitmap(enemy[j].image[0]);
                moveToEnd(bullet, i, nBullets);
                moveToEnd(enemy, j, nEnemys);
            }
            j++;
        }
        i++;
    }
}
