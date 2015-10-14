#ifndef SHOPSTATE_H
#define SHOPSTATE_H

#include "GameState.h"
#include "PlayState.h"

class ShopState : public GameState{
    public:
        void init();
        void cleanup();

        void pause();
        void resume();

        void update(Engine* engine);
        void draw(Engine* engine);

        static ShopState* instance() {
            return &m_ShopState;
        }

    protected:
        ShopState(){}

    private:
        static ShopState m_ShopState;
        float playerX = 0, playerY = 0;
};

#endif // SHOPSTATE_H
