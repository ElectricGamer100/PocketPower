#include "PressurePlateTile.h"

#include "mcpe/world/level/TileSource.h"
#include "mcpe/world/level/Level.h"

PressurePlateTile::PressurePlateTile(int id, const std::string& name, const std::string& texture, Material const* material) : Tile(id, texture, material) {
	init();
	setVisualShape(0.0625F, 0.0F, 0.0625F, 1.0F - 0.0625F, 0.0625F, 1.0F - 0.0625F);
	Tile::solid[id] = false;
	Tile::lightBlock[id] = 0;
	destroyTime = 0.5F;
	creativeTab = CreativeTab::ITEMS;
	setTicking(true);
	setNameId(name);
}

bool PressurePlateTile::entityInside(TileSource* region, int x, int y, int z, Entity* entity) {
	int power = getPowerFromData(region->getData(x, y, z));
	if(power == 0) setStateIfMobInteractsWithPlate(region, x, y, z, power);
}

bool PressurePlateTile::mayPlace(TileSource* region, int x, int y, int z) {
	int id = region->getTile(x, y - 1, z).id;
	return Tile::solid[id] || id == Tile::lightGem->id || id == Tile::fence->id;
}

void PressurePlateTile::neighborChanged(TileSource* region, int x, int y, int z, int newX, int newY, int newZ) {
	if(!mayPlace(region, x, y, z)) {
		//popResource(region, x, y, z, ItemInstance(id, 1, 0));
		region->removeTile(x, y, z);
		region->scheduleBlockUpdate(x, y, z, id, 0);
	}
}

void PressurePlateTile::tick(TileSource* region, int x, int y, int z, Random* random) {
	int power = getPowerFromData(region->getData(x, y, z));
	if(power > 0) setStateIfMobInteractsWithPlate(region, x, y, z, power);
}

int PressurePlateTile::getSignal(TileSource* region, int x, int y, int z, int side) {
	return (side == 1)? getPowerFromData(region->getData(x, y, z)) : 0;
}

int PressurePlateTile::getDirectSignal(TileSource* region, int x, int y, int z, int side) {
	return getPowerFromData(region->getData(x, y, z));
}

bool PressurePlateTile::isSignalSource() {
	return true;
}

void PressurePlateTile::onRemove(TileSource* region, int x, int y, int z) {
	if(getPowerFromData(region->getData(x, y, z)) > 0) {
		region->updateNeighborsAt({x, y, z}, id);
		region->updateNeighborsAt({x, y - 1, z}, id);
	}
	Tile::onRemove(region, x, y, z);
}

void PressurePlateTile::setStateIfMobInteractsWithPlate(TileSource* region, int x, int y, int z, int power) {
	int newPower = getPower(region, x, y, z);
	bool shouldBePowered = newPower > 0;
	bool isPowered = power > 0;

	if(power != newPower) {
		region->setTileAndData(x, y, z, {id, getDataFromPower(newPower)}, 2);
		region->updateNeighborsAt({x, y, z}, id);
		region->updateNeighborsAt({x, y - 1, z}, id);
		region->fireTilesDirty(x, y, z, x, y, z);
	}

	if(shouldBePowered && !isPowered)
		region->getLevel()->playSound(x + 0.5F, y + 0.1F, z + 0.5F, "random.click", 0.3F, 0.6F);
	if(!shouldBePowered && isPowered)
		region->getLevel()->playSound(x + 0.5F, y + 0.1F, z + 0.5F, "random.click", 0.3F, 0.5F);

	if(shouldBePowered)
		region->scheduleBlockUpdate(x, y, z, id, getTickDelay());
}

void PressurePlateTile::onPlace(TileSource* region, int x, int y, int z) {}

int PressurePlateTile::getTickDelay() {
	return 20;
}

AABB const& PressurePlateTile::getVisualShape(TileSource* region, int x, int y, int z, AABB& aabb, bool b) {
	aabb.set(0.0625F, 0.0F, 0.0625F, 1.0F - 0.0625F, 0.0625F, 1.0F - 0.0625F);
	if(getPowerFromData(region->getData(x, y, z)) == 0)
		aabb.set(0.0625F, 0.0F, 0.0625F, 1.0F - 0.0625F, 0.0625F, 1.0F - 0.0625F);
	else
		aabb.set(0.0625F, 0.0F, 0.0625F, 1.0F - 0.0625F, 0.03125F, 1.0F - 0.0625F);
	return aabb;
}

void PressurePlateTile::addCollisionShapes(TileSource& region, int x, int y, int z, AABB const* aabb, std::vector<AABB, std::allocator<AABB>>& aabbs) {}
