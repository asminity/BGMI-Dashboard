#include <iostream>
#include "user/Dashboard.h"
#include "CentralInventory.h"
#include "FileUtil.h"

int main() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(&std::cout);

	ensureDataDirs();

	CentralInventory::initializeDefaults();
	// Load persisted catalog if present
	CentralInventory::loadFromFile();
	Dashboard app;
	app.run();
	return 0;
}


