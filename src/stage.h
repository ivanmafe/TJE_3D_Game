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
	virtual void update();
};

class PlayStage : public Stage
{
public:
	PlayStage();
	void render();
	void update();
};

class MenuStage : public Stage
{
public:
	MenuStage();
	void render();
	void update();
};

class FightStage : public Stage
{
public:
	FightStage();
	void render();
	void update();
};

class DebugStage : public Stage
{
public:
	float camera_speed = 0.05;
	DebugStage();
	void render();
	void update();
};




//
#endif
