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
#include "entity_enemy.h"
#include "world.h"
#include "stage.h"
#include "play_stage.h"
#include "menu_stage.h"
#include "debug_stage.h"
#include "data.h"
#include <bass.h>
#include "AudioBass.h"
#include <cmath>

//shaders
Shader* shader = NULL;
Shader* shader_instanced = NULL;
Shader* shader_blanc = NULL;
Shader* anishader = NULL;

//some globals
bool free_cam = false;
bool inMenu = false;


//Data structures
std::map<std::string, Stage*> Stage::stages;

MyAudioBass* sample;

//World and Execution
Game* Game::instance = NULL;
Stage* Stage::current_stage = NULL;
PlayStage play;
DebugStage debug;
MenuStage menu;

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

void renderMinimap(Texture* tex) {

	Player player = Game::instance->my_world.player;

	glDisable(GL_DEPTH_TEST);
	Mesh quad;
	quad.createQuad(0.7, -0.7, 0.4, 0.5, false);
	Shader* shader = Shader::Get("data/shaders/quad.vs", "data/shaders/GUI.fs");
	shader->enable();
	shader->setUniform("u_color", Vector4(1, 1, 1, 1));
	shader->setUniform("u_texture", tex);
	quad.render(GL_TRIANGLES);
	shader->disable();

	glEnable(GL_CULL_FACE);
	Mesh quad2;

	float x = -((player.pos.z / (Game::instance->my_world.h * 2)));
	float y = (player.pos.x / -(Game::instance->my_world.h * 2)) + 2;

	x = (x * 0.2f) + 1.5f;
	y = (y * 0.25f) + 0.05f;

	quad2.vertices.push_back(Vector3(-1 + x, (-0.99f + y), 0));
	quad2.vertices.push_back(Vector3(-1.01f + x, (-1.01f + y), 0));
	quad2.vertices.push_back(Vector3(-0.99f + x, (-1.01f + y), 0));


	Shader* shader2 = Shader::Get("data/shaders/quad.vs", "data/shaders/GUI.fs");
	shader2->enable();
	shader2->setUniform("u_color", Vector4(0, 0, 1, 1));
	quad2.render(GL_TRIANGLES);
	shader2->disable();
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

		quad.createQuad(0, 0, 2, 2, false);
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
	Shader* shader = Shader::Get("data/shaders/quad.vs", "data/shaders/GUI.fs");//flat.fs");
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
	textures[96] = Texture::Get("data/Assets/Textures/GUI/minimap_pueblo.png");
	textures[97] = Texture::Get("data/Assets/Textures/GUI/minimap_greenland.png");
	textures[98] = Texture::Get("data/Assets/Textures/GUI/vida_baja.png");
	textures[99] = Texture::Get("data/Assets/Textures/GUI/vida.png");

	BASS_Init(-1, 44100, 0, 0, NULL);
	sample = MyAudioBass::Get("data/Assets/Music/VillageTheme.wav");
	//sample->PlaySoundAmbient();

	// Load Map
	my_world.loadScene("data/Assets/scene.txt");

	//hide the cursor
	SDL_ShowCursor(!mouse_locked); //hide or show the mouse	
}

