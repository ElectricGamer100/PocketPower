#include "mcpe/world/level/tile/entity/TileEntity.h"
#include "mcpe/world/level/tile/EntityTile.h"
#include "mcperedstone/world/level/tile/PistonMovingTile.h"


void (*_TileEntity$initTileEntities)();
void TileEntity$initTileEntities() {
	_TileEntity$initTileEntities();

	TileEntity::setId(TileEntityType::Piston, "Piston");
}

std::unique_ptr<TileEntity> (*_EntityTile$newTileEntity)(EntityTile*, const TilePos&);
std::unique_ptr<TileEntity> EntityTile$newTileEntity(EntityTile* self, const TilePos& pos) {
	if(self->id == 36)
		return ((PistonMovingTile*) self)->newTileEntity(pos);
		
	return _EntityTile$newTileEntity(self, pos);
}

std::unique_ptr<TileEntity> (*_TileEntityFactory$createTileEntity)(TileEntityType, const TilePos&);
std::unique_ptr<TileEntity> TileEntityFactory$createTileEntity(TileEntityType type, const TilePos& pos) {	
	return _TileEntityFactory$createTileEntity(type, pos);
}
