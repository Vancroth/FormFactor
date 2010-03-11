#include "LevelEntity.h"

namespace FormFactor {

	const unsigned int NUM_TILES = 2;

LevelEntity::LevelEntity(SceneNode *node, std::vector<Reference<Primitive> > &worldPrims) : GameEntity(node)
{
	nTiles = 0;

/*	Reference<LevelTileEntity> tile = new LevelTileEntity(node->createChildSceneNode("Tile0", Vector3(0, 0, 0)), this, nTiles++);
	tiles.push_back(tile);
	worldPrims.push_back(tile.getPtr());

	tile = new LevelTileEntity(node->createChildSceneNode("Tile1", Vector3(LevelTileEntity::TILE_WIDTH, 0, 0)), this, nTiles++);
	tiles.push_back(tile);
	worldPrims.push_back(tile.getPtr());

	tileInfo = new LevelTileInfo[2];
	int next = 1;
	tileInfo[0] = LevelTileInfo(1, &next, 0);
	tileInfo[1] = LevelTileInfo(0, NULL, 0);
*/
	Reference<LevelTileEntity> tile;
	tileInfo = new LevelTileInfo[NUM_TILES];
	int next = 1; int prev = 0;
	unsigned int displacement = 0;
	for(unsigned int i = 0; i < NUM_TILES; i++) {
		char buf[60]; sprintf(buf, "Tile%d", i);
		tile = new LevelTileEntity(node->createChildSceneNode(buf, Vector3(0, 0, -float(displacement))), this, nTiles++);
		tiles.push_back(tile);
		worldPrims.push_back(tile.getPtr());
		tileInfo[i] = LevelTileInfo((i==NUM_TILES-1 ? 0 : 1), &next, prev);
		
		next++;
		displacement += LevelTileEntity::TILE_HEIGHT;
		if(i!=0) prev++;
	}


	// Procedurally determine tiles

	nTiles = tiles.size();
}

LevelEntity::~LevelEntity(void)
{
	delete[] tileInfo;
}

bool LevelEntity::frameEvent(const FrameEvent &evt) {
	return true;
}

void LevelEntity::allocateTileTrees(unsigned int tileID) const {
	LevelTileInfo *info = &tileInfo[tileID];

	// Kill old trees
	for(unsigned int i = 0; i < info->predecessor; i++) {
		LevelTileEntity *ptr = const_cast<LevelTileEntity*>(tiles[i].getPtr());
		ptr->destroyAccelerator();
	}

	// Don't kill immediate predecessor incase still overlapping

	for(unsigned int i = info->predecessor + 1; i < tileID; i++) {
		LevelTileEntity *ptr = const_cast<LevelTileEntity*>(tiles[i].getPtr());
		ptr->destroyAccelerator();
	}

	// Build successor trees
	for(unsigned int i = 0; i < info->nSuccessors; i++) {
		LevelTileEntity *ptr = const_cast<LevelTileEntity*>(tiles[info->successors[i]].getPtr());
		ptr->buildAccelerator();
	}

}

} // end FormFactor