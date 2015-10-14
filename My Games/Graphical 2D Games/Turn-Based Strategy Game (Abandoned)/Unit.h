#ifndef UNIT_H
#define UNIT_H


class Unit{
    public:
        Unit(void);
        void setPos(int x, int y);
        void setUnitId(int unitId);
        void setHealth(float health);
        void setMaxHealth(float maxHealth);
        void setMovementPoints(int movementPoints);
        void setActive(bool active);
        bool checkActive();
        void draw();
        void update();

        int posX, posY;

        int unitId;

        float health, maxHealth;
        int movementPoints;

    protected:

        bool active;

    private:

};

#endif // UNIT_H
