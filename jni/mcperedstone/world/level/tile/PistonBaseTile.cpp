#include "PistonBaseTile.h"
#include "mcpe/world/material/Material.h"
#include "mcpe/world/level/TileSource.h"


PistonBaseTile::PistonBaseTile(int blockId, bool sticky) : Tile(blockId, &Material::stone) {
	init();

	if(!sticky) {
		setNameId("piston");
		tex = getTextureUVCoordinateSet("piston_top_normal", 0);
	}
	else {
		setNameId("piston_sticky");
		tex = getTextureUVCoordinateSet("piston_top_sticky", 0);
	}

	this->sticky = sticky;
	creativeTab = CreativeTab::ITEMS;
	renderType = 0;
	setDestroyTime(0.5F);
	solid[blockId] = false;
	lightBlock[blockId] = 0;

	texture_inner = getTextureUVCoordinateSet("piston_inner", 0);
	texture_bottom = getTextureUVCoordinateSet("piston_bottom", 0);
	texture_side = getTextureUVCoordinateSet("piston_side", 0);
}

/*
	Get the textures for the TileItem
*/
const TextureUVCoordinateSet& PistonBaseTile::getTexture(signed char side, int data) {
	return (side == 1)? tex : texture_side;
}

/*
	Get the textures for the block in the world
*/
const TextureUVCoordinateSet& PistonBaseTile::getTexture(TileSource* region, int x, int y, int z, signed char side) {
	int data = region->getData(x, y, z);
	bool powered = isPowered(data);
	int rotation = getRotation(data);

	if(side == rotation)
		return (powered)? texture_inner : tex;
	if(side == Facing::OPPOSITE[rotation])
		return texture_bottom;

	return texture_side;
}

/*
	Get the shape for the inventory block
*/

const AABB& PistonBaseTile::getVisualShape(unsigned char data, AABB& shape, bool idk) {
	shape.set(0.0F, 0.0F, 0.0F, 1.0F, 1.0F, 1.0F);
	return shape;
}

const AABB& PistonBaseTile::getVisualShape(TileSource* region, int x, int y, int z, AABB& shape, bool idk) {
	if(isPowered(region->getData(x, y, z))) {
		switch(getRotation(region->getData(x, y, z))) {
		case 0:
			shape.set(0.0F, 0.25F, 0.0F, 1.0F, 1.0F, 1.0F);
			break;
		case 0:
			shape.set(0.0F, 0.0F, 0.0F, 1.0F, 0.75F, 1.0F);
			break;
		case 0:
			shape.set(0.0F, 0.0F, 0.25F, 1.0F, 1.0F, 1.0F);
			break;
		case 0:
			shape.set(0.0F, 0.0F, 0.0F, 1.0F, 1.0F, 0.75F);
			break;
		case 0:
			shape.set(0.25F, 0.0F, 0.0F, 1.0F, 1.0F, 1.0F);
			break;
		case 0:
			shape.set(0.0F, 0.0F, 0.0F, 0.75F, 1.0F, 1.0F);
			break;
		}
	}
	else
		shape.set(0.0F, 0.0F, 0.0F, 1.0F, 1.0F, 1.0F);

	return shape;
}

int PistonBaseTile::getRotation(int data) {
	return data & 3;
}

bool PistonBaseTile::isPowered(int data) {
	return (data & 8) != 0;
}

void PistonBaseTile::neighborChanged(TileSource* region, int x, int y, int z, int neighborX, int neighborY, int neighborZ) {
	updateState(region, x, y, z);
}

void PistonBaseTile::onPlace(TileSource* region, int x, int y, int z) {
	updateState(region, x, y, z);
}

void PistonBaseTile::updateState(TileSource* region, int x, int y, int z) {
	int data = region->getData(x, y, z); // You might just get hit with the RICO
	int rotation = getRotation(data);
	if(data == 7)
		return;
	bool shouldBePowered = hasPower(region, x, y, z, rotation);
	if(shouldBePowered && !isPowered(data)) {
		if(canPushRow(region, x, y, z, rotation)) {
			region->setTileAndData(x, y, z, {id, rotation | 8}, 0);
			region->tileEvent(x, y, z, 0, rotation);
		}
	}
	else if(!shouldBePowered && isPowered(data)) {
		region->setTileAndData(x, y, z, {id, rotation}, 0);
		region->tileEvent(x, y, z, 1, rotation);
	}
}

bool PistonBaseTile::hasPower(TileSource* region, int x, int y, int z, int rotation) {
	// This can only return true if it is not being powered at its face
	// This is because back in Minecraft beta 1.7 (when pistons were first added), the
	// game crashes when we powered a piston at its face, because it would become powered
	// and unpowered in the same tick.
	if(rotation != 0 && region->getIndirectPowerOutput(x, y - 1, z, 0))
		return true;
	if(rotation != 1 && region->getIndirectPowerOutput(x, y + 1, z, 1))
		return true;
	if(rotation != 2 && region->getIndirectPowerOutput(x, y, z - 1, 2))
		return true;
	if(rotation != 3 && region->getIndirectPowerOutput(x, y, z + 1, 3))
		return true;
	if(rotation != 4 && region->getIndirectPowerOutput(x - 1, y, z, 4))
		return true;
	if(rotation != 5 && region->getIndirectPowerOutput(x + 1, y, z, 5))
		return true;

	// So BUD switches work
	if(region->getIndirectPowerOutput(x, y, z, 0) ||
		region->getIndirectPowerOutput(x, y + 2, z, 1) ||
		region->getIndirectPowerOutput(x, y + 1, z - 1, 2) ||
		region->getIndirectPowerOutput(x, y + 1, z + 1, 3) ||
		region->getIndirectPowerOutput(x - 1, y + 1, z, 4) ||
		region->getIndirectPowerOutput(x + 1, y + 1, z, 5))
			return true;
}

int PistonBaseTile::getPlacementDataValue(Mob* player, int x, int y, int z, signed char side, float xx, float yy, float zz, int data) {
	if(abs(player->x - x) < 2.0F && abs(player->z - z) < 2.0F) {
		float temp = player->y + 1.82F - player->heightOffset;
		if(temp - y > 2.0F)
			return 1;
		if(y - temp > 0.0F)
			return 0;
	}
	int temp2 = (int) floor(((player->yaw * 4.0F) / 360.0F) + 0.5F) & 3;
	return temp2 == 0 ? 2 : (temp2 == 1 ? 5 : (temp2 == 2 ? 3 : (temp2 == 3 ? 4 : 0)));
}

bool PistonBaseTile::canPushRow(TileSource* region, int x, int y, int z, int rotation) {
	int xx = x + Facing::STEP_X[rotation];
	int yy = y + Facing::STEP_Y[rotation];
	int zz = z + Facing::STEP_Z[rotation];
	int counter = 0;
	do {
		if(yy <= 0 || yy >= 127) break; // Can't push blocks out of the world
		int blockId = region->getData(xx, yy, zz).id;
		if(Tile::tiles[blockId]->replaceable) break; // Break things like flowers, etc.
		if(!isMoveableBlock(Tile::tiles[id], region, xx, yy, zz, true)) return false; // Can't extend the piston :(

		xx += Facing::STEP_X[rotation];
		yy += Facing::STEP_Y[rotation];
		zz += Facing::STEP_Z[rotation];
		counter++;
	} while(counter < 13); // You may only push 12 blocks

	return true;
}

