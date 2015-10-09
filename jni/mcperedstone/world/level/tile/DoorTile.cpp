#include "DoorTile.h"
#include "mcpe/world/level/TileSource.h"

void (*DoorTile::_$neighborChanged)(DoorTile*, TileSource*, int, int, int, int, int, int);

void DoorTile::$neighborChanged(DoorTile* self, TileSource* region, int x, int y, int z, int xx, int yy, int zz) {
	_$neighborChanged(self, region, x, y, z, xx, yy, zz);
	
	bool powered = region->isBlockIndirectlyGettingPowered(x, y, z);
	if(region->getTilePtr(xx, yy, zz) != self) {
		self->setOpen(*region, x, y, z, powered, NULL);
	}
}
