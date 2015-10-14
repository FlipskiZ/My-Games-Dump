#include "Engine.h"
#include "MenuState.h"
#include "GameState.h"
#include "PlayState.h"

//Initalization +
struct TILE_TYPE{
	bool isPassable;
	int directional;
};

bool isPassable(float x, float y, float width, float height, float deltaX, float deltaY);
bool directionalTileCheck(int direction, float deltaX, float deltaY);
bool checkCollision(int x, int y, int ex, int ey, int width, int height, int ewidth, int eheight);
bool insideMap(int x, int y, int width, int height);
void addBulletToList(Bullet *newBullet);
void addZombieToList(Zombie *newZombie);
void addButtonToList(Button *newButton);
void loadMapArray();
void saveMapArray();
void drawMap();
void drawTile(int x, int y, int tileId);
int useMapTile(float x, float y, float width, float height);
void nextWave();

ALLEGRO_DISPLAY *display;

ALLEGRO_FONT *defaultFont;
ALLEGRO_FONT *waveFont;
ALLEGRO_BITMAP *cursorImage;
ALLEGRO_BITMAP *attackImage;
ALLEGRO_BITMAP *zombieImage;
ALLEGRO_BITMAP *playerImage;
ALLEGRO_BITMAP *bulletImage;
ALLEGRO_BITMAP *concreteImage;
ALLEGRO_BITMAP *cobblestoneImage;
ALLEGRO_BITMAP *mossCobblestoneImage;
ALLEGRO_BITMAP *downDirectionalImage;
ALLEGRO_BITMAP *leftDirectionalImage;
ALLEGRO_BITMAP *upDirectionalImage;
ALLEGRO_BITMAP *rightDirectionalImage;
ALLEGRO_BITMAP *spawnerImage;
ALLEGRO_BITMAP *shopImage;
ALLEGRO_BITMAP *plankDImage;
ALLEGRO_BITMAP *plankVImage;

ALLEGRO_KEYBOARD_STATE keyState;
ALLEGRO_MOUSE_STATE mouseState;
ALLEGRO_EVENT_QUEUE *event_queue;
ALLEGRO_TIMER *timer;

Bullet *bulletList[MAX_BULLETS];
Zombie *zombieList[MAX_ZOMBIES];
Button *buttonList[MAX_BUTTONS];

int mapArray[mapWidth/tileSize][mapHeight/tileSize];

bool playerColX, playerColY;
float fpsTimeNew, fpsCounter, fpsTimeOld;

bool drawScreen, done, mouseButtonLeft, mouseButtonLeftClick, mouseButtonRight, mouseButtonRightClick, inGame, inEditor, inShop, pauseBetweenWavesCheck, enemySpawnAll;
float mouseX, mouseY;
int lastKeyPress, mouseWheel = 0;
float playerX, playerY, playerWidth, playerHeight, playerCenterX, playerCenterY, playerMovementSpeed, playerDeltaX, playerDeltaY;
float playerHealth, playerMaxHealth;
int playerFiringSpeed, playerFiringSpeedHelper, playerShots, playerDamage, playerMoney;
float playerBulletSpeed, playerBulletSpread;
int enemySpawnRate, enemySpawnRateHelper;
float enemyHealth, enemySpeed, enemyDamage, enemyAttackTime, enemyKillMoney;
int enemyWave, amountEnemiesSpawn, amountEnemiesSpawned, pauseBetweenWaves, pauseBetweenWavesHelper;

TILE_TYPE tileIndex[] = {
	{true, -1}, // (0) TILE_EMPTY
	{false, -1}, // (1) TILE_WALL
	{false, -1}, // (2) TILE_WALL2
	{true, 0}, // (3) TILE_DIRECTIONAL_UP
	{true, 1}, // (4) TILE_DIRECTIONAL_RIGHT
	{true, 2}, // (5) TILE_DIRECTIONAL_DOWN
	{true, 3}, // (6) TILE_DIRECTIONAL_LEFT
	{true, -1}, // (7) TILE_SPAWNER
	{true, -1}, // (8) TILE_SHOP
	{true, -1}, // (9) TILE_PLANK_D
	{true, -1}, // (10) TILE_PLANK_V
};
//Initalization -

