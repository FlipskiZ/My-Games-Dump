#include "Unit.h"
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

#define screenWidth 1024
#define screenHeight 768

#define mapDisplayWidth 820
#define mapDisplayHeight 620

#define mapWidth 160
#define mapHeight 120
#define tileSize 20

#define MAX_UNITS 1000

bool isPassable(int x, int y);
void updateCamera();
void addUnitToList(Unit *newUnit);
bool checkInsideMapDisplay(int x, int y);

extern ALLEGRO_FONT *defaultFont;

extern Unit *unitList[MAX_UNITS];

extern bool draw, retry, done, mouseButtonLeft;
extern float mouseX, mouseY;
extern int lastKeyPress, playerMovementPoints, playerX, playerY, playerDeltaX, playerDeltaY;
extern int cameraPosX, cameraPosY, cameraOffsetX, cameraOffsetY;

extern int mapArray[mapWidth][mapHeight];
