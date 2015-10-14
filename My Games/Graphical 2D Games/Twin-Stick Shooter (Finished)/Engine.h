#ifndef ENGINE_H
#define ENGINE_H

#include "Bullet.h"
#include "Zombie.h"
#include "Button.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <cstring>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <cstdio>
#include <vector>
#include <fstream>

#define screenWidth 1280
#define screenHeight 720

#define mapWidth 1280
#define mapHeight 608

#define tileSize 16

#define MAX_BULLETS 3000
#define MAX_ZOMBIES 1000
#define MAX_BUTTONS 100

#define FPS 60.0

using namespace std;

bool isPassable(float x, float y, float width, float height, float deltaX, float deltaY);
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

extern ALLEGRO_DISPLAY *display;

extern ALLEGRO_FONT *defaultFont;
extern ALLEGRO_FONT *waveFont;
extern ALLEGRO_BITMAP *cursorImage;
extern ALLEGRO_BITMAP *attackImage;
extern ALLEGRO_BITMAP *zombieImage;
extern ALLEGRO_BITMAP *playerImage;
extern ALLEGRO_BITMAP *bulletImage;
extern ALLEGRO_BITMAP *concreteImage;
extern ALLEGRO_BITMAP *cobblestoneImage;
extern ALLEGRO_BITMAP *mossCobblestoneImage;
extern ALLEGRO_BITMAP *downDirectionalImage;
extern ALLEGRO_BITMAP *leftDirectionalImage;
extern ALLEGRO_BITMAP *upDirectionalImage;
extern ALLEGRO_BITMAP *rightDirectionalImage;
extern ALLEGRO_BITMAP *spawnerImage;
extern ALLEGRO_BITMAP *shopImage;
extern ALLEGRO_BITMAP *plankDImage;
extern ALLEGRO_BITMAP *plankVImage;

extern ALLEGRO_KEYBOARD_STATE keyState;
extern ALLEGRO_MOUSE_STATE mouseState;
extern ALLEGRO_EVENT_QUEUE *event_queue;
extern ALLEGRO_TIMER *timer;

extern Bullet *bulletList[MAX_BULLETS];
extern Zombie *zombieList[MAX_ZOMBIES];
extern Button *buttonList[MAX_BUTTONS];

extern bool playerColX, playerColY;
extern float fpsTimeNew, fpsCounter, fpsTimeOld;

extern bool drawScreen, done, mouseButtonLeft, mouseButtonLeftClick, mouseButtonRight, mouseButtonRightClick, inGame, inEditor, inShop, pauseBetweenWavesCheck, enemySpawnAll;
extern float mouseX, mouseY;
extern int lastKeyPress, mouseWheel;
extern float playerX, playerY, playerWidth, playerHeight, playerCenterX, playerCenterY, playerMovementSpeed, playerDeltaX, playerDeltaY;
extern float playerHealth, playerMaxHealth;
extern int playerFiringSpeed, playerFiringSpeedHelper, playerShots, playerDamage, playerMoney;
extern float playerBulletSpeed, playerBulletSpread;
extern int enemySpawnRate, enemySpawnRateHelper;
extern float enemyHealth, enemySpeed, enemyDamage, enemyAttackTime, enemyKillMoney;
extern int enemyWave, amountEnemiesSpawn, amountEnemiesSpawned, pauseBetweenWaves, pauseBetweenWavesHelper;

extern int mapArray[mapWidth/tileSize][mapHeight/tileSize];

class GameState;

class Engine{
    public:
        void init(const char* title, int width, int height, bool fullscreen);
        void cleanup();

        void changeState(GameState* state);
        void pushState(GameState* state);
        void popState();

        void update();
        void draw();

        bool running(){ return m_running; }
        void quit(){m_running = false;}

    private:
        // the stack of states
        vector<GameState*> states;

        bool m_running;
        bool m_fullscreen;
};

#endif // ENGINE_H
