#include <jni.h>
#include <dlfcn.h>
#include <android/log.h>
#include <stdlib.h>
#include <substrate.h>

#include "virtualhook.h"

#include "mcpe/client/renderer/tile/TileTessellator.h"
#include "mcpe/client/gui/screens/CreativeInventoryScreen.h"
#include "mcpe/world/level/TilePos.h"
#include "mcpe/world/level/TileSource.h"
#include "mcpe/world/level/tile/TntTile.h"
#include "mcpe/world/level/tile/EntityTile.h"
#include "mcpe/world/level/tile/entity/TileEntity.h"
#include "mcpe/world/item/Item.h"
#include "mcpe/world/item/ItemInstance.h"
#include "mcpe/world/item/TileItem.h"
#include "mcpe/world/material/Material.h"
#include "mcpe/world/entity/player/Player.h"
#include "mcpe/world/Facing.h"

#include "mcperedstone/world/item/RepeaterItem.h"
#include "mcperedstone/world/level/tile/RedstoneWireTile.h"
#include "mcperedstone/world/level/tile/NotGateTile.h"
#include "mcperedstone/world/level/tile/RepeaterTile.h"
#include "mcperedstone/world/level/tile/LightPressurePlateTile.h"
#include "mcperedstone/world/level/tile/HeavyPressurePlateTile.h"
#include "mcperedstone/world/level/tile/LeverTile.h"
#include "mcperedstone/world/level/tile/LampTile.h"
#include "mcperedstone/world/level/tile/ButtonTile.h"
#include "mcperedstone/utils/CreativeTab.h"


extern void (*_TileEntity$initTileEntities)();
void TileEntity$initTileEntities();

extern std::unique_ptr<TileEntity> (*_TileEntityFactory$createTileEntity)(TileEntityType, const TilePos&);
std::unique_ptr<TileEntity> TileEntityFactory$createTileEntity(TileEntityType, const TilePos&);


bool (*_TileTessellator$tessellateInWorld)(TileTessellator*, Tile*, const TilePos&, unsigned char, bool);
bool TileTessellator$tessellateInWorld(TileTessellator* self, Tile* tile, const TilePos& pos, unsigned char data, bool b) {
	switch(tile->renderType) {
	case 5:
		return self->tessellateRedstoneWireInWorld((RedstoneWireTile*) tile, pos);
	case 12:
		return self->tessellateLeverInWorld((LeverTile*) tile, pos);
	case 15:
		return self->tessellateRepeaterInWorld((RepeaterTile*) tile, pos);
	case 100:
		return self->tessellateLitNotGateInWorld((NotGateTile*) tile, pos);
	}

	return _TileTessellator$tessellateInWorld(self, tile, pos, data, b);
}

void initTileItems() {
	new TileItem(55 - 0x100);
	new TileItem(75 - 0x100);
	new TileItem(76 - 0x100);
	new TileItem(70 - 0x100);
	new TileItem(72 - 0x100);
	new TileItem(69 - 0x100);
	new TileItem(123 - 0x100);
	new TileItem(124 - 0x100);
	new TileItem(77 - 0x100);
	new TileItem(143 - 0x100);
	new TileItem(147 - 0x100);
	new TileItem(148 - 0x100);
	new TileItem(93 - 0x100);
	new TileItem(94 - 0x100);
}

void initMaterials() {
	Material::circuits.blocksMotion = false;
	Material::circuits.isSolid = false;
}

void (*_Tile$initTiles)();
void Tile$initTiles() {
	_Tile$initTiles();

	initMaterials();

	Tile::redStoneDust = new RedstoneWireTile(55, "redstone_dust_cross", &Material::circuits);
	Tile::notGate_off = new NotGateTile(75, "redstone_torch_off");
	Tile::notGate_on = new NotGateTile(76, "redstone_torch_on");
	Tile::pressurePlate_stone = new LightPressurePlateTile(70, "stonePlate", "stone", &Material::stone, LightPressurePlateTile::SENSITIVITY::MOBS);
	Tile::pressurePlate_wood = new LightPressurePlateTile(72, "woodPlate", "planks", &Material::wood, LightPressurePlateTile::SENSITIVITY::EVERYTHING);
	Tile::lever = new LeverTile(69, "lever", &Material::circuits);
	Tile::offlamp = new LampTile(123, "redstone_lamp_off", &Material::stone);
	Tile::onlamp = new LampTile(124, "redstone_lamp_on", &Material::stone);
	Tile::button = new ButtonTile(77, "stone", &Material::circuits);
	Tile::buttonWood = new ButtonTile(143, "planks", &Material::circuits);
	Tile::pressurePlate_gold = new HeavyPressurePlateTile(147, "goldPlate", "gold_block", &Material::metal, 15);
	Tile::pressurePlate_iron = new HeavyPressurePlateTile(148, "ironPlate", "iron_block", &Material::metal, 150);
	Tile::diode_off = new RepeaterTile(93, "repeater_off", false);
	Tile::diode_on = new RepeaterTile(94, "repeater_on", true);

	initTileItems();
}

