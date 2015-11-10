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
#include "resources/platform.h"
#include "resources/level.h"

ALLEGRO_DISPLAY *janela = NULL;
ALLEGRO_EVENT_QUEUE *fila_eventos = NULL;
ALLEGRO_TIMER *timer = NULL;
ALLEGRO_FONT *font = NULL;

int main(void){
    bool sair = false;
    bool teclaDown[3] = {false, false, false};
    bool isGrounded = false,
         isFalling = false,
         isJumping = false,
         isShooting = false,
         receiveDamage = false,
         canMove = true,
         isWalking = false;
    bool redraw = true;
    bool freeMemory = false;

    int nBullets = 0, bulletsOut = 0;
    int nPlatforms = 9;
    int nEnemys = 2;
    int actualLevel = 1;

    int maxFrame = 4;
    int curPlayerFrame = 0;
    int curEnemyFrame = 0;
    int initFrame = 0;
    int curMoveFrame = 0;
    int frameCount = 0;
    int frameDelay = 7;

    int i;
    int FPS = 60;

    float jumpSpeed = 5, rightSpeed = 3, leftSpeed = 3;

    PLAYER player;
    SPRITE *platform;
    ENEMY *enemy;
    SPRITE *bullet;

    bullet = (SPRITE *) malloc(sizeof(SPRITE)*1000);
    platform = (SPRITE *) malloc(sizeof(SPRITE)*nPlatforms);
    enemy = (ENEMY *) malloc(sizeof(ENEMY)*nEnemys);

    initializeAllegro(&janela, &fila_eventos, &timer, &font, FPS);

    initializePlayer(&player);

    loadLevel(1, platform, enemy, &nPlatforms, &nEnemys, &janela);

    if (!player.sprite.image[0]){
        fprintf(stderr, "Falha ao carregar o arquivo de imagem.\n");
        al_destroy_display(janela);
        return -1;
    }

    while (!sair){
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
                if(player.sprite.positionX >= getScreenWidth()){
                    if(actualLevel == 1){
                        platform = (SPRITE *) realloc(platform, sizeof(SPRITE)*6);
                        enemy = (ENEMY *) realloc(enemy, sizeof(ENEMY)*2);
                        player.sprite.positionX = 0;
                        loadLevel(2, platform, enemy, &nPlatforms, &nEnemys, &janela);
                        actualLevel = 2;
                    }else if(actualLevel == 2){
                        platform = (SPRITE *) realloc(platform, sizeof(SPRITE)*3);
                        enemy = (ENEMY *) realloc(enemy, sizeof(ENEMY)*3);
                        player.sprite.positionX = 0;
                        loadLevel(3, platform, enemy, &nPlatforms, &nEnemys, &janela);
                        actualLevel = 3;
                    }
                }else if(player.sprite.positionX + al_get_bitmap_width(player.sprite.image[0]) <= 0){
                    if(actualLevel == 2){
                        platform = (SPRITE *) realloc(platform, sizeof(SPRITE)*2);
                        enemy = (ENEMY *) realloc(enemy, sizeof(ENEMY)*3);
                        player.sprite.positionX = getScreenWidth() - al_get_bitmap_width(player.sprite.image[0]);
                        loadLevel(1, platform, enemy, &nPlatforms, &nEnemys, &janela);
                        actualLevel = 1;
                    }else if(actualLevel == 3){
                        platform = (SPRITE *) realloc(platform, sizeof(SPRITE)*6);
                        enemy = (ENEMY *) realloc(enemy, sizeof(ENEMY)*2);
                        player.sprite.positionX = getScreenWidth() - al_get_bitmap_width(player.sprite.image[0]);
                        loadLevel(2, platform, enemy, &nPlatforms, &nEnemys, &janela);
                        actualLevel = 2;
                    }
                }
                for(i = 0; i < nBullets; i++){
                    bullet[i].positionX += bullet[i].speedX;
                }

                for(i = 0; i < nEnemys; i++){
                    enemy[i].sprite.positionX += enemy[i].sprite.speedX;
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
                    if(!canMove){
                        if(++curMoveFrame >= 4){
                            curMoveFrame = 0;
                            canMove = true;
                            receiveDamage = false;
                        }
                    }else{
                        if(++curPlayerFrame >= maxFrame){
                            curPlayerFrame = initFrame;
                        }
                    }
                    if(++curEnemyFrame >= 2){
                        curEnemyFrame = 0;
                    }
                    frameCount = 0;
                }

                if(isJumping){
                    curPlayerFrame = 8;
                    isGrounded = false;
                    isFalling = false;
                }else if(isFalling){
                    curPlayerFrame = 9;
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
                //Verifica colisão entre PLAYER e INIMIGOS, calcula o dano recebido pelo PLAYER em caso de colisão
                damageCount(&player, enemy, nEnemys, &receiveDamage, &canMove, &curPlayerFrame);

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
            drawScreen(&player, platform, enemy, bullet, &font, nBullets, nEnemys, nPlatforms, curPlayerFrame, curEnemyFrame);
        }
    }

    destroy(&player.sprite, platform, enemy, bullet, &janela, &fila_eventos, &font, nBullets, nEnemys, nPlatforms);

    return 0;
}
