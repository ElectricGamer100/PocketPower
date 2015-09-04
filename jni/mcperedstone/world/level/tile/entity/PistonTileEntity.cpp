#include "PistonTileEntity.h"

#include "mcpe/world/level/tile/Tile.h"
#include "mcpe/world/level/TileSource.h"
#include "mcpe/world/Facing.h"

PistonTileEntity::PistonTileEntity(const TilePos& pos, int blockId, int meta, int rotation, bool isExtended, bool drawHead) :
      TileEntity(TileEntityType::Piston, pos, "Piston") {

      storedBlock = Tile::tiles[54];
      storedData = meta;
      orientation = rotation;
      extending = isExtended;
      renderHead = drawHead;
}

float PistonTileEntity::getInterpolatedProgress(float ticks) {
      if(ticks > 1.0F) ticks = 1.0F;

      return oldProgress + (progress - oldProgress) * ticks;
}

float PistonTileEntity::getOffsetX(float ticks) {
      return extending ? (getInterpolatedProgress(ticks) - 1.0F) * Facing::STEP_X[orientation] : (1.0F - getInterpolatedProgress(ticks)) * Facing::STEP_X[orientation];
}

float PistonTileEntity::getOffsetY(float ticks) {
      return extending ? (getInterpolatedProgress(ticks) - 1.0F) * Facing::STEP_Y[orientation] : (1.0F - getInterpolatedProgress(ticks)) * Facing::STEP_Y[orientation];
}

float PistonTileEntity::getOffsetZ(float ticks) {
      return extending ? (getInterpolatedProgress(ticks) - 1.0F) * Facing::STEP_Z[orientation] : (1.0F - getInterpolatedProgress(ticks)) * Facing::STEP_Z[orientation];
}

void PistonTileEntity::pushEntitiesInside(float _progress, float diff) {
      if(extending) _progress = 1.0F - _progress;
      else --_progress;

      //AABB storedTileBB = Tile::pistonExtension->getStoredTileBoundingBox(region, pos, this, progress);

      //if(!storedTileBB.isEmpty()) {
            
      //}
}

void PistonTileEntity::onRemoved() {

}

void PistonTileEntity::tick(TileSource* region) {
      /*oldProgress = progress;
      
      if(oldProgress >= 1.0F) { // if it's completely pushed into the next block
            pushEntitiesInside(1.0F, 0.25F);
            removed = true;
            if(region->getTile(pos.x, pos.y, pos.z).id == 34) {
                  region->setTileAndData(pos.x, pos.y, pos.z, {storedBlock->id, storedData}, 3);
                  region->updateNeighborsAt(pos, storedBlock->id);
            }
      } else {
            progress += 0.5F;

            if(progress > 1.0F) progress = 1.0F;

            if(extending) {
                  pushEntitiesInside(progress, progress - oldProgress + 0.0625F);
            }
      }*/
}

void PistonTileEntity::load(CompoundTag* nbt) {

}

bool PistonTileEntity::save(CompoundTag* nbt) {

}
