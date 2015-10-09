#pragma once
#include "mcpe/world/level/tile/Tile.h"
class Mob;
class TileSource;

class TrapDoorTile : public Tile {
public:
	static void $neighborChanged(TrapDoorTile*, TileSource*, int, int, int, int, int, int);
	static void (*_$neighborChanged)(TrapDoorTile*, TileSource*, int, int, int, int, int, int);
	
private:
	void setOpen(TileSource*, int, int, int, bool);
};
