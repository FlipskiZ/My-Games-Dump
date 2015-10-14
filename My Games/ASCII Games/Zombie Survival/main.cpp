#include "Engine.h"
#include "Actor.h"
#include "Bullet.h"
#include "Shop.h"
#include "SkillTree.h"
#include "SpecialBullet.h"
#include <cstdlib>
#include <ctime>
#include <curses.h>

#define MAX_ACTORS 500
#define MAX_BULLETS 500

struct TILE_TYPE
{
	char character;
	short colorCode;
	bool isPassable;
};

Actor * actorList[MAX_ACTORS];
Bullet * bulletList[MAX_BULLETS];
SpecialBullet * specialBulletList[MAX_BULLETS];

void drawMap();
void drawTile(int y, int x);
void drawStats();
bool addActorToList(Actor *newActor);
int testForActor(int y, int x);
bool addBulletToList(int direction);
bool addSpecialBulletToList(int direction);
void reloadGun();
void roundStart();
bool gameOver();

int cursorY;
int cursorX;

bool retry = true;
bool breakLoop = false;

int playerMovementSpeed;
int playerMovementSpeedHelper;

int playerHealth;
int playerMaxHealth;
int playerStamina;
int playerMaxStamina;
int playerStaminaRegen;
int playerStaminaRegenHelper;
int playerAmmo;
int playerBulletSpeed;
int playerDamage;
int playerBulletRange;
int playerFiringSpeed;
int playerFiringSpeedHelper;
int playerClip;
int playerClipLimit;
int playerHiddenClip;
int playerReloading;
int playerReloadingRate;
int playerReloadingHelper;
int playerReloadingModifier;
bool playerSpecialWeaponTypes[10];
char playerWeaponName[30];
int playerCurrentWeapon[2];
int playerBandages;
bool playerBandaging;
int playerBandagesHelper;
int playerMeleeDamage;
int playerMeleeDirecton;
int playerMeleeHelper;
int playerStaminaPills;

int playerMoney;
int killMoney;
int playerXP;
int playerXPNeeded;
int playerXPGet;
int playerSkillPoints;
int playerLevel;
bool lastTiersUnlocked;

int round;
int roundZombies;
float roundZombiesFloat;
int roundZombiesHelper;
int zombieSpawnRate;
int zombieSpawnRateHelper;
bool betweenRoundPause;
int betweenRoundPauseHelper;
int zombieForceSpawnHelper;

int playerTimeSurvived;
int playerZombiesKilled;
int playerBulletsFired;
int playerTotalMoneyEarned;
int playerMoves;
int playerTimesBandaged;
int playerTimesReloaded;
int playerAmountBought;
int playerAmountEnteredShop;
int playerAmountEnteredSkillTree;
int playerPeakMoneyOwned;
int playerTotalStaminaPillsUsed;
int playerAmountMelee;
int playerTotalAmountForce;
int playerTimesHit;
int playerTimesPaused;

int mapArray[mapHeight][mapWidth] = {
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2,2,2,2,2,2,2,1},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2,2,2,6,2,2,2,1},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2,2,1,4,1,2,2,1},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,5,1,1,1,1},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,2,2,2,2,2,2,2,1},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2,2,2,2,2,2,2,1},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,2,1,1,1,1},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,8,8,8,8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,8,8,8,8,8,8,8,8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,8,8,8,8,8,8,8,8,8,8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,8,8,8,8,8,8,8,8,8,8,8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,8,8,8,8,8,8,8,8,8,8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,8,8,8,8,8,8,8,8,8,8,8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,8,8,8,8,8,8,8,8,8,8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,8,8,8,8,8,8,8,8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,8,8,8,8,8,8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
};

