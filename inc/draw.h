#ifndef DRAW_H
#define DRAW_H

#include <main.h>

void write(uint8 x, uint8 y, uint8* text);
void putc(uint8 x, uint8 y, uint8 c);
void draw_vram2(uint8 sx, uint8 sy);
void putc_vram2(uint8 x, uint8 y, uint8 c);
void clearline(uint8 y);
void clearscreen(void);

#endif
