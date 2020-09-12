#include <ncurses.h>
#include <stdlib.h>

typedef struct Player {
	int xPosition;
	int yPosition;
	int health;
} Player;

int screenSetUp();
int mapSetUp();
Player* playerSetUp();
int handleInput(int input, Player* hager);
int playerMove(int y, int x, Player* hager);

int main()
{
	int ch;
	Player* user;
	user = playerSetUp();

	screenSetUp();

	mapSetUp();

	playerSetUp();

	//#define getch()			wgetch(stdscr)
	//(int) wgetch (WINDOW *);
	while((ch = getch()) != 'q'){
		handleInput(ch, user);
	}
	//(int) NCURSES_SP_NAME(endwin) (SCREEN*);
	endwin();
	return 0;
}

int screenSetUp(){
	//(WINDOW *) initscr (void);
	initscr();
	//(int) printw (const char *,...)
	printw ("Hello World");
	//(int) NCURSES_SP_NAME(noecho) (SCREEN*);
	noecho();
	//#define refresh()		wrefresh(stdscr)
	//(int) wrefresh (WINDOW *);
	refresh();

	return 0;
}

int mapSetUp(){
	//(int) mvprintw (int,int, const char *,...) GCC_PRINTFLIKE(3,4);
	//mvprintw(y, x, string);
	mvprintw(13, 13,"--------");
	mvprintw(14, 13,"|------|");
	mvprintw(15, 13,"|------|");
	mvprintw(16, 13,"|------|");
	mvprintw(17, 13,"|------|");
	mvprintw(18, 13,"--------");

	mvprintw(2, 40,"--------");
	mvprintw(3, 40,"|------|");
	mvprintw(4, 40,"|------|");
	mvprintw(5, 40,"|------|");
	mvprintw(6, 40,"|------|");
	mvprintw(7, 40,"--------");

	mvprintw(10, 40,"------------");
	mvprintw(11, 40,"|----------|");
	mvprintw(12, 40,"|----------|");
	mvprintw(13, 40,"|----------|");
	mvprintw(14, 40,"|----------|");
	mvprintw(15, 40,"------------");
}

Player* playerSetUp(){
	Player* newPlayer;
	newPlayer = malloc(sizeof(Player));

	newPlayer->yPosition = 14;
	newPlayer->xPosition = 14;
	newPlayer->health = 20;

	mvprintw(newPlayer->yPosition, newPlayer->xPosition, "@");

	//#define move(y,x)		wmove(stdscr,(y),(x))
	move(newPlayer->yPosition,newPlayer->xPosition);

	return newPlayer;
}

int handleInput(int input, Player* hager){
	switch(input)
	{	
		//move up
		case 'w':
		case 'W':
			playerMove(hager->yPosition - 1, hager->xPosition, hager);
			break;
		//move down
		case 's':
		case 'S':
			playerMove(hager->yPosition + 1, hager->xPosition, hager);
			break;
	    //move left
		case 'a':
		case 'A':
			playerMove(hager->yPosition, hager->xPosition - 1, hager);
			break;
		//move right
		case 'd':
		case 'D':
			playerMove(hager->yPosition, hager->xPosition + 1, hager);
			break;

		default:
			break;	
	}
}

int playerMove(int y, int x, Player* hager){

	mvprintw(hager->yPosition, hager->xPosition, ".");
	hager->yPosition = y;
	hager->xPosition = x;
	mvprintw(hager->yPosition, hager->xPosition, ".");
	move(hager->yPosition,hager->xPosition);
}