#pragma once
#include <string>
#include "../Common.h"

enum class MatchMode { Solo=1, Duo=2, Squad=4 };
enum class MatchMap { Erangel, Miramar, Sanhok, Vikendi, Livik };

struct Match {
	MatchMode mode{MatchMode::Solo};
	MatchMap map{MatchMap::Erangel};
	int kills{0};
	long long damage{0};
	int rankPosition{100};
string timestamp;
	bool win{false};

string serialize() const;
	static bool deserialize(const string& line, Match& out);
	static string modeToString(MatchMode m);
	static string mapToString(MatchMap m);
};

