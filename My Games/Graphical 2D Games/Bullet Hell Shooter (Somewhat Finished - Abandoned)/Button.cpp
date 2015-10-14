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

#define buttonBorderSize 5

Button::Button(){
	this->posX = 0;
	this->posY = 0;
	this->width = 0;
	this->height = 0;
	this->buttonId = 0;
	this->selected = false;
	this->playButtonPress = false;
	this->playButtonPressHelper = false;
	this->buttonPressed = false;
	this->active = true;
}

void Button::setPos(int x, int y){
    this->posX = x;
    this->posY = y;
}

void Button::setDimensions(int width, int height){
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
    al_draw_rectangle(this->posX + buttonBorderSize/2, this->posY + buttonBorderSize/2, this->posX + this->width - buttonBorderSize/2, this->posY + this->height - buttonBorderSize/2, (!this->selected) ? al_map_rgb(122, 122, 122) : al_map_rgb(170, 170, 170), buttonBorderSize);
    switch(this->buttonId){
        case 0:
            al_draw_text(buttonFont, (!this->selected) ? al_map_rgb(122, 122, 122) : al_map_rgb(170, 170, 170), this->posX + this->width/2, this->posY + this->height/4, ALLEGRO_ALIGN_CENTRE, "Play");
            break;

        case 1:
            al_draw_text(buttonFont, (!this->selected) ? al_map_rgb(122, 122, 122) : al_map_rgb(170, 170, 170), this->posX + this->width/2, this->posY + this->height/4, ALLEGRO_ALIGN_CENTRE, "Options");
            break;

        case 2:
            al_draw_text(buttonFont, (!this->selected) ? al_map_rgb(122, 122, 122) : al_map_rgb(170, 170, 170), this->posX + this->width/2, this->posY + this->height/4, ALLEGRO_ALIGN_CENTRE, "Exit");
            break;

        case 3:
            al_draw_text(buttonFont, (!this->selected) ? al_map_rgb(122, 122, 122) : al_map_rgb(170, 170, 170), this->posX + this->width/2, this->posY + this->height/4, ALLEGRO_ALIGN_CENTRE, "W. Shots Up +1");
            al_draw_textf(buttonFont, al_map_rgb(122, 122, 122), this->posX + this->width/2, this->posY + this->height, ALLEGRO_ALIGN_CENTRE, "%d S.", playerShots[0]*20);
            al_draw_textf(buttonFont, al_map_rgb(122, 122, 122), this->posX + this->width/2, this->posY - 20, ALLEGRO_ALIGN_CENTRE, "%d Shots Up", playerShots[0]);
            break;

        case 4:
            al_draw_text(buttonFont, (!this->selected) ? al_map_rgb(122, 122, 122) : al_map_rgb(170, 170, 170), this->posX + this->width/2, this->posY + this->height/4, ALLEGRO_ALIGN_CENTRE, "W. Shots Right +1");
            al_draw_textf(buttonFont, al_map_rgb(122, 122, 122), this->posX + this->width/2, this->posY + this->height, ALLEGRO_ALIGN_CENTRE, "%d S.", playerShots[1]*20);
            al_draw_textf(buttonFont, al_map_rgb(122, 122, 122), this->posX + this->width/2, this->posY - 20, ALLEGRO_ALIGN_CENTRE, "%d Shots Right", playerShots[1]);
            break;

        case 5:
            al_draw_text(buttonFont, (!this->selected) ? al_map_rgb(122, 122, 122) : al_map_rgb(170, 170, 170), this->posX + this->width/2, this->posY + this->height/4, ALLEGRO_ALIGN_CENTRE, "W. Shots Down +1");
            al_draw_textf(buttonFont, al_map_rgb(122, 122, 122), this->posX + this->width/2, this->posY + this->height, ALLEGRO_ALIGN_CENTRE, "%d S.", playerShots[2]*20);
            al_draw_textf(buttonFont, al_map_rgb(122, 122, 122), this->posX + this->width/2, this->posY - 20, ALLEGRO_ALIGN_CENTRE, "%d Shots Down", playerShots[2]);
            break;

        case 6:
            al_draw_text(buttonFont, (!this->selected) ? al_map_rgb(122, 122, 122) : al_map_rgb(170, 170, 170), this->posX + this->width/2, this->posY + this->height/4, ALLEGRO_ALIGN_CENTRE, "W. Shots Left +1");
            al_draw_textf(buttonFont, al_map_rgb(122, 122, 122), this->posX + this->width/2, this->posY + this->height, ALLEGRO_ALIGN_CENTRE, "%d S.", playerShots[3]*20);
            al_draw_textf(buttonFont, al_map_rgb(122, 122, 122), this->posX + this->width/2, this->posY - 20, ALLEGRO_ALIGN_CENTRE, "%d Shots Left", playerShots[3]);
            break;

        case 7:
            al_draw_text(buttonFont, (!this->selected) ? al_map_rgb(122, 122, 122) : al_map_rgb(170, 170, 170), this->posX + this->width/2, this->posY + this->height/4, ALLEGRO_ALIGN_CENTRE, "W. Fire Rate -10%");
            al_draw_textf(buttonFont, al_map_rgb(122, 122, 122), this->posX + this->width/2, this->posY + this->height, ALLEGRO_ALIGN_CENTRE, "%f S.", 1000/playerShootingSpeed);
            al_draw_textf(buttonFont, al_map_rgb(122, 122, 122), this->posX + this->width/2, this->posY - 20, ALLEGRO_ALIGN_CENTRE, "%f Firing Speed", playerShootingSpeed);
            break;

        case 8:
            al_draw_text(buttonFont, (!this->selected) ? al_map_rgb(122, 122, 122) : al_map_rgb(170, 170, 170), this->posX + this->width/2, this->posY + this->height/4, ALLEGRO_ALIGN_CENTRE, "W. Damage +1");
            al_draw_textf(buttonFont, al_map_rgb(122, 122, 122), this->posX + this->width/2, this->posY + this->height, ALLEGRO_ALIGN_CENTRE, "%d S.", playerDamage*150);
            al_draw_textf(buttonFont, al_map_rgb(122, 122, 122), this->posX + this->width/2, this->posY - 20, ALLEGRO_ALIGN_CENTRE, "%d Bullet Damage", playerDamage);
            break;

        case 9:
            al_draw_text(buttonFont, (!this->selected) ? al_map_rgb(122, 122, 122) : al_map_rgb(170, 170, 170), this->posX + this->width/2, this->posY + this->height/4, ALLEGRO_ALIGN_CENTRE, "Heal Full");
            al_draw_textf(buttonFont, al_map_rgb(122, 122, 122), this->posX + this->width/2, this->posY + this->height, ALLEGRO_ALIGN_CENTRE, "%d S.", (playerMaxHealth - playerHealth)*5);
            al_draw_textf(buttonFont, al_map_rgb(122, 122, 122), this->posX + this->width/2, this->posY - 20, ALLEGRO_ALIGN_CENTRE, "%d Health To Replenish", playerMaxHealth - playerHealth);
            break;

        case 10:
            al_draw_text(buttonFont, (!this->selected) ? al_map_rgb(122, 122, 122) : al_map_rgb(170, 170, 170), this->posX + this->width/2, this->posY + this->height/4, ALLEGRO_ALIGN_CENTRE, "Health Amount +1");
            al_draw_textf(buttonFont, al_map_rgb(122, 122, 122), this->posX + this->width/2, this->posY + this->height, ALLEGRO_ALIGN_CENTRE, "%d S.", playerMaxHealth*20);
            al_draw_textf(buttonFont, al_map_rgb(122, 122, 122), this->posX + this->width/2, this->posY - 20, ALLEGRO_ALIGN_CENTRE, "%d Max Health", playerMaxHealth);
            break;

        case 11:
            al_draw_text(buttonFont, (!this->selected) ? al_map_rgb(122, 122, 122) : al_map_rgb(170, 170, 170), this->posX + this->width/2, this->posY + this->height/4, ALLEGRO_ALIGN_CENTRE, "Reset");
            break;

        case 12:
            al_draw_text(buttonFont, (!this->selected) ? al_map_rgb(122, 122, 122) : al_map_rgb(170, 170, 170), this->posX + this->width/2, this->posY + this->height/4, ALLEGRO_ALIGN_CENTRE, "Exit");
            break;
    }

    if(this->playButtonPressHelper){
        this->playButtonPress = true;
    }

    if(this->selected){
        this->playButtonPressHelper = false;
        this->playButtonPress = false;
    }else if(!this->selected && !playButtonPressHelper){
        this->playButtonPressHelper = true;
    }

    this->selected = false;
}

