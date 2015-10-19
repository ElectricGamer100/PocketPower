#pragma once
#include "mcpe/world/level/tile/EntityTile.h"

class PistonMovingTile : public EntityTile {
public:
	PistonMovingTile(int);

	virtual TileEntity* newTileEntity(const TilePos&);

	void setTileEntityAttributes(Tile*, int, int, bool, bool, TilePos&);

private:
	Tile* pushedTile;
	int pushedData;
	int rotation;
	bool isExtending;
	bool renderHead;
	TilePos pos;
};
