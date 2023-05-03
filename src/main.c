/*	NesLibShipBattle
 *	by mibi88
 *	based on the "Hello 
 *	World !" example
 *	using neslib
 *	by Doug Fraker in 2018
 */	
/*	Reminders
 *	-done- clear boats x, y and orient for player 2.
 *	let the players replay.
 */
 
 
#include <neslib.h>
#include <nesdoug.h>

#define BLACK 0x0f
#define DK_GY 0x00
#define LT_GY 0x10
#define WHITE 0x30
// there's some oddities in the palette code, black must be 0x0f, white must be 0x30.

#pragma bss-name(push, "ZEROPAGE")

// GLOBAL VARIABLES
// all variables should be global for speed,
// zeropage global is even faster.

unsigned char i;

const unsigned char palette[] = 
{
	WHITE, BLACK, BLACK, BLACK,
	0,0,0,0,
	0,0,0,0,
	0,0,0,0
}; 
unsigned char button;
int game;
int player_a_boats[100] = 
{
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0
};
int player_a_hits[100] = 
{
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0
};
int player_b_boats[100] = 
{
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0
};
int player_b_hits[100] = 
{
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0
};
int shipnum=1;
const unsigned char** shipnames[5] =
{
	(const unsigned char**) "porte avion",
	(const unsigned char**) "croiseur",
	(const unsigned char**) "contre-torpilleur",
	(const unsigned char**) "contre-torpilleur 2",
	(const unsigned char**) "torpilleur"
};
const int boats_sizes[5] = // 0 = vertical, 1 = horizontal.
{
	5,
	4,
	3,
	3,
	2
};
int boats_x[5] = // Y coordinates of the boats for select menu.
{
	0,
	0,
	0,
	0,
	0
};
int boats_y[5] = // Y coordinates of the boats for select menu.
{
	0,
	0,
	0,
	0,
	0
};
int boats_orient[5] = // 0 = vertical, 1 = horizontal.
{
	0,
	0,
	0,
	0,
	0
};
int boat_size = 5;
int boat_place_test[100] = 
{
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0
};
int x, y, orient, lenght, b, check, shoot_x, shoot_y;
void write (int x, int y, unsigned char* text) {
	// ppu_off(); // screen off.
	// set a starting point on the screen.
	// vram_adr(NTADR_A(x,y));
	vram_adr(NTADR_A(x,y)); // screen is 32 x 30 tiles.

	i = 0;
	while(text[i]){
		vram_put(text[i]); // this pushes 1 char to the screen.
		++i;
	}	
	
	// vram_adr and vram_put only work with screen off.
	// NOTE, you could replace everything between i = 0; and here with...
	// vram_write(text,sizeof(text));
	// does the same thing.
	// ppu_on_all(); // turn on screen.
}
void clearline (int y) {
	write(0, y, (unsigned char*) "                                ");
}
void clearscreen (void) {
	int i=0;
	while(i!=30){
		clearline(i);
		i++;
	}
}
void displayship (int x, int y, const unsigned char*** const shipnames, int shipnum) { // Display selected boat.
	clearline(y);
	write(x, y, (unsigned char*) shipnames[shipnum-1]);
}
void drawmap (int sx, int sy, int map[]) { // sx and sy are the start positions.
	int y=0;
	int x=0;
	int mapnum;
	unsigned char* mapitem;
	if(sy>0){
		write(sx-1, sy-1, (unsigned char*) "I");
		write(sx+10, sy-1, (unsigned char*) "I");
		for(x=0;x!=10;x++){
			write(sx+x, sy-1, (unsigned char*) "-");
			write(sx+x, sy+10, (unsigned char*) "-");
		}
	}
	for(y=0;y!=10;y++){
		if(sx>0){
			write(sx-1, sy+y, (unsigned char*) "I");
			write(sx+10, sy+y, (unsigned char*) "I");
		}
		for(x=0;x!=10;x++){
			mapnum=map[y*10+x];
			mapitem=(unsigned char*) "."; // Water cell.
			if(mapnum==0){
				mapitem=(unsigned char*) "."; // Water cell.
			} else if(mapnum==1){
				mapitem=(unsigned char*) "o"; // Shoot in the water cell.
			} else if(mapnum==2){
				mapitem=(unsigned char*) "B"; // Intact boat cell.
			} else if(mapnum==3){
				mapitem=(unsigned char*) "D"; // Destroyed boat cell.
			}
			write(sx+x, sy+y, mapitem);
		}
	}
	write(sx-1, sy+10, (unsigned char*) "I");
	write(sx+10, sy+10, (unsigned char*) "I");
}
void drawotherships (int sx, int sy, int ships_x[], int ships_y[], int ships_orient[], int ships_lenghts[], int notshipnum, int map[]) {
	// Variables for a boat.
	int x, y, orient, lenght, i;
	// Variable for the loop.
	int b;
	// Draw anything.
	drawmap(sx, sy, map);
	for(b=0;b!=5;b++){
		if(b!=notshipnum-1){ // We do not want to draw the ship the user is moving.
			// Set the variables for a ship.
			x=ships_x[b];
			y=ships_y[b];
			orient=ships_orient[b];
			lenght=ships_lenghts[b];
			// A little copy/paste of the code of shiponmap :-D, but drawing "O"'s.
			i=0;
			while(i!=lenght){
				if(orient==0){
					write(sx+x, sy+y+i, (unsigned char*) "O");
				}else if(orient==1){
					write(sx+x+i, sy+y, (unsigned char*) "O");
				}
				i++;
			}
		}
	}
}
void shiponmap (int sx, int sy, int x, int y, int orient, int lenght, int map[], int shipnum) { // Display the ship player a will be moving.
	int i;
	// sx and sy are the start positions.
	/* drawmap(sx, sy, map); // already in drawotherships function.
	ppu_off(); // screen off. */
	drawotherships (sx, sy, boats_x, boats_y, boats_orient, (int *) boats_sizes, shipnum, map);
	i=0;
	while(i!=lenght){
		if(orient==0){
			write(sx+x, sy+y+i, (unsigned char*) "X");
		}else if(orient==1){
			write(sx+x+i, sy+y, (unsigned char*) "X");
		}
		i++;
	}
}
void displayshootpos (int x, int y, int sx, int sy, int map[]) {
	drawmap(sx, sy, map);
	write(sx+x, sy+y, (unsigned char*) "X");
}
int main (void) {
	game=0;
	shipnum=1;
	boat_size=boats_sizes[shipnum-1];
	for(i=0;i!=100;i++){
		player_a_boats[i] = 0;
		player_a_hits[i] = 0;
		player_b_boats[i] = 0;
		player_b_hits[i] = 0;
	}

	ppu_off(); // screen off.

	pal_bg((const char*) palette); //	load the BG palette.
	
	write(7, 5, (unsigned char*) "NeslibShipBattle");
	write(8, 25, (unsigned char*) "Pressez [Start]");
	write(11, 15, (unsigned char*) "fait avec");
	write(1, 16, (unsigned char*) "la neslib modifiee de nesdoug.");
	write(1, 29, (unsigned char*) "(c) 2022 mibi88");
	write(26, 29, (unsigned char*) "v.1.1");
	
	ppu_on_all(); // turn on screen.
	
	while (1){
		// infinite loop.
		ppu_wait_frame();
		/* ********** PLAYER 1 ********** */
		button=pad_trigger(0);
		if(button&PAD_LEFT){
			// write(10, 14, (unsigned char*) "LEFT  ");
			if(game==2){
				if(boats_x[shipnum-1]>0){
					boats_x[shipnum-1]--;
				}
				ppu_off(); // screen off.
				shiponmap(10, 4, boats_x[shipnum-1], boats_y[shipnum-1], boats_orient[shipnum-1], (int) boats_sizes[shipnum-1], player_a_boats, shipnum);
				ppu_on_all(); // turn on screen.
			}else if(game==8){
				if(shoot_x>0){
					shoot_x--;
					ppu_off(); // screen off.
					displayshootpos(shoot_x, shoot_y, 2, 18, player_a_hits);
					ppu_on_all(); // turn on screen.
				}
			}
		}
		if(button&PAD_RIGHT){
			// write(10, 14, (unsigned char*) "RIGHT ");
			if(game==2){
				if(boats_orient[shipnum-1]==0){
					if(boats_x[shipnum-1]<9){
						boats_x[shipnum-1]++;
					}
				}else{
					if(boats_x[shipnum-1]<10-(int) boats_sizes[shipnum-1]){
						boats_x[shipnum-1]++;
					}
				}
				ppu_off(); // screen off.
				shiponmap(10, 4, boats_x[shipnum-1], boats_y[shipnum-1], boats_orient[shipnum-1], (int) boats_sizes[shipnum-1], player_a_boats, shipnum);
				ppu_on_all(); // turn on screen.
			}else if(game==8){
				if(shoot_x<9){
					shoot_x++;
					ppu_off(); // screen off.
					displayshootpos(shoot_x, shoot_y, 2, 18, player_a_hits);
					ppu_on_all(); // turn on screen.
				}
			}
		}
		if(button&PAD_UP){
			// write(10, 14, (unsigned char*) "UP    ");
			if(game==2){
				if(boats_y[shipnum-1]>0){
					boats_y[shipnum-1]--;
				}
				ppu_off(); // screen off.
				shiponmap(10, 4, boats_x[shipnum-1], boats_y[shipnum-1], boats_orient[shipnum-1], (int) boats_sizes[shipnum-1], player_a_boats, shipnum);
				ppu_on_all(); // turn on screen.
			}else if(game==8){
				if(shoot_y>0){
					shoot_y--;
					ppu_off(); // screen off.
					displayshootpos(shoot_x, shoot_y, 2, 18, player_a_hits);
					ppu_on_all(); // turn on screen.
				}
			}
		}
		if(button&PAD_DOWN){
			// write(10, 14, (unsigned char*) "DOWN  ");
			if(game==2){
				if(boats_orient[shipnum-1]==0){
					if(boats_y[shipnum-1]<10-(int) boats_sizes[shipnum-1]){
						boats_y[shipnum-1]++;
					}
				}else{
					if(boats_y[shipnum-1]<9){
						boats_y[shipnum-1]++;
					}
				}
				ppu_off(); // screen off.
				shiponmap(10, 4, boats_x[shipnum-1], boats_y[shipnum-1], boats_orient[shipnum-1], (int) boats_sizes[shipnum-1], player_a_boats, shipnum);
				ppu_on_all(); // turn on screen.
			}else if(game==8){
				if(shoot_y<9){
					shoot_y++;
					ppu_off(); // screen off.
					displayshootpos(shoot_x, shoot_y, 2, 18, player_a_hits);
					ppu_on_all(); // turn on screen.
				}
			}
		}
		if(button&PAD_A){
			// write(10, 14, (unsigned char*) "A     ");
			if(game==1){
				shipnum=1;
				for(i=0;i!=5;i++){
					boats_x[i]=0;
					boats_y[i]=0;
					boats_orient[i]=0;
				}
				ppu_off(); // screen off.
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
				shiponmap(10, 4, boats_x[shipnum-1], boats_y[shipnum-1], boats_orient[shipnum-1], (int) boats_sizes[shipnum-1], player_a_boats, shipnum);
				ppu_on_all(); // turn on screen.
				game=2; // Let the player 1 place his boats.
			}else if(game==2){
				if(boats_orient[shipnum-1]==0 && boats_x[shipnum-1]<11-(int) boats_sizes[shipnum-1]){
					boats_orient[shipnum-1]=1;
				}else if(boats_orient[shipnum-1]==1 && boats_y[shipnum-1]<11-(int) boats_sizes[shipnum-1]){
					boats_orient[shipnum-1]=0;
				}
				ppu_off(); // screen off.
				shiponmap(10, 4, boats_x[shipnum-1], boats_y[shipnum-1], boats_orient[shipnum-1], (int) boats_sizes[shipnum-1], player_a_boats, shipnum);
				ppu_on_all(); // turn on screen.
			}
			if(game==3){ // Put the boats on the grid.
				int i=0;
				for(b=0;b!=5;b++){
					// Little copy/paste from drawotherships ;-p.
					// Set the variables for a ship.
					x=boats_x[b];
					y=boats_y[b];
					orient=boats_orient[b];
					lenght=boats_sizes[b];
					i=0;
					while(i!=lenght){
						if(orient==0){
							player_a_boats[(y+i)*10+(x)]=2;
						}else if(orient==1){
							player_a_boats[y*10+(x+i)]=2;
						}
						i++;
					}
				}
				ppu_off(); // screen off.
				clearscreen();
				write(1, 1, (unsigned char*) "Joueur 1, ne regardez pas."); // Like for the player A.
				write(1, 2, (unsigned char*) "Joueur 2, appuyez sur A pour");
				write(1, 3, (unsigned char*) "placer vos bateaux");
				ppu_on_all(); // turn on screen.
				game=4;
			}else if(game==7){
				shoot_x=0;
				shoot_y=0;
				ppu_off(); // screen off.
				clearline(1);
				clearline(2);
				clearline(3);
				clearline(4);
				write(1, 2, (unsigned char*) "--- Vos bateaux ---");
				drawmap(2, 4, player_a_boats);
				write(1, 16, (unsigned char*) "--- Bateaux touches ---");
				displayshootpos(shoot_x, shoot_y, 2, 18, player_a_hits);
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
				int i=0;
				if(player_b_boats[shoot_y*10+shoot_x]==0){
					player_a_hits[shoot_y*10+shoot_x]=1;
					player_b_boats[shoot_y*10+shoot_x]=1;
				}else if(player_b_boats[shoot_y*10+shoot_x]==2){
					player_a_hits[shoot_y*10+shoot_x]=3;
					player_b_boats[shoot_y*10+shoot_x]=3;
				}
				check=1;
				for(i=0;i!=100;i++){
					if(player_b_boats[i]==2){
						check=0;
						break;
					}
				}
				ppu_off(); // screen off.
				i=0;
				clearscreen();
				write(1, 1, (unsigned char*) "Joueur 1, ne regardez pas.");
				write(1, 2, (unsigned char*) "Joueur 2, appuyez sur A pour");
				write(1, 3, (unsigned char*) "finir le tour.");
				if(player_a_hits[shoot_y*10+shoot_x]==1){
					write(1, 4, (unsigned char*) "Joueur 1 a rate son tir.");
				}else if(player_a_hits[shoot_y*10+shoot_x]==3){
					write(1, 4, (unsigned char*) "Joueur 1 a touche.");
				}
				game=10;
				if(check==1){
					clearscreen();
					write(1, 1, (unsigned char*) "LE JOUEUR 1 A GAGNE !!!");
					write(1, 3, (unsigned char*) "Pressez [Start] pour rejouer.");
					write(9, 5, (unsigned char*) "--- Bateaux ---");
					write(1, 7, (unsigned char*) "--- Joueur 1 ---");
					drawmap(2, 9, player_a_boats);
					write(15, 7, (unsigned char*) "--- Joueur 2 ---");
					drawmap(20, 9, player_b_boats);
					//
					game=13;
				}
				ppu_on_all(); // turn on screen.
			}
		}
		if(button&PAD_B){
			// write(10, 14, (unsigned char*) "B     ");
			if(game==3){
				ppu_off(); // screen off.
				clearline(1);
				clearline(29);
				ppu_on_all(); // turn on screen.
				game=2;
			}else if(game==9){
				ppu_off(); // screen off.
				clearline(1);
				ppu_on_all(); // turn on screen.
				game=8;
			}
		}
		if(button&PAD_START){
			// write(10, 14, (unsigned char*) "START ");
			if(game == 0){ // If game isn't started.
				ppu_off(); // screen off.
				clearscreen(); // Clear title screen.
				write(1, 1, (unsigned char*) "Joueur 2, ne regardez pas.");
				write(1, 2, (unsigned char*) "Joueur 1, appuyez sur A pour");
				write(1, 3, (unsigned char*) "placer vos bateaux");
				ppu_on_all(); // turn on screen.
				game=1; // Start game.
			}else if(game==2){
				for(i=0;i!=100;i++){
					boat_place_test[i]=0;
				}
				check=1;
				for(b=0;b!=5;b++){
					// Little copy/paste from drawotherships ;-p.
					// Set the variables for a ship.
					x=boats_x[b];
					y=boats_y[b];
					orient=boats_orient[b];
					lenght=boats_sizes[b];
					// Another copy/paste of the code of shiponmap :-D, to check if all boats have their cells.
					ppu_off(); // screen off.
					i=0;
					while(i!=lenght){
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
				ppu_on_all(); // turn on screen.
			}else if(game==8){
				ppu_off(); // screen off.
				if(player_a_hits[shoot_y*10+shoot_x]==0){
					write(1, 1, (unsigned char*) "Ok ! Tirer [(A)Y/(B)n] ?");
					game=9;
				}else{
					write(1, 1, (unsigned char*) "Vous avez deja tire ici !");
				}
				ppu_on_all(); // turn on screen.
			}else if(game==13){
				ppu_off(); // screen off.
				clearscreen();
				ppu_on_all(); // turn on screen.
				break;
			}
		}
		if(button&PAD_SELECT){
			// write(10, 14, (unsigned char*) "SELECT");
			if (game==2){ // If we are on the player 1 screen to place boats.
				if(shipnum<5){
					shipnum++;
				} else{
					shipnum=1;
				}
				boat_size=boats_sizes[shipnum-1];
				ppu_off(); // screen off.
				displayship(5, 25, shipnames, shipnum);
				shiponmap(10, 4, boats_x[shipnum-1], boats_y[shipnum-1], boats_orient[shipnum-1], (int) boats_sizes[shipnum-1], player_a_boats, shipnum);
				ppu_on_all(); // turn on screen.
			}
		}
		/* ********** PLAYER 2 ********** */
		button=pad_trigger(1);
		if(button&PAD_LEFT){
			// write(10, 14, (unsigned char*) "LEFT  ");
			if(game==5){
				if(boats_x[shipnum-1]>0){
					boats_x[shipnum-1]--;
				}
				ppu_off(); // screen off.
				shiponmap(10, 4, boats_x[shipnum-1], boats_y[shipnum-1], boats_orient[shipnum-1], (int) boats_sizes[shipnum-1], player_b_boats, shipnum);
				ppu_on_all(); // turn on screen.
			}else if(game==11){
				if(shoot_x>0){
					shoot_x--;
					ppu_off(); // screen off.
					displayshootpos(shoot_x, shoot_y, 2, 18, player_b_hits);
					ppu_on_all(); // turn on screen.
				}
			}
		}
		if(button&PAD_RIGHT){
			// write(10, 14, (unsigned char*) "RIGHT ");
			if(game==5){
				if(boats_orient[shipnum-1]==0){
					if(boats_x[shipnum-1]<9){
						boats_x[shipnum-1]++;
					}
				}else{
					if(boats_x[shipnum-1]<10-(int) boats_sizes[shipnum-1]){
						boats_x[shipnum-1]++;
					}
				}
				ppu_off(); // screen off.
				shiponmap(10, 4, boats_x[shipnum-1], boats_y[shipnum-1], boats_orient[shipnum-1], (int) boats_sizes[shipnum-1], player_b_boats, shipnum);
				ppu_on_all(); // turn on screen.
			}else if(game==11){
				if(shoot_x<9){
					shoot_x++;
					ppu_off(); // screen off.
					displayshootpos(shoot_x, shoot_y, 2, 18, player_b_hits);
					ppu_on_all(); // turn on screen.
				}
			}
		}
		if(button&PAD_UP){
			// write(10, 14, (unsigned char*) "UP    ");
			if(game==5){
				if(boats_y[shipnum-1]>0){
					boats_y[shipnum-1]--;
				}
				ppu_off(); // screen off.
				shiponmap(10, 4, boats_x[shipnum-1], boats_y[shipnum-1], boats_orient[shipnum-1], (int) boats_sizes[shipnum-1], player_b_boats, shipnum);
				ppu_on_all(); // turn on screen.
			}else if(game==11){
				if(shoot_y>0){
					shoot_y--;
					ppu_off(); // screen off.
					displayshootpos(shoot_x, shoot_y, 2, 18, player_b_hits);
					ppu_on_all(); // turn on screen.
				}
			}
		}
		if(button&PAD_DOWN){
			// write(10, 14, (unsigned char*) "DOWN  ");
			if(game==5){
				if(boats_orient[shipnum-1]==0){
					if(boats_y[shipnum-1]<10-(int) boats_sizes[shipnum-1]){
						boats_y[shipnum-1]++;
					}
				}else{
					if(boats_y[shipnum-1]<9){
						boats_y[shipnum-1]++;
					}
				}
				ppu_off(); // screen off.
				shiponmap(10, 4, boats_x[shipnum-1], boats_y[shipnum-1], boats_orient[shipnum-1], (int) boats_sizes[shipnum-1], player_b_boats, shipnum);
				ppu_on_all(); // turn on screen.
			}else if(game==11){
				if(shoot_y<9){
					shoot_y++;
					ppu_off(); // screen off.
					displayshootpos(shoot_x, shoot_y, 2, 18, player_b_hits);
					ppu_on_all(); // turn on screen.
				}
			}
		}
		if(button&PAD_A){
			// write(10, 14, (unsigned char*) "A     ");
			if(game==4){
				shipnum=1;
				for(i=0;i!=5;i++){
					boats_x[i]=0;
					boats_y[i]=0;
					boats_orient[i]=0;
				}
				ppu_off(); // screen off.
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
				shiponmap(10, 4, boats_x[shipnum-1], boats_y[shipnum-1], boats_orient[shipnum-1], (int) boats_sizes[shipnum-1], player_b_boats, shipnum);
				ppu_on_all(); // turn on screen.
				game=5; // Let the player 1 place his boats.
			}else if(game==5){
				if(boats_orient[shipnum-1]==0 && boats_x[shipnum-1]<11-(int) boats_sizes[shipnum-1]){
					boats_orient[shipnum-1]=1;
				}else if(boats_orient[shipnum-1]==1 && boats_y[shipnum-1]<11-(int) boats_sizes[shipnum-1]){
					boats_orient[shipnum-1]=0;
				}
				ppu_off(); // screen off.
				shiponmap(10, 4, boats_x[shipnum-1], boats_y[shipnum-1], boats_orient[shipnum-1], (int) boats_sizes[shipnum-1], player_b_boats, shipnum);
				ppu_on_all(); // turn on screen.
			}
			if(game==6){
				int i=0;
				for(b=0;b!=5;b++){
					// Little copy/paste from drawotherships ;-p.
					// Set the variables for a ship.
					x=boats_x[b];
					y=boats_y[b];
					orient=boats_orient[b];
					lenght=boats_sizes[b];
					i=0;
					while(i!=lenght){
						if(orient==0){
							player_b_boats[(y+i)*10+(x)]=2;
						}else if(orient==1){
							player_b_boats[y*10+(x+i)]=2;
						}
						i++;
					}
				}
				ppu_off(); // screen off.
				clearscreen();
				write(1, 1, (unsigned char*) "Joueur 2, ne regardez pas.");
				write(1, 2, (unsigned char*) "Joueur 1, appuyez sur A pour");
				write(1, 3, (unsigned char*) "debuter le tour.");
				ppu_on_all(); // turn on screen.
				game=7;
			}else if(game==10){
				shoot_x=0;
				shoot_y=0;
				ppu_off(); // screen off.
				clearline(1);
				clearline(2);
				clearline(3);
				clearline(4);
				write(1, 2, (unsigned char*) "--- Vos bateaux ---");
				drawmap(2, 4, player_b_boats);
				write(1, 16, (unsigned char*) "--- Bateaux touches ---");
				displayshootpos(shoot_x, shoot_y, 2, 18, player_b_hits);
				write(16, 18, (unsigned char*) "Utilisez les");
				write(16, 19, (unsigned char*) "fleches pour");
				write(16, 20, (unsigned char*) "choisir ou");
				write(16, 21, (unsigned char*) "tirer, puis");
				write(16, 22, (unsigned char*) "appuyez sur");
				write(16, 23, (unsigned char*) "[Start] pour");
				write(16, 24, (unsigned char*) "envoyer votre");
				write(16, 25, (unsigned char*) "missile.");
				ppu_on_all();
				game=11;
			}else if(game==12){
				int i=0;
				int check=1;
				if(player_a_boats[shoot_y*10+shoot_x]==0){
					player_b_hits[shoot_y*10+shoot_x]=1;
					player_a_boats[shoot_y*10+shoot_x]=1;
				}else if(player_a_boats[shoot_y*10+shoot_x]==2){
					player_b_hits[shoot_y*10+shoot_x]=3;
					player_a_boats[shoot_y*10+shoot_x]=3;
				}
				check=1;
				for(i=0;i!=100;i++){
					if(player_a_boats[i]==2){
						check=0;
						break;
					}
				}
				ppu_off(); // screen off.
				clearscreen();
				write(1, 1, (unsigned char*) "Joueur 2, ne regardez pas.");
				write(1, 2, (unsigned char*) "Joueur 1, appuyez sur A pour");
				write(1, 3, (unsigned char*) "debuter le tour.");
				if(player_b_hits[shoot_y*10+shoot_x]==1){
					write(1, 4, (unsigned char*) "Joueur 2 a rate son tir.");
				}else if(player_b_hits[shoot_y*10+shoot_x]==3){
					write(1, 4, (unsigned char*) "Joueur 2 a touche.");
				}
				game=7;
				if(check==1){
					i=0;
					while(i!=30){
						write(0, i, (unsigned char*) "                                "); // A copy/paste of clearline ;-p !!!
						i++;
					}
					write(1, 1, (unsigned char*) "LE JOUEUR 2 A GAGNE !!!");
					write(1, 3, (unsigned char*) "Pressez [Start] pour rejouer.");
					write(9, 5, (unsigned char*) "--- Bateaux ---");
					write(1, 7, (unsigned char*) "--- Joueur 2 ---");
					drawmap(2, 9, player_b_boats);
					write(15, 7, (unsigned char*) "--- Joueur 1 ---");
					drawmap(20, 9, player_a_boats);
					game=13;
				}
				ppu_on_all(); // turn on screen.
			}
		}
		if(button&PAD_B){
			// write(10, 14, (unsigned char*) "B     ");
			if(game==6){
				ppu_off(); // screen off.
				clearline(1);
				clearline(29);
				ppu_on_all(); // turn on screen.
				game=5;
			}else if(game==12){
				ppu_off(); // screen off.
				clearline(1);
				ppu_on_all(); // turn on screen.
				game=11;
			}
		}
		if(button&PAD_START){
			// write(10, 14, (unsigned char*) "START ");
			if(game==5){
				ppu_off(); // screen off.
				for(i=0;i!=100;i++){
					boat_place_test[i]=0;
				}
				check=1;
				for(b=0;b!=5;b++){
					// Little copy/paste from drawotherships ;-p.
					// Set the variables for a ship.
					x=boats_x[b];
					y=boats_y[b];
					orient=boats_orient[b];
					lenght=boats_sizes[b];
					// Another copy/paste of the code of shiponmap :-D, to check if all boats have their cells.
					i=0;
					while(i!=lenght){
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
					game=6;
				}
				ppu_on_all(); // turn on screen.
			}else if(game==11){
				ppu_off(); // screen off.
				if(player_b_hits[shoot_y*10+shoot_x]==0){
					write(1, 1, (unsigned char*) "Ok ! Tirer [(A)Y/(B)n] ?");
					game=12;
				}else{
					write(1, 1, (unsigned char*) "Vous avez deja tire ici !");
				}
				ppu_on_all(); // turn on screen.
			}
		}
		if(button&PAD_SELECT){
			// write(10, 14, (unsigned char*) "SELECT");
			if (game==5){ // If we are on the player 1 screen to place boats.
				if(shipnum<5){
					shipnum++;
				} else{
					shipnum=1;
				}
				boat_size=boats_sizes[shipnum-1];
				ppu_off(); // screen off.
				displayship(5, 25, shipnames, shipnum);
				shiponmap(10, 4, boats_x[shipnum-1], boats_y[shipnum-1], boats_orient[shipnum-1], (int) boats_sizes[shipnum-1], player_b_boats, shipnum);
				ppu_on_all(); // turn on screen.
			}
		}
	}
	main();
}


