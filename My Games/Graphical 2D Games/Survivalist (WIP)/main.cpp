#include "Engine.h"
#include "MenuState.h"
#include "GameState.h"
#include "PlayState.h"

//Initalization +
struct TILE_TYPE{
	bool isPassable;
};

bool isPassable(float x, float y, float width, float height, float deltaX, float deltaY);
bool checkCollision(float x, float y, float ex, float ey, float width, float height, float ewidth, float eheight);
bool insideMap(float x, float y, float width, float height);
void addButtonToList(Button *newButton);
void addMissileToList(MissileEntity *newMissile);
void addLivingToList(LivingEntity *newLiving);
void addDebugToList(DebugClass *newDebug);
void loadMapArray();
void saveMapArray();
void drawMap();
void drawTile(float x, float y, int tileId);
void updateCamera();

ALLEGRO_DISPLAY *display;

ALLEGRO_FONT *defaultFont;
ALLEGRO_BITMAP *cursorImage;
ALLEGRO_BITMAP *playerImage;
ALLEGRO_BITMAP *bulletImage;
ALLEGRO_BITMAP *pixelImage;
ALLEGRO_BITMAP *zombieImage;

ALLEGRO_KEYBOARD_STATE keyState;
ALLEGRO_MOUSE_STATE mouseState;
ALLEGRO_EVENT_QUEUE *event_queue;
ALLEGRO_TIMER *timer;

Button *buttonList[MAX_BUTTONS];
MissileEntity *missileList[MAX_MISSILES];
LivingEntity *livingList[MAX_LIVING];
DebugClass *debugList[MAX_DEBUG];
Player *playerEntity;

Engine engine;

int mapArray[mapArrayWidth][mapArrayHeight];

float fpsTimeNew, fpsCounter, fpsTimeOld;

bool drawScreen, timerEvent, done, mouseButtonLeft, mouseButtonLeftClick, mouseButtonRight, mouseButtonRightClick, inGame;
float mouseX, mouseY;
int lastKeyPress, mouseWheel = 0;
float cameraPosX, cameraPosY, cameraOffsetX, cameraOffsetY, mapDisplayOffsetX, mapDisplayOffsetY;

TILE_TYPE tileIndex[] = {
	{true}, // (0) TILE_EMPTY
	{false}, // (1) TILE_WALL
};
//Initalization -

int main(){
    srand(time(0));

	Engine engine;

	//Initialize the Engine
	engine.init("GAME ENGINE - CHANGE THIS", screenWidth, screenHeight, false);

	//Load the Menu
	engine.changeState(PlayState::instance());

    //Timestep Variables
    double t = 0.0;
    double dt = 1/60.0;

    double currentTime = 0.0;
    double newTime = 0.0;
    double frameTime = 0.0;

    double accumulator = 0.0;

	//Main Loop
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
            }

            if(al_mouse_button_down(&mouseState, 2)){
                if(!mouseButtonRight)
                    mouseButtonRightClick = true;
                else
                    mouseButtonRightClick = false;
                mouseButtonRight = true;
            }else{
                mouseButtonRight = false;
            }
            //Update Mouse Variables -

            //Rest +
            if(!al_key_down(&keyState, lastKeyPress)){
                lastKeyPress = 0;
            }
            //Rest -

            if(events.timer.source == timer){
                newTime = al_get_time();
                frameTime = newTime - currentTime;
                if(frameTime > 0.25)
                    frameTime = 0.25;	  // note: max frame time to avoid spiral of death
                currentTime = newTime;

                accumulator += frameTime;

                while(accumulator >= dt){
                    engine.update(); //Call the gameState specfic update
                    t += dt;
                    accumulator -= dt;
                }

                engine.draw();

                al_draw_bitmap(cursorImage, mouseX-4, mouseY-4, 0);

                al_flip_display();
                al_clear_to_color(al_map_rgb(0, 0, 0));
            }
        }
        //Main Timer Event -
    }
	// cleanup the engine
	engine.cleanup();

}

