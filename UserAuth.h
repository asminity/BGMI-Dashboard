#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include "Common.h"

class UserAuth {
private:
	struct Cred { string password; string role; };
unordered_map<string, Cred> credentials; // username->{password,role}
string usersFile{"data/users.txt"};
public:
	UserAuth();
	bool load();
	bool save();
	bool registerUser(const string& username, const string& password);
	bool login(const string& username, const string& password);
	bool userExists(const string& username) const;
vector<string> listUsers() const;
string getRole(const string& username) const;
	bool setPassword(const string& username, const string& newPass);
	bool deleteUser(const string& username);
private:
	void ensureDefaultAdmin();
};


