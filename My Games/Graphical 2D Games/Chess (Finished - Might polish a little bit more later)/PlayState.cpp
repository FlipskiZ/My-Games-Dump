#include "Engine.h"
#include "MenuState.h"
#include "GameState.h"
#include "PlayState.h"

PlayState PlayState::m_PlayState;

void PlayState::init(){

    selectedPieceX = -1, selectedPieceY = -1, whiteTurn = true, whiteCheck = false, blackCheck = false, whiteCheckmate = false, blackCheckmate = false;

    loadMapArray(true);

    updateGameArray();

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

        //Mouse Input +
        if(mouseButtonLeftClick && !upgradingPawn){
            if(selectedPieceX == -1 || selectedPieceY == -1){
                selectPiece(mouseX, mouseY);
            }else{
                movePiece(mouseX, mouseY);
            }
        }
        //Mouse Input -

        //Player Input +
        if(al_key_down(&keyState, ALLEGRO_KEY_F5)){
            if(lastKeyPress != ALLEGRO_KEY_F5){
                loadMapArray(true);
                lastKeyPress = ALLEGRO_KEY_F5;
            }
        }else if(al_key_down(&keyState, ALLEGRO_KEY_ESCAPE) && !upgradingPawn){
            if(lastKeyPress != ALLEGRO_KEY_ESCAPE){
                engine->pushState(MenuState::instance());
                lastKeyPress = ALLEGRO_KEY_ESCAPE;
            }
        }
        //Player Input -

        //Update Entities +
        for(int i = 0; i < MAX_BUTTONS; i++){
            if(buttonList[i] != NULL && buttonList[i]->checkActive()){
                buttonList[i]->update();
            }
        }
        //Update Entities -

        //Rest +

        //Rest -
        engine->draw();
    }
}

void PlayState::draw(Engine* engine){
    //Draw Map+
    drawMap();
    //Draw Map-

    //Draw Entities +
    for(int i = 0; i < MAX_BUTTONS; i++){
        if(buttonList[i] != NULL && buttonList[i]->checkActive()){
            buttonList[i]->draw();
        }
    }
    //Draw Entities -

    //Draw GUI +
    fpsTimeNew = al_get_time();
    fpsCounter = 1/(fpsTimeNew - fpsTimeOld);
    fpsTimeOld = fpsTimeNew;

    al_draw_textf(defaultFont, al_map_rgb(122, 122, 122), screenWidth-175, screenHeight-25, NULL, "FPS: %f", round(fpsCounter*10)/10);
    al_draw_textf(defaultFont, al_map_rgb(122, 122, 122), 50, screenHeight-50, NULL, "%s", (whiteCheckmate) ? "Black Won!" : " ");
    al_draw_textf(defaultFont, al_map_rgb(122, 122, 122), 50, screenHeight-25, NULL, "%s", (blackCheckmate) ? "White Won!" : " ");
    //Draw GUI -
}
