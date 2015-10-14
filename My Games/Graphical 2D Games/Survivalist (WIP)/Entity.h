#ifndef ENTITY_H
#define ENTITY_H

#include "Engine.h"

class Entity{
    public:
        Entity();
        void setPos(float x, float y);
        void setDimensions(float width, float height);
        void setEntityId(int entityId);
        void setActive(bool active);
        bool checkActive();
        void setSheetDimensions(int sheetColums, int sheetRows, int frameWidth, int frameHeight);
        void setAnimationSpeed(int animationSpeed);
        void setBitmap(ALLEGRO_BITMAP *sheetImage);
        void updateAnimation();
        virtual void draw() = 0;
        virtual void update() = 0;

        float posX, posY;
        float width, height;
        float angle;

        int entityId;

        int sheetColums, sheetRows, frameX, frameY, frameWidth, frameHeight, animationValue, animationValueHelper, animationCount, animationCountHelper;
        ALLEGRO_BITMAP *sheetImage, *frameImage;

    protected:

        bool active;

    private:

};

#endif // ENTITY_H
