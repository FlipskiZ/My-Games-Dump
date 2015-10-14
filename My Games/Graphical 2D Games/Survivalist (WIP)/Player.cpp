#include "Engine.h"
#include "MenuState.h"
#include "GameState.h"
#include "PlayState.h"

Player::Player(){
    //this->variable = 0;
}

void Player::fireBullet(){
    float width = 6, height = 6, movementSpeed = 64, sheetColums = 3, sheetRows = 1, animationSpeed = 1;

    PistolBullet *newMissile = new PistolBullet();
    newMissile->setPos(this->centerX, this->centerY);
    newMissile->setDimensions(width, height);
    newMissile->setAngle(-atan2(playerEntity->centerX - mouseX + newMissile->width/2 + cameraOffsetX, playerEntity->centerY - mouseY + newMissile->height/2 + cameraOffsetY));
    newMissile->setMovementSpeed(movementSpeed);
    newMissile->setSheetDimensions(sheetColums, sheetRows, width, height);
    newMissile->setAnimationSpeed(animationSpeed);
    newMissile->setBitmap(bulletImage);
    addMissileToList(newMissile);
}

void Player::update(){

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
