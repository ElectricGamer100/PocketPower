#pragma once

#include "mcpe/world/level/tile/Tile.h"
#include "mcpe/world/entity/Entity.h"
class AABB;
class TileSource;

class PressurePlateTile : public Tile {
public:
	PressurePlateTile(int, const std::string&, Material const*);

	virtual void neighborChanged(TileSource*, int, int, int, int, int, int);
	virtual bool mayPlace(TileSource*, int, int, int);
	virtual bool entityInside(TileSource*, int, int, int, Entity*);
	virtual void addCollisionShapes(TileSource&, int, int, int, AABB const*, std::vector<AABB, std::allocator<AABB>>&);
	virtual int getTickDelay();
	virtual void onPlace(TileSource*, int, int, int);
	virtual void tick(TileSource*, int, int, int, Random*);
	virtual AABB const& getVisualShape(TileSource*, int, int, int, AABB&, bool);
	virtual AABB const& getVisualShape(unsigned char, AABB&, bool);
	virtual int getSignal(TileSource*, int, int, int, int);
	virtual int getDirectSignal(TileSource*, int, int, int, int);
	virtual bool isSignalSource();
	virtual void onRemove(TileSource*, int, int, int);

	bool isWood();
	void setStateIfMobInteractsWithPlate(TileSource*, int, int, int);
	bool _listIncludesMob(EntityList&);
	bool _isMob(const Entity*);
};
