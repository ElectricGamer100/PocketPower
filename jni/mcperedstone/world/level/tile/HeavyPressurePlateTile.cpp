#include "HeavyPressurePlateTile.h"
#include "mcpe/world/level/TileSource.h"
#include "mcpe/world/entity/Entity.h"
#include "mcpe/world/phys/AABB.h"
#include <algorithm>

HeavyPressurePlateTile::HeavyPressurePlateTile(int blockId, const std::string& name, const std::string& texture, const Material* material, int weight) : PressurePlateTile(blockId, name, texture, material) {
	this->weight = weight;
}

int HeavyPressurePlateTile::getDataFromPower(int power) {
	return power;
}

int HeavyPressurePlateTile::getPowerFromData(int data) {
	return data;
}

int HeavyPressurePlateTile::getTickDelay() {
	return 10;
}

int HeavyPressurePlateTile::getPower(TileSource* region, int x, int y, int z) {
	AABB aabb({x + 0.125F, y, z + 0.125F}, {(x + 1) - 0.125F, y + 0.25F, (z + 1) - 0.125F});
	int smaller = std::min((int) region->getEntities(NULL, aabb).size(), weight);

	if(smaller <= 0)
		return 0;
	else {
		float divided = (std::min(weight, smaller) / weight) * 15.0F;
		int icast = (int) divided;
		return (divided > (float) icast)? icast + 1 : icast;
	}
}
