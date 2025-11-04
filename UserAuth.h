#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include "Common.h"

class UserAuth {
private:
	struct Cred { std::string password; std::string role; };
	std::unordered_map<std::string, Cred> credentials; // username->{password,role}
	std::string usersFile{"data/users.txt"};
public:
	UserAuth();
	bool load();
	bool save();
	bool registerUser(const std::string& username, const std::string& password);
	bool login(const std::string& username, const std::string& password);
	bool userExists(const std::string& username) const;
	std::vector<std::string> listUsers() const;
	std::string getRole(const std::string& username) const;
	bool setPassword(const std::string& username, const std::string& newPass);
	bool deleteUser(const std::string& username);
private:
	void ensureDefaultAdmin();
};


