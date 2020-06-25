#ifndef STAGE_H
#define STAGE_H

#include "utils.h"
#include <map>

class Stage
{
public:
	int max_mission = 1;
	static std::map<std::string, Stage*> stages;
	static Stage * current_stage;
	static void changeStage(const char*name) { current_stage = stages[name]; }

	Stage(const char* name);
	virtual void render();
	virtual void update(double seconds_elapsed);
	virtual int returnActualVal();
	virtual int returnNextVal();
	virtual char* returnMission();
	virtual void setActual(int m);
	virtual void setNext(int m);
};

#endif
