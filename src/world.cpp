#include "world.h"
#include "utils.h"
#include "game.h"
#include "data.h"

void chooseModel(Matrix44 * m, int tile, int * index) {

	Vector3 pos = m->getTranslation();
	m->setIdentity();
	m->scale(1.f, 1.f, 1.f);
	/*
	if (tile == 11) *index = 0; //SUELO
	else if (tile == 8) *index = 7;

	else*/ 
	// ESQUINAS
	if (tile == 0) { *index = 1; m->scale(1.f, 2.f, 1.f); }
	else if (tile == 2) { *index = 1; m->translate(0, 0, 0); m->rotate(90 * DEG2RAD, Vector3(0, 1, 0)); m->scale(1.f, 2.f, 1.f); }
	else if (tile == 20) { *index = 1; m->translate(0, 0, 0); m->rotate(-90 * DEG2RAD, Vector3(0, 1, 0)); m->scale(1.f, 2.f, 1.f); }
	else if (tile == 22) { *index = 1; m->translate(0, 0, 0); m->rotate(180 * DEG2RAD, Vector3(0, 1, 0)); m->scale(1.f, 2.f, 1.f); }
	//ESQUINAS-GRANDES
	else if (tile == 31) { *index = 12; m->scale(1.f, 2.f, 1.f); }
	else if (tile == 32) { *index = 12; m->translate(0, 0, 0); m->rotate(90 * DEG2RAD, Vector3(0, 1, 0)); m->scale(1.f, 2.f, 1.f); }
	else if (tile == 41) { *index = 12; m->translate(0, 0, 0); m->rotate(-90 * DEG2RAD, Vector3(0, 1, 0)); m->scale(1.f, 2.f, 1.f); }
	else if (tile == 42) { *index = 12; m->translate(0, 0, 0); m->rotate(180 * DEG2RAD, Vector3(0, 1, 0)); m->scale(1.f,2.f, 1.f); }
	//ESQUINAS-Pasillo
	else if (tile == 37) { *index = 17; m->scale(1.f, 2.f, 1.f); }
	else if (tile == 38) { *index = 17; m->translate(0, 0, 0); m->rotate(90 * DEG2RAD, Vector3(0, 1, 0)); m->scale(1.f, 2.f, 1.f); }
	else if (tile == 47) { *index = 17;  m->translate(0, 0, 0); m->rotate(-90 * DEG2RAD, Vector3(0, 1, 0)); m->scale(1.f, 2.f, 1.f); }
	else if (tile == 48) { *index = 17; m->translate(0, 0, 0); m->rotate(180 * DEG2RAD, Vector3(0, 1, 0)); m->scale(1.f, 2.f, 1.f); }
	//CONTRA-ESQUINAS
	else if (tile == 13) { *index = 2; m->translate(0, 0, 0); m->rotate(90 * DEG2RAD, Vector3(0, 1, 0)); m->scale(1.f, 2.f, 1.f); }
	else if (tile == 3) { *index = 2; m->translate(0, 0, 0); m->rotate(180 * DEG2RAD, Vector3(0, 1, 0)); m->scale(1.f, 2.f, 1.f); }
	else if (tile == 14) { *index = 2; m->scale(1.f, 2.f, 1.f); }
	else if (tile == 4) { *index = 2; m->translate(0, 0, 0); m->rotate(-90 * DEG2RAD, Vector3(0, 1, 0)); m->scale(1.f, 2.f, 1.f); }
	//CONTRA-ESQUINA DOBLE
	else if (tile == 33) { *index = 13; m->translate(0, 0, 0); m->rotate(-90 * DEG2RAD, Vector3(0, 1, 0)); m->scale(1.f, 2.f, 1.f); }
	else if (tile == 24) { *index = 13; m->translate(0, 0, 0); m->rotate(180 * DEG2RAD, Vector3(0, 1, 0)); m->scale(1.f, 2.f, 1.f); }
	else if (tile == 23) { *index = 13; m->translate(0, 0, 0); m->rotate(90 * DEG2RAD, Vector3(0, 1, 0)); m->scale(1.f, 2.f, 1.f); }
	else if (tile == 34) { *index = 13; m->scale(1.f, 2.f, 1.f); }
	//PARED 1x1
	else if (tile == 10) { *index = 3; m->scale(1.f, 2.f, 1.f); }
	else if (tile == 21) { *index = 3; m->translate(0, 0, 0); m->rotate(-90 * DEG2RAD, Vector3(0, 1, 0)); m->scale(1.f, 2.f, 1.f); }
	else if (tile == 1) { *index = 3; m->translate(0, 0, 0); m->rotate(90 * DEG2RAD, Vector3(0, 1, 0)); m->scale(1.f, 2.f, 1.f); }
	else if (tile == 12) { *index = 3; m->translate(0, 0, 0); m->rotate(180 * DEG2RAD, Vector3(0, 1, 0)); m->scale(1.f, 2.f, 1.f); }
	//PARED 2x1
	else if (tile == 25) { *index = 14; m->scale(1.f, 2.f, 1.f); }
	else if (tile == 16) { *index = 14; m->translate(0, 0, 0); m->rotate(-90 * DEG2RAD, Vector3(0, 1, 0)); m->scale(1.f, 2.f, 1.f); }
	else if (tile == 26) { *index = 14; m->translate(0, 0, 0); m->rotate(90 * DEG2RAD, Vector3(0, 1, 0)); m->scale(1.f, 2.f, 1.f); }
	else if (tile == 15) { *index = 14; m->translate(0, 0, 0); m->rotate(180 * DEG2RAD, Vector3(0, 1, 0)); m->scale(1.f, 2.f, 1.f); }
	//PARED 1X2
	else if (tile == 44) { *index = 15; m->scale(1.f, 2.f, 1.f); }
	else if (tile == 43) { *index = 15; m->translate(0, 0, 0); m->rotate(-90 * DEG2RAD, Vector3(0, 1, 0)); m->scale(1.f, 2.f, 1.f); }
	//PARED 2X2
	else if (tile == 35) { *index = 16; m->scale(1.f, 2.f, 1.f); }
	else if (tile == 36) { *index = 16; m->translate(0, 0, 0); m->rotate(-90 * DEG2RAD, Vector3(0, 1, 0)); m->scale(1.f, 2.f, 1.f); }
	else if (tile == 45) { *index = 16; m->translate(0, 0, 0); m->rotate(180 * DEG2RAD, Vector3(0, 1, 0)); m->scale(1.f, 2.f, 1.f); }
	else if (tile == 46) { *index = 16; m->translate(0, 0, 0); m->rotate(180 * DEG2RAD, Vector3(0, 1, 0)); m->scale(1.f, 2.f, 1.f); }
	//elementos mundo
	else if (tile == 30) { *index = 0; m->scale(1.f, 2.f, 1.f); }//vacio
	else if (tile == 5) { *index = 4;  m->rotate(180 * DEG2RAD, Vector3(0, 1, 0));} //cartel
	else if (tile == 6) { *index = 5; } //charco
	else if (tile == 7) { *index = 6; m->scale(1.f, 1.2f, 1.f); } //CASA1
	else if (tile == 8) { *index = 11; m->scale(1.f, 1.2f, 1.f); } //CASA2
	else if (tile == 17) { *index = 18; m->scale(1.f, 1.2f, 1.f);  m->rotate(180 * DEG2RAD, Vector3(0, 1, 0));} //vendedor
	else *index = -1;
	m->translateGlobal(pos.x, pos.y, pos.z);
}

