#include "intro_stage.h"
#include "input.h"
#include "game.h"
#include "world.h"
#include "utils.h"
#include "grender.h"


IntroStage::IntroStage() : Stage("IntroStage") {
	//empty constructor
};

void IntroStage::render() {
	
	glDisable(GL_DEPTH_TEST);
	Mesh quad;

	quad.createQuad(0, 0, 2, 2, false);

	Shader* shader = Shader::Get("data/shaders/quad.vs", "data/shaders/GUI.fs");//flat.fs");
	shader->enable();
	shader->setUniform("u_color", Vector4(1, 1, 1, 1));
	Texture* as = Texture::Get("data/Assets/Textures/GUI/intro.png");
	Texture* as0 = Texture::Get("data/Assets/Textures/GUI/intro_select1.png");
	Texture* as1 = Texture::Get("data/Assets/Textures/GUI/intro_select2.png");
	Texture* as2 = Texture::Get("data/Assets/Textures/GUI/intro_select3.png");
	Texture* as3 = Texture::Get("data/Assets/Textures/GUI/intro_select4.png");
	shader->setUniform("u_texture", as);
	quad.render(GL_TRIANGLES);
	if (actualpos == 0) {
		shader->setUniform("u_texture", as0);
	}
	if (actualpos == 1) {
		shader->setUniform("u_texture", as1);
	}
	if (actualpos == 2) {
		shader->setUniform("u_texture", as2);
	}
	if (actualpos == 3) {
		shader->setUniform("u_texture", as3);
	}
	quad.render(GL_TRIANGLES);
	shader->disable();

};

void IntroStage::update(double seconds_elapsed) {
	Game::instance->startMenu_theme->PlaySoundOnce();
	if (Input::wasKeyPressed(SDL_SCANCODE_E)) {
		if (actualpos == 0) {
			actualpos = 0;
			Stage::stages["SelectStage"]->setNext(0);
			Game::instance->startMenu_theme->StopSound();
			Game::instance->my_world.loadScene("data/Assets/Tutorial_mission.txt");
			Stage::stages["SelectStage"]->setActual(5);
			renderUI(0, Texture::Get("data/Assets/Textures/GUI/tutorial.png"));
			SDL_GL_SwapWindow(Game::instance->window);
			Sleep(5000);
			Stage::current_stage->changeStage("PlayStage");
		}
		if (actualpos == 1) {
			actualpos = 0;
			Game::instance->startMenu_theme->StopSound();
			Stage::current_stage->changeStage("PlayStage");
		}
		if (actualpos == 2) {
			actualpos = 0;
			Game::instance->startMenu_theme->StopSound();
			if (Game::instance->my_world.LoadGame()) {
				Stage::current_stage->changeStage("PlayStage");
			}
		}
		if (actualpos == 3) {
			Game::instance->must_exit=true;
		}
	}

	if (Input::wasKeyPressed(SDL_SCANCODE_W))
		if (actualpos > 0)
			actualpos--;
	if (Input::wasKeyPressed(SDL_SCANCODE_S))
		if (actualpos < 3)
			actualpos++;
	
};