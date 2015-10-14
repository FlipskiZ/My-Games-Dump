#include "Engine.h"
#include "Enemy.h"
#include "Bullet.h"
#include "Button.h"
#include "Pickup.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <cstring>
#include <cmath>
#include <cstdlib>
#include <ctime>

Bullet::Bullet(){
	this->posX = 0;
	this->posY = 0;
	this->width = 0;
	this->height = 0;
	this->damage = 0;
	this->moveSpeed = 0;
	this->angle = 0;
	this->playerShot = false;
	this->playHitSound = false;
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
        al_draw_filled_rectangle(this->posX, this->posY, this->posX + this->width, this->posY + this->height, al_map_rgb(50, 50, 255));
    }
}

void Bullet::update(){
    this->playHitSound = false;

    float deltaX = 0;
    float deltaY = 0;

    deltaX = sin(this->angle * 6.28318530718/* PI*2 */) * this->moveSpeed;
    deltaY = -cos(this->angle * 6.28318530718/* PI*2 */) * this->moveSpeed;

    if(this->playerShot){
        for(int i = 0; i < MAX_ENEMIES; i++){
            if(enemyList[i] != NULL && enemyList[i]->checkActive()){
                if(checkCollision(this->posX, this->posY, enemyList[i]->posX, enemyList[i]->posY, this->width, this->height, enemyList[i]->width, enemyList[i]->height)){
                    enemyList[i]->health -= this->damage;
                    this->active = false;
                    this->playHitSound = true;
                }
            }
        }
    }else{
        if(checkCollision(this->posX, this->posY, playerCenterX - playerHitBoxWidth/2, playerCenterY - playerHitBoxHeight/2, this->width, this->height, playerHitBoxWidth, playerHitBoxHeight)){
            playerHealth -= this->damage;
            this->active = false;
            this->playHitSound = true;
        }
    }

    if(isPassable(this->posX+deltaX, this->posY, 0, 0)){
        this->posX += deltaX;
    }else{
        this->active = false;
    }

    if(isPassable(this->posX, this->posY+deltaY, 0, 0)){
        this->posY += deltaY;
    }else{
        this->active = false;
    }
}
