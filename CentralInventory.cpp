#include "CentralInventory.h"
#include <fstream>
#include <string>

std::vector<CatalogItem> CentralInventory::catalog;

void CentralInventory::initializeDefaults() {
	catalog.clear();
	// Outfits
	catalog.push_back({"Classic Outfit", 1, "Outfit"});
	catalog.push_back({"Legendary Outfit", 15, "Outfit"});
	catalog.push_back({"Warrior Set", 12, "Outfit"});
	catalog.push_back({"Urban Tracker", 12, "Outfit"});
	catalog.push_back({"Night Ops", 12, "Outfit"});
	catalog.push_back({"Royal Guard", 15, "Outfit"});
	catalog.push_back({"Jungle Explorer", 8, "Outfit"});
	catalog.push_back({"Arctic Hunter", 10, "Outfit"});
	catalog.push_back({"Cyber Ninja", 18, "Outfit"});
	catalog.push_back({"Phoenix Armor", 20, "Outfit"});

	// Emotes
	catalog.push_back({"Victory Emote", 3, "Emote"});
	catalog.push_back({"Dance Emote", 20, "Emote"});
	catalog.push_back({"Clap Emote", 1, "Emote"});
	catalog.push_back({"Laugh Emote", 1, "Emote"});
	catalog.push_back({"Wave Emote", 1, "Emote"});
	catalog.push_back({"Chicken Dinner Emote", 10, "Emote"});
	catalog.push_back({"MVP Emote", 15, "Emote"});
	catalog.push_back({"Thumbs Up Emote", 1, "Emote"});
	catalog.push_back({"Facepalm Emote", 1, "Emote"});
	catalog.push_back({"Salute Emote", 1, "Emote"});

	// Gun Skins (many at same levels)
	catalog.push_back({"Scar-L Skin", 5, "Gun Skin"});
	catalog.push_back({"AKM Skin", 8, "Gun Skin"});
	catalog.push_back({"M416 Glacier (Replica)", 25, "Gun Skin"});
	catalog.push_back({"UMP45 Neon", 5, "Gun Skin"});
	catalog.push_back({"Uzi Tiger", 5, "Gun Skin"});
	catalog.push_back({"M24 Carbon", 10, "Gun Skin"});
	catalog.push_back({"Kar98k Gold", 10, "Gun Skin"});
	catalog.push_back({"AWM Emerald", 15, "Gun Skin"});
	catalog.push_back({"DP-28 Copper", 10, "Gun Skin"});
	catalog.push_back({"M16A4 Digital", 5, "Gun Skin"});
	catalog.push_back({"Vector Inferno", 10, "Gun Skin"});
	catalog.push_back({"SKS Rust", 8, "Gun Skin"});
	catalog.push_back({"Mini14 Camo", 10, "Gun Skin"});
	catalog.push_back({"SLR Steel", 12, "Gun Skin"});
	catalog.push_back({"AUG Sapphire", 20, "Gun Skin"});
	catalog.push_back({"Groza Crimson", 20, "Gun Skin"});
	catalog.push_back({"QBZ Jungle", 10, "Gun Skin"});
	catalog.push_back({"QBU Desert", 10, "Gun Skin"});

	// Parachutes (shared level 10)
	catalog.push_back({"Parachute Skin", 10, "Parachute"});
	catalog.push_back({"Parachute Stars", 10, "Parachute"});
	catalog.push_back({"Parachute Stripes", 10, "Parachute"});
	catalog.push_back({"Parachute Night", 10, "Parachute"});
	catalog.push_back({"Parachute Thunder", 10, "Parachute"});

	// Vehicle Skins
	catalog.push_back({"Dacia Classic", 5, "Vehicle Skin"});
	catalog.push_back({"Dacia Gold", 15, "Vehicle Skin"});
	catalog.push_back({"Buggy Flames", 8, "Vehicle Skin"});
	catalog.push_back({"UAZ Military", 12, "Vehicle Skin"});
	catalog.push_back({"Motorcycle Racer", 10, "Vehicle Skin"});
	catalog.push_back({"Boat Camo", 6, "Vehicle Skin"});
	catalog.push_back({"Mirado Royal", 18, "Vehicle Skin"});
	catalog.push_back({"Snowmobile Frost", 10, "Vehicle Skin"});

	// Backpacks (many share levels)
	catalog.push_back({"Backpack Camo", 5, "Backpack"});
	catalog.push_back({"Backpack Dragon", 15, "Backpack"});
	catalog.push_back({"Backpack Neon", 10, "Backpack"});
	catalog.push_back({"Backpack Tactical", 8, "Backpack"});
	catalog.push_back({"Backpack Stealth", 12, "Backpack"});
	catalog.push_back({"Backpack Arctic", 10, "Backpack"});

	// Helmets (many share levels)
	catalog.push_back({"Helmet Camo", 5, "Helmet"});
	catalog.push_back({"Helmet Dragon", 15, "Helmet"});
	catalog.push_back({"Helmet Neon", 10, "Helmet"});
	catalog.push_back({"Helmet Tactical", 8, "Helmet"});
	catalog.push_back({"Helmet Stealth", 12, "Helmet"});
	catalog.push_back({"Helmet Arctic", 10, "Helmet"});

	// Melee
	catalog.push_back({"Pan Gold", 10, "Melee"});
	catalog.push_back({"Pan Frymaster", 1, "Melee"});
	catalog.push_back({"Machete Jungle", 5, "Melee"});
	catalog.push_back({"Crowbar Steel", 5, "Melee"});

	// Keys/Tickets
	catalog.push_back({"Supply Crate Key", 1, "Ticket"});
	catalog.push_back({"Premium Crate Ticket", 1, "Ticket"});
}