void (*_Item$initItems)();
void Item$initItems() {
	Item::repeater = new RepeaterItem(100);
	Item::items[356] = Item::repeater;
	
	_Item$initItems();
}

void (*_Item$initCreativeItems)();
void Item$initCreativeItems() {
	CreativeTabWorker::reorderCreativeItems();
	_Item$initCreativeItems();
	Item::addCreativeItem(Item::repeater, 0);
	Item::addCreativeItem(Tile::notGate_on, 0);
	Item::addCreativeItem(Tile::pressurePlate_stone, 0);
	Item::addCreativeItem(Tile::pressurePlate_wood, 0);
	Item::addCreativeItem(Tile::pressurePlate_gold, 0);
	Item::addCreativeItem(Tile::pressurePlate_iron, 0);
	Item::addCreativeItem(Tile::lever, 0);
	Item::addCreativeItem(Tile::button, 0);
	Item::addCreativeItem(Tile::buttonWood, 0);
	Item::addCreativeItem(Tile::offlamp, 0);
}

ItemInstance (*_CreativeInventoryScreen$getItemFromType)(CreativeInventoryScreen*, CreativeTab);
ItemInstance CreativeInventoryScreen$getItemFromType(CreativeInventoryScreen* self, CreativeTab type) {
	if(type == CreativeTab::ITEMS) return ItemInstance(331, 1, 0);
	return _CreativeInventoryScreen$getItemFromType(self, type);
}

bool (*_Item$useOn)(Item*, ItemInstance*, Player*, int, int, int, signed char, float, float, float);
bool Item$useOn(Item* self, ItemInstance* item, Player* player, int x, int y, int z, signed char side, float xx, float yy, float zz) {
	if(item->item == Item::redStone) {
		if(Tile::redStoneDust->mayPlace(&player->region, x + Facing::STEP_X[side], y + Facing::STEP_Y[side], z + Facing::STEP_Z[side])) {
			player->region.setTileAndData(x + Facing::STEP_X[side], y + Facing::STEP_Y[side], z + Facing::STEP_Z[side], {55, 0}, 3);
			item->count--;
			return true;
		}
	}
	return _Item$useOn(self, item, player, x, y, z, side, xx, yy, zz);
}


JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* reserved) {

	MSHookFunction((void*) &Tile::initTiles, (void*) &Tile$initTiles, (void**) &_Tile$initTiles);
	MSHookFunction((void*) &Item::initItems, (void*) &Item$initItems, (void**) &_Item$initItems);
	MSHookFunction((void*) &TileTessellator::tessellateInWorld, (void*) &TileTessellator$tessellateInWorld, (void**) &_TileTessellator$tessellateInWorld);
	MSHookFunction((void*) &Item::initCreativeItems, (void*) &Item$initCreativeItems, (void**) &_Item$initCreativeItems);
	MSHookFunction((void*) &Item::useOn, (void*) &Item$useOn, (void**) &_Item$useOn);
	MSHookFunction((void*) &CreativeInventoryScreen::getItemFromType, (void*) &CreativeInventoryScreen$getItemFromType, (void**) &_CreativeInventoryScreen$getItemFromType);
	MSHookFunction((void*) &TileEntity::initTileEntities, (void*) &TileEntity$initTileEntities, (void**) &_TileEntity$initTileEntities);
	MSHookFunction((void*) &TileEntityFactory::createTileEntity, (void*) &TileEntityFactory$createTileEntity, (void**) &_TileEntityFactory$createTileEntity);

	VirtualHook("_ZTV7TntTile", "_ZN7TntTile15neighborChangedEP10TileSourceiiiiii", (void*) &TntTile::_neighborChanged);
	VirtualHook("_ZTV7TntTile", "_ZN4Tile7onPlaceEP10TileSourceiii", (void*) &TntTile::onPlace);

	return JNI_VERSION_1_2;
}
