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
	Position* position;
	int health;
	int attack;
	int gold;
	int exp;
	int maxHealth;
} Player;

typedef struct Level {
	char** tiles;
	int level;
	int numberOfRooms;
	struct Room** rooms;
	struct Monster** monsters;
	int numberOfMonsters;
	struct Player* user;
} Level;

typedef struct Monster {
	char string[2];
	char symbol;
	int health;
	int attack;
	int speed;
	int defence;
	int pathfinding;
	int alive;
	Position* position;
} Monster;

//screen functions
int screenSetUp();
int printGameHub(Level* level);

// level/map functions
Level* createLevel(int level);
Room** roomsSetUp();
char** saveLevelPositions();

// player functions
Player* playerSetUp();
int placePlayer(Room** rooms, Player* user);
Position* handleInput(int input, Player* user);
int checkPosition(Position* newPosition, Level* level);
int playerMove(Position* newPosition, Player* user, char** level);

//room functions
Room* createRoom(int grid);
int drawRoom(Room* room);
int connectDoors(Position* doorOne, Position* doorTwo);

//monster functions
int addMonsters(Level* level);
Monster* selectMonster(int level);
Monster* createMonster(char symbol, int health, int attack, int speed, int defence, int pathfinding);
int killMonster(Monster* monster);
int setStartingPosition(Monster* monster, Room* room);
int moveMonster(Level* level);
int pathfindingRandom(Position* position);
int pathfindingSeek(Position* start, Position* destination);
Monster* getMonsterAt(Position* position, Monster** monsters);

int combat(Player* player, Monster* monsters, int order);

