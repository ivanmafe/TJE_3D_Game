#include "game.h"
#include "utils.h"
#include "mesh.h"
#include "texture.h"
#include "fbo.h"
#include "shader.h"
#include "input.h"
#include "animation.h"

//own includes

#include "entity.h"
#include "world.h"
#include "stage.h"

#include <cmath>

//some globals
Shader* shader = NULL;
Shader* shader_instanced = NULL;
Animation* anim = NULL;
float angle = 0;

bool free_cam = true;
float speed = 0.05;
Entity player;
Entity dog;

///////////////////////
Entity casa;
///////////////////////

World my_world;

Game* Game::instance = NULL;


Mesh * meshes[20];
Texture * textures[20];



Game::Game(int window_width, int window_height, SDL_Window* window)
{
	this->window_width = window_width;
	this->window_height = window_height;
	this->window = window;
	instance = this;
	must_exit = false;

	player.speed = 0.05;


	fps = 0;
	frame = 0;
	time = 0.0f;
	elapsed_time = 0.0f;
	mouse_locked = false;

	//OpenGL flags
	glEnable( GL_CULL_FACE ); //render both sides of every triangle
	glEnable( GL_DEPTH_TEST ); //check the occlusions using the Z buffer

	//create our camera
	camera = new Camera();
	camera->lookAt(Vector3(0.2f,0.6f,-0.5f),Vector3(0.2f,0.f,1.f), Vector3(0.f,1.f,0.f)); //position the camera and point to 0,0,0
	camera->setPerspective(70.f,window_width/(float)window_height,0.1f,100000.f); //set the projection, we want to be perspective


	// example of shader loading using the shaders manager
	shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
	shader_instanced = Shader::Get("data/shaders/instanced.vs", "data/shaders/texture.fs");

	//SUELO
	meshes[0] = Mesh::Get("data/Assets/Meshes/ground.obj");
	textures[0] = Texture::Get("data/Assets/Textures/ground.png");
	//ESQUINA
	meshes[1] = Mesh::Get("data/Assets/Meshes/esquina_exterior_1.obj");
	textures[1] = Texture::Get("data/Assets/Textures/esquina_exterior_1.png");
	//ESQUINA INTERNA
	meshes[2] = Mesh::Get("data/Assets/Meshes/esquina_interna_x1.obj");
	textures[2] = Texture::Get("data/Assets/Textures/esquina_interna_x1.png");
	//PARED
	meshes[3] = Mesh::Get("data/Assets/Meshes/pared1_x1.obj");
	textures[3] = Texture::Get("data/Assets/Textures/pared1_x1.png");
	//OTROS
	meshes[4] = Mesh::Get("data/Assets/Meshes/cartel_misiones.obj");
	textures[4] = Texture::Get("data/Assets/Textures/cartel_misiones.png");
	meshes[5] = Mesh::Get("data/Assets/Meshes/vacio.obj");
	textures[5] = Texture::Get("data/Assets/Textures/vacio.png");
	meshes[6] = Mesh::Get("data/Assets/Meshes/casa1.obj");
	textures[6] = Texture::Get("data/Assets/Textures/casa1.png");

	player.mesh = Mesh::Get("data/Assets/Meshes/hero.obj");
	player.texture = Texture::Get("data/Assets/Textures/hero.tga");

	dog.mesh = Mesh::Get("data/Assets/Meshes/Dog.obj");
	dog.texture = Texture::Get("data/Assets/Textures/Dog.tga");


	casa.mesh = Mesh::Get("data/Assets/Meshes/casa1.obj");
	casa.texture = Texture::Get("data/Assets/Textures/casa1.png");
	
	/*
	meshes[9] = Mesh::Get("data/Assets/Meshes/Ghost.obj");
	textures[9] = Texture::Get("data/Assets/Textures/Ghost_Violet.tga");
	*/

	player.pos = Vector3(114.f, 0, -32.5f);
	player.setModelPos(Vector3(114.f, 0, -32.5f));

	casa.pos = Vector3(110.f, 0, -32.5f);
	casa.setModelPos(Vector3(110.f, 0, -32.5f));

	my_world.loadMap("data/Assets/Big_map.csv");

	//hide the cursor
	SDL_ShowCursor(!mouse_locked); //hide or show the mouse
}

