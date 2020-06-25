#ifndef SELECT_STAGE_H
#define SELECT_STAGE_H

#include "utils.h"
#include "stage.h"

class SelectStage : public Stage
{
public:
	int actual = 0;
	int next = 1;
	SelectStage();
	void render();
	void update(double seconds_elapsed);
	int returnActualVal();
	int returnNextVal();
	char* returnMission();
	void setActual(int m);
	void setNext(int m);
};

#endif