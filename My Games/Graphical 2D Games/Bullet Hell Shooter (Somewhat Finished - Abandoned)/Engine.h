#ifndef ENGINE_H
#define ENGINE_H

#include "Enemy.h"
#include "Bullet.h"
#include "Pickup.h"
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_font.h>

#define screenWidth 1280
#define screenHeight 720

#define mapWidth 1280
#define mapHeight 620

#define shopTestWidth 500
#define shopTestHeight 280

#define MAX_ENEMIES 500
#define MAX_BULLETS 25000
#define MAX_BUTTONS 100
#define MAX_PICKUPS 200

#define MAX_PLAYER_HEALTH 25
#define MAX_PLAYER_SHOTS 20
#define MAX_PLAYER_DAMAGE 5
#define MIN_PLAYER_SHOT_SPEED 10

#define titleState 0
#define gameState 1
#define loseState 2
#define shopState 3

#define pickupAmount 7

//Allegro Variables -- START
extern ALLEGRO_FONT *buttonFont;
extern ALLEGRO_FONT *defaultFont;
extern ALLEGRO_FONT *titleFont;
//Allegro Variables -- END

//Funtion Prototypes -- START
bool addBulletToList(Bullet *newBullet);
bool addEnemyToList(Enemy *newEnemy);
bool addPickupToList(Pickup *newPickup);
bool isPassable(int y, int x, int width, int height);
bool checkCollision(int x, int y, int ex, int ey, int width, int height, int ewidth, int eheight);
//Funtion Prototypes -- END

//Pointer Arrays -- START
extern Enemy *enemyList[MAX_ENEMIES];
extern Bullet *bulletList[MAX_BULLETS];
//Pointer Arrays -- END

//Technical Variables -- START
extern bool done, retry, draw, initButtons, mouseLeftDown, resetShop, startMenu, shopMenu;
extern int screenState;
extern float mouseX, mouseY, distanceX, distanceY, radiansToMouse;
//Technical Varuabkes -- END

//Player Variables -- START
extern bool playerAutoShoot;
extern float playerX, playerY, playerCenterX, playerCenterY;
extern float playerMoveSpeed;
extern float playerShootingSpeed, playerShootingSpeedHelper;
extern int playerShots[4];
extern int playerHealth, playerMaxHealth, playerDamage;
extern int playerWidth, playerHeight, playerHitBoxWidth, playerHitBoxHeight, lastKeyPress;
extern int playerScore;
extern float playerDPS, fpsTimeOld, fpsTimeNew, fpsCounter;
//Player Variables -- END

//Enemy Variables -- START
extern float enemyFormationSpawn, enemyFormationSpawnHelper;
extern int enemyStatsBooster;
//Enemy Variables -- END

#endif // ENGINE_H
