// Arquivos da Allegro 5
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

// Bibliotecas do C
#include <stdio.h>

// Arquivos do projeto
#include "physics.h"

bool collisionY(SPRITE collider1, SPRITE collider2, int i){
    if((collider2.positionY < collider1.positionY + al_get_bitmap_height(collider1.image[0]) - i) &&
       (collider2.positionY + al_get_bitmap_height(collider2.image[0]) > collider1.positionY + i)){
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

void bulletCollision(SPRITE *collider1, SPRITE *collider2, int *nBullets){
    int i = 0, j, k = 0;
    while(i < *nBullets){
        for(j = 0; j < 2; j++){
            if(collisionY(collider1[i], collider2[j], 0)){
                if(collisionLeft(collider1[i], collider2[j]) || collisionRight(collider1[i], collider2[j])){
                    al_destroy_bitmap(collider1[i].image[0]); //Destruindo bitmap do projétil que colidiu
                    k = i;
                    while(k < *nBullets){ //Movendo projétil que colidiu para o final do array
                        if(k < *nBullets-1){
                            collider1[k] = collider1[k+1];
                        }else{
                            *nBullets = *nBullets-1;
                        }
                        k++;
                    }
                }
            }
        }
        i++;
    }
}
