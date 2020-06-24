#include "game.h"
#include "utils.h"
#include "mesh.h"
#include "texture.h"
#include "fbo.h"
#include "shader.h"
#include "input.h"
#include "animation.h"
#include <string>
#include <cstring>

#include "grender.h"
#include "entity.h"
#include "entity_player.h"
#include "entity_enemy.h"
#include "world.h"
#include "stage.h"
#include "intro_stage.h"
#include "play_stage.h"
#include "menu_stage.h"
#include "select_stage.h"
#include "debug_stage.h"
#include "data.h"
#include <cmath>

#include "bass.h"
#include "AudioBass.h"

//shaders
Shader* shader = NULL;
Shader* shader_instanced = NULL;
Shader* shader_blanc = NULL;
Shader* anishader = NULL;

//Data structures
std::map<std::string, Stage*> Stage::stages;

//World and Execution
Game* Game::instance = NULL;
Stage* Stage::current_stage = NULL;
IntroStage intro;
PlayStage play;
DebugStage debug;
MenuStage menu;
SelectStage selected;



void renderAnimated(Matrix44 m, Mesh* mesh, Texture* texture, Skeleton* skeleton) {

	Camera* camera = Camera::current;
	Vector3 pos = m * mesh->box.center;

	BoundingBox global_box = transformBoundingBox(m, mesh->box);
	if (!camera->testBoxInFrustum(global_box.center, global_box.halfsize)) {
		return;
	}
	glPointSize(5);
	anishader = Shader::Get("data/shaders/skinning.vs", "data/shaders/textured.fs");

	anishader->enable();

	anishader->setUniform("u_color", Vector4(1, 1, 1, 1));
	anishader->setUniform("u_viewprojection", camera->viewprojection_matrix);
	if (texture)
		anishader->setUniform("u_texture", texture);
	anishader->setUniform("u_model", m);
	anishader->setUniform("u_texture_tiling", 1.0f);
	anishader->setUniform("u_time", Game::instance->time);
	anishader->setUniform("u_camera_position", camera->eye);
	anishader->setUniform("u_light_direction", Vector3(0.3, 0.6, 0.2).normalize());

	mesh->renderAnimated(GL_TRIANGLES, skeleton);

	anishader->disable();
}

void renderMesh(Matrix44 m, Mesh* mesh, Texture* texture, int submesh = -1)
{
	if (!shader)
		return;

	Camera* camera = Camera::current;

	//enable shader
	shader->enable();

	//upload uniforms
	shader->setUniform("u_color", Vector4(1, 1, 1, 1));
	shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
	shader->setUniform("u_texture", texture);
	shader->setUniform("u_model", m);
	shader->setUniform("u_light_direction", Vector3(10, 3, -13));
	shader->setUniform("u_camera_position", camera->eye);

	//shader->setUniform("u_time", time);


	mesh->render(GL_TRIANGLES);

	//disable shader
	shader->disable();
}

