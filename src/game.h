/*  by Javi Agenjo 2013 UPF  javi.agenjo@gmail.com
	This class encapsulates the game, is in charge of creating the game, getting the user input, process the update and render.
*/

#ifndef GAME_H
#define GAME_H

#include "includes.h"
#include "camera.h"
#include "utils.h"
#include <vector>
#include "entity.h"
#include <array>
#include "world.h"
#include <bass.h>
#include "stage.h"
#include "AudioBass.h"

class Game
{
public:
	static Game* instance;

	World my_world;
	std::vector<Entity> tiles = {};
	std::vector<Entity> trees = {};
	Mesh* meshes[100] = {};
	Texture* textures[100] = {};
	Texture* textures_seco[100] = {};
	Texture* textures_piedra[100] = {};
	std::vector<Matrix44> models[100] = {};
	
	//Audio

	MyAudioBass* startMenu_theme = NULL;
	MyAudioBass* intro_theme = NULL;
	MyAudioBass* step = NULL;
	MyAudioBass* theme = NULL;
	MyAudioBass* hit = NULL;
	MyAudioBass* final = NULL;


	MyAudioBass* verde = NULL;
	MyAudioBass* naranja = NULL;
	MyAudioBass* piedra = NULL;
	MyAudioBass* finalboss = NULL;

	//window
	SDL_Window* window;
	int window_width;
	int window_height;

	//some globals
	long frame;
    float time;
	float elapsed_time;
	int fps;
	bool must_exit;

	bool inpueblo;

	int actualmision;

	//some vars
	Camera* camera; //our global camera
	bool mouse_locked; //tells if the mouse is locked (not seen)

	Game( int window_width, int window_height, SDL_Window* window );

	//main functions
	void render( void );
	void update( double dt );

	//events
	void onKeyDown( SDL_KeyboardEvent event );
	void onKeyUp(SDL_KeyboardEvent event);
	void onMouseButtonDown( SDL_MouseButtonEvent event );
	void onMouseButtonUp(SDL_MouseButtonEvent event);
	void onMouseWheel(SDL_MouseWheelEvent event);
	void onGamepadButtonDown(SDL_JoyButtonEvent event);
	void onGamepadButtonUp(SDL_JoyButtonEvent event);
	void onResize(int width, int height);

};


#endif 