#include "IntrovidStage.h"
#include "game.h"
#include "world.h"
#include "input.h"
#include "grender.h"


IntrovidStage::IntrovidStage() : Stage("IntrovidStage") {
    //empty constructor
};

void IntrovidStage::render() {

    glDisable(GL_DEPTH_TEST);
    Mesh quad;

    quad.createQuad(0, 0, 2, 2, false);

    Shader* shader = Shader::Get("data/shaders/quad.vs", "data/shaders/GUI.fs");//flat.fs");
    shader->enable();
    shader->setUniform("u_color", Vector4(1, 1, 1, 1));
    Texture* as = Texture::Get("data/Assets/Textures/GUI/intro1.png");
    Texture* as0 = Texture::Get("data/Assets/Textures/GUI/intro2.png");
    Texture* as1 = Texture::Get("data/Assets/Textures/GUI/intro3.png");


    if (actualpos == 0) {
        shader->setUniform("u_texture", as);
    }
    if (actualpos == 1) {
        shader->setUniform("u_texture", as0);
    }
    if (actualpos == 2) {
        shader->setUniform("u_texture", as1);
    }

    quad.render(GL_TRIANGLES);
    shader->disable();



    //drawText(2, 20, "Menu", Vector3(1, 1, 1), 5);
    //drawText(2, 70, "Press [V] to debug", Vector3(1, 1, 1), 3);
    //drawText(2, 100, "Press [ESC] to exit", Vector3(1, 1, 1), 3);
};

void IntrovidStage::update(double seconds_elapsed) {
    Game::instance->intro_theme->PlaySoundOnce();
    //time += seconds_elapsed;
    //if((((int)time)%4)==0)
        //actualpos++;

    if (Input::wasKeyPressed(SDL_SCANCODE_E)) {
        actualpos++;

        if (actualpos == 3) {
            loadingScreen();
            BASS_Stop();
            BASS_Start();
            Stage::current_stage->changeStage("IntroStage");
        }
    }
};