TILE_TYPE tileIndex[] = {
	{'.', 1, true}, // (0) TILE_EMPTY
	{'#', 0, false}, // (1) TILE_WALL
	{'.', 0, true}, // (2) TILE_FLOOR
	{'|', 0, false}, // (3) TILE_WINDOW_VERTICAL
	{'-', 0, false}, // (4) TILE_WINDOW_HORIZONTAL
	{'$', 1, true}, // (5) TILE_SHOP
	{'@', 4, false}, // (6) TILE_DUMMY
	{'T', 1, false}, // (7) TILE_TREES
	{'~', 4, false}, // (8) TILE_WATER
};

int main()
{
    cursorY = 0;
    cursorX = 0;

    int deltaY = 0;
    int deltaX = 0;

    srand(time(0));

    initscr();

	raw();
	keypad(stdscr, TRUE);
	noecho();
	curs_set(0);
	start_color();

	resize_term(30, 80);

    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    init_pair(2, COLOR_YELLOW, COLOR_BLACK);
    init_pair(3, COLOR_RED, COLOR_BLACK);
    init_pair(4, COLOR_CYAN, COLOR_BLACK);
    init_pair(5, COLOR_WHITE, COLOR_RED);
    init_pair(6, COLOR_WHITE, COLOR_GREEN);
    init_pair(7, COLOR_WHITE, COLOR_YELLOW);
    init_pair(8, COLOR_RED, COLOR_RED);
    init_pair(9, COLOR_RED, COLOR_GREEN);
    init_pair(10, COLOR_RED, COLOR_YELLOW);
    init_pair(11, COLOR_WHITE, COLOR_BLACK);
    init_pair(12, COLOR_MAGENTA, COLOR_BLACK);

    while(retry){

        Shop shop;
        SkillTree skillTree;

        breakLoop = false;
        int timeStart = time(0);

        clear();

        cursorY = mapHeight/2;
        cursorX = mapWidth/2+5;

        playerMovementSpeed = 50;
        playerMovementSpeedHelper = 0;

        playerHealth = 100;
        playerMaxHealth = 100;
        playerStamina = 10;
        playerMaxStamina = 10;
        playerStaminaRegen = 250;
        playerStaminaRegenHelper = 0;
        playerAmmo = 100;
        playerBulletSpeed = 30;
        playerDamage = 1;
        playerBulletRange = 20;
        playerFiringSpeed = 500;
        playerFiringSpeedHelper = 0;
        playerClip = 10;
        playerClipLimit = 10;
        playerHiddenClip = 0;
        playerReloading = false;
        playerReloadingRate = 4000;
        playerReloadingHelper = 0;
        playerReloadingModifier = 0;
        playerSpecialWeaponTypes = {0,0,0,0,0,0,0,0,0,0};
        playerWeaponName = {'P','i','s','t','o','l'};
        playerCurrentWeapon = {0,0};
        playerBandages = 0;
        playerBandaging = false;
        playerBandagesHelper = 0;
        playerMeleeDamage = 0;
        playerMeleeDirecton = 0;
        playerMeleeHelper = 0;
        playerStaminaPills = 0;

        playerMoney = 0;
        killMoney = 1;
        playerXP = 0;
        playerXPNeeded = 10;
        playerXPGet = 1;
        playerSkillPoints = 0;
        playerLevel = 1;
        lastTiersUnlocked = false;

        round = 1;
        roundZombies = 5;
        roundZombiesFloat = 5;
        roundZombiesHelper = 0;
        zombieSpawnRate = 5000;
        zombieSpawnRateHelper = 0;
        betweenRoundPause = false;
        betweenRoundPauseHelper = 0;
        zombieForceSpawnHelper = 500;

        playerTimeSurvived = 0;
        playerZombiesKilled = 0;
        playerBulletsFired = 0;
        playerTotalMoneyEarned = 0;
        playerMoves = 0;
        playerTimesBandaged = 0;
        playerTimesReloaded = 0;
        playerAmountBought = 0;
        playerAmountEnteredShop = 0;
        playerAmountEnteredSkillTree = 0;
        playerPeakMoneyOwned = 0;
        playerTotalStaminaPillsUsed = 0;
        playerAmountMelee = 0;
        playerTotalAmountForce = 0;
        playerTimesHit = 0;
        playerTimesPaused = 0;

        timeout(0); //time before the getch passes with no input

        for(int i = 0; i < MAX_ACTORS; i++){
            actorList[i] = NULL;
        }

        for(int i = 0; i < MAX_BULLETS; i++){
            bulletList[i] = NULL;
        }

        for(int y = 0; y < mapHeight; y++){
            for(int x = 0; x < mapWidth; x++){
                if(mapArray[y][x] == 7){
                    mapArray[y][x] = 0;
                }
            }
        }

        int z = 25 + rand() % 125;

        for(int i = 0; i < z; i++){
            int y = rand() % mapHeight;
            int x = rand() % mapWidth;

            if(mapArray[y][x] == 0 && isPassable(y, x) && (cursorY != y || cursorX != x) && mapArray[y-1][x] != 2){
                mapArray[y][x] = 7;
            }
        }

        //put stuff before the game loop here

        while(!breakLoop){
            drawMap();
            drawStats();

            playerTimeSurvived = time(0) - timeStart;

            if(playerMoney > playerPeakMoneyOwned)
                playerPeakMoneyOwned = playerMoney;

            if(playerHealth <= 0){
                if(gameOver()){
                    breakLoop = true;
                    retry = true;
                }else{
                    breakLoop = true;
                    retry = false;
                }
            }

            if(playerStaminaRegenHelper >= playerStaminaRegen){
                if(playerStamina < playerMaxStamina){
                    playerStamina++;
                }
                playerStaminaRegenHelper = 0;
            }

            if(playerClip == 0 && !playerReloading && playerAmmo > 0){
                reloadGun();
            }

            if(playerXP >= playerXPNeeded){
                playerSkillPoints++;
                playerLevel++;
                playerXPNeeded *= 1.1;
                playerXP = 0;
            }

            for(int i = 0; i < MAX_ACTORS; i++){
                if(actorList[i] != NULL && actorList[i]->checkActive()){
                    actorList[i]->update();
                    actorList[i]->draw();
                }
            }

            for(int i = 0; i < MAX_BULLETS; i++){
                if(bulletList[i] != NULL && bulletList[i]->checkActive()){
                    bulletList[i]->update();
                    bulletList[i]->draw();
                }
            }

            for(int i = 0; i < MAX_BULLETS; i++){
                if(specialBulletList[i] != NULL && specialBulletList[i]->checkActive()){
                    specialBulletList[i]->update();
                    specialBulletList[i]->draw();
                }
            }

            if(roundZombiesHelper >= roundZombies){
                betweenRoundPause = true;
            }

            if(betweenRoundPause){
                betweenRoundPauseHelper++;
                if(betweenRoundPauseHelper >= 100000){
                    roundStart();
                }
            }

            if(zombieSpawnRateHelper >= zombieSpawnRate && !betweenRoundPause){
                int y, x;
                int side = rand() % 4;
                switch(side){
                    case 0:
                        y = 0;
                        x = rand() % mapWidth;
                        break;

                    case 1:
                        y = mapHeight - 1;
                        x = rand() % mapWidth;
                        break;

                    case 2:
                        y = rand() % mapHeight;
                        x = 0;
                        break;

                    case 3:
                        y = rand() % mapHeight;
                        x = mapWidth - 1;
                        break;

                }
                if(isPassable(y, x)){
                    Actor *newActor = new Actor();

                    newActor->setAppearance('@', 1);
                    newActor->setPos(y, x);
                    newActor->setSpeed(600-round*3);
                    newActor->setHealth(10);
                    newActor->setMaxRange(20);
                    addActorToList(newActor);
                    zombieSpawnRateHelper = 0;
                    roundZombiesHelper++;
                }
            }

            attron(COLOR_PAIR(0));
            move(cursorY, cursorX);
            addch('@');
            attroff(COLOR_PAIR(0));

            bool donePause = false;

            int input = getch();

            switch(input){
                case KEY_UP:
                    deltaY = -1;
                    playerMeleeDirecton = 0;
                    break;

                case KEY_DOWN:
                    deltaY = 1;
                    playerMeleeDirecton = 1;
                    break;

                case KEY_LEFT:
                    deltaX = -1;
                    playerMeleeDirecton = 2;
                    break;

                case KEY_RIGHT:
                    deltaX = 1;
                    playerMeleeDirecton = 3;
                    break;

                case 'w':
                    addBulletToList(0);
                    break;

                case 's':
                    addBulletToList(1);
                    break;

                case 'a':
                    addBulletToList(2);
                    break;

                case 'd':
                    addBulletToList(3);
                    break;

                case 'r':
                    reloadGun();
                    break;

                case 'n':
                    if(betweenRoundPause){
                        roundStart();
                        playerTotalAmountForce++;
                    }
                    break;

                case 'm':
                    if(!betweenRoundPause && zombieForceSpawnHelper >= 500){
                        int y, x;
                        int side = rand() % 4;
                        switch(side){
                            case 0:
                                y = 0;
                                x = rand() % mapWidth;
                                break;

                            case 1:
                                y = mapHeight - 1;
                                x = rand() % mapWidth;
                                break;

                            case 2:
                                y = rand() % mapHeight;
                                x = 0;
                                break;

                            case 3:
                                y = rand() % mapHeight;
                                x = mapWidth - 1;
                                break;

                        }
                        if(isPassable(y, x)){
                            Actor *newActor = new Actor();

                            newActor->setAppearance('@', 1);
                            newActor->setPos(y, x);
                            newActor->setSpeed(600-round*3);
                            newActor->setHealth(10);
                            newActor->setMaxRange(20);
                            addActorToList(newActor);
                            zombieSpawnRateHelper = 0;
                            roundZombiesHelper++;
                            zombieForceSpawnHelper = 0;
                        }
                        playerTotalAmountForce++;
                    }
                    break;

                case 't':
                    playerAmountEnteredSkillTree++;
                    skillTree.skillScreen();
                    break;

                case 'b':
                    if(playerHealth < playerMaxHealth && playerBandages > 0 && !playerReloading){
                        playerBandaging = true;
                    }
                    break;

                case 'v':
                    if(playerStaminaPills > 0 && playerStamina < playerMaxStamina){
                        if(playerStamina + playerStamina/2 <= playerMaxStamina){
                            playerStamina += playerStamina/2;
                        }else{
                            playerStamina = playerMaxStamina;
                        }
                        playerStaminaPills--;
                    }

                case 'e':
                    if(playerMeleeDamage > 0 && playerMeleeHelper >= 500){
                        for(int i = 0; i < MAX_BULLETS; i++){
                            if(bulletList[i] == NULL || !bulletList[i]->checkActive()){
                                int y, x;
                                Bullet *newBullet = new Bullet();
                                y = cursorY;
                                x = cursorX;
                                newBullet->setDirection(playerMeleeDirecton);
                                if(playerMeleeDamage < 4){
                                    if(playerMeleeDirecton == 0 || playerMeleeDirecton == 1){
                                        newBullet->setAppearance('|', 11);
                                    }else if(playerMeleeDirecton == 2 || playerMeleeDirecton == 3){
                                        newBullet->setAppearance('-', 11);
                                    }
                                }else if(playerMeleeDamage >= 4){
                                    if(playerMeleeDirecton == 0 || playerMeleeDirecton == 1){
                                        newBullet->setAppearance('|', 12);
                                    }else if(playerMeleeDirecton == 2 || playerMeleeDirecton == 3){
                                        newBullet->setAppearance('-', 12);
                                    }

                                }
                                newBullet->setPos(y, x);
                                newBullet->setSpeed(50);
                                if(playerMeleeDamage == 5){
                                    newBullet->setRange(3);
                                }else{
                                    newBullet->setRange(2);
                                }
                                newBullet->setDamage(playerMeleeDamage);
                                bulletList[i] = newBullet;
                                playerMeleeHelper = 0;
                                playerAmountMelee++;
                                break;
                            }
                        }
                    }
                    break;

                case 'p':
                    mvprintw(13, mapWidth+2, "GAME PAUSED - PRESS P TO UNPAUSE");
                    mvprintw(14, mapWidth+2, "PRESS R TO RESET THE GAME");
                    mvprintw(15, mapWidth+2, "WARNING: YOU WON'T SEE THE FINAL STATS");
                    playerTimesPaused++;
                    while(!donePause){
                        int input = getch();
                        if(input == 'p'){
                            donePause = true;
                        }else if(input == 'r'){
                            breakLoop = true;
                            donePause = true;
                        }
                    }
                    break;

                case ' ':
                    if(mapArray[cursorY][cursorX] == 5){
                        playerAmountEnteredShop++;
                        shop.shopScreen();
                    }
                    break;

                default:
                    break;

            }

            if(isPassable(cursorY + deltaY, cursorX + deltaX) && playerMovementSpeedHelper >= playerMovementSpeed && playerStamina > 0){
                cursorY += deltaY;
                cursorX += deltaX;
                if(input == KEY_UP || input == KEY_DOWN || input == KEY_LEFT || input == KEY_RIGHT){
                    playerMovementSpeedHelper = 0;
                    playerMoves++;
                    playerStamina--;
                    playerStaminaRegenHelper = 0;
                }
            }

            deltaX = 0;
            deltaY = 0;

            playerMovementSpeedHelper++;
            playerStaminaRegenHelper++;
            playerFiringSpeedHelper++;
            playerMeleeHelper++;
            zombieSpawnRateHelper++;
            zombieForceSpawnHelper++;

            if(playerReloading){
                playerReloadingHelper++;
                if(playerReloadingHelper >= playerReloadingRate){
                    playerReloading = false;
                    playerAmmo += playerHiddenClip;
                    if(playerAmmo - playerClipLimit > 0){
                        playerAmmo -= playerClipLimit;
                        playerClip = playerClipLimit;
                    }else{
                        playerAmmo -= playerClipLimit;
                        playerClip = playerClipLimit + playerAmmo;
                        playerAmmo = 0;
                    }
                    playerTimesReloaded++;
                    playerHiddenClip = 0;
                    playerReloadingHelper = 0;
                }
            }

            if(playerBandaging){
                playerBandagesHelper++;
                if(playerBandagesHelper >= 1500){
                    if(playerHealth + (playerMaxHealth/4) <= playerMaxHealth){
                        playerHealth += (playerMaxHealth/4);
                    }else{
                        playerHealth = playerMaxHealth;
                    }
                    playerBandaging = false;
                    playerBandages--;
                    playerTimesBandaged++;
                    playerBandagesHelper = 0;
                }
            }

            refresh();
        }
    }

	endwin();
}

