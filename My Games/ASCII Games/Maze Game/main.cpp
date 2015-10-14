#include "Engine.h"
#include "Actor.h"
#include <cstdlib>
#include <ctime>
#include <curses.h>

#define MAX_ACTORS 500

struct TILE_TYPE
{
	char character;
	short colorCode;
	bool isPassable;
};

Actor * actorList[MAX_ACTORS];

void drawMap();
void drawTile(int y, int x);
bool addActorToList(Actor *newActor);
int testForActor(int y, int x);
void gameWon();

int playerY;
int playerX;

int width;
int height;

bool retry = true;
bool breakLoop = false;
bool generating = true;

int playerMovementSpeed;
int playerMovementSpeedHelper;

int generationSpeed;
int side;

int mapArray[mapHeight][mapWidth];

TILE_TYPE tileIndex[] = {
	{' ', 1, true}, // (0) TILE_HALLWAY
	{' ', 0, false}, // (1) TILE_WALL
	{' ', 1, true}, // (2) TILE_BACKTRACKED_HALLWAY
	{'>', 4, true}, // (3) TILE_EXIT
};

int main()
{
    playerY = 0;
    playerX = 0;

    int deltaY = 0;
    int deltaX = 0;

    srand(time(0));

    initscr();

	raw();
	keypad(stdscr, TRUE);
	noecho();
	curs_set(0);
	start_color();

	resize_term(25, 80);

    init_pair(1, COLOR_WHITE, COLOR_WHITE);
    init_pair(2, COLOR_WHITE, COLOR_YELLOW);
    init_pair(3, COLOR_WHITE, COLOR_GREEN);
    init_pair(4, COLOR_WHITE, COLOR_RED);

    while(retry){

        clear();

        playerY = 0;
        playerX = 0;

        playerMovementSpeed = 0;
        playerMovementSpeedHelper = 0;

        generationSpeed = 0;
        side = 0;

        echo();
        curs_set(1);

        char temp[4];
        bool done = false;
        bool invalid = false;

        mvprintw(0,0,"How high shall the maze be? Max: 75 Min: 7 (has to be odd)");
        while(!done){
            temp = {0,0,0,0};
            if(invalid){
                mvprintw(10, 0, "ERROR: CANNOT COMPUTE INPUT");
            }
            mvprintw(15, 0, "%s", temp);
            move(1,0);
            getstr(temp);
            height = atoi(temp);
            if(height <= 75 && height >= 7 && height % 2 == 1){
                done = true;
            }else{
                invalid = true;
                mvprintw(1, 0, "                          ");
                temp = {0,0,0,0};
            }
        }

        done = false;
        invalid = false;
        mvprintw(10, 0, "                          ");

        mvprintw(2,0,"How wide shall the maze be? Max: 151 Min: 7 (has to be odd)");
        while(!done){
            temp = {0,0,0,0};
            if(invalid){
                mvprintw(10, 0, "Try Again");
            }
            mvprintw(15, 0, "%s", temp);
            move(3,0);
            getstr(temp);
            width = atoi(temp);
            if(width <= 151 && width >= 7 && width % 2 == 1){
                done = true;
            }else{
                invalid = true;
                mvprintw(3, 0, "                          ");
                temp = {0,0,0,0};
            }
        }

        done = false;
        invalid = false;
        mvprintw(10, 0, "                          ");

        mvprintw(4,0,"How fast shall it generate? Max: 100 (the lower the faster)");
        while(!done){
            temp = {0,0,0,0};
            if(invalid){
                mvprintw(10, 0, "Invalid Input");
            }
            mvprintw(15, 0, "%s", temp);
            move(5,0);
            getstr(temp);
            generationSpeed = atoi(temp);
            if(generationSpeed <= 100){
                done = true;
            }else{
                invalid = true;
                mvprintw(5, 0, "                          ");
                temp = {0,0,0,0};
            }
        }

        done = false;
        invalid = false;
        mvprintw(10, 0, "                          ");
        if(width < 80 && height > 25){
            resize_term(height, 80);
        }
        resize_term(height, width);

        noecho();
        curs_set(0);

        for(int i = 0; i < MAX_ACTORS; i++){
            actorList[i] = NULL;
        }

        for(int i = 0; i < 1; i++){
            int y, x;
            side = rand() % 4;

            switch(side){
                case 0:
                    y = 1;
                    x = (rand() % width/2)*2-1;
                    playerY = 0;
                    playerX = x;
                    break;

                case 1:
                    y = height-2;
                    x = (rand() % width/2)*2-1;
                    playerY = height-1;
                    playerX = x;
                    break;

                case 2:
                    y = (rand() % height/2)*2-1;
                    x = 1;
                    playerY = y;
                    playerX = 0;
                    break;

                case 3:
                    y = (rand() % height/2)*2-1;
                    x = width-2;
                    playerY = y;
                    playerX = width-1;
                    break;

            }

            Actor *newActor = new Actor();

            newActor->setAppearance(' ', 2);
            newActor->setPos(y, x);
            newActor->setSpeed(generationSpeed);
            addActorToList(newActor);
        }

        for(int y = 0; y < height; y++){
            for(int x = 0; x < width; x++){
                mapArray[y][x] = 1;
            }
        }

        mapArray[playerY][playerX] = 2;

        timeout(1); //time before the getch passes with no input

        //put stuff before the game loop here

        while(!breakLoop){
            drawMap();

            for(int i = 0; i < MAX_ACTORS; i++){
                if(actorList[i] != NULL && actorList[i]->checkActive()){
                    actorList[i]->update();
                    actorList[i]->draw();
                }
            }

            if(!generating){

                if(mapArray[playerY][playerX] == 3){
                    gameWon();
                }

                attron(COLOR_PAIR(3));
                move(playerY, playerX);
                addch(' ');
                attroff(COLOR_PAIR(3));

                int input = getch();

                switch(input){
                    case KEY_UP:
                        deltaY = -1;
                        break;

                    case KEY_DOWN:
                        deltaY = 1;
                        break;

                    case KEY_LEFT:
                        deltaX = -1;
                        break;

                    case KEY_RIGHT:
                        deltaX = 1;
                        break;

                    default:
                        break;

                }

                if(isPassable(playerY + deltaY, playerX + deltaX) && playerMovementSpeedHelper >= playerMovementSpeed){
                    playerY += deltaY;
                    playerX += deltaX;
                    if(input == KEY_UP || input == KEY_DOWN || input == KEY_LEFT || input == KEY_RIGHT){
                        playerMovementSpeedHelper = 0;
                    }
                }

                deltaX = 0;
                deltaY = 0;

                playerMovementSpeedHelper++;

            }

            refresh();
        }
    }

	endwin();
}

