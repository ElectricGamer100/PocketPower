#include "PocketPowerCraftingManager.h"
#include "mcpe/world/item/ItemInstance.h"
#include "mcpe/world/item/Item.h"
#include "mcpe/world/level/tile/Tile.h"
#include "mcpe/world/item/CompoundTag.h"

Recipes* PocketPowerCraftingManager::_Instance;
std::vector<Recipes::Type> PocketPowerCraftingManager::_ingredients;

void PocketPowerCraftingManager::initRecipes() {
	_Instance = Recipes::instance;
	
	pushIngredient(3, 0, '#');
	ItemInstance item(42, 1, 1);
	item.userData = new CompoundTag();
	//addRecipe(item, "##", "##");
}

void PocketPowerCraftingManager::pushIngredient(int id, int data, char letter) {
	Recipes::Type type {Item::items[id], NULL, ItemInstance(), letter};
	_ingredients.push_back(type);
}

void PocketPowerCraftingManager::addRecipe(const ItemInstance& item, std::string str1) {
	std::vector<std::string> vec;
	vec.push_back(str1);
	addRecipe(item, vec);
}

void PocketPowerCraftingManager::addRecipe(const ItemInstance& item, std::string str1, std::string str2) {
	std::vector<std::string> vec;
	vec.push_back(str1);
	vec.push_back(str2);
	addRecipe(item, vec);
}

void PocketPowerCraftingManager::addRecipe(const ItemInstance& item, std::string str1, std::string str2, std::string str3) {
	std::vector<std::string> vec;
	vec.push_back(str1);
	vec.push_back(str2);
	vec.push_back(str3);
	addRecipe(item, vec);
}

void PocketPowerCraftingManager::addRecipe(const ItemInstance& item, const std::vector<std::string>& vec) {
	ItemInstance item2;
	//item2.userData = new CompoundTag();
	Recipes::Type type {Item::items[280], NULL, item2};
	type.letter[0] = '#';
	_Instance->addShapedRecipe(item, {"##", "##"}, {type});
	_ingredients.clear();
}
