#include "Engine.h"
#include "MenuState.h"
#include "GameState.h"
#include "PlayState.h"

void Engine::init(const char* title, int width, int height, bool fullscreen){

	// initialize ALLEGRO
	al_init();
    al_init_font_addon();
    al_init_ttf_addon();
    al_init_primitives_addon();
    al_init_image_addon();
    al_init_acodec_addon();
    al_install_keyboard();
    al_install_mouse();
    al_install_audio();

    if(fullscreen){
		al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW);
	}else{
        al_set_new_display_flags(ALLEGRO_WINDOWED);
	}
    display = al_create_display(screenWidth, screenHeight);
    al_set_window_title(display, title);

    defaultFont = al_load_font("fonts/times.ttf", 24, NULL);
    chessTileSet = al_load_bitmap("graphics/chessTileSet.png");

    event_queue = al_create_event_queue();
    timer = al_create_timer(1/FPS);

    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_mouse_event_source());
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_display_event_source(display));

	m_fullscreen = fullscreen;
    m_running = true;

    //Pointer Lists +
    for(int i = 0; i < MAX_BUTTONS; i++){
        buttonList[i] = NULL;
    }
    //Pointer Lists -

    //Variables +
    fpsTimeNew = 0, fpsCounter = 0, fpsTimeOld = 0;

    drawScreen = false, timerEvent = false, done = false, mouseButtonLeft = false, mouseButtonLeftClick = false, mouseButtonRight = false, mouseButtonRightClick = false, inGame = false, whiteTurn = true, whiteCheck = false, blackCheck = false, whiteCheckmate = false, blackCheckmate = false, upgradingPawn = false;
    selectedPieceX = -1, selectedPieceY = -1;
    mouseX = 0, mouseY = 0;
    lastKeyPress = NULL, mouseWheel = 0;

    loadMapArray(true);

    for(int x = 0; x < mapArrayWidth; x++){
        for(int y = 0; y < mapArrayHeight; y++){
            gameArray[x][y][0] = 0;
            gameArray[x][y][1] = 0;
            if(x < 3 && y < 2){
                avaliableCastling[x][y] = true;
            }
        }
    }
    //Variables -

    al_start_timer(timer);

	printf("Engine Init\n");
}

void Engine::cleanup(){
	// cleanup the all states

	while(!states.empty()){
		states.back()->cleanup();
		states.pop_back();
	}

	// switch back to windowed mode so other
	// programs won't get accidentally resized
	if(m_fullscreen){
		al_set_new_display_flags(ALLEGRO_WINDOWED);
	}

    al_destroy_font(defaultFont);
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);
    al_destroy_timer(timer);

	printf("Engine Cleanup\n");
}

void Engine::changeState(GameState* state) {
	// cleanup the current state
	if(!states.empty()){
		states.back()->cleanup();
		states.pop_back();
	}

	// store and init the new state
	states.push_back(state);
	states.back()->cleanup();
	states.back()->init();
}

void Engine::pushState(GameState* state){
	// pause current state
	if (!states.empty()){
		states.back()->pause();
	}

	// store and init the new state
	states.push_back(state);
	states.back()->init();
}

void Engine::popState(){
	// cleanup the current state
	if (!states.empty()){
		states.back()->cleanup();
		states.pop_back();
	}

	// resume previous state
	if (!states.empty()){
		states.back()->resume();
	}
}

void Engine::update(){
	// let the state update the game
	states.back()->update(this);
}

void Engine::draw(){
	// let the state update the game
	drawScreen = true;
	states.back()->draw(this);
}
