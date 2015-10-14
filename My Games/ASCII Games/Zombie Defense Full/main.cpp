#include <cstdlib>
#include <ctime>
#include <curses.h>

#define MAX_MAP_HEIGHT 25
#define MAX_MAP_WIDTH 25

#define MAX_ACTORS 5000
#define MAX_BULLETS 10000

struct TILE_TYPE
{
	char character;
	short colorCode;
	bool isPassable;
};

class Actor{
    public:
        Actor(void);
        void setAppearance(char diplayChar, short displayColor);
        void setPos(int y, int x);
        void setSpeed(int speed);
        void setDirection(int direction);
        void setActive(bool active);
        bool checkActive();
        void setHealth(int health);
        void draw();
        void update();

        int posY;
        int posX;
        int health;

    protected:
        char displayChar;
        short colorCode;
        int movementSpeed;
        int movementSpeedHelper;
        int direction;
        bool active;
        int attackBaseHelper;

    private:

};

class Bullet{
    public:
        Bullet(void);
        void setAppearance(char diplayChar, short displayColor);
        void setPos(int y, int x);
        void setSpeed(int speed);
        void setDirection(int direction);
        void setActive(bool active);
        bool checkActive();
        void draw();
        void update();

        int posY;
        int posX;

    protected:
        char displayChar;
        short colorCode;
        int movementSpeed;
        int movementSpeedHelper;
        int direction;
        bool active;

    private:

};

Actor * actorList[MAX_ACTORS];
Bullet * bulletList[MAX_BULLETS];

bool addActorToList(Actor *newActor);
int testForActor(int y, int x);
bool addBulletToList(int direction);
int testForBullet(int y, int x);
void drawMap();
void drawTile(int y, int x);
void drawStats();
bool isPassable(int y, int x, int deltaY, int deltaX);
void reloadGun();
void shop();
void gameLost();

bool retry = true;
bool breakLoop = false;

int height = 25;
int width = 25;

int cursorY;
int cursorX;

int playerMovementSpeed;
int playerMovementSpeedHelper;

int playerFiringSpeed;
int playerFiringSpeedHelper;

int enemySpawn;
int enemySpawnHelper;
int enemySpawnFaster;
int enemySpawnFasterHelper;
int enemySpawnFasterSlower;
int enemySpawnFasterSlowerHelper;
int enemySpawnFasterJump;

int playerHealth;
int playerAmmo;
int playerClipLimit;
int playerClip;
int playerHiddenClip;
int playerReloadingRate;
int playerReloadingHelper;
bool playerReloading;
char playerWeaponName[30];

int playerScore;
int playerTotalEarnedMoney;
int healthTotalPaid;
int moreKillMoneyTotalPaid;
int ammoTotalPaid;
int weaponTotalPaid;

int warningTimer;
bool emptyClipWarning;
int emptyClipWarningTimer;
bool noAmmoWarning;
int noAmmoWarningTimer;
bool ammoBlink;
int ammoBlinkTimer;
bool littleMoneyWarning;
int littleMoneyWarningTimer;

int weaponTier;
int playerMoney;
int killMoney;
float healthCost;
int moreKillMoneyCost;
float ammoCost;
int shopWeapons1[5][5] = {
    {1000,5,3500,10,0},
    {800,6,3500,25,1},
    {600,8,2500,45,2},
    {500,15,2750,50,3},
    {300,13,2000,70,4},
};

int shopWeapons2[5][5] = {
    {1050,8,4250,15,5},
    {300,3,2500,30,6},
    {500,15,3500,40,7},
    {400,20,3000,60,8},
    {200,16,3000,80,9},
};

int shopWeapons3[5][5] = {
    {500,6,5000,25,10},
    {500,18,6000,40,11},
    {300,25,4500,60,12},
    {200,30,4000,70,13},
    {150,35,3000,99,14},
};

