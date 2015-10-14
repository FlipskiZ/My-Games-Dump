#include "Engine.h"
#include "MenuState.h"
#include "GameState.h"
#include "PlayState.h"

PlayState PlayState::m_PlayState;

void PlayState::init(){

    //Create Player
    int width = 24, height = 24, movementSpeed = 4, sheetColums = 4, sheetRows = 2, animationSpeed = 60/movementSpeed;

    playerEntity = new Player();
    playerEntity->setPos(0, 0);
    playerEntity->setDimensions(width, height);
    playerEntity->setMovementSpeed(movementSpeed);
    playerEntity->setSheetDimensions(sheetColums, sheetRows, width, height);
    playerEntity->setAnimationSpeed(animationSpeed);
    playerEntity->setBitmap(playerImage);

    loadMapArray();

	printf("PlayState Init\n");
}

void PlayState::cleanup(){
	printf("PlayState Cleanup\n");
}

void PlayState::pause(){
    inGame = true;
	printf("PlayState Pause\n");
}

void PlayState::resume(){
	printf("PlayState Resume\n");
}

void PlayState::update(Engine* engine){

    if(timerEvent){
        al_get_mouse_state(&mouseState);

        al_get_keyboard_state(&keyState);

        //Player Input +
        float movementSpeed = playerEntity->movementSpeed;

        playerEntity->animationValue = 0;

        int horizontal = 0, vertical = 0;

        if(al_key_down(&keyState, ALLEGRO_KEY_W)){
            playerEntity->deltaY = -movementSpeed;
            playerEntity->animationValue = 1;
            vertical = 1;
        }else if(al_key_down(&keyState, ALLEGRO_KEY_S)){
           playerEntity->deltaY = movementSpeed;
           playerEntity->animationValue = 1;
           vertical = 2;
        }
        if(al_key_down(&keyState, ALLEGRO_KEY_A)){
            playerEntity->deltaX = -movementSpeed;
            playerEntity->animationValue = 1;
            horizontal = 1;
        }else if(al_key_down(&keyState, ALLEGRO_KEY_D)){
            playerEntity->deltaX = movementSpeed;
            playerEntity->animationValue = 1;
            horizontal = 2;
        }

        if(!mouseButtonRight){
            if(vertical > 0){
                if(horizontal == 0){
                    playerEntity->angle = (vertical-1)*180*(PI*2/360);
                }else{
                    if(vertical == 1){
                        playerEntity->angle = (horizontal == 1) ? 315*(PI*2/360) : 45*(PI*2/360);
                    }else if(vertical == 2){
                        playerEntity->angle = (horizontal == 1) ? 225*(PI*2/360) : 135*(PI*2/360);
                    }
                }
            }else if(horizontal > 0){
                playerEntity->angle = 270*(PI*2/360)+(horizontal-1)*180*(PI*2/360);
            }
        }else{
            playerEntity->angle = -atan2(playerEntity->centerX - mouseX + cameraOffsetX, playerEntity->centerY - mouseY + cameraOffsetY);
            playerEntity->animationValue = 2;
            if(mouseButtonLeftClick){
                playerEntity->fireBullet();
            }
        }

        if(al_key_down(&keyState, ALLEGRO_KEY_F5)){
            if(lastKeyPress != ALLEGRO_KEY_F5){
                loadMapArray();
                lastKeyPress = ALLEGRO_KEY_F5;
            }
        }else if(al_key_down(&keyState, ALLEGRO_KEY_ESCAPE)){
            if(lastKeyPress != ALLEGRO_KEY_ESCAPE){
                engine->pushState(MenuState::instance());
                lastKeyPress = ALLEGRO_KEY_ESCAPE;
            }
        }else if(al_key_down(&keyState, ALLEGRO_KEY_F)){
            if(lastKeyPress != ALLEGRO_KEY_F){
                float width = 24, height = 24, movementSpeed = 2, sheetColums = 4, sheetRows = 3, animationSpeed = 60/movementSpeed;

                Zombie *newZombie = new Zombie();
                newZombie->setPos(mouseX - cameraOffsetX - width/2, mouseY - cameraOffsetY - height/2);
                newZombie->setDimensions(width, height);
                newZombie->setMovementSpeed(movementSpeed);
                newZombie->setSheetDimensions(sheetColums, sheetRows, width, height);
                newZombie->setAnimationSpeed(animationSpeed);
                newZombie->setBitmap(zombieImage);
                addLivingToList(newZombie);

                lastKeyPress = ALLEGRO_KEY_F;
            }
        }
        //Player Input -

        //Update Entities +
        playerEntity->update();
        for(int i = 0; i < MAX_LIVING; i++){
            if(livingList[i] != NULL && livingList[i]->checkActive()){
                livingList[i]->update();
            }
        }
        for(int i = 0; i < MAX_MISSILES; i++){
            if(missileList[i] != NULL && missileList[i]->checkActive()){
                missileList[i]->update();
            }
        }
        for(int i = 0; i < MAX_DEBUG; i++){
            if(debugList[i] != NULL && debugList[i]->checkActive()){
                debugList[i]->update();
            }
        }
        //Update Entities -

        //Rest +
        updateCamera();
        //Rest -
    }
}

void PlayState::draw(Engine* engine){
    //Draw Map+
    drawMap();
    //Draw Map-

    //Draw Entities +
    playerEntity->draw();
    for(int i = 0; i < MAX_LIVING; i++){
        if(livingList[i] != NULL && livingList[i]->checkActive()){
            livingList[i]->draw();
        }
    }
    for(int i = 0; i < MAX_MISSILES; i++){
        if(missileList[i] != NULL && missileList[i]->checkActive()){
            missileList[i]->draw();
        }
    }
    for(int i = 0; i < MAX_DEBUG; i++){
        if(debugList[i] != NULL && debugList[i]->checkActive()){
            debugList[i]->draw();
        }
    }
    //Draw Entities -

    //Draw GUI +
    al_draw_filled_rectangle(0, mapDisplayHeight, screenWidth, screenHeight, al_map_rgb(0,0,0));

    fpsTimeNew = al_get_time();
    fpsCounter = 1/(fpsTimeNew - fpsTimeOld);
    fpsTimeOld = fpsTimeNew;

    al_draw_textf(defaultFont, al_map_rgb(122, 122, 122), screenWidth-175, screenHeight-25, NULL, "FPS: %f", round(fpsCounter*10)/10);
    //Draw GUI -
}
