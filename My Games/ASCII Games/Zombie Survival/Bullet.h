#ifndef BULLET_H
#define BULLET_H


class Bullet{
    public:
        Bullet(void);
        void setAppearance(char diplayChar, short displayColor);
        void setPos(int y, int x);
        void setDamage(int damage);
        void setSpeed(int speed);
        void setRange(int range);
        void setDirection(int direction);
        void setActive(bool active);
        bool checkActive();
        void draw();
        void update();

        int posY;
        int posX;

    protected:
        int damage;
        int range;
        int rangeHelper;
        char displayChar;
        short colorCode;
        int movementSpeed;
        int movementSpeedHelper;
        int direction;
        bool active;

    private:

};

#endif // BULLET_H
