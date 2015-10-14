#include "Engine.h"
#include "MissileEntity.h"

MissileEntity::MissileEntity(){
    this->angle = 0;
    this->movementSpeed = 0;
    this->deltaX = 0;
    this->deltaY = 0;
    this->colX = false;
    this->colY = false;
    this->centerX = 0;
    this->centerY = 0;
}

void MissileEntity::setAngle(float angle){
    this->angle = angle;
}
void MissileEntity::setMovementSpeed(float movementSpeed){
    this->movementSpeed = movementSpeed;
}
void MissileEntity::updateCenter(){
    this->centerX = this->posX + this->width/2, this->centerY = this->posY + this->height/2;
}

