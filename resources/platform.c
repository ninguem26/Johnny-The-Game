#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#include <stdio.h>

#include "utils.h"
#include "platform.h"

void initializePlatform(SPRITE *platform, int **nPlatforms, ALLEGRO_DISPLAY ***janela, int small, int medium, int big, int larger){
    int totalPlatforms = small + medium + big + larger;
    int i, j = 0;

    for(i = 0; i < small; i++){
        platform[i].image[0] = al_load_bitmap("sprites/tijolos 32x32.png");
        j++;
    }
    for(i = j; i < small+medium; i++){
        platform[i].image[0] = al_load_bitmap("sprites/tijolos 64x32.png");
        j++;
    }
    for(i = j; i < small+medium+big; i++){
        platform[i].image[0] = al_load_bitmap("sprites/tijolos 32x600.png");
        j++;
    }
    for(i = j; i < totalPlatforms; i++){
        platform[i].image[0] = al_load_bitmap("sprites/tijolos 800x32.png");
        j++;
    }
    for(i = 0; i < totalPlatforms; i++){
        if (!platform[i].image[0]){
            fprintf(stderr, "Falha ao carregar o arquivo de imagem.\n");
            al_destroy_display(**janela);
        }
    }
    **nPlatforms = totalPlatforms;
}
