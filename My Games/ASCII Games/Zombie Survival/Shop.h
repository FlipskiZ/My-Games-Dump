#ifndef SHOP_H
#define SHOP_H


class Shop
{
    public:
        Shop();
        void shopScreen();
    protected:
        int weapons[5][8][9];
        char weaponNames[5][8][30];
        int meleeCost;
        int upgradeCost[5][8];
        int bandageCost;
    private:
};

#endif // SHOP_H
