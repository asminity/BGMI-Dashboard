#include "Dashboard.h"
#include "Match.h"
#include "../CentralInventory.h"
#include "../admin/AdminDashboard.h"
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <fstream>

static string nowString() {
	time_t t = time(nullptr);
	char buf[64];
	strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", localtime(&t));
	return buf;
}

void Dashboard::run() {
	srand((unsigned)time(nullptr));
	mainMenu();
}

void Dashboard::mainMenu() {
	while (true) {
		cout << "\n==== BGMI DASHBOARD ====\n";
		cout << "1. Register\n2. Login\n3. View Leaderboard\n4. Exit\n";
		cout << "Choose: " << flush;
		int ch = inputInt();
		if (ch == 1) handleRegister();
		else if (ch == 2) handleLogin();
		else if (ch == 3) handleLeaderboard();
		else if (ch == 4) { cout << "Goodbye!\n"; break; }
		else cout << "Invalid option.\n";
	}
}

void Dashboard::playerMenu() {
	while (currentPlayer) {
		cout << "\n==== PLAYER DASHBOARD ====\n";
		cout << "1. View Profile\n2. Start Match\n3. View Match History\n4. View Inventory\n5. View Other Players\n6. Logout\n";
		cout << "Choose: " << flush;
		int ch = inputInt();
		if (ch == 1) handleViewProfile();
		else if (ch == 2) handleStartMatch();
		else if (ch == 3) handleViewHistory();
		else if (ch == 4) handleViewInventory();
		else if (ch == 5) handleViewOtherPlayers();
		else if (ch == 6) {
			currentPlayer->saveToDisk();
			currentPlayer.reset();
			cout << "Logged out.\n";
			break;
		} else cout << "Invalid option.\n";
	}
}

void Dashboard::handleRegister() {
	string u,p;
	cout << "Enter new username: " << flush; getline(cin, u);
	cout << "Enter new password: " << flush; getline(cin, p);
	if (u.empty() || p.empty()) { cout << "Invalid input.\n"; return; }
	if (auth.registerUser(u,p)) cout << "Registered successfully.\n";
	else cout << "Username already exists.\n";
}

void Dashboard::handleLogin() {
	string u,p;
	cout << "Username: " << flush; getline(cin, u);
	cout << "Password: " << flush; getline(cin, p);
	if (auth.login(u,p)) {
		if (auth.getRole(u) == "admin") {
			cout << "Admin login successful.\n";
			AdminDashboard adb(auth); adb.run();
		} else {
			currentPlayer = make_unique<Player>(u);
			currentPlayer->loadFromDisk();
			cout << "Login successful.\n";
			playerMenu();
		}
	} else {
		cout << "Invalid credentials.\n";
	}
}

void Dashboard::handleLeaderboard() {
	vector<PublicPlayerInfo> infos;
	for (const auto& u : auth.listUsers()) {
		Player p(u); p.loadFromDisk();
		infos.push_back({u, p.getLevel(), p.getKD(), p.getRankTier(), p.getTotalWins()});
	}
	Leaderboard lb; lb.setEntries(infos);
	cout << "Sort by: 1) Rank 2) Level 3) KD : " << flush;
	int s = inputInt();
	if (s==2) lb.sortBy(SortBy::Level);
	else if (s==3) lb.sortBy(SortBy::KD);
	else lb.sortBy(SortBy::RankTier);
	int pos=1;
	for (const auto& e : lb.getEntries()) {
		cout << pos++ << ". " << e.username << " | Tier: " << e.tier << " | L" << e.level << " | KD: " << e.kd << " | Wins: " << e.wins << "\n";
	}
}

void Dashboard::handleViewProfile() {
	if (!currentPlayer) return;
	cout << "\n" << currentPlayer->publicSummary() << "\n";
}

void Dashboard::handleStartMatch() {
	if (!currentPlayer) return;
	cout << "Select Mode: 1) Solo 2) Duo 3) Squad: " << flush;
	int m = inputInt(); if (m<1||m>3) m=1;
	cout << "Select Map: 1) Erangel 2) Miramar 3) Sanhok 4) Vikendi 5) Livik: " << flush;
	int mp = inputInt(); if (mp<1||mp>5) mp=1;
	Match match;
	match.mode = (m==1?MatchMode::Solo:(m==2?MatchMode::Duo:MatchMode::Squad));
	match.map = (MatchMap)(mp-1);
	match.kills = rand()%16; // 0-15
	match.damage = 50 + (rand()%1000);
	match.rankPosition = 1 + (rand()%100);
	match.win = (match.rankPosition==1);
	match.timestamp = nowString();
	currentPlayer->addMatchAndUpdate(match);
	currentPlayer->saveToDisk();
	currentPlayer->saveMatches();
	cout << "Match saved. Kills: " << match.kills << ", Damage: " << match.damage << ", Rank: " << match.rankPosition << "\n";
}

void Dashboard::handleViewHistory() {
	if (!currentPlayer) return;
	// load and print last N from stack
	Player temp(currentPlayer->getUsername()); temp.loadMatches();
	cout << "Recent matches (latest first):\n";
	Player& p = *currentPlayer;
	// We'll re-read the file to present; simpler: reload into temp then print by popping
	string path = string("data/matches/") + p.getUsername() + ".txt";
	ifstream in(path);
	if (!in) { cout << "No history found.\n"; return; }
	vector<Match> list; string line; Match m;
	while (getline(in,line)) { if (Match::deserialize(line,m)) list.push_back(m);} 
	for (int i = (int)list.size()-1, shown=0; i>=0 && shown<10; --i,++shown) {
		const auto& a = list[i];
		cout << a.timestamp << " | " << Match::modeToString(a.mode) << " | " << Match::mapToString(a.map)
			<< " | K:" << a.kills << " Dmg:" << a.damage << " Rank:" << a.rankPosition << (a.win?" WIN":"") << "\n";
	}
}

void Dashboard::handleViewInventory() {
	if (!currentPlayer) return;
	const auto& items = currentPlayer->getInventory().getItems();
	cout << "Owned items (" << items.size() << ") by category:\n";
	// group by category via CentralInventory lookup
	vector<string> cats = CentralInventory::listCategories();
	cats.push_back("Misc");
	for (const auto& cat : cats) {
		bool any=false;
		for (const auto& it : items) {
			if (CentralInventory::getCategoryOf(it) == cat) {
				if (!any) { cout << "- " << cat << ":\n"; any=true; }
				cout << "  * " << it << "\n";
			}
		}
	}
}

void Dashboard::handleViewOtherPlayers() {
	cout << "All players:\n";
	for (const auto& u : auth.listUsers()) {
		Player p(u); p.loadFromDisk();
		cout << "- " << p.publicSummary() << "\n";
	}
}

int Dashboard::inputInt() {
	string s; getline(cin, s);
	try { return stoi(s); } catch (...) { return -1; }
}


