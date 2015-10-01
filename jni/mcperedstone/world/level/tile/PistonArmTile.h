#pragma once
#include "mcpe/world/level/tile/Tile.h"

class PistonArmTile : public Tile {
public:
	PistonArmTile(int);
	
	virtual const AABB& getVisualShape(TileSource*, int, int, int, AABB&, bool);
	virtual void neighborChanged(TileSource*, int, int, int, int, int, int);
	virtual void onRemove(TileSource*, int, int, int);
	virtual const TextureUVCoordinateSet& getTexture(signed char, int);
	
	static int getRotation(int);
	static bool isSticky(int);
	
private:
	TextureUVCoordinateSet texture_sticky;
	TextureUVCoordinateSet texture_side;
};
