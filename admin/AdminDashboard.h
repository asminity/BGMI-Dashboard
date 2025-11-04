#pragma once
#include <string>
#include "../Common.h"
#include "../UserAuth.h"

// Forward declaration
class Player;

class AdminDashboard {
private:
	UserAuth& auth;
public:
	explicit AdminDashboard(UserAuth& a) : auth(a) {}
	void run();
private:
	void listUsers();
	void viewUserSummary();
	void resetPassword();
	void deleteUser();
	void resetPlayerStats();
	void grantXp();
	void manageCatalog();
};


