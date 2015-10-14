#include "Engine.h"
#include "MenuState.h"
#include "GameState.h"
#include "PlayState.h"
#include "EditorState.h"
#include "ShopState.h"

EditorState EditorState::m_EditorState;

void EditorState::init(){
    this->selectedTileMouseWheel = mouseWheel;
    this->selectedTile = 1;
    printf("EditorState Init\n");
}
void EditorState::cleanup(){
    printf("EditorState Cleanup\n");
}

void EditorState::pause(){
    printf("EditorState Pause\n");
}
void EditorState::resume(){
    printf("EditorState Resume\n");
}

void EditorState::update(Engine* engine){
    ALLEGRO_EVENT events;
    ALLEGRO_MOUSE_EVENT mouseEvents;
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

        if(al_mouse_button_down(&mouseState, 2)){
            if(!mouseButtonRight)
                mouseButtonRightClick = true;
            else
                mouseButtonRightClick = false;
            mouseButtonRight = true;
        }else{
            mouseButtonRight = false;
        }
        //Update Mouse Variables -

        //Input +
        if(al_key_down(&keyState, ALLEGRO_KEY_ESCAPE)){
            if(lastKeyPress != ALLEGRO_KEY_ESCAPE){
                inEditor = true;
                engine->pushState(MenuState::instance());
                lastKeyPress = ALLEGRO_KEY_ESCAPE;
            }
        }else if(al_key_down(&keyState, ALLEGRO_KEY_F9)){
            if(lastKeyPress != ALLEGRO_KEY_F9){
                saveMapArray();
                lastKeyPress = ALLEGRO_KEY_F9;
            }
         }else if(al_key_down(&keyState, ALLEGRO_KEY_F5)){
            if(lastKeyPress != ALLEGRO_KEY_F5){
                loadMapArray();
                lastKeyPress = ALLEGRO_KEY_F5;
            }
         }else if(al_key_down(&keyState, ALLEGRO_KEY_1)){
            if(lastKeyPress != ALLEGRO_KEY_1){
                this->selectedTile = 1;
                lastKeyPress = ALLEGRO_KEY_1;
            }
         }else if(al_key_down(&keyState, ALLEGRO_KEY_2)){
            if(lastKeyPress != ALLEGRO_KEY_2){
                this->selectedTile = 2;
                lastKeyPress = ALLEGRO_KEY_2;
            }
         }else if(al_key_down(&keyState, ALLEGRO_KEY_3)){
            if(lastKeyPress != ALLEGRO_KEY_3){
                this->selectedTile = 3;
                lastKeyPress = ALLEGRO_KEY_3;
            }
         }else if(al_key_down(&keyState, ALLEGRO_KEY_4)){
            if(lastKeyPress != ALLEGRO_KEY_4){
                this->selectedTile = 4;
                lastKeyPress = ALLEGRO_KEY_4;
            }
         }else if(al_key_down(&keyState, ALLEGRO_KEY_5)){
            if(lastKeyPress != ALLEGRO_KEY_5){
                this->selectedTile = 5;
                lastKeyPress = ALLEGRO_KEY_5;
            }
         }else if(al_key_down(&keyState, ALLEGRO_KEY_6)){
            if(lastKeyPress != ALLEGRO_KEY_6){
                this->selectedTile = 6;
                lastKeyPress = ALLEGRO_KEY_6;
            }
         }else if(al_key_down(&keyState, ALLEGRO_KEY_7)){
            if(lastKeyPress != ALLEGRO_KEY_7){
                this->selectedTile = 7;
                lastKeyPress = ALLEGRO_KEY_7;
            }
         }else if(al_key_down(&keyState, ALLEGRO_KEY_8)){
            if(lastKeyPress != ALLEGRO_KEY_8){
                this->selectedTile = 8;
                lastKeyPress = ALLEGRO_KEY_8;
            }
         }else if(al_key_down(&keyState, ALLEGRO_KEY_9)){
            if(lastKeyPress != ALLEGRO_KEY_9){
                this->selectedTile = 9;
                lastKeyPress = ALLEGRO_KEY_9;
            }
         }else if(al_key_down(&keyState, ALLEGRO_KEY_0)){
            if(lastKeyPress != ALLEGRO_KEY_0){
                this->selectedTile = 10;
                lastKeyPress = ALLEGRO_KEY_0;
            }
         }

        //PUT IN MOUSE WHEEL SELECT TILE HERE!!
        if(mouseWheel - selectedTileMouseWheel < 0){
            selectedTileMouseWheel = mouseWheel;
        }else if(mouseWheel - selectedTileMouseWheel > 10){
            selectedTileMouseWheel = mouseWheel - 10;
        }else{
            selectedTile = mouseWheel - selectedTileMouseWheel;
        }
        //Input -

        //MapArray Editing +
        if(mouseButtonLeft && insideMap(mouseX, mouseY, 0 ,0)){
            int tX = floor(mouseX/tileSize), tY = floor(mouseY/tileSize);
            mapArray[tX][tY] = this->selectedTile;
        }
        if(mouseButtonRight && insideMap(mouseX, mouseY, 0 ,0)){
            int tX = floor(mouseX/tileSize), tY = floor(mouseY/tileSize);
            mapArray[tX][tY] = 0;
        }
        //MapArray Editing -

        //Rest +
        if(!al_key_down(&keyState, lastKeyPress)){
            lastKeyPress = NULL;
        }
        //Rest -
        engine->draw();
    }
}
void EditorState::draw(Engine* engine){
    //Draw Map +
    drawMap();
    //Draw Map -

    //Draw GUI +
    al_draw_text(defaultFont, al_map_rgb(155,155,155), 10, mapHeight+30, NULL, "Selected Tile:");

    drawTile(150, mapHeight+35, this->selectedTile);
    //Draw GUI -
}
