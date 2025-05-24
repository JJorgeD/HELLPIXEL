# Nome do executável
TARGET = hellpixel

# Compilador e flags
CC = gcc
CFLAGS = -Wall -g -Iinclude

# Fontes (ajuste para o diretório src)
SRCS = src/main.c src/screen.c src/keyboard.c src/timer.c
OBJS = $(SRCS:.c=.o)

# Regra padrão
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) -lm

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Rodar o jogo
run: $(TARGET)
	./$(TARGET)

# Limpar arquivos objeto e o executável
clean:
	rm -f $(OBJS) $(TARGET)