//what to do when the image has to be draw
void Game::render(void)
{
	Player player = my_world.player;
	if(!free_cam){
		camera->eye = Vector3(player.pos.x + sin(player.angle), 1.25, player.pos.z + cos(player.angle));
		camera->center = player.pos + Vector3(0,0.7,0);
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
	//mixamorig_RightHandIndex2
	Entity espada = my_world.espada;
	espada.model = player.skeleton->getBoneMatrix("mixamorig_RightHandIndex2", false);
	espada.model = espada.model * player.model;
	espada.model.scale(0.75f, 0.75f, 0.75f);
	espada.model.translate(0.1, 0, 0.1);
	espada.model.rotate(80 * DEG2RAD, Vector3(1, 0, 0));
	espada.model.rotate(60 * DEG2RAD, Vector3(0, 1, 0));
	renderMesh(espada.model, espada.mesh, espada.texture);
	my_world.espada = espada;

	my_world.tenosuke.dance_anim->assignTime(time);
	renderAnimated(my_world.tenosuke.model, meshes[10], textures[10], &my_world.tenosuke.dance_anim->skeleton);

	
	my_world.player = player;

	// Render World Floor
	Matrix44 m;
	Mesh* floor_mesh = new Mesh();
	floor_mesh->createPlane(2 * max(my_world.h, my_world.w));
	m.translateGlobal(my_world.h * 2, 0, -my_world.w * 2);
	Texture* floor_tex = Texture::Get("data/Assets/Textures/ground_plane2.png");
	renderMesh(m, floor_mesh, floor_tex);


	for (int k = 0; k < my_world.enemies.size(); ++k) {
		Enemy eaux = my_world.enemies[k];
		if (eaux.life > 0.f) {
			eaux.idle_anim->assignTime(time);
			renderAnimated(eaux.model, eaux.mesh, eaux.texture, &eaux.idle_anim->skeleton);
		}
	}
	my_world.renderMap(my_world.map, my_world.w, my_world.h,shader_instanced);

	//DRAW UI OR STAGE SPECIFIC ELEMENTS
	Stage::current_stage->render();


	//////////////////////////////////////////menu vida
	float aux = (window_width / (float)window_height);
	renderUI(2, textures[99], aux);
	//renderUI(4, textures[97], aux);
	renderMinimap(my_world.minimap);
	///////////////////////////////////////// sangre vida
	//renderUI(0, textures[98]);
	///////////////////////////////////////////

	//swap between front buffer and back buffer
	SDL_GL_SwapWindow(this->window);
}

int checkDif() {

	Player player = Game::instance->my_world.player;
	Vector3 a = Vector3(sin(player.angle * RAD2DEG), 1 , cos(player.angle * RAD2DEG));
	Vector3 b = Vector3(sin(player.moveAngle), 1, cos(player.moveAngle));
	a = a.normalize();
	b = b.normalize();
	//float x = a.cross(b).length();
	float x = b.cross(a).length();
	//std::cout << x << '\n'; 
	if (x > 0) return 1;
	else return -1;
}

void Game::update(double seconds_elapsed)
{
	Stage::current_stage->update();
	
	Player player = my_world.player;

	if (Input::wasKeyPressed(SDL_SCANCODE_E)) {
		player.attack = true;
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
	for (int k = 0; k < my_world.enemies.size(); ++k) {
		Enemy en = my_world.enemies[k];
		if (!en.wasHit && player.attack && en.mesh->testSphereCollision(en.model, my_world.espada.model.getTranslation(), 0.5, Vector3(), Vector3())) {
			my_world.enemies[k].wasHit = true;
			my_world.enemies[k].life -= player.light_atk;
		}
	}
	
	if (!free_cam && !inMenu && !player.attack) {

		Vector3 newPos = Vector3();
		bool moves = false;

		//Control camera angle
		if (Input::isKeyPressed(SDL_SCANCODE_LEFT)) {
			player.angle += 90 * DEG2RAD * seconds_elapsed;
		}
		else if (Input::isKeyPressed(SDL_SCANCODE_RIGHT)) {
			player.angle -= 90 * DEG2RAD * seconds_elapsed;
		}
		if (player.angle * RAD2DEG > 360.f) player.angle = (player.angle * RAD2DEG - 360.f) * DEG2RAD;
		if (player.angle * RAD2DEG < -360.f) player.angle = (player.angle * RAD2DEG + 360.f) * DEG2RAD;

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		Vector3 targetpos = player.pos;
		Matrix44 R;
		Vector3 front = R.rotateVector(Vector3(-sin(player.angle), 0, -cos(player.angle)));
		Vector3 Right = R.rotateVector(Vector3(cos(-player.angle), 0, sin(-player.angle)));

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		if (Input::isKeyPressed(SDL_SCANCODE_A)) {
			if (player.speed < player.max_speed) player.speed += 0.07f;
			//if (player.moveAngle > -90) player.moveAngle -= 270 * seconds_elapsed;
			newPos = newPos - (Right * player.speed * seconds_elapsed);
			moves = true;
		}
		if (Input::isKeyPressed(SDL_SCANCODE_D)) {
			if (player.speed < player.max_speed) player.speed += 0.07f;
			//if (player.moveAngle < 90) player.moveAngle += 270 * seconds_elapsed;
			newPos = newPos + (Right * player.speed * seconds_elapsed);
			moves = true;
		}
		if (Input::isKeyPressed(SDL_SCANCODE_W)) {
			player.momentum = 1;
			//check speed
			if (player.speed < player.max_speed) player.speed += 0.07f;
			//check model angle
			if (player.moveAngle > player.angle * RAD2DEG) player.moveAngle -= checkDif() * 360 * seconds_elapsed;
			else if (player.moveAngle < player.angle * RAD2DEG) player.moveAngle += checkDif() * 360 * seconds_elapsed;
			newPos = newPos + (front * player.speed * seconds_elapsed);
			//targetpos = player.pos + front * player.speed * seconds_elapsed;
			moves = true;
		}
		if (Input::isKeyPressed(SDL_SCANCODE_S)) {
			player.momentum = -1;
			if (player.speed < player.max_speed) player.speed += 0.07f;
			newPos = newPos - (front * player.speed * seconds_elapsed);
			//targetpos = player.pos - front * player.speed * seconds_elapsed;
			moves = true;
		}
		
		if (!moves && (player.speed > 0.f)) {
			player.speed -= 0.2f;
		} 
		
		targetpos = targetpos + newPos;


	
		//GAME COLISIONS

		//calculamos el centro de la esfera de colisión del player elevandola hasta la cintura
		Vector3 character_center = targetpos + Vector3(0, 0.3, 0);
		bool hascollision = false;

		//para cada objecto de la escena...
		std::vector<Entity> collidable = my_world.getNearEntities(player.pos.x, -player.pos.z);
		collidable.insert(collidable.end(), trees.begin(), trees.end());
		for(int k = 0 ; k < my_world.enemies.size() ; ++k)
			if (my_world.enemies[k].life > 0.f) collidable.push_back(my_world.enemies[k]);
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
		player.model.rotate(-player.moveAngle * DEG2RAD, Vector3(0, 0.5f, 0));

		if (Input::isKeyPressed(SDL_SCANCODE_LEFT)) {}
		if (Input::isKeyPressed(SDL_SCANCODE_RIGHT)) {}
		/*
		dog.pos = player.pos - Vector3(0.3f, 0, 0.4f);
		dog.model = player.model;
		Vector3 aux = player.model * Vector3(0.3f, 0, 0.4f);
		dog.setModelPos(aux);
		*/

		//GET PLAYER POS
		if (Input::wasKeyPressed(SDL_SCANCODE_C)) {
			std::cout << "Player pos: " << player.pos.x << ',' << player.pos.y << ',' << player.pos.z << '\n';
			std::cout << player.speed << '\n';
			std::cout << "Angle : " << player.angle * RAD2DEG << '\n';
			std::cout << "Move Angle : " << player.moveAngle  << '\n';
		}

	}
	else Stage::current_stage->update();
	
	my_world.player = player;

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