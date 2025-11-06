276480
# Project Classes and Methods

This document provides a summary of the classes and methods used in this project.

---

## `CentralInventory.h`

### Class `CentralInventory`
- **`initializeDefaults()`**: Initializes the catalog with default items.
- **`loadFromFile(const string& path)`**: Loads the item catalog from a file.
- **`saveToFile(const string& path)`**: Saves the item catalog to a file.
- **`getCatalog()`**: Returns a constant reference to the item catalog.
- **`addItem(const string& name, int minLevel, const string& category)`**: Adds a new item to the catalog.
- **`listCategories()`**: Returns a list of all unique item categories.
- **`getByCategory(const string& category)`**: Returns a list of catalog items belonging to a specific category.
- **`getCategoryOf(const string& name)`**: Returns the category of a specific item.

---

## `FileUtil.h`

- **`ensureDir(const char* path)`**: Ensures that a directory exists, creating it if necessary.
- **`ensureDataDirs()`**: Ensures that all necessary data directories are present.

---

## `UserAuth.h`

### Class `UserAuth`
- **`UserAuth()`**: Constructor for the UserAuth class.
- **`load()`**: Loads user credentials from a file.
- **`save()`**: Saves user credentials to a file.
- **`registerUser(const string& username, const string& password)`**: Registers a new user.
- **`login(const string& username, const string& password)`**: Authenticates a user.
- **`userExists(const string& username) const`**: Checks if a user exists.
- **`listUsers() const`**: Returns a list of all registered usernames.
- **`getRole(const string& username) const`**: Returns the role of a specific user.
- **`setPassword(const string& username, const string& newPass)`**: Sets a new password for a user.
-deleteUser(const string& username)`**: Deletes a user.

---

## `admin/AdminDashboard.h`

### Class `AdminDashboard`
- **`AdminDashboard(UserAuth& a)`**: Constructor for the AdminDashboard class.
- **`run()`**: Starts the admin dashboard interface.
- **`listUsers()`**: Lists all registered users.
- **`viewUserSummary()`**: Displays a summary of a specific user's profile.
- **`resetPassword()`**: Resets a user's password.
- **`deleteUser()`**: Deletes a user account.
- **`resetPlayerStats()`**: Resets a player's statistics.
- **`grantXp()`**: Grants experience points to a player.
- **`manageCatalog()`**: Manages the item catalog.

---

## `user/Achievement.h`

### Class `Achievement`
- **`~Achievement()`**: Virtual destructor for the Achievement class.
- **`checkAndUpdate(Player& player, const Match& m)`**: Checks if the achievement has been earned and updates its status.
- **`isCompleted() const`**: Returns whether the achievement has been completed.
- **`getRewardXp() const`**: Returns the XP reward for completing the achievement.
- **`serialize() const`**: Serializes the achievement's data to a string.
- **`deserializeFactory(const string& line)`**: Creates an Achievement object from a serialized string.

### Class `KillAchievement`
- **`KillAchievement(int target, int xpReward)`**: Constructor for the KillAchievement class.
- **`checkAndUpdate(Player& player, const Match& m)`**: Checks if the kill target has been met.
- **`serialize() const`**: Serializes the kill achievement's data.

### Class `WinAchievement`
- **`WinAchievement(int target, int xpReward)`**: Constructor for the WinAchievement class.
- **`checkAndUpdate(Player& player, const Match& m)`**: Checks if the win target has been met.
- **`serialize() const`**: Serializes the win achievement's data.

---

## `user/Dashboard.h`

### Class `Dashboard`
- **`run()`**: Starts the main user dashboard.
- **`mainMenu()`**: Displays the main menu.
- **`playerMenu()`**: Displays the player-specific menu.
- **`handleRegister()`**: Handles the user registration process.
- **`handleLogin()`**: Handles the user login process.
- **`handleLeaderboard()`**: Displays the leaderboard.
- **`handleViewProfile()`**: Displays the current player's profile.
- **`handleStartMatch()`**: Starts a new match for the player.
- **`handleViewHistory()`**: Displays the player's match history.
- **`handleViewInventory()`**: Displays the player's inventory.
- **`handleViewOtherPlayers()`**: Allows viewing other players' profiles.

---

## `user/Inventory.h`

### Class `Inventory`
- **`Inventory()`**: Default constructor for the Inventory class.
- **`unlockByLevel(int level)`**: Unlocks items based on the player's level.
- **`addItem(const string& item)`**: Adds an item to the inventory.
- **`hasItem(const string& item) const`**: Checks if the inventory contains a specific item.
- **`getItems() const`**: Returns a list of all items in the inventory.
- **`serialize() const`**: Serializes the inventory to a string.
- **`deserialize(const string& line)`**: Deserializes the inventory from a string.

---

## `user/Leaderboard.h`

### Class `Leaderboard`
- **`setEntries(const vector<PublicPlayerInfo>& e)`**: Sets the entries for the leaderboard.
- **`sortBy(SortBy s)`**: Sorts the leaderboard entries based on a specified criterion.
- **`getEntries() const`**: Returns the leaderboard entries.

---

## `user/Match.h`

### Struct `Match`
- **`serialize() const`**: Serializes the match data to a string.
- **`deserialize(const string& line, Match& out)`**: Deserializes match data from a string.
- **`modeToString(MatchMode m)`**: Converts a MatchMode enum to its string representation.
- **`mapToString(MatchMap m)`**: Converts a MatchMap enum to its string representation.

---

## `user/Player.h`

### Class `Player`
- **`Player(const string& user)`**: Constructor for the Player class.
- **`~Player()`**: Destructor for the Player class.
- **`getUsername() const`**: Returns the player's username.
- **`getLevel() const`**: Returns the player's level.
- **`getXp() const`**: Returns the player's experience points.
- **`getTotalMatches() const`**: Returns the total number of matches played.
- **`getTotalWins() const`**: Returns the total number of wins.
- **`getTotalKills() const`**: Returns the total number of kills.
- **`getTotalDamage() const`**: Returns the total damage dealt.
- **`getTotalDeaths() const`**: Returns the total number of deaths.
- **`getKD() const`**: Returns the player's kill/death ratio.
- **`getRankTier() const`**: Returns the player's rank tier.
- **`getInventory()`**: Returns a reference to the player's inventory.
- **`addMatchAndUpdate(const Match& m)`**: Adds a new match to the player's history and updates stats.
- **`addXp(long long amount)`**: Adds experience points to the player.
- **`recalcLevelAndTier()`**: Recalculates the player's level and rank tier.
- **`checkAndUnlockLevelRewards()`**: Checks for and unlocks level-based rewards.
- **`initDefaultAchievements()`**: Initializes the player's default achievements.
- **`evaluateAchievements(const Match& m)`**: Evaluates achievements based on a match's results.
- **`resetStats()`**: Resets the player's statistics.
- **`saveToDisk() const`**: Saves the player's data to disk.
- **`loadFromDisk()`**: Loads the player's data from disk.
- **`saveMatches() const`**: Saves the player's match history.
- **`loadMatches()`**: Loads the player's match history.
- **`publicSummary() const`**: Returns a public summary of the player's profile.