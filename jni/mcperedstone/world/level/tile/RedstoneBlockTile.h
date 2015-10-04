#pragma once
#include "mcpe/world/level/tile/Tile.h"

class RedstoneBlockTile : public Tile {
public:
	RedstoneBlockTile(int);
	
	virtual bool isSignalSource();
	virtual int getDirectSignal(TileSource*, int, int, int, int);
};
