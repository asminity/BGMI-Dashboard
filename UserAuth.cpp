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
	std::ifstream in(usersFile);
	if (!in) return false;
	std::string line;
	while (std::getline(in, line)) {
		if (line.empty()) continue;
		std::istringstream is(line); std::string u,p,r;
		if (std::getline(is, u, ',') && std::getline(is, p, ',') ) {
			if (!std::getline(is, r)) r = "user"; // backward compatible
			credentials[u] = Cred{p,r};
		}
	}
	return true;
}

bool UserAuth::save() {
	std::ofstream out(usersFile, std::ios::trunc);
	if (!out) return false;
	for (const auto& kv : credentials) out << kv.first << "," << kv.second.password << "," << kv.second.role << "\n";
	return true;
}

bool UserAuth::registerUser(const std::string& username, const std::string& password) {
	if (userExists(username)) return false;
	credentials[username] = Cred{password, "user"};
	save();
	// create default player file
	Player p(username);
	p.saveToDisk();
	return true;
}

bool UserAuth::login(const std::string& username, const std::string& password) {
	auto it = credentials.find(username);
	return it != credentials.end() && it->second.password == password;
}

bool UserAuth::userExists(const std::string& username) const {
	return credentials.find(username) != credentials.end();
}

std::vector<std::string> UserAuth::listUsers() const {
	std::vector<std::string> v;
	v.reserve(credentials.size());
	for (const auto& kv : credentials) v.push_back(kv.first);
	return v;
}

std::string UserAuth::getRole(const std::string& username) const {
	auto it = credentials.find(username);
	if (it == credentials.end()) return "";
	return it->second.role;
}

bool UserAuth::setPassword(const std::string& username, const std::string& newPass) {
	auto it = credentials.find(username);
	if (it == credentials.end()) return false;
	it->second.password = newPass;
	return save();
}

bool UserAuth::deleteUser(const std::string& username) {
	auto it = credentials.find(username);
	if (it == credentials.end()) return false;
	if (it->first == "admin") return false; // protect default admin
	credentials.erase(it);
	save();
	// remove player files
	std::string pfile = std::string("data/players/") + username + ".txt";
	std::string mfile = std::string("data/matches/") + username + ".txt";
	std::remove(pfile.c_str());
	std::remove(mfile.c_str());
	return true;
}

void UserAuth::ensureDefaultAdmin() {
	if (credentials.find("admin") == credentials.end()) {
		credentials["admin"] = Cred{"admin","admin"};
	}
}


