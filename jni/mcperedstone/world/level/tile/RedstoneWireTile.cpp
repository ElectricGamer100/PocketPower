#include "RedstoneWireTile.h"

#include "mcpe/world/level/TileSource.h"
#include "mcpe/world/Facing.h"


std::array<unsigned int, 16> RedstoneWireTile::colors = 
#include "RedstoneColors.h"

RedstoneWireTile::RedstoneWireTile(int blockId, const std::string& texture, const Material* material) : Tile(blockId, texture, material) {
	init();
	renderType = 5;
	renderPass = 3;
	setVisualShape(0.0F, 0.0F, 0.0F, 1.0F, 0.0625F, 1.0F);
	Tile::solid[blockId] = false;
	Tile::lightBlock[blockId] = 0;
	destroyTime = 0.0F;
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

void RedstoneWireTile::onRemove(TileSource* region, int x, int y, int z) {
	Tile::onRemove(region, x, y, z);
	region->updateNeighborsAt({x, y + 1, z}, id);
	region->updateNeighborsAt({x, y - 1, z}, id);
	recalculate(region, x, y, z);
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

bool RedstoneWireTile::canSurvive(TileSource* region, int x, int y, int z) {
	return Tile::solid[region->getTile(x, y - 1, z).id];
}

bool RedstoneWireTile::mayPlace(TileSource* region, int x, int y, int z) {
	if(region->getTile(x, y, z).id != 0) return false;
	// TODO: Implement placement exceptions for slabs, glowstone
	return Tile::solid[region->getTile(x, y - 1, z).id];
}

void RedstoneWireTile::neighborChanged(TileSource* region, int x, int y, int z, int newX, int newY, int newZ) {
      if(!canSurvive(region, x, y, z)) {
            //popResource(region, x, y, z, ItemInstance(331, 1, 0));
            //region->removeTile(x, y, z);
      }
	recalculate(region, x, y, z);
	Tile::neighborChanged(region, x, y, z, newX, newY, newZ);
}

void RedstoneWireTile::animateTick(TileSource* region, int x, int y, int z, Random* random) {

}

void RedstoneWireTile::tick(TileSource* region, int x, int y, int z, Random* random) {

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

void RedstoneWireTile::addCollisionShapes(TileSource& region, int x, int y, int z, AABB const* aabb, std::vector<AABB, std::allocator<AABB>>& pool) {}

void RedstoneWireTile::calculateChanges(TileSource* region, int x, int y, int z, int xx, int yy, int zz) {
	int oldPower = region->getData(x, y, z);
	int newPower = 0;
	wiresProvidePower = false;
	bool hasPower = region->isBlockIndirectlyGettingPowered(x, y, z);
	wiresProvidePower = true;
	if(hasPower)
		newPower = 15;
	else {
		for(int i2 = 0; i2 < 4; i2++) {
			int k2 = x;
			int i3 = z;
			if(i2 == 0)
				k2--;
			if(i2 == 1)
				k2++;
			if(i2 == 2)
				i3--;
			if(i2 == 3)
				i3++;
			if(k2 != xx || y != yy || i3 != zz)
				newPower = getStrongerSignal(region, k2, y, i3, newPower);
			if(Tile::solid[region->getTile(k2, y, i3).id] && !Tile::solid[region->getTile(x, y + 1, z).id]) {
				if(k2 != xx || y + 1 != yy || i3 != zz)
					newPower = getStrongerSignal(region, k2, y + 1, i3, newPower);
				continue;
			}
			if(!Tile::solid[region->getTile(k2, y, i3).id] && (k2 != xx || y - 1 != yy || i3 != zz))
				newPower = getStrongerSignal(region, k2, y - 1, i3, newPower);
		}

		if(newPower > 0)
			newPower--;
		else
			newPower = 0;
	}
	if(oldPower != newPower) {
		region->setTileAndData(x, y, z, {id, newPower}, 3);
		region->fireTilesDirty(x, y, z, x, y, z);
		for(int j2 = 0; j2 < 4; j2++) {
			int l2 = x;
			int j3 = z;
			int k3 = y - 1;
			if(j2 == 0)
			    l2--;
			if(j2 == 1)
			    l2++;
			if(j2 == 2)
			    j3--;
			if(j2 == 3)
			    j3++;
			if(Tile::solid[region->getTile(l2, y, j3).id])
			    k3 += 2;
			int l3 = 0;
			l3 = getStrongerSignal(region, l2, y, j3, -1);
			newPower = region->getData(x, y, z);
			if(newPower > 0)
			    newPower--;
			if(l3 >= 0 && l3 != newPower) {
			    calculateChanges(region, l2, y, j3, x, y, z);
			}
			l3 = getStrongerSignal(region, l2, k3, j3, -1);
			newPower = region->getData(x, y, z);
			if(newPower > 0)
			    newPower--;
			if(l3 >= 0 && l3 != newPower)
			{
			    calculateChanges(region, l2, k3, j3, x, y, z);
			}
		}

		if(oldPower == 0 || newPower == 0) {
			region->updateNeighborsAt({x, y, z}, id);
			region->updateNeighborsAt({x - 1, y, z}, id);
			region->updateNeighborsAt({x + 1, y, z}, id);
			region->updateNeighborsAt({x, y - 1, z}, id);
			region->updateNeighborsAt({x, y + 1, z + 1}, id);
			region->updateNeighborsAt({x, y, z - 1}, id);
			region->updateNeighborsAt({x, y, z + 1}, id);
			/*field_21031_b.add(new ChunkPosition(x, y, z));
			field_21031_b.add(new ChunkPosition(x - 1, y, z));
			field_21031_b.add(new ChunkPosition(x + 1, y, z));
			field_21031_b.add(new ChunkPosition(x, y - 1, z));
			field_21031_b.add(new ChunkPosition(x, y + 1, z));
			field_21031_b.add(new ChunkPosition(x, y, z - 1));
			field_21031_b.add(new ChunkPosition(x, y, z + 1));*/
		}
	}
}

void RedstoneWireTile::recalculate(TileSource* region, int x, int y, int z) {
      calculateChanges(region, x, y, z, x, y, z);
}

int RedstoneWireTile::getStrongerSignal(TileSource* region, int x, int y, int z, int signal2) {
	if(region->getTile(x, y, z).id != id) return signal2;
	int signal3 = region->getData(x, y, z);
	if(signal3 > signal2) return signal3;
	else return signal2;
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
	if(id == Tile::redStoneDust->id)
		return true;
	else if(id == 0)
		return false;
	else if(id != 93 && id != 94) // TODO
		return Tile::tiles[id]->isSignalSource();
	else {
		int data = region->getData(x, y, z);
		return side == (data & 3) || side == Facing::OPPOSITE_FACING[data & 3];
	}
}