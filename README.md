Trabalho da disciplina de Progamação Imperativa e Funcional.

HellPixel é um jogo de ação em terminal onde o jogador assume o controle de **Lena**, uma guerreira enfrentando ondas de inimigos em um mapa hostil.
Desenvolvido em linguagem C, o jogo utiliza uma engine baseada em caracteres ASCII e bibliotecas de tempo e entrada para criar uma experiência dinâmica em modo texto.


🎮 Sobre o Jogo

Neste jogo você:
- Controla a personagem Lena em um mapa renderizado no terminal.
- Deve eliminar inimigos que surgem constantemente no topo do mapa.
- Pode atirar com munição limitada (balas).
- Deve sobreviver o maior tempo possível, administrando saúde, recarregando munição e evitando inimigos.
- Possui um sistema de pontuação salvo em um arquivo ao final do jogo.


🕹️ Como Jogar

Controles principais:
- `A`, `D`: movimentam a personagem para esquerda e direita.
- `F`: atira na direção atual.

Objetivo:
- Sobreviver ao máximo contra inimigos que aparecem em intervalos regulares.
- Pontuar eliminando inimigos.


Estrutura do Projeto

- `src/`: arquivos-fonte do jogo (`main.c`, `keyboard.c`, `screen.c`, `timer.c`)
- `include/`: cabeçalhos com declarações das funções e structs.
- `menu/`: arquivos `.txt` com telas de início, vitória e derrota.
- `score/`: rankings de pontuação.
- `makefile`: automação de compilação.
- `HellPixel.out`: binário gerado após compilação.


Requisitos

Para compilar e executar o jogo, é necessário um sistema compatível com:
- GCC (compilador C)
- Linux/macOS com terminal
- Biblioteca padrão cli-bi: https://github.com/tgfb/cli-lib/


## ⚙️ Instruções de Compilação

1. Abra o terminal na pasta `Hellpixel-main`
2. Compile o jogo com o comando:

   ```bash
   make
   ```

3. Rode o executável:

   ```bash
   ./HellPixel.out
   ```

4. (Opcional) Para limpar os arquivos objetos:

   ```bash
   make clean
   ```

---

🏆 Créditos

 Video demonstrativo do jogo:
https://youtu.be/cX3LpLzaX9M

Desenvolvido por Rodrigo Tenório, Mirna Lustosa e Jorge Dias.  
