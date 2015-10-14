#ifndef ENGINE_H
#define ENGINE_H

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

#include "Button.h"
#include "Player.h"
#include "PistolBullet.h"
#include "DebugClass.h"
#include "Zombie.h"

#define PI 3.14159265359

#define screenWidth 1280
#define screenHeight 720

#define mapDisplayWidth 1280
#define mapDisplayHeight 640

#define mapArrayWidth 80
#define mapArrayHeight 40

#define tileSize 32

#define MAX_BUTTONS 100
#define MAX_LIVING 200
#define MAX_MISSILES 200
#define MAX_DEBUG 200

#define FPS 60.0

using namespace std;

bool isPassable(float x, float y, float width, float height);
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

extern ALLEGRO_DISPLAY *display;

extern ALLEGRO_FONT *defaultFont;
extern ALLEGRO_BITMAP *cursorImage;
extern ALLEGRO_BITMAP *playerImage;
extern ALLEGRO_BITMAP *bulletImage;
extern ALLEGRO_BITMAP *pixelImage;
extern ALLEGRO_BITMAP *zombieImage;

extern ALLEGRO_KEYBOARD_STATE keyState;
extern ALLEGRO_MOUSE_STATE mouseState;
extern ALLEGRO_EVENT_QUEUE *event_queue;
extern ALLEGRO_TIMER *timer;

extern Button *buttonList[MAX_BUTTONS];
extern MissileEntity *missileList[MAX_MISSILES];
extern LivingEntity *livingList[MAX_LIVING];
extern DebugClass *debugList[MAX_DEBUG];
extern Player *playerEntity;

extern float fpsTimeNew, fpsCounter, fpsTimeOld;

extern bool drawScreen, timerEvent, done, mouseButtonLeft, mouseButtonLeftClick, mouseButtonRight, mouseButtonRightClick, inGame;
extern float mouseX, mouseY;
extern int lastKeyPress, mouseWheel;
extern float cameraPosX, cameraPosY, cameraOffsetX, cameraOffsetY, mapDisplayOffsetX, mapDisplayOffsetY;

extern int mapArray[mapArrayWidth][mapArrayHeight];

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
