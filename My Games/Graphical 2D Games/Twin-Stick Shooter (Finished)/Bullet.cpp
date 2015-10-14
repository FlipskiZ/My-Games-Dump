#include "Engine.h"

Bullet::Bullet(){
	this->posX = 0;
	this->posY = 0;
	this->width = 0;
	this->height = 0;
	this->damage = 0;
	this->moveSpeed = 0;
	this->angle = 0;
	this->playerShot = false;
	this->active = true;
}

void Bullet::setPos(int x, int y){
    this->posX = x;
    this->posY = y;
}

void Bullet::setDimensions(int width, int height){
    this->width = width;
    this->height = height;
}

void Bullet::setDamage(int damage){
    this->damage = damage;
}

void Bullet::setAngleSpeed(float moveSpeed, float angle){
    this->moveSpeed = moveSpeed;
    this->angle = angle;
}

void Bullet::setPlayerShot(bool playerShot){
    this->playerShot = playerShot;
}

void Bullet::setActive(bool active){
    this->active = active;
}

bool Bullet::checkActive(){
    return this->active;
}

void Bullet::draw(){
    if(!this->playerShot){
        al_draw_filled_rectangle(this->posX, this->posY, this->posX + this->width, this->posY + this->height, al_map_rgb(255, 50, 50));
    }else{
        al_draw_rotated_bitmap(bulletImage, this->width/2, this->height/2, this->posX+width/2, this->posY+height/2, this->angle, NULL);
    }
}

void Bullet::update(){
    float deltaX = 0;
    float deltaY = 0;

    deltaX = sin(this->angle) * this->moveSpeed;
    deltaY = -cos(this->angle) * this->moveSpeed;

    if(!inShop){
        if(this->playerShot){
            for(int i = 0; i < MAX_ZOMBIES; i++){
                if(zombieList[i] != NULL && zombieList[i]->checkActive()){
                    if(checkCollision(this->posX, this->posY, zombieList[i]->posX, zombieList[i]->posY, this->width, this->height, zombieList[i]->width, zombieList[i]->height)){
                        zombieList[i]->health -= this->damage;
                        this->active = false;
                    }
                }
            }
        }else{
            if(checkCollision(this->posX, this->posY, playerCenterX, playerCenterY, this->width, this->height, playerWidth, playerHeight)){
                playerHealth -= this->damage;
                this->active = false;
            }
        }

        if(isPassable(this->posX+deltaX, this->posY, this->width, this->height, deltaX, deltaY)){
            this->posX += deltaX;
        }else{
            this->active = false;
        }

        if(isPassable(this->posX, this->posY+deltaY, this->width, this->height, deltaX, deltaY)){
            this->posY += deltaY;
        }else{
            this->active = false;
        }
    }else{
        if(this->posX+deltaX >= 0 && this->posX+deltaX + this->width < 400 && this->posY >= 0 && this->posY + this->height < 400){
            this->posX += deltaX;
        }else{
            this->active = false;
        }
        if(this->posX >= 0 && this->posX + this->width < 400 && this->posY+deltaY >= 0 && this->posY+deltaY + this->height < 400){
            this->posY += deltaY;
        }else{
            this->active = false;
        }
    }
}
