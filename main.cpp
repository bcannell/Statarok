#include "SmiteLog.h"
#include "./cs225/PNG.h"

int main() {
	SmiteLogInst currentGame("./example.txt");
	currentGame.makeHeatMap();
	currentGame.printDamageDealt();
	currentGame.printDamageTaken();
	return 0;
}
