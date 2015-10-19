// Arquivos da Allegro 5
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

// Bibliotecas do C
#include <stdio.h>

// Arquivos do projeto
#include "physics.h"

bool collisionY(SPRITE collider1, SPRITE collider2){
    if((collider2.positionY < collider1.positionY + al_get_bitmap_height(collider1.image[0]) - 10) &&
       (collider2.positionY + al_get_bitmap_height(collider2.image[0]) > collider1.positionY) + 10){
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
