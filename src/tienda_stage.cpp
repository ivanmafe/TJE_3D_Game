#include "tienda_stage.h"
#include "input.h"
#include "game.h"
#include "utils.h"

TiendaStage::TiendaStage() : Stage("TiendaStage") {
	//empty constructor
};

void TiendaStage::render() {

	glDisable(GL_DEPTH_TEST);
	Mesh quad;

	quad.createQuad(0, 0, 2, 2, false);

	Shader* shader = Shader::Get("data/shaders/quad.vs", "data/shaders/GUI.fs");//flat.fs");
	shader->enable();
	shader->setUniform("u_color", Vector4(1, 1, 1, 1));
	Texture* as = Texture::Get("data/Assets/Textures/GUI/tienda_basica.png");
	Texture* as0 = Texture::Get("data/Assets/Textures/GUI/tienda_lila_no.png");
	Texture* as1 = Texture::Get("data/Assets/Textures/GUI/tienda_lila_si.png");
	Texture* as2 = Texture::Get("data/Assets/Textures/GUI/tienda_alas_no.png");
	Texture* as3 = Texture::Get("data/Assets/Textures/GUI/tienda_alas_si.png");
	Texture* as4 = Texture::Get("data/Assets/Textures/GUI/tienda_salir.png");
	
	if (actualpos == 0) {
		shader->setUniform("u_texture", as);
	}
	if (actualpos == 1) {
		if (Stage::stages["SelectStage"]->getMaxMission()>1) {
			shader->setUniform("u_texture", as1);
		}
		else {
			shader->setUniform("u_texture", as0);
		}
	}
	if (actualpos == 2) {
		if (Stage::stages["SelectStage"]->getMaxMission() > 3) {
			shader->setUniform("u_texture", as3);
		}
		else {
			shader->setUniform("u_texture", as2);
		}
	}

	if (actualpos == 3) {
		shader->setUniform("u_texture", as4);
	}
	
	quad.render(GL_TRIANGLES);
	shader->disable();



	//drawText(2, 20, "Menu", Vector3(1, 1, 1), 5);
	//drawText(2, 70, "Press [V] to debug", Vector3(1, 1, 1), 3);
	//drawText(2, 100, "Press [ESC] to exit", Vector3(1, 1, 1), 3);
};

void TiendaStage::update(double seconds_elapsed) {

	if (Input::wasKeyPressed(SDL_SCANCODE_M)) {
		actualpos = 0;
		Stage::current_stage->changeStage("PlayStage");
	}

	if (Input::wasKeyPressed(SDL_SCANCODE_E)) {
		if (actualpos == 0) {
			Game::instance->my_world.espada.mesh = Mesh::Get("data/Assets/Meshes/basicsword.obj");
			Game::instance->my_world.espada.texture = Texture::Get("data/Assets/Textures/swordtexturelight.png");
			Game::instance->my_world.player.light_atk = 30;
			actualpos = 0;
			Stage::current_stage->changeStage("PlayStage");
		}
		if (actualpos == 1) {
			if (Stage::stages["SelectStage"]->getMaxMission() > 1) {
				Game::instance->my_world.espada.mesh = Mesh::Get("data/Assets/Meshes/purplesword.obj");
				Game::instance->my_world.espada.texture = Texture::Get("data/Assets/Textures/PurpleSwords.png");
				Game::instance->my_world.player.light_atk = 45;
				actualpos = 0;
				Stage::current_stage->changeStage("PlayStage");
			}
			else {
			}
		}
		if (actualpos == 2) {
			if (Stage::stages["SelectStage"]->getMaxMission() > 3) {
				Game::instance->my_world.espada.mesh = Mesh::Get("data/Assets/Meshes/espada1.obj");
				Game::instance->my_world.espada.texture = Texture::Get("data/Assets/Textures/espada1.png");
				Game::instance->my_world.player.light_atk = 60;
				actualpos = 0;
				Stage::current_stage->changeStage("PlayStage");
			}
			else {
			}
		}
		if (actualpos == 3) {
			actualpos = 0;
			Stage::current_stage->changeStage("PlayStage");
		}
	}


	if (Input::wasKeyPressed(SDL_SCANCODE_W))
		if (actualpos > 0)
			actualpos--;
	if (Input::wasKeyPressed(SDL_SCANCODE_S))
		if (Game::instance->inpueblo == true) {
			if (actualpos < 1)
				actualpos++;
		}
		else {
			if (actualpos < 3)
				actualpos++;
		}
};

