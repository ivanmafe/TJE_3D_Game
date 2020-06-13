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
	shader->setUniform("u_texture", as);
	quad.render(GL_TRIANGLES);
	shader->disable();

};

void IntroStage::update(double seconds_elapsed) {

	if (Input::wasKeyPressed(SDL_SCANCODE_E))
		Stage::current_stage->changeStage("PlayStage");
};