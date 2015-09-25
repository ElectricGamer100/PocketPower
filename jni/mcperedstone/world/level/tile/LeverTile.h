#pragma once

#include "mcpe/world/level/tile/Tile.h"


class LeverTile : public Tile {
public:
	LeverTile(int, const std::string&, Material const*);

	virtual bool isSignalSource();
	virtual int getPlacementDataValue(Mob*, int, int, int, signed char, float, float, float, int);
	virtual bool use(Player*, int, int, int);
	virtual void neighborChanged(TileSource*, int, int, int, int, int, int);
	virtual void tick(TileSource*, int, int, int, Random*);
	virtual void onPlace(TileSource*, int, int, int);
	virtual bool mayPlace(TileSource*, int, int, int, signed char);
	virtual bool canSurvive(TileSource*, int, int, int);
	virtual int getSignal(TileSource*, int, int, int, int);
	virtual int getDirectSignal(TileSource*, int, int, int, int);
	virtual const AABB& getVisualShape(TileSource*, int, int, int, AABB&, bool);
	virtual void addCollisionShapes(TileSource&, int, int, int, AABB const*, std::vector<AABB, std::allocator<AABB>>&);
};
