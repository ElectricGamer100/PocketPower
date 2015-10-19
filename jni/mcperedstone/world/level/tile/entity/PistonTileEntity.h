#pragma once
#include "mcpe/world/level/tile/entity/TileEntity.h"
#include <vector>

class Entity;

class PistonTileEntity : public TileEntity {
public:
	PistonTileEntity(Tile*, int, int, bool, bool, const TilePos&);

	virtual void load(CompoundTag*);
	virtual bool save(CompoundTag*);
	virtual void tick(TileSource*);
	virtual void onRemoved();
	virtual bool isFinished();

	Tile* storedBlock;
	int storedData;
	int orientation;
	bool extending;
	bool renderHead;
	float progress;
	float oldProgress;
	bool finished;

	float getInterpolatedProgress(float);
	float getOffsetX(float);
	float getOffsetY(float);
	float getOffsetZ(float);
	void pushEntitiesInside(float, float);
	void placeTileAndFinish(TileSource*);
	void finish();
};
