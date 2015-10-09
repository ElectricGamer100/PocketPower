#pragma once

#include <vector>

#include "mcpe/world/level/tile/Tile.h"
class Entity;
typedef std::vector<Entity*, std::allocator<Entity*>> EntityList;

class ButtonTile : public Tile {
public:
	ButtonTile(int, const std::string&, Material const*);

	virtual const AABB& getVisualShape(TileSource*, int, int, int, AABB&, bool);
	virtual const AABB& getVisualShape(unsigned char, AABB&, bool);
	virtual void onPlace(TileSource*, int, int, int);
	virtual void onRemove(TileSource*, int, int, int);
	virtual bool mayPlace(TileSource*, int, int, int, signed char);
	virtual bool canSurvive(TileSource*, int, int, int);
	virtual void neighborChanged(TileSource*, int, int, int, int, int, int);
	virtual int getPlacementDataValue(Mob*, int, int, int, signed char, float, float, float, int);
	virtual int getTickDelay();
	virtual bool entityInside(TileSource*, int, int, int, Entity*);
	virtual bool use(Player*, int, int, int);
	virtual bool addCollisionShapes(TileSource&, int, int, int, AABB const*, std::vector<AABB, std::allocator<AABB>>&);
	virtual int getSignal(TileSource*, int, int, int, int);
	virtual int getDirectSignal(TileSource*, int, int, int, int);
	virtual void tick(TileSource*, int, int, int, Random*);
	virtual bool isSignalSource();

	bool listIncludesArrow(EntityList);
	void toggleIfArrowInside(TileSource*, int, int, int);
	bool isWood();
};
