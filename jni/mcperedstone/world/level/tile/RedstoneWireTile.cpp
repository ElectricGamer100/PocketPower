#include "RedstoneWireTile.h"
#include "mcpe/world/entity/player/Player.h"
#include "mcpe/world/level/TileSource.h"
#include "mcpe/world/level/Level.h"
#include "mcpe/world/item/Item.h"
#include "mcpe/world/Facing.h"
#include "mcpe/util/Mth.h"


std::array<unsigned int, 16> RedstoneWireTile::colors =
#include "RedstoneColors.h"

RedstoneWireTile::RedstoneWireTile(int blockId, const std::string& texture, const Material* material) : Tile(blockId, texture, material) {
	init();
	renderType = 5;
	renderPass = 3;
	setVisualShape(0.0F, 0.0F, 0.0F, 1.0F, 0.0625F, 1.0F);
	Tile::solid[blockId] = false;
	Tile::lightBlock[blockId] = 0;
	setDestroyTime(0.0F);
	setNameId("redstoneDust");
}

int RedstoneWireTile::getColor(TileSource* region, int x, int y, int z) {
	return colors[(int)region->getData(x, y, z)];
}

int RedstoneWireTile::getResource(Random* random, int aux, int i) {
	return 331;
}

void RedstoneWireTile::onPlace(TileSource* region, int x, int y, int z) {
	Tile::onPlace(region, x, y, z);
	recalculate(region, x, y, z);
	region->updateNeighborsAt({x, y + 1, z}, id);
	region->updateNeighborsAt({x, y - 1, z}, id);
	updateWires(region, x - 1, y, z);
	updateWires(region, x + 1, y, z);
	updateWires(region, x, y, z - 1);
	updateWires(region, x, y, z + 1);

	if(Tile::solid[region->getTile(x - 1, y, z).id])
		updateWires(region, x - 1, y + 1, z);
	else
		updateWires(region, x - 1, y - 1, z);

	if(Tile::solid[region->getTile(x + 1, y, z).id])
		updateWires(region, x + 1, y + 1, z);
	else
		updateWires(region, x + 1, y - 1, z);

	if(Tile::solid[region->getTile(x, y, z - 1).id])
		updateWires(region, x, y + 1, z - 1);
	else
		updateWires(region, x, y - 1, z - 1);

	if(Tile::solid[region->getTile(x, y, z + 1).id])
		updateWires(region, x, y + 1, z + 1);
	else
		updateWires(region, x, y - 1, z + 1);
}

void RedstoneWireTile::playerDestroy(Player* player, int x, int y, int z, int side) {
	player->region.updateNeighborsAt({x, y + 1, z}, id);
	player->region.updateNeighborsAt({x, y - 1, z}, id);
	player->region.updateNeighborsAt({x + 1, y, z}, id);
	player->region.updateNeighborsAt({x - 1, y, z}, id);
	player->region.updateNeighborsAt({x, y, z + 1}, id);
	player->region.updateNeighborsAt({x, y, z - 1}, id);
	recalculate(&player->region, x, y, z);
	updateWires(&player->region, x - 1, y, z);
	updateWires(&player->region, x + 1, y, z);
	updateWires(&player->region, x, y, z - 1);
	updateWires(&player->region, x, y, z + 1);

	if(Tile::solid[player->region.getTile(x - 1, y, z).id])
		updateWires(&player->region, x - 1, y + 1, z);
	else
		updateWires(&player->region, x - 1, y - 1, z);

	if(Tile::solid[player->region.getTile(x + 1, y, z).id])
		updateWires(&player->region, x + 1, y + 1, z);
	else
		updateWires(&player->region, x + 1, y - 1, z);

	if(Tile::solid[player->region.getTile(x, y, z - 1).id])
		updateWires(&player->region, x, y + 1, z - 1);
	else
		updateWires(&player->region, x, y - 1, z - 1);

	if(Tile::solid[player->region.getTile(x, y, z + 1).id])
		updateWires(&player->region, x, y + 1, z + 1);
	else
		updateWires(&player->region, x, y - 1, z + 1);
		
	Tile::playerDestroy(player, x, y, z, side);
}

