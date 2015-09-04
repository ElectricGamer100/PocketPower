#include "PistonExtensionTile.h"

PistonExtensionTile::PistonExtensionTile(int blockId, const std::string& texture, const Material* material) : EntityTile(blockId, texture, material) {
	init();
	setNameId("pistonExtension");
	tileEntityType = TileEntityType::Piston;
	creativeTab = CreativeTab::ITEMS;
}
