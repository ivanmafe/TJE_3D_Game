#ifndef TIENDA_STAGE_H
#define TIENDA_STAGE_H

#include "utils.h"
#include "stage.h"

class TiendaStage : public Stage
{
public:
	int actualpos = 0;
	TiendaStage();
	void render();
	void update(double seconds_elapsed);
};

#endif