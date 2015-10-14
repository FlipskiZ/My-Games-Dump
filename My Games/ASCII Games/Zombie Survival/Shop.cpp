#include "Engine.h"
#include "Actor.h"
#include "Bullet.h"
#include "Shop.h"
#include "SkillTree.h"
#include "SpecialBullet.h"
#include <cstdlib>
#include <ctime>
#include <curses.h>

Shop::Shop()
{
    weapons = {
        {{10,30,1,20,500,10,4000,1,0},{50,10,5,30,750,3,3500,0,0},{130,0,1,40,10,50,3500,0,0},{180,5,10,40,500,2,1000,0,0},{285,10,4,30,50,30,4000,0,0},{475,8,4,32,45,35,3750,0,0},{550,7,5,35,40,40,3500,0,0},{650,6,5,40,30,50,3000,0,0}},
        {{25,25,1,20,450,15,3500,0,0},{75,20,4,5,500,5,3000,0,0},{165,50,10,40,1500,1,2000,0,0},{230,10,4,30,100,50,1500,0,0},{325,10,5,30,40,20,4000,0,0},{550,5,10,40,50,30,3000,0,0},{650,50,1,15,0,500,11000,0,0},{1500,5,10,40,10,1000,500,0,0}},
        {{45,25,1,30,400,15,2500,0,0},{100,20,1,25,200,25,4000,0,0},{220,15,3,25,75,25,4500,0,0},{280,0,2,40,5,100,4500,0,0},{370,20,3,30,35,40,3000,0,0},{800,25,10,20,500,5,3500,0,6},{1000,45,10,15,125,40,4000,0,3},{2000,5,10,5,0,50,3500,0,1}},
        {{55,20,1,25,350,15,2000,0,0},{120,15,2,30,150,30,3500,0,0},{255,15,3,25,65,25,4500,0,0},{310,5,5,8,250,15,3500,0,0},{420,0,3,40,2,100,4500,0,0},{800,15,3,40,10,200,7500,0,1},{1000,0,5,40,0,200,8000,0,1},{2500,25,4,40,300,5,3000,0,9}},
        {{70,15,2,40,50,40,2000,0,1},{180,35,10,25,1500,3,5000,0,2},{320,25,10,20,400,10,5000,0,3},{400,0,10,150,0,10,2000,0,5},{500,10,5,40,200,1,3000,0,4},{1400,15,10,25,100,35,4000,0,2},{2000,25,10,20,1000,3,2500,0,7},{5000,100,10,40,0,1,5000,0,8}},
    };

    weaponNames = {
        {{'P','i','s','t','o','l'}           ,{'M','i','n','i','-','S','n','i','p','e','r'}                 ,{'M','i','n','i','-','L','a','s','e','r'}                          ,{'S','n','i','p','e','r'}              ,{'M','G','-','5','4','0'}                                                                          ,{'M','G','-','5','4','5'}                                                                          ,{'M','G','-','5','5','0'}                                                                          ,{'M','G','-','5','9','9'}},
        {{'P','i','s','t','o','l','+','1'}   ,{'M','i','n','i','-','S','h','o','t','g','u','n'}             ,{'C','a','n','n','o','n'}                                          ,{'P','i','s','t','o','l','+','1','0'}  ,{'M','a','x','i','-','U','z','i'}                                                                  ,{'S','e','m','i','-','S','n','i','p','e','r'}                                                      ,{'W','a','t','e','r',' ','P','i','s','t','o','l'}                                                  ,{'B','u','l','l','e','t',' ','X'}},
        {{'P','i','s','t','o','l','+','2'}   ,{'S','e','m','i','-','P','i','s','t','o','l'}                 ,{'M','G','-','5','2','9'}                                          ,{'L','a','s','e','r'}                  ,{'S','u','p','e','r','-','M','M','G'}                                                              ,{'G','r','a','n','a','d','e',' ','G','r','a','n','a','d','e',' ','L','a','u','n','c','h','e','r'}  ,{'G','r','a','n','a','d','e',' ','M','G'}                                                          ,{'B','u','l','l','e','t','t','h','r','o','w','e','r'}},
        {{'P','i','s','t','o','l','+','3'}   ,{'M','i','n','i','-','U','z','i'}                             ,{'M','G','-','5','3','0'}                                          ,{'S','h','o','t','g','u','n'}          ,{'B','i','g',' ','L','a','s','e','r'}                                                              ,{'M','a','x','i','g','u','n'}                                                                      ,{'I','M','A',' ','F','I','R','I','N',' ','M','A','H',' ','L','A','Z','O','R'}                      ,{'P','i','e','r','c','i','n','g',' ','G','u','n'}},
        {{'M','i','n','i','g','u','n'}       ,{'R','o','c','k','e','t',' ','L','a','u','n','c','h','e','r'} ,{'G','r','a','n','a','d','e',' ','L','a','u','n','c','h','e','r'}  ,{'M','i','n','e','s'}                  ,{'G','l','i','t','c','h','y',' ','G',' ','o','f',' ','G','l','i','t','c','h','i','n','e','s','s'}  ,{'R','o','c','k','e','t',' ','M','G'}                                                              ,{'G','r','a','.',' ','G','r','a','.',' ','G','r','a','.',' ','L','a','u','n','c','h','e','r'}      ,{'T','H','E',' ','P','O','T','A','T','O'}},
    };

    upgradeCost = {
        {50, 50, 50, 50, 50, 50, 50, 50},
        {50, 50, 50, 50, 50, 50, 50, 50},
        {50, 50, 50, 50, 50, 50, 50, 50},
        {50, 50, 50, 50, 50, 50, 50, 50},
        {50, 50, 50, 50, 50, 50, 50, 50},
    };

    for(int y = 0; y < 5; y++){
        for(int x = 0; x < 8; x++){
            upgradeCost[y][x] = weapons[y][x][0]/5;
            if(x == 0){
                upgradeCost[y][x] = 10;
            }else if(x == 1){
                upgradeCost[y][x] = 20;
            }
        }
    }

    meleeCost = 50;

    bandageCost = 5;
}

