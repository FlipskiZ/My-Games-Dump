#ifndef BULLET_H
#define BULLET_H


class Bullet{
    public:
        Bullet(void);
        void setPos(int x, int y);
        void setDimensions(int width, int height);
        void setDamage(int damage);
        void setAngleSpeed(float moveSpeed, float angle);
        void setPlayerShot(bool playerShot);
        void setActive(bool active);
        bool checkActive();
        void draw();
        void update();

        float posX, posY;
        float width, height;

        int damage;
        float moveSpeed;
        float angle;
        bool playerShot;

        bool playHitSound;

    protected:

        bool active;

    private:

};

#endif // ENEMY_H
