#ifndef SHIP_H
#define SHIP_H

#include <main.h>

void displayship(uint8 x, uint8 y, const uint8 shipnames[5][20], uint8 shipnum);
void drawmap(uint8 *map);
void drawships(uint8 sx, uint8 sy, uint8 *ships_x, uint8 *ships_y,
    uint8 *ships_orient, uint8 *ships_lengths, uint8 notshipnum, uint8 *map);
void displayshootpos(uint8 x, uint8 y, uint8 sx, uint8 sy, uint8 *map);

#endif
