#ifndef STAGE_H
#define STAGE_H

#include "utils.h"
#include <map>

class Stage
{
public:

	static std::map<std::string, Stage*> stages;
	static Stage * current_stage;
	static void changeStage(const char*name) { current_stage = stages[name]; }

	Stage(const char* name);
	virtual void render();
	virtual void update(double seconds_elapsed);
};

class IntroStage : public Stage
{
public:
	IntroStage();
	void render();
	void update(double seconds_elapsed);
};

class SelectStage : public Stage
{
public:
	int actual;
	SelectStage();
	void render();
	void update(double seconds_elapsed);
};

#endif
