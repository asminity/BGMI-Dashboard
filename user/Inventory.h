#pragma once
#include <string>
#include <vector>
#include "../Common.h"

class Inventory {
private:
	std::vector<std::string> ownedItems;
public:
	Inventory() = default;
	void unlockByLevel(int level);
	void addItem(const std::string& item);
	bool hasItem(const std::string& item) const;
	const std::vector<std::string>& getItems() const;
	std::string serialize() const;
	void deserialize(const std::string& line);
};

