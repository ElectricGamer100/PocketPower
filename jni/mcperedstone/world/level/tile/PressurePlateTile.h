#pragma once

#include "mcpe/world/level/tile/Tile.h"
class Entity;
class AABB;
class TileSource;
class Player;

class PressurePlateTile : public Tile {
public:
	PressurePlateTile(int, const std::string&, const std::string&, Material const*);

	virtual void neighborChanged(TileSource*, int, int, int, int, int, int);
	virtual bool mayPlace(TileSource*, int, int, int);
	virtual bool entityInside(TileSource*, int, int, int, Entity*);
	virtual void addCollisionShapes(TileSource&, int, int, int, AABB const*, std::vector<AABB, std::allocator<AABB>>&);
	virtual int getTickDelay();
	virtual void onPlace(TileSource*, int, int, int);
	virtual void tick(TileSource*, int, int, int, Random*);
	virtual AABB const& getVisualShape(TileSource*, int, int, int, AABB&, bool);
	virtual int getSignal(TileSource*, int, int, int, int);
	virtual int getDirectSignal(TileSource*, int, int, int, int);
	virtual bool isSignalSource();
	virtual void playerDestroy(Player*, int, int, int, int);

	void setStateIfMobInteractsWithPlate(TileSource*, int, int, int, int);

protected:
	virtual int getPower(TileSource*, int, int, int) = 0;
	virtual int getPowerFromData(int) = 0;
	virtual int getDataFromPower(int) = 0;
};