bool RedstoneWireTile::canSurvive(TileSource* region, int x, int y, int z) {
	return Tile::solid[region->getTile(x, y - 1, z).id] || region->isRedstonePlacementException(x, y - 1, z);
}

bool RedstoneWireTile::mayPlace(TileSource* region, int x, int y, int z) {
	return region->getTile(x, y, z).id == 0 && (Tile::solid[region->getTile(x, y - 1, z).id] || region->isRedstonePlacementException(x, y - 1, z));
}

void RedstoneWireTile::neighborChanged(TileSource* region, int x, int y, int z, int newX, int newY, int newZ) {
	if(!canSurvive(region, x, y, z)) {
		popResource(region, x, y, z, ItemInstance(Item::items[getResource(NULL, 0, 0)]));
		region->removeTile(x, y, z);
		return;
	}
	recalculate(region, x, y, z);
	Tile::neighborChanged(region, x, y, z, newX, newY, newZ);
}

void RedstoneWireTile::animateTick(TileSource* region, int x, int y, int z, Random* random) {
	int data = region->getData(x, y, z);
	if(data == 0)
		return;
	float xoff = x + 0.5F + (Mth::random() - 0.5F) * 0.2F;
	float yoff = y + 0.0625F;
	float zoff = z + 0.5F + (Mth::random() - 0.5F) * 0.2F;
	
	float velHelper = data / 15.0F;
	float xVel = velHelper * 0.6F + 0.4F;
	float yVel = velHelper * velHelper * 0.7F - 0.5F;
	float zVel = velHelper * velHelper * 0.6F - 0.7F;
	if(yVel < 0.0F)
		yVel = 0.0F;
	if(zVel < 0.0F)
		zVel = 0.0F;

	region->getLevel()->addParticle(ParticleType::RedDust, {xoff, yoff, zoff}, {xVel, yVel, zVel}, 1);
}

bool RedstoneWireTile::isSignalSource() {
	return wiresProvidePower;
}

int RedstoneWireTile::getSignal(TileSource* region, int x, int y, int z, int side) {
	if(!wiresProvidePower) return 0;
	return getDirectSignal(region, x, y, z, side);
}

int RedstoneWireTile::getDirectSignal(TileSource* region, int x, int y, int z, int side) {
	if(!wiresProvidePower) return 0;
	int data = region->getData(x, y, z);
	if(data == 0) return 0;
	if(side == 1) return data;
	bool xm = canRedstoneConnectTo(region, x - 1, y, z, 1) || !Tile::solid[region->getTile(x - 1, y, z).id] && canRedstoneConnectTo(region, x - 1, y - 1, z, -1);
	bool xp = canRedstoneConnectTo(region, x + 1, y, z, 3) || !Tile::solid[region->getTile(x + 1, y, z).id] && canRedstoneConnectTo(region, x + 1, y - 1, z, -1);
	bool zm = canRedstoneConnectTo(region, x, y, z - 1, 2) || !Tile::solid[region->getTile(x, y, z - 1).id] && canRedstoneConnectTo(region, x, y - 1, z - 1, -1);
	bool zp = canRedstoneConnectTo(region, x, y, z + 1, 0) || !Tile::solid[region->getTile(x, y, z + 1).id] && canRedstoneConnectTo(region, x, y - 1, z + 1, -1);

	if(!Tile::solid[region->getTile(x, y + 1, z).id]) {
		if(Tile::solid[region->getTile(x - 1, y, z).id] && canRedstoneConnectTo(region, x - 1, y + 1, z, -1))
			xm = true;
		if(Tile::solid[region->getTile(x + 1, y, z).id] && canRedstoneConnectTo(region, x + 1, y + 1, z, -1))
			xp = true;
		if(Tile::solid[region->getTile(x, y, z - 1).id] && canRedstoneConnectTo(region, x, y + 1, z - 1, -1))
			zm = true;
		if(Tile::solid[region->getTile(x, y, z + 1).id] && canRedstoneConnectTo(region, x, y + 1, z + 1, -1))
			zp = true;
	}

	if(!zm && !xp && !xm && !zp && side >= 2 && side <= 5)
		return data;
	if(side == 2 && zm && !xm && !xp)
		return data;
	if(side == 3 && zp && !xm && !xp)
		return data;
	if(side == 4 && xm && !zm && !zp)
		return data;
	if(side == 5 && xp && !zm && !zp)
		return data;
	return 0;
}

