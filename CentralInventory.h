#pragma once
#include <string>
#include <vector>
#include "Common.h"

struct CatalogItem {
	std::string name;
	int minLevel;
	std::string category; 
};

class CentralInventory {
public:
	static void initializeDefaults();
	static bool loadFromFile(const std::string& path="data/catalog.txt");
	static bool saveToFile(const std::string& path="data/catalog.txt");
	static const std::vector<CatalogItem>& getCatalog();
    static void addItem(const std::string& name, int minLevel, const std::string& category);
    static std::vector<std::string> listCategories();
    static std::vector<CatalogItem> getByCategory(const std::string& category);
    static std::string getCategoryOf(const std::string& name);
private:
	static std::vector<CatalogItem> catalog;
};


