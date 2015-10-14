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

Pickup::Pickup(){
	this->posX = 0;
	this->posY = 0;
	this->width = 16;
	this->height = 16;
	this->moveSpeed = 1.25;
	this->pickupId = 0;
	this->playPickupSound = false;
	this->active = true;
}

void Pickup::setPos(int x, int y){
    this->posX = x;
    this->posY = y;
}

void Pickup::setPickupId(int pickupId){
    this->pickupId = pickupId;
}

void Pickup::setActive(bool active){
    this->active = active;
}

bool Pickup::checkActive(){
    return this->active;
}

void Pickup::draw(){
    al_draw_filled_rectangle(this->posX, this->posY, this->posX + this->width, this->posY + this->height, al_map_rgb(50, 50, 255));
}

void Pickup::update(){
    float deltaX = 0;
    float deltaY = 0;

    this->playPickupSound = false;

    deltaY = this->moveSpeed;

    if(checkCollision(playerCenterX - playerHitBoxWidth/2, playerCenterY - playerHitBoxHeight/2, this->posX, this->posY, playerHitBoxWidth, playerHitBoxHeight, this->width, this->height)){
        this->playPickupSound = true;
        switch(this->pickupId){
            case 0:
                if(playerHealth < playerMaxHealth){
                    playerHealth++;
                    this->active = false;
                }else{
                    this->playPickupSound = false;
                }
                break;

            case 1:
                if(playerMaxHealth < MAX_PLAYER_HEALTH){
                    playerMaxHealth++;
                    this->active = false;
                }
                if(playerHealth < playerMaxHealth){
                    playerHealth++;
                    this->active = false;
                }else{
                    this->playPickupSound = false;
                }
                break;

            case 2:
                if(playerShots[0] < MAX_PLAYER_SHOTS){
                    playerShots[0]++;
                    this->active = false;
                }else{
                    this->playPickupSound = false;
                }
                break;

            case 3:
                if(playerShots[1] < MAX_PLAYER_SHOTS){
                    playerShots[1]++;
                    this->active = false;
                }else{
                    this->playPickupSound = false;
                }
                break;

            case 4:
                if(playerShots[2] < MAX_PLAYER_SHOTS){
                    playerShots[2]++;
                    this->active = false;
                }else{
                    this->playPickupSound = false;
                }
                break;

            case 5:
                if(playerShots[3] < MAX_PLAYER_SHOTS){
                    playerShots[3]++;
                    this->active = false;
                }else{
                    this->playPickupSound = false;
                }
                break;

            case 6:
                if(playerShootingSpeed > MIN_PLAYER_SHOT_SPEED){
                    playerShootingSpeed -= 5;
                    this->active = false;
                }else{
                    this->playPickupSound = false;
                }
                break;
        }
    }

    if(isPassable(this->posX+deltaX, this->posY, 16, 16)){
        this->posX += deltaX;
    }else{
        this->active = false;
    }

    if(isPassable(this->posX, this->posY+deltaY, 16, 16)){
        this->posY += deltaY;
    }else{
        this->active = false;
    }
}