void chooseModel(Matrix44 * m, int tile, int * index) {

	Vector3 pos = m->getTranslation();
	m->setIdentity();
	m->scale(1.f, 1.f, 1.f);
	/*
	if (tile == 11) *index = 0; //SUELO
	else if (tile == 8) *index = 7;
	
	else*/ if (tile == 0) { *index = 1; m->scale(1.f, 2.f, 1.f);} //ESQUINAS
	else if (tile == 2) { *index = 1; m->translate(0, 0, 0); m->rotate(90 * DEG2RAD, Vector3(0, 1, 0)); m->scale(1.f, 2.f, 1.f);}
	else if (tile == 20) { *index = 1; m->translate(0, 0, 0); m->rotate(-90 * DEG2RAD, Vector3(0, 1, 0)); m->scale(1.f, 2.f, 1.f);}
	else if (tile == 22) { *index = 1; m->translate(0, 0, 0); m->rotate(180 * DEG2RAD, Vector3(0, 1, 0)); m->scale(1.f, 2.f, 1.f); }
	
	else if (tile == 13) { *index = 2; m->translate(0, 0, 0); m->rotate(90 * DEG2RAD, Vector3(0, 1, 0)); m->scale(1.f, 2.f, 1.f);} //CONTRA-ESQUINAS
	else if (tile == 3) { *index = 2; m->translate(0, 0, 0); m->rotate(180 * DEG2RAD, Vector3(0, 1, 0)); m->scale(1.f, 2.f, 1.f);}
	else if (tile == 14) { *index = 2; m->scale(1.f, 2.f, 1.f);}
	else if (tile == 4) { *index = 2; m->translate(0, 0, 0); m->rotate(-90 * DEG2RAD, Vector3(0, 1, 0)); m->scale(1.f, 2.f, 1.f);}

	else if (tile == 10) { *index = 3; m->scale(1.f, 2.f, 1.f);}//PARED
	else if (tile == 21) { *index = 3; m->translate(0, 0, 0); m->rotate(-90 * DEG2RAD, Vector3(0, 1, 0)); m->scale(1.f, 2.f, 1.f);}
	else if (tile == 1) { *index = 3; m->translate(0, 0, 0); m->rotate(90 * DEG2RAD, Vector3(0, 1, 0)); m->scale(1.f, 2.f, 1.f);}
	else if (tile == 12) { *index = 3; m->translate(0, 0, 0); m->rotate(180 * DEG2RAD, Vector3(0, 1, 0)); m->scale(1.f, 2.f, 1.f);}

	else if (tile == 5) { *index = 4; } //CASCADA-ESQUINAS
	else if (tile == 6) { *index = 5; }
	else if (tile == 7) { *index = 6; m->scale(1.f, 1.2f, 1.f);}
	else *index = -1;

	m->translateGlobal(pos.x, pos.y, pos.z);
}

void renderMesh(Matrix44 m, Mesh* mesh, Texture* texture, int submesh = 0)
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
	//shader->setUniform("u_time", time);
	mesh->render(GL_TRIANGLES);

	//disable shader
	shader->disable();
}

void renderMap(int * map, int w, int h) {

	Camera* camera = Camera::current;

	std::vector<Matrix44> test;

	std::vector<std::vector<Matrix44>> models(20); 
	glDisable(GL_CULL_FACE);
	shader_instanced->enable();
	shader_instanced->setUniform("u_viewprojection", camera->viewprojection_matrix);
	shader_instanced->setUniform("u_color", Vector4(1, 1, 1, 1));
	shader_instanced->setUniform("u_light_direction", Vector3(10, 3, -13));

	int ind = 0;
	for (int i = 0; i < h; ++i)
		for (int j = 0; j < w; ++j) {
			Matrix44 m;
			m.translateGlobal(4.*i + 2, 0, -4.*j -2);
			int tmp = map[i * w + j];
			chooseModel(&m, tmp, &ind);
			if (ind != -1) models[ind].push_back(m);
		}

	for (int i = 0; i < models.size(); ++i){
		if (models[i].size() != 0) {
			shader_instanced->setUniform("u_texture", textures[i]);
			meshes[i]->renderInstanced(GL_TRIANGLES, &models[i][0], models[i].size());
		}
	}
	shader_instanced->disable();


}


