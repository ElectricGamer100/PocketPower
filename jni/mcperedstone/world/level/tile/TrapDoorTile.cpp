#include "TrapDoorTile.h"
#include "mcpe/world/level/TileSource.h"

void (*TrapDoorTile::_$neighborChanged)(TrapDoorTile*, TileSource*, int, int, int, int, int, int);

void TrapDoorTile::$neighborChanged(TrapDoorTile* self, TileSource* region, int x, int y, int z, int xx, int yy, int zz) {
	_$neighborChanged(self, region, x, y, z, xx, yy, zz);

	bool isPowered = region->isBlockIndirectlyGettingPowered(x, y, z);
	Tile* neighbor = region->getTilePtr(xx, yy, zz);
	bool neighborProvidesPower = false;
	
	if(neighbor) {
		neighborProvidesPower = neighbor->isSignalSource();
	}
	
	if(isPowered || neighborProvidesPower || !region->isBlockIndirectlyGettingPowered(xx, yy, zz))
		self->setOpen(region, x, y, z, isPowered);
}
