#include <ncurses.h>
#include <stdlib.h>
#include <time.h>

typedef struct Position {
	int x;
	int y;
} Position;

typedef struct Room {
	Position position;
	int height;
	int width;
	Position** doors;
} Room;

typedef struct Player {
	Position position;
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
int connectDoors(Position* doorOne, Position* doorTwo);

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

	srand(time(NULL));

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

	connectDoors(rooms[0]->doors[3], rooms[2]->doors[1]);
	connectDoors(rooms[1]->doors[2], rooms[0]->doors[0]);

	return rooms;

}

Room* createRoom(int x, int y, int height, int width){
	Room* newRoom;
	newRoom = malloc(sizeof(Room));

	newRoom->position.x = x;
	newRoom->position.y = y;
	newRoom->height = height;
	newRoom->width = width;

	newRoom->doors = malloc(4*sizeof(Position));
	//top door
	newRoom->doors[0] = malloc(sizeof(Position));
	newRoom->doors[0]->x = rand() % (width - 2) + newRoom->position.x +1;
	newRoom->doors[0]->y = newRoom->position.y;
	//left door
	newRoom->doors[1] = malloc(sizeof(Position));
	newRoom->doors[1]->y = rand() % (height - 2) + newRoom->position.y +1;
	newRoom->doors[1]->x = newRoom->position.x;
	//bottom door
	newRoom->doors[2] = malloc(sizeof(Position));
	newRoom->doors[2]->x = rand() % (width - 2) + newRoom->position.x +1;
	newRoom->doors[2]->y = newRoom->position.y + newRoom->height - 1;
	//right door
	newRoom->doors[3] = malloc(sizeof(Position));
	newRoom->doors[3]->y = rand() % (height - 2) + newRoom->position.y +1;
	newRoom->doors[3]->x = newRoom->position.x + newRoom->width - 1;

	return newRoom;
}

int drawRoom(Room* room){
	int x;
	int y;

	//draw top and bottom
	for (x = room->position.x; x < room->position.x + room->width; x++)
	{
		//top
		mvprintw(room->position.y, x, "-");
		//bottom
		mvprintw(room->position.y + room->height - 1, x, "-");
	}

	//draw sides and floor
	for (y = room->position.y + 1; y < room->position.y + room->height - 1; y++)
	{   
		//draw sides
		mvprintw(y, room->position.x, "|");
		mvprintw(y, room->position.x + room->width - 1, "|");

		//draw the floor
		for (x = room->position.x + 1; x < room->position.x + room->width - 1; x++)
		{
			mvprintw(y, x, ".");
		}
	}

	//draw doors
	mvprintw(room->doors[0]->y, room->doors[0]->x, "+");
	mvprintw(room->doors[1]->y, room->doors[1]->x, "+");
	mvprintw(room->doors[2]->y, room->doors[2]->x, "+");
	mvprintw(room->doors[3]->y, room->doors[3]->x, "+");
}

int connectDoors(Position* doorOne, Position* doorTwo){

 	Position temp;
 	Position previous;

 	int count = 0;

 	temp.x = doorOne->x;
 	temp.y = doorOne->y;
 	while(1){
 		if ((abs((temp.x-1)-doorTwo->x) < abs(temp.x-doorTwo->x) ) &&  (mvinch(temp.y, temp.x-1) == ' '))
 		{
 			previous.x = temp.x;
 			temp.x = temp.x - 1;

 		}else if((abs((temp.x + 1)-doorTwo->x) < abs(temp.x - doorTwo->x) ) &&  (mvinch(temp.y, temp.x + 1) == ' ')){
 			
 			previous.x = temp.x;
 			temp.x = temp.x + 1;
 		}else if((abs((temp.y + 1)-doorTwo->y) < abs(temp.y - doorTwo->y) ) &&  (mvinch(temp.y + 1, temp.x) == ' ')){
 			
 			previous.y = temp.y;
 			temp.y = temp.y + 1;
 		}else if((abs((temp.y - 1)-doorTwo->y) < abs(temp.y - doorTwo->y) ) &&  (mvinch(temp.y - 1, temp.x) == ' ')){
 			
 			previous.y = temp.y;
 			temp.y = temp.y - 1;
 		}else{
 			if (count == 0)
 			{
 				temp = previous;
 				count++;
 				continue;

 			}else{
 				return 0;
 		    }
 		}

 		mvprintw(temp.y, temp.x, "#");	
 		
 		getch();
 	}

 	return 1;
}

Player* playerSetUp(){
	Player* newPlayer;
	newPlayer = malloc(sizeof(Player));

	newPlayer->position.y = 14;
	newPlayer->position.x = 14;
	newPlayer->health = 20;


	playerMove(newPlayer->position.y, newPlayer->position.x, newPlayer);

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
			newY = user->position.y - 1;
		    newX = user->position.x;
			break;
		//move down
		case 's':
		case 'S':
			newY = user->position.y + 1;
		    newX = user->position.x;
			break;
	    //move left
		case 'a':
		case 'A':
			newY = user->position.y;
		    newX = user->position.x - 1;
			break;
		//move right
		case 'd':
		case 'D':
			newY = user->position.y;
		    newX = user->position.x + 1;
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
			case '+':
			case '#':
				playerMove(newY, newX, user);
				break;
			default:
				move(user->position.y, user->position.x);
				break;
		}
}

int playerMove(int y, int x, Player* user){

	mvprintw(user->position.y, user->position.x, ".");
	user->position.y = y;
	user->position.x = x;
	mvprintw(user->position.y, user->position.x, "@");
	move(user->position.y, user->position.x);
}