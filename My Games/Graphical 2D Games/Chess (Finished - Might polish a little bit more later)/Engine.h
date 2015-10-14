#ifndef ENGINE_H
#define ENGINE_H

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

#define screenWidth 512
#define screenHeight 600

#define mapDisplayWidth 512
#define mapDisplayHeight 512

#define mapArrayWidth 8
#define mapArrayHeight 8

#define tileSize 64

#define MAX_BUTTONS 100

#define FPS 60.0

using namespace std;

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

extern ALLEGRO_DISPLAY *display;

extern ALLEGRO_FONT *defaultFont;
extern ALLEGRO_BITMAP *chessTileSet;

extern ALLEGRO_KEYBOARD_STATE keyState;
extern ALLEGRO_MOUSE_STATE mouseState;
extern ALLEGRO_EVENT_QUEUE *event_queue;
extern ALLEGRO_TIMER *timer;

extern Button *buttonList[MAX_BUTTONS];

extern float fpsTimeNew, fpsCounter, fpsTimeOld;

extern bool drawScreen, timerEvent, done, mouseButtonLeft, mouseButtonLeftClick, mouseButtonRight, mouseButtonRightClick, inGame, whiteTurn, whiteCheck, blackCheck, whiteCheckmate, blackCheckmate, upgradingPawn;
extern int selectedPieceX, selectedPieceY;
extern float mouseX, mouseY;
extern int lastKeyPress, mouseWheel;

extern int mapArray[mapArrayWidth][mapArrayHeight];
extern int gameArray[mapArrayWidth][mapArrayHeight][2];
extern bool avaliableCastling[3][2];

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
