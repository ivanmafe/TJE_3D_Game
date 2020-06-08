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

#include "entity.h"
#include "entity_player.h"
#include "world.h"
#include "stage.h"
#include "data.h"


#include <cmath>

//shaders
Shader* shader = NULL;
Shader* shader_instanced = NULL;
Shader* shader_blanc = NULL;
Shader* anishader = NULL;

//some globals
Animation* anim = NULL;
float angle = 0;
bool free_cam = false;
bool inMenu = false;

//Active Entities
Player player;
Entity espada;
Entity dog;
Entity tenosuke;
Entity ghost;

//Data structures
std::vector<Entity> tiles;
std::vector<Entity> trees;
std::vector<std::vector<Matrix44>> models(20);
std::map<std::string, Stage*> Stage::stages;
Mesh * meshes[100];
Texture * textures[100];
Animation* tenosukedance;
Animation* herorun;
Animation* heroidle;
Animation* heroattack;
Skeleton* heroblend;


//World and Execution
World my_world;
Game* Game::instance = NULL;
Stage* Stage::current_stage = NULL;
PlayStage play;
DebugStage debug;
MenuStage menu;


std::vector<Entity> getNearEntities(float pos_x , float pos_y) {
	int x = (int)floor(pos_x) / 4;
	int y = (int)floor(pos_y) / 4;
	
	std::vector<Entity> nearby;


	nearby.push_back(tiles[x * my_world.w + y]);
	if (x-1 >= 0) nearby.push_back(tiles[(x - 1) * my_world.w + y]);
	if (x+1 < my_world.w ) nearby.push_back(tiles[(x + 1)  * my_world.w + y]);
	
	if (y - 1 >= 0) {
		nearby.push_back(tiles[x * my_world.w + (y - 1)]);
		if (x - 1 >= 0) nearby.push_back(tiles[(x - 1) * my_world.w + (y - 1)]);
		if (x + 1 < my_world.w) nearby.push_back(tiles[(x + 1)  * my_world.w + (y - 1)]);
	}
	if (y + 1 < my_world.w) {
		nearby.push_back(tiles[x * my_world.w + (y + 1)]);
		if (x - 1 >= 0) nearby.push_back(tiles[(x - 1) * my_world.w + (y + 1)]);
		if (x + 1 < my_world.w) nearby.push_back(tiles[(x + 1)  * my_world.w + (y + 1)]);
	}
	/*
	for(int i = 0 ; i < nearby.size() ; ++i)
		std::cout << "entit" << i << " x:" << nearby[i].model.getTranslation().x / 4 << " z:" << nearby[i].model.getTranslation().z / 4 << '\n';
		*/
	return nearby;
}

void generateMap(int * map, int w, int h) {

	int ind = 0;
	for (int i = 0; i < h; ++i)
		for (int j = 0; j < w; ++j) {
			Matrix44 m;
			m.translateGlobal(4.*i + 2, 0, -4.*j - 2);
			int tmp = map[i * w + j];
			chooseModel(&m, tmp, &ind);
			Entity e;
			e.mesh = meshes[ind];
			e.texture = textures[ind];
			e.model = m;
			//if(poner_cosas que no van al array)
			tiles.push_back(e);
			if (ind != -1) models[ind].push_back(m);
			
		}

	int aux_h = h * 4;
	int aux_w = w * 4;
	for (int x = 0; x < aux_h; x++)
		for (int z = 0; z < aux_w; z++) {
			if ((rand() % 100) < 94) {
				continue;
			}
			if (my_world.isCellEmpty(x, z)) {
				Entity ent;

				//float s = 10 + random() * 4.0;
				ent.model.rotate(random() * DEG2RAD, Vector3(0, 1, 0));
				ent.model.setTranslation(x, 0, -z);
				ent.model.scale(1.5, 1.5, 1.5);
				models[9].push_back(ent.model);
				ent.mesh = meshes[9];
				ent.texture = textures[9];
				trees.push_back(ent);
			}

		}


	for (int x = 0; x < aux_h; x++)
		for (int z = 0; z < aux_w; z++) {
			if ((rand() % 100) < 90) {
				continue;
			}
			Matrix44 model;
			model.setTranslation(x, 0, -z);
			model.rotate(random() * PI, Vector3(0, 1, 0));
			models[8].push_back(model);
		}
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
	textures[97] = Texture::Get("data/Assets/Textures/GUI/minimapa.png");
	textures[98] = Texture::Get("data/Assets/Textures/GUI/vida_baja.png");
	textures[99] = Texture::Get("data/Assets/Textures/GUI/vida.png");

	// Load Animations
	heroblend = new Skeleton();
	tenosukedance = Animation::Get("data/Assets/animaciones/tenosuke_idle.skanim");
	herorun = Animation::Get("data/Assets/animaciones/fast_run2.skanim");
	heroidle = Animation::Get("data/Assets/animaciones/heroe_idle.skanim");
	heroattack = Animation::Get("data/Assets/animaciones/hero_atack1.skanim");

	// Load Entities
	player = *new Player("data/Assets/Meshes/heroe.mesh", "data/Assets/Textures/hero.tga", Vector3(10.4f, 0, -10.8f));
	espada = *new Entity("data/Assets/Meshes/espada1.obj", "data/Assets/Textures/espada1.png");
	dog = *new Entity("data/Assets/Meshes/Dog.obj", "data/Assets/Textures/Dog.tga");
	ghost = *new Entity("data/Assets/Meshes/Ghost.obj", "data/Assets/Textures/Ghost_Violet.tga", Vector3(20.f, 0, -26.f), -60, 0.75);
	tenosuke = *new Entity(Vector3(17.2f, 0, -28.9f), -60, 0.25);

	// Load Map
	my_world.loadMap("data/Assets/pueblo.csv");
	generateMap(my_world.map, my_world.w, my_world.h);
	//hide the cursor
	SDL_ShowCursor(!mouse_locked); //hide or show the mouse
}

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
	anishader->setUniform("u_light_direction", Vector3(0.3,0.6,0.2).normalize());

	mesh->renderAnimated(GL_TRIANGLES, skeleton);

	anishader->disable();


}

