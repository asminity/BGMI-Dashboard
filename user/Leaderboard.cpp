#include "Leaderboard.h"
#include <algorithm>

void Leaderboard::setEntries(const vector<PublicPlayerInfo>& e) { entries = e; }

static int tierOrder(const string& t) {
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
sort(entries.begin(), entries.end(), [](const auto& a, const auto& b){ return a.kd > b.kd; });
	} else if (s == SortBy::Level) {
sort(entries.begin(), entries.end(), [](const auto& a, const auto& b){ return a.level > b.level; });
	} else {
sort(entries.begin(), entries.end(), [](const auto& a, const auto& b){
			int ta = tierOrder(a.tier), tb = tierOrder(b.tier);
			if (ta != tb) return ta > tb;
			return a.level > b.level;
		});
	}
}

const vector<PublicPlayerInfo>& Leaderboard::getEntries() const { return entries; }