Game::Game(int window_width, int window_height, SDL_Window* window)
{
	
	this->window_width = window_width;
	this->window_height = window_height;
	this->window = window;
	instance = this;
	must_exit = false;

	fps = 0;
	frame = 0;
	time = 0.0f;
	elapsed_time = 0.0f;
	mouse_locked = false;

	inpueblo = false;

	//OpenGL flags
	glEnable( GL_CULL_FACE ); //render both sides of every triangle
	glEnable( GL_DEPTH_TEST ); //check the occlusions using the Z buffer

	//create our camera
	camera = new Camera();
	camera->lookAt(Vector3(0.2f,0.6f,-0.5f),Vector3(0.2f,0.f,1.f), Vector3(0.f,1.f,0.f)); //position the camera and point to 0,0,0
	camera->setPerspective(70.f,window_width/(float)window_height,0.1f,100000.f); //set the projection, we want to be perspective

	// Load Shaders
	shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
	shader_instanced = Shader::Get("data/shaders/instanced.vs", "data/shaders/texture.fs");
	shader_blanc = Shader::Get("data/shaders/basic.vs", "data/shaders/sky.fs");

	// Load Meshes and Textures
	for (int i = 0; i < data_h_size + 1; ++i) {
		std::string s1 = "data/Assets/Meshes/" + mesh_names[i];
		std::string s2 = "data/Assets/Textures/" + texture_names[i];
		meshes[i] = Mesh::Get(const_cast<char*>(s1.c_str()));
		textures[i] = Texture::Get(const_cast<char*>(s2.c_str()));
	}

	textures[95] = Texture::Get("data/Assets/Textures/GUI/mapa_con_logo2.png");
	textures[96] = Texture::Get("data/Assets/Textures/GUI/minimap_pueblo.png");
	textures[97] = Texture::Get("data/Assets/Textures/GUI/minimap_greenland.png");
	textures[98] = Texture::Get("data/Assets/Textures/GUI/vida_baja.png");
	textures[99] = Texture::Get("data/Assets/Textures/GUI/vida.png");
	Texture* t;
	t = Texture::Get("data/Assets/Textures/GUI/elegir mision1.png");
	t = Texture::Get("data/Assets/Textures/GUI/elegir mision2.png");
	t = Texture::Get("data/Assets/Textures/GUI/elegir mision3.png");
	t = Texture::Get("data/Assets/Textures/GUI/elegir mision4.png");

	BASS_Init(-1, 44100, 0, 0, NULL);
	theme = MyAudioBass::Get("data/Assets/Music/VillageTheme.wav");
	hit = MyAudioBass::Get("data/Assets/Music/Hit.wav");
	theme->PlaySoundAmbient(); 

	// Load Map
	my_world.loadScene("data/Assets/Village.txt");

	//hide the cursor
	SDL_ShowCursor(!mouse_locked); //hide or show the mouse	
}

