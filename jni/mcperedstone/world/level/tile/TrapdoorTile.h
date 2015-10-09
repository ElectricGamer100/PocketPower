#pragma once
#include "mcpe/world/level/tile/Tile.h"
class Mob;
class TileSource;

class TrapdoorTile : public Tile {
public:
	static void $neighborChanged(TrapdoorTile*, TileSource*, int, int, int, int, int, int);
	static void (*_$neighborChanged)(TrapdoorTile*, TileSource*, int, int, int, int, int, int);
	
private:
	void setOpen(TileSource&, int, int, int, bool);
};