void Button::update(){
    this->selected = true;

    if(mouseLeftDown && !this->buttonPressed){
            this->playButtonPress = true;
            this->buttonPressed = true;
        switch(this->buttonId){
            case 0:
                if(!shopMenu){
                    screenState = gameState;
                }else{
                    screenState = shopState;
                }
                break;

            case 1:
                break;

            case 2:
                done = true;
                retry = false;
                break;

            case 3:
                if(playerScore >= playerShots[0]*20 && playerShots[0] < MAX_PLAYER_SHOTS){
                    playerScore -= playerShots[0]*20;
                    playerShots[0] += 1;
                }
                break;

            case 4:
                if(playerScore >= playerShots[1]*20 && playerShots[1] < MAX_PLAYER_SHOTS){
                    playerScore -= playerShots[1]*20;
                    playerShots[1] += 1;
                }
                break;

            case 5:
                if(playerScore >= playerShots[2]*20 && playerShots[2] < MAX_PLAYER_SHOTS){
                    playerScore -= playerShots[2]*20;
                    playerShots[2] += 1;
                }
                break;

            case 6:
                if(playerScore >= playerShots[3]*20 && playerShots[3] < MAX_PLAYER_SHOTS){
                    playerScore -= playerShots[3]*20;
                    playerShots[3] += 1;
                }
                break;

            case 7:
                if(playerScore >= 1000/playerShootingSpeed && playerShootingSpeed > MIN_PLAYER_SHOT_SPEED){
                    playerScore -= 1000/playerShootingSpeed;
                    playerShootingSpeed *= 0.95;
                }
                break;

            case 8:
                if(playerScore >= playerDamage*150 && playerDamage < MAX_PLAYER_DAMAGE){
                    playerScore -= playerDamage*150;
                    playerDamage += 1;
                }
                break;

            case 9:
                if(playerScore >= (playerMaxHealth - playerHealth)*5){
                    playerScore -= (playerMaxHealth - playerHealth)*5;
                    playerHealth = playerMaxHealth;
                }
                break;

            case 10:
                if(playerScore >= playerMaxHealth*20 && playerMaxHealth < MAX_PLAYER_HEALTH){
                    playerScore -= playerMaxHealth*20;
                    playerMaxHealth += 1;
                }
                break;

            case 11:
                resetShop = true;
                break;

            case 12:
                screenState = gameState;
                break;

        }
    }else if(!mouseLeftDown){
        this->buttonPressed = false;
    }
}