bool isPassable(float x, float y, float width, float height){
    if(!insideMap(x, y, width, height)){
        return false;
    }else{
        int tX = floor(x/tileSize), tY = floor(y/tileSize), tWX = floor((x+width-1)/tileSize), tHY = floor((y+height-1)/tileSize);

        if(!tileIndex[mapArray[tX][tY]].isPassable || !tileIndex[mapArray[tWX][tY]].isPassable || !tileIndex[mapArray[tX][tHY]].isPassable || !tileIndex[mapArray[tWX][tHY]].isPassable){
            return false;
        }
    }
    return true;
}

bool checkCollision(float x, float y, float ex, float ey, float width, float height, float ewidth, float eheight){
    if(x + width - 1 < ex || x > ewidth + ex - 1 || y + height - 1 < ey || y > eheight + ey - 1){
        return false;
    }else{
        return true;
    }
}

bool insideMap(float x, float y, float width, float height){
    if(x < 0 || x + width-1 >= mapArrayWidth*tileSize || y < 0 || y + height-1 >= mapArrayHeight*tileSize){
        return false;
    }
    return true;
}

void addButtonToList(Button *newButton){
    for(int i = 0; i < MAX_BUTTONS; i++){
        if(buttonList[i] == NULL || !buttonList[i]->checkActive()){
            buttonList[i] = newButton;
            return;
        }
    }
}

void addMissileToList(MissileEntity *newMissile){
    for(int i = 0; i < MAX_MISSILES; i++){
        if(missileList[i] == NULL || !missileList[i]->checkActive()){
            missileList[i] = newMissile;
            return;
        }
    }
}

void addLivingToList(LivingEntity *newLiving){
    for(int i = 0; i < MAX_LIVING; i++){
        if(livingList[i] == NULL || !livingList[i]->checkActive()){
            livingList[i] = newLiving;
            return;
        }
    }
}

void addDebugToList(DebugClass *newDebug){
    for(int i = 0; i < MAX_DEBUG; i++){
        if(debugList[i] == NULL || !debugList[i]->checkActive()){
            debugList[i] = newDebug;
            return;
        }
    }
}

void loadMapArray(){
    ifstream mapArrayFile;
    mapArrayFile.open("config/MapArray.txt");

    for(int y = 0; y < mapArrayHeight; y++){
        for(int x = 0; x < mapArrayWidth; x++){
            mapArrayFile >> mapArray[x][y];
        }
    }

    mapArrayFile.close();

    printf("MapArray Loaded\n");
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
    for(int x = 0; x < mapDisplayWidth/tileSize+1; x++){
        for(int y = 0; y < mapDisplayHeight/tileSize+1; y++){
            int mapOffsetX = cameraPosX/tileSize, mapOffsetY = cameraPosY/tileSize;
            drawTile(x+mapDisplayOffsetX/tileSize, y+mapDisplayOffsetY/tileSize, mapArray[x+mapOffsetX][y+mapOffsetY]);
        }
    }
}

void drawTile(float x, float y, int tileId){
    switch(tileId){
        case 0:
            al_draw_filled_rectangle(x*tileSize, y*tileSize, (x+1)*tileSize, (y+1)*tileSize, al_map_rgb(150, 150, 150));
            break;

        case 1:
            al_draw_filled_rectangle(x*tileSize, y*tileSize, (x+1)*tileSize, (y+1)*tileSize, al_map_rgb(75, 75, 75));
            break;
    }
}

void updateCamera(){
    cameraPosX = playerEntity->posX-mapDisplayWidth/2+playerEntity->width/2;
    cameraPosY = playerEntity->posY-mapDisplayHeight/2+playerEntity->height/2;

    if(cameraPosX < 0){
        cameraPosX = 0;
    }
    if(cameraPosX + mapDisplayWidth >= mapArrayWidth*tileSize){
        cameraPosX = mapArrayWidth*tileSize - mapDisplayWidth;
    }
    if(cameraPosY < 0){
        cameraPosY = 0;
    }
    if(cameraPosY + mapDisplayHeight >= mapArrayHeight*tileSize){
        cameraPosY = mapArrayHeight*tileSize - mapDisplayHeight;
    }

    int cX = cameraPosX, cY = cameraPosY;

    mapDisplayOffsetX = -(cX%tileSize);
    mapDisplayOffsetY = -(cY%tileSize);

    cameraOffsetX = -cameraPosX;
    cameraOffsetY = -cameraPosY;
}
