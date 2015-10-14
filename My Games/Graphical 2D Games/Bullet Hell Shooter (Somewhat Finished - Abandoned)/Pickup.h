#ifndef PICKUP_H
#define PICKUP_H


class Pickup{
    public:
        Pickup(void);
        void setPos(int x, int y);
        void setPickupId(int pickupId);
        void setActive(bool active);
        bool checkActive();
        void draw();
        void update();

        float posX, posY;
        float width, height;
        int pickupId;
        float moveSpeed;

        bool playPickupSound;

    protected:

        bool active;

    private:

};

#endif // PICKUP_H
