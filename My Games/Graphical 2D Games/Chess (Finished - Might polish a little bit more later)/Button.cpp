#include "Engine.h"
#include "MenuState.h"
#include "GameState.h"
#include "PlayState.h"

Button::Button(){
    this->posX = 0;
    this->posY = 0;
    this->width = 0;
    this->height = 0;
    this->buttonId = 0;
    this->buttonPointerId = 0;
    this->storedValue1 = 0;
    this->storedValue2 = 0;
    this->storedValue3 = 0;
    this->selected = false;
    this->clicked = false;
    this->active = true;
}

void Button::setPos(float x, float y){
    this->posX = x;
    this->posY = y;
}
void Button::setDimensions(float width, float height){
    this->width = width;
    this->height = height;
}
void Button::setStoredValues(int value1, int value2, int value3){
    this->storedValue1 = value1;
    this->storedValue2 = value2;
    this->storedValue3 = value3;
}
void Button::setButtonId(int buttonId){
    this->buttonId = buttonId;
}
void Button::setButtonPointerId(int buttonPointerId){
    this->buttonPointerId = buttonPointerId;
}
void Button::setActive(bool active){
    this->active = active;
}
bool Button::checkActive(){
    return this->active;
}
void Button::draw(){
    al_draw_filled_rectangle(this->posX, this->posY, this->posX+this->width, this->posY+this->height, al_map_rgb(0,0,0));
    al_draw_rectangle(this->posX+5, this->posY+5, this->posX+this->width-5, this->posY+this->height-5, (this->selected) ? al_map_rgb(155,155,155) : al_map_rgb(215,215,215), 10);

    switch(this->buttonId){
        case 0:
            al_draw_text(defaultFont, (this->selected) ? al_map_rgb(155,155,155) : al_map_rgb(215,215,215), this->posX+this->width/2, this->posY+this->height/3, ALLEGRO_ALIGN_CENTRE, "Resume"); break;

        case 1:
            al_draw_text(defaultFont, (this->selected) ? al_map_rgb(155,155,155) : al_map_rgb(215,215,215), this->posX+this->width/2, this->posY+this->height/3, ALLEGRO_ALIGN_CENTRE, "Play"); break;

        case 2:
            al_draw_text(defaultFont, (this->selected) ? al_map_rgb(155,155,155) : al_map_rgb(215,215,215), this->posX+this->width/2, this->posY+this->height/3, ALLEGRO_ALIGN_CENTRE, "Quit"); break;

        case 3:
            al_draw_text(defaultFont, (this->selected) ? al_map_rgb(155,155,155) : al_map_rgb(215,215,215), this->posX+this->width/2, this->posY+this->height/3, ALLEGRO_ALIGN_CENTRE, "Rook"); break;

        case 4:
            al_draw_text(defaultFont, (this->selected) ? al_map_rgb(155,155,155) : al_map_rgb(215,215,215), this->posX+this->width/2, this->posY+this->height/3, ALLEGRO_ALIGN_CENTRE, "Knight"); break;

        case 5:
            al_draw_text(defaultFont, (this->selected) ? al_map_rgb(155,155,155) : al_map_rgb(215,215,215), this->posX+this->width/2, this->posY+this->height/3, ALLEGRO_ALIGN_CENTRE, "Bishop"); break;

        case 6:
            al_draw_text(defaultFont, (this->selected) ? al_map_rgb(155,155,155) : al_map_rgb(215,215,215), this->posX+this->width/2, this->posY+this->height/3, ALLEGRO_ALIGN_CENTRE, "Queen"); break;
    }
}
void Button::update(){
    this->clicked = false;
    if(checkCollision(this->posX, this->posY, mouseX, mouseY, this->width, this->height, 0, 0)){
        if(mouseButtonLeftClick){
            switch(this->buttonId){
                case 0:
                    break; //Reserved for resume

                case 1:
                    break; //Reserved for screenState change

                case 2:
                    break; //Reserved for quit

                case 3:
                    upgradePawn(this->storedValue1, this->storedValue2, this->buttonId, this->storedValue3);
                    break;

                case 4:
                    upgradePawn(this->storedValue1, this->storedValue2, this->buttonId, this->storedValue3);
                    break;

                case 5:
                    upgradePawn(this->storedValue1, this->storedValue2, this->buttonId, this->storedValue3);
                    break;

                case 6:
                    upgradePawn(this->storedValue1, this->storedValue2, this->buttonId, this->storedValue3);
                    break;

            }
            this->clicked = true;
        }
        this->selected = true;
    }else{
        this->selected = false;
    }
}
