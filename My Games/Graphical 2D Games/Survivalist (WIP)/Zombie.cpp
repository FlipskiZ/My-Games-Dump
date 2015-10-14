#include "Engine.h"
#include "Zombie.h"

Zombie::Zombie()
{
    this->animationValue = 1;
}

void Zombie::update(){

    this->angle = -atan2(this->centerX - playerEntity->centerX, this->centerY - playerEntity->centerY);

    this->deltaX = sin(this->angle);
    this->deltaY = -cos(this->angle);

    if(isPassable(this->posX + this->deltaX, this->posY, this->width, this->height) && !this->colX){
        this->posX += this->deltaX;
    }

    if(isPassable(this->posX, this->posY + this->deltaY, this->width, this->height) && !this->colY){
        this->posY += this->deltaY;
    }

    this->updateCenter();

    this->updateAnimation();

    this->deltaX = 0;
    this->deltaY = 0;
}
