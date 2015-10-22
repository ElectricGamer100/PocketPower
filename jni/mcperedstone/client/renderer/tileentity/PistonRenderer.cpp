#include "PistonRenderer.h"
#include "mcpe/client/renderer/tileentity/TileEntityRenderDispatcher.h"
#include "mcpe/client/game/MinecraftClient.h"
#include "mcpe/client/renderer/game/LevelRenderer.h"
#include "mcpe/world/entity/player/Player.h"
#include "mcpe/world/level/TileSource.h"
#include "mcpe/client/renderer/renderer/Tessellator.h"
#include "mcpe/client/renderer/tile/TileTessellator.h"
#include "mcpe/client/renderer/texture/Textures.h"
#include "mcpe/world/level/tile/Tile.h"
#include "mcperedstone/world/level/tile/entity/PistonTileEntity.h"
#include "mcperedstone/world/level/tile/PistonArmTile.h"

PistonRenderer* TileEntityRenderDispatcher::pistonRenderer;

PistonRenderer::PistonRenderer(TileEntityRenderDispatcher* dispatcher) : TileEntityRenderer() {
	this->dispatcher = dispatcher;
}

void PistonRenderer::renderPiston(TileSource& region, PistonTileEntity& piston, const Vec3& pos, float partialTicks, const mce::MaterialPtr& material) {
	TileTessellator& blockRenderer = dispatcher->minecraft.levelRenderer->_getTileRenderer();
	Tessellator& tessellator = Tessellator::instance;
	Tile* block = piston.storedBlock;
	if(block && piston.getInterpolatedProgress(partialTicks) < 1.0F) {
		dispatcher->textures->bindTexture("terrain-atlas.tga", 0, true);
		
		// MCPC uses 7 as the rendering mode
		tessellator.begin(7);
		tessellator.setOffset(pos.x - piston.pos.x + piston.getOffsetX(partialTicks), pos.y - piston.pos.y + piston.getOffsetY(partialTicks), pos.z - piston.pos.z + piston.getOffsetZ(partialTicks));
		tessellator.color(1.0F, 1.0F, 1.0F, 1.0F);
		
		if(block == Tile::pistonArm && piston.getInterpolatedProgress(partialTicks) < 0.5F) {
			blockRenderer.tessellatePistonArmInWorld((PistonArmTile*) block, piston.pos, false);
		} else if(piston.renderHead && !piston.extending) {
			((PistonArmTile*) Tile::pistonArm)->setOverrideTexture(block->tex);
			blockRenderer.tessellatePistonArmInWorld((PistonArmTile*) Tile::pistonArm, piston.pos, piston.getInterpolatedProgress(partialTicks) < 0.5F);
			tessellator.setOffset(pos.x - piston.pos.x, pos.y - piston.pos.y, pos.z - piston.pos.z);
			blockRenderer.tessellatePistonBaseInWorld((PistonBaseTile*) block, piston.pos, true);
		} else {
			blockRenderer.tessellateBlockInWorld(block, piston.pos);
		}
		
		tessellator.setOffset(0, 0, 0);
		// I need to call this to get it to work properly, but it crashes with the provided MaterialPtr...
		//tessellator.draw(material);
	}
}

void PistonRenderer::render(TileSource& region, TileEntity& tileEntity, const Vec3& pos, float partialTicks, const mce::MaterialPtr& material, const std::string&, int) {
	renderPiston(region, (PistonTileEntity&) tileEntity, pos, partialTicks, material);
}
