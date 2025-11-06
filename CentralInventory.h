#pragma once
#include <string>
#include <vector>
#include "Common.h"

struct CatalogItem {
string name;
	int minLevel;
string category; 
};

class CentralInventory {
public:
	static void initializeDefaults();
	static bool loadFromFile(const string& path="data/catalog.txt");
	static bool saveToFile(const string& path="data/catalog.txt");
	static const vector<CatalogItem>& getCatalog();
    static void addItem(const string& name, int minLevel, const string& category);
    static vector<string> listCategories();
    static vector<CatalogItem> getByCategory(const string& category);
    static string getCategoryOf(const string& name);
private:
	static vector<CatalogItem> catalog;
};


