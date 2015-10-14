#include "Engine.h"
#include "MenuState.h"
#include "GameState.h"
#include "PlayState.h"
#include "EditorState.h"

Button::Button(){
    this->posX = 0;
    this->posY = 0;
    this->width = 0;
    this->height = 0;
    this->buttonId = 0;
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
void Button::setButtonId(int buttonId){
    this->buttonId = buttonId;
}
void Button::setActive(bool active){
    this->active = active;
}
bool Button::checkActive(){
    return this->active;
}
void Button::draw(){
    al_draw_rectangle(this->posX+5, this->posY+5, this->posX+this->width-5, this->posY+this->height-5, (this->selected) ? al_map_rgb(155,155,155) : al_map_rgb(215,215,215), 10);

    switch(this->buttonId){
        case 0:
            al_draw_text(defaultFont, (this->selected) ? al_map_rgb(155,155,155) : al_map_rgb(215,215,215), this->posX+this->width/2, this->posY+this->height/3, ALLEGRO_ALIGN_CENTRE, "Resume"); break;

        case 1:
            al_draw_text(defaultFont, (this->selected) ? al_map_rgb(155,155,155) : al_map_rgb(215,215,215), this->posX+this->width/2, this->posY+this->height/3, ALLEGRO_ALIGN_CENTRE, "Play"); break;

        case 2:
            al_draw_text(defaultFont, (this->selected) ? al_map_rgb(155,155,155) : al_map_rgb(215,215,215), this->posX+this->width/2, this->posY+this->height/3, ALLEGRO_ALIGN_CENTRE, "Level Editor"); break;

        case 3:
            al_draw_text(defaultFont, (this->selected) ? al_map_rgb(155,155,155) : al_map_rgb(215,215,215), this->posX+this->width/2, this->posY+this->height/3, ALLEGRO_ALIGN_CENTRE, "Quit"); break;

        case 4:
            al_draw_text(defaultFont, (this->selected) ? al_map_rgb(155,155,155) : al_map_rgb(215,215,215), this->posX+this->width/2, this->posY+this->height/3, ALLEGRO_ALIGN_CENTRE, "Speed*2");
            al_draw_textf(defaultFont, (this->selected) ? al_map_rgb(155,155,155) : al_map_rgb(215,215,215), this->posX+this->width/2, this->posY+this->height+5, ALLEGRO_ALIGN_CENTRE, "Next Cost: %f", (playerMovementSpeed < 8) ? playerMovementSpeed*playerMovementSpeed*playerMovementSpeed*playerMovementSpeed+48 : INFINITY); break;

        case 5:
            al_draw_text(defaultFont, (this->selected) ? al_map_rgb(155,155,155) : al_map_rgb(215,215,215), this->posX+this->width/2, this->posY+this->height/3, ALLEGRO_ALIGN_CENTRE, "Bullet Amount+1");
            al_draw_textf(defaultFont, (this->selected) ? al_map_rgb(155,155,155) : al_map_rgb(215,215,215), this->posX+this->width/2, this->posY+this->height+5, ALLEGRO_ALIGN_CENTRE, "Next Cost: %d", playerShots*(playerShots+9)); break;

        case 6:
            al_draw_text(defaultFont, (this->selected) ? al_map_rgb(155,155,155) : al_map_rgb(215,215,215), this->posX+this->width/2, this->posY+this->height/3, ALLEGRO_ALIGN_CENTRE, "Weapon Spread/1.25");
            al_draw_textf(defaultFont, (this->selected) ? al_map_rgb(155,155,155) : al_map_rgb(215,215,215), this->posX+this->width/2, this->posY+this->height+5, ALLEGRO_ALIGN_CENTRE, "Next Cost: %f", (playerBulletSpread > 0.05) ? round(10/playerBulletSpread) : INFINITY); break;

        case 7:
            al_draw_text(defaultFont, (this->selected) ? al_map_rgb(155,155,155) : al_map_rgb(215,215,215), this->posX+this->width/2, this->posY+this->height/3, ALLEGRO_ALIGN_CENTRE, "Weapon Damage+1");
            al_draw_textf(defaultFont, (this->selected) ? al_map_rgb(155,155,155) : al_map_rgb(215,215,215), this->posX+this->width/2, this->posY+this->height+5, ALLEGRO_ALIGN_CENTRE, "Next Cost: %d", playerDamage*(playerDamage*10)); break;

        case 8:
            al_draw_text(defaultFont, (this->selected) ? al_map_rgb(155,155,155) : al_map_rgb(215,215,215), this->posX+this->width/2, this->posY+this->height/3, ALLEGRO_ALIGN_CENTRE, "Bullet Speed+1");
            al_draw_textf(defaultFont, (this->selected) ? al_map_rgb(155,155,155) : al_map_rgb(215,215,215), this->posX+this->width/2, this->posY+this->height+5, ALLEGRO_ALIGN_CENTRE, "Next Cost: %f", (playerBulletSpeed < 16) ? round(playerBulletSpeed*(playerBulletSpeed/4)) : INFINITY); break;

        case 9:
            al_draw_text(defaultFont, (this->selected) ? al_map_rgb(155,155,155) : al_map_rgb(215,215,215), this->posX+this->width/2, this->posY+this->height/3, ALLEGRO_ALIGN_CENTRE, "Weapon Fire Rate/1.1");
            al_draw_textf(defaultFont, (this->selected) ? al_map_rgb(155,155,155) : al_map_rgb(215,215,215), this->posX+this->width/2, this->posY+this->height+5, ALLEGRO_ALIGN_CENTRE, "Next Cost: %f", (playerFiringSpeed > 3) ? (60 - playerFiringSpeed)*((60 - playerFiringSpeed)/5) + 10 : INFINITY); break;

        case 10:
            al_draw_text(defaultFont, (this->selected) ? al_map_rgb(155,155,155) : al_map_rgb(215,215,215), this->posX+this->width/2, this->posY+this->height/3, ALLEGRO_ALIGN_CENTRE, "Heal");
            al_draw_textf(defaultFont, (this->selected) ? al_map_rgb(155,155,155) : al_map_rgb(215,215,215), this->posX+this->width/2, this->posY+this->height+5, ALLEGRO_ALIGN_CENTRE, "Heal Cost: %f", (playerMaxHealth-playerHealth)*10); break;

        case 11:
            al_draw_text(defaultFont, (this->selected) ? al_map_rgb(155,155,155) : al_map_rgb(215,215,215), this->posX+this->width/2, this->posY+this->height/3, ALLEGRO_ALIGN_CENTRE, "Max Health+1");
            al_draw_textf(defaultFont, (this->selected) ? al_map_rgb(155,155,155) : al_map_rgb(215,215,215), this->posX+this->width/2, this->posY+this->height+5, ALLEGRO_ALIGN_CENTRE, "Next Cost: %f", round(playerMaxHealth*(playerMaxHealth/4))); break;
    }
}
void Button::update(){
    this->clicked = false;
    if(checkCollision(this->posX, this->posY, mouseX, mouseY, this->width, this->height, 0, 0)){
        if(mouseButtonLeftClick){
            //Do all daz butonz clickz stufz
            switch(this->buttonId){
                case 0:
                    break; //Reserved for screenState change

                case 1:
                    break; //Reserved for screenState change

                case 2:
                    break; //Reserved for screenState change

                case 3:
                    break; //Reserved for quit

                case 4:
                    if(playerMoney >= playerMovementSpeed*playerMovementSpeed*playerMovementSpeed*playerMovementSpeed+48 && playerMovementSpeed < 8){
                        playerMoney -= playerMovementSpeed*playerMovementSpeed*playerMovementSpeed*playerMovementSpeed+48;
                        playerMovementSpeed *= 2;
                    }
                    break;

                case 5:
                    if(playerMoney >= playerShots*(playerShots+9)){
                        playerMoney -= playerShots*(playerShots+9);
                        playerShots += 1;
                    }
                    break;

                case 6:
                    if(playerMoney >= 10/playerBulletSpread && playerBulletSpread > 0.05){
                        playerMoney -= 10/playerBulletSpread;
                        playerBulletSpread /= 1.25;
                    }
                    break;

                case 7:
                    if(playerMoney >= playerDamage*(playerDamage*10)){
                        playerMoney -= playerDamage*(playerDamage*10);
                        playerDamage += 1;
                    }
                    break;

                case 8:
                    if(playerMoney >= playerBulletSpeed*(playerBulletSpeed/4) && playerBulletSpeed < 16){
                        playerMoney -= playerBulletSpeed*(playerBulletSpeed/4);
                        playerBulletSpeed += 1;
                    }
                    break;

                case 9:
                    if(playerMoney >= (60 - playerFiringSpeed)*((60 - playerFiringSpeed)/5) + 10 && playerFiringSpeed > 3){
                        playerMoney -= (60 - playerFiringSpeed)*((60 - playerFiringSpeed)/5) + 10;
                        playerFiringSpeed /= 1.1;
                    }
                    break;

                case 10:
                    if(playerMoney >= (playerMaxHealth-playerHealth)*10){
                        playerMoney -= (playerMaxHealth-playerHealth)*10;
                        playerHealth = playerMaxHealth;
                    }
                    break;

                case 11:
                    if(playerMoney >=  playerMaxHealth*(playerMaxHealth/4)){
                        playerMoney -= playerMaxHealth*(playerMaxHealth/4);
                        playerMaxHealth += 1;
                        playerHealth += 1;
                    }
                    break;
            }
            this->clicked = true;
        }
        this->selected = true;
    }else{
        this->selected = false;
    }
}
