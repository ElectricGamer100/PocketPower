#include "ButtonTile.h"

#include "mcpe/world/level/TileSource.h"
#include "mcpe/world/level/Level.h"
#include "mcpe/world/entity/player/Player.h"
#include "mcpe/world/phys/AABB.h"


ButtonTile::ButtonTile(int blockId, const std::string& texture, Material const* material) : Tile(blockId, texture, material) {
	init();
	setNameId("button");
	Tile::solid[id] = false;
	Tile::lightBlock[id] = 0;
	setTicking(true);
	destroyTime = 0.5;
	creativeTab = CreativeTab::ITEMS;
}

int ButtonTile::getTickDelay() {
	return (isWood())? 30 : 20;
}

void ButtonTile::onPlace(TileSource* region, int x, int y, int z) {
	region->scheduleBlockUpdate(x, y, z, id, 0);
}

int ButtonTile::getPlacementDataValue(Mob* placer, int x, int y, int z, signed char side, float xx, float yy, float zz, int ii) {
	switch(side) {
	case 0:
		return 6;
	case 1:
		return 5;
	case 2:
		return 4;
	case 3:
		return 3;
	case 4:
		return 2;
	case 5:
		return 1;
	default:
		return 4;
	}
}

bool ButtonTile::entityInside(TileSource* region, int x, int y, int z, Entity* entity) {
	//if(this->id == 143 && (ts->getData(x, y, z) & 8) == 0) toggleIfArrowInside(ts, x, y, z);
	return false;
}

bool ButtonTile::listIncludesArrow(EntityList list) {
	for(int i = 0; i < list.size(); i++) {
		//__android_log_print(ANDROID_LOG_INFO, "REDSTONE", "Arrow: %d", list[i]->isInstanceOf(80));
		//if(list[i]->getEntityTypeId() == 80) return true;
	}
	return false;
}

void ButtonTile::toggleIfArrowInside(TileSource* ts, int x, int y, int z) {
	/*EntityList list = ts->getEntities(NULL, AABB({x, y, z}, {x + 1, y + 1, z + 1}));
	this->listIncludesArrow(list);
	bool flag = false; //(ts->getData(x, y, z) & 8) == 1;
	bool flag1 = false;
	EntityList list = ts->getEntities(NULL, AABB({x, y, z}, {x + 1, y + 1, z + 1}));
	if(ts->getTile(x, y, z).id == 143 && list.size() > 0 && !this->listIncludesArrow(list)) flag1 = true;
	if(flag1 && !flag) {
	    ts->setTileAndData(x, y + 1, z, 3, 1, 3);
	    ts->getLevel()->playSound(x + 0.5F, y + 0.5F, z + 0.5F, "random.click", 0.3F, 0.6F);
	}
	*/
}

const AABB& ButtonTile::getVisualShape(unsigned char data, AABB& aabb, bool b) {
	float f = 0.1875F;
	float f1 = 0.125F;
	float f2 = 0.125F;
	aabb.set(0.5F - f, 0.5F - f1, 0.5F - f2, 0.5F + f, 0.5F + f1, 0.5F + f2);
	return aabb;
}

const AABB& ButtonTile::getVisualShape(TileSource* region, int x, int y, int z, AABB& aabb, bool b) {
	int data = region->getData(x, y, z);
	int rot = data & 7;
	bool powered = (data & 8) > 0;
	float f = 0.375F;
	float f1 = 0.625F;
	float f2 = 0.1875F;
	float f3 = 0.125F;
	if(powered) f3 = 0.0625F;
	if(rot == 1)
		aabb.set(0.0F, f, 0.5F - f2, f3, f1, 0.5F + f2);
	else if(rot == 2)
		aabb.set(1.0F - f3, f, 0.5F - f2, 1.0F, f1, 0.5F + f2);
	else if(rot == 3)
		aabb.set(0.5F - f2, f, 0.0F, 0.5F + f2, f1, f3);
	else if(rot == 4)
		aabb.set(0.5F - f2, f, 1.0F - f3, 0.5F + f2, f1, 1.0F);
	else if(rot == 5)
		aabb.set(f, 0.0F, 0.5F - f2, f1, f3, 0.5F + f2);
	else if(rot == 6)
		aabb.set(f, 1.0F - f3, 0.5F - f2, f1, 1.0F, 0.5F + f2);

	return aabb;
}

void ButtonTile::neighborChanged(TileSource* region, int x, int y, int z, int newX, int newY, int newZ) {
	if(!canSurvive(region, x, y, z)) {
		region->removeTile(x, y, z);
		region->scheduleBlockUpdate(x, y, z, id, 0);
	}
}

