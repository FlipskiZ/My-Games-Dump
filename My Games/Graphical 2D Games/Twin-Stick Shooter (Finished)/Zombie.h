#ifndef ZOMBIE_H
#define ZOMBIE_H


class Zombie{
    public:
        Zombie(void);
        void setPointerListId(int pointerListId);
        void setPos(float x, float y);
        void setDimensions(float width, float height);
        void setZombieId(int zombieId);
        void setHealth(float health);
        void setMaxHealth(float maxHealth);
        void setSpeed(float moveSpeed);
        void setDamage(float damage);
        void setAttackTime(int attackTime);
        void setActive(bool active);
        bool checkActive();
        void draw();
        void update();

        int pointerListId;
        float posX, posY;
        float width, height;

        int zombieId;

        float health, maxHealth;

        float centerX, centerY;
        float moveSpeed, angle, damage;

        bool attacking, canMove;
        int attackTime, attackTimeHelper;

    protected:

        bool active;

    private:

};

#endif // ZOMBIE_H
