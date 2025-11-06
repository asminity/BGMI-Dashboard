#pragma once
#include <string>
#include <vector>
#include "../Common.h"

class Inventory {
private:
vector<string> ownedItems;
public:
	Inventory() = default;
	void unlockByLevel(int level);
	void addItem(const string& item);
	bool hasItem(const string& item) const;
	const vector<string>& getItems() const;
string serialize() const;
	void deserialize(const string& line);
};

