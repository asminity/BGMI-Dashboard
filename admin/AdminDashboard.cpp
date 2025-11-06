#include "AdminDashboard.h"
#include "../CentralInventory.h"
#include "../user/Player.h"
#include <iostream>
#include <string>

static int readInt() { string s; getline(cin,s); try { return stoi(s);} catch(...) { return -1; } }

void AdminDashboard::run() {
	// Try to load catalog; if missing, keep defaults
	CentralInventory::loadFromFile();
	while (true) {
cout << "\n==== ADMIN DASHBOARD ====\n";
cout << "1. List Users\n2. View User Summary\n3. Reset User Password\n4. Delete User\n5. Reset Player Stats\n6. Grant XP to User\n7. Manage Catalog (Add/Save/Show)\n8. Back\n";
cout << "Choose: "; cout.flush();
		int ch = readInt();
		if (ch==1) listUsers();
		else if (ch==2) viewUserSummary();
		else if (ch==3) resetPassword();
		else if (ch==4) deleteUser();
		else if (ch==5) resetPlayerStats();
		else if (ch==6) grantXp();
		else if (ch==7) manageCatalog();
		else if (ch==8) { CentralInventory::saveToFile(); break; }
		else cout << "Invalid option.\n";
	}
}

void AdminDashboard::listUsers() {
cout << "Users:\n";
	for (const auto& u : auth.listUsers()) cout << "- " << u << " (" << auth.getRole(u) << ")\n";
}

void AdminDashboard::viewUserSummary() {
cout << "Username: "; cout.flush(); string u; getline(cin,u);
	Player p(u); if (!p.loadFromDisk()) { cout << "No data.\n"; return; }
cout << p.publicSummary() << "\n";
}

void AdminDashboard::resetPassword() {
cout << "Username: "; cout.flush(); string u; getline(cin,u);
cout << "New password: "; cout.flush(); string np; getline(cin,np);
	if (auth.setPassword(u,np)) cout << "Password updated.\n"; else cout << "Failed.\n";
}

void AdminDashboard::deleteUser() {
cout << "Username to delete: "; cout.flush(); string u; getline(cin,u);
	if (auth.deleteUser(u)) cout << "Deleted.\n"; else cout << "Failed.\n";
}

void AdminDashboard::resetPlayerStats() {
cout << "Username: "; cout.flush(); string u; getline(cin,u);
	Player p(u); if (!p.loadFromDisk()) { cout << "No data.\n"; return; }
	p.resetStats(); p.saveToDisk(); cout << "Stats reset.\n";
}

void AdminDashboard::grantXp() {
cout << "Username: "; cout.flush(); string u; getline(cin,u);
cout << "XP to grant: "; cout.flush(); int amt = readInt(); if (amt < 0) { cout << "Invalid.\n"; return; }
	Player p(u); if (!p.loadFromDisk()) { cout << "No data.\n"; return; }
	p.addXp(amt); p.saveToDisk(); cout << "Granted.\n";
}

void AdminDashboard::manageCatalog() {
cout << "1) Add Item  2) Save Catalog  3) Show Catalog  4) Back : "; cout.flush();
	int ch = readInt();
	if (ch==1) {
cout << "Item name: "; cout.flush(); string n; getline(cin,n);
cout << "Min level: "; cout.flush(); int lv = readInt(); if (lv<1) lv=1;
cout << "Category (Gun Skin/Outfit/Emote/Parachute/Misc): "; cout.flush(); string cat; getline(cin,cat); if (cat.empty()) cat="Misc";
		CentralInventory::addItem(n, lv, cat);
cout << "Added.\n";
	} else if (ch==2) {
		if (CentralInventory::saveToFile()) cout << "Catalog saved.\n"; else cout << "Save failed.\n";
	} else if (ch==3) {
cout << "Catalog (by category):\n";
		for (const auto& cat : CentralInventory::listCategories()) {
cout << "- " << cat << ":\n";
			for (const auto& item : CentralInventory::getByCategory(cat)) {
cout << "  * " << item.name << " (L" << item.minLevel << ")\n";
			}
		}
	}
}