int main()
{
	int ch;
	Level* level;
	Position* newPosition;

	screenSetUp();

	level = createLevel(1);

	playerSetUp();

	printGameHub(level);

	//#define getch()			wgetch(stdscr)
	//(int) wgetch (WINDOW *);
	while((ch = getch()) != 'q'){
		printGameHub(level);
		newPosition = handleInput(ch, level->user);
		checkPosition(newPosition, level);
		moveMonster(level);
		move(level->user->position->y, level->user->position->x);
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

int printGameHub(Level* level){
	mvprintw(25, 0, "    Level:%", level->level);
	printw("    Gold: %d", level->user->gold);
	printw("    Hp: %d(%d)", level->user->health, level->user->maxHealth);
	printw("   Attack: %d", level->user->attack);
	printw("   Exp: %d", level->user->exp);
	printw("   ");

	return 1;
}
 
Level* createLevel(int level){
	Level* newLevel;
	newLevel = malloc(sizeof(Level));
	newLevel->level = level;
	newLevel->numberOfRooms = 3;
	newLevel->rooms = roomsSetUp();
	newLevel->tiles = saveLevelPositions();
	newLevel->user = playerSetUp();
	placePlayer(newLevel->rooms, newLevel->user);
	addMonsters(newLevel);
	return newLevel;
}

Room** roomsSetUp(){
	int x;
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

	for (x = 0; x < 6; ++x)
	{
		rooms[x] = createRoom(x);
		drawRoom(rooms[x]);
	}

	//mvprintw(2, 40,"--------");
	//mvprintw(3, 40,"|......|");
	//mvprintw(4, 40,"|......|");
	//mvprintw(5, 40,"|......|");
	//mvprintw(6, 40,"|......|");
	//mvprintw(7, 40,"--------");

	//rooms[1] = createRoom(40, 2, 6, 8);
	//drawRoom(rooms[1]);

	//mvprintw(10, 40,"------------");
	//mvprintw(11, 40,"|..........|");
	//mvprintw(12, 40,"|..........|");
	//mvprintw(13, 40,"|..........|");
	//mvprintw(14, 40,"|..........|");
	//mvprintw(15, 40,"------------");

	//rooms[2] = createRoom(40, 10, 6, 12);
	//drawRoom(rooms[2]);

	//connectDoors(rooms[0]->doors[3], rooms[2]->doors[1]);
	//connectDoors(rooms[1]->doors[2], rooms[0]->doors[0]);

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


Room* createRoom(int grid){
	Room* newRoom;
	newRoom = malloc(sizeof(Room));

	switch(grid){

		case 0:
			newRoom->position.x = 0;
		    newRoom->position.y = 0;
		   	break;
		case 1:
			newRoom->position.x = 33;
		    newRoom->position.y = 0;
		   	break;
		case 2:
			newRoom->position.x = 66;
		    newRoom->position.y = 0;
		   	break;
		case 3:
			newRoom->position.x = 0;
		    newRoom->position.y = 14;
		   	break;
		case 4:
			newRoom->position.x = 33;
		    newRoom->position.y = 14;
		   	break;
		case 5:
			newRoom->position.x = 66;
		    newRoom->position.y = 14;
		   	break;
	}
	newRoom->height = rand() % 6 + 4;
	newRoom->width = rand() % 14 + 4;

	newRoom->position.x += rand() % (29 - newRoom->width + 1);
	newRoom->position.y += rand() % (9 - newRoom->height + 1);

	newRoom->doors = malloc(4*sizeof(Position));
	//top door
	newRoom->doors[0] = malloc(sizeof(Position));
	newRoom->doors[0]->x = rand() % (newRoom->width - 2) + newRoom->position.x +1;
	newRoom->doors[0]->y = newRoom->position.y;
	//left door
	newRoom->doors[1] = malloc(sizeof(Position));
	newRoom->doors[1]->y = rand() % (newRoom->height - 2) + newRoom->position.y +1;
	newRoom->doors[1]->x = newRoom->position.x;
	//bottom door
	newRoom->doors[2] = malloc(sizeof(Position));
	newRoom->doors[2]->x = rand() % (newRoom->width - 2) + newRoom->position.x +1;
	newRoom->doors[2]->y = newRoom->position.y + newRoom->height - 1;
	//right door
	newRoom->doors[3] = malloc(sizeof(Position));
	newRoom->doors[3]->y = rand() % (newRoom->height - 2) + newRoom->position.y +1;
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
	newPlayer->position = malloc(sizeof(Position));

	newPlayer->health = 20;
	newPlayer->attack = 1;
	newPlayer->gold = 0;
	newPlayer->exp = 0;
	newPlayer->maxHealth = 20;

	return newPlayer;
}

int placePlayer(Room** rooms, Player* user){
	user->position->x = rooms[3]->position.x + 1;
	user->position->y = rooms[3]->position.y + 1;

	mvprintw(user->position->y, user->position->x, "@");
	move(user->position->y, user->position->x);
}

Position* handleInput(int input, Player* user){
	Position* newPosition;
	newPosition = malloc(sizeof(Position));
	switch(input)
	{	
		//move up
		case 'w':
		case 'W':
			newPosition->y = user->position->y - 1;
		    newPosition->x = user->position->x;
			break;
		//move down
		case 's':
		case 'S':
			newPosition->y = user->position->y + 1;
		    newPosition->x = user->position->x;
			break;
	    //move left
		case 'a':
		case 'A':
			newPosition->y = user->position->y;
		    newPosition->x = user->position->x - 1;
			break;
		//move right
		case 'd':
		case 'D':
			newPosition->y = user->position->y;
		    newPosition->x = user->position->x + 1;
			break;

		default:
			break;	
	}

	return newPosition;
		
}

int checkPosition(Position* newPosition, Level* level){
		Player* user;
		user = level->user;
		int space;
		//#define mvinch(y,x)			mvwinch(stdscr,(y),(x))
		switch(mvinch(newPosition->y, newPosition->x))
		{
			case '.':
			case '+':
			case '#':
				playerMove(newPosition, user, level->tiles);
				break;
			case 'X':
			case 'G':
			case 'T':
				combat(user, getMonsterAt(newPosition, level->monsters), 1);
				break;	
			default:
				move(user->position->y, user->position->x);
				break;
		}
}

int playerMove(Position* newPosition, Player* user, char** level){

	char buffer[8];
	sprintf(buffer, "%c", level[user->position->y][user->position->x]);
	mvprintw(user->position->y, user->position->x, buffer);
	user->position->y = newPosition->y;
	user->position->x = newPosition->x;
	mvprintw(user->position->y, user->position->x, "@");
	move(user->position->y, user->position->x);
}

int addMonsters(Level* level){
	int x;
	level->monsters = malloc(6*sizeof(Monster*));
	level->numberOfMonsters = 0;
	for (x = 0; x < level->numberOfRooms; x++)
	{
		if ((rand()%2) == 0)
		{
			level->monsters[level->numberOfMonsters] = selectMonster(level->level);
			setStartingPosition(level->monsters[level->numberOfMonsters], level->rooms[x]);
			level->numberOfMonsters++;
		}
	}

}

Monster* selectMonster(int level){
	int monster;

	switch(level){
		case 1:
		case 2:
		case 3:
			monster = (rand()%2) + 1;
			break;
		case 4:
		case 5:
			monster = (rand()%2) + 2;
			break;
		case 6:
			monster = 3;
			break;
	}

	switch(monster){
		case 1:
			return createMonster('X', 2, 1, 1, 1, 1);
		case 2:
			return createMonster('G', 5, 3, 1, 1, 2);
		case 3:
			return createMonster('T', 15, 5, 1, 1, 1);
	}
}

Monster* createMonster(char symbol, int health, int attack, int speed, int defence, int pathfinding){

	Monster* newMonster;
	newMonster = malloc(sizeof(Monster));
	newMonster->symbol = symbol;
	newMonster->health = health;
	newMonster->attack = attack;
	newMonster->speed = speed;
	newMonster->defence = defence;
	newMonster->pathfinding = pathfinding;
	newMonster->alive = 1;

	sprintf(newMonster->string, "%c", symbol);

	newMonster->position = malloc(sizeof(Position));

	return newMonster;
}

int killMonster(Monster* monster){
	mvprintw(monster->position->y, monster->position->x, ".");
	monster->alive = 0;
	return 1;
}

int setStartingPosition(Monster* monster, Room* room){

	char buffer[8];

	monster->position = malloc(sizeof(Position));

	monster->position->x = (rand() % (room->width - 2)) + room->position.x + 1;
	monster->position->y = (rand() % (room->height - 2)) + room->position.y + 1;

	mvprintw(monster->position->y, monster->position->x, monster->string);
}

int moveMonster(Level* level){
	int x;
	for (x = 0; x < level->numberOfMonsters; ++x)
	{	if (level->monsters[x]->alive == 0)
			continue;


		mvprintw(level->monsters[x]->position->y, level->monsters[x]->position->x, ".");
		if (level->monsters[x]->pathfinding == 1)
		{
			pathfindingRandom(level->monsters[x]->position);
		}else{
			pathfindingSeek(level->monsters[x]->position, level->user->position);
		}
		mvprintw(level->monsters[x]->position->y, level->monsters[x]->position->x, level->monsters[x]->string);
	}
}

int pathfindingRandom(Position* position){
	int Random = rand() % 5;
	switch(Random){
		case 0:
			if (mvinch(position->y - 1, position->x) == '.')
			{
				position->y = position->y - 1;
			}
			break;
		case 1:
			if (mvinch(position->y + 1, position->x) == '.')
			{
				position->y = position->y + 1;
			}
			break;
		case 2:
			if (mvinch(position->y, position->x - 1) == '.')
			{
				position->x = position->x - 1;
			}
			break;
		case 3:
			if (mvinch(position->y, position->x + 1) == '.')
			{
				position->x = position->x + 1;
			}
			break;
		case 4:
			break;
	}
}

int pathfindingSeek(Position* start, Position* destination){
	if ((abs((start->x-1) - destination->x) < abs(start->x - destination->x) ) &&  (mvinch(start->y, start->x-1) == '.'))
 		{
 			start->x = start->x - 1;

 		}else if((abs((start->x + 1) - destination->x) < abs(start->x - destination->x) ) &&  (mvinch(start->y, start->x + 1) == '.')){
 			
 			start->x = start->x + 1;

 		}else if((abs((start->y + 1) - destination->y) < abs(start->y - destination->y) ) &&  (mvinch(start->y + 1, start->x) == '.')){
 			
 			start->y = start->y + 1;

 		}else if((abs((start->y - 1) - destination->y) < abs(start->y - destination->y) ) &&  (mvinch(start->y - 1, start->x) == '.')){
 			
 			start->y = start->y - 1;
 			
 		}else{
 				//do nothing
 		}

 		return 1;	
}

Monster* getMonsterAt(Position* position, Monster** monsters){
	int x;
	for (x = 0; x < 6; ++x)
	{
		if ((position->y == monsters[x]->position->y) && (position->x == monsters[x]->position->x))
		{
			return monsters[x];
		}
	}

	return NULL;
}

int combat(Player* player, Monster* monster, int order){
	if (order == 1)
	{
		monster->health -= player->attack;
		if (monster->health > 0)
		{
			player->health -= monster->attack;
		}else{
			killMonster(monster);
			player->exp++;
		}
	}else{
		player->health -= monster->attack;
		if (player->health > 0)
		{
			monster->health -= player->attack;
		}
	}
	return 1;
} 