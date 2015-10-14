#include "Engine.h"
#include "Actor.h"
#include "Bullet.h"
#include "Shop.h"
#include "SkillTree.h"
#include "SpecialBullet.h"
#include <cstdlib>
#include <ctime>
#include <curses.h>

SpecialBullet::SpecialBullet(){
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

	this->minigun = false;
	this->weaponSprayDirection = rand() % 2;

    this->rocket = false;
    this->rocketExplosion = false;

    this->granade = false;
    this->granadeTravel = true;
    this->frag = false;

    this->random = false;

    this->mine = false;

    this->granadeGranade = false;

    this->granadeGranadeGranade = false;

    this->potato = false;

    this-> piercing = false;
}
void SpecialBullet::setAppearance(char displayChar, short displayColor){
    this->displayChar = displayChar;
    this->colorCode = displayColor;
}

void SpecialBullet::setPos(int y, int x){
     if((y < 0) || (y >= mapHeight || x < 0) || (x >= mapWidth)){
        return;
     }
    this->posY = y;
    this->posX = x;
}

void SpecialBullet::setDamage(int damage){
    this->damage = damage;
}

void SpecialBullet::setSpeed(int speed){
    this->movementSpeed = speed;
}

void SpecialBullet::setRange(int range){
    this->range = range;
}

void SpecialBullet::setDirection(int direction){
    this->direction = direction;
}

void SpecialBullet::setActive(bool active){
    this->active = active;
}

bool SpecialBullet::checkActive(){
    return this->active;
}

void SpecialBullet::setBulletType(bool minigun, bool rocket, bool rocketExplosion, bool granade, bool frag, bool random, bool mine, bool granadeGranade, bool granadeGranadeGranade, bool potato, bool piercing){
    this->minigun = minigun;
    this->rocket = rocket;
    this->rocketExplosion = rocketExplosion;
    this->granade = granade;
    this->frag = frag;
    this->random = random;
    this->mine = mine;
    this->granadeGranade = granadeGranade;
    this->granadeGranadeGranade = granadeGranadeGranade;
    this->potato = potato;
    this->piercing = piercing;
}

void SpecialBullet::draw(){
    if(isPassable(this->posY, this->posX) || mapArray[this->posY][this->posX] == 8){
        attron(COLOR_PAIR(this->colorCode));
        move(this->posY, this->posX);
        addch(this->displayChar);
        attroff(COLOR_PAIR(this->colorCode));
    }
}

