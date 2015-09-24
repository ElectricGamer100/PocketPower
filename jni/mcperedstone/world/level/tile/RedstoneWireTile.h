#pragma once

#include <array>
#include <vector>

#include "mcpe/world/level/tile/Tile.h"
class TileSource;
class AABB;
class Random;

class RedstoneWireTile : public Tile {
public:
	bool wiresProvidePower;

	static std::array<unsigned int, 16> colors;

	RedstoneWireTile(int, const std::string&, const Material*);

	virtual int getColor(TileSource*, int, int, int);
	virtual int getResource(Random*, int, int);
	virtual void onPlace(TileSource*, int, int, int);
	virtual bool canSurvive(TileSource*, int, int, int);
	virtual bool mayPlace(TileSource*, int, int, int);
	virtual void neighborChanged(TileSource*, int, int, int, int, int, int);
	virtual void animateTick(TileSource*, int, int, int, Random*);
	virtual void tick(TileSource*, int, int, int, Random*);
	virtual void onRemove(TileSource*, int, int, int);
	virtual bool isSignalSource();
	virtual int getSignal(TileSource*, int, int, int, int);
	virtual int getDirectSignal(TileSource*, int, int, int, int);
	virtual void addCollisionShapes(TileSource&, int, int, int, AABB const*, std::vector<AABB, std::allocator<AABB>>&);

	void recalculate(TileSource*, int, int, int);
	void calculateChanges(TileSource*, int, int, int, int, int, int);
	void updateWires(TileSource*, int, int, int);
	int getStrongerSignal(TileSource*, int, int, int, int);
	bool canRedstoneConnectTo(TileSource*, int, int, int, int) const;
};