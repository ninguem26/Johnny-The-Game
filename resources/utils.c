#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

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

    al_set_window_title(*janela, "Johnny - The Game");

    if (!al_install_mouse()){
        fprintf(stderr, "Falha ao inicializar o mouse.\n");
        al_destroy_display(*janela);
    }

    if (!al_set_system_mouse_cursor(*janela, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT)){
        fprintf(stderr, "Falha ao atribuir ponteiro do mouse.\n");
        al_destroy_display(*janela);
    }

    if (!al_install_audio())
    {
        fprintf(stderr, "Falha ao inicializar áudio.\n");
    }

    if (!al_init_acodec_addon())
    {
        fprintf(stderr, "Falha ao inicializar codecs de áudio.\n");
    }

    if (!al_reserve_samples(3))
    {
        fprintf(stderr, "Falha ao alocar canais de áudio.\n");
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
    al_register_event_source(*fila_eventos, al_get_mouse_event_source());

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

void initializeMenu(SPRITE *botao_start, SPRITE *botao_sair, SPRITE *area_central, ALLEGRO_DISPLAY **janela){
    botao_start->image[0] = al_create_bitmap(200, 50);
    botao_start->positionX = getScreenWidth()/2 - al_get_bitmap_width(botao_start->image[0])/2;
    botao_start->positionY = 200;
    botao_sair->image[0] = al_create_bitmap(200, 50);
    botao_sair->positionX = getScreenWidth()/2 - al_get_bitmap_width(botao_sair->image[0])/2;
    botao_sair->positionY = 300;
    area_central->image[0] = al_create_bitmap(getScreenWidth()/2, getScreenHeigth()/2);
    area_central->positionX = getScreenWidth()/2 - al_get_bitmap_width(area_central->image[0])/2;
    area_central->positionY = getScreenHeigth()/2 - al_get_bitmap_height(area_central->image[0])/2;
    al_set_target_bitmap(area_central->image[0]);
    al_clear_to_color(al_map_rgb(68, 110, 127));

    al_set_target_bitmap(botao_start->image[0]);
    al_clear_to_color(al_map_rgb(47, 104, 127));

    al_set_target_bitmap(botao_sair->image[0]);
    al_clear_to_color(al_map_rgb(47, 104, 127));
    al_set_target_bitmap(al_get_backbuffer(*janela));
    if (!botao_start->image[0] || !botao_sair->image[0] || !area_central->image[0])
    {
        fprintf(stderr, "Falha ao criar botão de saída.\n");
        al_destroy_display(*janela);
    }
}

void drawMenu(SPRITE botao_start, SPRITE botao_sair, SPRITE area_central, ALLEGRO_FONT *font, ALLEGRO_FONT *fonte, int cabecalho, int startText){
    char mensagem1[100];
    char mensagem2[100];

    al_draw_bitmap(area_central.image[0], area_central.positionX, area_central.positionY, 0);
    al_draw_bitmap(botao_start.image[0], botao_start.positionX, botao_start.positionY, 0);
    al_draw_bitmap(botao_sair.image[0], botao_sair.positionX, botao_sair.positionY, 0);

    if(cabecalho == 1){
        strcpy(mensagem1, "Johnny - The Game");
    }else if(cabecalho == 2){
        strcpy(mensagem1, "Pause");
    }else{
        strcpy(mensagem1, "Game Over");
    }

    if(startText == 1){
        strcpy(mensagem2, "START");
    }else if(startText == 2){
        strcpy(mensagem2, "CONTINUE");
    }else{
        strcpy(mensagem2, "TRY AGAIN");
    }

    al_draw_text(font, al_map_rgb(255, 255, 255), botao_start.positionX + al_get_bitmap_width(botao_start.image[0])/2, botao_start.positionY + al_get_bitmap_height(botao_start.image[0])/4, ALLEGRO_ALIGN_CENTRE, mensagem2);
    al_draw_text(font, al_map_rgb(255, 255, 255), botao_sair.positionX + al_get_bitmap_width(botao_sair.image[0])/2, botao_sair.positionY + al_get_bitmap_height(botao_sair.image[0])/4, ALLEGRO_ALIGN_CENTRE, "QUIT");
    al_draw_text(fonte, al_map_rgb(255, 255, 255), getScreenWidth()/2, getScreenHeigth()/8, ALLEGRO_ALIGN_CENTRE, mensagem1);

    al_flip_display();
    al_clear_to_color(al_map_rgb(0, 0, 20));
}

bool interactButton(SPRITE button, int mouseX, int mouseY){
    if (mouseX >= button.positionX &&
        mouseX <= button.positionX + al_get_bitmap_width(button.image[0]) &&
        mouseY >= button.positionY &&
        mouseY <= button.positionY + al_get_bitmap_height(button.image[0])){
        return true;
    }
    return false;
}
