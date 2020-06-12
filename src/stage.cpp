#include "stage.h"
#include "input.h"
#include "mesh.h"
#include "shader.h"
#include "texture.h"
#include "game.h"


Stage::Stage(const char* name) {
	stages[name] = this;
	if (current_stage == NULL) current_stage = this;
}

void Stage::render() {};
void Stage::update(double seconds_elapsed) {};



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



SelectStage::SelectStage() : Stage("SelectStage") {
	//empty constructor
	actual = 1;
};

void SelectStage::render() {

	glDisable(GL_DEPTH_TEST);
	Mesh quad;
	Texture* as;
	quad.createQuad(0, 0, 2, 2, false);

	Shader* shader = Shader::Get("data/shaders/quad.vs", "data/shaders/GUI.fs");//flat.fs");
	shader->enable();
	shader->setUniform("u_color", Vector4(1, 1, 1, 1));
	if (actual == 1) {
		as = Texture::Get("data/Assets/Textures/GUI/elegir mision1.png");
	}
	else if (actual == 2) {
		as = Texture::Get("data/Assets/Textures/GUI/elegir mision2.png");
	}
	else if (actual == 3) {
			 as = Texture::Get("data/Assets/Textures/GUI/elegir mision3.png");
	}
	else if (actual == 4) {
			as = Texture::Get("data/Assets/Textures/GUI/elegir mision4.png");
	}
	shader->setUniform("u_texture", as);
	quad.render(GL_TRIANGLES);
	shader->disable();


	drawText(2, 20, "Intro", Vector3(1, 1, 1), 5);
	drawText(2, 70, "Press [Enter] to continue", Vector3(1, 1, 1), 3);
	drawText(2, 100, "Press [ESC] to exit", Vector3(1, 1, 1), 3);

};

void SelectStage::update(double seconds_elapsed) {

	if (Input::wasKeyPressed(SDL_SCANCODE_E)) {
		Game::instance->actualmision = actual;
		Stage::current_stage->changeStage("PlayStage");
	}

	if (Input::wasKeyPressed(SDL_SCANCODE_UP))
		if(actual>1)
			actual -= 1;
	if (Input::wasKeyPressed(SDL_SCANCODE_DOWN))
		if (actual < 4)
			actual += 1;



};
