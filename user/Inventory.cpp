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

void Inventory::addItem(const string& item) { if (!hasItem(item)) ownedItems.push_back(item); }

bool Inventory::hasItem(const string& item) const {
	for (const auto& it : ownedItems) if (it == item) return true;
	return false;
}

const vector<string>& Inventory::getItems() const { return ownedItems; }

string Inventory::serialize() const {
ostringstream os;
	for (size_t i = 0; i < ownedItems.size(); ++i) {
		if (i) os << ';';
		os << ownedItems[i];
	}
	return os.str();
}

void Inventory::deserialize(const string& line) {
	ownedItems.clear();
istringstream is(line);
string tok;
	while (getline(is, tok, ';')) {
		if (!tok.empty()) ownedItems.push_back(tok);
	}
}

