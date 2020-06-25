#include "select_stage.h"
#include "game.h"
#include "world.h"
#include "input.h"

SelectStage::SelectStage() : Stage("SelectStage") {
	//empty constructor
};

void SelectStage::render() {

	glDisable(GL_DEPTH_TEST);
	Mesh quad;
	Texture* as;
	quad.createQuad(0, 0, 2, 2, false);

	Shader* shader = Shader::Get("data/shaders/quad.vs", "data/shaders/GUI.fs");//flat.fs");
	shader->enable();
	shader->setUniform("u_color", Vector4(1, 1, 1, 1));
	if (next == 0) next = 1;
	if (next == 1) {
		as = Texture::Get("data/Assets/Textures/GUI/elegir mision1.png");
	}
	else if (next == 2) {
		as = Texture::Get("data/Assets/Textures/GUI/elegir mision2.png");
	}
	else if (next == 3) {
		as = Texture::Get("data/Assets/Textures/GUI/elegir mision3.png");
	}
	else if (next == 4) {
		as = Texture::Get("data/Assets/Textures/GUI/elegir mision4.png");
	}
	shader->setUniform("u_texture", as);
	quad.render(GL_TRIANGLES);
	shader->disable();


	//drawText(2, 20, "Intro", Vector3(1, 1, 1), 5);
	//drawText(2, 70, "Press [Enter] to continue", Vector3(1, 1, 1), 3);
	//drawText(2, 100, "Press [ESC] to exit", Vector3(1, 1, 1), 3);

};

void SelectStage::update(double seconds_elapsed) {
	if (next == 0) next = 1;
	if (Input::wasKeyPressed(SDL_SCANCODE_E)) {
		Game::instance->actualmision = actual;
		Stage::current_stage->changeStage("PlayStage");
	}
	if (Input::wasKeyPressed(SDL_SCANCODE_W))
		if(next>1)
			next -= 1;
	if (Input::wasKeyPressed(SDL_SCANCODE_S)) {
		int m = this->max_mission;
		if (next < 4 && m > next)
			next += 1;
	}
	
};

int SelectStage::returnActualVal() {
	return actual;
}

int SelectStage::returnNextVal() {
	return next;
}

char* SelectStage::returnMission() {
	
	if (actual == 0) return("data/Assets/Village.txt");
	else if (actual == 1) return("data/Assets/Skeleton_mission.txt");
	else if (actual == 2) return("data/Assets/Golem_mission.txt");
	else if (actual == 3) return("data/Assets/Ork_mission.txt");
	else if (actual == 4) return("data/Assets/Boss_mission.txt");
	
}

void SelectStage::setActual(int m) {
	actual = m;
};

void SelectStage::setNext(int m) {
	next = m;
};

void SelectStage::missionUP() {
	max_mission++;
};
int SelectStage::getMaxMission() {
	return max_mission;
}