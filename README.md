# Johnny-The-Game

Jogo em desenvolvimento, utilizando a linguagem C em conjunto com a biblioteca Allegro5.

## Notas para instalação

Para compilar e executar o projeto é necessário o cumprimento de alguns requisitos. São eles, a instalação da biblioteca Allegro5 e suas dependências e a chamada da biblioteca para compilar o código.

Para instalação das dependências da Allegro5, abra o terminal de sua distribuição Linux, navegue até a raiz da pasta com o projeto e execute o seguinte comando:

```
$ sudo bash compile.sh
```

Esse comando executará um arquivo de script em bash que deve instalar as dependências necessárias para a execução do jogo, além de compilar a Allegro 5 e o arquivo .c do jogo. É necessário executá-lo como super usuário, para dar permissão ao download dos arquivos.

Após finalizada a instalação, basta executar o jogo com o comando abaixo:

```
$ ./johnny-the-game
```
