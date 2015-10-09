#include "PocketPowerCraftingManager.h"
#include "mcpe/world/item/ItemInstance.h"
#include "mcpe/world/item/Item.h"
#include "mcpe/client/Recipes.h"

Recipes* PocketPowerCraftingManager::_Instance;
std::vector<Recipes::Type> PocketPowerCraftingManager::_ingredients;

void PocketPowerCraftingManager::initRecipes(Recipes* instance) {
	_Instance = instance;
	
		pushIngredient(331, 0, 'R');
		pushIngredient(280, 0, 'S');
	addRecipe(76, 1, {"R ", "S "});
		pushIngredient(5, 0, 'W');
	addRecipe(72, 1, {"  ", "WW"});
		pushIngredient(1, 0, 'S');
	addRecipe(70, 1, {"  ", "SS"});
		pushIngredient(265, 0, 'I');
	addRecipe(148, 1, {"  ", "II"});
		pushIngredient(266, 0, 'G');
	addRecipe(147, 1, {"  ", "GG"});
		pushIngredient(5, 0, 'W');
	addRecipe(143, 1, {"W ", "  "});
		pushIngredient(1, 0, 'S');
	addRecipe(77, 1, {"S ", "  "});
		pushIngredient(280, 0, 'S');
		pushIngredient(4, 0, 'C');
	addRecipe(69, 1, {"S ", "C "});
		pushIngredient(1, 0, 'S');
		pushIngredient(331, 0, 'R');
		pushIngredient(76, 0, 'T');
	addRecipe(356, 1, {"   ", "TRT", "SSS"});
		pushIngredient(331, 0, 'R');
		pushIngredient(89, 0, 'G');
	addRecipe(123, 1, {" R ", "RGR", " R "});
		pushIngredient(331, 0, 'R');
		pushIngredient(5, 0, 'W');
		pushIngredient(4, 0, 'C');
		pushIngredient(265, 0, 'I');
	addRecipe(33, 1, {"WWW", "CIC", "CRC"});
		pushIngredient(33, 0, 'P');
		pushIngredient(341, 0, 'S');
	addRecipe(29, 1, {"S ", "P "});
		pushIngredient(265, 0, 'I');
	addRecipe(330, 1, {"II ", "II ", "II "});
}

void PocketPowerCraftingManager::pushIngredient(int id, int data, char letter) {
	Recipes::Type type {Item::items[id], NULL, ItemInstance(), letter};
	_ingredients.push_back(type);
}

void PocketPowerCraftingManager::addRecipe(int outputID, int outputCount, const std::vector<std::string>& shape) {
	ItemInstance output(Item::items[outputID]);
	output.count = outputCount;
	_Instance->addShapedRecipe(output, shape, _ingredients);
	_ingredients.clear();
}
