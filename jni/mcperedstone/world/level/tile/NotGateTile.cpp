#include "NotGateTile.h"

#include "mcpe/world/level/TileSource.h"

NotGateTile::NotGateTile(int blockId, const std::string& texture) : TorchTile(blockId, texture) {
	init();
	destroyTime = 0.0F;
	creativeTab = CreativeTab::ITEMS;

	if(isActive()) {
		renderType = 100;
		renderPass = 7;
		setLightEmission(0.5F);
	}

	setNameId("notGate");
}

void NotGateTile::animateTick(TileSource* region, int x, int y, int z, Random* random) {
}

bool NotGateTile::mayPlace(TileSource* region, int x, int y, int z, signed char side) {
	switch(side) {
	case 0:
		return false;
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

bool NotGateTile::canSurvive(TileSource* region, int x, int y, int z) {
	int data = region->getData(x, y, z);
	switch(data) {
	case 5:
		return Tile::solid[region->getTile(x, y - 1, z).id];
	case 3:
		return Tile::solid[region->getTile(x, y, z - 1).id];
	case 4:
		return Tile::solid[region->getTile(x, y, z + 1).id];
	case 1:
		return Tile::solid[region->getTile(x - 1, y, z).id];
	case 2:
		return Tile::solid[region->getTile(x + 1, y, z).id];
	}
	return true;
}

void NotGateTile::onPlace(TileSource* region, int x, int y, int z) {
	if(region->getData(x, y, z) == 0) TorchTile::onPlace(region, x, y, z);
	if(isActive()) {
		region->updateNeighborsAt({x, y - 1, z}, 76);
		region->updateNeighborsAt({x, y + 1, z}, 76);
		region->updateNeighborsAt({x - 1, y, z}, 76);
		region->updateNeighborsAt({x + 1, y, z}, 76);
		region->updateNeighborsAt({x, y, z - 1}, 76);
		region->updateNeighborsAt({x, y, z + 1}, 76);
	}
}

void NotGateTile::onRemove(TileSource* region, int x, int y, int z) {
	if(isActive()) {
		region->scheduleBlockUpdate(x, y, z, id, 0);
	}
}

bool NotGateTile::isSignalSource() {
	return isActive();
}

int NotGateTile::getTickDelay() {
	return 2;
}

int NotGateTile::getDirectSignal(TileSource* region, int x, int y, int z, int side) {
	if(!isActive()) return 0;
	int data = region->getData(x, y, z);
	if(data == 5 && side == 1) return 0;
	if(data == 3 && side == 3) return 0;
	if(data == 4 && side == 2) return 0;
	if(data == 1 && side == 5) return 0;
	if(data == 2 && side == 4) return 0;
	return 15;
}

int NotGateTile::getSignal(TileSource* region, int x, int y, int z, int side) {
	if(side == 0) return getDirectSignal(region, x, y, z, side);
	return 0;
}

void NotGateTile::neighborChanged(TileSource* region, int x, int y, int z, int newX, int newY, int newZ) {
	if(!canSurvive(region, x, y, z)) {
		//popResource(region, x, y, z, ItemInstance(76, 1, 0));
		region->removeTile(x, y, z);
	}
	region->scheduleBlockUpdate(x, y, z, id, getTickDelay());
}

int NotGateTile::getResource(Random* random, int, int) {
	return 76;
}

void NotGateTile::tick(TileSource* region, int x, int y, int z, Random* random) {
	bool burnt = checkForBurnout(region, x, y, z);
	if(burnt && region->getTile(x, y, z).id == Tile::notGate_on->id) {
		region->setTileAndData(x, y, z, {Tile::notGate_off->id, region->getData(x, y, z)}, 3);
		region->updateNeighborsAt({x, y + 1, z}, Tile::notGate_off->id);
	} else if(!burnt && region->getTile(x, y, z).id == Tile::notGate_off->id)
		region->setTileAndData(x, y, z, {Tile::notGate_on->id, region->getData(x, y, z)}, 3);
}

bool NotGateTile::isActive() {
	return id == 76;
}

bool NotGateTile::checkForBurnout(TileSource* region, int x, int y, int z) {
	int data = region->getData(x, y, z);
	if(data == 5 && region->getIndirectPowerOutput(x, y - 1, z, 0)) return true;
	if(data == 3 && region->getIndirectPowerOutput(x, y, z - 1, 2)) return true;
	if(data == 4 && region->getIndirectPowerOutput(x, y, z + 1, 3)) return true;
	if(data == 1 && region->getIndirectPowerOutput(x - 1, y, z, 4)) return true;
	return data == 2 && region->getIndirectPowerOutput(x + 1, y, z, 5);
}