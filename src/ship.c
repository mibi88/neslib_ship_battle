#include <ship.h>
#include <draw.h>

const uint8 tiles[4] = {
    '.',
    'o',
    'B',
    'D'
};

void displayship(uint8 x, uint8 y, const uint8 shipnames[5][20],
    uint8 shipnum) {
    /* Display selected boat. */
    clearline(y);
    write(x, y, (uint8*) shipnames[shipnum-1]);
}

void drawmap(uint8 *map) {
    /* sx and sy are the start positions. */
    uint8 y=0;
    uint8 x=0;
    uint8 mapnum;
    putc_vram2(0, 0, 'I');
    putc_vram2(11, 11, 'I');
    for(x=0;x!=10;x++){
        putc_vram2(x, 0, '-');
        putc_vram2(x, 11, '-');
    }
    for(y=0;y!=10;y++){
        putc_vram2(0, y, 'I');
        putc_vram2(11, y, 'I');
        for(x=0;x!=10;x++){
            mapnum=map[y*10+x];
            putc_vram2(x+1, y+1, tiles[mapnum]);
        }
    }
    putc_vram2(0, 11, 'I');
    putc_vram2(11, 0, 'I');
}

void shiponmap(uint8 x, uint8 y, uint8 orient, uint8 length, uint8 c) {
    /* Display the ship player a will be moving. */
    uint8 i;
    /* sx and sy are the start positions. */
    i=0;
    while(i!=length){
        if(orient==0){
            putc_vram2(x, y+i, c);
        }else if(orient==1){
            putc_vram2(x+i, y, c);
        }
        i++;
    }
}

void drawships(uint8 sx, uint8 sy, uint8 *ships_x, uint8 *ships_y,
    uint8 *ships_orient, uint8 *ships_lengths, uint8 notshipnum, uint8 *map) {
    /* Variables for a boat. */
    uint8 x, y, orient, length, i;
    /* Variable for the loop. */
    uint8 b;
    /* Draw anything. */
    drawmap(map);
    for(b=0;b!=5;b++){
        if(b!=notshipnum-1){ /* We do not want to draw the ship the user is
            moving. */
            /* Set the variables for a ship. */
            x=ships_x[b];
            y=ships_y[b];
            orient=ships_orient[b];
            length=ships_lengths[b];
            shiponmap(x, y, orient, length, 'O');
        }else{
            shiponmap(x, y, orient, length, 'X');
        }
    }
    ppu_off();
    draw_vram2(sx-1, sy-1);
}

void displayshootpos(uint8 x, uint8 y, uint8 sx, uint8 sy, uint8 *map) {
    drawmap(map);
    ppu_off();
    draw_vram2(sx-1, sy-1);
    write(sx+x, sy+y, (uint8*) "X");
    ppu_on_all();
}
