# Makefile necessário para simplificar a compilação do jogo 

TARGET = main

# Compilador
CC = cc

# Flags de compilação e linkagem para Linux
CFLAGS = -Wall -std=c99
LDFLAGS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

.PHONY: all run clean

all: run

run: $(TARGET)
	./$(TARGET)

$(TARGET): main.c
	$(CC) $(CFLAGS) main.c -o $(TARGET) $(LDFLAGS)

clean:
	rm -f $(TARGET)
