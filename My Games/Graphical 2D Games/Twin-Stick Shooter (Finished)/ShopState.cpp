#include "Engine.h"
#include "ShopState.h"
#include "GameState.h"
#include "PlayState.h"
#include "EditorState.h"
#include "ShopState.h"

ShopState ShopState::m_ShopState;

void ShopState::init(){
    for(int i = 0; i < 6; i++){
        float x = 0, y = 0, width = 0, height = 0;
        int buttonId = i+4;

        width = 256, height = 64;

        x = screenWidth-384, y = 64+96*i;

        Button *newButton = new Button();
        newButton->setPos(x, y);
        newButton->setDimensions(width, height);
        newButton->setButtonId(buttonId);

        addButtonToList(newButton);
    }

    for(int i = 0; i < 2; i++){
        float x = 0, y = 0, width = 0, height = 0;
        int buttonId = i+10;

        width = 256, height = 64;

        x = screenWidth-640, y = 64+96*i;

        Button *newButton = new Button();
        newButton->setPos(x, y);
        newButton->setDimensions(width, height);
        newButton->setButtonId(buttonId);

        addButtonToList(newButton);
    }

    for(int i = 0; i < MAX_BUTTONS; i++)
        bulletList[i] = NULL;

    inShop = true;

    printf("ShopState Init\n");
}
void ShopState::cleanup(){
    for(int i = 0; i < MAX_BUTTONS; i++)
        buttonList[i] = NULL;

    for(int i = 0; i < MAX_BUTTONS; i++)
        bulletList[i] = NULL;

    inShop = false;
    printf("ShopState Cleanup\n");
}

void ShopState::pause(){
    inShop = false;
    printf("ShopState Pause\n");
}
void ShopState::resume(){
    inShop = true;
    printf("ShopState Resume\n");
}

void ShopState::update(Engine* engine){
    ALLEGRO_EVENT events;
    al_wait_for_event(event_queue, &events);

    if(events.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
        engine->quit();
    }

    if(events.type == ALLEGRO_EVENT_TIMER){

        al_get_mouse_state(&mouseState);

        al_get_keyboard_state(&keyState);

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

        if(al_key_down(&keyState, ALLEGRO_KEY_ESCAPE)){
            if(lastKeyPress != ALLEGRO_KEY_ESCAPE){
                engine->popState();
                lastKeyPress = ALLEGRO_KEY_ESCAPE;
            }
        }
        //Player Input -

        //Update Player Variables +

        if(mouseButtonLeft){
            if(playerFiringSpeedHelper >= playerFiringSpeed){
                playerFiringSpeedHelper = 0;

                int x, y, width, height, damage;
                float moveSpeed, angle = 0;

                width = 8, height = 8, x = playerCenterX - width/2, y = playerCenterY - height/2, damage = 1;
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

        if(playerX+playerDeltaX >= 0 && playerX+playerDeltaX + playerWidth < 400 && playerY >= 0 && playerY + playerHeight < 400){
            playerX += playerDeltaX;
        }
        if(playerX >= 0 && playerX + playerWidth < 400 && playerY+playerDeltaY >= 0 && playerY+playerDeltaY + playerHeight < 400){
            playerY += playerDeltaY;
        }

        playerCenterX = playerX + playerWidth/2, playerCenterY = playerY + playerHeight/2;

        playerDeltaX = 0;
        playerDeltaY = 0;
        //Update Player Variables -

        //Update Entities +
        for(int i = 0; i < MAX_BUTTONS; i++){
            if(buttonList[i] != NULL && buttonList[i]->checkActive()){
                buttonList[i]->update();
                if(buttonList[i]->clicked){
                    switch(buttonList[i]->buttonId){
                        case 0:
                            engine->popState();
                            break;

                        case 1:
                            engine->changeState(PlayState::instance());
                            break;

                        case 2:
                            engine->changeState(EditorState::instance());
                            break;

                        case 3:
                            engine->quit();
                            break;
                    }
                }
            }
        }

        for(int i = 0; i < MAX_BULLETS; i++){
            if(bulletList[i] != NULL && bulletList[i]->checkActive()){
                bulletList[i]->update();
            }
        }
        //Update Entities -

        //Rest +
        if(!al_key_down(&keyState, lastKeyPress)){
            lastKeyPress = NULL;
        }

        playerFiringSpeedHelper++;
        //Rest -

        engine->draw();
    }
}
void ShopState::draw(Engine* engine){

    for(int i = 0; i < MAX_BUTTONS; i++){
        if(buttonList[i] != NULL && buttonList[i]->checkActive()){
            buttonList[i]->draw();
        }
    }

    al_draw_rotated_bitmap(playerImage, playerWidth/2, playerHeight/2, playerX+playerWidth/2, playerY+playerHeight/2, -atan2(playerCenterX - mouseX, playerCenterY - mouseY), NULL);

    for(int i = 0; i < MAX_BULLETS; i++){
        if(bulletList[i] != NULL && bulletList[i]->checkActive()){
            bulletList[i]->draw();
        }
    }

    al_draw_textf(defaultFont, al_map_rgb(50, 150, 50), 0, 400, NULL, "%d$", playerMoney);
    al_draw_textf(defaultFont, al_map_rgb(122, 122, 122), 0, 400+32, NULL, "Health: %f", playerHealth);
    al_draw_textf(defaultFont, al_map_rgb(122, 122, 122), 0, 400+32*2, NULL, "Max Health: %f", playerMaxHealth);
    al_draw_textf(defaultFont, al_map_rgb(122, 122, 122), 0, 400+32*3, NULL, "Speed: %f", playerBulletSpeed);
    al_draw_textf(defaultFont, al_map_rgb(122, 122, 122), 0, 400+32*4, NULL, "Bullets: %d", playerShots);
    al_draw_textf(defaultFont, al_map_rgb(122, 122, 122), 0, 400+32*5, NULL, "Weapon Spread Arc: %f*", playerBulletSpread*360);
    al_draw_textf(defaultFont, al_map_rgb(122, 122, 122), 0, 400+32*6, NULL, "Damage: %d", playerDamage);
    al_draw_textf(defaultFont, al_map_rgb(122, 122, 122), 0, 400+32*7, NULL, "Bullet Speed: %d", playerBulletSpeed);
    al_draw_textf(defaultFont, al_map_rgb(122, 122, 122), 0, 400+32*8, NULL, "Fire Rate: %d", playerFiringSpeed);
}
