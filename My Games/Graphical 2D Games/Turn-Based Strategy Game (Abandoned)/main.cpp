#include "Engine.h"

bool isPassable(int x, int y);
void updateCamera();
void addUnitToList(Unit *newUnit);
bool checkInsideMapDisplay(int x, int y);

//Allegro Variables ++
ALLEGRO_FONT *defaultFont;
//Allegro Variables --

Unit *unitList[MAX_UNITS];

bool draw, retry, done, mouseButtonLeft, firstMouseButtonLeft;
float mouseX, mouseY;
int lastKeyPress, playerMovementPoints, playerX, playerY, playerDeltaX, playerDeltaY;
int cameraPosX, cameraPosY, cameraOffsetX, cameraOffsetY, cameraMouseSwipe[2];

int mapArray[mapWidth][mapHeight];

int main(){
    float FPS = 60.0;

    //Allegro Init ++
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
    al_set_window_title(display, "Turn-Based Strategy Game");

    defaultFont = al_load_font("fonts/times.ttf", 24, NULL);
    ALLEGRO_KEYBOARD_STATE keyState;
    ALLEGRO_MOUSE_STATE mouseState;
    ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();
    ALLEGRO_TIMER *timer = al_create_timer(1/FPS);

    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_mouse_event_source());
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_display_event_source(display));
    //Allegro Init --

    srand(time(0));

    retry = true;

    while(retry){

        //Variables ++
        float fpsTimeNew, fpsCounter, fpsTimeOld;

        draw = false, retry = true, done = false, mouseButtonLeft = false;
        mouseX = 0, mouseY = 0;
        lastKeyPress = NULL, playerMovementPoints = 5000;
        playerX = 0, playerY = 0, playerDeltaX = 0, playerDeltaX = 0;
        cameraPosX = 0, cameraPosY = 0, cameraOffsetX = 0, cameraOffsetY = 0;

        for(int x = 0; x < mapWidth; x++){
            for(int y = 0; y < mapHeight; y++){
                if((x + y)%3 == 0){
                    mapArray[x][y] = 0;
                }else if((x + y)%3 == 1){
                    mapArray[x][y] = 1;
                }else{
                    mapArray[x][y] = 2;
                }
            }
        }

        for(int i = 0; i < MAX_UNITS; i++){
            unitList[i] = NULL;
        }

        for(int i = 0; i < MAX_UNITS; i++){
            float health;
            int x, y, movementPoints, unitId;
            x = rand() % mapWidth;
            y = rand() % mapHeight;
            health = 5;
            movementPoints = 5;
            unitId = 0;

            Unit *newUnit = new Unit();

            newUnit->setPos(x, y);
            newUnit->setUnitId(unitId);
            newUnit->setHealth(health);
            newUnit->setMaxHealth(health);
            newUnit->setMovementPoints(movementPoints);
            addUnitToList(newUnit);
        }
        //Variables --

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

                //Update Mouse Variables ++
                mouseX = al_get_mouse_state_axis(&mouseState,0);
                mouseY = al_get_mouse_state_axis(&mouseState,1);

                if(al_mouse_button_down(&mouseState, 1)){
                    firstMouseButtonLeft = (!mouseButtonLeft) ? true : false;
                    mouseButtonLeft = true;
                }else{
                    mouseButtonLeft = false;
                }

                if(mouseButtonLeft){
                    int roundedMouseX = floor(mouseX/tileSize);
                    int roundedMouseY = floor(mouseY/tileSize);
                    if(firstMouseButtonLeft){
                        cameraMouseSwipe[0] = roundedMouseX+cameraPosX;
                        cameraMouseSwipe[1] = roundedMouseY+cameraPosY;
                    }
                    cameraPosX = cameraMouseSwipe[0] - roundedMouseX;
                    cameraPosY = cameraMouseSwipe[1] - roundedMouseY;
                    updateCamera();
                }
                //Update Mouse Variables --

                //Keyboard Input ++
                if(al_key_down(&keyState, ALLEGRO_KEY_UP)){
                    if(lastKeyPress != ALLEGRO_KEY_UP && playerMovementPoints > 0){
                        playerDeltaY = -1;
                        playerMovementPoints -= 1;
                        lastKeyPress = ALLEGRO_KEY_UP;
                    }
                }else if(al_key_down(&keyState, ALLEGRO_KEY_DOWN)){
                    if(lastKeyPress != ALLEGRO_KEY_DOWN && playerMovementPoints > 0){
                        playerDeltaY = 1;
                        playerMovementPoints -= 1;
                        lastKeyPress = ALLEGRO_KEY_DOWN;
                    }
                }else if(al_key_down(&keyState, ALLEGRO_KEY_LEFT)){
                    if(lastKeyPress != ALLEGRO_KEY_LEFT && playerMovementPoints > 0){
                        playerDeltaX = -1;
                        playerMovementPoints -= 1;
                        lastKeyPress = ALLEGRO_KEY_LEFT;
                    }
                }else if(al_key_down(&keyState, ALLEGRO_KEY_RIGHT)){
                    if(lastKeyPress != ALLEGRO_KEY_RIGHT && playerMovementPoints > 0){
                        playerDeltaX = 1;
                        playerMovementPoints -= 1;
                        lastKeyPress = ALLEGRO_KEY_RIGHT;
                    }
                }else if(al_key_down(&keyState, ALLEGRO_KEY_ENTER)){
                    if(lastKeyPress != ALLEGRO_KEY_ENTER){
                        playerMovementPoints = 50;
                        lastKeyPress = ALLEGRO_KEY_ENTER;
                    }
                }else if(al_key_down(&keyState, ALLEGRO_KEY_SPACE)){
                    if(lastKeyPress != ALLEGRO_KEY_SPACE){
                        mapArray[playerX][playerY] = 3;
                        lastKeyPress = ALLEGRO_KEY_SPACE;
                        cameraPosX = playerX - mapDisplayWidth/tileSize/2;
                        cameraPosY = playerY - mapDisplayHeight/tileSize/2;
                        updateCamera();
                    }
                }
                //Keyboard Input --

                //Update Player Variables ++
                if(isPassable(playerX + playerDeltaX, playerY)){
                    playerX += playerDeltaX;
                }
                if(isPassable(playerX, playerY + playerDeltaY)){
                    playerY += playerDeltaY;
                }

                playerDeltaX = 0;
                playerDeltaY = 0;
                //Update Player Variables --

                //Update Technical Variables ++
                if(!al_key_down(&keyState, lastKeyPress)){
                    lastKeyPress = NULL;
                }

                cameraOffsetX = -cameraPosX*tileSize;
                cameraOffsetY = -cameraPosY*tileSize;
                //Update Technical Variables --

                draw = true;
            }
            if(draw){
                draw = false;

                //Draw Map ++
                for(int x = 0; x < mapDisplayWidth/tileSize; x++){
                    for(int y = 0; y < mapDisplayHeight/tileSize; y++){
                        switch(mapArray[x+cameraPosX][y+cameraPosY]){
                            case 0:
                                al_draw_filled_rectangle(x*tileSize, y*tileSize, (x+1)*tileSize, (y+1)*tileSize, al_map_rgb(75,75,75));
                                break;

                            case 1:
                                al_draw_filled_rectangle(x*tileSize, y*tileSize, (x+1)*tileSize, (y+1)*tileSize, al_map_rgb(100,100,100));
                                break;

                            case 2:
                                al_draw_filled_rectangle(x*tileSize, y*tileSize, (x+1)*tileSize, (y+1)*tileSize, al_map_rgb(125,125,125));
                                break;

                            case 3:
                                al_draw_filled_rectangle(x*tileSize, y*tileSize, (x+1)*tileSize, (y+1)*tileSize, al_map_rgb(50,50,255));
                                break;
                        }
                    }
                }
                //Draw Map --

                //Draw Entities ++
                for(int i = 0; i < MAX_UNITS; i++){
                    if(unitList[i] != NULL && unitList[i]->checkActive()){
                        unitList[i]->draw();
                    }
                }
                if(checkInsideMapDisplay(playerX, playerY)){
                    al_draw_filled_rectangle(playerX*tileSize + cameraOffsetX, playerY*tileSize + cameraOffsetY, (playerX+1)*tileSize + cameraOffsetX, (playerY+1)*tileSize + cameraOffsetY, al_map_rgb(75,255,75));
                }
                //Draw Entities --

                //Draw GUI ++
                for(int x = 0; x < mapDisplayWidth/tileSize; x++){
                    al_draw_line(x*tileSize, 0, x*tileSize, mapDisplayHeight*tileSize, al_map_rgb(0,0,0), 2);
                }

                for(int y = 0; y < mapDisplayHeight/tileSize; y++){
                    al_draw_line(0, y*tileSize, mapDisplayWidth*tileSize, y*tileSize, al_map_rgb(0,0,0), 2);
                }

                al_draw_textf(defaultFont, al_map_rgb(150,150,150), 0, (mapDisplayHeight + screenHeight)/2, NULL, "Movementpoints Left: %d", playerMovementPoints);

                fpsTimeNew = al_get_time();
                fpsCounter = 1/(fpsTimeNew - fpsTimeOld);
                fpsTimeOld = fpsTimeNew;

                al_draw_textf(defaultFont, al_map_rgb(122, 122, 122), screenWidth-600, screenHeight-80, NULL, "FPS: %f", round(fpsCounter*10)/10);
                //Draw GUI --

                al_flip_display();
                al_clear_to_color(al_map_rgb(0, 0, 0));
            }
        }
    }
    //Pointer Destruction ++
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);
    al_destroy_timer(timer);
    //Pointer Destruction --
}

bool isPassable(int x, int y){
    if(x < 0 || x >= mapWidth || y < 0 || y >= mapHeight){
        return false;
    }
    return true;
}

void updateCamera(){
    if(cameraPosX < 0){
        cameraPosX = 0;
    }
    if(cameraPosX + mapDisplayWidth/tileSize >= mapWidth){
        cameraPosX = mapWidth - mapDisplayWidth/tileSize;
    }
    if(cameraPosY < 0){
        cameraPosY = 0;
    }
    if(cameraPosY + mapDisplayHeight/tileSize >= mapHeight){
        cameraPosY = mapHeight - mapDisplayHeight/tileSize;
    }
}

void addUnitToList(Unit *newUnit){
    for(int i = 0; i < MAX_UNITS; i++){
        if(unitList[i] == NULL || !unitList[i]->checkActive()){
            unitList[i] = newUnit;
            return;
        }
    }
}

bool checkInsideMapDisplay(int x, int y){
    if(x*tileSize-cameraPosX*tileSize < mapDisplayWidth && y*tileSize-cameraPosY*tileSize < mapDisplayHeight && x*tileSize-cameraPosX*tileSize >= 0 && y*tileSize-cameraPosY*tileSize >= 0){
        return true;
    }
    return false;
}