void Shop::shopScreen(){
    resize_term(30, 180);

    bool done = false;
    int y = 0;
    int x = 0;
    int weaponTier = 0;
    int weaponSelection = -1;

    while(!done){
        clear();

        mvprintw(0,1,"Welcome to the shop! Press q, esc or select the back button to exit the shop.");
        mvprintw(2,1,"Money: %d", playerMoney);
        mvprintw(4,1,"Back");
        mvprintw(5,1,"+10 Ammo: 1$ - Current: %d", playerAmmo);
        mvprintw(6,1,"+1 Bandage: %d$ - Current: %d", bandageCost, playerBandages);
        switch(playerMeleeDamage){
            case 0:
                mvprintw(7,1,"Tired of shooting? And/or the lack of ammunition? Buy our newest invention! THE DAGGER! - 50$");
                break;

            case 1:
                mvprintw(7,1,"The dagger is too weak for you? We got just what you need! Buy this finely crafted kitchen knife and continue you ammunition saving! - 100$");
                break;

            case 2:
                mvprintw(7,1,"Tired of chopping up zombies like vegetables with your finely crafted kitchen knife? This weapon is taken straight from our time machine! THE SWORD! - 200$");
                break;

            case 3:
                mvprintw(7,1,"Too modern for the good ol' sword? Buy this super advanced weapon taken straight from our lab! THE LASER DAGGER! - 400$");
                break;

            case 4:
                mvprintw(7,1,"The laser dagger is too short for you? Well.. Guess what? We took a visit to a galaxy far, far away... - 800$");
                break;

            case 5:
                mvprintw(7,1,"Not happy with your awesome lazor sword? Well, too bad.");
                break;
        }
        mvprintw(8,1,"+1 Stamina Pill: 5$ - Current: %d", playerStaminaPills);
        int temp1 = playerBulletSpeed/1.2;
        int temp2 = playerBulletRange*1.2;
        int temp3 = playerFiringSpeed/1.2;
        int temp4 = playerClipLimit*1.2;
        int temp5 = playerReloadingRate/1.2;
        mvprintw(9,1,"Upgrade your gun's bullet speed - Current: %d - If upgraded: %d - Upgrade cost: %d", playerBulletSpeed, temp1, upgradeCost[playerCurrentWeapon[0]][playerCurrentWeapon[1]]);
        mvprintw(10,1,"Upgrade your gun's attack damage - Current: %d - If upgraded: %d - Upgrade cost: %d", playerDamage, playerDamage+1, upgradeCost[playerCurrentWeapon[0]][playerCurrentWeapon[1]]);
        mvprintw(11,1,"Upgrade your gun's bullet range - Current: %d - If upgraded: %d - Upgrade cost: %d", playerBulletRange, temp2, upgradeCost[playerCurrentWeapon[0]][playerCurrentWeapon[1]]);
        mvprintw(12,1,"Upgrade your gun's fire rate - Current: %d - If upgraded: %d - Upgrade cost: %d", playerFiringSpeed, temp3, upgradeCost[playerCurrentWeapon[0]][playerCurrentWeapon[1]]);
        mvprintw(13,1,"Upgrade your gun's clip capacity - Current: %d - If upgraded: %d - Upgrade cost: %d", playerClipLimit, temp4, upgradeCost[playerCurrentWeapon[0]][playerCurrentWeapon[1]]);
        mvprintw(14,1,"Upgrade your gun's reload speed - Current: %d - If upgraded: %d - Upgrade cost: %d", playerReloadingRate, temp5, upgradeCost[playerCurrentWeapon[0]][playerCurrentWeapon[1]]);
        mvprintw(16,1,"Tier 1");
        mvprintw(16,11,"Tier 2");
        mvprintw(16,21,"Tier 3");
        mvprintw(16,31,"Tier 4");
        mvprintw(16,41,"Tier 5");
        if(lastTiersUnlocked){
            mvprintw(16,51,"Tier 6");
            mvprintw(16,61,"Tier 7");
            mvprintw(16,71,"Tier 8");
        }

        switch(weaponTier){
            case 1:
                attron(COLOR_PAIR(6));
                mvprintw(16,1,"Tier 1");
                attroff(COLOR_PAIR(6));
                break;

            case 2:
                attron(COLOR_PAIR(6));
                mvprintw(16,11,"Tier 2");
                attroff(COLOR_PAIR(6));
                break;

            case 3:
                attron(COLOR_PAIR(6));
                mvprintw(16,21,"Tier 3");
                attroff(COLOR_PAIR(6));
                break;

            case 4:
                attron(COLOR_PAIR(6));
                mvprintw(16,31,"Tier 4");
                attroff(COLOR_PAIR(6));
                break;

            case 5:
                attron(COLOR_PAIR(6));
                mvprintw(16,41,"Tier 5");
                attroff(COLOR_PAIR(6));
                break;

            case 6:
                attron(COLOR_PAIR(6));
                mvprintw(16,51,"Tier 6");
                attroff(COLOR_PAIR(6));
                break;

            case 7:
                attron(COLOR_PAIR(6));
                mvprintw(16,61,"Tier 7");
                attroff(COLOR_PAIR(6));
                break;

            case 8:
                attron(COLOR_PAIR(6));
                mvprintw(16,71,"Tier 8");
                attroff(COLOR_PAIR(6));
                break;
        }

        if(weaponTier != 0){
            mvprintw(18,1,"Current Weapon: %s - Weapon Worth: %d - Bullet Speed: %d - Bullet Damage: %d - Bullet Range: %d - Weapon Fire Rate: %d - Weapon Max Clip: %d - Weapon Reload Rate: %d", weaponNames[0][0], weapons[playerCurrentWeapon[0]][playerCurrentWeapon[1]][0]/2, playerBulletSpeed, playerDamage, playerBulletRange, playerFiringSpeed, playerClipLimit, playerReloadingRate);

            if(weapons[0][weaponTier-1][7] == 1){
                attron(COLOR_PAIR(6));
                mvprintw(20,1,"%s - Weapon Cost: %d - Bullet Speed: %d - Bullet Damage: %d - Bullet Range: %d - Weapon Fire Rate: %d - Weapon Max Clip: %d - Weapon Reload Rate: %d", weaponNames[0][weaponTier-1], weapons[0][weaponTier-1][0], weapons[0][weaponTier-1][1], weapons[0][weaponTier-1][2], weapons[0][weaponTier-1][3], weapons[0][weaponTier-1][4], weapons[0][weaponTier-1][5], weapons[0][weaponTier-1][6] + weapons[0][weaponTier-1][6]/100*playerReloadingModifier);
                attroff(COLOR_PAIR(6));
            }else if(playerMoney >= weapons[0][weaponTier-1][0]-weapons[playerCurrentWeapon[0]][playerCurrentWeapon[1]][0]/2){
                attron(COLOR_PAIR(7));
                mvprintw(20,1,"%s - Weapon Cost: %d - Bullet Speed: %d - Bullet Damage: %d - Bullet Range: %d - Weapon Fire Rate: %d - Weapon Max Clip: %d - Weapon Reload Rate: %d", weaponNames[0][weaponTier-1], weapons[0][weaponTier-1][0]-weapons[playerCurrentWeapon[0]][playerCurrentWeapon[1]][0]/2, weapons[0][weaponTier-1][1], weapons[0][weaponTier-1][2], weapons[0][weaponTier-1][3], weapons[0][weaponTier-1][4], weapons[0][weaponTier-1][5], weapons[0][weaponTier-1][6] + weapons[0][weaponTier-1][6]/100*playerReloadingModifier);
                attroff(COLOR_PAIR(7));
            }else{
                attron(COLOR_PAIR(5));
                mvprintw(20,1,"%s - Weapon Cost: %d - Bullet Speed: %d - Bullet Damage: %d - Bullet Range: %d - Weapon Fire Rate: %d - Weapon Max Clip: %d - Weapon Reload Rate: %d", weaponNames[0][weaponTier-1], weapons[0][weaponTier-1][0]-weapons[playerCurrentWeapon[0]][playerCurrentWeapon[1]][0]/2, weapons[0][weaponTier-1][1], weapons[0][weaponTier-1][2], weapons[0][weaponTier-1][3], weapons[0][weaponTier-1][4], weapons[0][weaponTier-1][5], weapons[0][weaponTier-1][6] + weapons[0][weaponTier-1][6]/100*playerReloadingModifier);
                attroff(COLOR_PAIR(5));
            }

            if(weapons[1][weaponTier-1][7] == 1){
                attron(COLOR_PAIR(6));
                mvprintw(22,1,"%s - Weapon Cost: %d - Bullet Speed: %d - Bullet Damage: %d - Bullet Range: %d - Weapon Fire Rate: %d - Weapon Max Clip: %d - Weapon Reload Rate: %d", weaponNames[1][weaponTier-1], weapons[1][weaponTier-1][0], weapons[1][weaponTier-1][1], weapons[1][weaponTier-1][2], weapons[1][weaponTier-1][3], weapons[1][weaponTier-1][4], weapons[1][weaponTier-1][5], weapons[1][weaponTier-1][6] + weapons[1][weaponTier-1][6]/100*playerReloadingModifier);
                attroff(COLOR_PAIR(6));
            }else if(playerMoney >= weapons[1][weaponTier-1][0]-weapons[playerCurrentWeapon[0]][playerCurrentWeapon[1]][0]/2){
                attron(COLOR_PAIR(7));
                mvprintw(22,1,"%s - Weapon Cost: %d - Bullet Speed: %d - Bullet Damage: %d - Bullet Range: %d - Weapon Fire Rate: %d - Weapon Max Clip: %d - Weapon Reload Rate: %d", weaponNames[1][weaponTier-1], weapons[1][weaponTier-1][0]-weapons[playerCurrentWeapon[0]][playerCurrentWeapon[1]][0]/2, weapons[1][weaponTier-1][1], weapons[1][weaponTier-1][2], weapons[1][weaponTier-1][3], weapons[1][weaponTier-1][4], weapons[1][weaponTier-1][5], weapons[1][weaponTier-1][6] + weapons[1][weaponTier-1][6]/100*playerReloadingModifier);
                attroff(COLOR_PAIR(7));
            }else{
                attron(COLOR_PAIR(5));
                mvprintw(22,1,"%s - Weapon Cost: %d - Bullet Speed: %d - Bullet Damage: %d - Bullet Range: %d - Weapon Fire Rate: %d - Weapon Max Clip: %d - Weapon Reload Rate: %d", weaponNames[1][weaponTier-1], weapons[1][weaponTier-1][0]-weapons[playerCurrentWeapon[0]][playerCurrentWeapon[1]][0]/2, weapons[1][weaponTier-1][1], weapons[1][weaponTier-1][2], weapons[1][weaponTier-1][3], weapons[1][weaponTier-1][4], weapons[1][weaponTier-1][5], weapons[1][weaponTier-1][6] + weapons[1][weaponTier-1][6]/100*playerReloadingModifier);
                attroff(COLOR_PAIR(5));
            }

            if(weapons[2][weaponTier-1][7] == 1){
                attron(COLOR_PAIR(6));
                mvprintw(24,1,"%s - Weapon Cost: %d - Bullet Speed: %d - Bullet Damage: %d - Bullet Range: %d - Weapon Fire Rate: %d - Weapon Max Clip: %d - Weapon Reload Rate: %d", weaponNames[2][weaponTier-1], weapons[2][weaponTier-1][0], weapons[2][weaponTier-1][1], weapons[2][weaponTier-1][2], weapons[2][weaponTier-1][3], weapons[2][weaponTier-1][4], weapons[2][weaponTier-1][5], weapons[2][weaponTier-1][6] + weapons[2][weaponTier-1][6]/100*playerReloadingModifier);
                attroff(COLOR_PAIR(6));
            }else if(playerMoney >= weapons[2][weaponTier-1][0]-weapons[playerCurrentWeapon[0]][playerCurrentWeapon[1]][0]/2){
                attron(COLOR_PAIR(7));
                mvprintw(24,1,"%s - Weapon Cost: %d - Bullet Speed: %d - Bullet Damage: %d - Bullet Range: %d - Weapon Fire Rate: %d - Weapon Max Clip: %d - Weapon Reload Rate: %d", weaponNames[2][weaponTier-1], weapons[2][weaponTier-1][0]-weapons[playerCurrentWeapon[0]][playerCurrentWeapon[1]][0]/2, weapons[2][weaponTier-1][1], weapons[2][weaponTier-1][2], weapons[2][weaponTier-1][3], weapons[2][weaponTier-1][4], weapons[2][weaponTier-1][5], weapons[2][weaponTier-1][6] + weapons[2][weaponTier-1][6]/100*playerReloadingModifier);
                attroff(COLOR_PAIR(7));
            }else{
                attron(COLOR_PAIR(5));
                mvprintw(24,1,"%s - Weapon Cost: %d - Bullet Speed: %d - Bullet Damage: %d - Bullet Range: %d - Weapon Fire Rate: %d - Weapon Max Clip: %d - Weapon Reload Rate: %d", weaponNames[2][weaponTier-1], weapons[2][weaponTier-1][0]-weapons[playerCurrentWeapon[0]][playerCurrentWeapon[1]][0]/2, weapons[2][weaponTier-1][1], weapons[2][weaponTier-1][2], weapons[2][weaponTier-1][3], weapons[2][weaponTier-1][4], weapons[2][weaponTier-1][5], weapons[2][weaponTier-1][6] + weapons[2][weaponTier-1][6]/100*playerReloadingModifier);
                attroff(COLOR_PAIR(5));
            }

            if(weapons[3][weaponTier-1][7] == 1){
                attron(COLOR_PAIR(6));
                mvprintw(26,1,"%s - Weapon Cost: %d - Bullet Speed: %d - Bullet Damage: %d - Bullet Range: %d - Weapon Fire Rate: %d - Weapon Max Clip: %d - Weapon Reload Rate: %d", weaponNames[3][weaponTier-1], weapons[3][weaponTier-1][0], weapons[3][weaponTier-1][1], weapons[3][weaponTier-1][2], weapons[3][weaponTier-1][3], weapons[3][weaponTier-1][4], weapons[3][weaponTier-1][5], weapons[3][weaponTier-1][6] + weapons[3][weaponTier-1][6]/100*playerReloadingModifier);
                attroff(COLOR_PAIR(6));
            }else if(playerMoney >= weapons[3][weaponTier-1][0]-weapons[playerCurrentWeapon[0]][playerCurrentWeapon[1]][0]/2){
                attron(COLOR_PAIR(7));
                mvprintw(26,1,"%s - Weapon Cost: %d - Bullet Speed: %d - Bullet Damage: %d - Bullet Range: %d - Weapon Fire Rate: %d - Weapon Max Clip: %d - Weapon Reload Rate: %d", weaponNames[3][weaponTier-1], weapons[3][weaponTier-1][0]-weapons[playerCurrentWeapon[0]][playerCurrentWeapon[1]][0]/2, weapons[3][weaponTier-1][1], weapons[3][weaponTier-1][2], weapons[3][weaponTier-1][3], weapons[3][weaponTier-1][4], weapons[3][weaponTier-1][5], weapons[3][weaponTier-1][6] + weapons[3][weaponTier-1][6]/100*playerReloadingModifier);
                attroff(COLOR_PAIR(7));
            }else{
                attron(COLOR_PAIR(5));
                mvprintw(26,1,"%s - Weapon Cost: %d - Bullet Speed: %d - Bullet Damage: %d - Bullet Range: %d - Weapon Fire Rate: %d - Weapon Max Clip: %d - Weapon Reload Rate: %d", weaponNames[3][weaponTier-1], weapons[3][weaponTier-1][0]-weapons[playerCurrentWeapon[0]][playerCurrentWeapon[1]][0]/2, weapons[3][weaponTier-1][1], weapons[3][weaponTier-1][2], weapons[3][weaponTier-1][3], weapons[3][weaponTier-1][4], weapons[3][weaponTier-1][5], weapons[3][weaponTier-1][6] + weapons[3][weaponTier-1][6]/100*playerReloadingModifier);
                attroff(COLOR_PAIR(5));
            }

            if(weapons[4][weaponTier-1][7] == 1){
                attron(COLOR_PAIR(6));
                mvprintw(28,1,"%s - Weapon Cost: %d - Bullet Speed: %d - Bullet Damage: %d - Bullet Range: %d - Weapon Fire Rate: %d - Weapon Max Clip: %d - Weapon Reload Rate: %d", weaponNames[4][weaponTier-1], weapons[4][weaponTier-1][0], weapons[4][weaponTier-1][1], weapons[4][weaponTier-1][2], weapons[4][weaponTier-1][3], weapons[4][weaponTier-1][4], weapons[4][weaponTier-1][5], weapons[4][weaponTier-1][6] + weapons[4][weaponTier-1][6]/100*playerReloadingModifier);
                attroff(COLOR_PAIR(6));
            }else if(playerMoney >= weapons[4][weaponTier-1][0]-weapons[playerCurrentWeapon[0]][playerCurrentWeapon[1]][0]/2){
                attron(COLOR_PAIR(7));
                mvprintw(28,1,"%s - Weapon Cost: %d - Bullet Speed: %d - Bullet Damage: %d - Bullet Range: %d - Weapon Fire Rate: %d - Weapon Max Clip: %d - Weapon Reload Rate: %d", weaponNames[4][weaponTier-1], weapons[4][weaponTier-1][0]-weapons[playerCurrentWeapon[0]][playerCurrentWeapon[1]][0]/2, weapons[4][weaponTier-1][1], weapons[4][weaponTier-1][2], weapons[4][weaponTier-1][3], weapons[4][weaponTier-1][4], weapons[4][weaponTier-1][5], weapons[4][weaponTier-1][6] + weapons[4][weaponTier-1][6]/100*playerReloadingModifier);
                attroff(COLOR_PAIR(7));
            }else{
                attron(COLOR_PAIR(5));
                mvprintw(28,1,"%s - Weapon Cost: %d - Bullet Speed: %d - Bullet Damage: %d - Bullet Range: %d - Weapon Fire Rate: %d - Weapon Max Clip: %d - Weapon Reload Rate: %d", weaponNames[4][weaponTier-1], weapons[4][weaponTier-1][0]-weapons[playerCurrentWeapon[0]][playerCurrentWeapon[1]][0]/2, weapons[4][weaponTier-1][1], weapons[4][weaponTier-1][2], weapons[4][weaponTier-1][3], weapons[4][weaponTier-1][4], weapons[4][weaponTier-1][5], weapons[4][weaponTier-1][6] + weapons[4][weaponTier-1][6]/100*playerReloadingModifier);
                attroff(COLOR_PAIR(5));
            }

            if(weapons[4][weaponTier-1][8] == 8 && weapons[4][weaponTier-1][7] == 1){
                attron(COLOR_PAIR(9));
                mvprintw(28,1,"%s - Weapon Cost: %d - Bullet Speed: %d - Bullet Damage: %d - Bullet Range: %d - Weapon Fire Rate: %d - Weapon Max Clip: %d - Weapon Reload Rate: %d", weaponNames[4][weaponTier-1], weapons[4][weaponTier-1][0], weapons[4][weaponTier-1][1], weapons[4][weaponTier-1][2], weapons[4][weaponTier-1][3], weapons[4][weaponTier-1][4], weapons[4][weaponTier-1][5], weapons[4][weaponTier-1][6] + weapons[4][weaponTier-1][6]/100*playerReloadingModifier);
                attroff(COLOR_PAIR(9));
            }else if(weapons[4][weaponTier-1][8] == 8 && playerMoney >= weapons[4][weaponTier-1][0]-weapons[playerCurrentWeapon[0]][playerCurrentWeapon[1]][0]/2){
                attron(COLOR_PAIR(10));
                mvprintw(28,1,"%s - Weapon Cost: %d - Bullet Speed: %d - Bullet Damage: %d - Bullet Range: %d - Weapon Fire Rate: %d - Weapon Max Clip: %d - Weapon Reload Rate: %d", weaponNames[4][weaponTier-1], weapons[4][weaponTier-1][0]-weapons[playerCurrentWeapon[0]][playerCurrentWeapon[1]][0]/2, weapons[4][weaponTier-1][1], weapons[4][weaponTier-1][2], weapons[4][weaponTier-1][3], weapons[4][weaponTier-1][4], weapons[4][weaponTier-1][5], weapons[4][weaponTier-1][6] + weapons[4][weaponTier-1][6]/100*playerReloadingModifier);
                attroff(COLOR_PAIR(10));
            }else if(weapons[4][weaponTier-1][8] == 8){
                attron(COLOR_PAIR(8));
                mvprintw(28,1,"%s - Weapon Cost: %d - Bullet Speed: %d - Bullet Damage: %d - Bullet Range: %d - Weapon Fire Rate: %d - Weapon Max Clip: %d - Weapon Reload Rate: %d", weaponNames[4][weaponTier-1], weapons[4][weaponTier-1][0]-weapons[playerCurrentWeapon[0]][playerCurrentWeapon[1]][0]/2, weapons[4][weaponTier-1][1], weapons[4][weaponTier-1][2], weapons[4][weaponTier-1][3], weapons[4][weaponTier-1][4], weapons[4][weaponTier-1][5], weapons[4][weaponTier-1][6] + weapons[4][weaponTier-1][6]/100*playerReloadingModifier);
                attroff(COLOR_PAIR(8));
            }


        }

        move(4+y, x*10);
        addch('>');

        int input = getch();

        switch(input){
            case KEY_UP:
                if(y > 0){
                    if(y > 10){
                        y -= 2;
                        if(y+2 == 16){
                            y -= 2;
                        }
                    }else{
                        y--;
                    }
                    x = 0;
                }

                break;

            case KEY_DOWN:
                if(y < 24){
                    if(y < 10){
                        y++;
                    }else{
                        if(y == 10 || weaponTier != 0){
                            y += 2;
                            if(y-2 == 12){
                                y += 2;
                            }
                        }
                    }
                    x = 0;
                }
                break;

            case KEY_RIGHT:
                if(!lastTiersUnlocked){
                    if(x < 4){
                        x++;
                    }
                }else{
                    if(x < 7){
                        x++;
                    }
                }
                if(y != 8){
                    y = 12;
                }
                break;

            case KEY_LEFT:
                if(x > 0){
                    x--;
                }
                break;

            case ' ':
                switch(y){
                    case 0:
                        done = true;
                        break;

                    case 1:
                        if(playerMoney >= 1){
                            playerMoney -= 1;
                            playerAmmo += 10;
                            playerAmountBought++;
                        }
                        break;

                    case 2:
                        if(playerMoney >= bandageCost){
                            playerMoney -= 5;
                            playerBandages++;
                            bandageCost *= 1.5;
                            playerAmountBought++;
                        }
                        break;

                    case 3:
                        if(playerMoney >= meleeCost && playerMeleeDamage < 5){
                            playerMeleeDamage++;
                            playerMoney -= meleeCost;
                            meleeCost *= 2;
                            playerAmountBought++;
                        }
                        break;

                    case 4:
                        if(playerMoney >= 5){
                            playerMoney -= 5;
                            playerStaminaPills++;
                            playerAmountBought++;
                        }

                    case 5:
                        if(playerMoney >= upgradeCost[playerCurrentWeapon[0]][playerCurrentWeapon[1]] && playerBulletSpeed != temp1){
                            playerMoney -= upgradeCost[playerCurrentWeapon[0]][playerCurrentWeapon[1]];
                            weapons[playerCurrentWeapon[0]][playerCurrentWeapon[1]][1] = playerBulletSpeed/1.2;
                            playerBulletSpeed = playerBulletSpeed/1.2;
                            weapons[playerCurrentWeapon[0]][playerCurrentWeapon[1]][0] += upgradeCost[playerCurrentWeapon[0]][playerCurrentWeapon[1]];
                            upgradeCost[playerCurrentWeapon[0]][playerCurrentWeapon[1]] *= 1.5;
                            playerAmountBought++;
                        }
                        break;

                    case 6:
                        if(playerMoney >= upgradeCost[playerCurrentWeapon[0]][playerCurrentWeapon[1]]){
                            playerMoney -= upgradeCost[playerCurrentWeapon[0]][playerCurrentWeapon[1]];
                            weapons[playerCurrentWeapon[0]][playerCurrentWeapon[1]][2] = playerDamage+1;
                            playerDamage = playerDamage+1;
                            weapons[playerCurrentWeapon[0]][playerCurrentWeapon[1]][0] += upgradeCost[playerCurrentWeapon[0]][playerCurrentWeapon[1]];
                            upgradeCost[playerCurrentWeapon[0]][playerCurrentWeapon[1]] *= 1.5;
                            playerAmountBought++;
                        }
                        break;

                    case 7:
                        if(playerMoney >= upgradeCost[playerCurrentWeapon[0]][playerCurrentWeapon[1]] && playerBulletRange != temp2){
                            playerMoney -= upgradeCost[playerCurrentWeapon[0]][playerCurrentWeapon[1]];
                            weapons[playerCurrentWeapon[0]][playerCurrentWeapon[1]][3] = playerBulletRange*1.2;
                            playerBulletRange = playerBulletRange*1.2;
                            weapons[playerCurrentWeapon[0]][playerCurrentWeapon[1]][0] += upgradeCost[playerCurrentWeapon[0]][playerCurrentWeapon[1]];
                            upgradeCost[playerCurrentWeapon[0]][playerCurrentWeapon[1]] *= 1.5;
                            playerAmountBought++;
                        }
                        break;

                    case 8:
                        if(playerMoney >= upgradeCost[playerCurrentWeapon[0]][playerCurrentWeapon[1]] && playerFiringSpeed != temp3){
                            playerMoney -= upgradeCost[playerCurrentWeapon[0]][playerCurrentWeapon[1]];
                            weapons[playerCurrentWeapon[0]][playerCurrentWeapon[1]][4] = playerFiringSpeed/1.2;
                            playerFiringSpeed = playerFiringSpeed/1.2;
                            weapons[playerCurrentWeapon[0]][playerCurrentWeapon[1]][0] += upgradeCost[playerCurrentWeapon[0]][playerCurrentWeapon[1]];
                            upgradeCost[playerCurrentWeapon[0]][playerCurrentWeapon[1]] *= 1.5;
                            playerAmountBought++;
                        }
                        break;

                    case 9:
                        if(playerMoney >= upgradeCost[playerCurrentWeapon[0]][playerCurrentWeapon[1]] && playerClipLimit != temp4){
                            playerMoney -= upgradeCost[playerCurrentWeapon[0]][playerCurrentWeapon[1]];
                            weapons[playerCurrentWeapon[0]][playerCurrentWeapon[1]][5] = playerClipLimit*1.2;
                            playerClipLimit= playerClipLimit*1.2;
                            weapons[playerCurrentWeapon[0]][playerCurrentWeapon[1]][0] += upgradeCost[playerCurrentWeapon[0]][playerCurrentWeapon[1]];
                            upgradeCost[playerCurrentWeapon[0]][playerCurrentWeapon[1]] *= 1.5;
                            playerAmountBought++;
                        }
                        break;

                    case 10:
                        if(playerMoney >= upgradeCost[playerCurrentWeapon[0]][playerCurrentWeapon[1]] && playerReloadingRate != temp5){
                            playerMoney -= upgradeCost[playerCurrentWeapon[0]][playerCurrentWeapon[1]];
                            weapons[playerCurrentWeapon[0]][playerCurrentWeapon[1]][6] = playerReloadingRate/1.2;
                            playerReloadingRate = playerReloadingRate/1.2;
                            weapons[playerCurrentWeapon[0]][playerCurrentWeapon[1]][0] += upgradeCost[playerCurrentWeapon[0]][playerCurrentWeapon[1]];
                            upgradeCost[playerCurrentWeapon[0]][playerCurrentWeapon[1]] *= 1.5;
                            playerAmountBought++;
                        }
                        break;

                    case 12:
                        switch(x){
                            case 0:
                                weaponTier = 1;
                                break;

                            case 1:
                                weaponTier = 2;
                                break;

                            case 2:
                                weaponTier = 3;
                                break;

                            case 3:
                                weaponTier = 4;
                                break;

                            case 4:
                                weaponTier = 5;
                                break;

                            case 5:
                                weaponTier = 6;
                                break;

                            case 6:
                                weaponTier = 7;
                                break;

                            case 7:
                                weaponTier = 8;
                                break;
                        }
                        break;

                    case 16:
                        weaponSelection = 0;
                        break;

                    case 18:
                        weaponSelection = 1;
                        break;

                    case 20:
                        weaponSelection = 2;
                        break;

                    case 22:
                        weaponSelection = 3;
                        break;

                    case 24:
                        weaponSelection = 4;
                        break;

                }
                break;

            case 'q':
                done = true;
                break;

            case 27:
                done = true;
                break;
        }

        if(weaponSelection != -1){
            if(playerMoney >= weapons[weaponSelection][weaponTier-1][0]-weapons[playerCurrentWeapon[0]][playerCurrentWeapon[1]][0]/2 && weapons[weaponSelection][weaponTier-1][7] == 0){
                playerMoney -= weapons[weaponSelection][weaponTier-1][0]-weapons[playerCurrentWeapon[0]][playerCurrentWeapon[1]][0]/2;
                weapons[playerCurrentWeapon[0]][playerCurrentWeapon[1]][7] = 0;
                weapons[weaponSelection][weaponTier-1][7] = 1;
                playerCurrentWeapon = {weaponSelection, weaponTier-1};
                playerBulletSpeed = weapons[weaponSelection][weaponTier-1][1];
                playerDamage = weapons[weaponSelection][weaponTier-1][2];
                playerBulletRange = weapons[weaponSelection][weaponTier-1][3];
                playerFiringSpeed = weapons[weaponSelection][weaponTier-1][4];
                playerClipLimit = weapons[weaponSelection][weaponTier-1][5];
                playerReloadingRate = weapons[weaponSelection][weaponTier-1][6] += weapons[weaponSelection][weaponTier-1][6]/100*playerReloadingModifier;
                for(int i = 0; i < 10; i++)
                    playerSpecialWeaponTypes[i] = false;

                if(weapons[weaponSelection][weaponTier-1][8] != 0){
                    playerSpecialWeaponTypes[0] = true;
                    playerSpecialWeaponTypes[weapons[weaponSelection][weaponTier-1][8]] = true;
                }
                for(int i = 0; i < 30; i++){
                    playerWeaponName[i] = weaponNames[weaponSelection][weaponTier-1][i];
                }
                if(playerClip > playerClipLimit){
                    playerAmmo += playerClip - playerClipLimit;
                    playerClip = playerClipLimit;
                }
                playerAmountBought++;
            }
        }
        weaponSelection = -1;

        refresh();
    }
    resize_term(30, 80);
}