char shopWeaponNames[15][30] = {
    {'9', 'm', 'm'}, //weapon1
    {'G', 'u', 'n', 'i', '-', '4', '2'},
    {'Q', 'u', 'i', 'r', 'e', 's'},
    {'P', 'e', 'n', 's'},
    {'N', 'u', 's', 't', 'u', 'g', '-', '2', '0', '0', '0'}, //weapon1
    {'C', 'o', 'l', 't'}, //weapon2
    {'P', 'i', 'n', 'n', 'n'},
    {'T', 'e', 'r', 'r', 'e', 's', 'i', 'l', '-', '5'},
    {'M', 'o', 'n', 'n','e', 'y', '-', '5', '8'},
    {'V', 'a', 'p', 'o', 'r', '-', '9'}, //weapon2
    {'R', 'e', 'v', 'o', 'l', 'v', 'e', 'r'}, //weapon3
    {'P', 'i', 's', 'k', 'l', 'e'},
    {'A', 'g', 'u', 'n', 'K', '-', '4', '7'},
    {'M', 'e', 'r', 's', 't'},
    {'B', 'a', 'n', 'g', 'y', '-', '1'}, //weapon3
};

int mapArray[MAX_MAP_HEIGHT][MAX_MAP_WIDTH] = {
    {2,2,2,2,2,2,2,2,2,2,0,0,0,0,0,2,2,2,2,2,2,2,2,2,2},
    {2,2,2,2,2,2,2,2,2,2,0,0,0,0,0,2,2,2,2,2,2,2,2,2,2},
    {2,2,2,2,2,2,2,2,2,2,0,0,0,0,0,2,2,2,2,2,2,2,2,2,2},
    {2,2,2,2,2,2,2,2,2,2,0,0,0,0,0,2,2,2,2,2,2,2,2,2,2},
    {2,2,2,2,2,2,2,2,2,2,0,0,0,0,0,2,2,2,2,2,2,2,2,2,2},
    {2,2,2,2,2,2,2,2,2,2,0,0,0,0,0,2,2,2,2,2,2,2,2,2,2},
    {2,2,2,2,2,2,2,2,2,2,0,0,0,0,0,2,2,2,2,2,2,2,2,2,2},
    {2,2,2,2,2,2,2,2,2,2,0,0,0,0,0,2,2,2,2,2,2,2,2,2,2},
    {2,2,2,2,2,2,2,2,2,2,0,0,0,0,0,2,2,2,2,2,2,2,2,2,2},
    {2,2,2,2,2,2,2,2,2,2,1,1,1,1,1,2,2,2,2,2,2,2,2,2,2},
    {0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0},
    {2,2,2,2,2,2,2,2,2,2,1,1,1,1,1,2,2,2,2,2,2,2,2,2,2},
    {2,2,2,2,2,2,2,2,2,2,0,0,0,0,0,2,2,2,2,2,2,2,2,2,2},
    {2,2,2,2,2,2,2,2,2,2,0,0,0,0,0,2,2,2,2,2,2,2,2,2,2},
    {2,2,2,2,2,2,2,2,2,2,0,0,0,0,0,2,2,2,2,2,2,2,2,2,2},
    {2,2,2,2,2,2,2,2,2,2,0,0,0,0,0,2,2,2,2,2,2,2,2,2,2},
    {2,2,2,2,2,2,2,2,2,2,0,0,0,0,0,2,2,2,2,2,2,2,2,2,2},
    {2,2,2,2,2,2,2,2,2,2,0,0,0,0,0,2,2,2,2,2,2,2,2,2,2},
    {2,2,2,2,2,2,2,2,2,2,0,0,0,0,0,2,2,2,2,2,2,2,2,2,2},
    {2,2,2,2,2,2,2,2,2,2,0,0,0,0,0,2,2,2,2,2,2,2,2,2,2},
    {2,2,2,2,2,2,2,2,2,2,0,0,0,0,0,2,2,2,2,2,2,2,2,2,2},
};

TILE_TYPE tileIndex[] = {
	{'.', 0, true}, // (0) TILE_FLOOR
	{'#', 0, false}, // (1) TILE_WALL
	{' ', 0, false}, // (2) TILE_EMPTY
};


Actor::Actor(){
    this->health = 0;
	this->displayChar = 0;
	this->colorCode = 0;
	this->posY = 0;
	this->posX = 0;
	this->movementSpeed = 0;
	this->movementSpeedHelper = 0;
	this->direction = 0;
	this->active = true;
	this->attackBaseHelper = 0;
}

