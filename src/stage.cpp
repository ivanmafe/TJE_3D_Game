#include "stage.h"
#include "game.h"
#include "camera.h"
#include "input.h"

Stage::Stage(const char* name) {
	stages[name] = this;
	if (current_stage == NULL) current_stage = this;
}

void Stage::render() {};
void Stage::update() {};

////////////////// PlayStage //////////////////////

PlayStage::PlayStage() : Stage("PlayStage") {
	//empty constructor
};

void PlayStage::render() {
	drawText(2,20,"PlayStage",Vector3(1,1,1),5);
};

void PlayStage::update() {};


////////////////// MenuStage //////////////////////

MenuStage::MenuStage() : Stage("MenuStage") {
	//empty constructor
};

void MenuStage::render() {
	drawText(2, 20, "Menu", Vector3(1, 1, 1), 5);
	drawText(2, 70, "Press [S] to save", Vector3(1, 1, 1), 3);
	drawText(2, 100, "Press [ESC] to exit", Vector3(1, 1, 1), 3);
};

void MenuStage::update() {};


////////////////// FightStage //////////////////////

FightStage::FightStage() : Stage("FightStage") {
	//empty constructor
};

void FightStage::render() {
	drawText(2, 20, "FightStage", Vector3(1, 1, 1), 5);
};

void FightStage::update() {};

////////////////// DebugStage //////////////////////

DebugStage::DebugStage() : Stage("DebugStage") {
	//empty constructor
};

void DebugStage::render() {

	Camera * camera = Camera::current;
	std::string gameTime = "Total Time : " + std::to_string(Game::instance->time);
	std::string cameraPos = "Camera pos: " + std::to_string(camera->eye.x) + ',' + std::to_string(camera->eye.y) + ',' + std::to_string(camera->eye.z);
	std::string cameraDir = "Camera dir: " + std::to_string(camera->center.x) + ',' + std::to_string(camera->center.y) + ',' + std::to_string(camera->center.z);

	drawText(2, 30, gameTime, Vector3(1, 1, 1), 2);
	drawText(2, 60, cameraPos, Vector3(1, 1, 1), 2);
	drawText(2, 90, cameraDir, Vector3(1, 1, 1), 2);

	//Draw the floor grid
	drawGrid();

	//render the FPS, Draw Calls, etc
	drawText(2, 2, getGPUStats(), Vector3(1, 1, 1), 2);

};

void DebugStage::update() {

	Camera * camera = Camera::current;
	//mouse input to rotate the cam
	if ((Input::mouse_state & SDL_BUTTON_LEFT) || Game::instance->mouse_locked) //is left button pressed?
	{
		camera->rotate(Input::mouse_delta.x * 0.005f, Vector3(0.0f, -1.0f, 0.0f));
		camera->rotate(Input::mouse_delta.y * 0.005f, camera->getLocalVector(Vector3(-1.0f, 0.0f, 0.0f)));
	}
	if (Input::isKeyPressed(SDL_SCANCODE_W) || Input::isKeyPressed(SDL_SCANCODE_UP)) camera->move(Vector3(0.0f, 0.0f, 1.0f) * camera_speed);
	if (Input::isKeyPressed(SDL_SCANCODE_S) || Input::isKeyPressed(SDL_SCANCODE_DOWN)) camera->move(Vector3(0.0f, 0.0f, -1.0f) * camera_speed);
	if (Input::isKeyPressed(SDL_SCANCODE_A) || Input::isKeyPressed(SDL_SCANCODE_LEFT)) camera->move(Vector3(1.0f, 0.0f, 0.0f) * camera_speed);
	if (Input::isKeyPressed(SDL_SCANCODE_D) || Input::isKeyPressed(SDL_SCANCODE_RIGHT)) camera->move(Vector3(-1.0f, 0.0f, 0.0f) * camera_speed);

};

///////////////////////////////////////////////////