bool World::isCellEmpty(float pos_x, float pos_y) {


	int x = (int)floor(pos_x) / 4;
	int y = (int)floor(pos_y) / 4;

	int val = map[y * w + x];
	int ind = 0;
	chooseModel(&Matrix44(), val, &ind);
	if (ind != -1) return false;
	else return true;
}

bool World::loadMap(std::string filesrc) { //archivo y tamaño de area
	std::fstream file;
	file.open(filesrc, std::fstream::in);
	if (!file.is_open()) {
		fprintf(stderr, "Error locating the file map");
		return 0;
	}

	while (map.size() != 0) {
		map.clear();
	}

	//get map header containing dimensions
	file >> w;
	file >> h;
	int size = w * h;

	//reallocate memory of the map
	int pos = 0;
	int aux = 0;
	while (file.good()) {
		std::string line;
		while (getline(file, line)) {   // get a whole line
			std::stringstream ss(line);
			while (getline(ss, line, ',')) {
				int aux;
				std::istringstream(line) >> aux;
				map.push_back(aux);
			}
		}
	}
	return true;
}

void World::generateMap(std::vector<int> map, int w, int h) {
	

	Game * g = Game::instance;

	int ind = 0;
	for (int i = 0; i < h; ++i)
		for (int j = 0; j < w; ++j) {
			Matrix44 m;
			m.translateGlobal(4. * i + 2, 0, -4. * j - 2);
			int tmp = map[i * w + j];
			chooseModel(&m, tmp, &ind);
			Entity e;
			e.mesh = NULL;
			if (ind >= 0 && ind <= data_h_size) {
				std::string s1 = "data/Assets/Meshes/" + mesh_names[ind];
				e.mesh = Mesh::Get(const_cast<char*>(s1.c_str()));
			}
			int val = Stage::stages["SelectStage"]->returnNextVal();
			if(val == 0 || val == 1)
				e.texture = g->textures[ind];
			else if (val == 2)
				e.texture = g->textures_seco[ind];
			else
				e.texture = g->textures_piedra[ind];
			e.model = m;
			//if(poner_cosas que no van al array)
			g->tiles.push_back(e);
			if (ind != -1) g->models[ind].push_back(m);

		}

	int aux_h = h * 4;
	int aux_w = w * 4;

	//trees
	for (int x = 0; x < aux_h; x++)
		for (int z = 0; z < aux_w; z++) {
			if ((rand() % 100) < 94) {
				continue;
			}
			
			if (isCellEmpty(x, z)) {
				Entity ent;

				//float s = 10 + random() * 4.0;
				ent.model.rotate(random() * DEG2RAD, Vector3(0, 1, 0));
				ent.model.setTranslation(x, 0, -z);
				ent.model.scale(1.5, 1.5, 1.5);
				g->models[9].push_back(ent.model);
				int mission = Stage::stages["SelectStage"]->returnActualVal();
				if (mission == 0 || mission == 1) {
					ent.mesh = g->meshes[9];
					ent.texture = g->textures[9];
				}
				else if (mission == 2) {
					ent.mesh = Mesh::Get("data/Assets/Meshes/arbol seco.obj");
					ent.texture = Texture::Get("data/Assets/Textures/arbol seco.png");
				} 
				else if (mission == 3 || mission == 4) {
					ent.mesh = Mesh::Get("data/Assets/Meshes/arbol muerto.obj");
					ent.texture = Texture::Get("data/Assets/Textures/arbol muerto.png");
				}
				g->trees.push_back(ent);
			}

		}
	//grass
	for (int x = 0; x < aux_h; x++)
		for (int z = 0; z < aux_w; z++) {
			if ((rand() % 100) < 90) {
				continue;
			}
			Matrix44 model;
			model.setTranslation(x, 0, -z);
			model.rotate(random() * PI, Vector3(0, 1, 0));
			g->models[8].push_back(model);
		}

}

