CC = mos-nes-nrom-clang
CFLAGS = -Wextra -Wpedantic -std=c89 -O3
LIBS = -lneslib -Iinc

SRC = ./src/main.c \
      ./src/draw.c \
	  ./src/ship.c \
      ./src/chr.s

BIN = build/game.nes

all:
	mkdir -p build
	$(CC) $(SRC) -o $(BIN) $(LIBS) $(CFLAGS)
clean:
	rm --recursive --force ./build/
