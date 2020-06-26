#include "play_stage.h"
#include "input.h"
#include "game.h"
#include "world.h"
#include <bass.h>
#include "AudioBass.h"
#include "grender.h"

PlayStage::PlayStage() : Stage("PlayStage") {
	//empty constructor if needed
};

void PlayStage::render() {

	Player player = Game::instance->my_world.player;
	Camera *camera = Game::instance->camera->current;
	camera->eye = Vector3(player.pos.x + sin(player.angle), 1.25, player.pos.z + cos(player.angle));
	camera->center = player.pos + Vector3(0, 0.7, 0);	
	if (!Stage::stages["SelectStage"]->returnActualVal() == 0) {
		Shader* shader2 = Shader::Get("data/shaders/quad.vs", "data/shaders/GUI.fs");
		Mesh qaux;
		float dist = (1 - (player.life / player.max_life));
		qaux.createQuad(-0.57 - dist / 4, 0.939f, 0.57 - dist / 2, 0.08, false);
		shader2->enable();
		shader2->setUniform("u_color", Vector4(1, 0, 0, 1));
		shader2->setUniform("u_texture", Texture::Get("data/Assets/Textures/GUI/vida_baja.png"));
		qaux.render(GL_TRIANGLES);
		shader2->disable();
		renderUI(2, Texture::Get("data/Assets/Textures/GUI/vida.png"));

	}


};

int checkDif() {

	Player player = Game::instance->my_world.player;
	Vector3 a = Vector3(sin(player.angle * RAD2DEG), 1, cos(player.angle * RAD2DEG));
	Vector3 b = Vector3(sin(player.moveAngle), 1, cos(player.moveAngle));
	a = a.normalize();
	b = b.normalize();
	//float x = a.cross(b).length();
	float x = b.cross(a).length();
	//std::cout << x << '\n'; 
	if (x > 0) return 1;
	else return -1;
}