bool addActorToList(Actor *newActor){
    for(int i = 0; i < MAX_ACTORS; i++){
        if(actorList[i] == NULL || !actorList[i]->checkActive()){
            actorList[i] = newActor;
            return true;
        }
    }
    return false;
}

int testForActor(int y, int x){
    for(int i = 0; i < MAX_ACTORS; i++){
        if(actorList[i] != NULL){
            if(actorList[i]->posY == y && actorList[i]->posX == x){
                return i;
            }
        }
    }
    return -1;
}

bool addBulletToList(int direction){
    if(playerFiringSpeedHelper >= playerFiringSpeed && playerClip > 0 && !playerBandaging && !playerSpecialWeaponTypes[0]){
        for(int i = 0; i < MAX_BULLETS; i++){
            if(bulletList[i] == NULL || !bulletList[i]->checkActive()){
                int y, x;
                Bullet *newBullet = new Bullet();
                y = cursorY;
                x = cursorX;
                newBullet->setDirection(direction);
                newBullet->setAppearance('*', 0);
                newBullet->setPos(y, x);
                newBullet->setSpeed(playerBulletSpeed);
                newBullet->setRange(playerBulletRange);
                newBullet->setDamage(playerDamage);
                bulletList[i] = newBullet;
                playerFiringSpeedHelper = 0;
                playerClip--;
                playerBulletsFired++;
                return true;
            }
        }
    }else if(playerSpecialWeaponTypes[0]){
        playerBulletsFired++;
        addSpecialBulletToList(direction);
    }
    return false;
}