int main(){
    srand(time(0));

	Engine engine;

	// initialize the engine
	engine.init("Twin-Stick Shooter", screenWidth, screenHeight, false);

	// load the intro
	engine.changeState(MenuState::instance());

	// main loop
        while(engine.running()){
            al_get_mouse_state(&mouseState);
            mouseWheel = al_get_mouse_state_axis(&mouseState, 2);
            engine.update();
            if(drawScreen){
                drawScreen = false;

                al_draw_bitmap(cursorImage, mouseX-4, mouseY-4, NULL);

                al_flip_display();
                al_clear_to_color(al_map_rgb(0, 0, 0));
            }
        }
	// cleanup the engine
	engine.cleanup();

}

bool isPassable(float x, float y, float width, float height, float deltaX, float deltaY){
    if(x < 0 || x + width > mapWidth || y < 0 || y + height > mapHeight){
        return false;
    }else{
        int tX = floor((x+1)/tileSize), tY = floor((y+1)/tileSize), tWX = floor((x+width-1)/tileSize), tHY = floor((y+height-1)/tileSize);

        if(!tileIndex[mapArray[tX][tY]].isPassable || !tileIndex[mapArray[tWX][tY]].isPassable || !tileIndex[mapArray[tX][tHY]].isPassable || !tileIndex[mapArray[tWX][tHY]].isPassable){
            return false;
        }else if(tileIndex[mapArray[tX][tY]].directional != -1 || tileIndex[mapArray[tWX][tY]].directional != -1 || tileIndex[mapArray[tX][tHY]].directional != -1 || tileIndex[mapArray[tWX][tHY]].directional != -1){
            if(tileIndex[mapArray[tX][tY]].directional != -1){
                if(directionalTileCheck(tileIndex[mapArray[tX][tY]].directional, deltaX, deltaY))
                    return false;
            }
            if(tileIndex[mapArray[tWX][tY]].directional != -1){
                if(directionalTileCheck(tileIndex[mapArray[tWX][tY]].directional, deltaX, deltaY))
                    return false;
            }
            if(tileIndex[mapArray[tX][tHY]].directional != -1){
                if(directionalTileCheck(tileIndex[mapArray[tX][tHY]].directional, deltaX, deltaY))
                    return false;
            }
            if(tileIndex[mapArray[tWX][tHY]].directional != -1){
                if(directionalTileCheck(tileIndex[mapArray[tWX][tHY]].directional, deltaX, deltaY))
                    return false;
            }
        }
    }
    return true;
}

bool directionalTileCheck(int direction, float deltaX, float deltaY){
    int playerDirection = -1;
    float pDeltaX = (deltaX < 0) ? -deltaX : deltaX;
    float pDeltaY = (deltaY < 0) ? -deltaY : deltaY;

    if(pDeltaY > pDeltaX && deltaY > 0){
        playerDirection = 0;
    }else if(pDeltaY < pDeltaX && deltaX < 0){
        playerDirection = 1;
    }else if(pDeltaY > pDeltaX && deltaY < 0){
        playerDirection = 2;
    }else if(pDeltaY < pDeltaX && deltaX > 0){
        playerDirection = 3;
    }
    if(playerDirection == direction){
        return false;
    }else{
        return true;
    }
}

bool checkCollision(int x, int y, int ex, int ey, int width, int height, int ewidth, int eheight){
    if(x + width < ex || x > ewidth + ex || y + height < ey || y > eheight + ey){
        return false;
    }else{
        return true;
    }
}

bool insideMap(int x, int y, int width, int height){
    if(x < 0 || x + width >= mapWidth || y < 0 || y + height >= mapHeight){
        return false;
    }
    return true;
}

void addBulletToList(Bullet *newBullet){
    for(int i = 0; i < MAX_BULLETS; i++){
        if(bulletList[i] == NULL || !bulletList[i]->checkActive()){
            bulletList[i] = newBullet;
            return;
        }
    }
}

void addZombieToList(Zombie *newZombie){
    for(int i = 0; i < MAX_ZOMBIES; i++){
        if(zombieList[i] == NULL || !zombieList[i]->checkActive()){
            newZombie->setPointerListId(i);
            zombieList[i] = newZombie;
            return;
        }
    }
}

