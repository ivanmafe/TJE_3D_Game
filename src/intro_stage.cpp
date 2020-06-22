#include "intro_stage.h"
#include "input.h"
#include "game.h"
#include "world.h"
#include "utils.h"


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

	if (Input::wasKeyPressed(SDL_SCANCODE_E)) {
	actualpos = 0;
	Stage::current_stage->changeStage("PlayStage");
	}
	if (Input::wasKeyPressed(SDL_SCANCODE_W))
		if (actualpos > 0)
			actualpos--;
	if (Input::wasKeyPressed(SDL_SCANCODE_S))
		if (actualpos < 3)
			actualpos++;
	
};