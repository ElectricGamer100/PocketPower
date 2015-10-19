#include "mcpe/world/level/tile/entity/TileEntity.h"

void (*_TileEntity$initTileEntities)();
void TileEntity$initTileEntities() {
	_TileEntity$initTileEntities();

	TileEntity::setId(TileEntityType::Piston, "Piston");
}

std::unique_ptr<TileEntity> (*_TileEntityFactory$createTileEntity)(TileEntityType, const TilePos&);
std::unique_ptr<TileEntity> TileEntityFactory$createTileEntity(TileEntityType type, const TilePos& pos) {	
	return _TileEntityFactory$createTileEntity(type, pos);
}
