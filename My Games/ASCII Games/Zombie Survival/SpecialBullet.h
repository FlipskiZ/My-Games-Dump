#ifndef SPECIALBULLET_H
#define SPECIALBULLET_H


class SpecialBullet{
    public:
        SpecialBullet(void);
        void setAppearance(char diplayChar, short displayColor);
        void setPos(int y, int x);
        void setDamage(int damage);
        void setSpeed(int speed);
        void setRange(int range);
        void setDirection(int direction);
        void setActive(bool active);
        bool checkActive();
        void setBulletType(bool minigun, bool rocket, bool rocketExplosion, bool granade, bool frag, bool random, bool mine, bool granadeGranade, bool granadeGranadeGranade, bool potato, bool piercing);
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

        bool minigun;
        int weaponSprayDirection;

        bool rocket;
        bool rocketExplosion;

        bool granade;
        bool granadeTravel;
        bool frag;

        bool random;

        bool mine;

        bool granadeGranade;

        bool granadeGranadeGranade;

        bool potato;

        bool piercing;

    private:

};

#endif // SPECIALBULLET_H
