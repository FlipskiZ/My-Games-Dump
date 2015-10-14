#include "Engine.h"
#include "MenuState.h"
#include "GameState.h"
#include "PlayState.h"
#include "EditorState.h"
#include "ShopState.h"

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
    waveFont = al_load_font("fonts/times.ttf", 240, NULL);
    cursorImage = al_load_bitmap("graphics/cursorImage.png");
    attackImage = al_load_bitmap("graphics/attackImage.png");
    zombieImage = al_load_bitmap("graphics/zombieImage.png");
    playerImage = al_load_bitmap("graphics/playerImage.png");
    bulletImage = al_load_bitmap("graphics/bulletImage.png");
    concreteImage = al_load_bitmap("graphics/concreteImage.png");
    cobblestoneImage = al_load_bitmap("graphics/cobblestoneImage.png");
    mossCobblestoneImage = al_load_bitmap("graphics/mossCobblestoneImage.png");
    downDirectionalImage = al_load_bitmap("graphics/downDirectionalImage.png");
    leftDirectionalImage = al_load_bitmap("graphics/leftDirectionalImage.png");
    upDirectionalImage = al_load_bitmap("graphics/upDirectionalImage.png");
    rightDirectionalImage = al_load_bitmap("graphics/rightDirectionalImage.png");
    spawnerImage = al_load_bitmap("graphics/spawnerImage.png");
    shopImage = al_load_bitmap("graphics/shopImage.png");
    plankDImage = al_load_bitmap("graphics/plankDImage.png");
    plankVImage = al_load_bitmap("graphics/plankVImage.png");

    keyState;
    mouseState;
    event_queue = al_create_event_queue();
    timer = al_create_timer(1/FPS);

    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_mouse_event_source());
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_display_event_source(display));

    al_hide_mouse_cursor(display);

	m_fullscreen = fullscreen;
    m_running = true;

    //Pointer Lists +
    for(int i = 0; i < MAX_BULLETS; i++){
        bulletList[i] = NULL;
    }
    //Pointer Lists -

    //Variables +
    playerColX = false, playerColY = false;
    fpsTimeNew = 0, fpsCounter = 0, fpsTimeOld = 0;

    done = false, mouseButtonLeft = false, mouseButtonLeftClick = false, mouseButtonRight = false, mouseButtonRightClick = false, inGame = false, inShop = false, pauseBetweenWavesCheck = false, enemySpawnAll = false;
    mouseX = 0, mouseY = 0;
    lastKeyPress = NULL;
    playerX = 0, playerY = 0, playerWidth = 0, playerHeight = 0, playerCenterX = 0, playerCenterY = 0, playerMovementSpeed = 0, playerDeltaX = 0, playerDeltaX = 0;
    playerHealth = 0, playerMaxHealth = 0;
    playerFiringSpeed = 0, playerFiringSpeedHelper = 0, playerShots = 0, playerDamage = 0, playerMoney = 0;
    playerBulletSpeed = 0, playerBulletSpread = 0;
    enemySpawnRate = 0, enemySpawnRateHelper = 0;
    enemyHealth = 0, enemySpeed = 0, enemyDamage = 0, enemyAttackTime = 0, enemyKillMoney = 0;
    enemyWave = 0, amountEnemiesSpawn = 0, amountEnemiesSpawned = 0, pauseBetweenWaves = 0, pauseBetweenWavesHelper = 0;

    loadMapArray();
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
