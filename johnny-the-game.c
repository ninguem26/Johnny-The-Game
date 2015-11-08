// Arquivos da Allegro 5
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

// Bibliotecas do C
#include <stdio.h>

// Arquivos do projeto
#include "resources/structs.h"
#include "resources/utils.h"
#include "resources/physics.h"
#include "resources/player.h"
#include "resources/enemy.h"

ALLEGRO_DISPLAY *janela = NULL;
ALLEGRO_EVENT_QUEUE *fila_eventos = NULL;
ALLEGRO_TIMER *timer = NULL;
ALLEGRO_FONT *font = NULL;

int main(void){
    char healthIndicator[100];

    bool sair = false;
    bool teclaDown[3] = {false, false, false};
    bool isGrounded = false, isFalling = false, isJumping = false, isShooting = false, receiveDamage = false, canMove = true, isWalking = false;
    bool redraw = true;
    bool freeMemory = false;

    int nBullets = 0, bulletsOut = 0;
    int nPlatforms = 9;
    int nEnemys = 2;

    int maxFrame = 4;
    int curPlayerFrame = 0;
    int initFrame = 0;
    int curMoveFrame = 0;
    int frameCount = 0;
    int frameDelay = 7;

    int i;
    int count = 0;
    int FPS = 60;

    float jumpSpeed = 5, rightSpeed = 3, leftSpeed = 3;

    PLAYER player;
    SPRITE *platform;
    SPRITE *enemy;
    SPRITE *bullet;

    initializePlayer(&player);

    bullet = (SPRITE *) malloc(sizeof(SPRITE)*1000);
    platform = (SPRITE *) malloc(sizeof(SPRITE)*nPlatforms);
    enemy = (SPRITE *) malloc(sizeof(SPRITE)*nEnemys);

    platform[0].positionX = 8*32;
    platform[0].positionY = 504;
    platform[0].rotationY = 0;

    platform[1].positionX = 7*32;
    platform[1].positionY = 536;
    platform[1].rotationY = 0;

    platform[2].positionX = 10*32;
    platform[2].positionY = 472;
    platform[2].rotationY = 0;

    platform[3].positionX = 200;
    platform[3].positionY = 250;
    platform[3].rotationY = 0;

    platform[4].positionX = 0;
    platform[4].positionY = 568;
    platform[4].rotationY = 0;

    platform[5].positionX = 0;
    platform[5].positionY = 537;
    platform[5].rotationY = 0;

    platform[6].positionX = 0;
    platform[6].positionY = 0;
    platform[6].rotationY = 0;

    platform[7].positionX = 0;
    platform[7].positionY = 0;
    platform[7].rotationY = 0;

    platform[8].positionX = 768;
    platform[8].positionY = 0;
    platform[8].rotationY = 0;

    enemy[0].positionX = 150;
    enemy[0].positionY = 536;
    enemy[0].rotationY = 0;
    enemy[0].speedX = -2;

    enemy[1].positionX = 420;
    enemy[1].positionY = 536;
    enemy[1].rotationY = 0;
    enemy[1].speedX = -2;

    if (!al_init()){
        fprintf(stderr, "Falha ao inicializar a Allegro.\n");
        return -1;
    }

    al_init_font_addon();

    if (!al_init_ttf_addon())
    {
        fprintf(stderr, "Falha ao inicializar add-on allegro_ttf.\n");
        return -1;
    }

    if (!al_init_image_addon()){
        fprintf(stderr, "Falha ao inicializar add-on allegro_image.\n");
        return -1;
    }

    janela = al_create_display(getScreenWidth(), getScreenHeigth());
    if (!janela){
        fprintf(stderr, "Falha ao criar janela.\n");
        return -1;
    }

    player.sprite.image[0] = al_load_bitmap("sprites/Johnny/johnny_frame-1 42x54.png");
    player.sprite.image[1] = al_load_bitmap("sprites/Johnny/johnny_frame-2 42x54.png");
    player.sprite.image[2] = al_load_bitmap("sprites/Johnny/johnny_frame-3 42x54.png");
    player.sprite.image[3] = al_load_bitmap("sprites/Johnny/johnny_frame-4 42x54.png");
    player.sprite.image[4] = al_load_bitmap("sprites/Johnny/johnny_walk_frame-1 42x54.png");
    player.sprite.image[5] = al_load_bitmap("sprites/Johnny/johnny_walk_frame-2 42x54.png");
    player.sprite.image[6] = al_load_bitmap("sprites/Johnny/johnny_walk_frame-3 42x54.png");
    player.sprite.image[7] = al_load_bitmap("sprites/Johnny/johnny_walk_frame-4 42x54.png");
    platform[0].image[0] = al_load_bitmap("sprites/tijolos 64x32.png");
    platform[1].image[0] = al_load_bitmap("sprites/tijolos 32x32.png");
    platform[2].image[0] = al_load_bitmap("sprites/tijolos 64x32.png");
    platform[3].image[0] = al_load_bitmap("sprites/tijolos 32x32.png");
    platform[4].image[0] = al_load_bitmap("sprites/tijolos 800x32.png");
    platform[5].image[0] = al_load_bitmap("sprites/tijolos 64x32.png");
    platform[6].image[0] = al_load_bitmap("sprites/tijolos 800x32.png");
    platform[7].image[0] = al_load_bitmap("sprites/tijolos 32x600.png");
    platform[8].image[0] = al_load_bitmap("sprites/tijolos 32x600.png");
    enemy[0].image[0] = al_load_bitmap("sprites/pedra 32x32.png");
    enemy[1].image[0] = al_load_bitmap("sprites/pedra 32x32.png");
    if (!player.sprite.image[0] || !platform[0].image[0] || !platform[1].image[0] || !enemy[0].image[0]){
        fprintf(stderr, "Falha ao carregar o arquivo de imagem.\n");
        al_destroy_display(janela);
        return -1;
    }

    if (!al_install_keyboard())
    {
        fprintf(stderr, "Falha ao inicializar o teclado.\n");
        return false;
    }

    timer = al_create_timer(1.0/FPS);
    fila_eventos = al_create_event_queue();
    if (!fila_eventos){
        fprintf(stderr, "Falha ao criar fila de eventos.\n");
        al_destroy_display(janela);
        return -1;
    }

    font = al_load_font("fonts/nobile.ttf", 18, 0);
    if (!font)
    {
        al_destroy_display(janela);
        fprintf(stderr, "Falha ao carregar fonte.\n");
        return -1;
    }

    al_register_event_source(fila_eventos, al_get_keyboard_event_source());
    al_register_event_source(fila_eventos, al_get_display_event_source(janela));
    al_register_event_source(fila_eventos, al_get_timer_event_source(timer));

    al_start_timer(timer);

    while (!sair){
        count++;
        while(!al_is_event_queue_empty(fila_eventos)){

            ALLEGRO_EVENT evento;
            al_wait_for_event(fila_eventos, &evento);

            if (evento.type == ALLEGRO_EVENT_KEY_DOWN){
                if(evento.keyboard.keycode == ALLEGRO_KEY_UP && isGrounded){
                    teclaDown[0] = true;
                }else if(evento.keyboard.keycode == ALLEGRO_KEY_LEFT){
                    player.sprite.speedX = leftSpeed;
                    teclaDown[1] = true;
                }else if(evento.keyboard.keycode == ALLEGRO_KEY_RIGHT){
                    player.sprite.speedX = rightSpeed;
                    teclaDown[2] = true;
                }else if(evento.keyboard.keycode == ALLEGRO_KEY_X && !isShooting){
                    isShooting = true;
                }
            }else if(evento.type == ALLEGRO_EVENT_KEY_UP){
                if(evento.keyboard.keycode == ALLEGRO_KEY_UP){
                    teclaDown[0] = false;
                    if(isJumping){
                        player.sprite.speedY = 0;
                    }
                }else if(evento.keyboard.keycode == ALLEGRO_KEY_LEFT){
                    teclaDown[1] = false;
                }else if(evento.keyboard.keycode == ALLEGRO_KEY_RIGHT){
                    teclaDown[2] = false;
                }
            }else if(evento.type == ALLEGRO_EVENT_TIMER){
                for(i = 0; i < nBullets; i++){
                    bullet[i].positionX += bullet[i].speedX;
                }

                for(i = 0; i < nEnemys; i++){
                    enemy[i].positionX += enemy[i].speedX;
                }

                if (player.sprite.positionY + al_get_bitmap_height(player.sprite.image[0]) > getScreenHeigth()){
                    isGrounded = true;
                    isFalling = false;
                    player.sprite.positionY = getScreenHeigth() - al_get_bitmap_height(player.sprite.image[0]);
                    player.sprite.speedY = jumpSpeed;
                }
                if(isJumping){
                    player.sprite.positionY -= player.sprite.speedY;
                    player.sprite.speedY-=0.2;
                    if(player.sprite.speedY <= 0){
                        isFalling = true;
                        isJumping = false;
                    }
                }
                if (isGrounded && teclaDown[0]){
                    isJumping = true;
                }else if(!isGrounded && !teclaDown[0]){
                    isFalling = true;
                }
                if (!isGrounded && isFalling){
                    player.sprite.positionY += player.sprite.speedY;
                    player.sprite.speedY+=0.2;
                }

                if(canMove){
                    if (teclaDown[1]){
                        player.sprite.positionX -= player.sprite.speedX;
                        player.sprite.rotationY = 1;
                        if(!isWalking){
                            initFrame = 4;
                            curPlayerFrame = 4;
                            maxFrame = 8;
                            isWalking = true;
                        }
                    }else if (teclaDown[2]){
                        player.sprite.positionX += player.sprite.speedX;
                        player.sprite.rotationY = 0;
                        if(!isWalking){
                            initFrame = 4;
                            curPlayerFrame = 4;
                            maxFrame = 8;
                            isWalking = true;
                        }
                    }else{
                        if(isWalking){
                            initFrame = 0;
                            curPlayerFrame = 0;
                            frameDelay = 7;
                            maxFrame = 4;
                            isWalking = false;
                        }
                    }
                }
                if(++frameCount >= frameDelay){
                    if(++curPlayerFrame >= maxFrame){
                        curPlayerFrame = initFrame;
                    }
                    if(!canMove){
                        if(++curMoveFrame >= 4){
                            curMoveFrame = 0;
                            canMove = true;
                            receiveDamage = false;
                        }
                    }
                    frameCount = 0;
                }

                if(isJumping){
                    isGrounded = false;
                    isFalling = false;
                }else if(isFalling){
                    isGrounded = false;
                    isJumping = false;
                }else if(isGrounded){
                    isJumping = false;
                    isFalling = false;
                }

                //Colisão do PLAYER com o cenário
                playerCollision(&player.sprite, platform, &rightSpeed, &leftSpeed, &jumpSpeed, &isGrounded, &isJumping, &isFalling, nPlatforms);

                //Colisão dos PROJETEIS com o cenário
                bulletCollision(bullet, platform, &nBullets, nPlatforms);

                //Colisão dos INIMIGOS com o cenário
                enemyCollision(enemy, platform, nPlatforms, nEnemys);

                //Destruindo inimigo após colisão com um projétil
                killEnemy(enemy, bullet, &nBullets, &nEnemys);

                damageCount(&player, enemy, nEnemys, &receiveDamage, &canMove);

                //Instanciando projétil
                if(isShooting){
                    createBullet(bullet, &player.sprite, &nBullets, janela);
                    isShooting = false;
                }

                for(i = 0; i < nBullets; i++){
                    if(bullet[i].positionX > getScreenWidth() || bullet[i].positionX < 0){
                        bulletsOut++;
                    }
                }

                if(nBullets > 0 && bulletsOut >= nBullets){
                    freeMemory = true;
                    bulletsOut = 0;
                }else{
                    bulletsOut = 0;
                }

                if(freeMemory || nBullets >= (sizeof(bullet)/sizeof(SPRITE))-1){
                    for(i = 0; i < nBullets; i++){
                        al_destroy_bitmap(bullet[i].image[0]);
                    }
                    nBullets = 0;
                    freeMemory = false;
                }

                if(player.actualHealth <= 0){
                    printf("Fim de jogo\n");
                }

                redraw = true;
            }else if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
                sair = true;
            }
        }

        if(redraw){
            redraw = false;

            for(i = 0; i < nBullets; i++){
                al_draw_bitmap(bullet[i].image[0], bullet[i].positionX, bullet[i].positionY, bullet[i].rotationY);
            }

            for(i = 0; i < nEnemys; i++){
                al_draw_bitmap(enemy[i].image[0], enemy[i].positionX, enemy[i].positionY, enemy[i].rotationY);
            }

            for(i = 0; i < nPlatforms; i++){
                al_draw_bitmap(platform[i].image[0], platform[i].positionX, platform[i].positionY, platform[i].rotationY);
            }
            al_draw_bitmap(player.sprite.image[curPlayerFrame], player.sprite.positionX, player.sprite.positionY, player.sprite.rotationY);

            sprintf(healthIndicator, "Health: %.0f", player.actualHealth);

            al_draw_text(font, al_map_rgb(255, 255, 255), 80, 30, ALLEGRO_ALIGN_CENTRE, healthIndicator);

            al_flip_display();
            al_clear_to_color(al_map_rgb(0, 0, 0));
        }
    }

    for(i = 0; i < maxFrame; i++){
        al_destroy_bitmap(player.sprite.image[i]);
    }
    for(i = 0; i < nBullets; i++){
        al_destroy_bitmap(bullet[i].image[0]);
    }
    for(i = 0; i < nPlatforms; i++){
        al_destroy_bitmap(platform[i].image[0]);
    }
    al_destroy_font(font);
    al_destroy_event_queue(fila_eventos);
    al_destroy_display(janela);

    return 0;
}
