#pragma once

#include <vector>

#include "mcpe/world/level/tile/entity/TileEntity.h"

class Entity;

class PistonTileEntity : public TileEntity {
public:
	Tile* storedBlock;
	int storedData;
	int orientation;
	bool extending;
	bool renderHead;
	float progress;
	float oldProgress;
	std::vector<Entity*> pushedMobs;
	bool finished;

	PistonTileEntity(const TilePos&, int, int, int, bool, bool);

	virtual void load(CompoundTag*);
	virtual bool save(CompoundTag*);
	virtual void tick(TileSource*);
	virtual void onRemoved();
	virtual bool isFinished();

	float getInterpolatedProgress(float);
	float getOffsetX(float);
	float getOffsetY(float);
	float getOffsetZ(float);
	void pushEntitiesInside(float, float);
	void finish();
};
