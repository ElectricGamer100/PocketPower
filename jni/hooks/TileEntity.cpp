#include "mcpe/world/level/tile/entity/TileEntity.h"

#include "mcperedstone/world/level/tile/entity/PistonTileEntity.h"

void (*_TileEntity$initTileEntities)();
void TileEntity$initTileEntities() {
	_TileEntity$initTileEntities();

	TileEntity::setId(TileEntityType::Piston, "Piston");
}

TileEntity* (*_TileEntityFactory$createTileEntity)(TileEntityFactory*, TileEntityType, const TilePos&);
TileEntity* TileEntityFactory$createTileEntity(TileEntityFactory* self, TileEntityType type, const TilePos& pos) {
	if(type == TileEntityType::Piston)
		return new PistonTileEntity(pos, 0, 0, 0, false, false);
		
	return _TileEntityFactory$createTileEntity(self, type, pos);
}
