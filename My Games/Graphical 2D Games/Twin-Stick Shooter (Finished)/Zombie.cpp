#include "Engine.h"

Zombie::Zombie(){
    this->pointerListId = 0;
	this->posX = 0;
	this->posY = 0;
	this->width = 0;
	this->height = 0;
	this->zombieId = 0;
	this->centerX = 0;
	this->centerY = 0;
	this->health = 0;
	this->maxHealth = 0;
	this->moveSpeed = 0;
	this->angle = 0;
	this->damage = 0;
	this->attackTime = 0;
	this->attackTimeHelper = 0;
	this->attacking = false;
	this->canMove = true;
	this->active = true;
}

void Zombie::setPointerListId(int pointerListId){
    this->pointerListId = pointerListId;
}

void Zombie::setPos(float x, float y){
    this->posX = x;
    this->posY = y;
}

void Zombie::setDimensions(float width, float height){
    this->width = width;
    this->height = height;
}

void Zombie::setZombieId(int zombieId){
    this->zombieId = zombieId;
}

void Zombie::setHealth(float health){
    this->health = health;
}

void Zombie::setMaxHealth(float maxHealth){
    this->maxHealth = maxHealth;
}

void Zombie::setSpeed(float moveSpeed){
    this->moveSpeed = moveSpeed;
}

void Zombie::setDamage(float damage){
    this->damage = damage;
}

void Zombie::setAttackTime(int attackTime){
    this->attackTime = attackTime;
}

void Zombie::setActive(bool active){
    this->active = active;
}

bool Zombie::checkActive(){
    return this->active;
}

void Zombie::draw(){
    al_draw_rotated_bitmap(zombieImage, this->width/2, this->height/2, this->posX+width/2, this->posY+height/2, this->angle, NULL);

    al_draw_rectangle(this->posX + this->width/2 - 20 - 1, this->posY - 12.5 - 1, this->posX + this->width/2 + 20 + 1, this->posY - 7.5 + 1, al_map_rgb(195, 0, 0), 2);
    al_draw_filled_rectangle(this->posX + this->width/2 -((20/this->maxHealth)*this->health), this->posY - 12.5, this->posX + this->width/2 + ((20/this->maxHealth)*this->health), this->posY - 7.5, al_map_rgb(255, 50, 50));

    if(this->attacking){
        al_draw_bitmap(attackImage, this->posX, this->posY, NULL);
    }
}

void Zombie::update(){
    if(this->health <= 0){
        playerMoney += enemyKillMoney;
        this->active = false;
    }

    bool colX = false, colY = false;
    float deltaY, deltaX;

    angle = -atan2(this->centerX - playerCenterX, this->centerY - playerCenterY);

    deltaX = sin(this->angle) * this->moveSpeed;
    deltaY = -cos(this->angle) * this->moveSpeed;

    for(int i = 0; i < MAX_ZOMBIES; i++){
        if(zombieList[i] != NULL && zombieList[i]->checkActive()){
            if(i != this->pointerListId){
                if(checkCollision(this->posX+deltaX, this->posY, zombieList[i]->posX, zombieList[i]->posY, this->width, this->height, zombieList[i]->width, zombieList[i]->height)){
                    colX = true;
                }
                if(checkCollision(this->posX, this->posY+deltaY, zombieList[i]->posX, zombieList[i]->posY, this->width, this->height, zombieList[i]->width, zombieList[i]->height)){
                    colY = true;
                }
            }
        }
    }

    if(checkCollision(this->posX+deltaX, this->posY, playerX, playerY, this->width, this->height, playerWidth, playerHeight)){
        colX = true;
        this->attacking = true;
    }
    if(checkCollision(this->posX, this->posY+deltaY, playerX, playerY, this->width, this->height, playerWidth, playerHeight)){
        colY = true;
        this->attacking = true;
    }

    if(isPassable(this->posX+deltaX, this->posY, this->width, this->height, deltaX, deltaY)){
        if(!colX && this->canMove)
            this->posX += deltaX;
    }

    if(isPassable(this->posX, this->posY+deltaY, this->width, this->height, deltaX, deltaY)){
       if(!colY && this->canMove)
            this->posY += deltaY;
    }

    if(this->attacking){
        this->attackTimeHelper++;
        this->canMove = false;
        if(attackTimeHelper >= attackTime){
            this->attacking = false;
            this->canMove = true;
            this->attackTimeHelper = 0;
            if(checkCollision(this->posX+deltaX, this->posY+deltaY, playerX, playerY, this->width, this->height, playerWidth, playerHeight)){
                playerHealth -= this->damage;
            }
        }
    }

    this->centerX = this->posX + this->width/2;
    this->centerY = this->posY + this->height/2;
}
