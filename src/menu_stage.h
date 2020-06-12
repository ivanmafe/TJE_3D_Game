#ifndef MENU_STAGE_H
#define MENU_STAGE_H

#include "utils.h"
#include "stage.h"

class MenuStage : public Stage
{
public:
	MenuStage();
	void render();
	void update(double seconds_elapsed);
};

#endif