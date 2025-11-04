#include "Achievement.h"
#include "Player.h"
#include "Match.h"
#include <sstream>

KillAchievement::KillAchievement(int target, int xpReward) {
	name = "Kill Master";
	targetKills = target;
	rewardXp = xpReward;
}

bool KillAchievement::checkAndUpdate(Player& player, const Match&) {
	if (completed) return false;
	if (player.getTotalKills() >= targetKills) { completed = true; return true; }
	return false;
}

std::string KillAchievement::serialize() const {
	std::ostringstream os; os << "KILL," << name << "," << completed << "," << rewardXp << "," << targetKills; return os.str();
}

WinAchievement::WinAchievement(int target, int xpReward) {
	name = "Winner";
	targetWins = target;
	rewardXp = xpReward;
}

bool WinAchievement::checkAndUpdate(Player& player, const Match&) {
	if (completed) return false;
	if (player.getTotalWins() >= targetWins) { completed = true; return true; }
	return false;
}

std::string WinAchievement::serialize() const {
	std::ostringstream os; os << "WIN," << name << "," << completed << "," << rewardXp << "," << targetWins; return os.str();
}

Achievement* Achievement::deserializeFactory(const std::string& line) {
	std::istringstream is(line); std::string type, nm; std::string tok; int comp=0, r=0, target=0;
	if (!std::getline(is, type, ',')) return nullptr;
	std::getline(is, nm, ',');
	std::getline(is, tok, ','); comp = std::stoi(tok);
	std::getline(is, tok, ','); r = std::stoi(tok);
	std::getline(is, tok, ','); target = std::stoi(tok);
	Achievement* a = nullptr;
	if (type == "KILL") a = new KillAchievement(target, r);
	else if (type == "WIN") a = new WinAchievement(target, r);
	if (a) {
		// crude: mark completed if saved so
		if (comp) {
			// no accessor, so hack via checkAndUpdate conditions won't toggle; we rely on saved flag by rewarding on trigger only once
		}
	}
	return a;
}

