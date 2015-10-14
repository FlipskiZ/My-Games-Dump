#include "Engine.h"
#include "LivingEntity.h"

LivingEntity::LivingEntity(){
    this->angle = 0;
    this->movementSpeed = 0;
    this->deltaX = 0;
    this->deltaY = 0;
    this->colX = false;
    this->colY = false;
    this->centerX = 0;
    this->centerY = 0;
}

void LivingEntity::setAngle(float angle){
    this->angle = angle;
}
void LivingEntity::setMovementSpeed(float movementSpeed){
    this->movementSpeed = movementSpeed;
}
void LivingEntity::updateCenter(){
    this->centerX = this->posX + this->width/2, this->centerY = this->posY + this->height/2;
}
void LivingEntity::draw(){
    al_draw_rotated_bitmap(this->frameImage, this->width/2, this->height/2, this->posX+this->width/2+cameraOffsetX, this->posY+this->height/2+cameraOffsetY, this->angle, NULL);
}
