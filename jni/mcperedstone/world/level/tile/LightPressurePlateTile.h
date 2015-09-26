#pragma once
#include "PressurePlateTile.h"
#include "mcpe/world/entity/Entity.h"

class LightPressurePlateTile : public PressurePlateTile {
public:
	enum class SENSITIVITY {
		EVERYTHING,
		MOBS
	};

	LightPressurePlateTile(int, const std::string&, const Material*, SENSITIVITY);

protected:
	virtual int getPower(TileSource*, int, int, int);
	virtual int getPowerFromData(int);
	virtual int getDataFromPower(int);

private:
	SENSITIVITY sensitivity;

	bool _listIncludesMob(EntityList&);
	bool _EntityisMob(const Entity*);
};
