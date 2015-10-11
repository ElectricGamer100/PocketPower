#pragma once
#include "mcpe/world/level/tile/Tile.h"
#include "PistonPushInfo.h"

class PistonBaseTile: public Tile {
public:
	PistonBaseTile(int, bool);

	virtual const TextureUVCoordinateSet& getTexture(signed char, int);
	virtual const TextureUVCoordinateSet& getTexture(TileSource*, int, int, int, signed char);
	virtual void onPlace(TileSource*, int, int, int);
	virtual const AABB& getVisualShape(TileSource*, int, int, int, AABB&, bool);
	virtual const AABB& getVisualShape(unsigned char, AABB&, bool);
	virtual int getPlacementDataValue(Mob*, int, int, int, signed char, float, float, float, int);
	//virtual bool addCollisionShapes(TileSource&, int, int, int, const AABB*, std::vector<AABB, std::allocator<AABB>>&);
	virtual void triggerEvent(TileSource*, int, int, int, int, int);
	virtual void neighborChanged(TileSource*, int, int, int, int, int, int);

	static int getRotation(int);
	static bool isPowered(int);

private:
	TextureUVCoordinateSet texture_inner;
	TextureUVCoordinateSet texture_bottom;
	TextureUVCoordinateSet texture_side;
	bool sticky;

	static bool canPushRow(TileSource*, int, int, int, int);
	bool actuallyPushRow(TileSource*, int, int, int, int);
	void pushEntitiesInto(TileSource*, int, int, int, int, int, int);
	bool hasPower(TileSource*, int, int, int, int);
	void updateState(TileSource*, int, int, int);
	
	static PistonPushInfo getPushInfoFor(TileSource*, int, int, int);
};
