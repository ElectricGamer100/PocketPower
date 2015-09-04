#include "PressurePlateTile.h"

#include "mcpe/world/level/TileSource.h"
#include "mcpe/world/phys/AABB.h"

PressurePlateTile::PressurePlateTile(int id, const std::string& texture, Material const* material) : Tile(id, texture, material) {
    init();
    setVisualShape(0.0625F, 0.0F, 0.0625F, 1.0F - 0.0625F, 0.0625F, 1.0F - 0.0625F);
    Tile::solid[id] = false;
    Tile::lightBlock[id] = 0;
    destroyTime = 1.0;
    creativeTab = CreativeTab::ITEMS;
    setTicking(true);

    if(isWood()) setNameId("pressurePlateWood");
    else setNameId("pressurePlateStone");
}

bool PressurePlateTile::entityInside(TileSource* region, int x, int y, int z, Entity* entity) {
	if(region->getData(x, y, z) != 1) setStateIfMobInteractsWithPlate(region, x, y, z);
}

bool PressurePlateTile::mayPlace(TileSource* region, int x, int y, int z) {
    int id = region->getTile(x, y - 1, z).id;
    return Tile::solid[id] || id == Tile::lightGem->id;
}

void PressurePlateTile::neighborChanged(TileSource* region, int x, int y, int z, int newX, int newY, int newZ) {
    if(!mayPlace(region, x, y, z)) {
        //popResource(region, x, y, z, ItemInstance(id, 1, 0));
        region->removeTile(x, y, z);
        region->scheduleBlockUpdate(x, y, z, id, 0);
    }
}

void PressurePlateTile::tick(TileSource* region, int x, int y, int z, Random* random) {
	if(region->getData(x, y, z) != 0) setStateIfMobInteractsWithPlate(region, x, y, z);
}

int PressurePlateTile::getSignal(TileSource* region, int x, int y, int z, int side) {
	if(region->getData(x, y, z) == 0) return 0; 
	if(side == 1) return 15;
     return 0;
}

int PressurePlateTile::getDirectSignal(TileSource* region, int x, int y, int z, int side) {
	return (region->getData(x, y, z) > 0)? 15 : 0;
}

bool PressurePlateTile::isSignalSource() {
	return true;
}

void PressurePlateTile::onRemove(TileSource* region, int x, int y, int z) {
    if(region->getData(x, y, z) > 0) {
        region->updateNeighborsAt({x, y, z}, id);
        region->updateNeighborsAt({x, y - 1, z}, id);
    }
    Tile::onRemove(region, x, y, z);
}

bool PressurePlateTile::_isMob(const Entity* entity) {
	int id = entity->getEntityTypeId();
	return id != 64 && id != 80 && id != 81 && id != 82 && id != 66;
}

bool PressurePlateTile::_listIncludesMob(EntityList& list) {
    for(int i = 0; i < list.size(); i++) {
        if(_isMob(list[i])) return true;
    }
    return false;
}

void PressurePlateTile::setStateIfMobInteractsWithPlate(TileSource* region, int x, int y, int z) {
    bool isPowered = region->getData(x, y, z) == 1;
    bool shouldBePowered = false;
    AABB aabb({x + 0.125F, y, z + 0.125F}, {(x + 1) - 0.125F, y + 0.25F, (z + 1) - 0.125F});
    EntityList list = region->getEntities(NULL, aabb);
    if(region->getTile(x, y, z).id == 70 && list.size() > 0 && _listIncludesMob(list)) shouldBePowered = true;
    else if(region->getTile(x, y, z).id == 72 && list.size() > 0) shouldBePowered = true;
    if(shouldBePowered && !isPowered) {
        region->setTileAndData(x, y, z, {id, 1}, 3);
        region->updateNeighborsAt({x, y, z}, id);
        region->updateNeighborsAt({x, y - 1, z}, id);
        region->fireTilesDirty(x, y, z, x, y, z);
        //region->getLevel()->playSound(x + 0.5F, y + 0.10000000000000001F, z + 0.5F, "random.click", 0.3F, 0.6F);
    }
    if(!shouldBePowered && isPowered) {
        region->setTileAndData(x, y, z, {id, 0}, 3);
        region->updateNeighborsAt({x, y, z}, id);
        region->updateNeighborsAt({x, y - 1, z}, id);
        region->fireTilesDirty(x, y, z, x, y, z);
        //region->getLevel()->playSound(x + 0.5F, y + 0.1F, z + 0.5F, "random.click", 0.3F, 0.5F);
    }
    if(shouldBePowered) region->scheduleBlockUpdate(x, y, z, id, getTickDelay());
}

void PressurePlateTile::onPlace(TileSource* region, int x, int y, int z) {}

int PressurePlateTile::getTickDelay() {
	return 20;
}

AABB const& PressurePlateTile::getVisualShape(TileSource* region, int x, int y, int z, AABB& aabb, bool b) {
    aabb.set(0.0625F, 0.0F, 0.0625F, 1.0F - 0.0625F, 0.0625F, 1.0F - 0.0625F);
    if(region->getData(x, y, z) == 0)
        aabb.set(0.0625F, 0.0F, 0.0625F, 1.0F - 0.0625F, 0.0625F, 1.0F - 0.0625F);
    else
        aabb.set(0.0625F, 0.0F, 0.0625F, 1.0F - 0.0625F, 0.03125F, 1.0F - 0.0625F);
    return aabb;
}

AABB const& PressurePlateTile::getVisualShape(unsigned char data, AABB& aabb, bool b) {
    aabb.set(0.0F, 0.5F - 0.125F, 0.0F, 1.0F, 0.5F + 0.125F, 1.0F);
    return aabb;
}

bool PressurePlateTile::isWood() {
      return id == 72;
}

void PressurePlateTile::addCollisionShapes(TileSource& region, int x, int y, int z, AABB const* aabb, std::vector<AABB, std::allocator<AABB>>& aabbs) {}
