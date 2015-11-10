#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <stdio.h>

#include "enemy.h"
#include "utils.h"
#include "physics.h"

void killEnemy(ENEMY *enemy, SPRITE *bullet, int *nBullets, int *nEnemys){
    int i = 0, j;
    while(i < *nBullets){
        j = 0;
        while(j < *nEnemys){
            if(collisionY(bullet[i], enemy[j].sprite, 0, 0) && collisionX(bullet[i], enemy[j].sprite)){
                enemy[j].health--;
                al_destroy_bitmap(bullet[i].image[0]); //Destruindo bitmap do projétil que colidiu
                moveToEnd(bullet, i, nBullets);
                if(enemy[j].health <= 0){
                    al_destroy_bitmap(enemy[j].sprite.image[0]); //Destruindo bitmaps do inimigo
                    al_destroy_bitmap(enemy[j].sprite.image[1]);
                    moveEnemyToEnd(enemy, j, nEnemys);
                }
            }
            j++;
        }
        i++;
    }
}

void initializeEnemy(ENEMY *enemy, int **nEnemys, ALLEGRO_DISPLAY ***janela, int type1, int type2, int type3){
    int i, j = 0, totalEnemys = type1 + type2 + type3;

    for(i = 0; i < type1; i++){
        enemy[i].sprite.image[0] = al_load_bitmap("sprites/Enemys/enemy_type_1_frame_1 32x32.png");
        enemy[i].sprite.image[1] = al_load_bitmap("sprites/Enemys/enemy_type_1_frame_2 32x32.png");
        j++;
    }
    for(i = j; i < type1 + type2; i++){
        enemy[i].sprite.image[0] = al_load_bitmap("sprites/Enemys/enemy_type_2_frame_1 32x32.png");
        enemy[i].sprite.image[1] = al_load_bitmap("sprites/Enemys/enemy_type_2_frame_2 32x32.png");
        j++;
    }
    for(i = j; i < totalEnemys; i++){
        enemy[i].sprite.image[0] = al_load_bitmap("sprites/Enemys/enemy_type_3_frame_1 32x32.png");
        enemy[i].sprite.image[1] = al_load_bitmap("sprites/Enemys/enemy_type_3_frame_2 32x32.png");
    }
    for(i = j; i < totalEnemys; i++){
        if (!enemy[i].sprite.image[0] || !enemy[i].sprite.image[1]){
            fprintf(stderr, "Falha ao carregar o arquivo de imagem.\n");
            al_destroy_display(**janela);
        }
    }
    **nEnemys = totalEnemys;
}
