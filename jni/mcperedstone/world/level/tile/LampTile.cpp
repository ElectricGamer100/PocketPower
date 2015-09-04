#include "LampTile.h"

#include "mcpe/world/level/TileSource.h"

LampTile::LampTile(int blockId, const std::string& texture, Material const* material) : Tile(blockId, texture, material) {
     init();
	if(isLit()) setLightEmission(1.0F);
	creativeTab = CreativeTab::ITEMS;
	setNameId("redstoneLight");
     destroyTime = 0.3F;
     soundType = &Tile::SOUND_GLASS;
}

void LampTile::onPlace(TileSource* region, int x, int y, int z) {
	if(isLit() && !region->isBlockIndirectlyGettingPowered(x, y, z))
           region->scheduleBlockUpdate(x, y, z, id, 4);
	else if(!isLit() && region->isBlockIndirectlyGettingPowered(x, y, z))
           region->setTileAndData(x, y, z, {Tile::onlamp->id, 0}, 3);
}

void LampTile::neighborChanged(TileSource* region, int x, int y, int z, int newX, int newY, int newZ) {
	if(isLit() && !region->isBlockIndirectlyGettingPowered(x, y, z))
           region->scheduleBlockUpdate(x, y, z, id, 4);
	else if(!isLit() && region->isBlockIndirectlyGettingPowered(x, y, z))
           region->setTileAndData(x, y, z, {Tile::onlamp->id, 0}, 3);
}

void LampTile::tick(TileSource* region, int x, int y, int z, Random* random) {
	if(isLit() && !region->isBlockIndirectlyGettingPowered(x, y, z))
           region->setTileAndData(x, y, z, {Tile::offlamp->id, 0}, 3);
}

bool LampTile::isLit() {
    return id == 124;
}

int LampTile::getResource(int id, Random* random) {
	return Tile::offlamp->id;
}