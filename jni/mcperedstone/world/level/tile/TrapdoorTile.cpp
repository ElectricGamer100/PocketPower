#include "TrapdoorTile.h"
#include "mcpe/world/level/TileSource.h"

void (*TrapdoorTile::_$neighborChanged)(DoorTile*, TileSource*, int, int, int, int, int, int);

void TrapdoorTile::$neighborChanged(DoorTile* self, TileSource* region, int x, int y, int z, int xx, int yy, int zz) {
	_$neighborChanged(self, region, x, y, z, xx, yy, zz);

	self->setOpen(*region, x, y, z, region->isBlockIndirectlyGettingPowered(x, y, z));
}
