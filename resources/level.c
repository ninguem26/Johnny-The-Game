#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#include <stdio.h>

#include "utils.h"
#include "level.h"
#include "platform.h"

void loadLevel(int levelNumber, SPRITE *platform, int *nPlatforms, ALLEGRO_DISPLAY **janela){
    if(levelNumber == 1){
        initializePlatform(platform, &nPlatforms, &janela, 2, 3, 2, 2);

        platform[0].positionX = 1*32;
        platform[0].positionY = 537;
        platform[0].rotationY = 0;

        platform[1].positionX = 8*32;
        platform[1].positionY = 536;
        platform[1].rotationY = 0;

        platform[2].positionX = 10*32;
        platform[2].positionY = 15*32;
        platform[2].rotationY = 0;

        platform[3].positionX = 1*32;
        platform[3].positionY = 8*32;
        platform[3].rotationY = 0;

        platform[4].positionX = 12*32;
        platform[4].positionY = 16*32;
        platform[4].rotationY = 0;

        platform[5].positionX = 768;
        platform[5].positionY = 0;
        platform[5].rotationY = 0;

        platform[6].positionX = 0;
        platform[6].positionY = 0;
        platform[6].rotationY = 0;

        platform[7].positionX = 0;
        platform[7].positionY = 0;
        platform[7].rotationY = 0;

        platform[8].positionX = 0;
        platform[8].positionY = 568;
        platform[8].rotationY = 0;
    }
}
