#include "menu_stage.h"
#include "input.h"
#include "game.h"
#include "utils.h"

MenuStage::MenuStage() : Stage("MenuStage") {
	//empty constructor
};

void MenuStage::render() {

	glDisable(GL_DEPTH_TEST);
	Mesh quad;

	quad.createQuad(0, 0, 2, 2, false);

	Shader* shader = Shader::Get("data/shaders/quad.vs", "data/shaders/GUI.fs");//flat.fs");
	shader->enable();
	shader->setUniform("u_color", Vector4(1, 1, 1, 1));
	Texture* as = Texture::Get("data/Assets/Textures/GUI/menupausa-juego.png");
	Texture* as0 = Texture::Get("data/Assets/Textures/GUI/menupausa-pueblo.png");
	Texture* as1 = Texture::Get("data/Assets/Textures/GUI/pausa-sel1.png");
	Texture* as2 = Texture::Get("data/Assets/Textures/GUI/pausa-sel2.png");
	Texture* as3 = Texture::Get("data/Assets/Textures/GUI/pausa-sel3.png");

	if (Stage::stages["SelectStage"]->returnActualVal() == 0) {
		shader->setUniform("u_texture", as0);
	}
	else {
		shader->setUniform("u_texture", as);
	}
	quad.render(GL_TRIANGLES);

	if (actualpos == 0) {
		shader->setUniform("u_texture", as1);
	}
	if (actualpos == 1) {
		shader->setUniform("u_texture", as2);
	}
	if (actualpos == 2) {
		shader->setUniform("u_texture", as3);
	}
	
	quad.render(GL_TRIANGLES);
	shader->disable();



	//drawText(2, 20, "Menu", Vector3(1, 1, 1), 5);
	//drawText(2, 70, "Press [V] to debug", Vector3(1, 1, 1), 3);
	//drawText(2, 100, "Press [ESC] to exit", Vector3(1, 1, 1), 3);
};

void MenuStage::update(double seconds_elapsed) {

	if (Input::wasKeyPressed(SDL_SCANCODE_M)) {
		actualpos = 0;
		Stage::current_stage->changeStage("PlayStage");
	}

	if (Input::wasKeyPressed(SDL_SCANCODE_E)) {
		if (Stage::stages["SelectStage"]->returnActualVal() == 0) {
			if (actualpos == 0) {
				actualpos = 0;
				Stage::current_stage->changeStage("PlayStage");
			}
			if (actualpos == 1) {
				actualpos = 0;
				Game::instance->my_world.SaveGame();
				Stage::current_stage->changeStage("PlayStage");
			}
			if (actualpos == 2) {
				actualpos = 0;
				Stage::current_stage->changeStage("IntroStage");
			}
		}
		else {
			if (actualpos == 0) {
				actualpos = 0;
				Stage::current_stage->changeStage("PlayStage");
			}
			if (actualpos == 1) {
				actualpos = 0;
				Stage::current_stage->changeStage("PlayStage");
			}
			if (actualpos == 2) {
				actualpos = 0;
				Stage::current_stage->changeStage("IntroStage");
			}
		}
	}


	if (Input::wasKeyPressed(SDL_SCANCODE_W))
		if (actualpos > 0)
			actualpos--;
	if (Input::wasKeyPressed(SDL_SCANCODE_S))
		if (actualpos < 2)
			actualpos++;
	
};