void Actor::setAppearance(char displayChar, short displayColor){
    this->displayChar = displayChar;
    this->colorCode = displayColor;
}

void Actor::setPos(int y, int x){
     if((y < 0) || (y >= height || x < 0) || (x >= width)){
        return;
     }
    this->posY = y;
    this->posX = x;
}

void Actor::setSpeed(int speed){
    this->movementSpeed = speed;
}

void Actor::setDirection(int direction){
    this->direction = direction;
}

void Actor::setActive(bool active){
    this->active = active;
}

bool Actor::checkActive(){
    return this->active;
}

void Actor::setHealth(int health){
    this->health = health;
}

void Actor::draw(){
    attron(COLOR_PAIR(this->colorCode));
    move(this->posY, this->posX);
    addch(this->displayChar);
    attroff(COLOR_PAIR(this->colorCode));
}

void Actor::update(){
    if(this->health == 0){
        this->active = false;
        this->posY = 0;
        this->posX = 0;
        playerMoney += killMoney;
        playerScore++;
        playerTotalEarnedMoney += killMoney;
    }else if(this->health == 1){
        this->colorCode = 3;
    }else if(this->health == 2){
        this->colorCode = 2;
    }

    if(this->movementSpeedHelper >= movementSpeed){
        int deltaY = 0;
        int deltaX = 0;
        if(this->direction == 0){
            deltaY = -1;
        }else if(this->direction == 1){
            deltaY = 1;
        }else if(this->direction == 2){
            deltaX = -1;
        }else if(this->direction == 3){
            deltaX = 1;
        }

        if(isPassable(this->posY, this->posX, deltaY, deltaX) && testForBullet(this->posY+deltaY, this->posX+deltaX)){
            this->posY += deltaY;
            this->posX += deltaX;
            movementSpeedHelper = 0;
        }
        if(mapArray[this->posY+deltaY][this->posX+deltaX] == 1){
            attackBaseHelper++;
            movementSpeedHelper = 0;
            if(attackBaseHelper >= 5){
                playerHealth--;
                this->active = false;
                this->posY = 0;
                this->posX = 0;
                if(playerHealth <= 0)
                    gameLost();
            }
        }
    }else{
        this->movementSpeedHelper++;
    }
}

Bullet::Bullet(){
	this->displayChar = 0;
	this->colorCode = 0;
	this->posY = 0;
	this->posX = 0;
	this->movementSpeed = 0;
	this->movementSpeedHelper = 0;
	this->direction = 0;
	this->active = true;
}

void Bullet::setAppearance(char displayChar, short displayColor){
    this->displayChar = displayChar;
    this->colorCode = displayColor;
}

void Bullet::setPos(int y, int x){
     if((y < 0) || (y >= height || x < 0) || (x >= width)){
        return;
     }
    this->posY = y;
    this->posX = x;
}

void Bullet::setSpeed(int speed){
    this->movementSpeed = speed;
}

void Bullet::setDirection(int direction){
    this->direction = direction;
}

void Bullet::setActive(bool active){
    this->active = active;
}

bool Bullet::checkActive(){
    return this->active;
}

void Bullet::draw(){
    if(mapArray[this->posY][this->posX] != 1){
        attron(COLOR_PAIR(this->colorCode));
        move(this->posY, this->posX);
        addch(this->displayChar);
        attroff(COLOR_PAIR(this->colorCode));
    }
}

void Bullet::update(){
    if(this->movementSpeedHelper >= movementSpeed){
        int deltaY = 0;
        int deltaX = 0;
        if(this->direction == 0){
            deltaY = 1;
        }else if(this->direction == 1){
            deltaY = -1;
        }else if(this->direction == 2){
            deltaX = 1;
        }else if(this->direction == 3){
            deltaX = -1;
        }

        int actorId = testForActor(this->posY + deltaY, this->posX + deltaX);
        int actorId2 = testForActor(this->posY, this->posX);

        if(this->posY+deltaY >= 0 && this->posY+deltaY < height && this->posX+deltaX >= 0 && this->posX+deltaX < width && actorId == -1 && actorId2 == -1){
            this->posY += deltaY;
            this->posX += deltaX;
            movementSpeedHelper = 0;
        }else if(actorId == -1 && actorId2 == -1){
            this->active = false;
            this->posY = 0;
            this->posX = 0;
        }else if(actorId != -1 || actorId2 != -1){
            this->active = false;
            this->posY = 0;
            this->posX = 0;
            if(actorId2 != -1){
                actorList[actorId2]->health--;
            }else if(actorId != -1){
                actorList[actorId]->health--;
            }
        }

    }else{
        this->movementSpeedHelper++;
    }
}

