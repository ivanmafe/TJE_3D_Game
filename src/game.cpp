#include "game.h"
#include "utils.h"
#include "mesh.h"
#include "texture.h"
#include "fbo.h"
#include "shader.h"
#include "input.h"
#include "animation.h"

//own includes
#include <fstream>
#include <sstream>
#include "entity.h"


#include <cmath>

//some globals
Shader* shader = NULL;
Animation* anim = NULL;
float angle = 0;

bool free_cam = true;
float speed = 0.05;
Entity player;

Game* Game::instance = NULL;


int* readCSV(std::string filesrc, int size) { //archivo y tamaño de area
	int pos = 0;
	int* mapborder = new int[size];
	std::fstream file;
	file.open(filesrc, std::fstream::in);
	if (!file.is_open()) {
		fprintf(stderr, "Error locating the file map");
	}
	int aux = 0;
	while (file.good()) {
		std::string line;
		while (getline(file, line)) {   // get a whole line
			std::stringstream ss(line);
			while (getline(ss, line, ',')) {
				int aux;
				std::istringstream(line) >> aux;
				mapborder[pos++] = aux;
			}
		}
	}
	return mapborder;
}

Mesh * meshes[20];
Texture * textures[20];

const int w = 7;
const int h = 5;
int map[w * h];


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

	//OpenGL flags
	glEnable( GL_CULL_FACE ); //render both sides of every triangle
	glEnable( GL_DEPTH_TEST ); //check the occlusions using the Z buffer

	//create our camera
	camera = new Camera();
	camera->lookAt(Vector3(0.2f,0.6f,-0.5f),Vector3(0.2f,0.f,1.f), Vector3(0.f,1.f,0.f)); //position the camera and point to 0,0,0
	camera->setPerspective(70.f,window_width/(float)window_height,0.1f,100000.f); //set the projection, we want to be perspective



	// example of shader loading using the shaders manager
	shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");


	//SUELO
	meshes[0] = Mesh::Get("data/Assets/Meshes/vacio.obj");
	textures[0] = Texture::Get("data/Assets/Textures/vacio.png");
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
	meshes[5] = Mesh::Get("data/Assets/Meshes/charco.obj");
	textures[5] = Texture::Get("data/Assets/Textures/charco.png");
	meshes[6] = Mesh::Get("data/Assets/Meshes/casa1.obj");
	textures[6] = Texture::Get("data/Assets/Textures/casa1.png");

	meshes[8] = Mesh::Get("data/Assets/Meshes/Hero.obj");
	textures[8] = Texture::Get("data/Assets/Textures/hero.tga");

	meshes[9] = Mesh::Get("data/Assets/Meshes/Ghost.obj");
	textures[9] = Texture::Get("data/Assets/Textures/Ghost_Violet.tga");

	memcpy(&map, readCSV("data/Assets/mapa_3d.csv", (w * h)), w * h * sizeof(int));

	//hide the cursor
	SDL_ShowCursor(!mouse_locked); //hide or show the mouse
}

void chooseModel(Matrix44 * m, int tile, int * index) {

	Vector3 pos = m->getTranslation();
	m->setIdentity();
	/*
	if (tile == 11) *index = 0; //SUELO
	else if (tile == 8) *index = 7;

	else*/ if (tile == 0) { *index = 1; } //ESQUINAS
	else if (tile == 2) { *index = 1; m->translate(0, 0, 0); m->rotate(90 * DEG2RAD, Vector3(0, 1, 0)); }
	else if (tile == 20) { *index = 1; m->translate(0, 0, 0); m->rotate(-90 * DEG2RAD, Vector3(0, 1, 0));}
	else if (tile == 22) { *index = 1; m->translate(0, 0, 0); m->rotate(180 * DEG2RAD, Vector3(0, 1, 0)); }
	
	else if (tile == 13) { *index = 2; m->translate(0, 0, 0); m->rotate(90 * DEG2RAD, Vector3(0, 1, 0)); } //CONTRA-ESQUINAS
	else if (tile == 3) { *index = 2; m->translate(0, 0, 0); m->rotate(180 * DEG2RAD, Vector3(0, 1, 0));}
	else if (tile == 14) { *index = 2; }
	else if (tile == 4) { *index = 2; m->translate(0, 0, 0); m->rotate(-90 * DEG2RAD, Vector3(0, 1, 0));}

	else if (tile == 10) { *index = 3; } //PARED
	else if (tile == 21) { *index = 3; m->translate(0, 0, 0); m->rotate(-90 * DEG2RAD, Vector3(0, 1, 0)); }
	else if (tile == 1) { *index = 3; m->translate(0, 0, 0); m->rotate(90 * DEG2RAD, Vector3(0, 1, 0)); }
	else if (tile == 12) { *index = 3; m->translate(0, 0, 0); m->rotate(180 * DEG2RAD, Vector3(0, 1, 0)); }

	else if (tile == 5) { *index = 4; } //CASCADA-ESQUINAS
	else if (tile == 6) { *index = 5; }
	else if (tile == 7) { *index = 6; }
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
	shader->setUniform("u_light_direction", Vector3(-2, 2, 3));
	//shader->setUniform("u_time", time);
	mesh->render(GL_TRIANGLES);

	//disable shader
	shader->disable();
}

