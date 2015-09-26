#pragma once
#include "PressurePlateTile.h"

class HeavyPressurePlateTile : public PressurePlateTile {
public:
	HeavyPressurePlateTile(int, const std::string&, const std::string&, const Material*);

	virtual int getTickDelay();

protected:
	virtual int getPower(TileSource*, int, int, int);
	virtual int getPowerFromData(int);
	virtual int getDataFromPower(int);

private:
	int weight;
};
