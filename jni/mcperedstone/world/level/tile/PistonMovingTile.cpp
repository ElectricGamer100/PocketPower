#include "PistonMovingTile.h"
#include "entity/PistonTileEntity.h"
#include "mcpe/world/material/Material.h"

Tile* PistonMovingTile::pushedTile;
int PistonMovingTile::pushedData;
int PistonMovingTile::rotation;
bool PistonMovingTile::isExtending;
bool PistonMovingTile::renderHead;
TilePos PistonMovingTile::pos;

PistonMovingTile::PistonMovingTile(int blockId) : EntityTile(blockId, "stone", &Material::stone) {
	init();
	setNameId("pistonExtending");
	renderType = -1;
	tileEntityType = TileEntityType::Piston;
	creativeTab = CreativeTab::ITEMS;
}

std::unique_ptr<TileEntity> PistonMovingTile::newTileEntity(const TilePos& position) {
	return std::unique_ptr<TileEntity>(new PistonTileEntity(pushedTile, pushedData, rotation, isExtending, renderHead, position));
}

void PistonMovingTile::setTileEntityAttributes(Tile* pushedTile, int pushedData, int rotation, bool isExtending, bool renderHead, const TilePos& pos) {
	PistonMovingTile::pushedTile = pushedTile;
	PistonMovingTile::pushedData = pushedData;
	PistonMovingTile::rotation = rotation;
	PistonMovingTile::isExtending = isExtending;
	PistonMovingTile::renderHead = renderHead;
	PistonMovingTile::pos = pos;
}
