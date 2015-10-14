#include "Engine.h"
#include "Enemy.h"
#include "Bullet.h"
#include "Button.h"
#include "Pickup.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <cstring>
#include <cmath>
#include <cstdlib>
#include <ctime>

// DO A TIMELAPSE! ----------------------------------------------------------------------------------------------- DO A TIMELAPSE

//Allegro Variables -- START
ALLEGRO_FONT *buttonFont;
ALLEGRO_FONT *defaultFont;
ALLEGRO_FONT *titleFont;
//Allegro Variables -- END

//Funtion Prototypes -- START
bool addBulletToList(Bullet *newBullet);
bool addEnemyToList(Enemy *newEnemy);
bool addButtonToList(Button *newButton);
bool addPickupToList(Pickup *newPickup);
bool isPassable(int x, int y, int width, int height);
bool checkCollision(int x, int y, int ex, int ey, int width, int height, int ewidth, int eheight);
//Funtion Prototypes -- END

//Pointer Arrays -- START
Enemy *enemyList[MAX_ENEMIES];
Bullet *bulletList[MAX_BULLETS];
Button *buttonList[MAX_BUTTONS];
Pickup *pickupList[MAX_PICKUPS];
//Pointer Arrays -- END

//Technical Variables -- START
bool done, retry = true, draw, initButtons, mouseLeftDown, resetShop, startMenu = true, shopMenu = false;
int screenState;
float mouseX, mouseY, distanceX, distanceY, radiansToMouse;
//Technical Varuabkes -- END

//Player Variables -- START
bool playerAutoShoot;
float playerX, playerY, playerCenterX, playerCenterY;
float playerMoveSpeed;
float playerShootingSpeed, playerShootingSpeedHelper;
int playerShots[4], playerHealth, playerMaxHealth, playerDamage;
int playerWidth, playerHeight, playerHitBoxWidth, playerHitBoxHeight, lastKeyPress;
int playerScore;
float playerDPS, fpsTimeOld, fpsTimeNew, fpsCounter;
//Player Variables -- END

//Enemy Variables -- START
float enemyFormationSpawn, enemyFormationSpawnHelper;
int enemyStatsBooster;
//Enemy Variables -- END

