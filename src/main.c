/*    NesLibShipBattle
 *    by mibi88
 *    based on the "Hello
 *    World !" example
 *    using neslib
 *    by Doug Fraker in 2018
 */
/*    Reminders
 *    -done- clear boats x, y and orient for player 2.
 *    let the players replay.
 */


#include <neslib.h>
#include <draw.h>
#include <main.h>

#define BLACK 0x0f
#define WHITE 0x30

#pragma bss-name(push, "ZEROPAGE")

unsigned char i;

const unsigned char palette[] = {
    WHITE, BLACK, BLACK, BLACK,
    0,0,0,0,
    0,0,0,0,
    0,0,0,0
};

typedef struct {
    unsigned char boats[100];
    unsigned char hits[100];
} Player;

unsigned char button;
unsigned char game;
unsigned char shipnum=1;
const unsigned char shipnames[5][20] = {
    "porte avion",
    "croiseur",
    "contre-torpilleur",
    "contre-torpilleur 2",
    "torpilleur"
};
const unsigned char boats_sizes[5] = {
    5,
    4,
    3,
    3,
    2
};

unsigned char boats_x[5]; /* Y coordinates of the boats for select menu. */

unsigned char boats_y[5]; /* Y coordinates of the boats for select menu. */

unsigned char boats_orient[5]; /* 0 = vertical, 1 = horizontal. */

unsigned char boat_size = 5;
unsigned char boat_place_test[100];
unsigned char x, y, orient, length, b, check, shoot_x, shoot_y;

Player player_a, player_b;

