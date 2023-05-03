CC = mos-nes-nrom-clang
LIBS = -lneslib -lnesdoug

SRC = ./src/main.c \
      ./src/chr.s

BIN = build/game.nes

all:
	mkdir build
	$(CC) $(SRC) -o $(BIN) $(LIBS)