void SpecialBullet::update(){
    if(this->posY < 0 || this->posX < 0){
        this->active = false;
        return;
    }
    if(minigun){
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

            if(direction == 0 || direction == 1){
                if(rand() % 20 == 0){
                    if(weaponSprayDirection == 0){
                        deltaX = 1;
                    }else{
                        deltaX = -1;
                    }
                }
            }else if(direction == 2 || direction == 3){
                if(rand() % 20 == 0){
                    if(weaponSprayDirection == 0){
                        deltaY = 1;
                    }else{
                        deltaY = -1;
                    }
              }
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
                }else if(actorId1 != -1){
                    this->active = false;
                    this->posY = -1;
                    this->posX = -1;
                    bulletHit(actorId1, this->damage);
                }
            }else{
                this->active = false;
                this->posY = -1;
                this->posX = -1;
            }
        }else{
            this->movementSpeedHelper++;
        }
    }else if(rocket){
        if(rangeHelper >= range){
            this->active = false;
            for(int y = -1; y < 2; y++){
                for(int x = -1; x < 2; x++){
                    addSpecialBulletToListSpecial(0, this->posY+y, this->posX+x, 100, 0, 0, false, true, false, false, false, false, false, false, false, false);
                }
            }
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

            if(isPassableBullet(this->posY+deltaY, this->posX+deltaX) && actorId2 == -1){
                this->posY += deltaY;
                this->posX += deltaX;
                movementSpeedHelper = 0;
                rangeHelper++;
            }else if(actorId2 != -1){
                this->active = false;
                for(int y = -1; y < 2; y++){
                    for(int x = -1; x < 2; x++){
                        addSpecialBulletToListSpecial(0, this->posY+y, this->posX+x, 100, 0, 0, false, true, false, false, false, false, false, false, false, false);
                    }
                }
                this->posY = -1;
                this->posX = -1;
                bulletHit(actorId2, this->damage);
            }else if(actorId1 == -1){
                this->active = false;
                for(int y = -1; y < 2; y++){
                    for(int x = -1; x < 2; x++){
                        addSpecialBulletToListSpecial(0, this->posY+y, this->posX+x, 100, 0, 0, false, true, false, false, false, false, false, false, false, false);
                    }
                }
                this->posY = -1;
                this->posX = -1;
            }
        }else{
            this->movementSpeedHelper++;
        }
    }else if(rocketExplosion){
        this->rangeHelper++;

        this->colorCode = 3;

        if(rangeHelper >= range){
            this->active = false;
            this->posY = -1;
            this->posX = -1;
        }

        int actorId2 = testForActor(this->posY, this->posX);

        if(actorId2 != -1){
            this->active = false;
            this->posY = -1;
            this->posX = -1;
            bulletHit(actorId2, rand()%11);
        }
    }else if(granade){
        this->colorCode = 1;
        if(rangeHelper >= range){
            this->active = false;

            for(int y = -1; y < 2; y++){
                for(int x = -1; x < 2; x++){
                    addSpecialBulletToListSpecial(0, this->posY+y, this->posX+x, 100, 0, 0, false, true, false, false, false, false, false, false, false, false);
                }
            }

            addSpecialBulletToListSpecial(4, this->posY-1, this->posX-1, 1 + (rand() % 10), 5, 0, false, false, false, true, false, false, false, false, false, false);
            addSpecialBulletToListSpecial(0, this->posY-1, this->posX, 1 + (rand() % 10), 5, 0, false, false, false, true, false, false, false, false, false, false);
            addSpecialBulletToListSpecial(5, this->posY-1, this->posX+1, 1 + (rand() % 10), 5, 0, false, false, false, true, false, false, false, false, false, false);
            addSpecialBulletToListSpecial(2, this->posY, this->posX-1, 1 + (rand() % 10), 5, 0, false, false, false, true, false, false, false, false, false, false);
            addSpecialBulletToListSpecial(3, this->posY, this->posX+1, 1 + (rand() % 10), 5, 0, false, false, false, true, false, false, false, false, false, false);
            addSpecialBulletToListSpecial(6, this->posY+1, this->posX-1, 1 + (rand() % 10), 5, 0, false, false, false, true, false, false, false, false, false, false);
            addSpecialBulletToListSpecial(1, this->posY+1, this->posX, 1 + (rand() % 10), 5, 0, false, false, false, true, false, false, false, false, false, false);
            addSpecialBulletToListSpecial(7, this->posY+1, this->posX+1, 1 + (rand() % 10), 5, 0, false, false, false, true, false, false, false, false, false, false);

            if(testForActor(this->posY, this->posX) != -1)
                bulletHit(testForActor(this->posY, this->posX), 10);
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
            }else if(this->direction == 4){
                deltaY = -1;
                deltaX = -1;
            }else if(this->direction == 5){
                deltaY = -1;
                deltaX = 1;
            }else if(this->direction == 6){
                deltaY = 1;
                deltaX = -1;
            }else if(this->direction == 7){
                deltaY = 1;
                deltaX = 1;
            }

            int actorId1 = testForActor(this->posY+deltaY, this->posX+deltaX);
            int actorId2 = testForActor(this->posY, this->posX);

            if(isPassableBullet(this->posY+deltaY, this->posX+deltaX) && actorId1 == -1 && actorId2 == -1 && this->granadeTravel){
                this->posY += deltaY;
                this->posX += deltaX;
                movementSpeedHelper = 0;
                rangeHelper++;
            }else{
                this->granadeTravel = false;
                movementSpeedHelper = 0;
                rangeHelper++;
            }
        }else{
            this->movementSpeedHelper++;
        }
    }else if(frag){
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
            }else if(this->direction == 4){
                deltaY = -1;
                deltaX = -1;
            }else if(this->direction == 5){
                deltaY = -1;
                deltaX = 1;
            }else if(this->direction == 6){
                deltaY = 1;
                deltaX = -1;
            }else if(this->direction == 7){
                deltaY = 1;
                deltaX = 1;
            }

            int actorId1 = testForActor(this->posY+deltaY, this->posX+deltaX);
            int actorId2 = testForActor(this->posY, this->posX);

            if(isPassableBullet(this->posY+deltaY, this->posX+deltaX)){
                this->posY += deltaY;
                this->posX += deltaX;
                movementSpeedHelper = 0;
                rangeHelper++;
            }else if(actorId1 != -1 || actorId2 != -1){
                    this->active = false;
                    this->posY = -1;
                    this->posX = -1;
                    bulletHit(actorId2, this->range-rangeHelper);
                }else if(actorId1 != -1){
                    this->active = false;
                    this->posY = -1;
                    this->posX = -1;
                    bulletHit(actorId1, this->range-rangeHelper);
            }else{
                this->active = false;
                this->posY = -1;
                this->posX = -1;
            }
        }else{
            this->movementSpeedHelper++;
        }
    }else if(random){
        if(rangeHelper >= range){
            this->active = false;
            this->posY = -1;
            this->posX = -1;
        }
        this->movementSpeed = rand() % 100;
        this->movementSpeedHelper = rand() % 2;
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
            }else if(this->direction == 4){
                deltaY = -1;
                deltaX = -1;
            }else if(this->direction == 5){
                deltaY = -1;
                deltaX = 1;
            }else if(this->direction == 6){
                deltaY = 1;
                deltaX = -1;
            }else if(this->direction == 7){
                deltaY = 1;
                deltaX = 1;
            }

            this->damage = 1 + rand() % 10;
            this->colorCode = rand() % 5;
            this->displayChar = 33 + rand() % 93;
            if(playerSpecialWeaponTypes[4]){
                playerClipLimit = 1 + rand() % 49;
                playerReloadingRate = rand() % 20000;
                playerFiringSpeed = rand() % 1000;
            }

            int actorId1 = testForActor(this->posY+deltaY, this->posX+deltaX);
            int actorId2 = testForActor(this->posY, this->posX);

            if(isPassableBullet(this->posY+deltaY, this->posX+deltaX) && actorId1 == -1 && actorId2 == -1){
                this->posY += deltaY;
                this->posX += deltaX;
                movementSpeedHelper = 0;
                rangeHelper++;
            }else if(actorId1 != -1 || actorId2 != -1){
                if(rand() % 3 == 0){
                    for(int i = 0; i < 1+rand() % 5; i++){
                        addSpecialBulletToListSpecial(rand() % 8, this->posY, this->posX, 40, 0, 0, false, false, false, false, true, false, false, false, false, false);
                    }
                }
                if(actorId2 != -1){
                    this->active = false;
                    this->posY = -1;
                    this->posX = -1;
                    bulletHit(actorId2, this->damage);
                }else if(actorId1 != -1){
                    this->active = false;
                    this->posY = -1;
                    this->posX = -1;
                    bulletHit(actorId1, this->damage);
                }
            }else{
                this->active = false;
                this->posY = -1;
                this->posX = -1;
            }
        }else{
            this->movementSpeedHelper++;
        }
    }else if(mine){

        int actorId2 = testForActor(this->posY, this->posX);

        if(actorId2 != -1){
            this->active = false;
            for(int y = -1; y < 2; y++){
                for(int x = -1; x < 2; x++){
                    addSpecialBulletToListSpecial(0, this->posY+y, this->posX+x, 100, 0, 0, false, true, false, false, false, false, false, false, false, false);
                }
            }
            this->posY = -1;
            this->posX = -1;
            bulletHit(actorId2, this->damage);
        }
    }else if(granadeGranade){
        this->colorCode = 1;
        if(rangeHelper >= range){
            this->active = false;

            for(int y = -1; y < 2; y++){
                for(int x = -1; x < 2; x++){
                    addSpecialBulletToListSpecial(0, this->posY+y, this->posX+x, 100, 0, 0, false, true, false, false, false, false, false, false, false, false);
                }
            }

            addSpecialBulletToListSpecial(4, this->posY-1, this->posX-1, 5 + (rand() % 11), 25, 10, false, false, true, false, false, false, false, false, false, false);
            addSpecialBulletToListSpecial(0, this->posY-1, this->posX, 5 + (rand() % 11), 25, 10, false, false, true, false, false, false, false, false, false, false);
            addSpecialBulletToListSpecial(5, this->posY-1, this->posX+1, 5 + (rand() % 11), 25, 10, false, false, true, false, false, false, false, false, false, false);
            addSpecialBulletToListSpecial(2, this->posY, this->posX-1, 5 + (rand() % 11), 25, 10, false, false, true, false, false, false, false, false, false, false);
            addSpecialBulletToListSpecial(3, this->posY, this->posX+1, 5 + (rand() % 11), 25, 10, false, false, true, false, false, false, false, false, false, false);
            addSpecialBulletToListSpecial(6, this->posY+1, this->posX-1, 5 + (rand() % 11), 25, 10, false, false, true, false, false, false, false, false, false, false);
            addSpecialBulletToListSpecial(1, this->posY+1, this->posX, 5 + (rand() % 11), 25, 10, false, false, true, false, false, false, false, false, false, false);
            addSpecialBulletToListSpecial(7, this->posY+1, this->posX+1, 5 + (rand() % 11), 25, 10, false, false, true, false, false, false, false, false, false, false);

            if(testForActor(this->posY, this->posX) != -1)
                bulletHit(testForActor(this->posY, this->posX), 10);
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
            }else if(this->direction == 4){
                deltaY = -1;
                deltaX = -1;
            }else if(this->direction == 5){
                deltaY = -1;
                deltaX = 1;
            }else if(this->direction == 6){
                deltaY = 1;
                deltaX = -1;
            }else if(this->direction == 7){
                deltaY = 1;
                deltaX = 1;
            }

            int actorId1 = testForActor(this->posY+deltaY, this->posX+deltaX);
            int actorId2 = testForActor(this->posY, this->posX);

            if(isPassableBullet(this->posY+deltaY, this->posX+deltaX) && actorId1 == -1 && actorId2 == -1 && this->granadeTravel){
                this->posY += deltaY;
                this->posX += deltaX;
                movementSpeedHelper = 0;
                rangeHelper++;
            }else{
                this->granadeTravel = false;
                movementSpeedHelper = 0;
                rangeHelper++;
            }
        }else{
            this->movementSpeedHelper++;
        }
    }else if(granadeGranadeGranade){
        this->colorCode = 1;
        if(rangeHelper >= range){
            this->active = false;

            for(int y = -1; y < 2; y++){
                for(int x = -1; x < 2; x++){
                    addSpecialBulletToListSpecial(0, this->posY+y, this->posX+x, 100, 0, 0, false, true, false, false, false, false, false, false, false, false);
                }
            }

            addSpecialBulletToListSpecial(4, this->posY-1, this->posX-1, 5 + (rand() % 11), 25, 10, false, false, false, false, false, false, true, false, false, false);
            addSpecialBulletToListSpecial(0, this->posY-1, this->posX, 5 + (rand() % 11), 25, 10, false, false, false, false, false, false, true, false, false, false);
            addSpecialBulletToListSpecial(5, this->posY-1, this->posX+1, 5 + (rand() % 11), 25, 10, false, false, false, false, false, false, true, false, false, false);
            addSpecialBulletToListSpecial(2, this->posY, this->posX-1, 5 + (rand() % 11), 25, 10, false, false, false, false, false, false, true, false, false, false);
            addSpecialBulletToListSpecial(3, this->posY, this->posX+1, 5 + (rand() % 11), 25, 10, false, false, false, false, false, false, true, false, false, false);
            addSpecialBulletToListSpecial(6, this->posY+1, this->posX-1, 5 + (rand() % 11), 25, 10, false, false, false, false, false, false, true, false, false, false);
            addSpecialBulletToListSpecial(1, this->posY+1, this->posX, 5 + (rand() % 11), 25, 10, false, false, false, false, false, false, true, false, false, false);
            addSpecialBulletToListSpecial(7, this->posY+1, this->posX+1, 5 + (rand() % 11), 25, 10, false, false, false, false, false, false, true, false, false, false);

            if(testForActor(this->posY, this->posX) != -1)
                bulletHit(testForActor(this->posY, this->posX), 10);
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
            }else if(this->direction == 4){
                deltaY = -1;
                deltaX = -1;
            }else if(this->direction == 5){
                deltaY = -1;
                deltaX = 1;
            }else if(this->direction == 6){
                deltaY = 1;
                deltaX = -1;
            }else if(this->direction == 7){
                deltaY = 1;
                deltaX = 1;
            }

            int actorId1 = testForActor(this->posY+deltaY, this->posX+deltaX);
            int actorId2 = testForActor(this->posY, this->posX);

            if(isPassableBullet(this->posY+deltaY, this->posX+deltaX) && actorId1 == -1 && actorId2 == -1 && this->granadeTravel){
                this->posY += deltaY;
                this->posX += deltaX;
                movementSpeedHelper = 0;
                rangeHelper++;
            }else{
                this->granadeTravel = false;
                movementSpeedHelper = 0;
                rangeHelper++;
            }
        }else{
            this->movementSpeedHelper++;
        }
    }else if(potato){
        this->displayChar = 'O';
        this->colorCode = 2;
        if(rangeHelper >= range){
            this->active = false;
            addSpecialBulletToListSpecial(0, this->posY, this->posX, 40, 100, 10, false, false, false, false, false, false, false, false, true, false);
            addSpecialBulletToListSpecial(1, this->posY, this->posX, 40, 100, 10, false, false, false, false, false, false, false, false, true, false);
            addSpecialBulletToListSpecial(2, this->posY, this->posX, 40, 100, 10, false, false, false, false, false, false, false, false, true, false);
            addSpecialBulletToListSpecial(3, this->posY, this->posX, 40, 100, 10, false, false, false, false, false, false, false, false, true, false);
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

            if(isPassableBullet(this->posY+deltaY, this->posX+deltaX) && actorId2 == -1){
                this->posY += deltaY;
                this->posX += deltaX;
                movementSpeedHelper = 0;
                rangeHelper++;
            }else if(actorId2 != -1){
                this->active = false;
                addSpecialBulletToListSpecial(0, this->posY, this->posX, 40, 100, 10, false, false, false, false, false, false, false, false, true, false);
                addSpecialBulletToListSpecial(1, this->posY, this->posX, 40, 100, 10, false, false, false, false, false, false, false, false, true, false);
                addSpecialBulletToListSpecial(2, this->posY, this->posX, 40, 100, 10, false, false, false, false, false, false, false, false, true, false);
                addSpecialBulletToListSpecial(3, this->posY, this->posX, 40, 100, 10, false, false, false, false, false, false, false, false, true, false);
                this->posY = -1;
                this->posX = -1;
                bulletHit(actorId2, this->damage);
            }else if(actorId1 == -1){
                this->active = false;
                this->posY = -1;
                this->posX = -1;
            }
        }else{
            this->movementSpeedHelper++;
        }
    }else if(piercing){
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

            if((isPassableBullet(this->posY+deltaY, this->posX+deltaX) || actorId1 != -1) && actorId2 == -1){
                this->posY += deltaY;
                this->posX += deltaX;
                movementSpeedHelper = 0;
                rangeHelper++;
            }else if(actorId2 != -1){
                this->range -= 5;
                bulletHit(actorId2, this->damage);
                this->posY += deltaY;
                this->posX += deltaX;
                movementSpeedHelper = 0;
                rangeHelper++;
            }else{
                this->active = false;
                this->posY = -1;
                this->posX = -1;
            }
        }else{
            this->movementSpeedHelper++;
        }
    }
}