void World::renderMap(std::vector<int> map, int w, int h, Shader* shad) {

	Camera* camera = Camera::current;
	Game* g = Game::instance;
	//glDisable(GL_CULL_FACE);
	shad->enable();
	shad->setUniform("u_viewprojection", camera->viewprojection_matrix);
	shad->setUniform("u_color", Vector4(1, 1, 1, 1));
	shad->setUniform("u_light_direction", Vector3(10, 3, -13));
	shad->setUniform("u_camera_position", camera->eye);

	for (int i = 0; i < 20; ++i) {
		if (g->models[i].size() != 0) {
			if (i == 9) {
				shad->setUniform("u_texture", Game::instance->trees[0].texture);
				Game::instance->trees[0].mesh->renderInstanced(GL_TRIANGLES, &g->models[i][0], g->models[i].size());
			}
			else {
				int val = Stage::stages["SelectStage"]->returnActualVal();
				if (val == 0 || val == 1)
					shad->setUniform("u_texture", Game::instance->textures[i]);
				else if (val == 2)
					shad->setUniform("u_texture", Game::instance->textures_seco[i]);
				else
					shad->setUniform("u_texture", Game::instance->textures_piedra[i]);
				//shad->setUniform("u_texture", Game::instance->textures[i]);
				Game::instance->meshes[i]->renderInstanced(GL_TRIANGLES, &g->models[i][0], g->models[i].size());
			}
		}
	}

	shad->disable();
}