void Game::render(void)
{
	//set the camera as default
	camera->enable();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	//// DRAW THE SKY ////
	glDisable(GL_DEPTH_TEST);
	Matrix44 m2;
	m2.setIdentity();

	shader_blanc->enable();

	shader_blanc->setUniform("u_color", Vector4(1, 1, 1, 1) * 0.6);
	shader_blanc->setUniform("u_viewprojection", camera->viewprojection_matrix);
	shader_blanc->setUniform("u_texture", textures[7]);
	shader_blanc->setUniform("u_model", m2);

	meshes[7]->render(GL_TRIANGLES);
	shader_blanc->disable();
	//////////////////////
	
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	//// DRAW THE PLAYER ////
	Player player = my_world.player;
	player.idle_anim->assignTime(time);
	float run_time = fmod(time, player.run_anim->duration);
	player.run_anim->assignTime(run_time);

	if (player.attack) {
		if (player.atk_time == 0.f) player.attack_anim->assignTime(0);
		player.atk_time += Game::instance->elapsed_time;
		player.attack_anim->assignTime(player.atk_time);
		blendSkeleton(&player.idle_anim->skeleton, &player.attack_anim->skeleton, sin(player.atk_time * (PI/ player.attack_anim->duration)), player.skeleton);
		renderAnimated(player.model, player.mesh, player.texture, player.skeleton);
		if (player.atk_time >= player.attack_anim->duration) {
			player.attack = false;
			player.atk_time = 0;
			for (int k = 0; k < my_world.enemies.size(); ++k) my_world.enemies[k].wasHit = false;
		}
	}
	else {
		float frac = player.speed / player.max_speed;
		blendSkeleton(&player.idle_anim->skeleton, &player.run_anim->skeleton, max(0, frac), player.skeleton);
		renderAnimated(player.model, player.mesh, player.texture, player.skeleton);
	}
	player.skeleton->updateGlobalMatrices();
	my_world.player = player;
	/////////////////////////
	
	//// DRAW THE PLAYER SWORD ////
	Entity espada = my_world.espada;
	espada.model = player.skeleton->getBoneMatrix("mixamorig_RightHandIndex2", false);
	espada.model = espada.model * player.model;
	espada.model.scale(0.75f, 0.75f, 0.75f);
	espada.model.translate(0.1, 0, 0.1);
	espada.model.rotate(80 * DEG2RAD, Vector3(1, 0, 0));
	espada.model.rotate(60 * DEG2RAD, Vector3(0, 1, 0));
	renderMesh(espada.model, espada.mesh, espada.texture);
	my_world.espada = espada;
	///////////////////////////////

	//// DRAW STORE CHARACTER ////
	my_world.tenosuke.dance_anim->assignTime(time);
	renderAnimated(my_world.tenosuke.model, meshes[10], textures[10], &my_world.tenosuke.dance_anim->skeleton);
	//////////////////////////////
	
	//// DRAW WORLD FLOOR ////
	Matrix44 m;
	Mesh* floor_mesh = new Mesh();
	floor_mesh->createPlane(2 * max(my_world.h, my_world.w));
	m.translateGlobal(my_world.h * 2, 0, -my_world.w * 2);
	Texture* floor_tex = Texture::Get("data/Assets/Textures/ground_plane2.png");
	renderMesh(m, floor_mesh, floor_tex);
	//////////////////////////

	//// DRAW WORLD ENEMIES ////
	for (int k = 0; k < my_world.enemies.size(); ++k) {
		Enemy eaux = my_world.enemies[k];
		if (eaux.life > 0.f) {
			eaux.idle_anim->assignTime(time);
			renderAnimated(eaux.model, eaux.mesh, eaux.texture, &eaux.idle_anim->skeleton);
			//renderMesh(eaux.model, eaux.mesh, eaux.texture);
			/*if (eaux.weapon_mesh != NULL) {
				eaux.weapon_model = eaux.skeleton->getBoneMatrix("mixamorig_RightHandIndex2", false);
				eaux.weapon_model = eaux.weapon_model * eaux.model;
				renderMesh(eaux.weapon_model, eaux.weapon_mesh, eaux.weapon_tex);
				my_world.enemies[k].weapon_model = eaux.weapon_model;
			}*/
		}
	}
	////////////////////////////

	my_world.renderMap(my_world.map, my_world.w, my_world.h,shader_instanced);
	renderMinimap(my_world.minimap);
	Stage::current_stage->render();

	//// DRAW GUI ELEMENTS ////
	//renderUI(2, textures[99]);
	//renderUI(0, textures[98]); // sangre vida

	//swap between front buffer and back buffer
	SDL_GL_SwapWindow(this->window);
}

void Game::update(double seconds_elapsed)
{
	Stage::current_stage->update(seconds_elapsed);

	if (mouse_locked)
		Input::centerMouse();
}

//Keyboard event handler (sync input)
void Game::onKeyDown( SDL_KeyboardEvent event )
{
	switch(event.keysym.sym)
	{
		case SDLK_ESCAPE: must_exit = true; break; //ESC key, kill the app
		case SDLK_F1: Shader::ReloadAll(); break; 
	}
}

void Game::onKeyUp(SDL_KeyboardEvent event)
{
}

void Game::onGamepadButtonDown(SDL_JoyButtonEvent event)
{

}

void Game::onGamepadButtonUp(SDL_JoyButtonEvent event)
{

}

void Game::onMouseButtonDown( SDL_MouseButtonEvent event )
{
	if (event.button == SDL_BUTTON_MIDDLE) //middle mouse
	{
		mouse_locked = !mouse_locked;
		SDL_ShowCursor(!mouse_locked);
	}
}

void Game::onMouseButtonUp(SDL_MouseButtonEvent event)
{
}

void Game::onMouseWheel(SDL_MouseWheelEvent event)
{
}

void Game::onResize(int width, int height)
{
    std::cout << "window resized: " << width << "," << height << std::endl;
	glViewport( 0,0, width, height );
	camera->aspect =  width / (float)height;
	window_width = width;
	window_height = height;
}