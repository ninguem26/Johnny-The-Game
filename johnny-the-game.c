// Os arquivos de cabeçalho
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

// Para utilizarmos o fprintf
#include <stdio.h>

// Atributos da tela
const int LARGURA_TELA = 640;
const int ALTURA_TELA = 480;

ALLEGRO_DISPLAY *janela = NULL;
ALLEGRO_BITMAP *imagem = NULL;
ALLEGRO_EVENT_QUEUE *fila_eventos = NULL;

int main(void){
    bool sair = false;
    bool teclaDown[3] = {false, false, false};
    float dir_x = 10, dir_y = 160, rot_y = 0;
    float velocidadeQueda = 3;

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

    imagem = al_load_bitmap("sprites/johnny-sobretudo 64x64.png");
    if (!imagem){
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

      if (teclaDown[0]){
          dir_y-=3;
      }
      if (dir_y < ALTURA_TELA - 68){
          dir_y += velocidadeQueda;
          velocidadeQueda+=0.2;
      }else{
        dir_y = ALTURA_TELA - 68;
        velocidadeQueda = 3;
      }
      if (teclaDown[1]){
          dir_x-=3;
          rot_y = 1;
      }
      if (teclaDown[2]){
          dir_x+=3;
          rot_y = 0;
      }
      al_draw_bitmap(imagem, dir_x, dir_y, rot_y);
      al_flip_display();
      al_clear_to_color(al_map_rgb(0, 0, 0));
    }

    al_destroy_display(janela);
    al_destroy_bitmap(imagem);
    al_destroy_event_queue(fila_eventos);

    return 0;
}
