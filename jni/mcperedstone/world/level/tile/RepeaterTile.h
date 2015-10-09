#pragma once
#include "mcpe/world/level/tile/Tile.h"
class Random;
class TileSource;
struct Player;
class Mob;

class RepeaterTile : public Tile {
public:
	bool powered;
	TextureUVCoordinateSet side_texture;

	RepeaterTile(int, const std::string&, bool);

	static float torchOffset[4];

	virtual const TextureUVCoordinateSet& getTexture(signed char, int);
	virtual void tick(TileSource*, int, int, int, Random*);
	virtual int getSignal(TileSource*, int, int, int, int);
	virtual int getDirectSignal(TileSource*, int, int, int, int);
	virtual void neighborChanged(TileSource*, int, int, int, int, int, int);
	virtual bool isSignalSource();
	virtual int getPlacementDataValue(Mob*, int, int, int, signed char, float, float, float, int);
	virtual void onPlace(TileSource*, int, int, int);
	virtual bool canSurvive(TileSource*, int, int, int);
	virtual bool mayPlace(TileSource*, int, int, int);
	virtual bool use(Player*, int, int, int);
	virtual int getResource(Random*, int, int);

private:
	bool isGettingPowered(TileSource*, int, int, int, int);
	static int delaySettings[4];
};
