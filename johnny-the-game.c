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

    float jumpSpeed = 5;

    struct SPRITE player;

    player.positionX = 10;
    player.positionY = 160;
    player.rotationY = 0;
    player.speedY = jumpSpeed;

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
    if (!player.image){
        fprintf(stderr, "Falha ao carregar o arquivo de imagem.\n");
        al_destroy_display(janela);
        return -1;
    }

    if (!al_install_keyboard())
    {
        fprintf(stderr, "Falha ao inicializar o teclado.\n");
        return false;
    }

    fila_eventos = al_create_event_queue();
    if (!fila_eventos){
        fprintf(stderr, "Falha ao criar fila de eventos.\n");
        al_destroy_display(janela);
        return -1;
    }

    al_register_event_source(fila_eventos, al_get_keyboard_event_source());
    al_register_event_source(fila_eventos, al_get_display_event_source(janela));

    while (!sair)    {
      while(!al_is_event_queue_empty(fila_eventos)){

        ALLEGRO_EVENT evento;
        al_wait_for_event(fila_eventos, &evento);

        if (evento.type == ALLEGRO_EVENT_KEY_DOWN){
          if(evento.keyboard.keycode == ALLEGRO_KEY_UP){
            teclaDown[0] = true;
          }else if(evento.keyboard.keycode == ALLEGRO_KEY_LEFT){
            teclaDown[1] = true;
          }else if(evento.keyboard.keycode == ALLEGRO_KEY_RIGHT){
            teclaDown[2] = true;
          }
        }else if(evento.type == ALLEGRO_EVENT_KEY_UP){
          if(evento.keyboard.keycode == ALLEGRO_KEY_UP){
            teclaDown[0] = false;
          }else if(evento.keyboard.keycode == ALLEGRO_KEY_LEFT){
            teclaDown[1] = false;
          }else if(evento.keyboard.keycode == ALLEGRO_KEY_RIGHT){
            teclaDown[2] = false;
          }
        }else if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
            sair = true;
        }
      }

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
          isJumping = false;
          isFalling = true;
        }
      }
      if (isGrounded && teclaDown[0]){
          isJumping = true;
          isGrounded = false;
          isFalling = false;
      }else if(!isGrounded && !teclaDown[0]){
        isFalling = true;
        isJumping = false;
      }
      if (!isGrounded && isFalling){
          player.positionY += player.speedY;
          player.speedY+=0.2;
      }
      if (teclaDown[1]){
          player.positionX-=3;
          player.rotationY = 1;
      }
      if (teclaDown[2]){
          player.positionX+=3;
          player.rotationY = 0;
      }
      al_draw_bitmap(player.image, player.positionX, player.positionY, player.rotationY);
      al_flip_display();
      al_clear_to_color(al_map_rgb(0, 0, 0));
    }

    al_destroy_display(janela);
    al_destroy_bitmap(player.image);
    al_destroy_event_queue(fila_eventos);

    return 0;
}
