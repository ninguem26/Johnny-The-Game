// Os arquivos de cabeçalho
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

// Para utilizarmos o fprintf
#include <stdio.h>

// Atributos da tela
const int LARGURA_TELA = 640;
const int ALTURA_TELA = 480;

ALLEGRO_DISPLAY *janela = NULL;
ALLEGRO_EVENT_QUEUE *fila_eventos = NULL;
ALLEGRO_TIMER *timer = NULL;

struct SPRITE {
    float positionX, positionY, rotationY;
    float speedX, speedY;
    float width, height;
    ALLEGRO_BITMAP *image;
};

int main(void){
    bool sair = false;
    bool teclaDown[3] = {false, false, false};
    bool isGrounded = false, isFalling = false, isJumping = false;
    bool redraw = true;
    int i;
    int count = 0;
    int FPS = 60;

    float jumpSpeed = 5, rightSpeed = 3, leftSpeed = 3;

    struct SPRITE player;
    struct SPRITE sapo[5];

    player.positionX = 10;
    player.positionY = 160;
    player.rotationY = 0;
    player.speedX = 3;
    player.speedY = jumpSpeed;

    sapo[0].positionX = 200;
    sapo[0].positionY = 440;
    sapo[0].rotationY = 0;

    sapo[1].positionX = 400;
    sapo[1].positionY = 400;
    sapo[1].rotationY = 0;

    if (!al_init()){
        fprintf(stderr, "Falha ao inicializar a Allegro.\n");
        return -1;
    }

    if (!al_init_image_addon()){
        fprintf(stderr, "Falha ao inicializar add-on allegro_image.\n");
        return -1;
    }

    janela = al_create_display(LARGURA_TELA, ALTURA_TELA);
    if (!janela){
        fprintf(stderr, "Falha ao criar janela.\n");
        return -1;
    }

    player.image = al_load_bitmap("sprites/johnny-sobretudo 64x64.png");
    sapo[0].image = al_load_bitmap("sprites/sapo-180x195.png");
    sapo[1].image = al_load_bitmap("sprites/sapo-180x195.png");
    if (!player.image || !sapo[0].image || !sapo[1].image){
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
                if (player.positionY > ALTURA_TELA - 68){
                    isGrounded = true;
                    isFalling = false;
                    player.positionY = ALTURA_TELA - 68;
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

        for(i = 0; i < 2; i++){
            if((sapo[i].positionY < player.positionY + al_get_bitmap_height(player.image) - 10) &&
               (sapo[i].positionY + al_get_bitmap_height(sapo[i].image) > player.positionY) + 10){
                if((sapo[i].positionX > player.positionX) &&
                   (sapo[i].positionX < player.positionX + al_get_bitmap_width(player.image))){
                    player.positionX = sapo[i].positionX - al_get_bitmap_width(player.image);
                    rightSpeed = 0;
                }else{
                    rightSpeed = 3;
                }
                if((sapo[i].positionX + al_get_bitmap_width(sapo[i].image) > player.positionX) &&
                   (sapo[i].positionX + al_get_bitmap_width(sapo[i].image) < player.positionX + al_get_bitmap_width(player.image))){
                    player.positionX = sapo[i].positionX + al_get_bitmap_width(sapo[i].image);
                    leftSpeed = 0;
                }else{
                    leftSpeed = 3;
                }
            }else{
                rightSpeed = 3;
                leftSpeed = 3;
            }
            if((sapo[i].positionX < player.positionX  + al_get_bitmap_width(player.image)) &&
               (sapo[i].positionX + al_get_bitmap_width(sapo[i].image) > player.positionX)){
                if((sapo[i].positionY > player.positionY) &&
                   (sapo[i].positionY < player.positionY + al_get_bitmap_height(player.image))){
                    player.positionY = sapo[i].positionY - al_get_bitmap_height(player.image) - 1;
                    isGrounded = true;
                    isFalling = false;
                    player.speedY = jumpSpeed;
                }
            }else{
                if(player.positionY == sapo[i].positionY - al_get_bitmap_height(player.image) - 1){
                    isGrounded = false;
                    isFalling = true;
                }
            }
        }

        if(redraw){
            redraw = false;

            al_draw_bitmap(sapo[0].image, sapo[0].positionX, sapo[0].positionY, sapo[0].rotationY);
            al_draw_bitmap(sapo[1].image, sapo[1].positionX, sapo[1].positionY, sapo[1].rotationY);
            al_draw_bitmap(player.image, player.positionX, player.positionY, player.rotationY);
            al_flip_display();
            al_clear_to_color(al_map_rgb(0, 0, 0));
        }
    }

    al_destroy_display(janela);
    al_destroy_bitmap(player.image);
    al_destroy_bitmap(sapo[0].image);
    al_destroy_event_queue(fila_eventos);

    return 0;
}
