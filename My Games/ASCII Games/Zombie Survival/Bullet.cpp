#include "Engine.h"
#include "Actor.h"
#include "Bullet.h"
#include "Shop.h"
#include "SkillTree.h"
#include "SpecialBullet.h"
#include <cstdlib>
#include <ctime>
#include <curses.h>

Bullet::Bullet(){
	this->displayChar = 0;
	this->colorCode = 0;
	this->posY = 0;
	this->posX = 0;
	this->damage = 0;
	this->movementSpeed = 0;
	this->movementSpeedHelper = 0;
	this->range = 0;
	this->rangeHelper = 0;
	this->active = true;
}
void Bullet::setAppearance(char displayChar, short displayColor){
    this->displayChar = displayChar;
    this->colorCode = displayColor;
}

void Bullet::setPos(int y, int x){
     if((y < 0) || (y >= mapHeight || x < 0) || (x >= mapWidth)){
        return;
     }
    this->posY = y;
    this->posX = x;
}

void Bullet::setDamage(int damage){
    this->damage = damage;
}

void Bullet::setSpeed(int speed){
    this->movementSpeed = speed;
}

void Bullet::setRange(int range){
    this->range = range;
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
    if(isPassable(this->posY, this->posX) || mapArray[this->posY][this->posX] == 8){
        attron(COLOR_PAIR(this->colorCode));
        move(this->posY, this->posX);
        addch(this->displayChar);
        attroff(COLOR_PAIR(this->colorCode));
    }
}

void Bullet::update(){
    if(rangeHelper >= range){
        this->active = false;
        this->posY = -1;
        this->posX = -1;
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

        int actorId1 = testForActor(this->posY+deltaY, this->posX+deltaX);
        int actorId2 = testForActor(this->posY, this->posX);

        if(isPassableBullet(this->posY+deltaY, this->posX+deltaX) && actorId1 == -1 && actorId2 == -1){
            this->posY += deltaY;
            this->posX += deltaX;
            movementSpeedHelper = 0;
            rangeHelper++;
        }else if(actorId1 != -1 || actorId2 != -1){
            if(actorId2 != -1){
                this->active = false;
                this->posY = -1;
                this->posX = -1;
                bulletHit(actorId2, this->damage);
                this->movementSpeedHelper = 0;
            }else if(actorId1 != -1){
                this->active = false;
                this->posY = -1;
                this->posX = -1;
                bulletHit(actorId1, this->damage);
                this->movementSpeedHelper = 0;
            }
        }else{
            this->active = false;
            this->posY = -1;
            this->posX = -1;
        }
    }else{
        this->movementSpeedHelper++;
    }
}