const std::vector<CatalogItem>& CentralInventory::getCatalog() { return catalog; }

bool CentralInventory::loadFromFile(const std::string& path) {
	std::ifstream in(path);
	if (!in) return false;
	catalog.clear();
	std::string line;
	while (std::getline(in, line)) {
		if (line.empty()) continue;
		// Support two formats:
		// 1) name,level (legacy)
		// 2) category|name|level (new)
		size_t pipe1 = line.find('|');
		if (pipe1 != std::string::npos) {
			size_t pipe2 = line.find('|', pipe1+1);
			if (pipe2 == std::string::npos) continue;
			std::string cat = line.substr(0, pipe1);
			std::string name = line.substr(pipe1+1, pipe2 - (pipe1+1));
			int lvl = std::stoi(line.substr(pipe2+1));
			catalog.push_back({name, lvl, cat});
		} else {
			size_t pos = line.rfind(',');
			if (pos == std::string::npos) continue;
			std::string name = line.substr(0, pos);
			int lvl = std::stoi(line.substr(pos+1));
			catalog.push_back({name, lvl, "Misc"});
		}
	}
	return true;
}

bool CentralInventory::saveToFile(const std::string& path) {
	std::ofstream out(path, std::ios::trunc);
	if (!out) return false;
	for (const auto& c : catalog) out << c.category << "|" << c.name << "|" << c.minLevel << "\n";
	return true;
}

void CentralInventory::addItem(const std::string& name, int minLevel, const std::string& category) {
	catalog.push_back({name, minLevel, category});
}

std::vector<std::string> CentralInventory::listCategories() {
	std::vector<std::string> cats;
	for (const auto& c : catalog) {
		bool found=false; for (const auto& k : cats) if (k==c.category) { found=true; break; }
		if (!found) cats.push_back(c.category);
	}
	return cats;
}

std::vector<CatalogItem> CentralInventory::getByCategory(const std::string& category) {
	std::vector<CatalogItem> out;
	for (const auto& c : catalog) if (c.category == category) out.push_back(c);
	return out;
}

std::string CentralInventory::getCategoryOf(const std::string& name) {
	for (const auto& c : catalog) if (c.name == name) return c.category;
	return "Misc";
}


