#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

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
    bullet[*nBullets].image[0] = al_create_bitmap(getScreenWidth()/50, getScreenHeigth()/100);
    al_set_target_bitmap(bullet[*nBullets].image[0]);
    al_clear_to_color(al_map_rgb(255, 255, 255));

    al_set_target_bitmap(al_get_backbuffer(janela));

    *nBullets = *nBullets+1;
}

//Move para o fim do array um elemento de indice indicado pelo parâmetro 'index' no array '*sprite'
void moveToEnd(SPRITE *sprite, int index, int *length){
    while(index < *length){
        if(index < *length-1){
            sprite[index] = sprite[index+1];
        }else{
            *length = *length-1;
        }
        index++;
    }
}

//Move para o fim do array um elemento de indice indicado pelo parâmetro 'index' no array '*sprite'
void moveEnemyToEnd(ENEMY *enemy, int index, int *length){
    while(index < *length){
        if(index < *length-1){
            enemy[index] = enemy[index+1];
        }else{
            *length = *length-1;
        }
        index++;
    }
}

void initializeAllegro(ALLEGRO_DISPLAY **janela, ALLEGRO_EVENT_QUEUE **fila_eventos, ALLEGRO_TIMER **timer, ALLEGRO_FONT **font, int FPS){
    if (!al_init()){
        fprintf(stderr, "Falha ao inicializar a Allegro.\n");
    }

    al_init_font_addon();

    if (!al_init_ttf_addon())
    {
        fprintf(stderr, "Falha ao inicializar add-on allegro_ttf.\n");
    }

    if (!al_init_image_addon()){
        fprintf(stderr, "Falha ao inicializar add-on allegro_image.\n");
    }

    *janela = al_create_display(getScreenWidth(), getScreenHeigth());
    if (!*janela){
        fprintf(stderr, "Falha ao criar janela.\n");
    }

    if (!al_install_keyboard())
    {
        fprintf(stderr, "Falha ao inicializar o teclado.\n");
    }

    *timer = al_create_timer(1.0/FPS);
    *fila_eventos = al_create_event_queue();
    if (!*fila_eventos){
        fprintf(stderr, "Falha ao criar fila de eventos.\n");
        al_destroy_display(*janela);
    }

    *font = al_load_font("fonts/nobile.ttf", 18, 0);
    if (!*font)
    {
        al_destroy_display(*janela);
        fprintf(stderr, "Falha ao carregar fonte.\n");
    }

    al_register_event_source(*fila_eventos, al_get_keyboard_event_source());
    al_register_event_source(*fila_eventos, al_get_display_event_source(*janela));
    al_register_event_source(*fila_eventos, al_get_timer_event_source(*timer));

    al_start_timer(*timer);
}

void destroy(SPRITE *player, SPRITE *platform, ENEMY *enemy, SPRITE *bullet, ALLEGRO_DISPLAY **janela, ALLEGRO_EVENT_QUEUE **fila_eventos,
             ALLEGRO_FONT **font, int nBullets, int nEnemys, int nPlatforms){
    int i;
    for(i = 0; i < 10; i++){
        al_destroy_bitmap(player->image[i]);
    }
    for(i = 0; i < nBullets; i++){
        al_destroy_bitmap(bullet[i].image[0]);
    }
    for(i = 0; i < nPlatforms; i++){
        al_destroy_bitmap(platform[i].image[0]);
    }
    for(i = 0; i < nEnemys; i++){
        al_destroy_bitmap(enemy[i].sprite.image[0]);
        al_destroy_bitmap(enemy[i].sprite.image[1]);
    }
    al_destroy_font(*font);
    al_destroy_event_queue(*fila_eventos);
    al_destroy_display(*janela);
}

void drawScreen(PLAYER *player, SPRITE *platform, ENEMY *enemy, SPRITE *bullet, ALLEGRO_FONT **font,
                int nBullets, int nEnemys, int nPlatforms, int curPlayerFrame, int curEnemyFrame){
    int i;
    char healthIndicator[100];

    for(i = 0; i < nBullets; i++){
        al_draw_bitmap(bullet[i].image[0], bullet[i].positionX, bullet[i].positionY, bullet[i].rotationY);
    }

    for(i = 0; i < nEnemys; i++){
        al_draw_bitmap(enemy[i].sprite.image[curEnemyFrame], enemy[i].sprite.positionX, enemy[i].sprite.positionY, enemy[i].sprite.rotationY);
    }

    for(i = 0; i < nPlatforms; i++){
        al_draw_bitmap(platform[i].image[0], platform[i].positionX, platform[i].positionY, platform[i].rotationY);
    }
    al_draw_bitmap(player->sprite.image[curPlayerFrame], player->sprite.positionX, player->sprite.positionY, player->sprite.rotationY);

    sprintf(healthIndicator, "Health: %.0f", player->actualHealth);

    al_draw_text(*font, al_map_rgb(255, 255, 255), 80, 30, ALLEGRO_ALIGN_CENTRE, healthIndicator);

    al_flip_display();
    al_clear_to_color(al_map_rgb(0, 0, 20));
}
