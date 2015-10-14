#ifndef LIVINGENTITY_H
#define LIVINGENTITY_H

#include "Entity.h"

class LivingEntity : public Entity{
    public:
        LivingEntity();
        void setAngle(float angle);
        void setMovementSpeed(float movementSpeed);
        void updateCenter();
        void draw();

        float angle, movementSpeed;
        float deltaX, deltaY;
        float centerX, centerY;

        bool colX, colY;

    protected:

    private:
};

#endif // LIVINGENTITY_H
