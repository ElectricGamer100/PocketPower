#include "mcpe/world/level/tile/entity/TileEntity.h"

#include "mcperedstone/world/level/tile/entity/PistonTileEntity.h"

void (*_TileEntity$initTileEntities)();
void TileEntity$initTileEntities() {
	_TileEntity$initTileEntities();

	TileEntity::setId(TileEntityType::Piston, "Piston");
}

std::unique_ptr<TileEntity> (*_TileEntityFactory$createTileEntity)(TileEntityType, const TilePos&);
std::unique_ptr<TileEntity> TileEntityFactory$createTileEntity(TileEntityType type, const TilePos& pos) {
	if(type == TileEntityType::Piston)
		return std::unique_ptr<TileEntity>(new PistonTileEntity(pos, 0, 0, 0, false, false));
		
	return _TileEntityFactory$createTileEntity(type, pos);
}
