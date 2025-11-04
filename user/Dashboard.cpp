#include "Dashboard.h"
#include "Match.h"
#include "../CentralInventory.h"
#include "../admin/AdminDashboard.h"
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <fstream>

static std::string nowString() {
	std::time_t t = std::time(nullptr);
	char buf[64];
	std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::localtime(&t));
	return buf;
}

void Dashboard::run() {
	std::srand((unsigned)std::time(nullptr));
	mainMenu();
}

void Dashboard::mainMenu() {
	while (true) {
		std::cout << "\n==== BGMI DASHBOARD ====\n";
		std::cout << "1. Register\n2. Login\n3. View Leaderboard\n4. Exit\n";
		std::cout << "Choose: " << std::flush;
		int ch = inputInt();
		if (ch == 1) handleRegister();
		else if (ch == 2) handleLogin();
		else if (ch == 3) handleLeaderboard();
		else if (ch == 4) { std::cout << "Goodbye!\n"; break; }
		else std::cout << "Invalid option.\n";
	}
}

void Dashboard::playerMenu() {
	while (currentPlayer) {
		std::cout << "\n==== PLAYER DASHBOARD ====\n";
		std::cout << "1. View Profile\n2. Start Match\n3. View Match History\n4. View Inventory\n5. View Other Players\n6. Logout\n";
		std::cout << "Choose: " << std::flush;
		int ch = inputInt();
		if (ch == 1) handleViewProfile();
		else if (ch == 2) handleStartMatch();
		else if (ch == 3) handleViewHistory();
		else if (ch == 4) handleViewInventory();
		else if (ch == 5) handleViewOtherPlayers();
		else if (ch == 6) {
			currentPlayer->saveToDisk();
			currentPlayer.reset();
			std::cout << "Logged out.\n";
			break;
		} else std::cout << "Invalid option.\n";
	}
}

void Dashboard::handleRegister() {
	std::string u,p;
	std::cout << "Enter new username: " << std::flush; std::getline(std::cin, u);
	std::cout << "Enter new password: " << std::flush; std::getline(std::cin, p);
	if (u.empty() || p.empty()) { std::cout << "Invalid input.\n"; return; }
	if (auth.registerUser(u,p)) std::cout << "Registered successfully.\n";
	else std::cout << "Username already exists.\n";
}

void Dashboard::handleLogin() {
	std::string u,p;
	std::cout << "Username: " << std::flush; std::getline(std::cin, u);
	std::cout << "Password: " << std::flush; std::getline(std::cin, p);
	if (auth.login(u,p)) {
		if (auth.getRole(u) == "admin") {
			std::cout << "Admin login successful.\n";
			AdminDashboard adb(auth); adb.run();
		} else {
			currentPlayer = std::make_unique<Player>(u);
			currentPlayer->loadFromDisk();
			std::cout << "Login successful.\n";
			playerMenu();
		}
	} else {
		std::cout << "Invalid credentials.\n";
	}
}

void Dashboard::handleLeaderboard() {
	std::vector<PublicPlayerInfo> infos;
	for (const auto& u : auth.listUsers()) {
		Player p(u); p.loadFromDisk();
		infos.push_back({u, p.getLevel(), p.getKD(), p.getRankTier(), p.getTotalWins()});
	}
	Leaderboard lb; lb.setEntries(infos);
	std::cout << "Sort by: 1) Rank 2) Level 3) KD : " << std::flush;
	int s = inputInt();
	if (s==2) lb.sortBy(SortBy::Level);
	else if (s==3) lb.sortBy(SortBy::KD);
	else lb.sortBy(SortBy::RankTier);
	int pos=1;
	for (const auto& e : lb.getEntries()) {
		std::cout << pos++ << ". " << e.username << " | Tier: " << e.tier << " | L" << e.level << " | KD: " << e.kd << " | Wins: " << e.wins << "\n";
	}
}

void Dashboard::handleViewProfile() {
	if (!currentPlayer) return;
	std::cout << "\n" << currentPlayer->publicSummary() << "\n";
}

void Dashboard::handleStartMatch() {
	if (!currentPlayer) return;
	std::cout << "Select Mode: 1) Solo 2) Duo 3) Squad: " << std::flush;
	int m = inputInt(); if (m<1||m>3) m=1;
	std::cout << "Select Map: 1) Erangel 2) Miramar 3) Sanhok 4) Vikendi 5) Livik: " << std::flush;
	int mp = inputInt(); if (mp<1||mp>5) mp=1;
	Match match;
	match.mode = (m==1?MatchMode::Solo:(m==2?MatchMode::Duo:MatchMode::Squad));
	match.map = (MatchMap)(mp-1);
	match.kills = std::rand()%16; // 0-15
	match.damage = 50 + (std::rand()%1000);
	match.rankPosition = 1 + (std::rand()%100);
	match.win = (match.rankPosition==1);
	match.timestamp = nowString();
	currentPlayer->addMatchAndUpdate(match);
	currentPlayer->saveToDisk();
	currentPlayer->saveMatches();
	std::cout << "Match saved. Kills: " << match.kills << ", Damage: " << match.damage << ", Rank: " << match.rankPosition << "\n";
}

void Dashboard::handleViewHistory() {
	if (!currentPlayer) return;
	// load and print last N from stack
	Player temp(currentPlayer->getUsername()); temp.loadMatches();
	std::cout << "Recent matches (latest first):\n";
	Player& p = *currentPlayer;
	// We'll re-read the file to present; simpler: reload into temp then print by popping
	std::string path = std::string("data/matches/") + p.getUsername() + ".txt";
	std::ifstream in(path);
	if (!in) { std::cout << "No history found.\n"; return; }
	std::vector<Match> list; std::string line; Match m;
	while (std::getline(in,line)) { if (Match::deserialize(line,m)) list.push_back(m);} 
	for (int i = (int)list.size()-1, shown=0; i>=0 && shown<10; --i,++shown) {
		const auto& a = list[i];
		std::cout << a.timestamp << " | " << Match::modeToString(a.mode) << " | " << Match::mapToString(a.map)
			<< " | K:" << a.kills << " Dmg:" << a.damage << " Rank:" << a.rankPosition << (a.win?" WIN":"") << "\n";
	}
}

void Dashboard::handleViewInventory() {
	if (!currentPlayer) return;
	const auto& items = currentPlayer->getInventory().getItems();
	std::cout << "Owned items (" << items.size() << ") by category:\n";
	// group by category via CentralInventory lookup
	std::vector<std::string> cats = CentralInventory::listCategories();
	cats.push_back("Misc");
	for (const auto& cat : cats) {
		bool any=false;
		for (const auto& it : items) {
			if (CentralInventory::getCategoryOf(it) == cat) {
				if (!any) { std::cout << "- " << cat << ":\n"; any=true; }
				std::cout << "  * " << it << "\n";
			}
		}
	}
}

void Dashboard::handleViewOtherPlayers() {
	std::cout << "All players:\n";
	for (const auto& u : auth.listUsers()) {
		Player p(u); p.loadFromDisk();
		std::cout << "- " << p.publicSummary() << "\n";
	}
}

int Dashboard::inputInt() {
	std::string s; std::getline(std::cin, s);
	try { return std::stoi(s); } catch (...) { return -1; }
}


