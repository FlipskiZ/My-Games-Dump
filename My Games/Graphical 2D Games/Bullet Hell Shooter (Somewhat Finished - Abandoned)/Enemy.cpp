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


Enemy::Enemy(){

	this->posX = 0;
	this->posY = 0;
	this->width = 0;
	this->height = 0;
    this->enemyId = 0;
	this->centerX = 0;
	this->centerY = 0;
	this->health = 0;
	this->maxHealth = 0;
	this->shotAmount = 0;
	this->moveSpeed = 0;
	this->angle = 0;
    this->shootingSpeed = 0;
    this->shootingSpeedHelper = 0;
    this->playDeathSound = false;
	this->active = true;
}

void Enemy::setPos(float x, float y){
    this->posX = x;
    this->posY = y;
}

void Enemy::setDimensions(float width, float height){
    this->width = width;
    this->height = height;
}

void Enemy::setEnemyId(int enemyId){
    this->enemyId = enemyId;
}

void Enemy::setHealth(float health){
    this->health = health;
}

void Enemy::setMaxHealth(float maxHealth){
    this->maxHealth = maxHealth;
}

void Enemy::setShotAmount(int shotAmount){
    this->shotAmount = shotAmount;
}

void Enemy::setAngleSpeed(float moveSpeed, float angle){
    this->moveSpeed = moveSpeed;
    this->angle = angle;
}

void Enemy::setShootingSpeed(float shootingSpeed){
    this->shootingSpeed = shootingSpeed;
}

void Enemy::setActive(bool active){
    this->active = active;
}

bool Enemy::checkActive(){
    return this->active;
}

void Enemy::draw(){
    if(this->enemyId == 0){
        al_draw_filled_rectangle(this->posX, this->posY, this->posX + this->width, this->posY + this->height, al_map_rgb(255, 50, 255));
    }
    if(this->enemyId == 1){
        al_draw_filled_rectangle(this->posX, this->posY, this->posX + this->width, this->posY + this->height, al_map_rgb(220, 122, 122));
    }

    al_draw_rectangle(this->posX + this->width/2 - this->maxHealth*2, this->posY - 20, this->posX + this->width/2 + this->maxHealth*2, this->posY - 10, al_map_rgb(215, 70, 70), 2);
    al_draw_filled_rectangle(this->posX + this->width/2 - this->health*2, this->posY - 20, this->posX + this->width/2 + this->health*2, this->posY - 10, al_map_rgb(215, 70, 70));
}

void Enemy::update(){
    this->playDeathSound = false;
    if(this->health <= 0){
        this->active = false;
        this->playDeathSound = true;
        playerScore++;
        if(rand() % 15 == 0){
            int pickupId = 0;
            float randomPercent = rand() % 1000 / 10;

            if(randomPercent <= 30)
                pickupId = 0;
            else if(randomPercent > 30 && randomPercent <= 40)
                pickupId = 1;
            else if(randomPercent > 40 && randomPercent <= 52.5)
                pickupId = 2;
            else if(randomPercent > 52.5 && randomPercent <= 65)
                pickupId = 3;
            else if(randomPercent > 65 && randomPercent <= 77.5)
                pickupId = 4;
            else if(randomPercent > 77.5 && randomPercent <= 90)
                pickupId = 5;
            else
                pickupId = 6;


            Pickup *newPickup = new Pickup();

            newPickup->setPos(this->posX + this->width/2 - 8, this->posY + this->height/2 - 8);
            newPickup->setPickupId(pickupId);
            addPickupToList(newPickup);
        }
        return;
    }

    float deltaY, deltaX;

    if(this->enemyId == 0){
        if(this->shootingSpeedHelper >= this->shootingSpeed){
            int x, y, width, height;
            float angle;
            width = 10;
            height = 10;
            x = this->centerX - width/2;
            y = this->centerY - height/2;

            for(int i = 0; i < this->shotAmount; i++){
                if(this->shotAmount != 1){
                    angle = 0.375+(0.25/(this->shotAmount - 1))*i;
                }else{
                    angle = 0.5;
                }
                Bullet *newBullet = new Bullet();
                newBullet->setPos(x, y);
                newBullet->setDimensions(width, height);
                newBullet->setDamage(1);
                newBullet->setAngleSpeed(2, angle);
                newBullet->setPlayerShot(false);

                addBulletToList(newBullet);
            }

            this->shootingSpeedHelper = 0;
        }
        deltaX = sin(this->angle * 6.28318530718/* PI*2 */) * this->moveSpeed;
        deltaY = -cos(this->angle * 6.28318530718/* PI*2 */) * this->moveSpeed;
    }else if(this->enemyId == 1){
        if(checkCollision(this->posX, this->posY, playerCenterX - playerHitBoxWidth/2, playerCenterY - playerHitBoxHeight/2, this->width, this->height, playerHitBoxWidth, playerHitBoxHeight)){
            playerHealth -= 1;
            this->active = false;
        }else{
            deltaX = sin(this->angle) * this->moveSpeed;
            deltaY = -cos(this->angle) * this->moveSpeed;
        }
    }

    if(isPassable(this->posX+deltaX, this->posY, this->width, this->height)){
        this->posX += deltaX;
    }else{
        this->active = false;
    }

    if(isPassable(this->posX, this->posY+deltaY, this->width, this->height)){
        this->posY += deltaY;
    }else{
        this->active = false;
    }

    this->centerX = this->posX + this->width/2;
    this->centerY = this->posY + this->height/2;

    this->shootingSpeedHelper++;
}
