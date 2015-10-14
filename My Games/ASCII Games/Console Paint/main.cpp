#include <cstdlib>
#include <ctime>
#include <fstream>
#include <curses.h>

#define MAX_MAP_HEIGHT 25
#define MAX_MAP_WIDTH 60

struct TILE_TYPE
{
	char character;
	short colorCode;
};

int mapArray[MAX_MAP_HEIGHT][MAX_MAP_WIDTH];

TILE_TYPE tileIndex[] = {
	{' ', 1}, // (0) TILE_BLACK
	{' ', 2}, // (1) TILE_WHITE
	{' ', 3}, // (2) TILE_BLUE
	{' ', 4}, // (3) TILE_RED
	{' ', 5}, // (4) TILE_GREEN
	{' ', 6}, // (5) TILE_MAGENTA
	{' ', 7}, // (6) TILE_CYAN
	{' ', 8}, // (7) TILE_YELLOW
};

void drawMap();
void drawTile(int x, int y);
void drawMenu();
void drawColor();
void bucketFill(int y, int x);
void brush();
void save();
void load();

bool retry = true;
bool breakLoop = false;

int height = MAX_MAP_HEIGHT;
int width = MAX_MAP_WIDTH;

int cursorY;
int cursorX;

int activeTile;
int cursorTile;
int brushSize;

