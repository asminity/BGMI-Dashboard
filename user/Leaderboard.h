#pragma once
#include <vector>
#include <string>
#include "../Common.h"

struct PublicPlayerInfo {
string username;
	int level;
	double kd;
string tier;
	int wins;
};

enum class SortBy { RankTier, Level, KD };

class Leaderboard {
private:
vector<PublicPlayerInfo> entries;
public:
	void setEntries(const vector<PublicPlayerInfo>& e);
	void sortBy(SortBy s);
	const vector<PublicPlayerInfo>& getEntries() const;
};

