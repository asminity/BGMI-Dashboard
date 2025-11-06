#include "Match.h"
#include <sstream>

string Match::modeToString(MatchMode m) {
	switch (m) {
		case MatchMode::Solo: return "Solo";
		case MatchMode::Duo: return "Duo";
		case MatchMode::Squad: return "Squad";
	}
	return "Solo";
}

string Match::mapToString(MatchMap m) {
	switch (m) {
		case MatchMap::Erangel: return "Erangel";
		case MatchMap::Miramar: return "Miramar";
		case MatchMap::Sanhok: return "Sanhok";
		case MatchMap::Vikendi: return "Vikendi";
		case MatchMap::Livik: return "Livik";
	}
	return "Erangel";
}

string Match::serialize() const {
ostringstream os;
	os << (int)mode << "," << (int)map << "," << kills << "," << damage << "," << rankPosition << "," << (win?1:0) << "," << timestamp;
	return os.str();
}

bool Match::deserialize(const string& line, Match& out) {
istringstream is(line);
string token;
	int mm=0, mp=0, w=0;
	if (!getline(is, token, ',')) return false; mm = stoi(token);
	if (!getline(is, token, ',')) return false; mp = stoi(token);
	if (!getline(is, token, ',')) return false; out.kills = stoi(token);
	if (!getline(is, token, ',')) return false; out.damage = stoll(token);
	if (!getline(is, token, ',')) return false; out.rankPosition = stoi(token);
	if (!getline(is, token, ',')) return false; w = stoi(token);
	if (!getline(is, out.timestamp)) out.timestamp = "";
	out.mode = (MatchMode)mm;
	out.map = (MatchMap)mp;
	out.win = (w==1);
	return true;
}