bool addSpecialBulletToList(int direction){
    if(playerFiringSpeedHelper >= playerFiringSpeed && playerClip > 0 && !playerBandaging){
        for(int i = 0; i < MAX_BULLETS; i++){
            if(specialBulletList[i] == NULL || !specialBulletList[i]->checkActive()){
                int y, x;
                SpecialBullet *newSpecialBullet = new SpecialBullet();
                y = cursorY;
                x = cursorX;
                newSpecialBullet->setDirection(direction);
                newSpecialBullet->setAppearance('*', 0);
                newSpecialBullet->setPos(y, x);
                newSpecialBullet->setSpeed(playerBulletSpeed);
                newSpecialBullet->setRange(playerBulletRange);
                newSpecialBullet->setDamage(playerDamage);
                newSpecialBullet->setBulletType(playerSpecialWeaponTypes[1], playerSpecialWeaponTypes[2], false, playerSpecialWeaponTypes[3], false, playerSpecialWeaponTypes[4], playerSpecialWeaponTypes[5], playerSpecialWeaponTypes[6], playerSpecialWeaponTypes[7], playerSpecialWeaponTypes[8], playerSpecialWeaponTypes[9]);
                specialBulletList[i] = newSpecialBullet;
                playerFiringSpeedHelper = 0;
                playerClip--;
                return true;
            }
        }
    }
    return false;
}

