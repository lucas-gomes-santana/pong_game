# Makefile para simplificar a compilação e inicialização do jogo
# Funciona em Linux, macOS e Windows 

TARGET = main

# Compilador
CC = cc

# Flags de compilação
CFLAGS = -Wall -std=c99

# Detecção de sistema operacional 
ifeq ($(OS),Windows_NT)
    DETECTED_OS := Windows
else
    DETECTED_OS := $(shell uname -s)
endif

ifeq ($(DETECTED_OS),Windows)
    TARGET_EXT = .exe
    LDFLAGS = -lraylib -lopengl32 -lgdi32 -lwinmm
    RM = del /Q
    RUN = $(TARGET)$(TARGET_EXT)
else ifeq ($(DETECTED_OS),Darwin)
    TARGET_EXT =
    LDFLAGS = -lraylib -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
    RM = rm -f
    RUN = ./$(TARGET)$(TARGET_EXT)
else
    TARGET_EXT =
    LDFLAGS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
    RM = rm -f
    RUN = ./$(TARGET)$(TARGET_EXT)
endif

.PHONY: all run clean

all: run

run: $(TARGET)$(TARGET_EXT)
	$(RUN)/

$(TARGET)$(TARGET_EXT): main.c
	$(CC) $(CFLAGS) main.c -o $(TARGET)$(TARGET_EXT) $(LDFLAGS)

clean:
	$(RM) $(TARGET)$(TARGET_EXT)
