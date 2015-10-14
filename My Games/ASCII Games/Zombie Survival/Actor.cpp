#include "Engine.h"
#include "Actor.h"
#include "Bullet.h"
#include "Shop.h"
#include "SkillTree.h"
#include "SpecialBullet.h"
#include <cstdlib>
#include <ctime>
#include <curses.h>

Actor::Actor(){
    this->health = 0;
	this->displayChar = 0;
	this->colorCode = 0;
	this->posY = 0;
	this->posX = 0;
	this->movementSpeed = 0;
	this->movementSpeedHelper = 0;
	this->active = true;
	this->metObstacle = false;
}
void Actor::setAppearance(char displayChar, short displayColor){
    this->displayChar = displayChar;
    this->colorCode = displayColor;
}

void Actor::setPos(int y, int x){
     if((y < 0) || (y >= mapHeight || x < 0) || (x >= mapWidth)){
        return;
     }
    this->posY = y;
    this->posX = x;
}

void Actor::setSpeed(int speed){
    this->movementSpeed = speed;
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

int Actor::checkHealth(){
    return this->health;
}

void Actor::setMaxRange(int maxRange){
    this->maxRange = maxRange;
}

void Actor::draw(){
    if((this->posY < 0) || (this->posY >= mapHeight || this->posX < 0) || this->posX >= mapWidth){
        return;
    }
    attron(COLOR_PAIR(this->colorCode));
    move(this->posY, this->posX);
    addch(this->displayChar);
    attroff(COLOR_PAIR(this->colorCode));
}

void Actor::update(){
    if(this->health >= 7){
        this->colorCode = 1;
    }else if(this->health >= 4){
        this->colorCode = 2;
    }else if(this->health >= 1){
        this->colorCode = 3;
    }else if(this->health <= 0){
        this->active = false;
        this->posY = -1;
        this->posX = -1;
        playerMoney += killMoney;
        playerTotalMoneyEarned += killMoney;
        playerXP += playerXPGet;
        playerZombiesKilled++;
    }
    if(rand()%20 == 0){
        this->movementSpeedHelper++;
    }
    if(this->movementSpeedHelper >= movementSpeed){
        int deltaY = 0;
        int deltaX = 0;
        int y = cursorY;
        int x = cursorX;
        bool check = false;
        int temp = 0;
        int temp2 = 0;

        if(this->posY - y >= 0 && this->posX - x >= 0){
            if(this->posY - y <= this->maxRange && this->posX - x <= this->maxRange)
                check = true;

        }else if(this->posY - y <= 0 && this->posX - x >= 0){
            temp = posY - y;
            if(this->posY - y - temp - temp <= this->maxRange && this->posX - x <= this->maxRange)
                check = true;

        }else if(this->posY - y >= 0 && this->posX - x <= 0){
            temp = posX - x;
            if(this->posY - y <= this->maxRange && this->posX - x - temp - temp <= this->maxRange)
                check = true;

        }else if(this->posY - y <= 0 && this->posX - x <= 0){
            temp = posY - y;
            temp2 = posX - x;
            if(this->posY - y - temp - temp <= this->maxRange && this->posX - x - temp2 - temp2 <= this->maxRange)
                check = true;
        }

        if(this->posY > y && check){
            deltaY--;
        }else if(this->posY < y && check){
            deltaY++;
        }
        if(this->posX > x && check){
            deltaX--;
        }else if (this->posX < x && check){
            deltaX++;
        }

        if((this->posY+deltaY == y && this->posX+deltaX == x) || (this->posY == y && this->posX == x)){
            playerHealth -= 5;
            playerTimesHit++;
            deltaY = 0;
            deltaX = 0;
            movementSpeedHelper = 0;
        }

        if(!check){
            deltaY = 1 - rand() % 3;
            deltaX = 1 - rand() % 3;
        }

        int smart = 0;
        if(metObstacle){
            deltaY = 1 - rand() % 3;
            deltaX = 1 - rand() % 3;

            metObstacle = false;

            if(isPassable(this->posY+deltaY, this->posX) || mapArray[this->posY+deltaY][this->posX] == 3 || (mapArray[this->posY+deltaY][this->posX] == 4 && (this->posY+deltaY != cursorY || this->posX != cursorX))){
                this->posY += deltaY;
                movementSpeedHelper = 0;
                if(mapArray[this->posY][this->posX] == 3 || mapArray[this->posY][this->posX] == 4){
                    movementSpeedHelper -= movementSpeed;
                }
            }

            if(isPassable(this->posY, this->posX+deltaX) || mapArray[this->posY][this->posX+deltaX] == 3 || (mapArray[this->posY][this->posX+deltaX] == 4 && (this->posY != cursorY || this->posX+deltaX != cursorX))){
                this->posX += deltaX;
                movementSpeedHelper = 0;
                if(mapArray[this->posY][this->posX] == 3 || mapArray[this->posY][this->posX] == 4){
                    movementSpeedHelper -= movementSpeed;
                }
            }
        }else{
            if(isPassable(this->posY+deltaY, this->posX) || mapArray[this->posY+deltaY][this->posX] == 3 || mapArray[this->posY+deltaY][this->posX] == 4){
                this->posY += deltaY;
                movementSpeedHelper = 0;
                if(mapArray[this->posY][this->posX] == 3 || mapArray[this->posY][this->posX] == 4){
                    movementSpeedHelper -= movementSpeed;
                }
            }else if(deltaX != 0 || deltaY != 0){
                smart++;
            }

            if(isPassable(this->posY, this->posX+deltaX) || mapArray[this->posY][this->posX+deltaX] == 3 || mapArray[this->posY][this->posX+deltaX] == 4){
                this->posX += deltaX;
                movementSpeedHelper = 0;
                if(mapArray[this->posY][this->posX] == 3 || mapArray[this->posY][this->posX] == 4){
                    movementSpeedHelper -= movementSpeed;
                }
            }else if(deltaX != 0 || deltaY != 0){
                smart++;
            }
        }

        if(smart == 2){
            deltaY = 1 - rand() % 3;
            deltaX = 1 - rand() % 3;

            metObstacle = true;

            if(isPassable(this->posY+deltaY, this->posX) || mapArray[this->posY+deltaY][this->posX] == 3 || (mapArray[this->posY+deltaY][this->posX] == 4 && (this->posY+deltaY != cursorY || this->posX != cursorX))){
                this->posY += deltaY;
                movementSpeedHelper = 0;
                if(mapArray[this->posY][this->posX] == 3 || mapArray[this->posY][this->posX] == 4){
                    movementSpeedHelper -= movementSpeed;
                }
            }

            if(isPassable(this->posY, this->posX+deltaX) || mapArray[this->posY][this->posX+deltaX] == 3 || (mapArray[this->posY][this->posX+deltaX] == 4 && (this->posY != cursorY || this->posX+deltaX != cursorX))){
                this->posX += deltaX;
                movementSpeedHelper = 0;
                if(mapArray[this->posY][this->posX] == 3 || mapArray[this->posY][this->posX] == 4){
                    movementSpeedHelper -= movementSpeed;
                }
            }
        }

    }else{
        this->movementSpeedHelper++;
    }
}
