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
	enum { HOME, STREET_1, LAURA_HOUSE, STREET_2, DAVID_HOUSE, PABLO_HOUSE, SCHOOL };
	bool can_talk = false;
	PlayStage();
	void render();
	void update();
};


#endif
