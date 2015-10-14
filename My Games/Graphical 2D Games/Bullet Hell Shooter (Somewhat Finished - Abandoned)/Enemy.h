#ifndef ENEMY_H
#define ENEMY_H


class Enemy{
    public:
        Enemy(void);
        void setPos(float x, float y);
        void setDimensions(float width, float height);
        void setEnemyId(int enemyId);
        void setHealth(float health);
        void setMaxHealth(float maxHealth);
        void setShotAmount(int shotAmount);
        void setAngleSpeed(float moveSpeed, float angle);
        void setShootingSpeed(float shootingSpeed);
        void setActive(bool active);
        bool checkActive();
        void draw();
        void update();

        float posX, posY;
        float width, height;

        int enemyId;

        float health, maxHealth;
        int shotAmount;

        float centerX, centerY;
        float shootingSpeed, shootingSpeedHelper;
        float moveSpeed, angle;

        bool playDeathSound;

    protected:

        bool active;

    private:

};

#endif // ENEMY_H
