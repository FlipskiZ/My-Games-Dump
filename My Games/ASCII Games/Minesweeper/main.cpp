#include <cstdlib>
#include <ctime>
#include <curses.h>

#define MAX_MAP_HEIGHT 25
#define MAX_MAP_WIDTH 45

struct TILE_TYPE
{
	char character;
	short colorCode;
};

int mapArray[MAX_MAP_HEIGHT][MAX_MAP_WIDTH];

TILE_TYPE tileIndex[] = {
	{'_', 7}, // (0) TILE_EMPTY
	{'_', 7}, // (1) TILE_BOMB
	{'*', 8}, // (2) TILE_UNCOVERED_BOMB
    {'.', 7}, // (3) TILE_UNCOVERED_EMPTY
	{'1', 6}, // (4) TILE_UNCOVERED_ONE
	{'2', 2}, // (5) TILE_UNCOVERED_TWO
	{'3', 3}, // (6) TILE_UNCOVERED_THREE
	{'4', 6}, // (7) TILE_UNCOVERED_FOUR
	{'5', 2}, // (8) TILE_UNCOVERED_FIVE
	{'6', 6}, // (9) TILE_UNCOVERED_SIX
	{'7', 5}, // (10) TILE_UNCOVERED_SEVEN
	{'8', 8}, // (11) TILE_UNCOVERED_EIGHT
	{'*', 9}, // (12) TILE_UNCOVERED_BOMB_LOSE
	{'!', 3}, // (13) TILE_FLAG
	{'!', 3}, // (14) TILE_FAILED_FLAG
	{'X', 3}, // (15) TILE_UNCOVERED_FAILED_FLAG
};

void drawMap();
void drawTile(int x, int y);
void generateMap(int totalBombs, bool placeOnCursor);
void deGenerateMap();
void drawStats();
void timer();
bool uncoverTile();
void uncoverEmpty(int y, int x);
void placeFlag();
void gameLost();
bool checkWinCondition();
void gameWon();

bool retry = true;
bool timerStarted = false;

int cursorY;
int cursorX;

int totalBombs;
int width;
int height;

char temp[3];

int amountMoves;
int amountFlagsLeft;
int amountTilesUncovered;
float amountTime;

int timeHelper;