int main()
{
    int deltaY = 0;
    int deltaX = 0;

    srand(time(0));

    initscr();

	raw();
	keypad(stdscr, TRUE);
	noecho();
	curs_set(0);
	start_color();

    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    init_pair(2, COLOR_YELLOW, COLOR_BLACK);
    init_pair(3, COLOR_RED, COLOR_BLACK);
    init_pair(4, COLOR_CYAN, COLOR_BLACK);
    init_pair(5, COLOR_WHITE, COLOR_RED);
    init_pair(6, COLOR_WHITE, 8);
    init_pair(7, COLOR_WHITE, COLOR_YELLOW);

    playerWeaponName = {'P', 'i', 's', 't', 'o', 'l'};

    while(retry){

        breakLoop = false;

        cursorY = 12;
        cursorX = 12;

        playerMovementSpeed = 50;
        playerMovementSpeedHelper = 0;

        playerFiringSpeed = 850;
        playerFiringSpeedHelper = 0;

        enemySpawn = 10000;
        enemySpawnHelper = 0;
        enemySpawnFaster = 10;
        enemySpawnFasterHelper = 0;
        enemySpawnFasterSlower = 2400;
        enemySpawnFasterSlowerHelper = 0;
        enemySpawnFasterJump = 0;

        playerHealth = 3;
        playerAmmo = 50;
        playerClipLimit = 3;
        playerClip = 3;
        playerHiddenClip = 0;
        playerReloadingRate = 4000;
        playerReloadingHelper = 0;
        playerReloading = false;

        playerScore = 0;
        playerTotalEarnedMoney = 0;
        healthTotalPaid = 0;
        moreKillMoneyTotalPaid = 0;
        ammoTotalPaid = 0;
        weaponTotalPaid = 0;

        warningTimer = 400;
        emptyClipWarningTimer = 0;
        noAmmoWarning = false;
        noAmmoWarningTimer = 0;
        ammoBlink = false;
        ammoBlinkTimer = 0;
        littleMoneyWarning = false;
        littleMoneyWarningTimer = 0;

        weaponTier = 0;
        playerMoney = 0;
        killMoney = 1;
        healthCost = 5;
        moreKillMoneyCost = 5;
        ammoCost = 1;

        timeout(0); //time before the getch passes with no input

        for(int i = 0; i < MAX_ACTORS; i++){
            actorList[i] = NULL;
        }

        for(int i = 0; i < MAX_BULLETS; i++){
            bulletList[i] = NULL;
        }

        //put stuff before the game loop here

        while(!breakLoop){
            drawMap();
            drawStats();

            if(emptyClipWarning){
                emptyClipWarningTimer++;
                if(emptyClipWarningTimer >= warningTimer){
                    emptyClipWarning = false;
                }
            }else{
                emptyClipWarningTimer = 0;
            }

            if(noAmmoWarning){
                noAmmoWarningTimer++;
                if(noAmmoWarningTimer >= warningTimer){
                    noAmmoWarning = false;
                }
            }else{
                noAmmoWarningTimer = 0;
            }

            if(ammoBlink){
                ammoBlinkTimer++;
                if(playerAmmo >= playerClipLimit){
                    ammoBlink = false;
                }
                if(ammoBlinkTimer >= 1800){
                    noAmmoWarning = true;
                    ammoBlinkTimer = 0;
                }
            }else{
                ammoBlinkTimer = 0;
            }

            if(playerClip == 0 && !playerReloading && playerAmmo > 0){
                if(playerAmmo < playerClipLimit*2)
                    noAmmoWarning = true;
                reloadGun();
                ammoBlink = false;
            }else if(playerAmmo < playerClipLimit){
                ammoBlink = true;
            }

            if(enemySpawnFasterSlowerHelper >= enemySpawnFasterSlower && enemySpawnFaster < 1000){
                enemySpawnFaster++;
                enemySpawnFasterSlowerHelper = 0;
            }

            if(enemySpawnFasterHelper >= enemySpawnFaster && enemySpawn > 50){
                enemySpawn--;
                enemySpawnFasterHelper = 0;
            }
            if(enemySpawnFasterJump >= 185000){
                enemySpawn -= (enemySpawn/5);
                enemySpawnFasterJump = 0;
            }

            if(enemySpawnHelper >= enemySpawn){
                int y = rand() % 5;
                int x = rand() % 5;
                int side = rand() % 4;
                Actor *newActor = new Actor();
                newActor->setAppearance('@', 1);
                switch(side){
                    case 0:
                        newActor->setPos(0, 10+x);
                        newActor->setDirection(1);
                        break;

                    case 1:
                        newActor->setPos(24, 10+x);
                        newActor->setDirection(0);
                        break;

                    case 2:
                        newActor->setPos(10+y, 0);
                        newActor->setDirection(3);
                        break;

                    case 3:
                        newActor->setPos(10+y, 24);
                        newActor->setDirection(2);
                        break;

                    default:
                        break;
                }
                newActor->setSpeed(1750);
                newActor->setHealth(3);
                addActorToList(newActor);
                enemySpawnHelper = 0;
            }

            for(int i = 0; i < MAX_BULLETS; i++){
                if(bulletList[i] != NULL && bulletList[i]->checkActive()){
                    bulletList[i]->update();
                    bulletList[i]->draw();
                }
            }

            for(int i = 0; i < MAX_ACTORS; i++){
                if(actorList[i] != NULL && actorList[i]->checkActive()){
                    actorList[i]->update();
                    actorList[i]->draw();
                }
            }

            attron(COLOR_PAIR(0));
            move(cursorY, cursorX);
            addch('@');
            attroff(COLOR_PAIR(0));

                int input = getch();

                switch(input){
                    case KEY_UP:
                        if(playerMovementSpeedHelper >= playerMovementSpeed){
                            deltaY = -1;
                        }
                        break;

                    case KEY_DOWN:
                        if(playerMovementSpeedHelper >= playerMovementSpeed){
                            deltaY = 1;
                        }
                        break;

                    case KEY_LEFT:
                        if(playerMovementSpeedHelper >= playerMovementSpeed){
                            deltaX = -1;
                        }
                        break;

                    case KEY_RIGHT:
                        if(playerMovementSpeedHelper >= playerMovementSpeed){
                            deltaX = 1;
                        }
                        break;

                    case 'w':
                        if(playerFiringSpeedHelper >= playerFiringSpeed && !playerReloading && playerClip > 0){
                            addBulletToList(0);
                            playerClip--;
                            playerFiringSpeedHelper = 0;
                        }else if(playerClip <= 0){
                            emptyClipWarning = true;
                        }
                        break;

                    case 's':
                        if(playerFiringSpeedHelper >= playerFiringSpeed && !playerReloading && playerClip > 0){
                            addBulletToList(1);
                            playerClip--;
                            playerFiringSpeedHelper = 0;
                        }else if(playerClip <= 0){
                            emptyClipWarning = true;
                        }
                        break;

                    case 'a':
                        if(playerFiringSpeedHelper >= playerFiringSpeed && !playerReloading && playerClip > 0){
                            addBulletToList(2);
                            playerClip--;
                            playerFiringSpeedHelper = 0;
                        }else if(playerClip <= 0){
                            emptyClipWarning = true;
                        }
                        break;

                    case 'd':
                        if(playerFiringSpeedHelper >= playerFiringSpeed && !playerReloading && playerClip > 0){
                            addBulletToList(3);
                            playerClip--;
                            playerFiringSpeedHelper = 0;
                        }else if(playerClip <= 0){
                            emptyClipWarning = true;
                        }
                        break;

                    case 'r':
                        if(playerClip < playerClipLimit && !playerReloading && playerAmmo > 0){
                            if(playerAmmo < playerClipLimit*2)
                                noAmmoWarning = true;
                            reloadGun();
                        }else if(playerAmmo <= 0){
                            noAmmoWarning = true;
                        }
                        break;

                    case ' ':
                        shop();
                        break;

                    default:
                        break;

                }

                if(isPassable(cursorY, cursorX, deltaY, deltaX)){
                    cursorY += deltaY;
                    cursorX += deltaX;
                }

                deltaX = 0;
                deltaY = 0;

                if(input != -1 && (input == KEY_UP || input == KEY_DOWN || input == KEY_LEFT || input == KEY_RIGHT) && playerMovementSpeedHelper >= playerMovementSpeed)
                    playerMovementSpeedHelper = 0;

            playerMovementSpeedHelper++;
            playerFiringSpeedHelper++;
            enemySpawnHelper++;
            enemySpawnFasterHelper++;
            enemySpawnFasterSlowerHelper++;
            enemySpawnFasterJump++;

            if(playerReloading && playerReloadingHelper < playerReloadingRate){
                playerReloadingHelper++;
            }else if(playerReloadingHelper >= playerReloadingRate){
                playerReloading = false;
                if(playerAmmo - playerClipLimit < 0){
                    playerAmmo -= playerClipLimit;
                    playerClip = playerClipLimit + playerAmmo;
                }else{
                    playerAmmo -= playerClipLimit;
                    playerClip = playerClipLimit;
                }
                if(playerAmmo < 0)
                    playerAmmo = 0;
                playerAmmo += playerHiddenClip;
                playerHiddenClip = 0;
                playerReloadingHelper = 0;
            }

            refresh();
        }
    }

	endwin();
}