void renderMap(int * map, int w, int h) {

	std::vector<Matrix44> models; 

	int ind = 0;
	for (int i = 0; i < h; ++i)
		for (int j = 0; j < w; ++j) {
			Matrix44 m;
			m.translateGlobal(4.*i + 2, 0, -4.*j -2);
			int tmp = map[i * w + j];
			chooseModel(&m, tmp, &ind);
			if(ind != -1)renderMesh(m, meshes[ind], textures[ind]);

		}
	//meshes[0]->renderInstanced(GL_TRIANGLES, -1 , );
}

//what to do when the image has to be draw
void Game::render(void)
{

	/*
	Camera* camera = Camera::current;

	Vector3 eye = camera->eye;
	Vector3 center = camera->center;
	Vector3 up = camera->up;

	if(!free_cam)
		camera->lookAt( eye , center, up); //position the camera and point to 0,0,0
		*/
	//set the clear color (the background color)
	glClearColor(0.0, 0.0, 0.0, 1.0);

	// Clear the window and the depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//set the camera as default
	camera->enable();

	//set flags
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
   


	//create model matrix for cube
	renderMesh(player.model, meshes[8], textures[8]);
	Matrix44 m;
	m.scale(100, 0, 100);
	m.translate(0, -1.0f, 0);
	renderMesh(m, meshes[0], textures[0]);
	renderMap(map,w,h);

/*
	Matrix44 m2;
	m2.scale(0.2, 0.2, 0.2);
	m2.translate(0, 1.2, 0);

	renderMesh(m2, meshes[9], textures[9]);

//renderMap(map, w, h);
	
	Matrix44 m1;
	m1.translate(2.0f, -0.35f, -2.0f);
	renderMesh(m1, meshes[10], textures[10]);
*/


	//Draw the floor grid
	drawGrid();

	//render the FPS, Draw Calls, etc
	drawText(2, 2, getGPUStats(), Vector3(1, 1, 1), 2);

	//swap between front buffer and back buffer
	SDL_GL_SwapWindow(this->window);
}

void Game::update(double seconds_elapsed)
{
	if (Input::isKeyPressed(SDL_SCANCODE_TAB)) free_cam = !free_cam;
	if (!free_cam) {
		if (Input::isKeyPressed(SDL_SCANCODE_W)) {
			player.movePos(Vector3(0.f,0.f,-1.f) * player.speed);
		}
		if (Input::isKeyPressed(SDL_SCANCODE_S)) {
			player.movePos(Vector3(0.f, 0.f, 1.f) * player.speed);
		}
		if (Input::isKeyPressed(SDL_SCANCODE_A)) {
			player.movePos(Vector3(-1.f, 0.f, 0.f) * player.speed);
		}
		if (Input::isKeyPressed(SDL_SCANCODE_D)) {
			player.movePos(Vector3(1.f, 0.f, 0.f) * player.speed);
		}
		if (Input::isKeyPressed(SDL_SCANCODE_LEFT)) player.changeView(-1.0f);
		if (Input::isKeyPressed(SDL_SCANCODE_RIGHT)) player.changeView(1.0f);
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
			//std::cout << "Camera pos: " << camera->eye.x << ',' << camera->eye.y << ',' << camera->eye.z << '\n';
			//std::cout << "Camera dir: " << camera->center.x << ',' << camera->center.y << ',' << camera->center.z << '\n';
			std::cout << "Player pos: " << player.pos.x << ',' << player.pos.y << ',' << player.pos.z << '\n';
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

