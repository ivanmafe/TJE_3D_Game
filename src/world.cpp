#include "world.h"
#include "utils.h"


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

	//get map header containing dimensions
	file >> w;
	file >> h;
	int size = w * h;

	//reallocate memory of the map
	if (map != NULL)
		free(map);
	map = (int*)calloc(w * h, sizeof(int));

	int pos = 0;
	int aux = 0;
	while (file.good()) {
		std::string line;
		while (getline(file, line)) {   // get a whole line
			std::stringstream ss(line);
			while (getline(ss, line, ',')) {
				int aux;
				std::istringstream(line) >> aux;
				map[pos++] = aux;
			}
		}
	}
	return true;
}

bool World::loadVegetation(const char* filesrc) {

	return true;
}
