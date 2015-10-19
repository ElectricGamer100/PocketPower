#include "PistonTileEntity.h"
#include "mcpe/world/level/tile/Tile.h"
#include "mcpe/world/level/TileSource.h"
#include "mcpe/world/Facing.h"


PistonTileEntity::PistonTileEntity(Tile* storedBlock, int storedData, int orientation, bool extending, bool renderHead, const TilePos& position) :
	TileEntity(TileEntityType::Piston, position, "Piston") {

	this->storedBlock = storedBlock;
	this->storedData = storedData;
	this->orientation = orientation;
	this->extending = extending;
	this->renderHead = renderHead;
}

float PistonTileEntity::getInterpolatedProgress(float partialTicks) {
	if(partialTicks > 1.0F)
		partialTicks = 1.0F;

	return oldProgress + (progress - oldProgress) * partialTicks;
}

float PistonTileEntity::getOffsetX(float partialTicks) {
	return extending ? (getInterpolatedProgress(partialTicks) - 1.0F) * Facing::STEP_X[orientation] : (1.0F - getInterpolatedProgress(partialTicks)) * Facing::STEP_X[orientation];
}

float PistonTileEntity::getOffsetY(float partialTicks) {
	return extending ? (getInterpolatedProgress(partialTicks) - 1.0F) * Facing::STEP_Y[orientation] : (1.0F - getInterpolatedProgress(partialTicks)) * Facing::STEP_Y[orientation];
}

float PistonTileEntity::getOffsetZ(float partialTicks) {
	return extending ? (getInterpolatedProgress(partialTicks) - 1.0F) * Facing::STEP_Z[orientation] : (1.0F - getInterpolatedProgress(partialTicks)) * Facing::STEP_Z[orientation];
}

void PistonTileEntity::pushEntitiesInside(float _progress, float diff) {
	//if(extending) _progress = 1.0F - _progress;
	//else --_progress;

	//AABB storedTileBB = Tile::pistonExtension->getStoredTileBoundingBox(region, pos, this, progress);

	//if(!storedTileBB.isEmpty()) {
	      
	//}
}

void PistonTileEntity::placeTileAndFinish(TileSource* region) {
	if(oldProgress < 1.0F) {
		oldProgress = progress = 1.0F;
		finish();

		if(region->getTile(pos.x, pos.y, pos.z).id == 36) {
			region->setTileAndData(pos.x, pos.y, pos.z, {storedBlock->id, storedData}, 3);
			region->updateNeighborsAt(pos, storedBlock->id);
		}
	}
}

void PistonTileEntity::onRemoved() {

}

void PistonTileEntity::tick(TileSource* region) {
	TileEntity::tick(region);
	oldProgress = progress;
	
	if(oldProgress >= 1.0F) {
		pushEntitiesInside(1.0F, 0.25F);
		
		if(region->getTile(pos.x, pos.y, pos.z).id == 36) {
			if(storedBlock == NULL)
				region->setTileAndData(pos.x, pos.y, pos.z, {0, 0}, 3);
			else
				region->setTileAndData(pos.x, pos.y, pos.z, {storedBlock->id, storedData}, 3);
			
			region->updateNeighborsAt(pos, storedBlock->id);
		}
	} else {
		progress += 0.5F;

		if(progress > 1.0F)
			progress = 1.0F;

		if(extending)
			pushEntitiesInside(progress, progress - oldProgress + 0.0625F);
	}
}

bool PistonTileEntity::isFinished() {
	return finished;
}

void PistonTileEntity::finish() {
	finished = true;
}

void PistonTileEntity::load(CompoundTag* nbt) {

}

bool PistonTileEntity::save(CompoundTag* nbt) {

}