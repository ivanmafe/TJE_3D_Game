#include "menu_stage.h"

MenuStage::MenuStage() : Stage("MenuStage") {
	//empty constructor
};

void MenuStage::render() {
	drawText(2, 20, "Menu", Vector3(1, 1, 1), 5);
	drawText(2, 70, "Press [V] to debug", Vector3(1, 1, 1), 3);
	drawText(2, 100, "Press [ESC] to exit", Vector3(1, 1, 1), 3);
};

void MenuStage::update() {};

