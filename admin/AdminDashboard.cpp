#include "AdminDashboard.h"
#include "../CentralInventory.h"
#include "../user/Player.h"
#include <iostream>
#include <string>

static int readInt() { std::string s; std::getline(std::cin,s); try { return std::stoi(s);} catch(...) { return -1; } }

void AdminDashboard::run() {
	// Try to load catalog; if missing, keep defaults
	CentralInventory::loadFromFile();
	while (true) {
		std::cout << "\n==== ADMIN DASHBOARD ====\n";
		std::cout << "1. List Users\n2. View User Summary\n3. Reset User Password\n4. Delete User\n5. Reset Player Stats\n6. Grant XP to User\n7. Manage Catalog (Add/Save/Show)\n8. Back\n";
		std::cout << "Choose: "; std::cout.flush();
		int ch = readInt();
		if (ch==1) listUsers();
		else if (ch==2) viewUserSummary();
		else if (ch==3) resetPassword();
		else if (ch==4) deleteUser();
		else if (ch==5) resetPlayerStats();
		else if (ch==6) grantXp();
		else if (ch==7) manageCatalog();
		else if (ch==8) { CentralInventory::saveToFile(); break; }
		else std::cout << "Invalid option.\n";
	}
}

void AdminDashboard::listUsers() {
	std::cout << "Users:\n";
	for (const auto& u : auth.listUsers()) std::cout << "- " << u << " (" << auth.getRole(u) << ")\n";
}

void AdminDashboard::viewUserSummary() {
	std::cout << "Username: "; std::cout.flush(); std::string u; std::getline(std::cin,u);
	Player p(u); if (!p.loadFromDisk()) { std::cout << "No data.\n"; return; }
	std::cout << p.publicSummary() << "\n";
}

void AdminDashboard::resetPassword() {
	std::cout << "Username: "; std::cout.flush(); std::string u; std::getline(std::cin,u);
	std::cout << "New password: "; std::cout.flush(); std::string np; std::getline(std::cin,np);
	if (auth.setPassword(u,np)) std::cout << "Password updated.\n"; else std::cout << "Failed.\n";
}

void AdminDashboard::deleteUser() {
	std::cout << "Username to delete: "; std::cout.flush(); std::string u; std::getline(std::cin,u);
	if (auth.deleteUser(u)) std::cout << "Deleted.\n"; else std::cout << "Failed.\n";
}

void AdminDashboard::resetPlayerStats() {
	std::cout << "Username: "; std::cout.flush(); std::string u; std::getline(std::cin,u);
	Player p(u); if (!p.loadFromDisk()) { std::cout << "No data.\n"; return; }
	p.resetStats(); p.saveToDisk(); std::cout << "Stats reset.\n";
}

void AdminDashboard::grantXp() {
	std::cout << "Username: "; std::cout.flush(); std::string u; std::getline(std::cin,u);
	std::cout << "XP to grant: "; std::cout.flush(); int amt = readInt(); if (amt < 0) { std::cout << "Invalid.\n"; return; }
	Player p(u); if (!p.loadFromDisk()) { std::cout << "No data.\n"; return; }
	p.addXp(amt); p.saveToDisk(); std::cout << "Granted.\n";
}

void AdminDashboard::manageCatalog() {
	std::cout << "1) Add Item  2) Save Catalog  3) Show Catalog  4) Back : "; std::cout.flush();
	int ch = readInt();
	if (ch==1) {
		std::cout << "Item name: "; std::cout.flush(); std::string n; std::getline(std::cin,n);
		std::cout << "Min level: "; std::cout.flush(); int lv = readInt(); if (lv<1) lv=1;
		std::cout << "Category (Gun Skin/Outfit/Emote/Parachute/Misc): "; std::cout.flush(); std::string cat; std::getline(std::cin,cat); if (cat.empty()) cat="Misc";
		CentralInventory::addItem(n, lv, cat);
		std::cout << "Added.\n";
	} else if (ch==2) {
		if (CentralInventory::saveToFile()) std::cout << "Catalog saved.\n"; else std::cout << "Save failed.\n";
	} else if (ch==3) {
		std::cout << "Catalog (by category):\n";
		for (const auto& cat : CentralInventory::listCategories()) {
			std::cout << "- " << cat << ":\n";
			for (const auto& item : CentralInventory::getByCategory(cat)) {
				std::cout << "  * " << item.name << " (L" << item.minLevel << ")\n";
			}
		}
	}
}


