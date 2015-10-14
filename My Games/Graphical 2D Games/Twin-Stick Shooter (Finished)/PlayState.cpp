#include "Engine.h"
#include "MenuState.h"
#include "GameState.h"
#include "PlayState.h"
#include "EditorState.h"
#include "ShopState.h"

PlayState PlayState::m_PlayState;

void PlayState::init(){
    playerColX = false, playerColY = false;

    pauseBetweenWavesCheck = true, enemySpawnAll = false;
    playerX = 0, playerY = 0, playerWidth = 16, playerHeight = 16, playerCenterX = playerX + playerWidth/2, playerCenterY = playerY + playerHeight/2, playerMovementSpeed = 2, playerDeltaX = 0, playerDeltaX = 0;
    playerHealth = 3, playerMaxHealth = 3;
    playerFiringSpeed = 60, playerFiringSpeedHelper = 0, playerShots = 1, playerDamage = 1, playerMoney = 0;
    playerBulletSpeed = 4, playerBulletSpread = 0.5;
    enemySpawnRate = 120, enemySpawnRateHelper = 0;
    enemyHealth = 1, enemySpeed = 1, enemyDamage = 1, enemyAttackTime = 20, enemyKillMoney = 1;
    enemyWave = 1, amountEnemiesSpawn = 10, amountEnemiesSpawned = 0, pauseBetweenWaves = 600, pauseBetweenWavesHelper = 0;

    for(int i = 0; i < MAX_BULLETS; i++)
        bulletList[i] = NULL;

    for(int i = 0; i < MAX_ZOMBIES; i++)
        zombieList[i] = NULL;

    loadMapArray();

    this->paused = false;

	printf("PlayState Init\n");
}

void PlayState::cleanup(){
    for(int i = 0; i < MAX_BULLETS; i++)
        bulletList[i] = NULL;

    for(int i = 0; i < MAX_ZOMBIES; i++)
        zombieList[i] = NULL;

	printf("PlayState Cleanup\n");
}

void PlayState::pause(){
	printf("PlayState Pause\n");
}

void PlayState::resume(){
	printf("PlayState Resume\n");
}