int main()
{
    cursorY = 0;
    cursorX = 0;

    int deltaY = 0;
    int deltaX = 0;

    activeTile = 0;

    bool cursor = true;
    bool cursorActive = true;

    brushSize = 3;

    initscr();

	raw();
	keypad(stdscr, TRUE);
	noecho();
	curs_set(0);
	start_color();

    init_pair(1, COLOR_BLACK, COLOR_BLACK);
    init_pair(2, COLOR_BLACK, COLOR_WHITE);
    init_pair(3, COLOR_BLACK, COLOR_BLUE);
    init_pair(4, COLOR_BLACK, COLOR_RED);
    init_pair(5, COLOR_BLACK, COLOR_GREEN);
    init_pair(6, COLOR_BLACK, COLOR_MAGENTA);
    init_pair(7, COLOR_BLACK, COLOR_CYAN);
    init_pair(8, COLOR_BLACK, COLOR_YELLOW);

    while(retry){

        breakLoop = false;

        clear();

        cursorY = 0;
        cursorX = 0;

        for(int y = 0; y < height; y++){
            for(int x = 0; x < width; x++){
                mapArray[y][x] = 1;
            }
        }

        //put stuff before the game loop here

        while(!breakLoop){
            drawMap();
            drawMenu();

            if(cursor && cursorActive){
                attron(COLOR_PAIR(9));
                move(cursorY, cursorX);
                addch('|');
                attroff(COLOR_PAIR(9));
                cursor = false;
            }else if(!cursor){
                cursor = true;
            }

            timeout(550); //time before the getch passes with no input

            int input = getch();

            switch(input){
                case KEY_UP:
                    deltaY = -1;
                    cursor = true;
                    break;

                case KEY_DOWN:
                    deltaY = 1;
                    cursor = true;
                    break;

                case KEY_LEFT:
                    deltaX = -1;
                    cursor = true;
                    break;

                case KEY_RIGHT:
                    deltaX = 1;
                    cursor = true;
                    break;

                case ' ':
                    drawColor();
                    cursor = false;
                    break;

                case 'q':
                    cursorTile = mapArray[cursorY][cursorX];
                    bucketFill(cursorY, cursorX);
                    break;

                case 'w':
                    brush();
                    break;

                case 'e':
                    if(brushSize < 15)
                        brushSize += 2;
                    break;

                case 'r':
                    if(brushSize > 3)
                        brushSize -= 2;
                    break;

                case 't':
                    cursorActive = !cursorActive;
                    break;

                case 'a':
                    load();
                    break;

                case 's':
                    save();
                    break;

                case '1':
                    activeTile = 0;
                    break;

                case '2':
                    activeTile = 1;
                    break;

                case '3':
                    activeTile = 2;
                    break;

                case '4':
                    activeTile = 3;
                    break;

                case '5':
                    activeTile = 4;
                    break;

                case '6':
                    activeTile = 5;
                    break;

                case '7':
                    activeTile = 6;
                    break;

                case '8':
                    activeTile = 7;
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

void drawMenu(){
    int row;
    int col;

    getmaxyx(stdscr,row,col);

    for(int y = 0; y < 25; y++){
        mvprintw(y, width, "|");
    }

    mvprintw(1, width+((col-width)/2)-7, "BUCKET - Q");
    mvprintw(3, width+((col-width)/2)-7, "BRUSH %dx%d - W", brushSize, brushSize);
    mvprintw(5, width+((col-width)/2)-7, "BRUSH UP - E");
    mvprintw(7, width+((col-width)/2)-7, "BRUSH DOWN - R");
    mvprintw(9, width+((col-width)/2)-7, "CURSOR - T");
    mvprintw(11, width+((col-width)/2)-7, "LOAD - A");
    mvprintw(13, width+((col-width)/2)-7, "SAVE - S");

    for(int y = 0; y < 2; y++){
        for(int x = 0; x < 8; x++){
            move(20+y, width+1+x*2+((col-width)/2-8));
            if(y == 0){
                attron(COLOR_PAIR(1+x));
                addch(' ');
                attroff(COLOR_PAIR(1+x));
            }else if(y == 1){
                printw("%d", x+1);
            }
        }
    }

    attron(COLOR_PAIR(activeTile+1));
    mvprintw(23, width+((col-width)/2), " ");
    attroff(COLOR_PAIR(activeTile+1));
    mvprintw(24, width+((col-width)/2), "%d", activeTile+1);
}

void drawColor(){
    mapArray[cursorY][cursorX] = activeTile;
}

void bucketFill(int y, int x){
    if(cursorTile == activeTile)
        return;
    int tempY;
    int tempX;
    for(int iY = -1; iY <= 1; iY++){
        for(int iX = -1; iX <= 1; iX++){
            tempY = y + iY;
            tempX = x + iX;
            if(tempY >= 0 && tempY < height && tempX >= 0 && tempX < width && (tempY != cursorY || tempX != cursorX) && mapArray[tempY][tempX] == cursorTile){
                mapArray[tempY][tempX] = activeTile;
                bucketFill(tempY, tempX);
            }
        }
    }
    mapArray[cursorY][cursorX] = activeTile;
}

void brush(){
    int brush = brushSize/2;
    int nBrush = 0 - brush;
    for(int y = nBrush; y <= brush; y++){
        for(int x = nBrush; x <= brush; x++){
            int tempY = cursorY + y;
            int tempX = cursorX + x;
            if(tempY >= 0 && tempY < height && tempX >= 0 && tempX < width){
                mapArray[tempY][tempX] = activeTile;
            }
        }
    }
}

void save(){

    timeout(-1);

    clear();

    int choice;

    mvprintw(0,0,"What savefile slot do you want to save in?");

    mvprintw(5,0,"Slot - 1");

    mvprintw(7,0,"Slot - 2");

    mvprintw(9,0,"Slot - 3");

    mvprintw(11,0,"Press any other key to cancel the saving");

    choice = getch();

    switch(choice){

        case '1':{
            std::ofstream drawing("drawing.txt");

            for(int y = 0; y < height; y++){
                for(int x = 0; x <= width; x++){
                    if(x != width){
                        drawing << mapArray[y][x] << " ";
                    }else if(x == width){
                        drawing << std::endl;
                    }
                }
            }
            drawing.close();
            break;
        }

        case '2':{
            std::ofstream drawing2("drawing2.txt");

            for(int y = 0; y < height; y++){
                for(int x = 0; x <= width; x++){
                    if(x != width){
                        drawing2 << mapArray[y][x] << " ";
                    }else if(x == width){
                        drawing2 << std::endl;
                    }
                }
            }
            drawing2.close();
            break;
        }

        case '3':{
            std::ofstream drawing3("drawing3.txt");

            for(int y = 0; y < height; y++){
                for(int x = 0; x <= width; x++){
                    if(x != width){
                        drawing3 << mapArray[y][x] << " ";
                    }else if(x == width){
                        drawing3 << std::endl;
                    }
                }
            }
            drawing3.close();
            break;
        }

        default:
            break;
    }
}

void load(){

    timeout(-1);

    clear();

    int choice;

    mvprintw(0,0,"What savefile slot do you want to load from?");

    mvprintw(5,0,"Slot - 1");

    mvprintw(7,0,"Slot - 2");

    mvprintw(9,0,"Slot - 3");

    mvprintw(11,0,"Press any other key to cancel the loading");

    choice = getch();

    switch(choice){
        case '1':{
            std::ifstream drawing("drawing.txt");

            for(int y = 0; y < height; y++){
                for(int x = 0; x <= width; x++){
                    if(x != width){
                        drawing >> mapArray[y][x];
                    }
                }
            }
            drawing.close();
            break;
        }

        case '2':{
            std::ifstream drawing2("drawing2.txt");

            for(int y = 0; y < height; y++){
                for(int x = 0; x <= width; x++){
                    if(x != width){
                        drawing2 >> mapArray[y][x];
                    }
                }
            }
            drawing2.close();
            break;
        }

        case '3':{
            std::ifstream drawing3("drawing3.txt");

            for(int y = 0; y < height; y++){
                for(int x = 0; x <= width; x++){
                    if(x != width){
                        drawing3 >> mapArray[y][x];
                    }
                }
            }
            drawing3.close();
            break;
        }

        default:
            break;
    }
}
