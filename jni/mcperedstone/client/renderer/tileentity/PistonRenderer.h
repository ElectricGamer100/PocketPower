#pragma once
#include "mcpe/client/renderer/tileentity/TileEntityRenderer.h"
class PistonTileEntity;

namespace mce {
	class MaterialPtr {
	public:
		MaterialPtr();
		void* whocares;
		void* whocares2;
		std::string str;
	};
};

class PistonRenderer : public TileEntityRenderer {
public:
	PistonRenderer(TileEntityRenderDispatcher*);
	virtual void render(TileSource&, TileEntity&, const Vec3&, float, const mce::MaterialPtr&, const std::string&, int);
	
private:
	void renderPiston(TileSource&, PistonTileEntity&, const Vec3&, float, const mce::MaterialPtr&);
};
