#include "CreativeTab.h"

#include "mcpe/world/item/Item.h"
#include "mcpe/world/level/tile/Tile.h"

void CreativeTabWorker::reorderCreativeItems() {
	for(int i = 256; i < 512; i++) {
		if(Item::items[i] == NULL) continue;
		if(Item::items[i]->creativeTab == CreativeTab::ITEMS)
			reorder(Item::items[i], CreativeTab::TOOLS);
	}
	_reorderPocketpowerItems();
}

void CreativeTabWorker::reorder(Item* item, CreativeTab _tab) {
	item->creativeTab = _tab;
}

void CreativeTabWorker::reorder(Tile* tile, CreativeTab _tab) {
	tile->creativeTab = _tab;
}

void CreativeTabWorker::_reorderPocketpowerItems() {
	reorder(Item::redStone, CreativeTab::ITEMS);
	reorder(Tile::tnt, CreativeTab::ITEMS);
	reorder(Tile::rail, CreativeTab::ITEMS);
	reorder(Tile::goldenRail, CreativeTab::ITEMS);
	reorder(Tile::redstoneBlock, CreativeTab::ITEMS);
	reorder(Item::repeater, CreativeTab::ITEMS);
}