std::vector<Entity> World::getNearEntities(float pos_x, float pos_y) {
	int x = (int)floor(pos_x) / 4;
	int y = (int)floor(pos_y) / 4;

	const std::vector<Entity> tiles = Game::instance->tiles;
	std::vector<Entity> nearby;

	nearby.push_back(tiles[x * w + y]);
	if (x - 1 >= 0) nearby.push_back(tiles[(x - 1) * w + y]);
	if (x + 1 < w) nearby.push_back(tiles[(x + 1) * w + y]);

	if (y - 1 >= 0) {
		nearby.push_back(tiles[x * w + (y - 1)]);
		if (x - 1 >= 0) nearby.push_back(tiles[(x - 1) * w + (y - 1)]);
		if (x + 1 < w) nearby.push_back(tiles[(x + 1) * w + (y - 1)]);
	}
	if (y + 1 < w) {
		nearby.push_back(tiles[x * w + (y + 1)]);
		if (x - 1 >= 0) nearby.push_back(tiles[(x - 1) * w + (y + 1)]);
		if (x + 1 < w) nearby.push_back(tiles[(x + 1) * w + (y + 1)]);
	}
	/*
	for(int i = 0 ; i < nearby.size() ; ++i)
		std::cout << "entit" << i << " x:" << nearby[i].model.getTranslation().x / 4 << " z:" << nearby[i].model.getTranslation().z / 4 << '\n';
		*/
	return nearby;
}

