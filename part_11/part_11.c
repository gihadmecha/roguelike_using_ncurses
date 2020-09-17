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

typedef struct Level {
	char** tiles;
	int level;
	int numberOfRooms;
	struct Room** rooms;
} Level;


int screenSetUp();

// level/map functions
Level* createLevel(int level);
Room** roomsSetUp();
char** saveLevelPositions();

// player functions
Player* playerSetUp();
Position* handleInput(int input, Player* user);
int checkPosition(Position* newPosition, Player* user, char** level);
int playerMove(Position* newPosition, Player* user, char** level);

//room functions
Room* createRoom(int x, int y, int height, int width);
int drawRoom(Room* room);
int connectDoors(Position* doorOne, Position* doorTwo);

int main()
{
	int ch;
	Player* user;
	Level* level;
	Position* newPosition;

	user = playerSetUp();

	screenSetUp();

	level = createLevel(1);

	playerSetUp();

	//#define getch()			wgetch(stdscr)
	//(int) wgetch (WINDOW *);
	while((ch = getch()) != 'q'){
		newPosition = handleInput(ch, user);
		checkPosition(newPosition, user, level->tiles);
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

Level* createLevel(int level){
	Level* newLevel;
	newLevel = malloc(sizeof(Level));
	newLevel->level = level;
	newLevel->numberOfRooms = 3;
	newLevel->rooms = roomsSetUp();
	newLevel->tiles = saveLevelPositions();
}

Room** roomsSetUp(){
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


char** saveLevelPositions(){

	int x, y;
	char** positions;
	positions = malloc(25 * sizeof(char*));

	for (y = 0; y < 25; ++y)
	{
		positions[y] = malloc(100 * sizeof(char	));
		for (x = 0; x < 100; ++x)
		{
			positions[y][x] = mvinch(y, x);
		}
	}

	return positions;
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
 		if ((abs((temp.x-1) - doorTwo->x) < abs(temp.x - doorTwo->x) ) &&  (mvinch(temp.y, temp.x-1) == ' '))
 		{
 			previous.x = temp.x;
 			temp.x = temp.x - 1;

 		}else if((abs((temp.x + 1) - doorTwo->x) < abs(temp.x - doorTwo->x) ) &&  (mvinch(temp.y, temp.x + 1) == ' ')){
 			
 			previous.x = temp.x;
 			temp.x = temp.x + 1;

 		}else if((abs((temp.y + 1) - doorTwo->y) < abs(temp.y - doorTwo->y) ) &&  (mvinch(temp.y + 1, temp.x) == ' ')){
 			
 			previous.y = temp.y;
 			temp.y = temp.y + 1;

 		}else if((abs((temp.y - 1) - doorTwo->y) < abs(temp.y - doorTwo->y) ) &&  (mvinch(temp.y - 1, temp.x) == ' ')){
 			
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
 		
 		//getch();
 	}

 	return 1;
}

Player* playerSetUp(){
	Player* newPlayer;
	newPlayer = malloc(sizeof(Player));

	newPlayer->position.y = 14;
	newPlayer->position.x = 14;
	newPlayer->health = 20;

	mvprintw(newPlayer->position.y, newPlayer->position.x, "@");
	move(newPlayer->position.y, newPlayer->position.x);

	return newPlayer;
}

Position* handleInput(int input, Player* user){
	Position* newPosition;
	newPosition = malloc(sizeof(Position));
	switch(input)
	{	
		//move up
		case 'w':
		case 'W':
			newPosition->y = user->position.y - 1;
		    newPosition->x = user->position.x;
			break;
		//move down
		case 's':
		case 'S':
			newPosition->y = user->position.y + 1;
		    newPosition->x = user->position.x;
			break;
	    //move left
		case 'a':
		case 'A':
			newPosition->y = user->position.y;
		    newPosition->x = user->position.x - 1;
			break;
		//move right
		case 'd':
		case 'D':
			newPosition->y = user->position.y;
		    newPosition->x = user->position.x + 1;
			break;

		default:
			break;	
	}

	return newPosition;
		
}

int checkPosition(Position* newPosition, Player* user, char** level){
		int space;
		//#define mvinch(y,x)			mvwinch(stdscr,(y),(x))
		switch(mvinch(newPosition->y, newPosition->x))
		{
			case '.':
			case '+':
			case '#':
				playerMove(newPosition, user, level);
				break;
			default:
				move(user->position.y, user->position.x);
				break;
		}
}

int playerMove(Position* newPosition, Player* user, char** level){

	char buffer[8];
	sprintf(buffer, "%c", level[user->position.y][user->position.x]);
	mvprintw(user->position.y, user->position.x, buffer);
	user->position.y = newPosition->y;
	user->position.x = newPosition->x;
	mvprintw(user->position.y, user->position.x, "@");
	move(user->position.y, user->position.x);
}