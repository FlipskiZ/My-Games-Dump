#include "Engine.h"
#include "MenuState.h"
#include "GameState.h"
#include "PlayState.h"

//Initalization +
struct TILE_TYPE{
	int pieceOwner;
};

void selectPiece(float x, float y);
void movePiece(float x, float y);
int checkLegalMove(int x, int y, int targetX, int targetY, bool everyMove);
void updateGameArray();
void upgradePawn(int x, int y, int clickedId, bool whiteFigure);
bool checkCollision(float x, float y, float ex, float ey, float width, float height, float ewidth, float eheight);
void addButtonToList(Button *newButton);
void loadMapArray(bool defaultMap);
void saveMapArray();
void drawMap();
void drawTile(float x, float y, int tileId);

ALLEGRO_DISPLAY *display;

ALLEGRO_FONT *defaultFont;
ALLEGRO_BITMAP *chessTileSet;

ALLEGRO_KEYBOARD_STATE keyState;
ALLEGRO_MOUSE_STATE mouseState;
ALLEGRO_EVENT_QUEUE *event_queue;
ALLEGRO_TIMER *timer;

Button *buttonList[MAX_BUTTONS];

float fpsTimeNew, fpsCounter, fpsTimeOld;

bool drawScreen, timerEvent, done, mouseButtonLeft, mouseButtonLeftClick, mouseButtonRight, mouseButtonRightClick, inGame, whiteTurn, whiteCheck, blackCheck, whiteCheckmate, blackCheckmate, upgradingPawn;
int selectedPieceX, selectedPieceY;
float mouseX, mouseY;
int lastKeyPress, mouseWheel = 0;

int mapArray[mapArrayWidth][mapArrayHeight];
int gameArray[mapArrayWidth][mapArrayHeight][2];
bool avaliableCastling[3][2];

TILE_TYPE tileIndex[] = {
	{0}, // (0) TILE_EMPTY
	{1}, // (1) TILE_WKING
	{1}, // (2) TILE_WQUEEN
	{1}, // (3) TILE_WTOWER
	{1}, // (4) TILE_WKNIGHT
	{1}, // (5) TILE_WDIAGONAL
	{1}, // (6) TILE_WPAWN
	{2}, // (7) TILE_BKING
	{2}, // (8) TILE_BQUEEN
	{2}, // (9) TILE_BTOWER
	{2}, // (10) TILE_BKNIGHT
	{2}, // (11) TILE_BDIAGONAL
	{2}, // (12) TILE_BPAWN

};
//Initalization -

int main(){
    srand(time(0));

	Engine engine;

	// initialize the engine
	engine.init("Chess Ultra Deluxe Edition 3000", screenWidth, screenHeight, false);

	// load the intro
	engine.changeState(MenuState::instance());

	// main loop
        while(engine.running()){
            ALLEGRO_EVENT events;
            al_wait_for_event(event_queue, &events);

            timerEvent = false;

            if(events.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
                engine.quit();
            }

            if(events.type == ALLEGRO_EVENT_TIMER){
                timerEvent = true;
            }
            //Main Timer Event +
            if(timerEvent){
                al_get_mouse_state(&mouseState);

                al_get_keyboard_state(&keyState);

                //Update Mouse Variables +
                mouseX = al_get_mouse_state_axis(&mouseState,0);
                mouseY = al_get_mouse_state_axis(&mouseState,1);
                mouseWheel = al_get_mouse_state_axis(&mouseState, 2);

                if(al_mouse_button_down(&mouseState, 1)){
                    if(!mouseButtonLeft)
                        mouseButtonLeftClick = true;
                    else
                        mouseButtonLeftClick = false;
                    mouseButtonLeft = true;
                }else{
                    mouseButtonLeft = false;
                    mouseButtonLeftClick = false;
                }
                //Update Mouse Variables -

                //Rest +
                if(!al_key_down(&keyState, lastKeyPress)){
                    lastKeyPress = NULL;
                }
                //Rest -
            }
            //Main Timer Event -

            engine.update(); //Call the gameState specfic update

            if(drawScreen){
                drawScreen = false;

                al_flip_display();
                al_clear_to_color(al_map_rgb(0, 0, 0));
            }

        }
	// cleanup the engine
	engine.cleanup();

}

