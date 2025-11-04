#include "Match.h"
#include <sstream>

std::string Match::modeToString(MatchMode m) {
	switch (m) {
		case MatchMode::Solo: return "Solo";
		case MatchMode::Duo: return "Duo";
		case MatchMode::Squad: return "Squad";
	}
	return "Solo";
}

std::string Match::mapToString(MatchMap m) {
	switch (m) {
		case MatchMap::Erangel: return "Erangel";
		case MatchMap::Miramar: return "Miramar";
		case MatchMap::Sanhok: return "Sanhok";
		case MatchMap::Vikendi: return "Vikendi";
		case MatchMap::Livik: return "Livik";
	}
	return "Erangel";
}

std::string Match::serialize() const {
	std::ostringstream os;
	os << (int)mode << "," << (int)map << "," << kills << "," << damage << "," << rankPosition << "," << (win?1:0) << "," << timestamp;
	return os.str();
}

bool Match::deserialize(const std::string& line, Match& out) {
	std::istringstream is(line);
	std::string token;
	int mm=0, mp=0, w=0;
	if (!std::getline(is, token, ',')) return false; mm = std::stoi(token);
	if (!std::getline(is, token, ',')) return false; mp = std::stoi(token);
	if (!std::getline(is, token, ',')) return false; out.kills = std::stoi(token);
	if (!std::getline(is, token, ',')) return false; out.damage = std::stoll(token);
	if (!std::getline(is, token, ',')) return false; out.rankPosition = std::stoi(token);
	if (!std::getline(is, token, ',')) return false; w = std::stoi(token);
	if (!std::getline(is, out.timestamp)) out.timestamp = "";
	out.mode = (MatchMode)mm;
	out.map = (MatchMap)mp;
	out.win = (w==1);
	return true;
}

