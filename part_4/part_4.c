#include <ncurses.h>
#include <stdlib.h>

typedef struct Room {
	int xPosition;
	int yPosition;
	int height;
	int width;
} Room;

typedef struct Player {
	int xPosition;
	int yPosition;
	int health;
} Player;

int screenSetUp();
Room** mapSetUp();
Player* playerSetUp();
int handleInput(int input, Player* user);
int checkPosition(int newY, int newX, Player* user);
int playerMove(int y, int x, Player* user);

//room functions
Room* createRoom(int x, int y, int height, int width);
int drawRoom(Room* room);

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

Room** mapSetUp(){
	Room** rooms;
	rooms = malloc(6*sizeof(Room));

	//(int) mvprintw (int,int, const char *,...) GCC_PRINTFLIKE(3,4);
	//mvprintw(y, x, string);

	//mvprintw(13, 13,"--------");
	//mvprintw(14, 13,"|......|");
	//mvprintw(15, 13,"|......|");
	//mvprintw(16, 13,"|......|");
	//mvprintw(17, 13,"|......|");
	//mvprintw(18, 13,"--------");

	rooms[0] = createRoom(13, 13, 6, 8);
	drawRoom(rooms[0]);

	//mvprintw(2, 40,"--------");
	//mvprintw(3, 40,"|......|");
	//mvprintw(4, 40,"|......|");
	//mvprintw(5, 40,"|......|");
	//mvprintw(6, 40,"|......|");
	//mvprintw(7, 40,"--------");

	rooms[1] = createRoom(40, 2, 6, 8);
	drawRoom(rooms[1]);

	//mvprintw(10, 40,"------------");
	//mvprintw(11, 40,"|..........|");
	//mvprintw(12, 40,"|..........|");
	//mvprintw(13, 40,"|..........|");
	//mvprintw(14, 40,"|..........|");
	//mvprintw(15, 40,"------------");

	rooms[2] = createRoom(40, 10, 6, 12);
	drawRoom(rooms[2]);

	return rooms;

}

Room* createRoom(int x, int y, int height, int width){
	Room* newRoom;
	newRoom = malloc(sizeof(Room));

	newRoom->xPosition = x;
	newRoom->yPosition = y;
	newRoom->height = height;
	newRoom->width = width;

	return newRoom;
}

int drawRoom(Room* room){
	int x;
	int y;

	//draw top and bottom
	for (x = room->xPosition; x < room->xPosition + room->width; x++)
	{
		//top
		mvprintw(room->yPosition, x, "-");
		//bottom
		mvprintw(room->yPosition + room->height - 1, x, "-");
	}

	//draw sides and floor
	for (y = room->yPosition + 1; y < room->yPosition + room->height - 1; y++)
	{   
		//draw sides
		mvprintw(y, room->xPosition, "|");
		mvprintw(y, room->xPosition + room->width - 1, "|");

		//draw the floor
		for (x = room->xPosition + 1; x < room->xPosition + room->width - 1; x++)
		{
			mvprintw(y, x, ".");
		}
	}
}

Player* playerSetUp(){
	Player* newPlayer;
	newPlayer = malloc(sizeof(Player));

	newPlayer->yPosition = 14;
	newPlayer->xPosition = 14;
	newPlayer->health = 20;


	playerMove(newPlayer->yPosition, newPlayer->xPosition, newPlayer);

	return newPlayer;
}

int handleInput(int input, Player* user){
	int newY;
	int newX;
	switch(input)
	{	
		//move up
		case 'w':
		case 'W':
			newY = user->yPosition - 1;
		    newX = user->xPosition;
			break;
		//move down
		case 's':
		case 'S':
			newY = user->yPosition + 1;
		    newX = user->xPosition;
			break;
	    //move left
		case 'a':
		case 'A':
			newY = user->yPosition;
		    newX = user->xPosition - 1;
			break;
		//move right
		case 'd':
		case 'D':
			newY = user->yPosition;
		    newX = user->xPosition + 1;
			break;

		default:
			break;	
	}

	checkPosition(newY, newX, user);
		
}

int checkPosition(int newY, int newX, Player* user){
		int space;
		//#define mvinch(y,x)			mvwinch(stdscr,(y),(x))
		switch(mvinch(newY, newX))
		{
			case '.':
				playerMove(newY, newX, user);
				break;
			default:
				move(user->yPosition, user->xPosition);
				break;
		}
}

int playerMove(int y, int x, Player* user){

	mvprintw(user->yPosition, user->xPosition, ".");
	user->yPosition = y;
	user->xPosition = x;
	mvprintw(user->yPosition, user->xPosition, "@");
	move(user->yPosition, user->xPosition);
}