bool addActorToList(Actor *newActor){
    for(int i = 0; i < MAX_ACTORS; i++){
        if(actorList[i] == NULL || !actorList[i]->checkActive()){
            actorList[i] = newActor;
            return true;
        }
    }
    return false;
}

int testForActor(int y, int x){
    for(int i = 0; i < MAX_ACTORS; i++){
        if(actorList[i] != NULL){
            if(actorList[i]->posY == y && actorList[i]->posX == x){
                return i;
            }
        }
    }
    return -1;
}

void drawMap(){
    clear();
	for(int y = 0; y < height; y++){
		for(int x = 0; x < width; x++){
			drawTile(y, x);
		}
	}
}

void drawTile(int y, int x){
	move(y, x);

	int color;
	char character;
    int tileType = mapArray[y][x];
    color = tileIndex[tileType].colorCode;
    character = tileIndex[tileType].character;

	attron(COLOR_PAIR(color));

	addch(character);

	attroff(COLOR_PAIR(color));
}

bool isPassable(int y, int x){
    if(y >= 0 && y < height && x >= 0 && x < width && tileIndex[mapArray[y][x]].isPassable){
        return true;
    }
    return false;
}

void gameWon(){
    bool done = false;

    clear();
    resize_term(25,80);
    mvprintw(0,0,"YOU WON!");
    mvprintw(5,0,"Wanna Play Again? Y/N");

    while(!done){
        int input = getch();
        if(input == 'y'){
            retry = true;
            done = true;
        }else if(input == 'n'){
            retry = false;
            done = true;
        }
    }
    breakLoop = true;
}
