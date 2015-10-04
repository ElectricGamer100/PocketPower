#include "RedstoneBlockTile.h"
#include "mcpe/world/material/Material.h"

RedstoneBlockTile::RedstoneBlockTile(int blockId) : Tile(blockId, "redstone_block", &Material::stone) {
	init();
	creativeTab = CreativeTab::ITEMS;
	setNameId("redstone_block");
}

bool RedstoneBlockTile::isSignalSource() {
	return true;
}

int RedstoneBlockTile::getDirectSignal(TileSource*, int, int, int, int) {
	return 15;
}