void PlayState::update(Engine* engine){
    ALLEGRO_EVENT events;
    al_wait_for_event(event_queue, &events);

    if(events.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
        engine->quit();
    }

    if(events.type == ALLEGRO_EVENT_TIMER){

        al_get_mouse_state(&mouseState);

        al_get_keyboard_state(&keyState);

        //Enemy Spawning +

        if(amountEnemiesSpawned >= amountEnemiesSpawn){
            bool check = false;
            for(int i = 0; i < MAX_ZOMBIES; i++){
                if(zombieList[i] != NULL && zombieList[i]->checkActive()){
                    check = true;
                }
            }
            if(!check){
                pauseBetweenWavesCheck = true;
            }
        }

        if(enemySpawnRateHelper >= enemySpawnRate && !pauseBetweenWavesCheck && amountEnemiesSpawned < amountEnemiesSpawn){
            int amountSpawnPoints = 0;
            for(int l_x = 0; l_x < mapWidth/tileSize; l_x++){
                for(int l_y = 0; l_y < mapHeight/tileSize; l_y++){
                    if(mapArray[l_x][l_y] == 7){
                        amountSpawnPoints++;
                    }
                }
            }
            int loopValue = (enemySpawnAll) ? amountSpawnPoints : 1;
            for(int i = 0; i < loopValue; i++){
                float x = 0, y = 0, width = 0, height = 0, health = 0, moveSpeed = 0, damage = 0;
                int zombieId = 0, randomSpawnpointHelper = 0, random = rand(), attackTime = 0;
                bool active = true;

                width = 16, height = 16;
                health = enemyHealth, moveSpeed = enemySpeed, damage = enemyDamage, attackTime = enemyAttackTime;

                for(int l_x = 0; l_x < mapWidth/tileSize; l_x++){
                    for(int l_y = 0; l_y < mapHeight/tileSize; l_y++){
                        if(mapArray[l_x][l_y] == 7){
                            if(random % amountSpawnPoints == randomSpawnpointHelper){
                                do{
                                    x = l_x*tileSize + (1 - rand() % 3)*tileSize;
                                    y = l_y*tileSize + (1 - rand() % 3)*tileSize;
                                }while(!isPassable(x, y, width, height, 0, 0));
                            }
                            randomSpawnpointHelper++;
                        }
                    }
                }

                for(int i = 0; i < MAX_ZOMBIES; i++){
                    if(zombieList[i] != NULL && zombieList[i]->checkActive()){
                        if(checkCollision(x, y, zombieList[i]->posX, zombieList[i]->posY, width, height, zombieList[i]->width, zombieList[i]->height)){
                            active = false;
                        }
                    }
                }

                if(checkCollision(x, y, playerX, playerY, width, height, playerWidth, playerHeight)){
                    active = false;
                }

                Zombie *newZombie = new Zombie();
                newZombie->setPos(x, y);
                newZombie->setDimensions(width, height);
                newZombie->setZombieId(zombieId);
                newZombie->setHealth(health);
                newZombie->setMaxHealth(health);
                newZombie->setSpeed(moveSpeed);
                newZombie->setDamage(damage);
                newZombie->setAttackTime(attackTime);
                newZombie->setActive(active);

                addZombieToList(newZombie);

                enemySpawnRateHelper = 0;
            }
            amountEnemiesSpawned += 1;
        }

        //Enemy Spawning -

        //Update Mouse Variables +
        mouseX = al_get_mouse_state_axis(&mouseState,0);
        mouseY = al_get_mouse_state_axis(&mouseState,1);

        if(al_mouse_button_down(&mouseState, 1)){
            if(!mouseButtonLeft)
                mouseButtonLeftClick = true;
            else
                mouseButtonLeftClick = false;
            mouseButtonLeft = true;
        }else{
            mouseButtonLeft = false;
        }
        //Update Mouse Variables -

        //Player Input +
        if(al_key_down(&keyState, ALLEGRO_KEY_W)){
            playerDeltaY = -playerMovementSpeed;
        }else if(al_key_down(&keyState, ALLEGRO_KEY_S)){
            playerDeltaY = playerMovementSpeed;
        }if(al_key_down(&keyState, ALLEGRO_KEY_A)){
            playerDeltaX = -playerMovementSpeed;
        }else if(al_key_down(&keyState, ALLEGRO_KEY_D)){
            playerDeltaX = playerMovementSpeed;
        }

        if(al_key_down(&keyState, ALLEGRO_KEY_E)){
            if(lastKeyPress != ALLEGRO_KEY_E){
                switch(useMapTile(playerX, playerY, playerWidth, playerHeight)){
                    case 8:
                        engine->pushState(ShopState::instance());
                }
                lastKeyPress = ALLEGRO_KEY_E;
            }
        }else if(al_key_down(&keyState, ALLEGRO_KEY_F5)){
            if(lastKeyPress != ALLEGRO_KEY_F5){
                loadMapArray();
                lastKeyPress = ALLEGRO_KEY_F5;
            }
        }else if(al_key_down(&keyState, ALLEGRO_KEY_ESCAPE)){
            if(lastKeyPress != ALLEGRO_KEY_ESCAPE){
                inGame = true;
                engine->pushState(MenuState::instance());
                lastKeyPress = ALLEGRO_KEY_ESCAPE;
            }
        }
        //Player Input -

        //Update Player Variables +
        if(playerHealth <= 0){
            engine->changeState(MenuState::instance());
        }

        if(mouseButtonLeft){
            if(playerFiringSpeedHelper >= playerFiringSpeed){
                playerFiringSpeedHelper = 0;

                int x, y, width, height, damage;
                float moveSpeed, angle = 0;

                width = 8, height = 8, x = playerCenterX - width/2, y = playerCenterY - height/2, damage = playerDamage;
                moveSpeed = playerBulletSpeed;

                for(int i = 0; i < playerShots; i++){
                    //Shots Angle Calculation -- START
                    if(playerShots != 1){
                        angle = (-atan2(playerCenterX - mouseX, playerCenterY - mouseY)) + (-playerBulletSpread/2+(playerBulletSpread/(playerShots-1))*i);
                    }else{
                        angle = -atan2(playerCenterX - mouseX, playerCenterY - mouseY);
                    }

                    int spread = round(playerBulletSpread*100);

                    angle += (-spread/2+rand()%spread)/100.0;

                    //Shots Angle Calculation -- END

                    Bullet *newBullet = new Bullet();
                    newBullet->setPos(x, y);
                    newBullet->setDimensions(width, height);
                    newBullet->setDamage(damage);
                    newBullet->setAngleSpeed(moveSpeed, angle);
                    newBullet->setPlayerShot(true);

                    addBulletToList(newBullet);
                }
            }
        }

        playerColX = false, playerColY = false;
        for(int i = 0; i < MAX_ZOMBIES; i++){
            if(zombieList[i] != NULL && zombieList[i]->checkActive()){
                if(checkCollision(playerX+playerDeltaX, playerY, zombieList[i]->posX, zombieList[i]->posY, playerWidth, playerHeight, zombieList[i]->width, zombieList[i]->height)){
                    playerColX = true;
                }
                if(checkCollision(playerX, playerY+playerDeltaY, zombieList[i]->posX, zombieList[i]->posY, playerWidth, playerHeight, zombieList[i]->width, zombieList[i]->height)){
                    playerColY = true;
                }
            }
        }

        if(isPassable(playerX + playerDeltaX, playerY, playerWidth, playerHeight, playerDeltaX, playerDeltaY) && !playerColX){
            playerX += playerDeltaX;
        }
        if(isPassable(playerX, playerY + playerDeltaY, playerWidth, playerHeight, playerDeltaX, playerDeltaY) && !playerColY){
            playerY += playerDeltaY;
        }

        playerCenterX = playerX + playerWidth/2, playerCenterY = playerY + playerHeight/2;

        playerDeltaX = 0;
        playerDeltaY = 0;
        //Update Player Variables -

        //Update Entities +
        for(int i = 0; i < MAX_BULLETS; i++){
            if(bulletList[i] != NULL && bulletList[i]->checkActive()){
                bulletList[i]->update();
            }
        }

        for(int i = 0; i < MAX_ZOMBIES; i++){
            if(zombieList[i] != NULL && zombieList[i]->checkActive()){
                zombieList[i]->update();
            }
        }
        //Update Entities -

        //Rest +
        if(!al_key_down(&keyState, lastKeyPress)){
            lastKeyPress = NULL;
        }

        if(pauseBetweenWavesCheck){
            pauseBetweenWavesHelper++;
            if(pauseBetweenWavesHelper >= pauseBetweenWaves){
                pauseBetweenWavesHelper = 0;
                pauseBetweenWavesCheck = false;
                if(enemyWave != 1){
                    nextWave();
                }else{
                    enemyWave++;
                }
            }
        }

        playerFiringSpeedHelper++;
        enemySpawnRateHelper++;
        //Rest -
        engine->draw();
    }
}

