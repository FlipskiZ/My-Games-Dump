#include "Engine.h"
#include "PistolBullet.h"

PistolBullet::PistolBullet()
{
    this->animationValue = 1;
}

void PistolBullet::draw(){
    al_draw_rotated_bitmap(this->frameImage, this->width/2, this->height/2, this->posX+this->width/2+cameraOffsetX, this->posY+this->height/2+cameraOffsetY, this->angle, NULL);
}
void PistolBullet::update(){
    this->colX = false, this->colY = false;

    this->deltaX = sin(this->angle) * this->movementSpeed;
    this->deltaY = -cos(this->angle) * this->movementSpeed;

    float loopI = ceil(this->movementSpeed/this->width);

    for(float i = 0; i < loopI && !colX; i+=0.25){
        if(!isPassable(this->posX + this->width*i*(deltaX/loopI/this->width), this->posY + this->height*i*(deltaY/loopI/this->height), this->width, this->height) && !this->colX){
            this->colX = true;
        }
        for(int lI = 0; lI < MAX_LIVING && !colX; lI++){
            if(livingList[lI] != NULL && livingList[lI]->checkActive()){
                if(checkCollision(this->posX + this->width*i*(deltaX/loopI/this->width), this->posY + this->height*i*(deltaY/loopI/this->height), livingList[lI]->posX, livingList[lI]->posY,
                    this->width, this->height, livingList[lI]->width, livingList[lI]->height)){

                    this->colX = true;
                    printf("\nHit LivingEntity");
                    //Function to hurt LivingEntity
                }
            }
        }
        if(colX){
            DebugClass *purplePixel = new DebugClass();
            purplePixel->setPos(this->posX + this->width*i*(deltaX/loopI/this->width), this->posY + this->height*i*(deltaY/loopI/this->height));
            purplePixel->setDimensions(1, 1);
            purplePixel->setSheetDimensions(1, 1, 1, 1);
            purplePixel->setAnimationSpeed(0);
            purplePixel->setBitmap(pixelImage);
            addDebugToList(purplePixel);
        }
    }
    if(!colX){
        this->posX += this->deltaX;
        this->posY += this->deltaY;
    }else{
        this->active = false;
    }

    this->updateCenter();

    this->updateAnimation();

    this->deltaX = 0;
    this->deltaY = 0;
}
