#pragma once

class Item;
class Tile;
class CompoundTag;

class ItemInstance {
public:
	short count;
	short damage;
	CompoundTag* userData;
	Item* item;
	Tile* tile;
	char filler[12];

	void _setItem(int);
	ItemInstance(int, int, int);
};
