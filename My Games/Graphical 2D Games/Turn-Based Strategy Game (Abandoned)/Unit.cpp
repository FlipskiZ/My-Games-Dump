#include "Engine.h"

Unit::Unit(){

	this->posX = 0;
	this->posY = 0;
    this->unitId = 0;
	this->health = 0;
	this->maxHealth = 0;
	this->movementPoints = 0;
	this->active = true;
}

void Unit::setPos(int x, int y){
    this->posX = x;
    this->posY = y;
}

void Unit::setUnitId(int unitId){
    this->unitId = unitId;
}

void Unit::setHealth(float health){
    this->health = health;
}

void Unit::setMaxHealth(float maxHealth){
    this->maxHealth = maxHealth;
}

void Unit::setMovementPoints(int movementPoints){
    this->movementPoints = movementPoints;
}

void Unit::setActive(bool active){
    this->active = active;
}

bool Unit::checkActive(){
    return this->active;
}

void Unit::draw(){
    if(this->unitId == 0 && checkInsideMapDisplay(this->posX, this->posY)){
        al_draw_filled_rectangle(this->posX*tileSize + cameraOffsetX, this->posY*tileSize + cameraOffsetY, (this->posX+1)*tileSize + cameraOffsetX, (this->posY+1)*tileSize + cameraOffsetY, al_map_rgb(255,75,75));
    }
}

void Unit::update(){
    if(this->health <= 0){
        this->active = false;
    }

    float deltaY, deltaX;

    if(this->unitId == 0){

    }

    if(isPassable(this->posX+deltaX, this->posY)){
        this->posX += deltaX;
    }

    if(isPassable(this->posX, this->posY+deltaY)){
        this->posY += deltaY;
    }
}
