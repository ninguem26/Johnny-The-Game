#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <stdio.h>

#include "utils.h"

// Atributos da tela
const int LARGURA_TELA = 800;
const int ALTURA_TELA = 600;

int getScreenWidth(){
    return LARGURA_TELA;
}
int getScreenHeigth(){
    return ALTURA_TELA;
}

void createBullet(SPRITE *bullet, SPRITE *player, int *nBullets, ALLEGRO_DISPLAY *janela){
    //Definindo valores das variáveis do projétil
    if(player->rotationY == 0){
        bullet[*nBullets].speedX = 8;
    }else{
        bullet[*nBullets].speedX = -8;
    }
    bullet[*nBullets].positionX = player->positionX + (al_get_bitmap_width(player->image[0]) - al_get_bitmap_width(player->image[0])*player->rotationY);
    bullet[*nBullets].positionY = player->positionY + (al_get_bitmap_height(player->image[0])/2);
    bullet[*nBullets].rotationY = 0;

    //Definindo bitmap do projétil
    bullet[*nBullets].image[0] = al_create_bitmap(getScreenWidth()/100, getScreenHeigth()/100);
    al_set_target_bitmap(bullet[*nBullets].image[0]);
    al_clear_to_color(al_map_rgb(255, 255, 255));

    al_set_target_bitmap(al_get_backbuffer(janela));

    *nBullets = *nBullets+1;
}
