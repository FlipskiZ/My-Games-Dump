#ifndef ACTOR_H
#define ACTOR_H


class Actor{
    public:
        Actor(void);
        void setAppearance(char diplayChar, short displayColor);
        void setPos(int y, int x);
        void setSpeed(int speed);
        void setActive(bool active);
        bool checkActive();
        void setHealth(int health);
        int checkHealth();
        void setMaxRange(int maxRange);
        void draw();
        void update();

        int posY;
        int posX;

    protected:
        char displayChar;
        short colorCode;
        int movementSpeed;
        int movementSpeedHelper;
        bool active;

    private:

};

#endif // ACTOR_H
