#pragma once
#include <vector>
#include <string>
#include "../Common.h"

struct PublicPlayerInfo {
	std::string username;
	int level;
	double kd;
	std::string tier;
	int wins;
};

enum class SortBy { RankTier, Level, KD };

class Leaderboard {
private:
	std::vector<PublicPlayerInfo> entries;
public:
	void setEntries(const std::vector<PublicPlayerInfo>& e);
	void sortBy(SortBy s);
	const std::vector<PublicPlayerInfo>& getEntries() const;
};