bool addActorToList(Actor *newActor){
    for(int i = 0; i < MAX_ACTORS; i++){
        if(actorList[i] == NULL ){
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
    for(int i = 0; i < MAX_BULLETS; i++){
        if(bulletList[i] == NULL ){
            int y, x;
            Bullet *newBullet = new Bullet();
            y = cursorY;
            x = cursorX;
            switch(direction){
                case 0:
                    newBullet->setDirection(1);
                    break;

                case 1:
                    newBullet->setDirection(0);
                    break;

                case 2:
                    newBullet->setDirection(3);
                    break;

                case 3:
                    newBullet->setDirection(2);
                    break;

                default:
                    break;
            }
            newBullet->setAppearance('*', 1);
            newBullet->setPos(y, x);
            newBullet->setSpeed(40);
            bulletList[i] = newBullet;
            return true;
        }
    }
    return false;
}

int testForBullet(int y, int x){
    for(int i = 0; i < MAX_ACTORS; i++){
        if(bulletList[i] != NULL){
            if(bulletList[i]->posY == y && bulletList[i]->posX == x){
                return i;
            }
        }
    }
    return -1;
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

void drawStats(){

    mvprintw(3, width+2, "Zombies Killed: %d", playerScore);

    mvprintw(6, width+2, "Health: %d", playerHealth);

    if(noAmmoWarning){
        attron(COLOR_PAIR(7));
        mvprintw(7, width+2, "Ammo: %d", playerAmmo);
        attroff(COLOR_PAIR(7));

    }else{
        mvprintw(7, width+2, "Ammo: %d", playerAmmo);
    }

    if(emptyClipWarning){
        attron(COLOR_PAIR(5));
        mvprintw(9, width+2, "%s %d/%d", playerWeaponName, playerClip, playerClipLimit);
        attroff(COLOR_PAIR(5));

    }else{
        mvprintw(9, width+2, "%s %d/%d", playerWeaponName, playerClip, playerClipLimit);
    }

    if(playerReloading){
        mvprintw(10, width+2, "Reloading... %d/%d", playerReloadingHelper, playerReloadingRate);
    }
    mvprintw(12, width+2, "%d$", playerMoney);
}


bool isPassable(int y, int x, int deltaY, int deltaX){
    if(y + deltaY >= 0 && y + deltaY < height && x + deltaX >= 0 && x + deltaX < width && tileIndex[mapArray[y+deltaY][x+deltaX]].isPassable && testForActor(y+deltaY, x+deltaX) == -1){
        return true;
    }
    return false;
}

void reloadGun(){
    if(playerClip > 0){
        playerHiddenClip = playerClip;
        playerClip = 0;
    }
    playerReloading = true;
}

void shop(){
    bool done = false;
    bool boughtWeapon = false;

    int y = 0;

    while(!done){
        clear();

        if(littleMoneyWarning){
            littleMoneyWarningTimer++;
            if(littleMoneyWarningTimer >= warningTimer){
                littleMoneyWarning = false;
            }
        }else{
            littleMoneyWarningTimer = 0;
        }
        if(littleMoneyWarning){
            attron(COLOR_PAIR(5));
            mvprintw(7, 1, "%d$", playerMoney);
            attroff(COLOR_PAIR(5));
        }else{
            mvprintw(7, 1, "%d$", playerMoney);
        }
        int healthCostInt = healthCost;
        int ammoCostInt = ammoCost;
        mvprintw(9, 1, "Back");
        mvprintw(10, 1, "+1 Health - %d$ - Current: %d", healthCostInt, playerHealth);
        mvprintw(11, 1, "+1 Money Per Kill - %d$ - Current: %d", moreKillMoneyCost, killMoney);
        mvprintw(12, 1, "+10 Ammunition - %d$ - Current: %d", ammoCostInt, playerAmmo);
        if(weaponTier < 5){
            if(boughtWeapon == true){
                attron(COLOR_PAIR(6));
                mvprintw(13, 1, "%s - Fire Rate:%d - Clip Capacity:%d - Reload Speed:%d - Price:%d$", shopWeaponNames[shopWeapons1[weaponTier][4]], shopWeapons1[weaponTier][0], shopWeapons1[weaponTier][1], shopWeapons1[weaponTier][2], shopWeapons1[weaponTier][3]);
                mvprintw(14, 1, "%s - Fire Rate:%d - Clip Capacity:%d - Reload Speed:%d - Price:%d$", shopWeaponNames[shopWeapons2[weaponTier][4]], shopWeapons2[weaponTier][0], shopWeapons2[weaponTier][1], shopWeapons2[weaponTier][2], shopWeapons2[weaponTier][3]);
                mvprintw(15, 1, "%s - Fire Rate:%d - Clip Capacity:%d - Reload Speed:%d - Price:%d$", shopWeaponNames[shopWeapons3[weaponTier][4]], shopWeapons3[weaponTier][0], shopWeapons3[weaponTier][1], shopWeapons3[weaponTier][2], shopWeapons3[weaponTier][3]);
                attroff(COLOR_PAIR(6));
            }else{
                mvprintw(13, 1, "%s - Fire Rate:%d - Clip Capacity:%d - Reload Speed:%d - Price:%d$", shopWeaponNames[shopWeapons1[weaponTier][4]], shopWeapons1[weaponTier][0], shopWeapons1[weaponTier][1], shopWeapons1[weaponTier][2], shopWeapons1[weaponTier][3]);
                mvprintw(14, 1, "%s - Fire Rate:%d - Clip Capacity:%d - Reload Speed:%d - Price:%d$", shopWeaponNames[shopWeapons2[weaponTier][4]], shopWeapons2[weaponTier][0], shopWeapons2[weaponTier][1], shopWeapons2[weaponTier][2], shopWeapons2[weaponTier][3]);
                mvprintw(15, 1, "%s - Fire Rate:%d - Clip Capacity:%d - Reload Speed:%d - Price:%d$", shopWeaponNames[shopWeapons3[weaponTier][4]], shopWeapons3[weaponTier][0], shopWeapons3[weaponTier][1], shopWeapons3[weaponTier][2], shopWeapons3[weaponTier][3]);
            }
        }

        move(9+y, 0);
        addch('>');

        refresh();

        int input = getch();

        switch(input){
            case KEY_UP:
                if(y > 0){
                    y--;
                }
                break;

            case KEY_DOWN:
                if(y < 6 && (y < 3 || weaponTier < 5)){
                    y++;
                }
                break;

            case ' ':

                switch(y){
                    case 0:
                        done = true;
                        break;

                    case 1:
                        if(playerMoney >= healthCostInt){
                            playerHealth++;
                            playerMoney -= healthCostInt;
                            healthTotalPaid += healthCostInt;
                            healthCost *= 1.1;
                        }else{
                            littleMoneyWarning = true;
                        }
                        break;

                    case 2:
                        if(playerMoney >= moreKillMoneyCost){
                            killMoney++;
                            playerMoney -= moreKillMoneyCost;
                            moreKillMoneyTotalPaid += moreKillMoneyCost;
                            moreKillMoneyCost *= 2;
                        }else{
                            littleMoneyWarning = true;
                        }
                        break;

                    case 3:
                        if(playerMoney >= ammoCostInt){
                            playerAmmo += 10;
                            playerMoney -= ammoCostInt;
                            ammoTotalPaid += ammoCostInt;
                            ammoCost *= 1.05;
                        }else{
                            littleMoneyWarning = true;
                        }
                        break;

                    case 4:
                        if(playerMoney >= shopWeapons1[weaponTier][3] && !boughtWeapon){
                            boughtWeapon = true;
                            playerMoney -= shopWeapons1[weaponTier][3];
                            weaponTotalPaid += shopWeapons1[weaponTier][3];
                            playerFiringSpeed = shopWeapons1[weaponTier][0];
                            playerClipLimit = shopWeapons1[weaponTier][1];
                            playerReloadingRate = shopWeapons1[weaponTier][2];
                            for(int i = 0; i < 30; i++){
                                playerWeaponName[i] = shopWeaponNames[shopWeapons1[weaponTier][4]][i];
                            }

                        }else if(playerMoney < shopWeapons1[weaponTier][3]){
                            littleMoneyWarning = true;
                        }
                        break;

                    case 5:
                        if(playerMoney >= shopWeapons2[weaponTier][3] && !boughtWeapon){
                            boughtWeapon = true;
                            playerMoney -= shopWeapons2[weaponTier][3];
                            weaponTotalPaid += shopWeapons2[weaponTier][3];
                            playerFiringSpeed = shopWeapons2[weaponTier][0];
                            playerClipLimit = shopWeapons2[weaponTier][1];
                            playerReloadingRate = shopWeapons2[weaponTier][2];
                            for(int i = 0; i < 30; i++){
                                playerWeaponName[i] = shopWeaponNames[shopWeapons2[weaponTier][4]][i];
                            }

                        }else if(playerMoney < shopWeapons2[weaponTier][3]){
                            littleMoneyWarning = true;
                        }
                        break;

                    case 6:
                        if(playerMoney >= shopWeapons3[weaponTier][3] && !boughtWeapon){
                            boughtWeapon = true;
                            playerMoney -= shopWeapons3[weaponTier][3];
                            weaponTotalPaid += shopWeapons3[weaponTier][3];
                            playerFiringSpeed = shopWeapons3[weaponTier][0];
                            playerClipLimit = shopWeapons3[weaponTier][1];
                            playerReloadingRate = shopWeapons3[weaponTier][2];
                            for(int i = 0; i < 30; i++){
                                playerWeaponName[i] = shopWeaponNames[shopWeapons3[weaponTier][4]][i];
                            }

                        }else if(playerMoney < shopWeapons3[weaponTier][3]){
                            littleMoneyWarning = true;
                        }
                        break;

                    default:
                        break;

                }

                break;

            case 27:
                done = true;
                break;

            case 'q':
                done = true;
                break;

            default:
                break;
        }
    }
    if(boughtWeapon)
        weaponTier++;
}

void gameLost(){
    clear();
    mvprintw(0,0,"GAME OVER");
    mvprintw(1,0,"Zombie Killed: %d", playerScore);
    mvprintw(2,0,"Money Left: %d$", playerMoney);
    mvprintw(3,0,"Total Money Earned: %d$", playerTotalEarnedMoney);
    mvprintw(4,0,"Total Money Spent: %d$", playerTotalEarnedMoney - playerMoney);
    mvprintw(5,0,"Money Spent On Health: %d$", healthTotalPaid);
    mvprintw(6,0,"Money Spent On Kill Money: %d$", moreKillMoneyTotalPaid);
    mvprintw(7,0,"Money Spent On Ammo: %d$", ammoTotalPaid);
    mvprintw(8,0,"Money Spent On Weapons: %d$", weaponTotalPaid);
    mvprintw(9,0,"Press the X button in the top right corner to quit the game...");

    while(true){
        timeout(-1);
        getch();
    }
}
