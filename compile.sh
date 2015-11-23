#!/bin/bash

	sudo apt-get install build-essential cmake xorg-dev libgl1-mesa-dev libglu-dev

  sudo apt-get install libpng-dev libcurl4-openssl-dev libfreetype6-dev  libjpeg-dev libvorbis-dev libopenal-dev  libphysfs-dev libgtk2.0-dev libasound-dev
libpulse-dev libflac-dev libdumb1-dev

  cd libs/allegro-5.0.10/
  mkdir build
  cd build;

  cmake .. -DCMAKE_INSTALL_PREFIX=/usr

  make

  sudo make install

  cd ..
  cd ..
  cd ..

  gcc -Wall johnny-the-game.c -o johnny-the-game resources/structs.h resources/utils.h resources/utils.c resources/physics.h resources/physics.c resources/player.h resources/player.c resources/enemy.h resources/enemy.c resources/platform.h resources/platform.c resources/level.h resources/level.c `pkg-config --cflags --libs allegro-5.0 allegro_image-5.0 allegro_acodec-5.0 allegro_audio-5.0 allegro_font-5.0 allegro_ttf-5.0`
