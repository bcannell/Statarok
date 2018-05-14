<<<<<<< HEAD
#include "SmiteLog.h"
#include "./cs225/PNG.h"

int main() {
	SmiteLogInst currentGame("./example.txt");
	currentGame.makeHeatMap();
	currentGame.printDamageDealt();
	currentGame.printDamageTaken();
=======
#include <iostream>
#include <vector>
#include "SmiteLog.h"

int main() {
	read_smite_log("./example.txt");
>>>>>>> 6a153fa061542863f9113298dfcbf3badc3502ea
	return 0;
}
