#pragma once

#include "mcpe/world/level/tile/TorchTile.h"
class Random;
class TileSource;

class NotGateTile : public TorchTile {
public:
	NotGateTile(int, const std::string&);

	virtual void animateTick(TileSource*, int, int, int, Random*);
	virtual bool mayPlace(TileSource*, int, int, int, signed char);
	virtual bool canSurvive(TileSource*, int, int, int);
	virtual void onPlace(TileSource*, int, int, int);
	virtual void onRemove(TileSource*, int, int, int);
	virtual bool isSignalSource();
	virtual int getTickDelay();
	virtual int getDirectSignal(TileSource*, int, int, int, int);
	virtual int getSignal(TileSource*, int, int, int, int);
	virtual void neighborChanged(TileSource*, int, int, int, int, int, int);
	virtual int getResource(Random*, int, int);
	virtual void tick(TileSource*, int, int, int, Random*);

	virtual bool isActive();
	bool checkForBurnout(TileSource*, int, int, int);
};
