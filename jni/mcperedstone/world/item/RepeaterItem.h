#pragma once
#include "mcpe/world/item/Item.h"

class RepeaterItem: public Item {
public:
	RepeaterItem(int);
	
	virtual bool useOn(ItemInstance*, Player*, int, int, int, signed char, float, float, float);
};
