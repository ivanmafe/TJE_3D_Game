#ifndef INTRO_STAGE_H
#define INTRO_STAGE_H

#include "utils.h"
#include "stage.h"

class IntroStage : public Stage
{
public:
	int actualpos = 0;
	IntroStage();
	void render();
	void update(double seconds_elapsed);
};


#endif