void addButtonToList(Button *newButton){
    for(int i = 0; i < MAX_BUTTONS; i++){
        if(buttonList[i] == NULL || !buttonList[i]->checkActive()){
            buttonList[i] = newButton;
            return;
        }
    }
}

void loadMapArray(){
    ifstream mapArrayFile;
    mapArrayFile.open("MapArray.txt");

    for(int y = 0; y < mapHeight/tileSize; y++){
        for(int x = 0; x < mapWidth/tileSize; x++){
            mapArrayFile >> mapArray[x][y];
        }
    }

    mapArrayFile.close();

    printf("MapArray Loaded\n");
}

void saveMapArray(){
    ofstream mapArrayFile;
    mapArrayFile.open("MapArray.txt");

    for(int y = 0; y < mapHeight/tileSize; y++){
        for(int x = 0; x <= mapWidth/tileSize; x++){
            if(x < mapWidth/tileSize){
                mapArrayFile << mapArray[x][y] << " ";
            }else if(x == mapWidth/tileSize){
                mapArrayFile << endl;
            }
        }
    }

    mapArrayFile.close();

    printf("MapArray Saved\n");
}

void drawMap(){
    for(int x = 0; x < mapWidth/tileSize; x++){
        for(int y = 0; y < mapHeight/tileSize; y++){
            drawTile(x*tileSize, y*tileSize, mapArray[x][y]);
        }
    }
}

void drawTile(int x, int y, int tileId){
    switch(tileId){
        case 0:
            al_draw_bitmap(concreteImage, x, y, NULL);
            break;

        case 1:
            al_draw_bitmap(cobblestoneImage, x, y, NULL);
            break;

        case 2:
            al_draw_bitmap(mossCobblestoneImage, x, y, NULL);
            break;

        case 3:
            al_draw_bitmap(downDirectionalImage, x, y, NULL);
            break;

        case 4:
            al_draw_bitmap(leftDirectionalImage, x, y, NULL);
            break;

        case 5:
            al_draw_bitmap(upDirectionalImage, x, y, NULL);
            break;

        case 6:
            al_draw_bitmap(rightDirectionalImage, x, y, NULL);
            break;

        case 7:
            al_draw_bitmap(spawnerImage, x, y, NULL);
            break;

        case 8:
            al_draw_bitmap(shopImage, x, y, NULL);
            break;

        case 9:
            al_draw_bitmap(plankDImage, x, y, NULL);
            break;

        case 10:
            al_draw_bitmap(plankVImage, x, y, NULL);
            break;
    }
}

int useMapTile(float x, float y, float width, float height){
    int tX = floor((x+1)/tileSize), tY = floor((y+1)/tileSize), tWX = floor((x+width-1)/tileSize), tHY = floor((y+height-1)/tileSize);

    if(mapArray[tX][tY] == 8){
        return mapArray[tX][tY];
    }
    if(mapArray[tWX][tY] == 8){
        return mapArray[tWX][tY];
    }
    if(mapArray[tX][tHY] == 8){
        return mapArray[tX][tHY];
    }
    if(mapArray[tWX][tHY] == 8){
        return mapArray[tWX][tHY];
    }
}

void nextWave(){
    enemyWave += 1;
    if(enemyWave % 10 == 0){
        if(enemySpeed < 8){
            enemySpeed *= 2;
        }
        enemyKillMoney *= 2;
        enemyAttackTime -= 1;
    }
    if(enemyWave % 5 == 0){
        enemyDamage += 1;
    }
    if(enemyWave != 1 && enemyWave != 2 && enemyWave != 3){
        enemyHealth = enemyWave*enemyWave/12;
    }
    if(enemyWave % 2 == 0 && enemySpawnRate > 2){
        enemySpawnRate = floor(enemySpawnRate/1.3);
    }else if(enemySpawnRate <= 2 && !enemySpawnAll){
        enemySpawnAll = true;
        enemySpawnRate = 60;
        amountEnemiesSpawn /= 8;
    }else if(enemySpawnAll && enemySpawnRate > 1 && enemyWave % 2 == 0){
        enemySpawnRate = floor(enemySpawnRate/1.25);
    }
    amountEnemiesSpawn = ceil(amountEnemiesSpawn*1.05);
    amountEnemiesSpawned = 0;
}
