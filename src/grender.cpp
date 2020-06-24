#include "grender.h"
#include "game.h"
#include "entity_player.h"

void renderMinimap(Texture* tex) {

	Player player = Game::instance->my_world.player;

	glDisable(GL_DEPTH_TEST);
	Mesh quad;
	quad.createQuad(0.7, -0.7, 0.4, 0.5, false);
	Shader* shader = Shader::Get("data/shaders/quad.vs", "data/shaders/GUI.fs");
	shader->enable();
	shader->setUniform("u_color", Vector4(1, 1, 1, 1));
	shader->setUniform("u_texture", tex);
	quad.render(GL_TRIANGLES);
	shader->disable();

	glEnable(GL_CULL_FACE);
	Mesh quad2;

	float x = -((player.pos.z / (Game::instance->my_world.h * 2)));
	float y = (player.pos.x / -(Game::instance->my_world.h * 2)) + 2;

	x = (x * 0.2f) + 1.5f;
	y = (y * 0.25f) + 0.05f;

	quad2.vertices.push_back(Vector3(-1 + x, (-0.99f + y), 0));
	quad2.vertices.push_back(Vector3(-1.01f + x, (-1.01f + y), 0));
	quad2.vertices.push_back(Vector3(-0.99f + x, (-1.01f + y), 0));


	Shader* shader2 = Shader::Get("data/shaders/quad.vs", "data/shaders/GUI.fs");
	shader2->enable();
	shader2->setUniform("u_color", Vector4(0, 0, 1, 1));
	quad2.render(GL_TRIANGLES);
	shader2->disable();
}

void renderUI(int cuadrante, Texture* tex) {
	
	glDisable(GL_DEPTH_TEST);
	Mesh quad;

	if (cuadrante == 0) quad.createQuad(0, 0, 2, 2, false);
	else if (cuadrante == 1) quad.createQuad(0.5, 0.5, 1, 1, false);
	else if (cuadrante == 2) quad.createQuad(-0.5, 0.5, 1, 1, false);
	else if (cuadrante == 3) quad.createQuad(-0.5, -0.5, 1, 1, false);
	else if (cuadrante == 4) quad.createQuad(0.5, -0.5, 1, 1, false);

	Shader* shader = Shader::Get("data/shaders/quad.vs", "data/shaders/GUI.fs");//flat.fs");
	shader->enable();
	shader->setUniform("u_color", Vector4(1, 1, 1, 1));
	shader->setUniform("u_texture", tex);
	quad.render(GL_TRIANGLES);
	shader->disable();
}

void loadingScreen(){
	renderUI(0, Texture::Get("data/Assets/Textures/GUI/mapa_con_logo2.png"));
	SDL_GL_SwapWindow(Game::instance->window);
}