// Arquivos da Allegro 5
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

// Bibliotecas do C
#include <stdio.h>

// Arquivos do projeto
#include "utils.h"
#include "physics.h"

bool collisionY(SPRITE collider1, SPRITE collider2, int base, int topo){
    if((collider2.positionY < collider1.positionY + al_get_bitmap_height(collider1.image[0]) - base) &&
       (collider2.positionY + al_get_bitmap_height(collider2.image[0]) > collider1.positionY + topo)){
         return true;
    }
    return false;
}

bool collisionRight(SPRITE collider1, SPRITE collider2){
    if((collider2.positionX + al_get_bitmap_width(collider2.image[0]) > collider1.positionX) &&
       (collider2.positionX + al_get_bitmap_width(collider2.image[0]) < collider1.positionX + al_get_bitmap_width(collider1.image[0]))){
         return true;
    }
    return false;
}

bool collisionLeft(SPRITE collider1, SPRITE collider2){
    if((collider2.positionX > collider1.positionX) &&
       (collider2.positionX < collider1.positionX + al_get_bitmap_width(collider1.image[0]))){
         return true;
    }
    return false;
}

bool collisionX(SPRITE collider1, SPRITE collider2){
    if((collider2.positionX < collider1.positionX  + al_get_bitmap_width(collider1.image[0])) &&
       (collider2.positionX + al_get_bitmap_width(collider2.image[0]) > collider1.positionX)){
         return true;
    }
    return false;
}

bool collisionTop(SPRITE collider1, SPRITE collider2){
    if((collider2.positionY > collider1.positionY) &&
       (collider2.positionY < collider1.positionY + al_get_bitmap_height(collider1.image[0]))){
         return true;
    }
    return false;
}

bool collisionDown(SPRITE collider1, SPRITE collider2){
    if((collider2.positionY + al_get_bitmap_height(collider2.image[0]) > collider1.positionY) &&
       (collider2.positionY + al_get_bitmap_height(collider2.image[0]) < collider1.positionY + al_get_bitmap_height(collider1.image[0]))){
         return true;
    }
    return false;
}

void bulletCollision(SPRITE *collider1, SPRITE *collider2, int *nBullets, int nPlatforms){
    int i = 0, j;
    while(i < *nBullets){
        for(j = 0; j < nPlatforms; j++){
            if(collisionY(collider1[i], collider2[j], 0, 0)){
                if(collisionLeft(collider1[i], collider2[j]) || collisionRight(collider1[i], collider2[j])){
                    al_destroy_bitmap(collider1[i].image[0]); //Destruindo bitmap do projétil que colidiu
                    moveToEnd(collider1, i, nBullets);        //Movendo projetil destruido para o fim do array
                }
            }
        }
        i++;
    }
}

void playerCollision(SPRITE *collider1, SPRITE *collider2, float *rightSpeed, float *leftSpeed, float *jumpSpeed,
                        bool *isGrounded, bool *isJumping, bool *isFalling, int nPlatforms){
    int i;
    for(i = 0; i < nPlatforms; i++){
        if(collisionY(*collider1, collider2[i], 10, 10)){
            if(collisionLeft(*collider1, collider2[i])){
                collider1->positionX = collider2[i].positionX - al_get_bitmap_width(collider1->image[0]);
                *rightSpeed = 0;
            }else{
                *rightSpeed = 3;
            }
            if(collisionRight(*collider1, collider2[i])){
                collider1->positionX = collider2[i].positionX + al_get_bitmap_width(collider2[i].image[0]);
                *leftSpeed = 0;
            }else{
                *leftSpeed = 3;
            }
        }else{
            *rightSpeed = 3;
            *leftSpeed = 3;
        }

        if(collisionX(*collider1, collider2[i])){
            if(collisionTop(*collider1, collider2[i])){
                collider1->positionY = collider2[i].positionY - al_get_bitmap_height(collider1->image[0]) - 1;
                *isGrounded = true;
                *isFalling = false;
                collider1->speedY = *jumpSpeed;
            }
            if(collisionDown(*collider1, collider2[i])){
                collider1->positionY = collider2[i].positionY + al_get_bitmap_height(collider2[i].image[0]) - 1;
                if(*isJumping){
                    collider1->speedY = 0;
                }
            }
        }else{
            if(collider1->positionY == collider2[i].positionY - al_get_bitmap_height(collider1->image[0]) - 1){
                *isGrounded = false;
                *isFalling = true;
            }
        }
    }
}

void enemyCollision(SPRITE *collider1, SPRITE *collider2, int nPlatforms, int nEnemys){
    int i, j;

    for(i = 0; i < nPlatforms; i++){
        for(j = 0; j < nEnemys; j++){
            if(collisionY(collider1[j], collider2[i], -32, -32)){
                if(collisionLeft(collider1[j], collider2[i])){
                    if(collider1[j].speedX < 0){
                        collider1[j].positionX = collider2[i].positionX;
                    }else{
                        collider1[j].positionX = collider2[i].positionX - al_get_bitmap_width(collider1[j].image[0]);
                    }
                    collider1[j].speedX *= -1;
                }
                if(collisionRight(collider1[j], collider2[i])){
                    if(collider1[j].speedX > 0){
                        collider1[j].positionX = collider2[i].positionX + al_get_bitmap_width(collider2[i].image[0]) - al_get_bitmap_width(collider1[j].image[0]);
                    }else{
                        collider1[j].positionX = collider2[i].positionX + al_get_bitmap_width(collider2[i].image[0]);
                    }
                    collider1[j].speedX *= -1;
                }
            }
        }
    }
}