bool ButtonTile::use(Player* player, int x, int y, int z) {
	int data = player->region.getData(x, y, z);
	int rot = data & 7;
	int power = 8 - (data & 8);
	if(power == 0) return true;
	player->region.setTileAndData(x, y, z, {id, rot + power}, 3);
	player->region.fireTilesDirty(x, y, z, x, y, z);
	player->region.getLevel()->playSound(x + 0.5F, y + 0.5F, z + 0.5F, "random.click", 0.3F, 0.6F);
	player->region.updateNeighborsAt({x, y, z}, id);
	if(rot == 1)
		player->region.updateNeighborsAt({x - 1, y, z}, id);
	else if(rot == 2)
		player->region.updateNeighborsAt({x + 1, y, z}, id);
	else if(rot == 3)
		player->region.updateNeighborsAt({x, y, z - 1}, id);
	else if(rot == 4)
		player->region.updateNeighborsAt({x, y, z + 1}, id);
	else if(rot == 5)
		player->region.updateNeighborsAt({x, y - 1, z}, id);
	else if(rot == 6)
		player->region.updateNeighborsAt({x, y + 1, z}, id);
	player->region.scheduleBlockUpdate(x, y, z, id, getTickDelay());
	return true;
}

void ButtonTile::onRemove(TileSource* region, int x, int y, int z) {
	int data = region->getData(x, y, z);
	if((data & 8) > 0) {
		region->updateNeighborsAt({x, y, z}, id);

		switch(data & 7) {
		case 6:
			region->updateNeighborsAt({x, y + 1, z}, id);
			break;
		case 5:
			region->updateNeighborsAt({x, y - 1, z}, id);
			break;
		case 4:
			region->updateNeighborsAt({x, y, z + 1}, id);
			break;
		case 3:
			region->updateNeighborsAt({x, y, z - 1}, id);
			break;
		case 2:
			region->updateNeighborsAt({x + 1, y, z}, id);
			break;
		case 1:
			region->updateNeighborsAt({x - 1, y, z}, id);
			break;
		}
	}
	Tile::onRemove(region, x, y, z);
}

int ButtonTile::getDirectSignal(TileSource* region, int x, int y, int z, int side) {
	return ((region->getData(x, y, z) & 8) > 0)? 15: 0;
}

int ButtonTile::getSignal(TileSource* region, int x, int y, int z, int side) {
	int data = region->getData(x, y, z);
	if((data & 8) == 0) return 0;
	int rot = data & 7;
	if(rot == 6 && side == 0)
		return 15;
	if(rot == 5 && side == 1)
		return 15;
	if(rot == 4 && side == 2)
		return 15;
	if(rot == 3 && side == 3)
		return 15;
	if(rot == 2 && side == 4)
		return 15;
	if(rot == 1 && side == 5)
		return 15;
	return 0;
}

bool ButtonTile::isSignalSource() {
	return true;
}

bool ButtonTile::mayPlace(TileSource* region, int x, int y, int z, signed char side) {
	switch(side) {
	case 2:
		return Tile::solid[region->getTile(x, y, z + 1).id];
	case 3:
		return Tile::solid[region->getTile(x, y, z - 1).id];
	case 4:
		return Tile::solid[region->getTile(x + 1, y, z).id];
	case 5:
		return Tile::solid[region->getTile(x - 1, y, z).id];
	case 1:
		return Tile::solid[region->getTile(x, y - 1, z).id];
	case 0:
		return Tile::solid[region->getTile(x, y + 1, z).id];
	}
}

bool ButtonTile::canSurvive(TileSource* region, int x, int y, int z) {
	int rot = region->getData(x, y, z) & 7;
	switch(rot) {
	case 6:
		return Tile::solid[region->getTile(x, y + 1, z).id];
	case 5:
		return Tile::solid[region->getTile(x, y - 1, z).id];
	case 4:
		return Tile::solid[region->getTile(x, y, z + 1).id];
	case 3:
		return Tile::solid[region->getTile(x, y, z - 1).id];
	case 2:
		return Tile::solid[region->getTile(x + 1, y, z).id];
	case 1:
		return Tile::solid[region->getTile(x - 1, y, z).id];
	}
	return false;
}

void ButtonTile::tick(TileSource* region, int x, int y, int z, Random* random) {
	int data = region->getData(x, y, z);

	if((data & 8) == 0) {
		if(isWood()) toggleIfArrowInside(region, x, y, z);
		return;
	}

	region->setTileAndData(x, y, z, {id, data & 7}, 3);
	region->updateNeighborsAt({x, y, z}, id);
	int rot = data & 7;
	if(rot == 1)
		region->updateNeighborsAt({x - 1, y, z}, id);
	else if(rot == 2)
		region->updateNeighborsAt({x + 1, y, z}, id);
	else if(rot == 3)
		region->updateNeighborsAt({x, y, z - 1}, id);
	else if(rot == 4)
		region->updateNeighborsAt({x, y, z + 1}, id);
	else if(rot == 5)
		region->updateNeighborsAt({x, y - 1, z}, id);
	else if(rot == 6)
		region->updateNeighborsAt({x, y + 1, z}, id);
	region->getLevel()->playSound(x + 0.5F, y + 0.5F, z + 0.5F, "random.click", 0.3F, 0.5F);
	region->fireTilesDirty(x, y, z, x, y, z);
}

bool ButtonTile::isWood() {
	return id == 143;
}

void ButtonTile::addCollisionShapes(TileSource& region, int x, int y, int z, AABB const* aabb, std::vector<AABB, std::allocator<AABB>>& list) {}
