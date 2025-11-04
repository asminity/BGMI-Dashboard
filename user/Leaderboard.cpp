#include "Leaderboard.h"
#include <algorithm>

void Leaderboard::setEntries(const std::vector<PublicPlayerInfo>& e) { entries = e; }

static int tierOrder(const std::string& t) {
	if (t == "Conqueror") return 8;
	if (t == "Ace") return 7;
	if (t == "Crown") return 6;
	if (t == "Diamond") return 5;
	if (t == "Platinum") return 4;
	if (t == "Gold") return 3;
	if (t == "Silver") return 2;
	return 1; // Bronze
}

void Leaderboard::sortBy(SortBy s) {
	if (s == SortBy::KD) {
		std::sort(entries.begin(), entries.end(), [](const auto& a, const auto& b){ return a.kd > b.kd; });
	} else if (s == SortBy::Level) {
		std::sort(entries.begin(), entries.end(), [](const auto& a, const auto& b){ return a.level > b.level; });
	} else {
		std::sort(entries.begin(), entries.end(), [](const auto& a, const auto& b){
			int ta = tierOrder(a.tier), tb = tierOrder(b.tier);
			if (ta != tb) return ta > tb;
			return a.level > b.level;
		});
	}
}

const std::vector<PublicPlayerInfo>& Leaderboard::getEntries() const { return entries; }

