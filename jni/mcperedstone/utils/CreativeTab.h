#pragma once

class Item;
class Tile;

enum CreativeTab : int {
	BLOCKS = 1,
	DECORATIONS,
	TOOLS,
	ITEMS
};

class CreativeTabWorker {
public:
	static void reorderCreativeItems();
	static void reorder(Item*, CreativeTab);
	static void reorder(Tile*, CreativeTab);
	static void _reorderPocketpowerItems();
};