void selectPiece(float x, float y){
    int tX = floor((x)/tileSize), tY = floor((y)/tileSize);

    if(tX < 0 || tX >= mapArrayWidth || tY < 0 || tY >= mapArrayHeight){
        return;
    }

    if(selectedPieceX == tX && selectedPieceY == tY)
        selectedPieceX = -1, selectedPieceY = -1;
    else if((tileIndex[mapArray[tX][tY]].pieceOwner == 1 && whiteTurn) || (tileIndex[mapArray[tX][tY]].pieceOwner == 2 && !whiteTurn))
        selectedPieceX = tX, selectedPieceY = tY;
}

void movePiece(float x, float y){
    int tX = floor((x)/tileSize), tY = floor((y)/tileSize);

    if(tX < 0 || tX >= mapArrayWidth || tY < 0 || tY >= mapArrayHeight){
        return;
    }

    if(tileIndex[mapArray[tX][tY]].pieceOwner == tileIndex[mapArray[selectedPieceX][selectedPieceY]].pieceOwner){
        selectPiece(tX*tileSize, tY*tileSize);
        return;
    }

    int legalCheck = checkLegalMove(selectedPieceX, selectedPieceY, tX, tY, false);

    if(legalCheck != 0){
        if(mapArray[selectedPieceX][selectedPieceY] == 1 || mapArray[selectedPieceX][selectedPieceY] == 3 || mapArray[selectedPieceX][selectedPieceY] == 7 || mapArray[selectedPieceX][selectedPieceY] == 9 ){
            if(selectedPieceX == 0){
                avaliableCastling[0][tileIndex[mapArray[selectedPieceX][selectedPieceY]].pieceOwner-1] = false;
            }else if(selectedPieceX == 4){
                avaliableCastling[1][tileIndex[mapArray[selectedPieceX][selectedPieceY]].pieceOwner-1] = false;
            }else if(selectedPieceX == 7){
                avaliableCastling[2][tileIndex[mapArray[selectedPieceX][selectedPieceY]].pieceOwner-1] = false;
            }
        }

        if(legalCheck == 3){
            mapArray[selectedPieceX-1][selectedPieceY] = mapArray[selectedPieceX-4][selectedPieceY];
            mapArray[selectedPieceX-4][selectedPieceY] = 0;
        }else if(legalCheck == 4){
            mapArray[selectedPieceX+1][selectedPieceY] = mapArray[selectedPieceX+3][selectedPieceY];
            mapArray[selectedPieceX+3][selectedPieceY] = 0;
        }
        int replacedTile = mapArray[tX][tY];
        mapArray[tX][tY] = mapArray[selectedPieceX][selectedPieceY];
        mapArray[selectedPieceX][selectedPieceY] = 0;
        updateGameArray();

        for(int x = 0; x < 8; x++){
            for(int y = 0; y < 8; y++){
                if(mapArray[x][y] == 1 || mapArray[x][y] == 7){
                    if(mapArray[x][y] == 1){
                        if(gameArray[x][y][1] == 1){
                            if(whiteTurn && !blackCheck){
                                mapArray[selectedPieceX][selectedPieceY] = mapArray[tX][tY];
                                mapArray[tX][tY] = replacedTile;
                                whiteTurn = !whiteTurn;
                            }else if(!blackCheck){
                                whiteCheck = true;
                                whiteCheckmate = true;
                                for(int iX = 0; iX < 8; iX++){
                                    for(int iY = 0; iY < 8; iY++){
                                        if(mapArray[iX][iY] != 0 && tileIndex[mapArray[iX][iY]].pieceOwner == 1){
                                            for(int lX = 0; lX < 8; lX++){
                                                for(int lY = 0; lY < 8; lY++){
                                                    if(checkLegalMove(iX, iY, lX, lY, false) != 0){
                                                        replacedTile = mapArray[lX][lY];
                                                        mapArray[lX][lY] = mapArray[iX][iY];
                                                        mapArray[iX][iY] = 0;
                                                        updateGameArray();

                                                        if(mapArray[x][y] == 1){
                                                            if(gameArray[x][y][1] == 0){
                                                                whiteCheckmate = false;
                                                            }
                                                        }else{
                                                            for(int sX = 0; sX < 8; sX++){
                                                                for(int sY = 0; sY < 8; sY++){
                                                                    if(mapArray[sX][sY] == 1){
                                                                        if(gameArray[sX][sY][1] == 0){
                                                                            whiteCheckmate = false;
                                                                        }
                                                                    }
                                                                }
                                                            }
                                                        }

                                                        mapArray[iX][iY] = mapArray[lX][lY];
                                                        mapArray[lX][lY] = replacedTile;
                                                        if(!whiteCheckmate){
                                                            iX = 8, iY = 8, lX = 8, lY = 8, x = 8, y = 8;
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }else{
                            whiteCheck = false;
                        }
                    }else if(mapArray[x][y] == 7){
                        if(gameArray[x][y][0] == 1){
                            if(!whiteTurn && !whiteCheck){
                                mapArray[selectedPieceX][selectedPieceY] = mapArray[tX][tY];
                                mapArray[tX][tY] = replacedTile;
                                whiteTurn = !whiteTurn;
                            }else if(!whiteCheck){
                                blackCheck = true;
                                blackCheckmate = true;
                                for(int iX = 0; iX < 8; iX++){
                                    for(int iY = 0; iY < 8; iY++){
                                        if(mapArray[iX][iY] != 0 && tileIndex[mapArray[iX][iY]].pieceOwner == 2){
                                            for(int lX = 0; lX < 8; lX++){
                                                for(int lY = 0; lY < 8; lY++){
                                                    if(checkLegalMove(iX, iY, lX, lY, false) != 0){
                                                        replacedTile = mapArray[lX][lY];
                                                        mapArray[lX][lY] = mapArray[iX][iY];
                                                        mapArray[iX][iY] = 0;
                                                        updateGameArray();

                                                        if(mapArray[x][y] == 7){
                                                            if(gameArray[x][y][0] == 0){
                                                                blackCheckmate = false;
                                                            }
                                                        }else{
                                                            for(int sX = 0; sX < 8; sX++){
                                                                for(int sY = 0; sY < 8; sY++){
                                                                    if(mapArray[sX][sY] == 7){
                                                                        if(gameArray[sX][sY][0] == 0){
                                                                            blackCheckmate = false;
                                                                        }
                                                                    }
                                                                }
                                                            }
                                                        }
                                                        mapArray[iX][iY] = mapArray[lX][lY];
                                                        mapArray[lX][lY] = replacedTile;
                                                        if(!blackCheckmate){
                                                            iX = 8, iY = 8, lX = 8, lY = 8, x = 8, y = 8;
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }else{
                            blackCheck = false;
                        }
                    }
                }
            }
        }
        if((mapArray[tX][tY] == 6 || mapArray[tX][tY] == 12) && ((whiteTurn && !whiteCheck) || (!whiteTurn && !blackCheck)) && ((whiteTurn && tY == 0) || (!whiteTurn && tY == 7)))
            upgradePawn(tX, tY, 0, (mapArray[tX][tY] == 6) ? true : false);

        selectedPieceX = -1, selectedPieceY = -1;

        whiteTurn = !whiteTurn;

        updateGameArray();
    }
}

int checkLegalMove(int x, int y, int targetX, int targetY, bool everyMove){
    int chessPiece = mapArray[x][y];
    bool contUp = true, contUpRight = true, contRight = true, contDownRight = true, contDown = true, contDownLeft = true, contLeft = true, contUpLeft = true;

    if(tileIndex[mapArray[x][y]].pieceOwner == 2)
        chessPiece -= 6;

    switch(chessPiece){
        case 1:
            for(int lX = -1; lX <= 1; lX++){
                for(int lY = -1; lY <= 1; lY++){
                    if(x+lX == targetX && y+lY == targetY){
                        if(tileIndex[mapArray[x+lX][y+lY]].pieceOwner != tileIndex[mapArray[x][y]].pieceOwner || everyMove){
                            if((tileIndex[mapArray[x][y]].pieceOwner == 1 && gameArray[x+lX][y+lY][1] == 0) || (tileIndex[mapArray[x][y]].pieceOwner == 2 && gameArray[x+lX][y+lY][0] == 0)){
                                if(tileIndex[mapArray[x+lX][y+lY]].pieceOwner == 0)
                                    return 1;
                                else
                                    return 2;
                            }
                        }
                    }
                }
            }
            for(int i = 1; i <= 2; i++){
                if(contLeft){
                    if(tileIndex[mapArray[x-i][y]].pieceOwner != 0 || ((gameArray[x-i][y][1] == 1 || gameArray[x][y][1] == 1) && tileIndex[mapArray[x][y]].pieceOwner == 1) || ((gameArray[x-i][y][0] == 1 || gameArray[x][y][0] == 1) && tileIndex[mapArray[x][y]].pieceOwner == 2)){
                        contLeft = false;
                    }
                    if(tileIndex[mapArray[x-i][y]].pieceOwner == 0){
                        if(y == targetY && x-i == targetX){
                            if((avaliableCastling[1][tileIndex[mapArray[x][y]].pieceOwner-1] && (targetX == 2 && avaliableCastling[0][tileIndex[mapArray[x][y]].pieceOwner-1])) && ((tileIndex[mapArray[x][y]].pieceOwner == 1 && gameArray[x-i][y][1] == 0) || (tileIndex[mapArray[x][y]].pieceOwner == 2 && gameArray[x-i][y][0] == 0))){
                                return 3;
                            }
                        }
                    }
                }
                if(contRight){
                    if(tileIndex[mapArray[x+i][y]].pieceOwner != 0 || ((gameArray[x+i][y][1] == 1 || gameArray[x][y][1] == 1) && tileIndex[mapArray[x][y]].pieceOwner == 1) || ((gameArray[x+i][y][0] == 1 || gameArray[x][y][0] == 1) && tileIndex[mapArray[x][y]].pieceOwner == 2)){
                        contRight = false;
                    }
                    if(tileIndex[mapArray[x+i][y]].pieceOwner == 0){
                        if(y == targetY && x+i == targetX){
                            if((avaliableCastling[1][tileIndex[mapArray[x][y]].pieceOwner-1] && (targetX == 6 && avaliableCastling[2][tileIndex[mapArray[x][y]].pieceOwner-1])) && ((tileIndex[mapArray[x][y]].pieceOwner == 1 && gameArray[x+i][y][1] == 0) || (tileIndex[mapArray[x][y]].pieceOwner == 2 && gameArray[x+i][y][0] == 0))){
                                return 4;
                            }
                        }
                    }
                }
            }
            break;

        case 2:
            for(int i = 1; i < 8; i++){
                if(y-i < 0) contUp = false; if(y-i < 0 || x+i >= mapArrayWidth) contUpRight = false; if(x+i >= mapArrayWidth) contRight = false; if(y+i >= mapArrayHeight || x+i >= mapArrayWidth) contDownRight = false; if(y+i >= mapArrayHeight) contDown = false; if(y+i >= mapArrayHeight || x-i < 0) contDownLeft = false; if(x-i < 0) contLeft = false; if(y-i < 0 || x-i < 0) contUpLeft = false;
                if(contUp){
                    if(tileIndex[mapArray[x][y-i]].pieceOwner != 0){
                        contUp = false;
                    }
                    if(tileIndex[mapArray[x][y-i]].pieceOwner != tileIndex[mapArray[x][y]].pieceOwner || everyMove){
                        if(y-i == targetY && x == targetX){
                            if(contUp)
                                return 1;
                            else
                                return 2;
                        }
                    }
                }
                if(contUpRight){
                    if(tileIndex[mapArray[x+i][y-i]].pieceOwner != 0){
                        contUpRight = false;
                    }
                    if(tileIndex[mapArray[x+i][y-i]].pieceOwner != tileIndex[mapArray[x][y]].pieceOwner || everyMove){
                        if(y-i == targetY && x+i == targetX){
                            if(contUpRight)
                                return 1;
                            else
                                return 2;
                        }
                    }
                }
                if(contRight){
                    if(tileIndex[mapArray[x+i][y]].pieceOwner != 0){
                        contRight = false;
                    }
                    if(tileIndex[mapArray[x+i][y]].pieceOwner != tileIndex[mapArray[x][y]].pieceOwner || everyMove){
                        if(y == targetY && x+i == targetX){
                            if(contRight)
                                return 1;
                            else
                                return 2;
                        }
                    }
                }
                if(contDownRight){
                    if(tileIndex[mapArray[x+i][y+i]].pieceOwner != 0){
                        contDownRight = false;
                    }
                    if(tileIndex[mapArray[x+i][y+i]].pieceOwner != tileIndex[mapArray[x][y]].pieceOwner || everyMove){
                        if(y+i == targetY && x+i == targetX){
                            if(contDownRight)
                                return 1;
                            else
                                return 2;
                        }
                    }
                }
                if(contDown){
                    if(tileIndex[mapArray[x][y+i]].pieceOwner != 0){
                        contDown = false;
                    }
                    if(tileIndex[mapArray[x][y+i]].pieceOwner != tileIndex[mapArray[x][y]].pieceOwner || everyMove){
                        if(y+i == targetY && x == targetX){
                            if(contDown)
                                return 1;
                            else
                                return 2;
                        }
                    }
                }
                if(contDownLeft){
                    if(tileIndex[mapArray[x-i][y+i]].pieceOwner != 0){
                        contDownLeft = false;
                    }
                    if(tileIndex[mapArray[x-i][y+i]].pieceOwner != tileIndex[mapArray[x][y]].pieceOwner || everyMove){
                        if(y+i == targetY && x-i == targetX){
                            if(contDownLeft)
                                return 1;
                            else
                                return 2;
                        }
                    }
                }
                if(contLeft){
                    if(tileIndex[mapArray[x-i][y]].pieceOwner != 0){
                        contLeft = false;
                    }
                    if(tileIndex[mapArray[x-i][y]].pieceOwner != tileIndex[mapArray[x][y]].pieceOwner || everyMove){
                        if(y == targetY && x-i == targetX){
                            if(contLeft)
                                return 1;
                            else
                                return 2;
                        }
                    }
                }
                if(contUpLeft){
                    if(tileIndex[mapArray[x-i][y-i]].pieceOwner != 0){
                        contUpLeft = false;
                    }
                    if(tileIndex[mapArray[x-i][y-i]].pieceOwner != tileIndex[mapArray[x][y]].pieceOwner || everyMove){
                        if(y-i == targetY && x-i == targetX){
                            if(contUpLeft)
                                return 1;
                            else
                                return 2;
                        }
                    }
                }
            }
            break;

        case 3:
            for(int i = 1; i < 8; i++){
                if(y-i < 0) contUp = false; if(x+i >= mapArrayWidth) contRight = false; if(y+i >= mapArrayHeight) contDown = false; if(x-i < 0) contLeft = false; if(y-i < 0 || x-i < 0) contUpLeft = false;
                if(contUp){
                    if(tileIndex[mapArray[x][y-i]].pieceOwner != 0){
                        contUp = false;
                    }
                    if(tileIndex[mapArray[x][y-i]].pieceOwner != tileIndex[mapArray[x][y]].pieceOwner || everyMove){
                        if(y-i == targetY && x == targetX){
                            if(contUp)
                                return 1;
                            else
                                return 2;
                        }
                    }
                }
                if(contRight){
                    if(tileIndex[mapArray[x+i][y]].pieceOwner != 0){
                        contRight = false;
                    }
                    if(tileIndex[mapArray[x+i][y]].pieceOwner != tileIndex[mapArray[x][y]].pieceOwner || everyMove){
                        if(y == targetY && x+i == targetX){
                            if(contRight)
                                return 1;
                            else
                                return 2;
                        }
                    }
                }
                if(contDown){
                    if(tileIndex[mapArray[x][y+i]].pieceOwner != 0){
                        contDown = false;
                    }
                    if(tileIndex[mapArray[x][y+i]].pieceOwner != tileIndex[mapArray[x][y]].pieceOwner || everyMove){
                        if(y+i == targetY && x == targetX){
                            if(contDown)
                                return 1;
                            else
                                return 2;
                        }
                    }
                }
                if(contLeft){
                    if(tileIndex[mapArray[x-i][y]].pieceOwner != 0){
                        contLeft = false;
                    }
                    if(tileIndex[mapArray[x-i][y]].pieceOwner != tileIndex[mapArray[x][y]].pieceOwner || everyMove){
                        if(y == targetY && x-i == targetX){
                            if(contLeft)
                                return 1;
                            else
                                return 2;
                        }
                    }
                }
            }
            break;

        case 4:
            if(tileIndex[mapArray[targetX][targetY]].pieceOwner != tileIndex[mapArray[x][y]].pieceOwner || everyMove){
                if((x-1 == targetX && y-2 == targetY) || (x-2 == targetX && y-1 == targetY) || (x-1 == targetX && y+2 == targetY) || (x-2 == targetX && y+1 == targetY) || (x+1 == targetX && y-2 == targetY) || (x+2 == targetX && y-1 == targetY) || (x+1 == targetX && y+2 == targetY) || (x+2 == targetX && y+1 == targetY)){
                    if(tileIndex[mapArray[targetX][targetY]].pieceOwner == 0)
                        return 1;
                    else
                        return 2;
                }
            }
            break;

        case 5:
            for(int i = 1; i < 8; i++){
                if(y-i < 0 || x+i >= mapArrayWidth) contUpRight = false; if(y+i >= mapArrayHeight || x+i >= mapArrayWidth) contDownRight = false; if(y+i >= mapArrayHeight || x-i < 0) contDownLeft = false; if(y-i < 0 || x-i < 0) contUpLeft = false;
                if(contUpRight){
                    if(tileIndex[mapArray[x+i][y-i]].pieceOwner != 0){
                        contUpRight = false;
                    }
                    if(tileIndex[mapArray[x+i][y-i]].pieceOwner != tileIndex[mapArray[x][y]].pieceOwner || everyMove){
                        if(y-i == targetY && x+i == targetX){
                            if(contUpRight)
                                return 1;
                            else
                                return 2;
                        }
                    }
                }
                if(contDownRight){
                    if(tileIndex[mapArray[x+i][y+i]].pieceOwner != 0){
                        contDownRight = false;
                    }
                    if(tileIndex[mapArray[x+i][y+i]].pieceOwner != tileIndex[mapArray[x][y]].pieceOwner || everyMove){
                        if(y+i == targetY && x+i == targetX){
                            if(contDownRight)
                                return 1;
                            else
                                return 2;
                        }
                    }
                }
                if(contDownLeft){
                    if(tileIndex[mapArray[x-i][y+i]].pieceOwner != 0){
                        contDownLeft = false;
                    }
                    if(tileIndex[mapArray[x-i][y+i]].pieceOwner != tileIndex[mapArray[x][y]].pieceOwner || everyMove){
                        if(y+i == targetY && x-i == targetX){
                            if(contDownLeft)
                                return 1;
                            else
                                return 2;
                        }
                    }
                }
                if(contUpLeft){
                    if(tileIndex[mapArray[x-i][y-i]].pieceOwner != 0){
                        contUpLeft = false;
                    }
                    if(tileIndex[mapArray[x-i][y-i]].pieceOwner != tileIndex[mapArray[x][y]].pieceOwner || everyMove){
                        if(y-i == targetY && x-i == targetX){
                            if(contUpLeft)
                                return 1;
                            else
                                return 2;
                        }
                    }
                }
            }
            break;

        case 6:
            for(int i = 1; i <= 2; i++){
                if(tileIndex[mapArray[x][y]].pieceOwner == 1){
                    if(contUp){
                        if(tileIndex[mapArray[x][y-i]].pieceOwner != 0){
                            contUp = false;
                        }
                        if(y == 6){
                            if(tileIndex[mapArray[x][y-i]].pieceOwner == 0){
                                if(y-i == targetY && x == targetX)
                                    return 1;
                            }
                        }else{
                            if(tileIndex[mapArray[x][y-1]].pieceOwner == 0){
                                if(y-1 == targetY && x == targetX)
                                    return 1;
                            }
                        }
                    }
                    if(y-1 >= 0){
                        if(x-1 >= 0){
                            if((tileIndex[mapArray[x-1][y-1]].pieceOwner == 2 || everyMove) && y-1 == targetY && x-1 == targetX){
                                return 2;
                            }
                        }
                        if(x+1 < mapArrayWidth){
                            if((tileIndex[mapArray[x+1][y-1]].pieceOwner == 2 || everyMove) && y-1 == targetY && x+1 == targetX){
                                return 2;
                            }
                        }
                    }
                }else{
                    if(contDown){
                        if(tileIndex[mapArray[x][y+i]].pieceOwner != 0){
                            contDown = false;
                        }
                        if(y == 1){
                            if(tileIndex[mapArray[x][y+i]].pieceOwner == 0){
                                if(y+i == targetY && x == targetX)
                                    return 1;
                            }
                        }else{
                            if(tileIndex[mapArray[x][y+1]].pieceOwner == 0){
                                if(y+1 == targetY && x == targetX)
                                    return 1;
                            }
                        }
                    }
                    if(y+1 < mapArrayHeight){
                        if(x-1 >= 0){
                            if((tileIndex[mapArray[x-1][y+1]].pieceOwner == 1 || everyMove) && y+1 == targetY && x-1 == targetX){
                                return 2;
                            }
                        }
                        if(x+1 < mapArrayWidth){
                            if((tileIndex[mapArray[x+1][y+1]].pieceOwner == 1 || everyMove) && y+1 == targetY && x+1 == targetX){
                                return 2;
                            }
                        }
                    }
                }
            }
            break;


    }

    return 0;
}

void updateGameArray(){
    int legalMove = 0;

    for(int x = 0; x < 8; x++){
        for(int y = 0; y < 8; y++){
            gameArray[x][y][0] = 0;
            gameArray[x][y][1] = 0;
        }
    }

    for(int x = 0; x < 8; x++){
        for(int y = 0; y < 8; y++){
            if(mapArray[x][y] != 0){
                for(int lX = 0; lX < 8; lX++){
                    for(int lY = 0; lY < 8; lY++){
                        legalMove = checkLegalMove(x, y, lX, lY, true);
                        if(legalMove != 0 && ((mapArray[x][y] != 12 && mapArray[x][y] != 6) || legalMove > 1)){
                            gameArray[lX][lY][tileIndex[mapArray[x][y]].pieceOwner-1] = 1;
                        }
                    }
                }
            }
        }
    }
}

void upgradePawn(int tX, int tY, int clickedId, bool whiteFigure){
    if(clickedId == 0){
        float x = 0, y = 0, width = 128, height = 64;
        int buttonId = 0;

        for(int i = 0; i < 4; i++){
            buttonId = i;

            x = screenWidth/2-width/2-width*1.5+width*i, y = mapDisplayHeight/2-height/2;

            Button *newButton = new Button();
            newButton->setPos(x, y);
            newButton->setDimensions(width, height);
            newButton->setButtonId(buttonId+3);
            newButton->setStoredValues(tX, tY, whiteFigure);

            addButtonToList(newButton);
            upgradingPawn = true;
        }
    }else{
        if(whiteFigure){
            mapArray[tX][tY] = (clickedId == 6) ? 2 : clickedId;
        }else{
            mapArray[tX][tY] = (clickedId == 3) ? 9 : (clickedId == 4) ? 10 : (clickedId == 5) ? 11 : (clickedId == 6) ? 8 : 0;
        }
        for(int i = 0; i < MAX_BUTTONS; i++){
            buttonList[i] = NULL;
        }
        upgradingPawn = false;
    }
}

bool checkCollision(float x, float y, float ex, float ey, float width, float height, float ewidth, float eheight){
    if(x + width < ex || x > ewidth + ex || y + height < ey || y > eheight + ey){
        return false;
    }else{
        return true;
    }
}

void addButtonToList(Button *newButton){
    for(int i = 0; i < MAX_BUTTONS; i++){
        if(buttonList[i] == NULL || !buttonList[i]->checkActive()){
            newButton->setButtonPointerId(i);
            buttonList[i] = newButton;
            return;
        }
    }
}

void loadMapArray(bool defaultMap){
    if(!defaultMap){
        ifstream mapArrayFile;
        mapArrayFile.open("config/MapArray.txt");

        for(int y = 0; y < mapArrayHeight; y++){
            for(int x = 0; x < mapArrayWidth; x++){
                mapArrayFile >> mapArray[x][y];
            }
        }

        mapArrayFile.close();

        printf("MapArray Loaded\n");
    }else{
        ifstream defMapArrayFile;
        defMapArrayFile.open("config/DefMapArray.txt");

        for(int y = 0; y < mapArrayHeight; y++){
            for(int x = 0; x < mapArrayWidth; x++){
                defMapArrayFile >> mapArray[x][y];
            }
        }

        defMapArrayFile.close();

        printf("Default MapArray Loaded\n");
    }
}

void saveMapArray(){
    ofstream mapArrayFile;
    mapArrayFile.open("config/MapArray.txt");

    for(int y = 0; y < mapArrayHeight; y++){
        for(int x = 0; x <= mapArrayWidth; x++){
            if(x < mapArrayWidth){
                mapArrayFile << mapArray[x][y] << " ";
            }else if(x == mapArrayWidth){
                mapArrayFile << endl;
            }
        }
    }

    mapArrayFile.close();

    printf("MapArray Saved\n");
}

void drawMap(){
    int avaliableMove = 0;
    for(int x = 0; x < mapDisplayWidth/tileSize; x++){
        for(int y = 0; y < mapDisplayHeight/tileSize; y++){
            avaliableMove = 0;
            if(selectedPieceX != -1 && selectedPieceY != -1){
                for(int iX = 0; iX < mapArrayWidth; iX++){
                    for(int iY = 0; iY < mapArrayHeight; iY++){
                        avaliableMove = checkLegalMove(selectedPieceX, selectedPieceY, x, y, false);
                    }
                }
            }
            al_draw_filled_rectangle(x*tileSize, y*tileSize, (x+1)*tileSize, (y+1)*tileSize, (avaliableMove == 1) ? al_map_rgb(25, 225, 25) : (avaliableMove == 2) ? al_map_rgb(225, 25, 25) : (avaliableMove == 3 || avaliableMove == 4) ? al_map_rgb(225, 225, 50) : (((x == selectedPieceX && y == selectedPieceY) ? al_map_rgb(25, 225, 25) : ((x+y)%2==0) ? al_map_rgb(255, 255, 255) : al_map_rgb(0, 0, 0))));
            if(avaliableMove && (x+y)%2==1)
                al_draw_filled_rectangle(x*tileSize, y*tileSize, (x+1)*tileSize, (y+1)*tileSize, al_map_rgba(0, 0, 0, 50));
            drawTile(x*tileSize, y*tileSize, mapArray[x][y]);

        }
    }
   al_draw_filled_rectangle(0, mapDisplayHeight, screenWidth, screenHeight, al_map_rgb(50,50,50));
}

void drawTile(float x, float y, int tileId){
    if(tileId >= 1 && tileId <= 12){
        al_draw_bitmap_region(chessTileSet, 64*(tileId-1), 0, 64, 64, x, y, NULL);
    }
}
