#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

#ifndef STRUCTS_H
#define STRUCTS_H
//Estrutura de um SPRITE, contendo posições e velocidades nos eixos X e Y, rotação em Y, altura e largura do SPRITE e um bitmap
typedef struct {
    float positionX, positionY, rotationY;
    float speedX, speedY;
    float width, height;
    ALLEGRO_BITMAP *image[4];
} SPRITE;

//Estrutura do PLAYER, contendo o valor da saúde do player e seu respectivo SPRITE
typedef struct {
    float actualHealth, maxHealth;
    SPRITE sprite;
} PLAYER;

#endif
