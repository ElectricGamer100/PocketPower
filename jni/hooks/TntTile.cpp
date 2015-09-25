#include "mcpe/world/level/tile/TntTile.h"

#include "mcpe/world/level/TileSource.h"

void TntTile::onPlace(TileSource* region, int x, int y, int z) {
	neighborChanged(region, x, y, z, x, y, z);
}

void TntTile::_neighborChanged(TileSource* region, int x, int y, int z, int newX, int newY, int newZ) {
	if(region->isBlockIndirectlyGettingPowered(x, y, z)) {
		destroy(region, x, y, z, 1, NULL);
		region->removeTile(x, y, z);
	}
}
