#include "PistonMovingTile.h"
#include "mcpe/world/material/Material.h"

PistonMovingTile::PistonMovingTile(int blockId) : EntityTile(blockId, "stone", &Material::stone) {
	init();
	setNameId("pistonExtending");
	renderType = -1;
	tileEntityType = TileEntityType::Piston;
	creativeTab = CreativeTab::ITEMS;
}

TileEntity* PistonMovingTile::newTileEntity(const TilePos& position) {
	return new PistonTileEntity(pushedTile, pushedData, rotation, isExtending, renderHead, pos);
}

void PistonMovingTile::setTileEntityAttributes(Tile* pushedTile, int pushedData, int rotation, bool isExtending, bool renderHead, TilePos& pos) {
	this->pushedTile = pushedTile;
	this->pushedData = pushedData;
	this->rotation = rotation;
	this->isExtending = isExtending;
	this->renderHead = renderHead;
	this->pos = pos;
}