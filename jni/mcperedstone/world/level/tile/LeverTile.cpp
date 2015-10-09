#include "LeverTile.h"

#include "mcpe/world/entity/player/Player.h"
#include "mcpe/world/level/TileSource.h"
#include "mcpe/world/level/Level.h"


LeverTile::LeverTile(int blockId, const std::string& texture, Material const* material) : Tile(blockId, texture, material) {
	init();
	setNameId("lever");
	Tile::solid[id] = false;
	Tile::lightBlock[id] = 0;
	creativeTab = CreativeTab::ITEMS;
	renderType = 12;
	destroyTime = 0.5;
	renderPass = 3;
}

int LeverTile::getPlacementDataValue(Mob* placer, int x, int y, int z, signed char side, float xx, float yy, float zz, int ii) {
	int power = placer->region.getData(x, y, z) & 8;
	int sides[6] = {7, 5, 4, 3, 2, 1};
	return sides[side] + power;
}

bool LeverTile::use(Player* player, int x, int y, int z) {
	int data = player->region.getData(x, y, z);
	int rot = data & 7;
	int power = 8 - (data & 8);
	player->region.setTileAndData(x, y, z, {69, rot + power}, 3);
	player->region.fireTilesDirty(x, y, z, x, y, z);
	player->region.getLevel()->playSound(x + 0.5F, y + 0.5F, z + 0.5F, "random.click", 0.3F, (power == 0)? 0.5F : 0.6F);
	player->region.updateNeighborsAt({x, y, z}, 69);
	if(rot == 1) player->region.updateNeighborsAt({x - 1, y, z}, 69);
	else if(rot == 2) player->region.updateNeighborsAt({x + 1, y, z}, 69);
	else if(rot == 3) player->region.updateNeighborsAt({x, y, z - 1}, 69);
	else if(rot == 4) player->region.updateNeighborsAt({x, y, z + 1}, 69);
	else if(rot == 5) player->region.updateNeighborsAt({x, y - 1, z}, 69);
	else if(rot == 7) player->region.updateNeighborsAt({x, y + 1, z}, 69);
}

int LeverTile::getDirectSignal(TileSource* region, int x, int y, int z, int side) {
	return ((region->getData(x, y, z) & 8) > 0)? 15: 0;
}

int LeverTile::getSignal(TileSource* region, int x, int y, int z, int side) {
	int data = region->getData(x, y, z);
	if((data & 8) == 0) return 0;

	int rot = data & 7;
	if(rot == 0 && side == 0) return 15;
	if(rot == 7 && side == 0) return 15;
	if(rot == 6 && side == 1) return 15;
	if(rot == 5 && side == 1) return 15;
	if(rot == 4 && side == 2) return 15;
	if(rot == 3 && side == 3) return 15;
	if(rot == 2 && side == 4) return 15;
	if(rot == 1 && side == 5) return 15;
	return 0;
}

const AABB& LeverTile::getVisualShape(TileSource* region, int x, int y, int z, AABB& aabb, bool b) {
	float f = 0.25F;
	float f1 = 0.1875F;
	float f2 = 0.1875F;
	switch(region->getData(x, y, z) & 7) {
	case 1:
		aabb.set(0.0F, 0.5F - f, 0.5F - f1, f2, 0.5F + f, 0.5F + f1);
		break;
	case 2:
		aabb.set(1.0F - f2, 0.5F - f, 0.5F - f1, 1.0F, 0.5F + f, 0.5F + f1);
		break;
	case 3:
		aabb.set(0.5F - f1, 0.5F - f, 0.0F, 0.5F + f1, 0.5F + f, f2);
		break;
	case 4:
		aabb.set(0.5F - f1, 0.5F - f, 1.0F - f2, 0.5F + f1, 0.5F + f, 1.0F);
		break;
	case 5:
		aabb.set(0.5F - f1, 0.0F, 0.5F - f, 0.5F + f1, f2, 0.5F + f);
		break;
	case 7:
		aabb.set(0.5F - f1, 1.0F - f2, 0.5F - f, 0.5F + f1, 1.0F, 0.5F + f);
		break;
	}
	return aabb;
}

bool LeverTile::mayPlace(TileSource* region, int x, int y, int z, signed char side) {
	switch(side) {
	case 0:
		return Tile::solid[region->getTile(x, y + 1, z).id];
	case 1:
		return Tile::solid[region->getTile(x, y - 1, z).id];
	case 2:
		return Tile::solid[region->getTile(x, y, z + 1).id];
	case 3:
		return Tile::solid[region->getTile(x, y, z - 1).id];
	case 4:
		return Tile::solid[region->getTile(x + 1, y, z).id];
	case 5:
		return Tile::solid[region->getTile(x - 1, y, z).id];
	}
}

bool LeverTile::canSurvive(TileSource* region, int x, int y, int z) {
	switch(region->getData(x, y, z) & 7) {
	case 7:
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

void LeverTile::neighborChanged(TileSource* region, int x, int y, int z, int newX, int newY, int newZ) {
	if(!canSurvive(region, x, y, z)) {
		popResource(region, x, y, z, ItemInstance(id, 1, 0));
		region->removeTile(x, y, z);
		region->scheduleBlockUpdate(x, y, z, id, 0);
	}
}

void LeverTile::tick(TileSource* region, int x, int y, int z, Random* random) {
}

void LeverTile::onPlace(TileSource* region, int x, int y, int z) {
	region->scheduleBlockUpdate(x, y, z, id, 0);
}

bool LeverTile::isSignalSource() {
	return true;
}

void LeverTile::addCollisionShapes(TileSource& region, int x, int y, int z, AABB const* aabb, std::vector<AABB, std::allocator<AABB>>& list) {}
