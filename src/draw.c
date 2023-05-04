#include <draw.h>
#include <neslib.h>

static uint8 vram2[12*12];

void write(uint8 x, uint8 y, uint8* text) {
    /* set where I need to start writing in the VRAM. */
    vram_adr(NTADR_A(x, y)); /* screen is 32 x 30 tiles. */
    for(i=0;text[i];i++){
        vram_put(text[i]); /* this pushes 1 char to the screen. */
        if(i>=255) break;
    }
}

void putc(uint8 x, uint8 y, uint8 c) {
    vram_adr(NTADR_A(x, y));
    vram_put(c);
}

void draw_vram2(uint8 sx, uint8 sy) {
    uint8 x, y, dy;
    dy = 0;
    for(y=0;y<12;y++){
        vram_adr(NTADR_A(sx, sy+y));
        for(x=0;x<12;x++){
            vram_put(vram2[dy+x]);
        }
        dy += 12;
    }
}

void putc_vram2(uint8 x, uint8 y, uint8 c) {
    if(x>=0 && x<12 && y>0 && y<12){
        vram2[y*12+x] = c;
    }
}

void clearline(uint8 y) {
    vram_adr(NTADR_A(0, y));
    for(i=0;i<32;i++){
        vram_put(' '); /* this pushes 1 char to the screen. */
    }
}

void clearscreen(void) {
    vram_adr(NTADR_A(0, 0));
    for(i=0;i<32*30/4;i++){
        vram_put(' ');
        vram_put(' ');
        vram_put(' ');
        vram_put(' ');
    }
}
