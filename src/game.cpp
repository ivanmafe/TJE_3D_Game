#include "game.h"
#include "utils.h"
#include "mesh.h"
#include "texture.h"
#include "fbo.h"
#include "shader.h"
#include "input.h"
#include "animation.h"

#include "entity.h"
#include "world.h"
#include "stage.h"

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
bool isMenu = false;

//Active Entities
Entity player;
Entity dog;
Entity ghost;
Entity tenosuke;

//Data structures
std::vector<Entity> tiles;
std::vector<Entity> trees;
std::vector<std::vector<Matrix44>> models(20);
std::map<std::string, Stage*> Stage::stages;
Mesh * meshes[100];
Texture * textures[100];
Animation* tenosukedance;
Animation* herorun;


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


	for (int x = 0; x < 128; x++) 
		for (int z = 0; z < 128; z++) {
			if ((rand() % 100) < 98) {
				continue;
			}
			if (my_world.isCellEmpty(x, z)) {
				Entity ent;
				ent.model.setTranslation(x, 0, -z);
				//float s = 10 + random() * 4.0;
				ent.model.rotate(random() * PI, Vector3(0, 1, 0));
				ent.model.scale(1.5, 1.5, 1.5);
				models[9].push_back(ent.model);
				ent.mesh = meshes[9];
				ent.texture = textures[9];
				trees.push_back(ent);
			}

		}


	for (int x = 0; x < 128; x++)
		for (int z = 0; z < 128; z++) {
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

	player.speed = 5;

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
	shader_blanc = Shader::Get("data/shaders/basic.vs", "data/shaders/sky.fs");

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


	meshes[7] = Mesh::Get("data/Assets/Meshes/cielo.ase");
	textures[7] = Texture::Get("data/Assets/Textures/Dani_sky2.tga");




	meshes[8] = Mesh::Get("data/Assets/Meshes/hierba.obj");
	textures[8] = Texture::Get("data/Assets/Textures/hierba.png");

	meshes[9] = Mesh::Get("data/Assets/Meshes/arbol verde.obj");
	textures[9] = Texture::Get("data/Assets/Textures/arbol_verde.png");


	
	meshes[10] = Mesh::Get("data/Assets/Meshes/tenosuke_character.mesh");
	textures[10] = Texture::Get("data/Assets/Textures/tenosuke_piel.png");
	tenosukedance = Animation::Get("data/Assets/animaciones/tenosuke_dancing.skanim");


	player.mesh = Mesh::Get("data/Assets/Meshes/heroe.mesh");
	player.texture = Texture::Get("data/Assets/Textures/hero.tga");

	herorun = Animation::Get("data/Assets/animaciones/heroe_fastrun.skanim");

	dog.mesh = Mesh::Get("data/Assets/Meshes/Dog.obj");
	dog.texture = Texture::Get("data/Assets/Textures/Dog.tga");

	ghost.mesh  = Mesh::Get("data/Assets/Meshes/Ghost.obj");
	ghost.texture = Texture::Get("data/Assets/Textures/Ghost_Violet.tga");

	textures[97] = Texture::Get("data/Assets/Textures/minimapa.png");
	textures[98] = Texture::Get("data/Assets/Textures/vida_baja.png");
	textures[99] = Texture::Get("data/Assets/Textures/vida.png");

	
	player.pos = Vector3(114.f, 0, -32.5f);
	player.setModelPos(Vector3(114.f, 0, -32.5f));

	ghost.pos = Vector3(109.f, 0, -36.f);
	ghost.setModelPos(Vector3(109.f, 0, -36.f));
	ghost.model.rotate(-60 * DEG2RAD, Vector3(0, 1, 0));
	ghost.model.scale(0.75, 0.75, 0.75);

	tenosuke.pos = Vector3(80.f, 0, -36.f);
	tenosuke.setModelPos(Vector3(80.f, 0, -36.f));
	tenosuke.model.scale(0.25, 0.25, 0.25);

	my_world.loadMap("data/Assets/Big_map.csv");
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
	shader->setUniform("u_color", Vector4(1, 1, 1, 1));
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


	//Render World Sky
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	Matrix44 m2;
	m2.setIdentity();
	shader_blanc->enable();
	shader_blanc->setUniform("u_color", Vector4(1, 1, 1, 1) * 0.6);
	shader_blanc->setUniform("u_viewprojection", camera->viewprojection_matrix);
	shader_blanc->setUniform("u_texture", textures[7]);
	shader_blanc->setUniform("u_model", m2);
	meshes[7]->render(GL_TRIANGLES);
	shader_blanc->disable();

	// Render World Floor
	Matrix44 m;
	Mesh* floor_mesh = new Mesh();
	floor_mesh->createPlane(2 * max(my_world.h, my_world.w));
	m.translateGlobal(my_world.h * 2, 0, -my_world.w * 2);
	Texture* floor_tex = Texture::Get("data/Assets/Textures/ground_plane3.png");
	renderMesh(m, floor_mesh, floor_tex);

	//Render Characters

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
   
	//renderMesh(player.model, player.mesh, player.texture);
	renderMesh(dog.model, dog.mesh, dog.texture);
	renderMesh(ghost.model, ghost.mesh, ghost.texture);

	herorun->assignTime(time);
	renderAnimated(player.model, player.mesh, player.texture, &herorun->skeleton);
	
	renderAnimated(tenosuke.model, meshes[10], textures[10], &tenosukedance->skeleton);
	tenosukedance->assignTime(time);
	//tenosukedance->skeleton.renderSkeleton(camera, tenosuke.model);

	//Skeleton result; //no hacer en local
	//blendSkeleton(&tenosukedance->skeleton, &anim2->skeleton, 0.5, &result); //para fusionar animaciones
	//result.renderSkeleton(camera, tenosuke.model);
	/*

	m.scale(100, 0, 100);
	m.translate(0, -0.9f, 0);
	renderMesh(m, meshes[0], textures[0]);
	*/



	// Render Tile Map 3D
	renderMap(my_world.map, my_world.w, my_world.h);


	//DRAW UI OR STAGE SPECIFIC ELEMENTS
	Stage::current_stage->render();

	//////////////////////////////////////////menu vida
	float aux = (window_width / (float)window_height);
	renderUI(2, textures[99], aux);
	renderUI(0, textures[97],aux);

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


	if (Input::wasKeyPressed(SDL_SCANCODE_V)) {
		if (free_cam) {
			Stage::current_stage->changeStage("MenuStage");
			free_cam = false;
		}
		else {
			Stage::current_stage->changeStage("DebugStage");
			isMenu = false;
			free_cam = true;
		}
	}

	if (Input::wasKeyPressed(SDL_SCANCODE_M)) {
		if (free_cam) {
			Stage::current_stage->changeStage("MenuStage");
			free_cam = false;
		}
		else if (!isMenu) {
			Stage::current_stage->changeStage("MenuStage");
			isMenu = true;
		}
		else {
			Stage::current_stage->changeStage("PlayStage");
			isMenu = false;
		}
	}

	if (!free_cam && !isMenu) {

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		Vector3 targetpos = player.pos; 
		Matrix44 R;
		R.setRotation(player.angle * DEG2RAD, Vector3(0, 1, 0));
		Vector3 front = R.rotateVector(Vector3(0, 0, -1));
		Vector3 Right = R.rotateVector(Vector3(1, 0, -0));


		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		Stage::current_stage->update();


		if (Input::isKeyPressed(SDL_SCANCODE_W)) {
			targetpos = player.pos + front * player.speed * seconds_elapsed;
		}
		if (Input::isKeyPressed(SDL_SCANCODE_S)) {
			targetpos = player.pos - front * player.speed * seconds_elapsed;
		}
		if (Input::isKeyPressed(SDL_SCANCODE_A)) {
			player.angle -= 90 * seconds_elapsed;
		}
		if (Input::isKeyPressed(SDL_SCANCODE_D)) {
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
		collidable.push_back(ghost);
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