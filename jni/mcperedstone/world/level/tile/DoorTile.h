#pragma once
#include "mcpe/world/level/tile/Tile.h"
class Mob;
class TileSource;

class DoorTile : public Tile {
public:
	static void $neighborChanged(DoorTile*, TileSource*, int, int, int, int, int, int);
	static void (*_$neighborChanged)(DoorTile*, TileSource*, int, int, int, int, int, int);
	
private:
	void setOpen(TileSource&, int, int, int, bool, Mob*);
	
	static bool isOpen(TileSource&, int, int, int);
};
