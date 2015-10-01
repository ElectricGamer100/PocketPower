#include "PistonArmTile.h"
#include "PistonBaseTile.h"
#include "mcpe/world/level/TileSource.h"
#include "mcpe/world/Facing.h"
#include "mcpe/world/phys/AABB.h"
#include "mcpe/world/material/Material.h"

PistonArmTile::PistonArmTile(int blockId) : Tile(blockId, &Material::stone) {
	init();
	
	setDestroyTime(0.5F);
	setNameId("piston_extension");
	renderType = 17;
	creativeTab = CreativeTab::ITEMS;
	Tile::solid[blockId] = false;
	Tile::lightBlock[blockId] = 0;

	tex = getTextureUVCoordinateSet("piston_top_normal", 0);
	texture_sticky = getTextureUVCoordinateSet("piston_top_sticky", 0);
	texture_side = getTextureUVCoordinateSet("piston_side", 0);
}

void PistonArmTile::onRemove(TileSource* region, int x, int y, int z) {
	// Remove the attached piston base
	int data = region->getData(x, y, z);
	int rotation = Facing::OPPOSITE_FACING[getRotation(data)];
	x += Facing::STEP_X[rotation];
	y += Facing::STEP_Y[rotation];
	z += Facing::STEP_Z[rotation];
	/*int tile = region->getTile(x, y, z).id;
	if(tile == Tile::pistonNormal->id || tile == Tile::pistonSticky->id) {
		data = region->getData(x, y, z);
		if(PistonBaseTile::isPowered(data)) {
			region->setTileAndData(x, y, z, {0, 0}, 0);
		}
	}*/
}

const AABB& PistonArmTile::getVisualShape(TileSource* region, int x, int y, int z, AABB& shape, bool b) {
	switch(getRotation(region->getData(x, y, z))) {
	case 0:
		shape.set(0.0F, 0.0F, 0.0F, 1.0F, 0.25F, 1.0F);
		break;
	case 1:
		shape.set(0.0F, 0.75F, 0.0F, 1.0F, 1.0F, 1.0F);
		break;
	case 2:
		shape.set(0.0F, 0.0F, 0.0F, 1.0F, 1.0F, 0.25F);
		break;
	case 3:
		shape.set(0.0F, 0.0F, 0.75F, 1.0F, 1.0F, 1.0F);
		break;
	case 4:
		shape.set(0.0F, 0.0F, 0.0F, 0.25F, 1.0F, 1.0F);
		break;
	case 5:
		shape.set(0.75F, 0.0F, 0.0F, 1.0F, 1.0F, 1.0F);
		break;
	}
	
	return shape;
}

void PistonArmTile::neighborChanged(TileSource* region, int x, int y, int z, int xx, int yy, int zz) {
	int rotation = getRotation(region->getData(x, y, z));
	Tile* tile = Tile::tiles[region->getTile(x - Facing::STEP_X[rotation], y - Facing::STEP_Y[rotation], z - Facing::STEP_Z[rotation]).id];
	if(tile != Tile::pistonNormal && tile != Tile::pistonSticky)
		region->setTileAndData(x, y, z, {0, 0}, 3);
	else
		tile->neighborChanged(region, x - Facing::STEP_X[rotation], y - Facing::STEP_Y[rotation], z - Facing::STEP_Z[rotation], xx, yy, zz);
}

const TextureUVCoordinateSet& PistonArmTile::getTexture(signed char side, int data) {
	int rotation = getRotation(data);
	if(side == rotation)
		return isSticky(data)? texture_sticky : tex;
	int opposite[6] = {1, 0, 3, 2, 5, 4};
	if(side == opposite[rotation])
		return tex;
	return texture_side;
}

int PistonArmTile::getRotation(int data) {
	return data & 7;
}

bool PistonArmTile::isSticky(int data) {
    int sticky = data & 8;
    if(sticky == 8) return true;
    if(sticky == 0) return false;
    return false;
}
