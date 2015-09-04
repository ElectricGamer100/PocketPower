#pragma once

#include "mcpe/world/level/tile/EntityTile.h"

class PistonExtensionTile : public EntityTile {
public:
	PistonExtensionTile(int, const std::string&, const Material*);
};
