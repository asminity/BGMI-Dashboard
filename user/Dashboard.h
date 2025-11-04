#pragma once
#include <string>
#include <vector>
#include <memory>
#include "../Common.h"
#include "../UserAuth.h"
#include "Leaderboard.h"
#include "Player.h"

class Dashboard {
private:
	UserAuth auth;
	std::unique_ptr<Player> currentPlayer;
public:
	void run();
private:
	void mainMenu();
	void playerMenu();
	void handleRegister();
	void handleLogin();
	void handleLeaderboard();
	void handleViewProfile();
	void handleStartMatch();
	void handleViewHistory();
	void handleViewInventory();
	void handleViewOtherPlayers();
	static int inputInt();
};