bool RedstoneWireTile::addCollisionShapes(TileSource& region, int x, int y, int z, AABB const* aabb, std::vector<AABB, std::allocator<AABB>>& pool) {}

void RedstoneWireTile::calculateChanges(TileSource* region, int x, int y, int z, int xx, int yy, int zz) {
	int oldPower = region->getData(x, y, z);
	int newPower = getStrongerSignal(region, xx, yy, zz, 0);
	wiresProvidePower = false;
	int receivedPower = region->getStrongestIndirectPower(x, y, z);
	wiresProvidePower = true;
	
	if(receivedPower > 0 && receivedPower > newPower - 1)
		newPower = receivedPower;
		
	int temp = 0;
		
	for(int it = 0; it < 4; ++it) {
		int newX = x;
		int newZ = z;
		
		if(it == 0)
			newX = x - 1;
		if(it == 1)
			++newX;
		if(it == 2)
			newZ = z - 1;
		if(it == 3)
			++newZ;
			
		if(newX != xx || newZ != zz)
			temp = getStrongerSignal(region, newX, y, newZ, temp);
			
		if(solid[region->getTile(newX, y, newZ).id] && !solid[region->getTile(x, y + 1, z).id]) {
			if((newX != xx || newZ != zz) && y >= yy)
				temp = getStrongerSignal(region, newX, y + 1, newZ, temp);
		}
		else if(!solid[region->getTile(newX, y, newZ).id] && (newX != xx || newZ != zz) && y <= yy)
			temp = getStrongerSignal(region, newX, y - 1, newZ, temp);
	}
	
	if(temp > newPower)
		newPower = temp - 1;
	else if(newPower > 0)
		--newPower;
	else
		newPower = 0;
		
	if(receivedPower > newPower - 1)
		newPower = receivedPower;
		
	if(oldPower != newPower) {
		region->setTileAndData(x, y, z, {id, newPower}, 2);
		region->updateNeighborsAt({x, y, z}, id);
		region->updateNeighborsAt({x - 1, y, z}, id);
		region->updateNeighborsAt({x + 1, y, z}, id);
		region->updateNeighborsAt({x, y - 1, z}, id);
		region->updateNeighborsAt({x, y + 1, z}, id);
		region->updateNeighborsAt({x, y, z - 1}, id);
		region->updateNeighborsAt({x, y, z + 1}, id);
	}
}

void RedstoneWireTile::recalculate(TileSource* region, int x, int y, int z) {
	calculateChanges(region, x, y, z, x, y, z);
}

int RedstoneWireTile::getStrongerSignal(TileSource* region, int x, int y, int z, int signal) {
	if(region->getTile(x, y, z).id != id) return signal;
	int signal2 = region->getData(x, y, z);
	return (signal2 > signal)? signal2: signal;
}

void RedstoneWireTile::updateWires(TileSource* region, int x, int y, int z) {
	if(region->getTile(x, y, z).id != id) return;
	region->updateNeighborsAt({x, y, z}, id);
	region->updateNeighborsAt({x - 1, y, z}, id);
	region->updateNeighborsAt({x + 1, y, z}, id);
	region->updateNeighborsAt({x, y, z - 1}, id);
	region->updateNeighborsAt({x, y, z + 1}, id);
	region->updateNeighborsAt({x, y - 1, z}, id);
	region->updateNeighborsAt({x, y + 1, z}, id);
}

bool RedstoneWireTile::canRedstoneConnectTo(TileSource* region, int x, int y, int z, int side) const {
	int id = region->getTile(x, y, z).id;
	if(id == this->id)
		return true;
	else if(id == 0)
		return false;
	return Tile::tiles[id]->isSignalSource() && side != -1;
}
