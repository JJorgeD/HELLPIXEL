Trabalho da disciplina de Progamação Imperativa e Funcional.

HellPixel é um jogo de ação em terminal onde o jogador assume o controle de **Lena**, uma guerreira enfrentando ondas de inimigos em um mapa hostil.
Desenvolvido em linguagem C, o jogo utiliza uma engine baseada em caracteres ASCII e bibliotecas de tempo e entrada para criar uma experiência dinâmica em modo texto.


🎮 Sobre o Jogo

Neste jogo você:
- Controla a personagem Lena em um mapa de 72x24.
- Deve eliminar inimigos que surgem constantemente nas bordas do mapa.
- Pode atirar com munição limitada (balas).
- Deve sobreviver o maior tempo possível, administrando saúde, recarregando munição e evitando inimigos.
- Possui um sistema de pontuação e ranking.


🕹️ Como Jogar

Controles principais:
- `W`, `A`, `S`, `D`: movimentam a personagem para, esquerda e direita.
- `ESPACO`: atira na direção atual.
- `R`: recarrega as balas (tempo de recarga é necessário).
- `Q`: sai do jogo.

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
- Biblioteca padrão `unistd.h`, `time.h`, `math.h`, `stdio.h`, `stdlib.h`, `string.h`


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

Desenvolvido por Rodrigo Tenório, Mirna Lustosa e Jorge Dias.  