void PlayState::draw(Engine* engine){
    //Draw Map+
    drawMap();
    //Draw Map-

    //Draw Entities +
    for(int i = 0; i < MAX_ZOMBIES; i++){
        if(zombieList[i] != NULL && zombieList[i]->checkActive()){
            zombieList[i]->draw();
        }
    }

    al_draw_rotated_bitmap(playerImage, playerWidth/2, playerHeight/2, playerX+playerWidth/2, playerY+playerHeight/2, -atan2(playerCenterX - mouseX, playerCenterY - mouseY), NULL);

    for(int i = 0; i < MAX_BULLETS; i++){
        if(bulletList[i] != NULL && bulletList[i]->checkActive()){
            bulletList[i]->draw();
        }
    }
    //Draw Entities -

    //Draw GUI +
    al_draw_rectangle(52.5, mapHeight+7.5, 447.5, mapHeight+52.5, al_map_rgb(195,0,0), 5);
    al_draw_filled_rectangle(55, mapHeight+10, 45+((400/playerMaxHealth)*playerHealth), mapHeight+50, al_map_rgb(255,50,50));

    al_draw_textf(defaultFont, al_map_rgb(50, 150, 50), screenWidth-375, mapHeight+20, NULL, "%d$", playerMoney);
    //Draw GUI -

    fpsTimeNew = al_get_time();
    fpsCounter = 1/(fpsTimeNew - fpsTimeOld);
    fpsTimeOld = fpsTimeNew;

    al_draw_textf(defaultFont, al_map_rgb(122, 122, 122), screenWidth-175, mapHeight+20, NULL, "FPS: %f", round(fpsCounter*10)/10);

    al_draw_textf(defaultFont, al_map_rgb(122, 122, 122), 500, mapHeight+20, NULL, "Wave %d", enemyWave);

    al_draw_textf(defaultFont, al_map_rgb(122, 122, 122), 650, mapHeight+20, NULL, "Enemies Left: %d", amountEnemiesSpawn - amountEnemiesSpawned);

    if(pauseBetweenWavesCheck){
        al_draw_textf(defaultFont, al_map_rgb(122, 122, 122), 500, screenHeight-40, NULL, "Next Wave In %f Seconds", pauseBetweenWaves/FPS-pauseBetweenWavesHelper/FPS);
    }

    if(pauseBetweenWavesCheck){
        if(pauseBetweenWaves-pauseBetweenWavesHelper <= 180){
            if(pauseBetweenWaves-pauseBetweenWavesHelper <= 180 && pauseBetweenWaves-pauseBetweenWavesHelper > 120){

                al_draw_text(waveFont, al_map_rgba(0, 0, 0, (pauseBetweenWaves-pauseBetweenWavesHelper-120)*2), screenWidth/2-20, mapHeight/3-20, ALLEGRO_ALIGN_CENTER, "3");

            }else if(pauseBetweenWaves-pauseBetweenWavesHelper <= 120 && pauseBetweenWaves-pauseBetweenWavesHelper > 60){

                al_draw_text(waveFont, al_map_rgba(0, 0, 0, (pauseBetweenWaves-pauseBetweenWavesHelper-60)*2), screenWidth/2-20, mapHeight/3-20, ALLEGRO_ALIGN_CENTER, "2");

            }else if(pauseBetweenWaves-pauseBetweenWavesHelper <= 60){

                al_draw_text(waveFont, al_map_rgba(0, 0, 0, (pauseBetweenWaves-pauseBetweenWavesHelper)*2), screenWidth/2-20, mapHeight/3-20, ALLEGRO_ALIGN_CENTER, "1");
            }
        }
    }
}