int main(void) {
    game=0;
    shipnum=1;
    boat_size=boats_sizes[shipnum-1];
    for(i=0;i!=100;i++){
        player_a.boats[i] = 0;
        player_a.hits[i] = 0;
        player_b.boats[i] = 0;
        player_b.hits[i] = 0;
    }

    ppu_off(); /* screen off. */

    pal_bg((const char*) palette); /* load the BG palette. */

    write(7, 5, (unsigned char*) "NeslibShipBattle");
    write(8, 25, (unsigned char*) "Pressez [Start]");
    write(11, 15, (unsigned char*) "fait avec");
    write(1, 16, (unsigned char*) "la neslib modifiee de nesdoug.");
    write(1, 29, (unsigned char*) "(c) 2022 mibi88");
    write(26, 29, (unsigned char*) "v.1.2");

    ppu_on_all(); /* turn on screen. */

    while(1){
        /* infinite loop. */
        ppu_wait_frame();
        /* ********** PLAYER 1 ********** */
        button=pad_trigger(0);
        if(button&PAD_LEFT){
        if(game==2){
            if(boats_x[shipnum-1]>0){
                boats_x[shipnum-1]--;
            }
            ppu_on_all(); /* turn on screen. */
            drawships(10, 4, boats_x, boats_y, boats_orient, (unsigned char *)boats_sizes, shipnum, player_a.boats);
            ppu_on_all(); /* turn on screen. */
        }else if(game==8){
            if(shoot_x>0){
                shoot_x--;
                ppu_off(); /* screen off. */
                displayshootpos(shoot_x, shoot_y, 2, 18, player_a.hits);
                ppu_on_all(); /* turn on screen. */
            }
        }
        }
        if(button&PAD_RIGHT){
            if(game==2){
                if(boats_orient[shipnum-1]==0){
                    if(boats_x[shipnum-1]<9){
                        boats_x[shipnum-1]++;
                    }
                }else{
                    if(boats_x[shipnum-1]<10-(unsigned char) boats_sizes[shipnum-1]){
                        boats_x[shipnum-1]++;
                    }
                }
                ppu_on_all(); /* turn on screen. */
                drawships(10, 4, boats_x, boats_y, boats_orient, (unsigned char *)boats_sizes, shipnum, player_a.boats);
                ppu_on_all(); /* turn on screen. */
            }else if(game==8){
                if(shoot_x<9){
                    shoot_x++;
                    ppu_off(); /* screen off. */
                    displayshootpos(shoot_x, shoot_y, 2, 18, player_a.hits);
                    ppu_on_all(); /* turn on screen. */
                }
            }
        }
        if(button&PAD_UP){
            if(game==2){
                if(boats_y[shipnum-1]>0){
                    boats_y[shipnum-1]--;
                }
                ppu_on_all(); /* turn on screen. */
                drawships(10, 4, boats_x, boats_y, boats_orient, (unsigned char *)boats_sizes, shipnum, player_a.boats);
                ppu_on_all(); /* turn on screen. */
            }else if(game==8){
                if(shoot_y>0){
                    shoot_y--;
                    ppu_off(); /* screen off. */
                    displayshootpos(shoot_x, shoot_y, 2, 18, player_a.hits);
                    ppu_on_all(); /* turn on screen. */
                }
            }
        }
        if(button&PAD_DOWN){
            if(game==2){
                if(boats_orient[shipnum-1]==0){
                    if(boats_y[shipnum-1]<10-(unsigned char) boats_sizes[shipnum-1]){
                        boats_y[shipnum-1]++;
                    }
                }else{
                    if(boats_y[shipnum-1]<9){
                        boats_y[shipnum-1]++;
                    }
                }
                ppu_on_all(); /* turn on screen. */
                drawships(10, 4, boats_x, boats_y, boats_orient, (unsigned char *)boats_sizes, shipnum, player_a.boats);
                ppu_on_all(); /* turn on screen. */
            }else if(game==8){
                if(shoot_y<9){
                    shoot_y++;
                    ppu_off(); /* screen off. */
                    displayshootpos(shoot_x, shoot_y, 2, 18, player_a.hits);
                    ppu_on_all(); /* turn on screen. */
                }
            }
        }
        if(button&PAD_A){
            if(game==1){
                shipnum=1;
                for(i=0;i!=5;i++){
                    boats_x[i]=0;
                    boats_y[i]=0;
                    boats_orient[i]=0;
                }
                ppu_off();
                clearline(1);
                clearline(2);
                clearline(3);
                write(5, 2, (unsigned char*) "--- Vos bateaux ---");
                write(1, 18, (unsigned char*) "Utilisez [Select] pour changer");
                write(1, 19, (unsigned char*) "de bateau et les fleches pour");
                write(1, 20, (unsigned char*) "naviguer sur la grille.");
                write(1, 21, (unsigned char*) "Avec [A] vous pouvez tourner");
                write(1, 22, (unsigned char*) "le bateau, symbolise par des");
                write(1, 23, (unsigned char*) "X.");
                displayship(5, 25, shipnames, shipnum);
                ppu_on_all();
                drawships(10, 4, boats_x, boats_y, boats_orient,
                    (unsigned char *)boats_sizes, shipnum, player_a.boats);
                game=2; /* Let the player 1 place his boats. */
                ppu_on_all(); /* turn on screen. */
            }else if(game==2){
                if(boats_orient[shipnum-1]==0 && boats_x[shipnum-1] <
                    11-(unsigned char) boats_sizes[shipnum-1]){
                    boats_orient[shipnum-1]=1;
                }else if(boats_orient[shipnum-1]==1 && boats_y[shipnum-1] <
                    11-(unsigned char) boats_sizes[shipnum-1]){
                    boats_orient[shipnum-1]=0;
                }
                ppu_on_all();
                drawships(10, 4, boats_x, boats_y, boats_orient,
                    (unsigned char *)boats_sizes, shipnum, player_a.boats);
                ppu_on_all(); /* turn on screen. */
            }
            if(game==3){ /* Put the boats on the grid. */
                unsigned char i=0;
                for(b=0;b!=5;b++){
                    /* Little copy/paste from drawotherships ;-p. */
                    /* Set the variables for a ship. */
                    x=boats_x[b];
                    y=boats_y[b];
                    orient=boats_orient[b];
                    length=boats_sizes[b];
                    i=0;
                    while(i!=length){
                        if(orient==0){
                            player_a.boats[(y+i)*10+(x)]=2;
                        }else if(orient==1){
                            player_a.boats[y*10+(x+i)]=2;
                        }
                        i++;
                    }
                }
                ppu_off(); /* screen off. */
                clearscreen();
                write(1, 1, (unsigned char*) "Joueur 1, ne regardez pas.");
                /* Like for the player A. */
                write(1, 2, (unsigned char*) "Joueur 2, appuyez sur A pour");
                write(1, 3, (unsigned char*) "placer vos bateaux");
                ppu_on_all(); /* turn on screen. */
                game=4;
            }else if(game==7){
                shoot_x=0;
                shoot_y=0;
                drawmap(player_a.boats);
                ppu_off(); /* screen off. */
                draw_vram2(2-1, 4-1);
                clearline(1);
                clearline(2);
                clearline(3);
                clearline(4);
                write(1, 2, (unsigned char*) "--- Vos bateaux ---");
                write(1, 16, (unsigned char*) "--- Bateaux touches ---");
                displayshootpos(shoot_x, shoot_y, 2, 18, player_a.hits);
                write(16, 18, (unsigned char*) "Utilisez les");
                write(16, 19, (unsigned char*) "fleches pour");
                write(16, 20, (unsigned char*) "choisir ou");
                write(16, 21, (unsigned char*) "tirer, puis");
                write(16, 22, (unsigned char*) "appuyez sur");
                write(16, 23, (unsigned char*) "[Start] pour");
                write(16, 24, (unsigned char*) "envoyer votre");
                write(16, 25, (unsigned char*) "missile.");
                ppu_on_all();
                game=8;
            }else if(game==9){
                unsigned char i=0;
                if(player_b.boats[shoot_y*10+shoot_x]==0){
                    player_a.hits[shoot_y*10+shoot_x]=1;
                    player_b.boats[shoot_y*10+shoot_x]=1;
                }else if(player_b.boats[shoot_y*10+shoot_x]==2){
                    player_a.hits[shoot_y*10+shoot_x]=3;
                    player_b.boats[shoot_y*10+shoot_x]=3;
                }
                check=1;
                for(i=0;i!=100;i++){
                    if(player_b.boats[i]==2){
                        check=0;
                        break;
                    }
                }
                if(check){
                    drawmap(player_b.boats);
                    ppu_off();
                    draw_vram2(20-1, 9-1);
                    ppu_on_all(); /* turn on screen. */
                    drawmap(player_a.boats);
                    ppu_off();
                    draw_vram2(2-1, 9-1);
                    clearscreen();
                    write(1, 1, (unsigned char*) "LE JOUEUR 1 A GAGNE !!!");
                    write(1, 3, (unsigned char*) "Pressez [Start] pour rejouer.");
                    write(9, 5, (unsigned char*) "--- Bateaux ---");
                    write(1, 7, (unsigned char*) "--- Joueur 1 ---");
                    write(15, 7, (unsigned char*) "--- Joueur 2 ---");
                    game=13;
                }else{
                    ppu_off(); /* screen off. */
                    i=0;
                    clearscreen();
                    write(1, 1, (unsigned char*) "Joueur 1, ne regardez pas.");
                    write(1, 2, (unsigned char*) "Joueur 2, appuyez sur A pour");
                    write(1, 3, (unsigned char*) "finir le tour.");
                    if(player_a.hits[shoot_y*10+shoot_x]==1){
                        write(1, 4, (unsigned char*) "Joueur 1 a rate son tir.");
                    }else if(player_a.hits[shoot_y*10+shoot_x]==3){
                        write(1, 4, (unsigned char*) "Joueur 1 a touche.");
                    }
                    game=10;
                    ppu_on_all(); /* turn on screen. */
                }
            }
        }
        if(button&PAD_B){
            if(game==3){
                ppu_off(); /* screen off. */
                clearline(1);
                clearline(29);
                ppu_on_all(); /* turn on screen. */
                game=2;
            }else if(game==9){
                ppu_off(); /* screen off. */
                clearline(1);
                ppu_on_all(); /* turn on screen. */
                game=8;
            }
        }
        if(button&PAD_START){
            if(game == 0){ /* If game isn't started. */
                ppu_off(); /* screen off. */
                clearscreen(); /* Clear title screen. */
                write(1, 1, (unsigned char*) "Joueur 2, ne regardez pas.");
                write(1, 2, (unsigned char*) "Joueur 1, appuyez sur A pour");
                write(1, 3, (unsigned char*) "placer vos bateaux");
                ppu_on_all(); /* turn on screen. */
                game=1; /* Start game. */
            }else if(game==2){
                for(i=0;i!=100;i++){
                    boat_place_test[i]=0;
                }
                check=1;
                for(b=0;b!=5;b++){
                    /* Little copy/paste from drawotherships ;-p. */
                    /* Set the variables for a ship. */
                    x=boats_x[b];
                    y=boats_y[b];
                    orient=boats_orient[b];
                    length=boats_sizes[b];
                    /* Another copy/paste of the code of shiponmap :-D, to check if all boats have their cells. */
                    ppu_off(); /* screen off. */
                    i=0;
                    while(i!=length){
                        if(orient==0){
                            if(boat_place_test[(y+i)*10+(x)]!=1){
                                boat_place_test[(y+i)*10+(x)]=1;
                            }else{
                                clearline(1);
                                clearline(29);
                                write(5, 1, (unsigned char*) "Des bateaux se recouvrent.");
                                check=0;
                                break;
                            }
                        }else if(orient==1){
                            if(boat_place_test[y*10+(x+i)]!=1){
                                boat_place_test[y*10+(x+i)]=1;
                            }else{
                                clearline(1);
                                clearline(29);
                                write(1, 1, (unsigned char*) "Des bateaux se recouvrent.");
                                check=0;
                                break;
                            }
                        }
                        i++;
                    }
                }
                if(check==1){
                    clearline(1);
                    clearline(29);
                    write(1, 1, (unsigned char*) "Ok !");
                    write(1, 29, (unsigned char*) "Tout est bon [(A)Y/(B)n] ?");
                    game=3;
                }
                ppu_on_all(); /* turn on screen. */
            }else if(game==8){
                ppu_off(); /* screen off. */
                if(player_a.hits[shoot_y*10+shoot_x]==0){
                    write(1, 1, (unsigned char*) "Ok ! Tirer [(A)Y/(B)n] ?");
                    game=9;
                }else{
                    write(1, 1, (unsigned char*) "Vous avez deja tire ici !");
                }
                ppu_on_all(); /* turn on screen. */
            }else if(game==13){
                ppu_off(); /* screen off. */
                clearscreen();
                ppu_on_all(); /* turn on screen. */
                break;
            }
        }
        if(button&PAD_SELECT){
            if (game==2){ /* If we are on the player 1 screen to place boats. */
                if(shipnum<5){
                    shipnum++;
                } else{
                    shipnum=1;
                }
                boat_size=boats_sizes[shipnum-1];
                ppu_off(); /* screen off. */
                displayship(5, 25, shipnames, shipnum);
                ppu_on_all();
                drawships(10, 4, boats_x, boats_y, boats_orient, (unsigned char *)boats_sizes, shipnum, player_a.boats);
                ppu_on_all(); /* turn on screen. */
            }
        }
        /* Player 2 */
    }
    main();
}
