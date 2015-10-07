#pragma once
#include "mcpe/client/Recipes.h"
#include <string>
#include <vector>

class PocketPowerCraftingManager {
public:
	static void initRecipes();

private:
	static Recipes* _Instance;
	static std::vector<Recipes::Type> _ingredients;
	
	static void pushIngredient(int, int, char);
	static void addRecipe(const ItemInstance&, std::string);
	static void addRecipe(const ItemInstance&, std::string, std::string);
	static void addRecipe(const ItemInstance&, std::string, std::string, std::string);
	static void addRecipe(const ItemInstance&, const std::vector<std::string>&);
};
