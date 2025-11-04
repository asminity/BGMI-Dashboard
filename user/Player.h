#pragma once
#include <string>
#include <vector>
#include <stack>
#include "../Common.h"
#include "Match.h"
#include "Inventory.h"
#include "Achievement.h"

class Player {
private:
	std::string username;
	int level{1};
	long long xp{0};
	int totalMatches{0};
	int totalWins{0};
	int totalKills{0};
	long long totalDamage{0};
	int totalDeaths{0};
	std::string rankTier{"Bronze"};
	Inventory inventory;
	std::vector<Achievement*> achievements; // owned by Player
	std::stack<Match> matchHistory; // latest on top

public:
	Player() = default;
	explicit Player(const std::string& user);
	~Player();

	const std::string& getUsername() const;
	int getLevel() const;
	long long getXp() const;
	int getTotalMatches() const;
	int getTotalWins() const;
	int getTotalKills() const;
	long long getTotalDamage() const;
	int getTotalDeaths() const;
	double getKD() const;
	const std::string& getRankTier() const;
	Inventory& getInventory();

	void addMatchAndUpdate(const Match& m);
	void addXp(long long amount);
	void recalcLevelAndTier();
	void checkAndUnlockLevelRewards();
	void initDefaultAchievements();
	void evaluateAchievements(const Match& m);
	void resetStats();

	// persistence
	bool saveToDisk() const;
	bool loadFromDisk();
	bool saveMatches() const;
	bool loadMatches();

	// public view snapshot
	std::string publicSummary() const;
};

