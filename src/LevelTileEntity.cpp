#include "LevelTileEntity.h"
#include "LevelEntity.h"

#include "LevelTilePrims.h"

namespace FormFactor {
	const unsigned int LevelTileEntity::TILE_WIDTH =  100;
	const unsigned int LevelTileEntity::TILE_HEIGHT = 100;

	char* terrains[] = {"Examples/GrassFloor", "Examples/BeachStones", "Examples/BumpyMetal"};

LevelTileEntity::LevelTileEntity(SceneNode *node, Reference<LevelEntity> l, unsigned int tileID) : Primitive(node)
{
	id = tileID;
	firstIntersect = true;
	level = l;

	// Procedurally build tile and add Primitives to prims
		
	// Create power ups
	char buf[60]; sprintf(buf, "PowerUp1-%d", id);
	SceneNode *powerUpNode = node->createChildSceneNode(buf, Vector3(-20, 0, 0));
	PowerUp *powerUp = new PowerUp(powerUpNode);
	powerUp->start();
	prims.push_back(powerUp);

	memset(buf, 0, 60); sprintf(buf, "PowerUp2-%d", id);
	powerUpNode = node->createChildSceneNode(buf, Vector3(0, 10, -200));
	powerUp = new FormFactor::PowerUp(powerUpNode);
	powerUp->start();
	prims.push_back(powerUp);

	memset(buf, 0, 60); sprintf(buf, "Ground-%d", id);
	SceneNode *groundNode = node->createChildSceneNode(buf, Vector3(0, 0, 0));
	FormFactor::Ground *ground = new FormFactor::Ground(groundNode, terrains[rand() % 3], TILE_WIDTH, TILE_HEIGHT);
	ground->start();
	prims.push_back(ground);

	// Make bounding box encompass all prims
	for(unsigned int i = 0; i < prims.size(); i++)
		bounds = bounds.getUnion(prims[i]->worldBound());
}

LevelTileEntity::~LevelTileEntity(void)
{
}

bool LevelTileEntity::frameEvent(const FrameEvent &evt) {
	return true;
}

void LevelTileEntity::buildAccelerator() {
	tree = new KdTree(prims, 40, 1, .5f, 4, 20);

	// Now tree is built, can clear prims
	prims.clear();
}

void LevelTileEntity::destroyAccelerator() {
	// Destroy only reference
	tree = NULL;
}

bool LevelTileEntity::intersects(Reference<Primitive> &test, Reference<Primitive> &objHit) const {
	if(!worldBound().intersects(test->worldBound()))
		return false;

	if(firstIntersect) {
		firstIntersect = false;
		level->allocateTileTrees(id);
		if(!tree) const_cast<LevelTileEntity*>(this)->buildAccelerator();  // if tree not built, build own tree
	}

	return tree->intersects(test, objHit);
}

} // end FormFactor