#ifndef INTROVIDSTAGE_H
#define INTROVIDSTAGE_H

#include "utils.h"
#include "stage.h"

class IntrovidStage : public Stage
{
public:
    int actualpos = 0;
    float time = 0;
    IntrovidStage();
    void render();
    void update(double seconds_elapsed);
};
#endif