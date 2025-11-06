#include "UserAuth.h"
#include <fstream>
#include <sstream>
#include <cstdio>
#include "FileUtil.h"
#include "user/Player.h"

UserAuth::UserAuth() { load(); ensureDefaultAdmin(); save(); }

bool UserAuth::load() {
	ensureDir("data");
	credentials.clear();
ifstream in(usersFile);
	if (!in) return false;
string line;
	while (getline(in, line)) {
		if (line.empty()) continue;
istringstream is(line); string u,p,r;
		if (getline(is, u, ',') && getline(is, p, ',') ) {
			if (!getline(is, r)) r = "user"; // backward compatible
			credentials[u] = Cred{p,r};
		}
	}
	return true;
}

bool UserAuth::save() {
ofstream out(usersFile, ios::trunc);
	if (!out) return false;
	for (const auto& kv : credentials) out << kv.first << "," << kv.second.password << "," << kv.second.role << "\n";
	return true;
}

bool UserAuth::registerUser(const string& username, const string& password) {
	if (userExists(username)) return false;
	credentials[username] = Cred{password, "user"};
	save();
	// create default player file
	Player p(username);
	p.saveToDisk();
	return true;
}

bool UserAuth::login(const string& username, const string& password) {
	auto it = credentials.find(username);
	return it != credentials.end() && it->second.password == password;
}

bool UserAuth::userExists(const string& username) const {
	return credentials.find(username) != credentials.end();
}

vector<string> UserAuth::listUsers() const {
vector<string> v;
	v.reserve(credentials.size());
	for (const auto& kv : credentials) v.push_back(kv.first);
	return v;
}

string UserAuth::getRole(const string& username) const {
	auto it = credentials.find(username);
	if (it == credentials.end()) return "";
	return it->second.role;
}

bool UserAuth::setPassword(const string& username, const string& newPass) {
	auto it = credentials.find(username);
	if (it == credentials.end()) return false;
	it->second.password = newPass;
	return save();
}

bool UserAuth::deleteUser(const string& username) {
	auto it = credentials.find(username);
	if (it == credentials.end()) return false;
	if (it->first == "admin") return false; // protect default admin
	credentials.erase(it);
	save();
	// remove player files
string pfile = string("data/players/") + username + ".txt";
string mfile = string("data/matches/") + username + ".txt";
remove(pfile.c_str());
remove(mfile.c_str());
	return true;
}

void UserAuth::ensureDefaultAdmin() {
	if (credentials.find("admin") == credentials.end()) {
		credentials["admin"] = Cred{"admin","admin"};
	}
}


