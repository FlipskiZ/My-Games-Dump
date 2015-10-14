#include <cstdlib>
#include <ctime>
#include <curses.h>

#define mapHeight 50
#define mapWidth 100

struct TILE_TYPE
{
	char character;
	short colorCode;
	bool isPassable;
};

void drawMap();
void drawTile(int y, int x);
void drawStats();
bool isPassable(int y, int x);
void checkSpawn(int y, int x);
void checkSurvival(int y, int x);
void simulationOptions();

int cursorY;
int cursorX;

bool retry = true;
bool breakLoop = false;

int simulationSpeed;
int simulationSpeedHelper;

bool paused;

int mapArray[mapHeight][mapWidth];

TILE_TYPE tileIndex[] = {
	{' ', 0, true}, // (0) TILE_EMPTY
	{' ', 2, true}, // (1) TILE_CELL
	{' ', 3, true}, // (2) TILE_TEMPSPAWNCELL
	{' ', 4, true}, // (3) TILE_TEMPDELETECELL
};

int main()
{
    cursorY = 0;
    cursorX = 0;

    int deltaY = 0;
    int deltaX = 0;

    srand(time(0));

    initscr();

	raw();
	keypad(stdscr, TRUE);
	noecho();
	curs_set(0);
	start_color();

	resize_term(mapHeight, mapWidth+21);

    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    init_pair(2, COLOR_WHITE, COLOR_YELLOW);
    init_pair(3, COLOR_WHITE, COLOR_GREEN);
    init_pair(4, COLOR_WHITE, COLOR_RED);

    while(retry){

        clear();

        cursorY = mapHeight/2;
        cursorX = mapWidth/2;

        simulationSpeed = 20;
        simulationSpeedHelper = 0;

        paused = true;

        for(int y = 0; y < mapHeight; y++){
            for(int x = 0; x < mapWidth; x++){
                mapArray[y][x] = 0;
            }
        }

        timeout(0); //time before the getch passes with no input

        //put stuff before the game loop here

        while(!breakLoop){
            drawMap();
            drawStats();

            if(mapArray[cursorY][cursorX] == 0){
                attron(COLOR_PAIR(0));
                move(cursorY, cursorX);
                addch('X');
                attroff(COLOR_PAIR(0));
            }else{
                attron(COLOR_PAIR(2));
                move(cursorY, cursorX);
                addch('X');
                attroff(COLOR_PAIR(2));
            }

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

                case 'p':
                    paused = !paused;
                    break;

                case 'o':
                    simulationOptions();
                    break;

                case ' ':
                    if(mapArray[cursorY][cursorX] == 0){
                        mapArray[cursorY][cursorX] = 1;
                    }else{
                        mapArray[cursorY][cursorX] = 0;
                    }
                    break;

                default:
                    break;

            }

            if(isPassable(cursorY + deltaY, cursorX + deltaX)){
                cursorY += deltaY;
                cursorX += deltaX;
            }

            deltaX = 0;
            deltaY = 0;

            if(!paused){
            simulationSpeedHelper++;
            }

            if(simulationSpeedHelper >= simulationSpeed){
                for(int y = 0; y < mapHeight; y++){
                    for(int x = 0; x < mapWidth; x++){
                        if(mapArray[y][x] == 0){
                            checkSpawn(y, x);
                        }else if(mapArray[y][x] == 1){
                            checkSurvival(y, x);
                        }
                    }
                }

                for(int y = 0; y < mapHeight; y++){
                    for(int x = 0; x < mapWidth; x++){
                        if(mapArray[y][x] == 2){
                            mapArray[y][x] = 1;
                        }else if(mapArray[y][x] == 3){
                            mapArray[y][x] = 0;
                        }
                    }
                }
                simulationSpeedHelper = 0;
            }

            refresh();
        }
    }

	endwin();
}

void drawMap(){
    clear();
	for(int y = 0; y < mapHeight; y++){
		for(int x = 0; x < mapWidth; x++){
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

void drawStats(){
	for(int y = 0; y < mapHeight; y++){
	    mvprintw(y, mapWidth, "|");
	}
	if(paused){
        mvprintw(1, mapWidth+1, "SIMULATION - PAUSED");
	}else{
        mvprintw(1, mapWidth+1, "SIMULATION - RUNNING");
    }
}

bool isPassable(int y, int x){
    if(y >= 0 && y < mapHeight && x >= 0 && x < mapWidth && tileIndex[mapArray[y][x]].isPassable){
        return true;
    }
    return false;
}

void checkSpawn(int y, int x){
    int cellsAround = 0;
    for(int y1 = -1; y1 < 2; y1++){
        for(int x1 = -1; x1 < 2; x1++){
            if((mapArray[y1+y][x1+x] == 1 || mapArray[y1+y][x1+x] == 3) && isPassable(y1+y, x1+x) && (y1 != 0 || x1 != 0)){
                cellsAround++;
            }
        }
    }
    if(cellsAround == 3){
        mapArray[y][x] = 2;
    }
}

void checkSurvival(int y, int x){
    int cellsAround = 0;
    for(int y1 = -1; y1 < 2; y1++){
        for(int x1 = -1; x1 < 2; x1++){
            if((mapArray[y1+y][x1+x] == 1 || mapArray[y1+y][x1+x] == 3) && isPassable(y1+y, x1+x) && (y1 != 0 || x1 != 0)){
                cellsAround++;
            }
        }
    }
    if(cellsAround <= 1 || cellsAround >= 4){
        mapArray[y][x] = 3;
    }
}

void simulationOptions(){

}
