#include "Player.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include "../FileUtil.h"

Player::Player(const std::string& user) : username(user) {
	initDefaultAchievements();
}

Player::~Player() {
	for (auto* a : achievements) delete a;
	achievements.clear();
}

const std::string& Player::getUsername() const { return username; }
int Player::getLevel() const { return level; }
long long Player::getXp() const { return xp; }
int Player::getTotalMatches() const { return totalMatches; }
int Player::getTotalWins() const { return totalWins; }
int Player::getTotalKills() const { return totalKills; }
long long Player::getTotalDamage() const { return totalDamage; }
int Player::getTotalDeaths() const { return totalDeaths; }
double Player::getKD() const { return totalDeaths == 0 ? (double)totalKills : (double)totalKills / (double)totalDeaths; }
const std::string& Player::getRankTier() const { return rankTier; }
Inventory& Player::getInventory() { return inventory; }

void Player::addXp(long long amount) {
	xp += amount;
	recalcLevelAndTier();
}

void Player::recalcLevelAndTier() {
	int newLevel = (int)(xp / 1000) + 1;
	if (newLevel < 1) newLevel = 1;
	level = newLevel;
	if (level <= 10) rankTier = "Bronze";
	else if (level <= 20) rankTier = "Silver";
	else if (level <= 30) rankTier = "Gold";
	else if (level <= 40) rankTier = "Platinum";
	else if (level <= 50) rankTier = "Diamond";
	else if (level <= 60) rankTier = "Crown";
	else if (level <= 70) rankTier = "Ace";
	else rankTier = "Conqueror";
	checkAndUnlockLevelRewards();
}

void Player::checkAndUnlockLevelRewards() {
	inventory.unlockByLevel(level);
}

void Player::initDefaultAchievements() {
	for (auto* a : achievements) delete a;
	achievements.clear();
	achievements.push_back(new KillAchievement(10, 200));
	achievements.push_back(new WinAchievement(5, 300));
}

void Player::evaluateAchievements(const Match& m) {
	for (auto* a : achievements) {
		if (!a->isCompleted() && a->checkAndUpdate(*this, m)) {
			addXp(a->getRewardXp());
		}
	}
}

void Player::addMatchAndUpdate(const Match& m) {
	matchHistory.push(m);
	++totalMatches;
	totalKills += m.kills;
	if (m.rankPosition == 1) ++totalWins;
	long long dmg = m.damage;
	if (dmg < 0) dmg = 0;
	totalDamage += dmg;
	totalDeaths += (m.rankPosition == 1 ? 0 : 1);

	long long earnedXp = (m.kills * 20) + (m.damage / 10) + ((100 - m.rankPosition) * 5);
	if (earnedXp < 0) earnedXp = 0;
	addXp(earnedXp);
	evaluateAchievements(m);
}

void Player::resetStats() {
	level = 1;
	xp = 0;
	totalMatches = 0;
	totalWins = 0;
	totalKills = 0;
	totalDamage = 0;
	totalDeaths = 0;
	rankTier = "Bronze";
	initDefaultAchievements();
	while (!matchHistory.empty()) matchHistory.pop();
	saveMatches();
}

bool Player::saveMatches() const {
	ensureDir("data/matches");
	std::string path = std::string("data/matches/") + username + ".txt";
	std::ofstream out(path, std::ios::trunc);
	if (!out) return false;
	// dump from oldest to newest
	std::vector<Match> temp;
	std::stack<Match> copy = matchHistory;
	while (!copy.empty()) { temp.push_back(copy.top()); copy.pop(); }
	for (int i = (int)temp.size() - 1; i >= 0; --i) {
		out << temp[i].serialize() << "\n";
	}
	return true;
}

bool Player::loadMatches() {
	while (!matchHistory.empty()) matchHistory.pop();
	std::string path = std::string("data/matches/") + username + ".txt";
	std::ifstream in(path);
	if (!in) return false;
	std::string line;
	std::vector<Match> list;
	while (std::getline(in, line)) {
		if (line.empty()) continue;
		Match m;
		if (Match::deserialize(line, m)) list.push_back(m);
	}
	for (const auto& m : list) matchHistory.push(m);
	return true;
}

bool Player::saveToDisk() const {
	ensureDir("data/players");
	std::string path = std::string("data/players/") + username + ".txt";
	std::ofstream out(path, std::ios::trunc);
	if (!out) return false;
	out << username << "\n" << level << " " << xp << "\n";
	out << totalMatches << " " << totalWins << " " << totalKills << " " << totalDamage << " " << totalDeaths << "\n";
	out << rankTier << "\n";
	out << inventory.serialize() << "\n";
	// achievements state
	out << achievements.size() << "\n";
	for (auto* a : achievements) out << a->serialize() << "\n";
	saveMatches();
	return true;
}

bool Player::loadFromDisk() {
	std::string path = std::string("data/players/") + username + ".txt";
	std::ifstream in(path);
	if (!in) return false;
	std::string line;
	std::getline(in, username);
	in >> level >> xp; in.ignore();
	in >> totalMatches >> totalWins >> totalKills >> totalDamage >> totalDeaths; in.ignore();
	std::getline(in, rankTier);
	std::getline(in, line);
	inventory.deserialize(line);
	size_t achCount = 0; in >> achCount; in.ignore();
	initDefaultAchievements();
	for (size_t i = 0; i < achCount; ++i) {
		std::getline(in, line);
		Achievement* a = Achievement::deserializeFactory(line);
		if (a) { achievements.push_back(a); }
	}
	loadMatches();
	return true;
}

std::string Player::publicSummary() const {
	std::ostringstream os;
	os << username << ", L" << level << ", Tier: " << rankTier
	   << ", KD: " << (double)((int)(getKD()*100))/100.0
	   << ", Matches: " << totalMatches << ", Wins: " << totalWins;
	return os.str();
}