int main(){

    float FPS = 60.0;

    //Allegro Init -- START
    al_init();
    al_init_font_addon();
    al_init_ttf_addon();
    al_init_primitives_addon();
    al_init_image_addon();
    al_init_acodec_addon();
    al_install_keyboard();
    al_install_mouse();
    al_install_audio();

    al_set_new_display_flags(ALLEGRO_WINDOWED);
    ALLEGRO_DISPLAY *display = al_create_display(screenWidth, screenHeight);
    al_set_window_title(display, "Bullet Hell Shooter");

    ALLEGRO_BITMAP *cursorImage = al_load_bitmap("graphics/cursorImage.png");
    ALLEGRO_BITMAP *enemySeeker = al_load_bitmap("graphics/enemySeeker.png");
    ALLEGRO_BITMAP *healthPickup = al_load_bitmap("graphics/healthPickup.png");
    ALLEGRO_BITMAP *healthPickupUpgrade = al_load_bitmap("graphics/healthPickupUpgrade.png");
    ALLEGRO_BITMAP *weaponPickupUp = al_load_bitmap("graphics/WeaponPickupUp.png");
    ALLEGRO_BITMAP *weaponPickupDown = al_load_bitmap("graphics/WeaponPickupDown.png");
    ALLEGRO_BITMAP *weaponPickupLeft = al_load_bitmap("graphics/WeaponPickupLeft.png");
    ALLEGRO_BITMAP *weaponPickupRight = al_load_bitmap("graphics/WeaponPickupRight.png");
    ALLEGRO_BITMAP *weaponPickupUpgrade = al_load_bitmap("graphics/WeaponPickupUpgrade.png");
    buttonFont = al_load_font("fonts/times.ttf", 18, NULL);
    defaultFont = al_load_font("fonts/times.ttf", 24, NULL);
    titleFont = al_load_font("fonts/times.ttf", 48, NULL);
    ALLEGRO_SAMPLE *playerShotSound = al_load_sample("sounds/playerShotSound.wav");
    ALLEGRO_SAMPLE *playerDeathSound = al_load_sample("sounds/playerDeathSound.wav");
    ALLEGRO_SAMPLE *playerHitSound = al_load_sample("sounds/playerHitSound.wav");
    ALLEGRO_SAMPLE *enemyShotSound = al_load_sample("sounds/enemyShotSound.wav");
    ALLEGRO_SAMPLE *enemyDeathSound = al_load_sample("sounds/enemyDeathSound.wav");
    ALLEGRO_SAMPLE *enemyHitSound = al_load_sample("sounds/enemyHitSound.wav");
    ALLEGRO_SAMPLE *pickupSound = al_load_sample("sounds/pickupSound.wav");
    ALLEGRO_SAMPLE *buttonPressSound = al_load_sample("sounds/buttonPressSound.wav");
    ALLEGRO_COLOR gray = al_map_rgb(122, 122, 122);
    ALLEGRO_COLOR red = al_map_rgb(255, 50, 50);
    ALLEGRO_COLOR green = al_map_rgb(50, 255, 50);
    ALLEGRO_COLOR blue = al_map_rgb(50, 50, 255);
    ALLEGRO_COLOR silver = al_map_rgb(205, 205, 205);
    ALLEGRO_COLOR gold = al_map_rgb(255, 215, 50);
    ALLEGRO_KEYBOARD_STATE keyState;
    ALLEGRO_MOUSE_STATE mouseState;
    ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();
    ALLEGRO_TIMER *timer = al_create_timer(1/FPS);

    al_reserve_samples(8);

    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_mouse_event_source());
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_display_event_source(display));
    //Allegro Init -- END

    al_hide_mouse_cursor(display);

    srand(time(0));

    while(retry){
            //Technical Variables -- START
            float shopResetValues[9] = {0,0,0,0,0,0,0,0,0};
            float deltaX = 0, deltaY = 0;
            done = false, retry = true, draw = true, initButtons = true, mouseLeftDown = false, resetShop = false;
            screenState = titleState;
            mouseX = 0, mouseY = 0, distanceX = 0, distanceY = 0, radiansToMouse = 0;
            //Technical Variables -- END

            //Player Variables -- START
            playerAutoShoot = false;
            playerX = 10, playerY = 10, playerCenterX = 0, playerCenterY = 0;
            playerMoveSpeed = 0;
            playerShootingSpeed = 60, playerShootingSpeedHelper = 0;
            playerHealth = 3, playerMaxHealth = 3, playerDamage = 1;
            playerWidth = 24, playerHeight = 24, playerHitBoxWidth = 5, playerHitBoxHeight = 5, lastKeyPress = NULL;
            playerScore = 0;
            playerDPS = 0, fpsTimeOld = al_get_time(), fpsTimeNew = al_get_time(), fpsCounter = 0;

            for(int i = 0; i < 4; i++){
                playerShots[i] = 0;
            }
            playerShots[0] = 1;
            //Player Variables -- END

            //Enemy Variables -- START
            enemyFormationSpawn = 100, enemyFormationSpawnHelper = 0, enemyStatsBooster = 0;
            //Enemy Variables -- END

        {   //Pointer Lists Resetting
            for(int i = 0; i < MAX_ENEMIES; i++){
                enemyList[i] = NULL;
            }

            for(int i = 0; i < MAX_BULLETS; i++){
                bulletList[i] = NULL;
            }

            for(int i = 0; i < MAX_BUTTONS; i++){
                buttonList[i] = NULL;
            }

            for(int i = 0; i < MAX_PICKUPS; i++){
                pickupList[i] = NULL;
            }
        }

        al_start_timer(timer);

        while(!done){
            ALLEGRO_EVENT events;
            al_wait_for_event(event_queue, &events);

            if(events.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
                done = true;
                retry = false;
            }

            if(events.type == ALLEGRO_EVENT_TIMER){

                al_get_mouse_state(&mouseState);

                al_get_keyboard_state(&keyState);

                //Update Mouse Variables -- START
                mouseX = al_get_mouse_state_axis(&mouseState,0);
                mouseY = al_get_mouse_state_axis(&mouseState,1);

                if(al_mouse_button_down(&mouseState, 1)){
                    mouseLeftDown = true;
                }else{
                    mouseLeftDown = false;
                }
                //Update Mouse Variables -- END

                if(screenState == titleState){

                    //Button Init -- START
                    if(initButtons){
                        initButtons = false;
                        for(int i = 0; i < 3; i++){
                            float x, y, width, height;
                            width = 100;
                            height = 50;
                            x = screenWidth/2 - width/2;
                            y = screenHeight/2 - height/2 + i*100;

                            Button *newButton = new Button();

                            newButton->setPos(x, y);
                            newButton->setDimensions(width, height);
                            newButton->setButtonId(i);
                            addButtonToList(newButton);
                        }
                    }
                    //Button Init -- END

                    if(al_key_down(&keyState, ALLEGRO_KEY_ESCAPE) && lastKeyPress != ALLEGRO_KEY_ESCAPE){
                        if(!startMenu){
                            if(!shopMenu){
                                screenState = gameState;
                            }else{
                                screenState = shopState;
                            }
                        }
                        lastKeyPress = ALLEGRO_KEY_ESCAPE;
                    }

                    for(int i = 0; i < MAX_BUTTONS; i++){
                        if(buttonList[i] != NULL && buttonList[i]->checkActive()){
                            if(checkCollision(mouseX, mouseY, buttonList[i]->posX, buttonList[i]->posY, 0, 0, buttonList[i]->width, buttonList[i]->height)){
                                buttonList[i]->update();
                                if(buttonList[i]->playButtonPress){
                                    al_play_sample(buttonPressSound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                                }
                            }
                        }
                    }

                    if(screenState != titleState){
                        initButtons = true;
                    }

                }
                else if(screenState == gameState){

                    if(initButtons){
                        startMenu = true;
                        initButtons = false;
                        for(int i = 0; i < MAX_BUTTONS; i++){
                            buttonList[i] = NULL;
                        }
                    }

                    if(playerHealth <= 0){
                        screenState = loseState;
                        al_play_sample(playerDeathSound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                    }

                    if(enemyStatsBooster % 8750 == 0 && enemyStatsBooster != 0){
                        screenState = shopState;
                    }

                    //Enemy Formation Spawn -- START

                    if(enemyFormationSpawnHelper >= enemyFormationSpawn - enemyStatsBooster/350){
                        if(enemyFormationSpawn - enemyStatsBooster/350 >= 10 || (enemyFormationSpawn - enemyStatsBooster/350 <= 10 && enemyFormationSpawnHelper >= 10)){
                            float health;
                            int shotAmount = 1, enemyId = 0;
                            float x, y, width, height, moveSpeed, shootingSpeed, angle = 0.5;
                            x = rand() % mapWidth;
                            y = 0;
                            width = (rand()%100/10 + 10)*2;
                            height = width;
                            health = (width-4)/8 * (enemyStatsBooster/3750+1);
                            moveSpeed = (20-width/2)/4;
                            shootingSpeed = 100 - width/2 - enemyStatsBooster/750;

                            if(enemyStatsBooster >= 8750){
                                enemyId = rand() % 2;
                            }

                            if(shootingSpeed <= 10)
                                shootingSpeed = 10;

                            if(enemyStatsBooster >= 17500 && enemyId == 0){
                                int randomPercent = rand() % 10000/100;

                                if(randomPercent <= 50)
                                    shotAmount = 1;
                                else if(randomPercent > 50 && randomPercent <= 75)
                                    shotAmount = 2;
                                else if(randomPercent > 75 && randomPercent <= 87.5)
                                    shotAmount = 3;
                                else if(randomPercent > 87.5 && randomPercent <= 93.75)
                                    shotAmount = 4;
                                else
                                    shotAmount = 5;

                            }

                            if(enemyId == 1){
                                health /= 3;
                                moveSpeed = 7.5;
                                width = 32;
                                height = 32;
                                angle = -atan2((x + width/2) - playerCenterX, (y + height/2) - playerCenterY);
                            }

                            Enemy *newEnemy = new Enemy();

                            newEnemy->setPos(x, y);
                            newEnemy->setDimensions(width, height);
                            newEnemy->setEnemyId(enemyId);
                            newEnemy->setHealth(health);
                            newEnemy->setMaxHealth(health);
                            newEnemy->setShotAmount(shotAmount);
                            newEnemy->setAngleSpeed(moveSpeed, angle);
                            newEnemy->setShootingSpeed(shootingSpeed);
                            addEnemyToList(newEnemy);

                            enemyFormationSpawnHelper = 0;
                        }
                    }

                    //Enemy Formation Spawn -- END

                    //Player Shooting Segment -- START

                    if((mouseLeftDown || playerAutoShoot) && playerShootingSpeedHelper >= playerShootingSpeed){

                        int playerShotsTemp[4] = {0,0,0,0};

                        int x, y, width, height, damage;
                        float moveSpeed, angle = 0;

                        width = 10, height = 10, x = playerCenterX - width/2, y = playerCenterY - height/2, damage = playerDamage;
                        moveSpeed = 2;

                        for(int i = 0; i < (playerShots[0] + playerShots[1] + playerShots[2] + playerShots[3]); i++){

                            //Shots Angle Calculation -- START
                            if(playerShotsTemp[0] < playerShots[0]){ //UP
                                playerShotsTemp[0]++;
                                if(playerShots[0] != 1){
                                    angle = -0.125+(0.25/(playerShots[0]-1))*i;
                                }else{
                                    angle = 0;
                                }
                            }else if(playerShotsTemp[1] < playerShots[1]){ //RIGHT
                                playerShotsTemp[1]++;
                                if(playerShots[1] != 1){
                                    angle = 0.125+(0.25/(playerShots[1]-1))*(i-playerShotsTemp[0]);
                                }else{
                                    angle = 0.25;
                                }
                            }else if(playerShotsTemp[2] < playerShots[2]){ //DOWN
                                playerShotsTemp[2]++;
                                if(playerShots[2] != 1){
                                    angle = 0.375+(0.25/(playerShots[2]-1))*(i-(playerShotsTemp[0] + playerShotsTemp[1]));
                                }else{
                                    angle = 0.5;
                                }
                            }else if(playerShotsTemp[3] < playerShots[3]){ //LEFT
                                playerShotsTemp[3]++;
                                if(playerShots[3] != 1){
                                    angle = 0.625+(0.25/(playerShots[3]-1))*(i-(playerShotsTemp[0] + playerShotsTemp[1] + playerShotsTemp[2]));
                                }else{
                                    angle = 0.75;
                                }
                            }
                            //Shots Angle Calculation -- END

                            Bullet *newBullet = new Bullet();
                            newBullet->setPos(x, y);
                            newBullet->setDimensions(width, height);
                            newBullet->setDamage(damage);
                            newBullet->setAngleSpeed(moveSpeed, angle);
                            newBullet->setPlayerShot(true);

                            addBulletToList(newBullet);
                        }
                        al_play_sample(playerShotSound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                        playerShootingSpeedHelper = 0;
                    }

                    //Player Shooting Segment -- END

                    //Player Button Press Check -- START

                    if(al_key_down(&keyState, ALLEGRO_KEY_R) && lastKeyPress != ALLEGRO_KEY_R){
                        done = true;
                    }else if(al_key_down(&keyState, ALLEGRO_KEY_F) && lastKeyPress != ALLEGRO_KEY_F){
                        playerAutoShoot = !playerAutoShoot;
                        lastKeyPress = ALLEGRO_KEY_F;
                    }else if(al_key_down(&keyState, ALLEGRO_KEY_ESCAPE) && lastKeyPress != ALLEGRO_KEY_ESCAPE){
                        screenState = titleState;
                        lastKeyPress = ALLEGRO_KEY_ESCAPE;
                    }

                    //Player Button Press Check -- END

                    //Player Movement Calculation -- START

                    distanceX = (playerCenterX - mouseX >= 0) ? playerCenterX - mouseX : -(playerCenterX - mouseX);
                    distanceY = (playerCenterY - mouseY >= 0) ? playerCenterY - mouseY : -(playerCenterY - mouseY);

                    playerMoveSpeed = (distanceX + distanceY) * 0.1;

                    radiansToMouse = -atan2(playerCenterX - mouseX, playerCenterY - mouseY);

                    deltaX = sin(radiansToMouse) * playerMoveSpeed;
                    deltaY = -cos(radiansToMouse) * playerMoveSpeed;

                   //Player Movement Calculation -- END

                    //Position Updates -- START

                    if(isPassable(playerX+deltaX, playerY, playerWidth, playerHeight)){
                        playerX += deltaX;
                    }

                    if(isPassable(playerX, playerY+deltaY, playerWidth, playerHeight)){
                        playerY += deltaY;
                    }

                    for(int i = 0; i < MAX_ENEMIES; i++){
                        if(enemyList[i] != NULL && enemyList[i]->checkActive()){
                            enemyList[i]->update();

                            if(enemyList[i]->playDeathSound){
                                al_play_sample(enemyDeathSound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                            }
                        }
                    }

                    for(int i = 0; i < MAX_BULLETS; i++){
                        if(bulletList[i] != NULL && bulletList[i]->checkActive()){
                            bulletList[i]->update();
                            if(bulletList[i]->playHitSound){
                                if(bulletList[i]->playerShot){
                                    al_play_sample(enemyHitSound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                                }else if(!bulletList[i]->playerShot){
                                    al_play_sample(playerHitSound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                                }
                            }
                        }
                    }

                    for(int i = 0; i < MAX_PICKUPS; i++){
                        if(pickupList[i] != NULL && pickupList[i]->checkActive()){
                            pickupList[i]->update();
                            if(pickupList[i]->playPickupSound){
                                al_play_sample(pickupSound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                            }
                        }
                    }

                    if(screenState != gameState){
                        initButtons = true;
                    }

                    playerCenterX = playerX + playerWidth / 2;
                    playerCenterY = playerY + playerHeight / 2;

                    // Position Updates -- END

                    //The Rest

                    playerShootingSpeedHelper++;
                    enemyFormationSpawnHelper++;
                    enemyStatsBooster++;

                }
                else if(screenState == loseState){
                    if(al_key_down(&keyState, ALLEGRO_KEY_ENTER) && lastKeyPress != ALLEGRO_KEY_ENTER){
                        done = true;
                        lastKeyPress = ALLEGRO_KEY_ENTER;
                    }else if(al_key_down(&keyState, ALLEGRO_KEY_ESCAPE) && lastKeyPress != ALLEGRO_KEY_ESCAPE){
                        done = true;
                        lastKeyPress = ALLEGRO_KEY_ESCAPE;
                    }
                }
                else if(screenState == shopState){
                    if(initButtons){
                        for(int i = 0; i < MAX_ENEMIES; i++){
                            enemyList[i] = NULL;
                        }

                        for(int i = 0; i < MAX_BULLETS; i++){
                            bulletList[i] = NULL;
                        }

                        for(int i = 0; i < MAX_BUTTONS; i++){
                            buttonList[i] = NULL;
                        }

                        for(int i = 0; i < MAX_PICKUPS; i++){
                            pickupList[i] = NULL;
                        }
                        playerX = 100;
                        playerY = screenHeight-shopTestHeight-100;
                        playerShootingSpeedHelper = 60;
                        shopMenu = true;
                        startMenu = false;
                        initButtons = false;

                        shopResetValues[0] = playerShootingSpeed;
                        shopResetValues[1] = playerHealth;
                        shopResetValues[2] = playerMaxHealth;
                        shopResetValues[3] = playerScore;
                        shopResetValues[8] = playerDamage;

                        for(int i = 0; i < 4; i++){
                            shopResetValues[i+4] = playerShots[i];
                        }

                        int amountButtons[2] = {6, 4};

                        for(int i = 0; i < amountButtons[0]; i++){
                            float x, y, width, height;
                            width = 150;
                            height = 50;
                            x = 50+((screenWidth-50)/amountButtons[0])*i;
                            y = 100;

                            Button *newButton = new Button();

                            newButton->setPos(x, y);
                            newButton->setDimensions(width, height);
                            newButton->setButtonId(3+i);
                            addButtonToList(newButton);
                        }

                        for(int i = 0; i < amountButtons[1]; i++){
                            float x, y, width, height;
                            width = 150;
                            height = 50;
                            x = 50+((screenWidth-50)/amountButtons[0])*5;
                            y = 200+100*i;

                            Button *newButton = new Button();

                            newButton->setPos(x, y);
                            newButton->setDimensions(width, height);
                            newButton->setButtonId(9+i);
                            addButtonToList(newButton);
                        }

                    }

                    //Player Shooting Segment -- START

                    if((mouseLeftDown || playerAutoShoot) && playerShootingSpeedHelper >= playerShootingSpeed){

                        int playerShotsTemp[4] = {0,0,0,0};

                        int x, y, width, height, damage;
                        float moveSpeed, angle = 0;

                        width = 10, height = 10, x = playerCenterX - width/2, y = playerCenterY - height/2, damage = playerDamage;
                        moveSpeed = 2;

                        for(int i = 0; i < (playerShots[0] + playerShots[1] + playerShots[2] + playerShots[3]); i++){

                            //Shots Angle Calculation -- START
                            if(playerShotsTemp[0] < playerShots[0]){ //UP
                                playerShotsTemp[0]++;
                                if(playerShots[0] != 1){
                                    angle = -0.125+(0.25/(playerShots[0]-1))*i;
                                }else{
                                    angle = 0;
                                }
                            }else if(playerShotsTemp[1] < playerShots[1]){ //RIGHT
                                playerShotsTemp[1]++;
                                if(playerShots[1] != 1){
                                    angle = 0.125+(0.25/(playerShots[1]-1))*(i-playerShotsTemp[0]);
                                }else{
                                    angle = 0.25;
                                }
                            }else if(playerShotsTemp[2] < playerShots[2]){ //DOWN
                                playerShotsTemp[2]++;
                                if(playerShots[2] != 1){
                                    angle = 0.375+(0.25/(playerShots[2]-1))*(i-(playerShotsTemp[0] + playerShotsTemp[1]));
                                }else{
                                    angle = 0.5;
                                }
                            }else if(playerShotsTemp[3] < playerShots[3]){ //LEFT
                                playerShotsTemp[3]++;
                                if(playerShots[3] != 1){
                                    angle = 0.625+(0.25/(playerShots[3]-1))*(i-(playerShotsTemp[0] + playerShotsTemp[1] + playerShotsTemp[2]));
                                }else{
                                    angle = 0.75;
                                }
                            }
                            //Shots Angle Calculation -- END

                            Bullet *newBullet = new Bullet();
                            newBullet->setPos(x, y);
                            newBullet->setDimensions(width, height);
                            newBullet->setDamage(damage);
                            newBullet->setAngleSpeed(moveSpeed, angle);
                            newBullet->setPlayerShot(true);

                            addBulletToList(newBullet);
                        }
                        al_play_sample(playerShotSound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                        playerShootingSpeedHelper = 0;
                    }

                    //Player Shooting Segment -- END

                    //Player Button Press Check -- START

                    if(al_key_down(&keyState, ALLEGRO_KEY_R) && lastKeyPress != ALLEGRO_KEY_R){
                        done = true;
                    }else if(al_key_down(&keyState, ALLEGRO_KEY_F) && lastKeyPress != ALLEGRO_KEY_F){
                        playerAutoShoot = !playerAutoShoot;
                        lastKeyPress = ALLEGRO_KEY_F;
                    }else if(al_key_down(&keyState, ALLEGRO_KEY_ESCAPE) && lastKeyPress != ALLEGRO_KEY_ESCAPE){
                        screenState = titleState;
                        lastKeyPress = ALLEGRO_KEY_ESCAPE;
                    }

                    //Player Button Press Check -- END

                    //Player Movement Calculation -- START

                    distanceX = (playerCenterX - mouseX >= 0) ? playerCenterX - mouseX : -(playerCenterX - mouseX);
                    distanceY = (playerCenterY - mouseY >= 0) ? playerCenterY - mouseY : -(playerCenterY - mouseY);

                    playerMoveSpeed = (distanceX + distanceY) * 0.1;

                    radiansToMouse = -atan2(playerCenterX - mouseX, playerCenterY - mouseY);

                    deltaX = sin(radiansToMouse) * playerMoveSpeed;
                    deltaY = -cos(radiansToMouse) * playerMoveSpeed;

                   //Player Movement Calculation -- END

                    //Position Updates -- START

                    if(isPassable(playerX+deltaX, playerY, playerWidth, playerHeight)){
                        playerX += deltaX;
                    }

                    if(isPassable(playerX, playerY+deltaY, playerWidth, playerHeight)){
                        playerY += deltaY;
                    }

                    for(int i = 0; i < MAX_BULLETS; i++){
                        if(bulletList[i] != NULL && bulletList[i]->checkActive()){
                            bulletList[i]->update();
                            if(bulletList[i]->playHitSound){
                                if(bulletList[i]->playerShot){
                                    al_play_sample(enemyHitSound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                                }else if(!bulletList[i]->playerShot){
                                    al_play_sample(playerHitSound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                                }
                            }
                        }
                    }

                    for(int i = 0; i < MAX_BUTTONS; i++){
                        if(buttonList[i] != NULL && buttonList[i]->checkActive()){
                            if(checkCollision(mouseX, mouseY, buttonList[i]->posX, buttonList[i]->posY, 0, 0, buttonList[i]->width, buttonList[i]->height)){
                                buttonList[i]->update();
                                if(buttonList[i]->playButtonPress){
                                    al_play_sample(buttonPressSound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                                }
                            }
                        }
                    }

                    if(resetShop){
                        resetShop = false;

                        playerShootingSpeed = shopResetValues[0];
                        playerHealth = shopResetValues[1];
                        playerMaxHealth = shopResetValues[2];
                        playerScore = shopResetValues[3];
                        playerDamage = shopResetValues[8];

                        for(int i = 0; i < 4; i++){
                            playerShots[i] = shopResetValues[i+4];
                        }
                    }

                    if(screenState != shopState){
                        initButtons = true;
                        shopMenu = false;
                        for(int i = 0; i < MAX_BUTTONS; i++){
                            buttonList[i] = NULL;
                        }
                    }

                    playerCenterX = playerX + playerWidth / 2;
                    playerCenterY = playerY + playerHeight / 2;

                    playerShootingSpeedHelper++;
                }

                if(!al_key_down(&keyState, lastKeyPress)){
                    lastKeyPress = NULL;
                }

                draw = true;
            }

            if(draw){
                draw = false;

                if(screenState == titleState){
                    for(int i = 0; i < MAX_BUTTONS; i++){
                        if(buttonList[i] != NULL && buttonList[i]->checkActive()){
                            buttonList[i]->draw();
                        }
                    }
                    al_draw_text(titleFont, al_map_rgb(122, 122, 122), screenWidth/2, screenHeight/2-250, ALLEGRO_ALIGN_CENTRE, "Game Title");

                }
                else if(screenState == gameState){

                    for(int i = 0; i < MAX_ENEMIES; i++){
                        if(enemyList[i] != NULL && enemyList[i]->checkActive()){
                            enemyList[i]->draw();
                        }
                    }
                    al_draw_filled_triangle(playerX + playerWidth/2, playerY, playerX + playerWidth, playerY + playerHeight, playerX, playerY + playerHeight, gray);
                    al_draw_rectangle(playerCenterX - playerHitBoxWidth/2, playerCenterY - playerHitBoxHeight/2, playerCenterX + playerHitBoxWidth/2, playerCenterY + playerHitBoxHeight/2, red, 1);
                    al_draw_line(playerCenterX, playerCenterY, mouseX, mouseY, blue, playerMoveSpeed*0.15);

                    for(int i = 0; i < MAX_BULLETS; i++){
                        if(bulletList[i] != NULL && bulletList[i]->checkActive()){
                            bulletList[i]->draw();
                        }
                    }

                    for(int i = 0; i < MAX_PICKUPS; i++){
                        if(pickupList[i] != NULL && pickupList[i]->checkActive()){
                            switch(pickupList[i]->pickupId){
                                case 0:
                                    al_draw_bitmap(healthPickup, pickupList[i]->posX, pickupList[i]->posY, NULL);
                                    break;

                                case 1:
                                    al_draw_bitmap(healthPickupUpgrade, pickupList[i]->posX, pickupList[i]->posY, NULL);
                                    break;

                                case 2:
                                    al_draw_bitmap(weaponPickupUp, pickupList[i]->posX, pickupList[i]->posY, NULL);
                                    break;

                                case 3:
                                    al_draw_bitmap(weaponPickupRight, pickupList[i]->posX, pickupList[i]->posY, NULL);
                                    break;

                                case 4:
                                    al_draw_bitmap(weaponPickupDown, pickupList[i]->posX, pickupList[i]->posY, NULL);
                                    break;

                                case 5:
                                    al_draw_bitmap(weaponPickupLeft, pickupList[i]->posX, pickupList[i]->posY, NULL);
                                    break;

                                case 6:
                                    al_draw_bitmap(weaponPickupUpgrade, pickupList[i]->posX, pickupList[i]->posY, NULL);
                                    break;
                            }
                        }
                    }

                    //Draw Player Stats -- START
                    playerDPS = (playerShots[0]+playerShots[1]+playerShots[2]+playerShots[3])*playerDamage*(FPS/playerShootingSpeed);

                    al_draw_textf(defaultFont, al_map_rgb(122, 122, 122), screenWidth-200, screenHeight-80, NULL, "Score: %d", playerScore);
                    al_draw_textf(defaultFont, al_map_rgb(122, 122, 122), screenWidth-400, screenHeight-80, NULL, "DPS: %f", playerDPS);
                    al_draw_textf(defaultFont, al_map_rgb(122, 122, 122), screenWidth-800, screenHeight-80, NULL, "Time: %d", enemyStatsBooster);

                    {
                        int goldFilledHealths = 0, silverFilledHealths = 0, normalFilledHealths = 0;
                        int goldDepletedHealths = 0, silverDepletedHealths = 0, normalDepletedHealths = 0;

                        goldFilledHealths = floor(playerHealth/10);
                        silverFilledHealths = floor((playerHealth-goldFilledHealths*10)/5);
                        normalFilledHealths = floor(playerHealth-goldFilledHealths*10-silverFilledHealths*5);

                        goldDepletedHealths = floor((playerMaxHealth-playerHealth)/10);
                        silverDepletedHealths = floor(((playerMaxHealth-playerHealth)-goldDepletedHealths*10)/5);
                        normalDepletedHealths = floor((playerMaxHealth-playerHealth)-goldDepletedHealths*10-silverDepletedHealths*5);

                        for(int i = 0; i < 10; i++){
                            if(goldFilledHealths >= i+1){
                                al_draw_filled_rectangle(i*20 + 20, screenHeight-50, i*20 + 30, screenHeight-40, gold);

                            }else if(silverFilledHealths+goldFilledHealths >= i+1){
                                al_draw_filled_rectangle(i*20 + 20, screenHeight-50, i*20 + 30, screenHeight-40, silver);

                            }else if(normalFilledHealths+silverFilledHealths+goldFilledHealths >= i+1){
                                al_draw_filled_rectangle(i*20 + 20, screenHeight-50, i*20 + 30, screenHeight-40, red);

                            }
                        }

                        for(int i = 0; i < 10; i++){
                            if(goldDepletedHealths >= i+1){
                                al_draw_rectangle(i*20 + 20 + 0.5, screenHeight-25 + 0.5, i*20 + 30 - 0.5, screenHeight-15 - 0.5, gold, 2);

                            }else if(silverDepletedHealths+goldDepletedHealths >= i+1){
                                al_draw_rectangle(i*20 + 20 + 0.5, screenHeight-25 + 0.5, i*20 + 30 - 0.5, screenHeight-15 - 0.5, silver, 2);

                            }else if(normalDepletedHealths+silverDepletedHealths+goldDepletedHealths >= i+1){
                                al_draw_rectangle(i*20 + 20 + 0.5, screenHeight-25 + 0.5, i*20 + 30 - 0.5, screenHeight-15 - 0.5, red, 2);

                            }
                        }
                    }
                    //Draw Player Stats -- END

                }
                else if(screenState == loseState){
                    al_draw_text(defaultFont, al_map_rgb(122, 122, 122), screenWidth/2, screenHeight/2-15, ALLEGRO_ALIGN_CENTRE, "GAME OVER");
                    al_draw_text(defaultFont, al_map_rgb(122, 122, 122), screenWidth/2, screenHeight/2+15, ALLEGRO_ALIGN_CENTRE, "Press Enter To Continue");
                    al_draw_textf(defaultFont, al_map_rgb(122, 122, 122), screenWidth/2, screenHeight/2+45, ALLEGRO_ALIGN_CENTRE, "Score: %d", playerScore);
                    al_draw_textf(defaultFont, al_map_rgb(122, 122, 122), screenWidth/2, screenHeight/2+75, ALLEGRO_ALIGN_CENTRE, "DPS: %f", playerDPS);
                }
                else if(screenState == shopState){
                    al_draw_filled_triangle(playerX + playerWidth/2, playerY, playerX + playerWidth, playerY + playerHeight, playerX, playerY + playerHeight, gray);
                    al_draw_rectangle(playerCenterX - playerHitBoxWidth/2, playerCenterY - playerHitBoxHeight/2, playerCenterX + playerHitBoxWidth/2, playerCenterY + playerHitBoxHeight/2, red, 1);

                    for(int i = 0; i < MAX_BULLETS; i++){
                        if(bulletList[i] != NULL && bulletList[i]->checkActive()){
                            bulletList[i]->draw();
                        }
                    }

                    for(int i = 0; i < MAX_BUTTONS; i++){
                        if(buttonList[i] != NULL && buttonList[i]->checkActive()){
                            buttonList[i]->draw();
                        }
                    }

                    {
                        int goldFilledHealths = 0, silverFilledHealths = 0, normalFilledHealths = 0;
                        int goldDepletedHealths = 0, silverDepletedHealths = 0, normalDepletedHealths = 0;

                        goldFilledHealths = floor(playerHealth/10);
                        silverFilledHealths = floor((playerHealth-goldFilledHealths*10)/5);
                        normalFilledHealths = floor(playerHealth-goldFilledHealths*10-silverFilledHealths*5);

                        goldDepletedHealths = floor((playerMaxHealth-playerHealth)/10);
                        silverDepletedHealths = floor(((playerMaxHealth-playerHealth)-goldDepletedHealths*10)/5);
                        normalDepletedHealths = floor((playerMaxHealth-playerHealth)-goldDepletedHealths*10-silverDepletedHealths*5);

                        for(int i = 0; i < 10; i++){
                            if(goldFilledHealths >= i+1){
                                al_draw_filled_rectangle(i*20 + 20, screenHeight-50, i*20 + 30, screenHeight-40, gold);

                            }else if(silverFilledHealths+goldFilledHealths >= i+1){
                                al_draw_filled_rectangle(i*20 + 20, screenHeight-50, i*20 + 30, screenHeight-40, silver);

                            }else if(normalFilledHealths+silverFilledHealths+goldFilledHealths >= i+1){
                                al_draw_filled_rectangle(i*20 + 20, screenHeight-50, i*20 + 30, screenHeight-40, red);

                            }
                        }

                        for(int i = 0; i < 10; i++){
                            if(goldDepletedHealths >= i+1){
                                al_draw_rectangle(i*20 + 20 + 0.5, screenHeight-25 + 0.5, i*20 + 30 - 0.5, screenHeight-15 - 0.5, gold, 2);

                            }else if(silverDepletedHealths+goldDepletedHealths >= i+1){
                                al_draw_rectangle(i*20 + 20 + 0.5, screenHeight-25 + 0.5, i*20 + 30 - 0.5, screenHeight-15 - 0.5, silver, 2);

                            }else if(normalDepletedHealths+silverDepletedHealths+goldDepletedHealths >= i+1){
                                al_draw_rectangle(i*20 + 20 + 0.5, screenHeight-25 + 0.5, i*20 + 30 - 0.5, screenHeight-15 - 0.5, red, 2);

                            }
                        }
                    }

                    al_draw_textf(defaultFont, al_map_rgb(122, 122, 122), screenWidth-200, screenHeight-80, NULL, "Score: %d", playerScore);
                }

                fpsTimeNew = al_get_time();
                fpsCounter = 1/(fpsTimeNew - fpsTimeOld);
                fpsTimeOld = fpsTimeNew;

                al_draw_textf(defaultFont, al_map_rgb(122, 122, 122), screenWidth-600, screenHeight-80, NULL, "FPS: %f", fpsCounter);

                al_draw_bitmap(cursorImage, mouseX - al_get_bitmap_width(cursorImage)/2, mouseY - al_get_bitmap_height(cursorImage)/2, NULL);

                al_flip_display();
                al_clear_to_color(al_map_rgb(0, 0, 0));
            }
        }
    }

    //Pointer Destruction -- START
    al_destroy_display(display);
    al_destroy_bitmap(cursorImage);
    al_destroy_bitmap(enemySeeker);
    al_destroy_bitmap(healthPickup);
    al_destroy_bitmap(healthPickupUpgrade);
    al_destroy_bitmap(weaponPickupUp);
    al_destroy_bitmap(weaponPickupDown);
    al_destroy_bitmap(weaponPickupLeft);
    al_destroy_bitmap(weaponPickupRight);
    al_destroy_bitmap(weaponPickupUpgrade);
    al_destroy_font(defaultFont);
    al_destroy_font(titleFont);
    al_destroy_sample(playerShotSound);
    al_destroy_sample(playerDeathSound);
    al_destroy_sample(enemyShotSound);
    al_destroy_sample(enemyDeathSound);
    al_destroy_sample(enemyHitSound);
    al_destroy_sample(pickupSound);
    al_destroy_sample(playerHitSound);
    al_destroy_sample(buttonPressSound);
    al_destroy_event_queue(event_queue);
    al_destroy_timer(timer);
    //Pointer Destruction -- END
}

bool addEnemyToList(Enemy *newEnemy){
    for(int i = 0; i < MAX_ENEMIES; i++){
        if(enemyList[i] == NULL || !enemyList[i]->checkActive()){
            enemyList[i] = newEnemy;
            return true;
        }
    }
    return false;
}

bool addBulletToList(Bullet *newBullet){
    for(int i = 0; i < MAX_BULLETS; i++){
        if(bulletList[i] == NULL || !bulletList[i]->checkActive()){
            bulletList[i] = newBullet;
            return true;
        }
    }
    return false;
}

bool addButtonToList(Button *newButton){
    for(int i = 0; i < MAX_BUTTONS; i++){
        if(buttonList[i] == NULL || !buttonList[i]->checkActive()){
            buttonList[i] = newButton;
            return true;
        }
    }
    return false;
}

bool addPickupToList(Pickup *newPickup){
    for(int i = 0; i < MAX_PICKUPS; i++){
        if(pickupList[i] == NULL || !pickupList[i]->checkActive()){
            pickupList[i] = newPickup;
            return true;
        }
    }
    return false;
}


bool isPassable(int x, int y, int width, int height){
    int startPos[2] = {0,0};
    if(screenState == gameState){
        if(x < startPos[0] || x+width > mapWidth+startPos[0] || y < startPos[1] || y+height > mapHeight+startPos[1]){
            return false;
        }
    }else if(screenState == shopState){
        startPos[0] = 100, startPos[1] = screenHeight-shopTestHeight-100;
        if(x < startPos[0] || x+width > shopTestWidth+startPos[0] || y < startPos[1] || y+height > shopTestHeight+startPos[1]){
            return false;
        }
    }
    return true;
}

bool checkCollision(int x, int y, int ex, int ey, int width, int height, int ewidth, int eheight){
    if(x + width < ex || x > ewidth + ex || y + height < ey || y > eheight + ey){
        return false;
    }else{
        return true;
    }
}
