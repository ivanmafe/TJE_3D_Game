#include "play_stage.h"
#include "input.h"
#include "game.h"
#include "world.h"
#include <bass.h>
#include "AudioBass.h"

PlayStage::PlayStage() : Stage("PlayStage") {
	//empty constructor if needed
};

void PlayStage::render() {

	Player player = Game::instance->my_world.player;
	Camera *camera = Game::instance->camera->current;
	camera->eye = Vector3(player.pos.x + sin(player.angle), 1.25, player.pos.z + cos(player.angle));
	camera->center = player.pos + Vector3(0, 0.7, 0);	

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

	World my_world = Game::instance->my_world;
	Player player = my_world.player;

	if (Input::wasKeyPressed(SDL_SCANCODE_E)) {
		player.attack = true;
		player.speed = 0.f;
	}

	if (Input::wasKeyPressed(SDL_SCANCODE_V)) 
		Stage::current_stage->changeStage("DebugStage"); 

	if (Input::wasKeyPressed(SDL_SCANCODE_M))
		Stage::current_stage->changeStage("MenuStage");

	if (Input::wasKeyPressed(SDL_SCANCODE_I))
		Stage::current_stage->changeStage("IntroStage");

	if (Input::wasKeyPressed(SDL_SCANCODE_T))
		Stage::current_stage->changeStage("SelectStage");
		
	for (int k = 0; k < my_world.enemies.size(); ++k) {
		Enemy en = my_world.enemies[k];
		if (!en.wasHit && player.attack && en.mesh->testSphereCollision(en.model, my_world.espada.model.getTranslation(), 0.5, Vector3(), Vector3())) {
			my_world.enemies[k].wasHit = true;
			Game::instance->hit->PlaySoundAmbient();
			my_world.enemies[k].life -= player.light_atk;
		}
	}

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

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		Vector3 targetpos = player.pos;
		Matrix44 R;
		Vector3 front = R.rotateVector(Vector3(-sin(player.angle), 0, -cos(player.angle)));
		Vector3 Right = R.rotateVector(Vector3(cos(-player.angle), 0, sin(-player.angle)));

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		if (Input::isKeyPressed(SDL_SCANCODE_A)) {
			if (player.speed < player.max_speed) player.speed += 0.07f;
			//if (player.moveAngle > -90) player.moveAngle -= 270 * seconds_elapsed;
			newPos = newPos - (Right * player.speed * seconds_elapsed);
			moves = true;
		}
		if (Input::isKeyPressed(SDL_SCANCODE_D)) {
			if (player.speed < player.max_speed) player.speed += 0.07f;
			//if (player.moveAngle < 90) player.moveAngle += 270 * seconds_elapsed;
			newPos = newPos + (Right * player.speed * seconds_elapsed);
			moves = true;
		}
		if (Input::isKeyPressed(SDL_SCANCODE_W)) {
			player.momentum = 1;
			//check speed
			if (player.speed < player.max_speed) player.speed += 0.07f;
			//check model angle
			if (player.moveAngle > player.angle * RAD2DEG +2) player.moveAngle -= checkDif() * 360 * seconds_elapsed;
			else if (player.moveAngle < player.angle * RAD2DEG -2) player.moveAngle += checkDif() * 360 * seconds_elapsed;
			newPos = newPos + (front * player.speed * seconds_elapsed);
			//targetpos = player.pos + front * player.speed * seconds_elapsed;
			moves = true;
		}
		if (Input::isKeyPressed(SDL_SCANCODE_S)) {
			player.momentum = -1;
			if (player.speed < player.max_speed) player.speed += 0.07f;
			newPos = newPos - (front * player.speed * seconds_elapsed);
			//targetpos = player.pos - front * player.speed * seconds_elapsed;
			moves = true;
		}

		if (!moves && (player.speed > 0.f)) {
			player.speed -= 0.2f;
		}

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
