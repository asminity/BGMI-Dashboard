#include "Inventory.h"
#include "../CentralInventory.h"
#include <sstream>

void Inventory::unlockByLevel(int level) {
	for (const auto& entry : CentralInventory::getCatalog()) {
		if (level >= entry.minLevel && !hasItem(entry.name)) {
			ownedItems.push_back(entry.name);
		}
	}
}

void Inventory::addItem(const std::string& item) { if (!hasItem(item)) ownedItems.push_back(item); }

bool Inventory::hasItem(const std::string& item) const {
	for (const auto& it : ownedItems) if (it == item) return true;
	return false;
}

const std::vector<std::string>& Inventory::getItems() const { return ownedItems; }

std::string Inventory::serialize() const {
	std::ostringstream os;
	for (size_t i = 0; i < ownedItems.size(); ++i) {
		if (i) os << ';';
		os << ownedItems[i];
	}
	return os.str();
}

void Inventory::deserialize(const std::string& line) {
	ownedItems.clear();
	std::istringstream is(line);
	std::string tok;
	while (std::getline(is, tok, ';')) {
		if (!tok.empty()) ownedItems.push_back(tok);
	}
}

