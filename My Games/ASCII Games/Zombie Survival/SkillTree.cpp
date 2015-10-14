#include "Engine.h"
#include "Actor.h"
#include "Bullet.h"
#include "Shop.h"
#include "SkillTree.h"
#include "SpecialBullet.h"
#include <cstdlib>
#include <ctime>
#include <curses.h>

SkillTree::SkillTree()
{
    skills = {
        {{1,1,1},{3,2,25},{0,0,0},{0,0,0},{0,0,0}},
        {{1,2,25},{0,0,0},{3,4,-50},{0,0,0},{0,0,0}},
        {{1,3,5},{3,3,15},{0,0,0},{3,1,3},{0,0,0}},
        {{1,4,-25},{0,0,0},{3,2,50},{0,0,0},{3,0,0}},
        {{1,5,-5},{3,4,-25},{0,0,0},{3,7,3},{0,0,0}},
        {{1,6,-5},{0,0,0},{3,5,-10},{0,0,0},{3,0,0}},
        {{1,7,1},{3,7,2},{0,0,0},{3,3,20},{0,0,0}},
        {{1,8,25},{0,0,0},{3,6,-15},{0,0,0},{0,0,0}},
        {{1,8,25},{3,1,2},{0,0,0},{0,0,0},{0,0,0}},
    };

    skills[3][4][1] = 9 + rand() % 2;

    if(skills[3][4][1] == 9){
        skills[5][4][1] = 10;
    }else{
        skills[5][4][1] = 9;
    }

    skillNames = {
        {{'L','o','o','t','i','n','g','+','1'}                      ,{'M','a','x',' ','H','e','a','l','t','h','+','2','5'}      ,{}                                                         ,{}                                                             ,{}},
        {{'M','a','x',' ','H','e','a','l','t','h','+','2','5'}      ,{}                                                         ,{'S','t','a','m','i','n','a',' ','R','e','g','-','5','0'}  ,{}                                                             ,{}},
        {{'M','a','x',' ', 'S','t','a','m','i','n','a','+','5'}     ,{'M','a','x',' ','S','t','a','m','i','n','a','+','1','5'}  ,{}                                                         ,{'L','o','o','t','i','n','g','+','3'}                          ,{}},
        {{'S','t','a','m','i','a',' ','R','e','g','-','2','5'}      ,{}                                                         ,{'M','a','x',' ','H','e','a','l','t','h','+','5','0'}      ,{}                                                             ,{'?','?','?'}},
        {{'M','o','v','e',' ','S','p','e','e','d','-','5'}          ,{'S','t','a','m','i','n','a',' ','R','e','g','-','2','5'}  ,{}                                                         ,{'L','e','a','r','n','i','n','g','+','3'}                      ,{}},
        {{'R','e','l','o','a','d',' ','S','p','d','-','5','%'}      ,{}                                                         ,{'M','o','v','e',' ','S','p','e','e','d','-','1','0'}      ,{}                                                             ,{'?','?','?'}},
        {{'L','e','a','r','n','i','n','g','+','1'}                  ,{'L','e','a','r','n','i','n','g','+','2'}                  ,{}                                                         ,{'M','a','x',' ','S','t','a','m','i','n','a','+','2','0'}      ,{}},
        {{'M','o','n','e','y','+','2','5'}                          ,{}                                                         ,{'R','e','l','o','a','d',' ','S','p','d','-','1','5','%'}  ,{}                                                             ,{}},
        {{'M','o','n','e','y','+','2','5'}                          ,{'L','o','o','t','i','n','g','+','2'}                      ,{}                                                         ,{}                                                             ,{}},
    };
}