void World::loadScene(char* scene_name) {

	std::cout << "\nLOADING SCENE!!!\n";

	for (int i = 0; i < 20; ++i) {
		if (Game::instance->models[i].size() != 0) {
			for (int m = 0; m < Game::instance->models[i].size(); ++m)
				Game::instance->models[i][m].clear();
		}
	}
	Game::instance->tiles.clear();
	Game::instance->trees.clear();
	this->enemies.clear();
	
	std::fstream file;
	file.open(scene_name, std::fstream::in);

	if (!file.is_open()) {
		fprintf(stderr, "Error locating the scene map\n");
		return;
	}

	std::string s;
	float x, z, sk; int a;

	//LOAD MAP
	// map file
	std::string map_name;
	file >> map_name;
	std::cout << map_name << '\n';
	loadMap(map_name);
	generateMap(map, w, h);

	// minimap
	file >> map_name;
	char* cstr = new char[map_name.length() + 1];
	strcpy(cstr, map_name.c_str());
	minimap = Texture::Get(cstr);
	// exit point
	file >> x; file >> z;
	exit_point = Vector3(x, 0, z);
	// mission point
	file >> x; file >> z;
	mission_point = Vector3(x, 0, z);

	// MAIN CHARACTERS
	file >> s; std::cout << "Loading " + s + "\n";
	file >> x; file >> z; file >> a;
	float dmg = player.light_atk;

	player = *new Player("data/Assets/Meshes/heroe.mesh", "data/Assets/Textures/hero.png", Vector3(x, 0, z));
	player.light_atk = dmg;


	player.angle = a * DEG2RAD; player.moveAngle = a;
	//espada = *new Entity("data/Assets/Meshes/purplesword.obj", "data/Assets/Textures/PurpleSwords.png");
	file >> s; std::cout << "Loading " + s + "\n";
	file >> x; file >> z;
	file >> a; file >> sk;
	tenosuke = *new Entity(Vector3(x, 0, z), a, sk);

	file >> s; file >> x; file >> z;
	healing = *new Entity(Vector3(x, 0, z), 0, 1);

	// Load Animations
	player.skeleton = new Skeleton();
	tenosuke.dance_anim = Animation::Get("data/Assets/animaciones/tenosuke_idle.skanim");
	player.run_anim = Animation::Get("data/Assets/animaciones/fast_run2.skanim");
	player.idle_anim = Animation::Get("data/Assets/animaciones/heroe_idle.skanim");
	player.attack_anim = Animation::Get("data/Assets/animaciones/hero_atack1.skanim");

	// ALL ENEMIES
	int enemy_ent = 0;
	file >> enemy_ent;
	for (int i = 0; i < enemy_ent; ++i) {
		file >> s; 
		file >> x; file >> z;
		file >> a; file >> sk;
		std::cout << s + " pos: " << x << ',' << z << '\n';
		Enemy aux;
		if (!s.compare("skeleton")) {
			aux = *new Enemy("data/Assets/Meshes/skeleton.mesh", "data/Assets/Textures/skeleton.png", Vector3(x, 0, z), a, sk, 0);
			aux.skeleton = new Skeleton();
			aux.idle_anim = Animation::Get("data/Assets/animaciones/skeleton_idle.skanim");
			aux.run_anim = Animation::Get("data/Assets/animaciones/skeleton_walking.skanim");
			aux.attack_anim = Animation::Get("data/Assets/animaciones/skeleton_atack1.skanim");
			aux.death_anim = Animation::Get("data/Assets/animaciones/skeleton_dying.skanim");
		}
		else if (!s.compare("ork")) {
			aux = *new Enemy("data/Assets/Meshes/ork.mesh", "data/Assets/Textures/ork.png", Vector3(x, 0, z), a, sk, 1);
			aux.skeleton = new Skeleton();
			aux.idle_anim = Animation::Get("data/Assets/animaciones/ork_idle.skanim");
			aux.run_anim = Animation::Get("data/Assets/animaciones/ork_walking.skanim");
			aux.attack_anim = Animation::Get("data/Assets/animaciones/ork_atack1.skanim");
			aux.death_anim = Animation::Get("data/Assets/animaciones/ork_dying.skanim");
		}
		else if (!s.compare("golem")) {
			aux = *new Enemy("data/Assets/Meshes/golem.mesh", "data/Assets/Textures/golem.png", Vector3(x, 0, z), a, sk, 2);
			aux.skeleton = new Skeleton();
			aux.idle_anim = Animation::Get("data/Assets/animaciones/golem_idle.skanim");
			aux.run_anim = Animation::Get("data/Assets/animaciones/golem_walking.skanim");
			aux.attack_anim = Animation::Get("data/Assets/animaciones/golem_atack1.skanim");
			aux.death_anim = Animation::Get("data/Assets/animaciones/golem_dying.skanim");
		}
		else if (!s.compare("boss")) {
			aux = *new Enemy("data/Assets/Meshes/boss.mesh", "data/Assets/Textures/boss.png", Vector3(x, 0, z), a, sk, 3);
			aux.weapon_model = Matrix44();
			aux.weapon_mesh = Mesh::Get("data/Assets/Meshes/espada_boss.obj");
			aux.weapon_tex = Texture::Get("data/Assets/Textures/espada_boss.png");
			aux.skeleton = new Skeleton();
			aux.idle_anim = Animation::Get("data/Assets/animaciones/boss_idle.skanim");
			aux.run_anim = Animation::Get("data/Assets/animaciones/boss_walking.skanim");
			aux.attack_anim = Animation::Get("data/Assets/animaciones/boss_atack1.skanim");
			aux.death_anim = Animation::Get("data/Assets/animaciones/boss_dying.skanim");
		}
		enemies.push_back(aux);
	}
}

void World::SaveGame() {

	FILE* f;
	f = fopen("data/save_log.txt", "wb");
	if (f) {
		fwrite(&player, sizeof(Player), 1, f);
		fwrite(&espada, sizeof(Entity), 1, f);
		int x = Stage::stages["SelectStage"]->getMaxMission();
		fwrite(&x, sizeof(int), 1, f);
		int y = Stage::stages["SelectStage"]->returnNextVal();
		fwrite(&y, sizeof(int), 1, f);
	}
}

bool World::LoadGame() {

	FILE* f;
	f = fopen("data/save_log.txt", "r");
	if (f) {
		fread(&player, sizeof(Player), 1, f);
		fread(&espada, sizeof(int), 1, f);
		int x = 0;
		fread(&x, sizeof(int), 1, f);
		Stage::stages["SelectStage"]->setMax(x);
		fread(&x, sizeof(int), 1, f);
		Stage::stages["SelectStage"]->setNext(x);
		return true;
	}
	else return false;


}