Vector3 camera_offset = Vector3(0,0,-1);
//what to do when the image has to be draw
void Game::render(void)
{
	
	if(!free_cam){

		camera->eye = player.model * Vector3(0, 1.25, 1);   //0,1.3,0.5   //0.5 GUAY
		camera->center = player.model * Vector3(0, 0.7, -0.5);
	}
	
	//set the clear color (the background color)
	glClearColor(0.52, 0.8, 0.92, 1.0);

	// Clear the window and the depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//set the camera as default
	camera->enable();

	//set flags
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
   

	renderMesh(player.model, player.mesh, player.texture);
	renderMesh(dog.model, dog.mesh, dog.texture);
	renderMesh(casa.model, casa.mesh, casa.texture);

	Matrix44 m;
	m.scale(100, 0, 100);
	m.translate(0, -0.9f, 0);
	renderMesh(m, meshes[0], textures[0]);
	
	renderMap(my_world.map, my_world.w, my_world.h);

	//Draw the floor grid
	drawGrid();

	//render the FPS, Draw Calls, etc
	drawText(2, 2, getGPUStats(), Vector3(1, 1, 1), 2);

	//swap between front buffer and back buffer
	SDL_GL_SwapWindow(this->window);
}

void Game::update(double seconds_elapsed)
{

	if (Input::isKeyPressed(SDL_SCANCODE_V)) free_cam = !free_cam;

	if (!free_cam) {

		if (Input::isKeyPressed(SDL_SCANCODE_W)) {
			player.movePos(Vector3(0.f,0.f,-1.f) * player.speed);
			dog.movePos(Vector3(0.f, 0.f, -1.f) * dog.speed);
		}
		if (Input::isKeyPressed(SDL_SCANCODE_S)) {
			player.movePos(Vector3(0.f, 0.f, 1.f) * player.speed);
			dog.movePos(Vector3(0.f, 0.f, 1.f) * dog.speed);
		}
		if (Input::isKeyPressed(SDL_SCANCODE_A)) {
			
			player.changeView(-1.0f); dog.changeView(-1.0f);
			//player.movePos(Vector3(-1.f, 0.f, 0.f) * player.speed);
			//dog.movePos(Vector3(-1.f, 0.f, 0.f) * dog.speed);
		}
		if (Input::isKeyPressed(SDL_SCANCODE_D)) {
			//player.movePos(Vector3(1.f, 0.f, 0.f) * player.speed);
			//dog.movePos(Vector3(1.f, 0.f, 0.f) * dog.speed);
			player.changeView(1.0f); dog.changeView(1.0f);
		}


		if (Input::isKeyPressed(SDL_SCANCODE_LEFT)) {}
		if (Input::isKeyPressed(SDL_SCANCODE_RIGHT)) {}
		
		Vector3 aux = player.model * Vector3(0.3f, 0, 0.4f);
		dog.setModelPos(aux);

		//GET PLAYER POS
		if (Input::wasKeyPressed(SDL_SCANCODE_C)) {
			std::cout << "Player pos: " << player.pos.x << ',' << player.pos.y << ',' << player.pos.z << '\n';
		}
	}
	else {
		//mouse input to rotate the cam
		if ((Input::mouse_state & SDL_BUTTON_LEFT) || mouse_locked) //is left button pressed?
		{
			camera->rotate(Input::mouse_delta.x * 0.005f, Vector3(0.0f, -1.0f, 0.0f));
			camera->rotate(Input::mouse_delta.y * 0.005f, camera->getLocalVector(Vector3(-1.0f, 0.0f, 0.0f)));
		}
		if (Input::isKeyPressed(SDL_SCANCODE_W) || Input::isKeyPressed(SDL_SCANCODE_UP)) camera->move(Vector3(0.0f, 0.0f, 1.0f) * speed);
		if (Input::isKeyPressed(SDL_SCANCODE_S) || Input::isKeyPressed(SDL_SCANCODE_DOWN)) camera->move(Vector3(0.0f, 0.0f, -1.0f) * speed);
		if (Input::isKeyPressed(SDL_SCANCODE_A) || Input::isKeyPressed(SDL_SCANCODE_LEFT)) camera->move(Vector3(1.0f, 0.0f, 0.0f) * speed);
		if (Input::isKeyPressed(SDL_SCANCODE_D) || Input::isKeyPressed(SDL_SCANCODE_RIGHT)) camera->move(Vector3(-1.0f, 0.0f, 0.0f) * speed);
		
		//get camera values
		if (Input::wasKeyPressed(SDL_SCANCODE_C)) {
			std::cout << "Camera pos: " << camera->eye.x << ',' << camera->eye.y << ',' << camera->eye.z << '\n';
			std::cout << "Camera dir: " << camera->center.x << ',' << camera->center.y << ',' << camera->center.z << '\n';
		}
	}

	if (mouse_locked)
		Input::centerMouse();
}

//Keyboard event handler (sync input)
void Game::onKeyDown( SDL_KeyboardEvent event )
{
	switch(event.keysym.sym)
	{
		case SDLK_ESCAPE: must_exit = true; break; //ESC key, kill the app
		case SDLK_TAB: free_cam = !free_cam; break;
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