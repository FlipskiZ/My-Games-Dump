#ifndef PISTOLBULLET_H
#define PISTOLBULLET_H

#include "MissileEntity.h"

class PistolBullet : public MissileEntity{
    public:
        PistolBullet();
        void draw();
        void update();

    protected:
    private:
};

#endif // PISTOLBULLET_H
