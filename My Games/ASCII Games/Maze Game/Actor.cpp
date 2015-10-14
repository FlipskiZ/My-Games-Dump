#include "Engine.h"
#include "Actor.h"
#include <cstdlib>
#include <ctime>
#include <curses.h>

Actor::Actor(){
	this->displayChar = 0;
	this->colorCode = 0;
	this->posY = 0;
	this->posX = 0;
	this->movementSpeed = 0;
	this->movementSpeedHelper = 0;
	this->active = true;
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

void Actor::setActive(bool active){
    this->active = active;
}

bool Actor::checkActive(){
    return this->active;
}

void Actor::draw(){
    attron(COLOR_PAIR(this->colorCode));
    move(this->posY, this->posX);
    addch(this->displayChar);
    attroff(COLOR_PAIR(this->colorCode));
}

void Actor::update(){
    if(this->movementSpeedHelper >= movementSpeed){
        int deltaY = 0;
        int deltaX = 0;

        for(int i = 0; i < 50; i++){
            if(rand() % 2 == 0){
                if(rand() % 2 == 0){
                    deltaY = 2;
                }else{
                    deltaY = -2;
                }
            }else{
                if(rand() % 2 == 0){
                    deltaX = 2;
                }else{
                    deltaX = -2;
                }
            }
            if(this->posY+deltaY < height-1 && this->posY+deltaY > 0 && this->posX+deltaX < width-1 && this->posX+deltaX > 0){
                if(mapArray[this->posY+(deltaY/2)][this->posX+(deltaX/2)] == 1 && mapArray[this->posY+deltaY][this->posX+deltaX] == 1){
                    break;
                }
            }

            deltaY = 0;
            deltaX = 0;
        }

        if(deltaY == 0 && deltaX == 0){
            int y = this->posY;
            int x = this->posX;

            if(mapArray[y-1][x] == 0){
                mapArray[y][x] = 2;
                mapArray[y-1][x] = 2;
                this->posY -= 2;

            }else if(mapArray[y+1][x] == 0){
                mapArray[y][x] = 2;
                mapArray[y+1][x] = 2;
                this->posY += 2;

            }else if(mapArray[y][x-1] == 0){
                mapArray[y][x] = 2;
                mapArray[y][x-1] = 2;
                this->posX -= 2;

            }else if(mapArray[y][x+1] == 0){
                mapArray[y][x] = 2;
                mapArray[y][x+1] = 2;
                this->posX += 2;

            }else{
                this->active = false;
                generating = false;
                bool done = false;
                while(!done){
                    int temp;
                    switch(side){
                        case 0:
                            temp = (rand() % width/2)*2-1;
                            if(mapArray[height-2][temp] == 2){
                                done = true;
                                mapArray[height-1][temp] = 3;
                            }
                            break;

                        case 1:
                            temp = (rand() % width/2)*2-1;
                            if(mapArray[1][temp] == 2){
                                done = true;
                                mapArray[0][temp] = 3;
                            }
                            break;

                        case 2:
                            temp = (rand() % height/2)*2-1;
                            if(mapArray[temp][width-2] == 2){
                                done = true;
                                mapArray[temp][width-1] = 3;
                            }
                            break;

                        case 3:
                            temp = (rand() % height/2)*2-1;
                            if(mapArray[temp][1] == 2){
                                done = true;
                                mapArray[temp][0] = 3;
                            }
                            break;
                    }
                }
            }
            this->movementSpeedHelper = this->movementSpeed/1.5;
        }else{
            mapArray[this->posY+(deltaY/2)][this->posX+(deltaX/2)] = 0;
            mapArray[this->posY+deltaY][this->posX+deltaX] = 0;
            mapArray[this->posY][this->posX] = 0;
            this->posY += deltaY;
            this->posX += deltaX;
            this->movementSpeedHelper = 0;
        }

    }else{
        this->movementSpeedHelper++;
    }
}
