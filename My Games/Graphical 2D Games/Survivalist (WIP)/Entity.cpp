#include "Engine.h"
#include "Entity.h"

Entity::Entity(){
    this->posX = 0;
    this->posY = 0;
    this->width = 0;
    this->height = 0;
    this->angle = 0;
    this->entityId = 0;
    this->sheetColums = 1;
    this->sheetRows = 1;
    this->frameWidth = 0;
    this->frameHeight = 0;
    this->animationValue = 0;
    this->animationValueHelper = 0;
    this->sheetImage = 0;
    this->frameImage = 0;
    this->active = true;
}

void Entity::setPos(float x, float y){
    this->posX = x;
    this->posY = y;
}
void Entity::setDimensions(float width, float height){
    this->width = width;
    this->height = height;
}
void Entity::setEntityId(int entityId){
    this->entityId = entityId;
}
void Entity::setActive(bool active){
    this->active = active;
}
bool Entity::checkActive(){
    return this->active;
}
void Entity::setSheetDimensions(int sheetColums, int sheetRows, int frameWidth, int frameHeight){
    this->sheetColums = sheetColums;
    this->sheetRows = sheetRows;
    this->frameWidth = frameWidth;
    this->frameHeight = frameHeight;
}
void Entity::setBitmap(ALLEGRO_BITMAP *sheetImage){
    this->sheetImage = sheetImage;
    this->frameImage = al_create_sub_bitmap(sheetImage, this->frameX, this->frameY, this->frameWidth, this->frameHeight);
}
void Entity::setAnimationSpeed(int animationSpeed){
    this->animationCount = animationSpeed;
}
void Entity::updateAnimation(){
    if(this->animationValue != this->animationValueHelper){
        this->animationCountHelper = this->animationCount;
        this->animationValueHelper = this->animationValue;
    }

    if(this->animationValue <= 0){
        this->frameX = 0;
        this->frameY = 0;
        this->animationCountHelper = animationCount;
        this->frameImage = al_create_sub_bitmap(this->sheetImage, this->frameX, this->frameY, this->frameWidth, this->frameHeight);
        return;
    }

    if(this->animationCountHelper >= this->animationCount){
        this->frameY = (this->animationValue-1)*this->frameHeight;
        if(this->frameX+1 < (this->sheetColums-1)*this->frameWidth)
            this->frameX = this->frameX+this->frameWidth;
        else
            this->frameX = 0;

        this->frameImage = al_create_sub_bitmap(this->sheetImage, this->frameX, this->frameY, this->frameWidth, this->frameHeight);
        this->animationCountHelper = 0;
    }

    this->animationCountHelper++;
}