void renderUI(int cuadrante, Texture* tex, float relation) {
	glDisable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	Mesh quad;

	if (cuadrante == 0) {

		//existia la funcion quad.createQuad(centro, ancho, alto (w/ float h), invertir evs)...
		//o pasandole pixels y una camara asi:
		/*
		glDisable(GL_CULL_FACE)
		Camera cam2D;
		cam2D.setOrthographic(0,w_w,w_h,0,-1,1);
		cam2D.enable();
		shader->setUniform("u_model", matrix44());
		shader->setUniform("u_viewprojection", cam2D.viewprojection_matrix);
		
		*/


		/*quad.vertices.push_back(Vector3(-1, 1, 0));
		quad.uvs.push_back(Vector2(0, 1));
		quad.vertices.push_back(Vector3(-1, -1, 0));
		quad.uvs.push_back(Vector2(0, 0));
		quad.vertices.push_back(Vector3(1, 1, 0));
		quad.uvs.push_back(Vector2(1, 1));

		quad.vertices.push_back(Vector3(1, 1, 0));
		quad.uvs.push_back(Vector2(1, 1));
		quad.vertices.push_back(Vector3(-1, -1, 0));
		quad.uvs.push_back(Vector2(0, 0));
		quad.vertices.push_back(Vector3(1, -1, 0));
		quad.uvs.push_back(Vector2(1, 0));
		*/

		quad.createQuad(0, 0, 2, 2 , false);
	}
	else if (cuadrante == 1) {
		
		quad.createQuad(0.5, 0.5, 1, 1, false);
	}
	else if (cuadrante == 2) {
		
		quad.createQuad(-0.5, 0.5, 1, 1, false);
	}
	else if (cuadrante == 3) {
		
		quad.createQuad(-0.5, -0.5, 1, 1, false);
		
	}
	else if (cuadrante == 4) {
		
		quad.createQuad(0.5, -0.5, 1, 1, false);
	}
	Shader* shader = Shader::Get("data/shaders/quad.vs", "data/shaders/texture.fs");//flat.fs");
	shader->enable();
	shader->setUniform("u_color", Vector4(1,1,1,1));
	shader->setUniform("u_texture", tex);
	//shader->setUniform("u_texture_tiling", 1.0f);
	quad.render(GL_TRIANGLES);
	shader->disable();
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

void renderMap(int * map, int w, int h) {

	Camera* camera = Camera::current;

	//glDisable(GL_CULL_FACE);
	shader_instanced->enable();
	shader_instanced->setUniform("u_viewprojection", camera->viewprojection_matrix);
	shader_instanced->setUniform("u_color", Vector4(1, 1, 1, 1));
	shader_instanced->setUniform("u_light_direction", Vector3(10, 3, -13));
	shader_instanced->setUniform("u_camera_position", camera->eye);

	for (int i = 0; i < models.size(); ++i){
		if (models[i].size() != 0) {
			shader_instanced->setUniform("u_texture", textures[i]);
			meshes[i]->renderInstanced(GL_TRIANGLES, &models[i][0], models[i].size());
		}
	}

	shader_instanced->disable();
}

bool attack = false;
float atk_time = 0.f;
//what to do when the image has to be draw
void Game::render(void)
{
	if(!free_cam){

		camera->eye = player.model * Vector3(0, 1.25, 1);   //0,1.3,0.5   //0.5 GUAY
		camera->center = player.model * Vector3(0, 0.7, -0.5);
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//set the camera as default
	camera->enable();


	////////////////////////////////////////////////////////////////////// pintar cielo
	glDisable(GL_DEPTH_TEST);
	Matrix44 m2;
	m2.setIdentity();

	//enable shader
	shader_blanc->enable();

	//upload uniforms
	shader_blanc->setUniform("u_color", Vector4(1, 1, 1, 1) * 0.6);
	shader_blanc->setUniform("u_viewprojection", camera->viewprojection_matrix);
	shader_blanc->setUniform("u_texture", textures[7]);
	shader_blanc->setUniform("u_model", m2);

	meshes[7]->render(GL_TRIANGLES);
	shader_blanc->disable();

	//////////////////////////////////////////////////////////////////////

	//set flags
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
   
	//renderMesh(player.model, player.mesh, player.texture);
	renderMesh(dog.model, dog.mesh, dog.texture);
	if(ghost.life > 0.f) renderMesh(ghost.model, ghost.mesh, ghost.texture);

	heroidle->assignTime(time);
	float run_time = fmod(time, herorun->duration);
	herorun->assignTime(run_time);

	if (attack) {
		if (atk_time == 0.f) heroattack->assignTime(0);
		atk_time += Game::instance->elapsed_time;
		heroattack->assignTime(atk_time);
		blendSkeleton(&heroidle->skeleton, &heroattack->skeleton, sin(atk_time * (PI/ heroattack->duration)), heroblend);
		renderAnimated(player.model, player.mesh, player.texture, heroblend);
		if (atk_time >= heroattack->duration) { 
			attack = false; 
			atk_time = 0;
			ghost.wasHit = false;
		}
	}
	else {
		float frac = player.speed / player.max_speed;
		blendSkeleton(&heroidle->skeleton, &herorun->skeleton, max(0, frac), heroblend);
		renderAnimated(player.model, player.mesh, player.texture, heroblend);
	}

	heroblend->updateGlobalMatrices();
	//mixamorig_RightHandIndex2
	espada.model = heroblend->getBoneMatrix("mixamorig_RightHandIndex2", false);
	espada.model = espada.model * player.model;
	espada.model.scale(0.25, 0.25, 0.25);
	espada.model.translate(0.1, 0, 0.1);
	espada.model.rotate(80 * DEG2RAD, Vector3(1, 0, 0));
	espada.model.rotate(60 * DEG2RAD, Vector3(0, 1, 0));
	renderMesh(espada.model, espada.mesh, espada.texture);
	
	renderAnimated(tenosuke.model, meshes[10], textures[10], &tenosukedance->skeleton);
	tenosukedance->assignTime(time);
	//tenosukedance->skeleton.renderSkeleton(camera, tenosuke->model);

	//Skeleton result; //no hacer en local
	//blendSkeleton(&tenosukedance->skeleton, &anim2->skeleton, 0.5, &result); //para fusionar animaciones
	//result.renderSkeleton(camera, tenosuke.model);
	
	// Render World Floor
	Matrix44 m;
	Mesh* floor_mesh = new Mesh();
	floor_mesh->createPlane(2 * max(my_world.h, my_world.w));
	m.translateGlobal(my_world.h * 2, 0, -my_world.w * 2);
	Texture* floor_tex = Texture::Get("data/Assets/Textures/ground_plane2.png");
	renderMesh(m, floor_mesh, floor_tex);

	renderMap(my_world.map, my_world.w, my_world.h);

	//DRAW UI OR STAGE SPECIFIC ELEMENTS
	Stage::current_stage->render();




	
	
	


	//////////////////////////////////////////menu vida
	float aux = (window_width / (float)window_height);
	renderUI(2, textures[99], aux);
	renderUI(4, textures[97], aux);
	//renderUI(0, textures[98], aux);

	
	
	
	///personaje en mini mapa

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	Mesh quad;

	float x = -((player.pos.z /220)-1.34);//bastante bien
	float y = (player.pos.x / (60 * -2.7))  +0.85 ;


	quad.vertices.push_back(Vector3(-0.98f +x, (-0.96f + y) , 0));
	quad.vertices.push_back(Vector3(-1 +x, (-1 + y), 0));
	quad.vertices.push_back(Vector3(-0.96f +x , (-1 + y) , 0));

	Shader* shader = Shader::Get("data/shaders/quad.vs", "data/shaders/flat.fs");
	shader->enable();
	shader->setUniform("u_color", Vector4(0, 0,1, 1));
	quad.render(GL_TRIANGLES);


	///////////////////////////////////////// sangre vida
	//renderUI(0, textures[98]);
	///////////////////////////////////////////

	//swap between front buffer and back buffer
	SDL_GL_SwapWindow(this->window);
}


void Game::update(double seconds_elapsed)
{

	if (Input::wasKeyPressed(SDL_SCANCODE_E)) {
		attack = true;
		player.speed = 0.f;
	}


	if (Input::wasKeyPressed(SDL_SCANCODE_V)) {
		if (free_cam) { Stage::current_stage->changeStage("MenuStage"); inMenu = true; }
		else { Stage::current_stage->changeStage("DebugStage");  inMenu = false; }
		free_cam = !free_cam;
	}

	if (Input::wasKeyPressed(SDL_SCANCODE_M)) {
		if (inMenu)  Stage::current_stage->changeStage("PlayStage"); 
		else Stage::current_stage->changeStage("MenuStage");
		inMenu = !inMenu;
	}

	if (!ghost.wasHit && attack && ghost.mesh->testSphereCollision(ghost.model, espada.model.getTranslation(), 0.5, Vector3(), Vector3()) ) {
		ghost.wasHit = true;
		ghost.life -= player.light_atk;
		std::cout << "Ouch!!!\n" ;
	}

	if (!free_cam && !inMenu && !attack) {

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		Vector3 targetpos = player.pos;
		Matrix44 R;
		R.setRotation(player.angle * DEG2RAD, Vector3(0, 1, 0));
		Vector3 front = R.rotateVector(Vector3(0, 0, -1));
		Vector3 Right = R.rotateVector(Vector3(1, 0, -0));


		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		Stage::current_stage->update();

		if (Input::isKeyPressed(SDL_SCANCODE_W)) {
			player.momentum = 1;
			if (player.speed < player.max_speed) player.speed += 0.07f;
			targetpos = player.pos + front * player.speed * seconds_elapsed;
		}
		else if (Input::isKeyPressed(SDL_SCANCODE_S)) {
			player.momentum = -1;
			if (player.speed < player.max_speed) player.speed += 0.07f;
			targetpos = player.pos - front * player.speed * seconds_elapsed;
		}
		else {
			if (player.speed > 0.f) {
				targetpos = player.pos + (front * player.momentum) * player.speed * seconds_elapsed;
				player.speed -= 0.05;
			} 
		}
		




		if (Input::isKeyPressed(SDL_SCANCODE_A)) {
			player.angle -= 90 * seconds_elapsed;
		}if (Input::isKeyPressed(SDL_SCANCODE_D)) {
			player.angle += 90 * seconds_elapsed;
		}


		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//COLISIONES	
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
		//calculamos el centro de la esfera de colisión del player elevandola hasta la cintura
		Vector3 character_center = targetpos + Vector3(0, 0.3, 0);

		bool hascollision = false;

		//para cada objecto de la escena...
		
		std::vector<Entity> collidable = getNearEntities(player.pos.x, -player.pos.z);
		collidable.insert(collidable.end(), trees.begin(), trees.end());
		if (ghost.life > 0.f) collidable.push_back(ghost);
		int size = collidable.size();

		for (int i = 0; i < size; i++) {

			Entity ent = collidable[i];
			Vector3 v = ent.model.getTranslation();
			if (ent.mesh == NULL) continue;
			Mesh* mesh = ent.mesh;


			//comprobamos si colisiona el objeto con la esfera (radio 3)
			Vector3 coll;
			Vector3 collnorm;
			if (mesh->testSphereCollision(ent.model, character_center, 0.2, coll, collnorm) == false)
				continue; //si no colisiona, pasamos al siguiente objeto
			hascollision = true;
			Vector3 contrapush = normalize(coll - character_center) * seconds_elapsed;   ///faltaria interpolar la posicion actual con la del rebote para que no tiemble tanto

			targetpos = player.pos - contrapush;
			targetpos.y = 0;
			break;
			//si la esfera está colisionando muevela a su posicion anterior alejandola del objeto


		}
		player.pos = targetpos;

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


		player.model.setIdentity();
		player.model.setTranslation(player.pos.x, player.pos.y, player.pos.z);
		player.model.rotate(player.angle * DEG2RAD, Vector3(0, 0.5f, 0));


		if (Input::isKeyPressed(SDL_SCANCODE_LEFT)) {}
		if (Input::isKeyPressed(SDL_SCANCODE_RIGHT)) {}
		
		dog.pos = player.pos - Vector3(0.3f, 0, 0.4f);
		dog.model = player.model;
		Vector3 aux = player.model * Vector3(0.3f, 0, 0.4f);
		dog.setModelPos(aux);

		//GET PLAYER POS
		if (Input::wasKeyPressed(SDL_SCANCODE_C)) {
			std::cout << "Player pos: " << player.pos.x << ',' << player.pos.y << ',' << player.pos.z << '\n';
			std::cout << player.speed << '\n';
		}
	}
	else Stage::current_stage->update();
	

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