#pragma once
#include "mcpe/world/level/tile/Tile.h"
class TilePos;

class FenceGateTile : public Tile {
public:
	static void _neighborChanged(FenceGateTile*, TileSource*, int, int, int, int, int, int);

private:
	static bool isOpen(int);
	void _checkIsInWall(TileSource*, const TilePos&);
};