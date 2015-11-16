// Arquivos da Allegro 5
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

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
ALLEGRO_FONT *fonte = NULL;
ALLEGRO_SAMPLE *menuSong = NULL;
ALLEGRO_SAMPLE *menuMusic = NULL;
ALLEGRO_SAMPLE *gameMusic = NULL;
ALLEGRO_BITMAP *background = NULL;

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
    bool redraw = false;
    //bool freeMemory = false;
    bool gameStarted = false;
    bool gameOver = false;
    bool playingGameMusic = false, playingMenuMusic = true;
    bool canShoot = true;
    bool endGame = false;

    int nBullets = 0 /**, bulletsOut = 0**/;
    int nPlatforms = 2;
    int nEnemys = 0;
    int actualLevel = 1;
    int cabecalho = 1, startText = 1;
    int mouseX, mouseY;

    int maxFrame = 4;
    int curPlayerFrame = 0;
    int curEnemyFrame = 0;
    int initFrame = 0;
    int curMoveFrame = 0;
    int curShootFrame = 0;
    int curCreditsFrame = 0;
    int creditContent = 0;
    int frameCount = 0;
    int frameDelay = 7;

    int i;
    int FPS = 60;

    float jumpSpeed = 5, rightSpeed = 3, leftSpeed = 3;

    PLAYER player;
    SPRITE *platform;
    ENEMY *enemy;
    SPRITE *bullet;
    SPRITE botao_start, botao_sair, area_central;

    bullet = (SPRITE *) malloc(sizeof(SPRITE)*1000);
    platform = (SPRITE *) malloc(sizeof(SPRITE)*nPlatforms);
    enemy = (ENEMY *) malloc(sizeof(ENEMY)*nEnemys);

    initializeAllegro(&janela, &fila_eventos, &timer, &font, &fonte, &menuSong, &menuMusic, &gameMusic, FPS);

    initializeMenu(&botao_start, &botao_sair, &area_central, &janela);

    initializePlayer(&player);

    background = al_load_bitmap("sprites/tijolos 800x600.png");
    if (!background){
        fprintf(stderr, "Falha ao carregar imagem de fundo.\n");
        al_destroy_display(janela);
        return false;
    }

    al_play_sample(menuSong, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
    al_play_sample(menuMusic, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);
    loadLevel(1, platform, enemy, bullet, &nPlatforms, &nEnemys, &nBullets, &janela);

    if (!player.sprite.image[0]){
        fprintf(stderr, "Falha ao carregar o arquivo de imagem.\n");
        al_destroy_display(janela);
        return -1;
    }

    while (!sair){
        while(!al_is_event_queue_empty(fila_eventos)){

            ALLEGRO_EVENT evento;
            al_wait_for_event(fila_eventos, &evento);

            if(evento.type == ALLEGRO_EVENT_KEY_DOWN){
                if(gameStarted){
                    if(evento.keyboard.keycode == ALLEGRO_KEY_UP && isGrounded){
                        teclaDown[0] = true;
                    }else if(evento.keyboard.keycode == ALLEGRO_KEY_LEFT){
                        player.sprite.speedX = leftSpeed;
                        teclaDown[1] = true;
                    }else if(evento.keyboard.keycode == ALLEGRO_KEY_RIGHT){
                        player.sprite.speedX = rightSpeed;
                        teclaDown[2] = true;
                    }else if(evento.keyboard.keycode == ALLEGRO_KEY_X && canShoot){
                        isShooting = true;
                        canShoot = false;
                    }else if(evento.keyboard.keycode == ALLEGRO_KEY_ENTER){
                        gameStarted = false;
                        teclaDown[0] = false;
                        teclaDown[1] = false;
                        teclaDown[2] = false;
                        if(actualLevel == 9){
                            cabecalho = 1;
                            startText = 1;
                        }else{
                            cabecalho = 2;
                            startText = 2;
                        }
                    }
                }else{
                    if(evento.keyboard.keycode == ALLEGRO_KEY_ENTER){
                        gameStarted = true;
                        endGame = false;
                        creditContent = 0;
                        curCreditsFrame = 0;
                        if(playingMenuMusic){
                            al_stop_samples();
                            playingMenuMusic = false;
                        }
                        if(!playingGameMusic){
                            al_play_sample(gameMusic, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);
                            playingGameMusic = true;
                        }
                        if(gameOver || actualLevel == 9){
                            initializePlayer(&player);
                            isFalling = true;
                            isJumping = false;
                            isGrounded = false;
                            platform = (SPRITE *) realloc(platform, sizeof(SPRITE)*2);
                            enemy = (ENEMY *) realloc(enemy, sizeof(ENEMY)*0);
                            loadLevel(1, platform, enemy, bullet, &nPlatforms, &nEnemys, &nBullets, &janela);
                            actualLevel = 1;
                            gameOver = false;
                        }
                    }
                }
            }else if(evento.type == ALLEGRO_EVENT_KEY_UP && gameStarted){
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
            }else if(evento.type == ALLEGRO_EVENT_TIMER && gameStarted){
                if(player.sprite.positionX >= getScreenWidth()){
                    if(actualLevel == 1){
                        platform = (SPRITE *) realloc(platform, sizeof(SPRITE)*5);
                        enemy = (ENEMY *) realloc(enemy, sizeof(ENEMY)*2);
                        player.sprite.positionX = 0;
                        loadLevel(2, platform, enemy, bullet, &nPlatforms, &nEnemys, &nBullets, &janela);
                        actualLevel = 2;
                    }else if(actualLevel == 2){
                        platform = (SPRITE *) realloc(platform, sizeof(SPRITE)*11);
                        enemy = (ENEMY *) realloc(enemy, sizeof(ENEMY)*6);
                        player.sprite.positionX = 0;
                        loadLevel(3, platform, enemy, bullet, &nPlatforms, &nEnemys, &nBullets, &janela);
                        actualLevel = 3;
                    }else if(actualLevel == 3){
                        platform = (SPRITE *) realloc(platform, sizeof(SPRITE)*5);
                        enemy = (ENEMY *) realloc(enemy, sizeof(ENEMY)*4);
                        player.sprite.positionX = 0;
                        loadLevel(4, platform, enemy, bullet, &nPlatforms, &nEnemys, &nBullets, &janela);
                        actualLevel = 4;
                    }else if(actualLevel == 6){
                        platform = (SPRITE *) realloc(platform, sizeof(SPRITE)*12);
                        enemy = (ENEMY *) realloc(enemy, sizeof(ENEMY)*3);
                        player.sprite.positionX = 0;
                        loadLevel(5, platform, enemy, bullet, &nPlatforms, &nEnemys, &nBullets, &janela);
                        actualLevel = 5;
                    }else if(actualLevel == 8){
                        platform = (SPRITE *) realloc(platform, sizeof(SPRITE)*7);
                        enemy = (ENEMY *) realloc(enemy, sizeof(ENEMY)*4);
                        player.sprite.positionX = 0;
                        loadLevel(7, platform, enemy, bullet, &nPlatforms, &nEnemys, &nBullets, &janela);
                        actualLevel = 7;
                    }
                }else if(player.sprite.positionX + al_get_bitmap_width(player.sprite.image[0]) <= 0){
                    if(actualLevel == 2){
                        platform = (SPRITE *) realloc(platform, sizeof(SPRITE)*2);
                        enemy = (ENEMY *) realloc(enemy, sizeof(ENEMY)*3);
                        player.sprite.positionX = getScreenWidth() - al_get_bitmap_width(player.sprite.image[0]);
                        loadLevel(1, platform, enemy, bullet, &nPlatforms, &nEnemys, &nBullets, &janela);
                        actualLevel = 1;
                    }else if(actualLevel == 3){
                        platform = (SPRITE *) realloc(platform, sizeof(SPRITE)*5);
                        enemy = (ENEMY *) realloc(enemy, sizeof(ENEMY)*2);
                        player.sprite.positionX = getScreenWidth() - al_get_bitmap_width(player.sprite.image[0]);
                        loadLevel(2, platform, enemy, bullet, &nPlatforms, &nEnemys, &nBullets, &janela);
                        actualLevel = 2;
                    }else if(actualLevel == 4){
                        platform = (SPRITE *) realloc(platform, sizeof(SPRITE)*11);
                        enemy = (ENEMY *) realloc(enemy, sizeof(ENEMY)*6);
                        player.sprite.positionX = getScreenWidth() - al_get_bitmap_width(player.sprite.image[0]);
                        loadLevel(3, platform, enemy, bullet, &nPlatforms, &nEnemys, &nBullets, &janela);
                        actualLevel = 3;
                    }else if(actualLevel == 5){
                        platform = (SPRITE *) realloc(platform, sizeof(SPRITE)*3);
                        enemy = (ENEMY *) realloc(enemy, sizeof(ENEMY)*2);
                        player.sprite.positionX = getScreenWidth() - al_get_bitmap_width(player.sprite.image[0]);
                        isFalling = true;
                        isGrounded = false;
                        loadLevel(6, platform, enemy, bullet, &nPlatforms, &nEnemys, &nBullets, &janela);
                        actualLevel = 6;
                    }else if(actualLevel == 7){
                        platform = (SPRITE *) realloc(platform, sizeof(SPRITE)*14);
                        enemy = (ENEMY *) realloc(enemy, sizeof(ENEMY)*0);
                        player.sprite.positionX = getScreenWidth() - al_get_bitmap_width(player.sprite.image[0]);
                        loadLevel(8, platform, enemy, bullet, &nPlatforms, &nEnemys, &nBullets, &janela);
                        actualLevel = 8;
                    }else if(actualLevel == 8){
                        endGame = true;
                        actualLevel = 9;
                        playingMenuMusic = true;
                        playingGameMusic = false;
                        al_stop_samples();
                        al_play_sample(menuMusic, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);
                    }
                }
                if(player.sprite.positionY + al_get_bitmap_height(player.sprite.image[0]) <= 0){
                    if(actualLevel == 3){
                        platform = (SPRITE *) realloc(platform, sizeof(SPRITE)*12);
                        enemy = (ENEMY *) realloc(enemy, sizeof(ENEMY)*3);
                        player.sprite.positionY = 514;
                        loadLevel(5, platform, enemy, bullet, &nPlatforms, &nEnemys, &nBullets, &janela);
                        actualLevel = 5;
                    }
                }else if(player.sprite.positionY >= getScreenHeigth()){
                    if(actualLevel == 5){
                        platform = (SPRITE *) realloc(platform, sizeof(SPRITE)*11);
                        enemy = (ENEMY *) realloc(enemy, sizeof(ENEMY)*6);
                        player.sprite.positionY = 0;
                        loadLevel(3, platform, enemy, bullet, &nPlatforms, &nEnemys, &nBullets, &janela);
                        actualLevel = 3;
                    }else if(actualLevel == 6){
                        platform = (SPRITE *) realloc(platform, sizeof(SPRITE)*7);
                        enemy = (ENEMY *) realloc(enemy, sizeof(ENEMY)*4);
                        player.sprite.positionY = 0;
                        loadLevel(7, platform, enemy, bullet, &nPlatforms, &nEnemys, &nBullets, &janela);
                        actualLevel = 7;
                    }
                }
                for(i = 0; i < nBullets; i++){
                    bullet[i].positionX += bullet[i].speedX;
                }

                for(i = 0; i < nEnemys; i++){
                    enemy[i].sprite.positionX += enemy[i].sprite.speedX;
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
                    if(!canShoot){
                        if(++curShootFrame >= 6){
                            curShootFrame = 0;
                            canShoot = true;
                        }
                    }
                    if(endGame){
                        if(++curCreditsFrame >= 18){
                            curCreditsFrame = 0;
                            creditContent++;
                            if(creditContent > 16){
                                creditContent = 16;
                            }
                        }
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

                if(player.actualHealth <= 0){
                    gameStarted = false;
                    gameOver = true;
                    teclaDown[0] = false;
                    teclaDown[1] = false;
                    teclaDown[2] = false;
                    isJumping = false;
                    isGrounded = false;
                    isFalling = true;
                    cabecalho = 3;
                    startText = 3;
                    al_stop_samples();
                    playingGameMusic = false;
                }

                redraw = true;
            }else if (evento.type == ALLEGRO_EVENT_MOUSE_AXES){
                mouseX = evento.mouse.x;
                mouseY = evento.mouse.y;
                // Verificamos se ele está sobre a região do retângulo central
                if (interactButton(botao_start, mouseX, mouseY)){
                    setButtonCollor(botao_start.image[0], 213, 243, 255, janela);
                }else{
                    setButtonCollor(botao_start.image[0], 47, 104, 127, janela);
                }
                if (interactButton(botao_sair, mouseX, mouseY)){
                    setButtonCollor(botao_sair.image[0], 213, 243, 255, janela);
                }else{
                    setButtonCollor(botao_sair.image[0], 47, 104, 127, janela);
                }
            }else if(evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP){
                if (interactButton(botao_start, mouseX, mouseY)){
                    gameStarted = true;
                    endGame = false;
                    creditContent = 0;
                    curCreditsFrame = 0;
                    if(playingMenuMusic){
                        al_stop_samples();
                        playingMenuMusic = false;
                    }
                    if(!playingGameMusic){
                        al_play_sample(gameMusic, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);
                        playingGameMusic = true;
                    }
                    if(gameOver || actualLevel == 9){
                        initializePlayer(&player);
                        isFalling = true;
                        isJumping = false;
                        isGrounded = false;
                        platform = (SPRITE *) realloc(platform, sizeof(SPRITE)*2);
                        enemy = (ENEMY *) realloc(enemy, sizeof(ENEMY)*0);
                        loadLevel(1, platform, enemy, bullet, &nPlatforms, &nEnemys, &nBullets, &janela);
                        actualLevel = 1;
                        gameOver = false;
                    }
                }
                if (interactButton(botao_sair, mouseX, mouseY)){
                    sair = 1;
                }
            }else if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
                sair = true;
            }
        }

        if(redraw){
            redraw = false;
            if(!endGame){
                al_draw_bitmap(background, 0, 0, 0);
                drawScreen(&player, platform, enemy, bullet, &font, nBullets, nEnemys, nPlatforms, curPlayerFrame, curEnemyFrame, actualLevel);
            }else{
                drawCredits(creditContent, font, fonte);
            }
        }
        if(!gameStarted){
            drawMenu(botao_start, botao_sair, area_central, font, fonte, cabecalho, startText);
        }
    }

    destroy(&player.sprite, platform, enemy, bullet, &janela, &fila_eventos, &font, &fonte, &menuSong, &menuMusic, &gameMusic,
            nBullets, nEnemys, nPlatforms);

    return 0;
}
