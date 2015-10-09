#include "RepeaterItem.h"
#include "mcpe/world/item/ItemInstance.h"
#include "mcpe/world/entity/player/Player.h"
#include "mcpe/world/level/TileSource.h"
#include "mcpe/world/level/tile/Tile.h"
#include "mcpe/world/Facing.h"

RepeaterItem::RepeaterItem(int id) : Item(id) {
	setIcon("repeater", 0);
	setNameID("diode");
	creativeTab = CreativeTab::ITEMS;
}

bool RepeaterItem::useOn(ItemInstance* item, Player* player, int x, int y, int z, signed char side, float xx, float yy, float zz) {
	int movedX = x + Facing::STEP_X[side], movedY = y + Facing::STEP_Y[side], movedZ = z + Facing::STEP_Z[side];
	
	if(Tile::tiles[93]->canSurvive(&player->region, movedX, movedY, movedZ)) {
		int data = Tile::diode_off->getPlacementDataValue(player, x, y, z, side, xx, yy, zz, 0);
		player->region.setTileAndData(movedX, movedY, movedZ, {Tile::diode_off->id, data}, 3);
		return true;
	}
	
	return false;
}