bool addSpecialBulletToListSpecial(int direction, int y, int x, int range, int speed, int damage, bool rocket, bool rocketExplosion, bool granade, bool frag, bool random, bool mine, bool granadeGranade, bool granadeGranadeGranade, bool potato, bool piercing){
    for(int i = 0; i < MAX_BULLETS; i++){
        if(specialBulletList[i] == NULL || !specialBulletList[i]->checkActive()){
            if(isPassableBullet(y, x)){
                SpecialBullet *newSpecialBullet = new SpecialBullet();
                newSpecialBullet->setDirection(direction);
                newSpecialBullet->setAppearance('*', 0);
                newSpecialBullet->setPos(y, x);
                newSpecialBullet->setSpeed(speed);
                newSpecialBullet->setRange(range);
                newSpecialBullet->setDamage(damage);
                newSpecialBullet->setBulletType(false, rocket, rocketExplosion, granade, frag, random, mine, granadeGranade, granadeGranadeGranade, potato, piercing);
                specialBulletList[i] = newSpecialBullet;
                return true;
            }
        }
    }
    return false;
}

void drawMap(){
    clear();
	for(int y = 0; y < mapHeight; y++){
		for(int x = 0; x < mapWidth; x++){
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

void drawStats(){
    mvprintw(0, mapWidth+2, "Round: %d", round);
    mvprintw(1, mapWidth+2, "Health: %d/%d", playerHealth, playerMaxHealth);
    mvprintw(2, mapWidth+2, "Stamina: %d/%d", playerStamina, playerMaxStamina);
    mvprintw(3, mapWidth+2, "Money: %d$", playerMoney);
    mvprintw(4, mapWidth+2, "XP: %d/%d", playerXP, playerXPNeeded);
    mvprintw(5, mapWidth+2, "Level: %d", playerLevel);
    if(playerSkillPoints >= 5){
        attron(COLOR_PAIR(6));
        mvprintw(6, mapWidth+2, "Skill Points: %d", playerSkillPoints);
        attroff(COLOR_PAIR(6));
    }else if(playerSkillPoints > 0){
        attron(COLOR_PAIR(1));
        mvprintw(6, mapWidth+2, "Skill Points: %d", playerSkillPoints);
        attroff(COLOR_PAIR(1));
    }else{
        mvprintw(6, mapWidth+2, "Skill Points: %d", playerSkillPoints);
    }
    mvprintw(7, mapWidth+2, "Stamina Pills: %d", playerStaminaPills);
    if(playerBandages == 0){
        attron(COLOR_PAIR(5));
        mvprintw(8, mapWidth+2, "Bandages: %d", playerBandages);
        attroff(COLOR_PAIR(5));
    }else if(playerBandages <= 2){
        attron(COLOR_PAIR(7));
        mvprintw(8, mapWidth+2, "Bandages: %d", playerBandages);
        attroff(COLOR_PAIR(7));
    }else{
        mvprintw(8, mapWidth+2, "Bandages: %d", playerBandages);
    }
    if(playerBandaging){
        mvprintw(9, mapWidth+2, "Bandaging %d/1500", playerBandagesHelper);
    }
    if(playerAmmo == 0){
        attron(COLOR_PAIR(5));
        mvprintw(10, mapWidth+2, "Ammo: %d", playerAmmo);
        attroff(COLOR_PAIR(5));
    }else if(playerAmmo < playerClipLimit*2){
        attron(COLOR_PAIR(7));
        mvprintw(10, mapWidth+2, "Ammo: %d", playerAmmo);
        attroff(COLOR_PAIR(7));
    }else{
        mvprintw(10, mapWidth+2, "Ammo: %d", playerAmmo);
    }
    mvprintw(11, mapWidth+2, "%s %d/%d", playerWeaponName, playerClip, playerClipLimit);
    if(playerSpecialWeaponTypes[8] == true && playerSpecialWeaponTypes[8] != 0){
        attron(COLOR_PAIR(3));
        mvprintw(11, mapWidth+2, "%s %d/%d", playerWeaponName, playerClip, playerClipLimit);
        attroff(COLOR_PAIR(3));
    }
    if(playerReloading){
        mvprintw(12, mapWidth+2, "Reloading %d/%d", playerReloadingHelper, playerReloadingRate);
    }else if(playerFiringSpeedHelper < playerFiringSpeed){
        mvprintw(12, mapWidth+2, "Time Until You Can Fire Again %d", playerFiringSpeed-playerFiringSpeedHelper);
    }

    if(betweenRoundPause){
        mvprintw(16, mapWidth+2, "Round Done");
        mvprintw(17, mapWidth+2, "Time Left To Next Round: %d", 100000-betweenRoundPauseHelper);
        mvprintw(18, mapWidth+2, "Press N To Start The Next Round");

        float temp = roundZombiesFloat;
        roundZombiesFloat *= 1.1;
        roundZombies = roundZombiesFloat;
        roundZombiesFloat = temp;

        mvprintw(19, mapWidth+2, "Zombies Next Round: %d", roundZombies);
    }else{
        mvprintw(16, mapWidth+2, "Zombies Left To Spawn: %d", roundZombies - roundZombiesHelper);
        mvprintw(17, mapWidth+2, "Time Left For Zombie To Spawn: %d", zombieSpawnRate - zombieSpawnRateHelper);
        mvprintw(18, mapWidth+2, "Press M To Force Spawn A Zombie");
        if(zombieForceSpawnHelper < 500){
            mvprintw(19, mapWidth+2, "Time Until You Can Force Spawn %d", 500-zombieForceSpawnHelper);
        }
    }
    mvprintw(21, mapWidth+2, "Time Survived: %d", playerTimeSurvived);
    mvprintw(22, mapWidth+2, "Zombies Killed: %d", playerZombiesKilled);
    mvprintw(23, mapWidth+2, "Bullets Fired: %d", playerBulletsFired);
    mvprintw(24, mapWidth+2, "Total Money Earned: %d", playerTotalMoneyEarned);
}

bool isPassable(int y, int x){
    if(y >= 0 && y < mapHeight && x >= 0 && x < mapWidth && tileIndex[mapArray[y][x]].isPassable && testForActor(y, x) == -1){
        return true;
    }
    return false;
}

bool isPassableBullet(int y, int x){
    if(y >= 0 && y < mapHeight && x >= 0 && x < mapWidth && (tileIndex[mapArray[y][x]].isPassable || mapArray[y][x] == 3 || mapArray[y][x] == 4 || mapArray[y][x] == 8 || (mapArray[y][x] == 7 && rand() % 2 == 0))){
        return true;
    }
    return false;
}

void bulletHit(int actorId, int damage){
    actorList[actorId]->setHealth(actorList[actorId]->checkHealth() - damage);
}

void reloadGun(){
    if(playerClip < playerClipLimit && playerAmmo > 0 && !playerBandaging){
        playerReloading = true;
        playerHiddenClip = playerClip;
        playerClip = 0;
    }
}

void roundStart(){
    betweenRoundPause = false;
    betweenRoundPauseHelper = 0;
    roundZombiesFloat *= 1.2;
    roundZombies = roundZombiesFloat;
    roundZombiesHelper = 0;
    zombieSpawnRate /= 1.1;
    zombieSpawnRateHelper = 0;
    round++;
}

bool gameOver(){
    clear();

    int counter = 0;
    bool counterFreeze = false;

    mvprintw(0,0,"GAME OVER");

    mvprintw(2,0, "Time Survived: %d", playerTimeSurvived);
    mvprintw(3,0, "Zombies Killed: %d", playerZombiesKilled);
    mvprintw(4,0, "Bullets Fired: %d", playerBulletsFired);
    mvprintw(5,0, "Total Money Earned: %d", playerTotalMoneyEarned);
    mvprintw(6,0, "Times Moved: %d", playerMoves);
    mvprintw(7,0, "Times Bandaged: %d", playerTimesBandaged);
    mvprintw(8,0, "Times Reloaded: %d", playerTimesReloaded);
    mvprintw(9,0, "Total Items Bought From Shop: %d", playerAmountBought);
    mvprintw(10,0, "Times Went Into Shop: %d", playerAmountEnteredShop);
    mvprintw(11,0, "Times Went Into Skilltree: %d", playerAmountEnteredSkillTree);
    mvprintw(12,0, "Peak Money Owned: %d", playerPeakMoneyOwned);
    mvprintw(13,0, "Total Stamina Pills Consumed: %d", playerTotalStaminaPillsUsed);
    mvprintw(14,0, "Times Used Melee: %d", playerAmountMelee);
    mvprintw(15,0, "Times Force Spawned: %d", playerTotalAmountForce);
    mvprintw(16,0, "Times Hit: %d", playerTimesHit);
    mvprintw(17,0, "Times Paused: %d", playerTimesPaused);

    while(true){
        if(counter >= 100000 || counterFreeze){
            mvprintw(20,0,"If you want to play again press y, and if not, press n");
            counterFreeze = true;
        }
        int input = getch();

        if((input == 'y' || input == 'Y') && counterFreeze){
            return true;
        }else if((input == 'n' || input == 'N') && counterFreeze){
            return false;
        }
        if(!counterFreeze){
            counter++;
        }
    }
}