void PlayStage::update(double seconds_elapsed) {

	Game::instance->theme->PlaySoundOnce();
	World my_world = Game::instance->my_world;
	Player player = my_world.player;

	//HAS PLAYER DIED
	if (player.life <= 0.f) {
		renderUI(0, Texture::Get("data/Assets/Textures/GUI/misionfallida.png"));
		SDL_GL_SwapWindow(Game::instance->window);
		Sleep(7500);
		Stage::stages["SelectStage"]->setActual(0);
		Stage::stages["SelectStage"]->setNext(1);
		char* s = Stage::stages["SelectStage"]->returnMission();
		player.pos = Vector3(-40, 0, 40);
		Game::instance->my_world.loadScene(s);
		return;
	}

	//CLOSE TO STORE
	if (Stage::stages["SelectStage"]->returnActualVal() == 0) {
		if (player.pos.distance(my_world.tenosuke.pos)<1) {
			if (Input::wasKeyPressed(SDL_SCANCODE_E)) {
				loadingScreen();
				Stage::current_stage->changeStage("TiendaStage");
			}
		}
	}

	//CLOSE TO HEALING POST
	if (my_world.healing.pos.distance(player.pos) < 2) {
		if (Input::wasKeyPressed(SDL_SCANCODE_E)) {
			Game::instance->my_world.player.life = player.max_life;
			return;
		}
	}

	//CLOSE TO SELECT MISSION
	if (my_world.mission_point.distance(player.pos) < 2) {
		if (Input::wasKeyPressed(SDL_SCANCODE_E)) {
			Stage::current_stage->changeStage("SelectStage");
			return;
		}
	} //CLOSE TO THE EXIT
	else if (my_world.exit_point.distance(player.pos) < 2) {
		if (Stage::stages["SelectStage"]->returnActualVal() == 0) {
			Stage::stages["SelectStage"]->setActual(Stage::stages["SelectStage"]->returnNextVal());
		}
		else {
			Stage::stages["SelectStage"]->setActual(0);
		}
		Stage::stages["SelectStage"]->setNext(0);
		char* s = Stage::stages["SelectStage"]->returnMission();
		player.pos = Vector3(-40, 0, 40);
		loadingScreen();
		Game::instance->my_world.loadScene(s);
		return;
	}

	//ATTACK
	if (Input::wasKeyPressed(SDL_SCANCODE_E)) {
		player.attack = true;
		player.speed = 0.f;
	}
	/// CHEAT MODE ///
	if (Input::wasKeyPressed(SDL_SCANCODE_O)) {
		player.life -= 10;
	}
	if (Input::wasKeyPressed(SDL_SCANCODE_L))
		Stage::stages["SelectStage"]->setMax(4);

	if (Input::wasKeyPressed(SDL_SCANCODE_V)) 
		Stage::current_stage->changeStage("DebugStage"); 

	if (Input::wasKeyPressed(SDL_SCANCODE_M))
		Stage::current_stage->changeStage("MenuStage");

	if (Input::wasKeyPressed(SDL_SCANCODE_P))
		Stage::current_stage->changeStage("TiendaStage");

	if (Input::wasKeyPressed(SDL_SCANCODE_I))
		Stage::current_stage->changeStage("IntroStage");

	if (Input::wasKeyPressed(SDL_SCANCODE_T))
		Stage::current_stage->changeStage("SelectStage");
	///////////////////

	for (int k = 0; k < my_world.enemies.size(); ++k) {
		Enemy en = my_world.enemies[k];
		if (!en.wasHit && player.attack && en.mesh->testSphereCollision(en.model, my_world.espada.model.getTranslation(), 0.5, Vector3(), Vector3())) {
			my_world.enemies[k].wasHit = true;
			Game::instance->hit->PlaySoundOnce();
			my_world.enemies[k].life -= player.light_atk;
		}
	}
	//NO MORE ENEMIES
	if (my_world.enemies.size() > 0) {
		bool completed = true;
		for (int k = 0; k < my_world.enemies.size(); ++k) {
			if (my_world.enemies[k].life > 0.f) {
				completed = false;
				Enemy en = my_world.enemies[k];
				if (en.pos.distance(player.pos) < en.range && !en.attack) {
					Vector3 vec = en.pos - player.pos;
					Vector3 xpos = en.model.getTranslation();
					en.setModelPos(Vector3());
					//en.model.setRotation(90 * DEG2RAD, Vector3(0, 1, 0));
					//en.model.rotateGlobal(90 * DEG2RAD * seconds_elapsed, Vector3(0, 1, 0));
					en.setModelPos(xpos);
					en.movePos(vec * en.speed * seconds_elapsed);
					if (en.speed < en.max_speed) en.speed += 0.07f;
				}
				else if (en.speed > 0.f) en.speed -= 0.2f;
				my_world.enemies[k] = en;
			}
		}
		if (completed) {
			if (Stage::stages["SelectStage"]->returnActualVal() == 4) {
				//FINAL DEL JUEGO
				renderUI(0, Texture::Get("data/Assets/Textures/GUI/final.png"));
				SDL_GL_SwapWindow(Game::instance->window);
				BASS_Stop();
				BASS_Start();
				Game::instance->final->PlaySoundOnce();
				Sleep(15000);
				Stage::stages["SelectStage"]->setActual(0);
				Stage::stages["SelectStage"]->setNext(1);
				char* s = Stage::stages["SelectStage"]->returnMission();
				player.pos = Vector3(-40, 0, 40);
				Game::instance->my_world.loadScene(s);
				Stage::changeStage("IntroStage");
				BASS_Stop();
				BASS_Start();
				return;
			}
			else {
				//FINAL MISION
				renderUI(0, Texture::Get("data/Assets/Textures/GUI/misioncumplida.png"));
				SDL_GL_SwapWindow(Game::instance->window);
				Sleep(5000);
				if (Stage::stages["SelectStage"]->getMaxMission() == Stage::stages["SelectStage"]->returnActualVal())
					Stage::stages["SelectStage"]->missionUP();
				Stage::stages["SelectStage"]->setActual(0);
				Stage::stages["SelectStage"]->setNext(0);
				char* s = Stage::stages["SelectStage"]->returnMission();
				player.pos = Vector3(-40, 0, 40);
				Game::instance->my_world.loadScene(s);
				return;
			}
		}
	}
	//MOVEMENT
	if (!player.attack) {

		Vector3 newPos = Vector3();
		bool moves = false;

		//Control camera angle
		if (Input::isKeyPressed(SDL_SCANCODE_LEFT)) {
			player.angle += 90 * DEG2RAD * seconds_elapsed;
		}
		else if (Input::isKeyPressed(SDL_SCANCODE_RIGHT)) {
			player.angle -= 90 * DEG2RAD * seconds_elapsed;
		}
		if (player.angle * RAD2DEG > 360.f) player.angle = (player.angle * RAD2DEG - 360.f) * DEG2RAD;
		if (player.angle * RAD2DEG < -360.f) player.angle = (player.angle * RAD2DEG + 360.f) * DEG2RAD;
		if (player.moveAngle > 360.f) player.moveAngle = (player.moveAngle - 360.f);
		if (player.moveAngle < -360.f) player.moveAngle = (player.moveAngle + 360.f);

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		Vector3 targetpos = player.pos;
		Matrix44 R;
		Vector3 front = R.rotateVector(Vector3(-sin(player.angle), 0, -cos(player.angle)));
		Vector3 Right = R.rotateVector(Vector3(cos(-player.angle), 0, sin(-player.angle)));

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		if (Input::isKeyPressed(SDL_SCANCODE_A)) {
			if (player.speed < player.max_speed) player.speed += 0.07f;

			if (player.moveAngle > player.angle * RAD2DEG + 96) player.moveAngle += 270 * seconds_elapsed;
			else if (player.moveAngle < player.angle * RAD2DEG + 84) player.moveAngle += 270 * seconds_elapsed;
			
			newPos = newPos - (Right * player.speed * seconds_elapsed);
			moves = true;
		}
		if (Input::isKeyPressed(SDL_SCANCODE_D)) {
			if (player.speed < player.max_speed) player.speed += 0.07f;			

			if (player.moveAngle < player.angle * RAD2DEG - 96) player.moveAngle -= 270 * seconds_elapsed;
			else if (player.moveAngle > player.angle * RAD2DEG - 84) player.moveAngle -= 270 * seconds_elapsed;

			newPos = newPos + (Right * player.speed * seconds_elapsed);
			moves = true;
		}
		if (Input::isKeyPressed(SDL_SCANCODE_W)) {
			player.momentum = 1;
			//check speed
			if (player.speed < player.max_speed) player.speed += 0.07f;
			//check model angle
			if (player.moveAngle > player.angle * RAD2DEG + 4) player.moveAngle -= checkDif() * 360 * seconds_elapsed;
			else if (player.moveAngle < player.angle * RAD2DEG - 4) player.moveAngle += checkDif() * 360 * seconds_elapsed;
			newPos = newPos + (front * player.speed * seconds_elapsed);
			//targetpos = player.pos + front * player.speed * seconds_elapsed;
			moves = true;
		}
		if (Input::isKeyPressed(SDL_SCANCODE_S)) {
			player.momentum = -1;
			if (player.speed < player.max_speed) player.speed += 0.07f;
			if (player.moveAngle < player.angle * RAD2DEG - 176) player.moveAngle += 270 * seconds_elapsed;
			else if (player.moveAngle > player.angle * RAD2DEG - 184) player.moveAngle -= 270 * seconds_elapsed;
			newPos = newPos - (front * player.speed * seconds_elapsed);
			//targetpos = player.pos - front * player.speed * seconds_elapsed;
			moves = true;
		}

		if (!moves && (player.speed > 0.f)) {
			player.speed -= 0.2f;
		}

		if (player.speed > 0.f) {  Game::instance->step->PlaySoundOnce(); }
		targetpos = targetpos + newPos;



		//GAME COLISIONS

		//calculamos el centro de la esfera de colisión del player elevandola hasta la cintura
		Vector3 character_center = targetpos + Vector3(0, 0.3, 0);
		bool hascollision = false;

		//para cada objecto de la escena...
		std::vector<Entity> collidable = my_world.getNearEntities(player.pos.x, -player.pos.z);
		collidable.insert(collidable.end(), Game::instance->trees.begin(), Game::instance->trees.end());
		for (int k = 0; k < my_world.enemies.size(); ++k)
			if (my_world.enemies[k].life > 0.f) collidable.push_back(my_world.enemies[k]);
		int size = collidable.size();

		for (int i = 0; i < size; i++) {

			Entity ent = collidable[i];
			Vector3 v = ent.model.getTranslation();
			if (ent.mesh == NULL) continue;
			Mesh* mesh = ent.mesh;

			//comprobamos si colisiona el objeto con la esfera (radio 3)
			Vector3 coll;
			Vector3 collnorm;
			if (mesh->testSphereCollision(ent.model, character_center, 0.2, coll, collnorm) == false)
				continue; //si no colisiona, pasamos al siguiente objeto
			hascollision = true;
			Vector3 contrapush = normalize(coll - character_center) * seconds_elapsed;   ///faltaria interpolar la posicion actual con la del rebote para que no tiemble tanto

			targetpos = player.pos - contrapush;
			targetpos.y = 0;
			break;
			//si la esfera está colisionando muevela a su posicion anterior alejandola del objeto


		}
		player.pos = targetpos;

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		player.model.setIdentity();
		player.model.setTranslation(player.pos.x, player.pos.y, player.pos.z);
		player.model.rotate(-player.moveAngle * DEG2RAD, Vector3(0, 0.5f, 0));

		//GET PLAYER POS
		if (Input::wasKeyPressed(SDL_SCANCODE_C)) {
			std::cout << "Player pos: " << player.pos.x << ',' << player.pos.y << ',' << player.pos.z << '\n';
			std::cout << player.speed << '\n';
			std::cout << "Angle : " << player.angle * RAD2DEG << '\n';
			std::cout << "Move Angle : " << player.moveAngle << '\n';
		}

	}

	my_world.player = player;
	Game::instance->my_world = my_world;
};
