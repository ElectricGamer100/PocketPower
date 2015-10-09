#pragma once
#include "mcpe/world/level/tile/Tile.h"
class Player;

class PistonArmTile : public Tile {
public:
	PistonArmTile(int);
	
	virtual const AABB& getVisualShape(TileSource*, int, int, int, AABB&, bool);
	virtual void neighborChanged(TileSource*, int, int, int, int, int, int);
	virtual void playerDestroy(Player*, int, int, int, int);
	virtual const TextureUVCoordinateSet& getTexture(signed char, int);
	virtual int getResourceCount(Random*, int, int);
	
	static int getRotation(int);
	static bool isSticky(int);
	
	TextureUVCoordinateSet texture_sticky;
	TextureUVCoordinateSet texture_side;
};
