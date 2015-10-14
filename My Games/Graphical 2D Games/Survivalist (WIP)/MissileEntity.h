#ifndef MISSILEENTITY_H
#define MISSILEENTITY_H

#include "Entity.h"

class MissileEntity : public Entity{
    public:
        MissileEntity();
        void setAngle(float angle);
        void setMovementSpeed(float movementSpeed);
        void updateCenter();

        float angle, movementSpeed;
        float deltaX, deltaY;
        float centerX, centerY;

        bool colX, colY;

    protected:

    private:
};

#endif // MISSILEENTITY_H