void SkillTree::skillScreen(){

    bool done = false;
    int y = -1;
    int x = 0;

    while(!done){
        clear();

        mvprintw(0,1,"Welcome to the skill tree!");
        mvprintw(1,1,"Press q, t, esc or select the back button to exit the skill tree.");
        mvprintw(2,1,"Skill Points: %d", playerSkillPoints);
        mvprintw(5,1,"Back");

        for(int y = 0; y < 9; y++){
            for(int x = 0; x < 5; x++){

                if(x == 1){
                    if(skills[y][x][0] == 3){
                        if(y != 0 || y != 8){
                            if(skills[y][x-1][0] == 2 && skills[y-1][x-1][0] == 2 && skills[y+1][x-1][0] == 2){
                                skills[y][x][0] = 1;
                            }
                        }
                        if(y == 0){
                            if(skills[y][x-1][0] == 2 && skills[y+1][x-1][0] == 2){
                                skills[y][x][0] = 1;
                            }
                        }
                        if(y == 8){
                            if(skills[y][x-1][0] == 2 && skills[y-1][x-1][0] == 2){
                                skills[y][x][0] = 1;
                            }
                        }
                    }
                }else if(x == 2){
                    if(skills[y][x][0] == 3){
                        if(skills[y-1][x-1][0] == 2 && skills[y+1][x-1][0] == 2){
                            skills[y][x][0] = 1;
                        }
                    }
                }else if(x == 3){
                    if(skills[y][x][0] == 3){
                        if(skills[y-1][x-1][0] == 2 && skills[y+1][x-1][0] == 2){
                            skills[y][x][0] = 1;
                        }
                    }
                }else if(x == 4){
                    if(skills[y][x][0] == 3){
                        if(skills[y-1][x-1][0] == 2 && skills[y+1][x-1][0] == 2){
                            skills[y][x][0] = 1;
                        }
                    }
                }

                if(skills[y][x][0] == 1){
                    mvprintw(6+y,1+x*15, "%s", skillNames[y][x]);
                }else if(skills[y][x][0] == 2){
                    attron(COLOR_PAIR(6));
                    mvprintw(6+y,1+x*15, "%s", skillNames[y][x]);
                    attroff(COLOR_PAIR(6));
                }else if(skills[y][x][0] == 3){
                    attron(COLOR_PAIR(5));
                    mvprintw(6+y,1+x*15, "%s", skillNames[y][x]);
                    attroff(COLOR_PAIR(5));
                }
            }
        }

        move(6+y, x*15);
        addch('>');

        int input = getch();

        switch(input){
            case KEY_UP:
                if(x == 0){
                    if(y > -1){
                        y--;
                    }
                }else if(x == 1){
                    if(y > 0){
                        y -= 2;
                    }
                }else if(x == 2){
                    if(y > 1){
                        y -= 2;
                    }
                }else if(x == 3){
                    if(y > 2){
                        y -= 2;
                    }
                }else if(x == 4){
                    if(y > 3){
                        y -= 2;
                    }
                }
                break;

            case KEY_DOWN:
                if(x == 0){
                    if(y < 8){
                        y++;
                    }
                }else if(x == 1){
                    if(y < 8){
                        y += 2;
                    }
                }else if(x == 2){
                    if(y < 7){
                        y += 2;
                    }
                }else if(x == 3){
                    if(y < 6){
                        y += 2;
                    }
                }else if(x == 4){
                    if(y < 5){
                        y += 2;
                    }
                }
                break;

            case KEY_LEFT:
                if(x > 0){
                    x--;
                    if(x == 0){
                        y = 0;
                    }else if(x == 1){
                        y = 0;
                    }else if(x == 2){
                        y = 1;
                    }else if(x == 3){
                        y = 2;
                    }
                }
                break;

            case KEY_RIGHT:
                if(x < 4){
                    x++;
                    if(x == 1){
                        y = 0;
                    }else if(x == 2){
                        y = 1;
                    }else if(x == 3){
                        y = 2;
                    }else if(x == 4){
                        y = 3;
                    }
                }
                break;

            case ' ':
                if(y == -1 && x == 0)
                    done = true;

                if(skills[y][x][0] == 1){
                    if(playerSkillPoints >= x+1){
                        switch(skills[y][x][1]){
                            case 1:
                                killMoney += skills[y][x][2];
                                skills[y][x][0] = 2;
                                playerSkillPoints -= x+1;
                                break;

                            case 2:
                                playerMaxHealth += skills[y][x][2];
                                playerHealth += skills[y][x][2];
                                skills[y][x][0] = 2;
                                playerSkillPoints -= x+1;
                                break;

                            case 3:
                                playerMaxStamina += skills[y][x][2];
                                skills[y][x][0] = 2;
                                playerSkillPoints -= x+1;
                                break;

                            case 4:
                                playerStaminaRegen += skills[y][x][2];
                                skills[y][x][0] = 2;
                                playerSkillPoints -= x+1;
                                break;

                            case 5:
                                playerMovementSpeed += skills[y][x][2];
                                skills[y][x][0] = 2;
                                playerSkillPoints -= x+1;
                                break;

                            case 6:
                                playerReloadingModifier += skills[y][x][2];
                                playerReloadingRate += (playerReloadingRate/100*skills[y][x][2]);
                                skills[y][x][0] = 2;
                                playerSkillPoints -= x+1;
                                break;

                            case 7:
                                playerXPGet += skills[y][x][2];
                                skills[y][x][0] = 2;
                                playerSkillPoints -= x+1;
                                break;

                            case 8:
                                playerMoney += skills[y][x][2];
                                skills[y][x][0] = 2;
                                playerSkillPoints -= x+1;
                                break;
                        }

                        if(x == 4){
                            if(skills[y][x][1] == 9){
                                skillNames[y][x] = {'+','3',' ','W','e','a','p','o','n',' ','T','i','e','r','s'};
                                skills[y][x][0] = 2;
                                lastTiersUnlocked = true;
                                playerSkillPoints -= x+1;
                            }else if(skills[y][x][1] == 10){
                                skillNames[y][x] = {'5','x',' ','Z','o','m','b','i','e','s'};
                                roundZombiesFloat *= 5;
                                zombieSpawnRate -= (zombieSpawnRate/5);
                                skills[y][x][0] = 2;
                                playerSkillPoints -= x+1;
                            }
                        }
                    }
                }

                break;

            case 'q':
                done = true;
                break;

            case 27:
                done = true;
                break;

            case 't':
                done = true;
                break;
        }

        refresh();
    }
}
