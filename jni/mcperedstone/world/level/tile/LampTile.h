#pragma once

#include "mcpe/world/level/tile/Tile.h"

class LampTile : public Tile {
public:
	LampTile(int, const std::string&, Material const*);

	virtual void onPlace(TileSource*, int, int, int);
	virtual void neighborChanged(TileSource*, int, int, int, int, int, int);
	virtual void tick(TileSource*, int, int, int, Random*);
	virtual int getResource(int, Random*);

	bool isLit();
};
