# Johnny-The-Game

Jogo em desenvolvimento, utilizando a linguagem C em conjunto com a biblioteca Allegro5.

## Notas para instalação

Para compilar e executar o projeto é necessário o cumprimento de alguns requisitos. São eles, a instalação da biblioteca Allegro5 e suas dependências e a chamada da biblioteca para compilar o código.

Para instalação das dependências da Allegro5, abra o terminal de sua distribuição Linux e execute os seguintes comandos:

```
$ sudo apt-get install build-essential cmake xorg-dev libgl1-mesa-dev libglu-dev
$ sudo apt-get install libpng-dev libcurl4-dev libfreetype6-dev  libjpeg-dev libvorbis-dev libopenal-dev  libphysfs-dev libgtk2.0-dev libasound-dev 
libpulse-dev libflac-dev libdumb1-dev
```

Agora, instalando a Allegro5, é preciso descompactar o arquivo .tar.gz. Para isto, basta usar o comando:

```
$ tar -xvzf allegro-5.0.10.tar.gz
```

Cria-se, então, uma build da Allegro5 antes de compilar o binary source:

```
$ cmake .. -DCMAKE_INSTALL_PREFIX=/usr
```

Agora é hora de compilar e instalar a biblioteca no sistema:

```
$ make
$ sudo make install
```

Com a Allegro5 instalada, basta agora compilar o arquivo do projeto e executá-lo:

```
$ gcc -Wall johnny-the-game.c -o johnny-the-game `pkg-config --cflags --libs allegro-5.0 allegro_image-5.0`
$ ./johnny-the-game
```
