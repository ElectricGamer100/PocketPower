#include "LightPressurePlateTile.h"

#include "mcpe/world/level/TileSource.h"
#include "mcpe/world/phys/AABB.h"

LightPressurePlateTile::LightPressurePlateTile(int blockId, const std::string& texture, const Material* material, SENSITIVITY sensitivity) : PressurePlateTile(blockId, texture, material) {
	this->sensitivity = sensitivity;
}

int LightPressurePlateTile::getDataFromPower(int power) {
	return power > 0 ? 1 : 0;
}

int LightPressurePlateTile::getPowerFromData(int data) {
	return data == 1 ? 15 : 0;
}

int LightPressurePlateTile::getPower(TileSource* region, int x, int y, int z) {
	AABB aabb({x + 0.125F, y, z + 0.125F}, {(x + 1) - 0.125F, y + 0.25F, (z + 1) - 0.125F});
	EntityList list = region->getEntities(NULL, aabb);

	if(list.size() <= 0)
		return 0;
	if(sensitivity == SENSITIVITY::EVERYTHING || (sensitivity == SENSITIVITY::MOBS && _listIncludesMob(list)))
		return 15;
	
	return 0;
}

bool LightPressurePlateTile::_EntityisMob(const Entity* entity) {
	int id = entity->getEntityTypeId();
	return id != 64 && id != 80 && id != 81 && id != 82 && id != 66;
}

bool LightPressurePlateTile::_listIncludesMob(EntityList& list) {
	for(int i = 0; i < list.size(); i++) {
		if(_EntityisMob(list[i])) return true;
	}
	return false;
}
