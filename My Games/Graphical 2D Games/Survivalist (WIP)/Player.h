#ifndef PLAYER_H
#define PLAYER_H

#include "LivingEntity.h"

class Player : public LivingEntity{
    public:
        Player();
        void update();

        void fireBullet();

    protected:
    private:
};

#endif // PLAYER_H