int main()
{
    cursorY = 0;
    cursorX = 0;

    int deltaY = 0;
    int deltaX = 0;

    amountMoves = 0;
    amountTilesUncovered = 0;
    amountTime = 0;

    char choice;

    initscr();

	raw();
	keypad(stdscr, TRUE);
	noecho();
	curs_set(0);
	start_color();

    init_pair(1, COLOR_BLUE, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_RED, COLOR_BLACK);
    init_pair(4, COLOR_YELLOW, COLOR_BLACK);
    init_pair(5, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(6, COLOR_CYAN, COLOR_BLACK);
    init_pair(7, COLOR_WHITE, COLOR_BLACK);
    init_pair(8, 8, COLOR_BLACK);
    init_pair(9, 8, COLOR_RED);
    init_pair(10, COLOR_BLUE, 8);
    init_pair(11, COLOR_GREEN, 8);
    init_pair(12, COLOR_RED, 8);
    init_pair(13, COLOR_YELLOW, 8);
    init_pair(14, COLOR_MAGENTA, 8);
    init_pair(15, COLOR_CYAN, 8);
    init_pair(16, COLOR_WHITE, 8);

    while(retry){

    bool breakLoop = false;
    bool done = false;
    bool invalid = false;
    bool firstAction = true;

    amountMoves = 0;
    amountTilesUncovered = 0;

    clear();

    deGenerateMap();

    mvprintw(0, 0, "Customize your game!");
    mvprintw(1, 0, "You can use a pre-made game, or you could make your own.");
    mvprintw(2, 0, "Press 'B' for beginner game");
    mvprintw(3, 0, "Press 'I' for intermediate game");
    mvprintw(4, 0, "Press 'E' for expert game");
    mvprintw(5, 0, "Or press 'C' to make a custom game");

    while(!done){
        choice = getch();

        switch(choice){
            case 'b':
                height = 10;
                width = 10;
                totalBombs = 15;
                done = true;
                break;

            case 'i':
                height = 20;
                width = 20;
                totalBombs = 60;
                done = true;
                break;

            case 'e':
                height = 20;
                width = 40;
                totalBombs = 150;
                done = true;
                break;

            case 'c':
                clear();

                done = false;

                echo();

                while(!done){
                    clear();
                    mvprintw(0, 0, "What height shall the game have? (max is 25)");
                    if(invalid){
                        mvprintw(3, 0, "Invalid selection! Try again.");
                    }
                    move(1,0);
                    getstr(temp);
                    height = atoi(temp);
                    if(height <= 25 && height > 0){
                        done = true;
                    }else{
                        invalid = true;
                    }
                }

                done = false;
                invalid = false;
                clear();

                while(!done){
                    clear();
                    mvprintw(0, 0, "What width shall the game have? (max is 45)");
                    if(invalid){
                        mvprintw(3, 0, "Invalid selection! Try again.");
                    }
                    move(1,0);
                    getstr(temp);
                    width = atoi(temp);
                    if(width <= 45 && width > 0){
                        done = true;
                    }else{
                        invalid = true;
                    }
                }

                done = false;
                invalid = false;
                clear();

                if(height*width==1){
                    mvprintw(0, 0, "Height * Width much equal at least 2 (You can't do 1 on both)");
                    mvprintw(3, 0, "Press any key to continue...");
                    getch();
                    breakLoop = true;
                    done = true;
                    break;
                }

                while(!done){
                    clear();
                    mvprintw(0, 0, "How many bombs shall the game have? (max is %d)", height*width-1);
                    if(invalid){
                        mvprintw(3, 0, "Invalid selection! Try again.");
                    }
                    move(1,0);
                    getstr(temp);
                    totalBombs = atoi(temp);
                    if(totalBombs < height*width && totalBombs != 0){
                        done = true;
                    }else{
                        invalid = true;
                    }
                }

                noecho();

                break;

            default:
                mvprintw(8, 0, "Invalid selection! Try again.");

            break;
        }
    }
    if(!breakLoop)
    generateMap(totalBombs, true);

    amountFlagsLeft = totalBombs;

    cursorY = 0;
    cursorX = 0;

    while(!breakLoop){
        timer();
        drawMap();
        drawStats();

        attron(COLOR_PAIR(tileIndex[mapArray[cursorY][cursorX]].colorCode+9));
        move(cursorY, cursorX);
        addch(tileIndex[mapArray[cursorY][cursorX]].character);
        attroff(COLOR_PAIR(tileIndex[mapArray[cursorY][cursorX]].colorCode+9));

        timeout(0);

        int input = getch();

        switch(input){
            case KEY_UP:
                amountMoves += 1;
                deltaY = -1;
                break;

            case KEY_DOWN:
                amountMoves += 1;
                deltaY = 1;
                break;

            case KEY_LEFT:
                amountMoves += 1;
                deltaX = -1;
                break;

            case KEY_RIGHT:
                amountMoves += 1;
                deltaX = 1;
                break;

            case ' ':
                amountMoves += 1;
                if(firstAction){
                    firstAction = false;
                    if(mapArray[cursorY][cursorX] == 1){
                        mapArray[cursorY][cursorX] = 0;
                        generateMap(1, false);
                    }
                }
                if(!uncoverTile()){
                    breakLoop = true;
                }
                break;

            case 'f':
                amountMoves += 1;
                placeFlag();
                break;

            case 'r':
                breakLoop = true;
                break;

            default:
                break;

        }

        if(cursorY + deltaY >= 0 && cursorY + deltaY < height && cursorX + deltaX >= 0 && cursorX + deltaX < width){
            cursorY += deltaY;
            cursorX += deltaX;
        }
        deltaX = 0;
        deltaY = 0;

        if(checkWinCondition()){
            gameWon();
            breakLoop = true;
        }

        refresh();
    }
    }

	endwin();
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

void generateMap(int totalBombs, bool placeOnCursor){
    srand(time(0));

    int chance = height * width;
    int random = 0;
    int bombsPlaced = 0;
    bool filled = false;

    while(!filled){
        for(int y = 0; y < height; y++){
            for(int x = 0; x < width; x++){
                random = rand() % chance + 1;
                if(random == 1 && totalBombs > bombsPlaced && mapArray[y][x] != 1 && ((cursorY != y || cursorX != x)||(cursorY == y && cursorX == x && placeOnCursor))){
                    mapArray[y][x] = 1;
                    bombsPlaced += 1;
                    if(totalBombs <= bombsPlaced){
                        filled = true;
                    }
                }
            }
        }
    }
}

void deGenerateMap(){
    for(int y = 0; y <= height; y++){
        for(int x = 0; x <= width; x++){
            mapArray[y][x] = 0;
        }
    }
}

void drawStats(){
    mvprintw(2, width+1, "Amount of moves done: %d", amountMoves);
    mvprintw(5, width+1, "Amount of flags left: %d/%d", amountFlagsLeft, totalBombs);
    mvprintw(8, width+1, "Amount of tiles uncovered: %d/%d", amountTilesUncovered, height*width-totalBombs);
    mvprintw(11, width+1, "Amount of time used: %f", ((float)amountTime)/CLOCKS_PER_SEC);
}

void timer(){
    if(!timerStarted){
        timeHelper = clock();
        timerStarted = true;
    }
    amountTime = clock() - timeHelper;
}

bool uncoverTile(){
    switch(mapArray[cursorY][cursorX]){
        case 0:
            uncoverEmpty(cursorY, cursorX);
            break;

        case 1:
            gameLost();
            return false;
            break;

        default:
            break;
    }
    return true;
}

void uncoverEmpty(int y, int x){
    int iY = 0;
    int iX = 0;
    int amountBombs = 0;

    if(mapArray[y][x] == 0){
        amountTilesUncovered++;
    }

    for(iY = -1; iY <= 1; iY++){
        for(iX = -1; iX <= 1; iX++){
            int tempY = y + iY;
            int tempX = x + iX;
            if((tempY >= 0 && tempY < height && tempX >= 0 && tempX < width) && (mapArray[tempY][tempX] == 1 || mapArray[tempY][tempX] == 13)){
                amountBombs += 1;
            }
        }
    }

    mapArray[y][x] = 3+amountBombs;

    if(amountBombs <= 0){
        for(iY = -1; iY <= 1; iY++){
            for(iX = -1; iX <= 1; iX++){
                int tempY = y + iY;
                int tempX = x + iX;
                if(tempY >= 0 && tempY <= height && tempX >= 0 && tempX <= width && mapArray[tempY][tempX] != 3){
                    uncoverEmpty(tempY, tempX);
                }
            }
        }
    }
}

void placeFlag(){
    if(mapArray[cursorY][cursorX] == 0 && amountFlagsLeft > 0){
        amountFlagsLeft -= 1;
        mapArray[cursorY][cursorX] = 14;
    }else if(mapArray[cursorY][cursorX] == 1 && amountFlagsLeft > 0){
        amountFlagsLeft -= 1;
        mapArray[cursorY][cursorX] = 13;
    }else if(mapArray[cursorY][cursorX] == 13){
        amountFlagsLeft += 1;
        mapArray[cursorY][cursorX] = 1;
    }else if(mapArray[cursorY][cursorX] == 14){
        amountFlagsLeft += 1;
        mapArray[cursorY][cursorX] = 0;
    }
}

void gameLost(){
    char choice;
    bool done = false;

    mapArray[cursorY][cursorX] = 12;

	for(int y = 0; y < height; y++){
		for(int x = 0; x < width; x++){
			if(mapArray[y][x] == 1){
                mapArray[y][x] = 2;
			}else if(mapArray[y][x] == 14){
                mapArray[y][x] = 15;
			}
		}
	}

    drawMap();

    drawStats();

    mvprintw(14, width+1, "GAME OVER");
    mvprintw(16, width+1, "Do you want to play again? Y/N");

    while(!done){
        choice = getch();

        if(choice == 'y' || choice == 'Y'){
            retry = true;
            done = true;
        }else if(choice == 'n' || choice == 'N'){
            retry = false;
            done = true;
        }
    }
}

bool checkWinCondition(){
    bool won = false;
    int tileAmount = height * width;
    int tileGotten = 0;

    for(int y = 0; y < height; y++){
        for(int x = 0; x < width; x++){
            if(mapArray[y][x] == 1 || (mapArray[y][x] >= 3 && mapArray[y][x] <= 11) || mapArray[y][x] == 13){
                tileGotten += 1;
            }else{
                break;
            }
        }
    }

    if(tileAmount <= tileGotten){
        won = true;
    }

    return won;
}

void gameWon(){
    char choice;
    bool done = false;

    drawMap();

    drawStats();

    mvprintw(14, width+1, "YOU WON");
    mvprintw(16, width+1, "Do you want to play again? Y/N");

    while(!done){
        choice = getch();

        if(choice == 'y' || choice == 'Y'){
            retry = true;
            done = true;
        }else if(choice == 'n' || choice == 'N'){
            retry = false;
            done = true;
        }
    }
}
