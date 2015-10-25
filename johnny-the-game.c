// Arquivos da Allegro 5
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

// Bibliotecas do C
#include <stdio.h>

// Arquivos do projeto
#include "structs.h"
#include "utils.h"
#include "physics.h"
#include "player.h"

ALLEGRO_DISPLAY *janela = NULL;
ALLEGRO_EVENT_QUEUE *fila_eventos = NULL;
ALLEGRO_TIMER *timer = NULL;

int main(void){
    bool sair = false;
    bool teclaDown[3] = {false, false, false};
    bool isGrounded = false, isFalling = false, isJumping = false, isShooting = false;
    bool redraw = true;
    bool freeMemory = false;

    int nBullets = 0, bulletsOut = 0;

    const int maxFrame = 4;
    int curFrame = 0;
    int frameCount = 0;
    int frameDelay = 7;

    int i;
    int count = 0;
    int FPS = 60;

    float jumpSpeed = 5, rightSpeed = 3, leftSpeed = 3;

    SPRITE player;
    SPRITE platform[5];
    SPRITE bullet[1000];

    initializePlayer(&player);

    platform[0].positionX = 200;
    platform[0].positionY = 540;
    platform[0].rotationY = 0;

    platform[1].positionX = 400;
    platform[1].positionY = 500;
    platform[1].rotationY = 0;

    if (!al_init()){
        fprintf(stderr, "Falha ao inicializar a Allegro.\n");
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

    player.image[0] = al_load_bitmap("sprites/Johnny/johnny_frame-1 42x54.png");
    player.image[1] = al_load_bitmap("sprites/Johnny/johnny_frame-2 42x54.png");
    player.image[2] = al_load_bitmap("sprites/Johnny/johnny_frame-3 42x54.png");
    player.image[3] = al_load_bitmap("sprites/Johnny/johnny_frame-4 42x54.png");
    platform[0].image[0] = al_load_bitmap("sprites/tijolos 32x32.png");
    platform[1].image[0] = al_load_bitmap("sprites/tijolos 32x32.png");
    if (!player.image[0] || !platform[0].image[0] || !platform[1].image[0]){
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
                if(evento.keyboard.keycode == ALLEGRO_KEY_UP){
                    teclaDown[0] = true;
                }else if(evento.keyboard.keycode == ALLEGRO_KEY_LEFT){
                    player.speedX = leftSpeed;
                    teclaDown[1] = true;
                }else if(evento.keyboard.keycode == ALLEGRO_KEY_RIGHT){
                    player.speedX = rightSpeed;
                    teclaDown[2] = true;
                }else if(evento.keyboard.keycode == ALLEGRO_KEY_X && !isShooting){
                    isShooting = true;
                    printf("%d\n", nBullets);
                }
            }else if(evento.type == ALLEGRO_EVENT_KEY_UP){
                if(evento.keyboard.keycode == ALLEGRO_KEY_UP){
                    teclaDown[0] = false;
                    if(isJumping){
                        player.speedY = 0;
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
                if (player.positionY + al_get_bitmap_height(player.image[0]) > getScreenHeigth()){
                    isGrounded = true;
                    isFalling = false;
                    player.positionY = getScreenHeigth() - al_get_bitmap_height(player.image[0]);
                    player.speedY = jumpSpeed;
                }
                if(isJumping){
                    player.positionY -= player.speedY;
                    player.speedY-=0.2;
                    if(player.speedY <= 0){
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
                    player.positionY += player.speedY;
                    player.speedY+=0.2;
                }
                if (teclaDown[1]){
                    player.positionX -= player.speedX;
                    player.rotationY = 1;
                }
                if (teclaDown[2]){
                    player.positionX += player.speedX;
                    player.rotationY = 0;
                }
                if(++frameCount >= frameDelay){
                    if(++curFrame >= maxFrame){
                        curFrame = 0;
                    }
                    frameCount = 0;
                }

                redraw = true;
            }else if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
                sair = true;
            }
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
        for(i = 0; i < 2; i++){
            if(collisionY(player, platform[i], 10)){
                if(collisionLeft(player, platform[i])){
                    player.positionX = platform[i].positionX - al_get_bitmap_width(player.image[0]);
                    rightSpeed = 0;
                }else{
                    rightSpeed = 3;
                }
                if(collisionRight(player, platform[i])){
                    player.positionX = platform[i].positionX + al_get_bitmap_width(platform[i].image[0]);
                    leftSpeed = 0;
                }else{
                    leftSpeed = 3;
                }
            }else{
                rightSpeed = 3;
                leftSpeed = 3;
            }
            if(collisionX(player, platform[i])){
                if(collisionTop(player, platform[i])){
                    player.positionY = platform[i].positionY - al_get_bitmap_height(player.image[0]) - 1;
                    isGrounded = true;
                    isFalling = false;
                    player.speedY = jumpSpeed;
                }
                if(collisionDown(player, platform[i])){
                    player.positionY = platform[i].positionY + al_get_bitmap_height(platform[i].image[0]) - 1;
                    if(isJumping){
                        player.speedY = 0;
                    }
                }
            }else{
                if(player.positionY == platform[i].positionY - al_get_bitmap_height(player.image[0]) - 1){
                    isGrounded = false;
                    isFalling = true;
                }
            }
        }

        //Colisão dos PROJETEIS com o cenário
        bulletCollision(bullet, platform, &nBullets);

        //Instanciando projétil
        if(isShooting){
            createBullet(bullet, &player, &nBullets, janela);
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

        if(redraw){
            redraw = false;

            for(i = 0; i < nBullets; i++){
                al_draw_bitmap(bullet[i].image[0], bullet[i].positionX, bullet[i].positionY, bullet[i].rotationY);
            }

            al_draw_bitmap(platform[0].image[0], platform[0].positionX, platform[0].positionY, platform[0].rotationY);
            al_draw_bitmap(platform[1].image[0], platform[1].positionX, platform[1].positionY, platform[1].rotationY);
            al_draw_bitmap(player.image[curFrame], player.positionX, player.positionY, player.rotationY);
            al_flip_display();
            al_clear_to_color(al_map_rgb(0, 0, 0));
        }
    }

    for(i = 0; i < maxFrame; i++){
        al_destroy_bitmap(player.image[i]);
    }
    for(i = 0; i < nBullets; i++){
        al_destroy_bitmap(bullet[i].image[0]);
    }
    al_destroy_bitmap(platform[0].image[0]);
    al_destroy_bitmap(platform[1].image[0]);
    al_destroy_event_queue(fila_eventos);
    al_destroy_display(janela);

    return 0;
}
