#pragma once
#include <string>
#include "../Common.h"
class Player;
struct Match;

class Achievement {
protected:
string name;
	bool completed{false};
	int rewardXp{0};
public:
	virtual ~Achievement() = default;
	virtual bool checkAndUpdate(Player& player, const Match& m) = 0;
	bool isCompleted() const { return completed; }
	int getRewardXp() const { return rewardXp; }
	virtual string serialize() const = 0;
	static Achievement* deserializeFactory(const string& line);
};

class KillAchievement : public Achievement {
private:
	int targetKills{0};
public:
	KillAchievement(int target, int xpReward);
	bool checkAndUpdate(Player& player, const Match& m) override;
string serialize() const override;
};

class WinAchievement : public Achievement {
private:
	int targetWins{0};
public:
	WinAchievement(int target, int xpReward);
	bool checkAndUpdate(Player& player, const Match& m) override;
string serialize() const override;
};

