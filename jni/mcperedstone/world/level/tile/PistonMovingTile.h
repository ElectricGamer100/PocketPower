#pragma once
#include "mcpe/world/level/tile/EntityTile.h"
#include "mcpe/world/level/TilePos.h"

class PistonMovingTile : public EntityTile {
public:
	PistonMovingTile(int);

	virtual std::unique_ptr<TileEntity> newTileEntity(const TilePos&);

	static void setTileEntityAttributes(Tile*, int, int, bool, bool, const TilePos&);

private:
	static Tile* pushedTile;
	static int pushedData;
	static int rotation;
	static bool isExtending;
	static bool renderHead;
	static TilePos pos;
};
