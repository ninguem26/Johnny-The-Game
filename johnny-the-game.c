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

ALLEGRO_DISPLAY *janela = NULL;
ALLEGRO_EVENT_QUEUE *fila_eventos = NULL;
ALLEGRO_TIMER *timer = NULL;
ALLEGRO_FONT *font = NULL;

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
    SPRITE enemy[5];

    initializePlayer(&player);

    platform[0].positionX = 200;
    platform[0].positionY = 540;
    platform[0].rotationY = 0;

    platform[1].positionX = 400;
    platform[1].positionY = 500;
    platform[1].rotationY = 0;

    enemy[0].positionX = 400;
    enemy[0].positionY = 560;
    enemy[0].rotationY = 0;
    enemy[0].speedX = 2;

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

    player.image[0] = al_load_bitmap("sprites/Johnny/johnny_frame-1 42x54.png");
    player.image[1] = al_load_bitmap("sprites/Johnny/johnny_frame-2 42x54.png");
    player.image[2] = al_load_bitmap("sprites/Johnny/johnny_frame-3 42x54.png");
    player.image[3] = al_load_bitmap("sprites/Johnny/johnny_frame-4 42x54.png");
    platform[0].image[0] = al_load_bitmap("sprites/tijolos 32x32.png");
    platform[1].image[0] = al_load_bitmap("sprites/tijolos 32x32.png");
    enemy[0].image[0] = al_load_bitmap("sprites/pedra 32x32.png");
    if (!player.image[0] || !platform[0].image[0] || !platform[1].image[0] || !enemy[0].image[0]){
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

                enemy[0].positionX += -enemy[0].speedX;

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
        playerCollision(&player, platform, &rightSpeed, &leftSpeed, &jumpSpeed, &isGrounded, &isJumping, &isFalling);

        //Colisão dos PROJETEIS com o cenário
        bulletCollision(bullet, platform, &nBullets);

        for(i = 0; i < 2; i++){
            if(collisionY(enemy[0], platform[i], 0)){
                if(collisionLeft(enemy[0], platform[i])){
                    enemy[0].positionX = platform[i].positionX - al_get_bitmap_width(enemy[0].image[0]);
                    enemy[0].speedX *= -1;
                }
                if(collisionRight(enemy[0], platform[i])){
                    enemy[0].positionX = platform[i].positionX + al_get_bitmap_width(platform[i].image[0]);
                    enemy[0].speedX *= -1;
                }
            }
        }

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

            al_draw_bitmap(enemy[0].image[0], enemy[0].positionX, enemy[0].positionY, enemy[0].rotationY);
            al_draw_bitmap(platform[0].image[0], platform[0].positionX, platform[0].positionY, platform[0].rotationY);
            al_draw_bitmap(platform[1].image[0], platform[1].positionX, platform[1].positionY, platform[1].rotationY);
            al_draw_bitmap(player.image[curFrame], player.positionX, player.positionY, player.rotationY);

            al_draw_text(font, al_map_rgb(255, 255, 255), 80, 30, ALLEGRO_ALIGN_CENTRE, "Vidas: ???");

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
    al_destroy_font(font);
    al_destroy_event_queue(fila_eventos);
    al_destroy_display(janela);

    return 0;
}
