#include "LevelTileEntity.h"
#include "LevelEntity.h"
#include "LevelTilePrims.h"
#include "VehicleEntity.h"

namespace FormFactor {
	const unsigned int LevelTileEntity::TILE_WIDTH =  50;
	const unsigned int LevelTileEntity::TILE_HEIGHT = 100;

	char* terrains[] = {"Examples/GrassFloor", "Examples/BeachStones", "LevelTiles/Lava"};

LevelTileEntity::LevelTileEntity(SceneNode *node, Reference<LevelEntity> l, unsigned int tileID) : Primitive(node)
{
	id = tileID;
	firstIntersect = true;
	level = l;

	// Procedurally build tile and add Primitives to prims

	int randTerrain = rand() % 3;
	if (tileID == 0) { randTerrain = 0; }
	switch (randTerrain) {
		case 0: curTerrain = GRASS; break;
		case 1: curTerrain = METAL; break;
		case 2: curTerrain = LAVA; break;
	}
	char *terrain = terrains[randTerrain];
		
	char buf[60];
	//// Create power ups
	//char buf[60]; sprintf(buf, "PowerUp1-%d", id);
	//SceneNode *powerUpNode = node->createChildSceneNode(buf, Vector3(-20, 0, 0));
	//PowerUp *powerUp = new PowerUp(powerUpNode);
	//powerUp->start();
	//prims.push_back(powerUp);

/*	memset(buf, 0, 60); sprintf(buf, "PowerUp2-%d", id);
	powerUpNode = node->createChildSceneNode(buf, Vector3(0, 10, -200));
	powerUp = new FormFactor::PowerUp(powerUpNode);
	powerUp->start();
	prims.push_back(powerUp);*/

	makeRoom(node, prims, terrain);

	//memset(buf, 0, 60); sprintf(buf, "Platform-%d", id);
	//SceneNode *platformNode = node->createChildSceneNode(buf, Vector3(20, 20, 0));
	//MovingPlatform *platform = new MovingPlatform(platformNode, terrain, Point(0, 0, 30), 2.f, PlaneDirection::negZ, 10, 10);
	//platform->start();
	//prims.push_back(platform);

	// Make bounding box encompass all prims
	for(unsigned int i = 0; i < prims.size(); i++)
		bounds = bounds.getUnion(prims[i]->worldBound());
}

LevelTileEntity::~LevelTileEntity(void)
{
}

LevelTileEntity::TerrainType LevelTileEntity::getTerrainType()
{
	return this->curTerrain;
}

void LevelTileEntity::makeRoom(SceneNode *node, std::vector<Reference<Primitive> > &prims, char* terrain) {
	float height = 2*TILE_WIDTH/3.f;

	// floor
	char buf[60]; sprintf(buf, "Ground-%d", id);
	SceneNode *groundNode = node->createChildSceneNode(buf, Vector3(0, 0, 0));
	FormFactor::Ground *ground = new FormFactor::Ground(groundNode, terrain, PlaneDirection::posY, TILE_WIDTH, TILE_HEIGHT);
	ground->start();
	prims.push_back(ground);

	//// ceiling
	//memset(buf, 0, 60); sprintf(buf, "Ceiling-%d", id);
	//SceneNode *ceilingNode = node->createChildSceneNode(buf, Vector3(0, height, 0));
	//ground = new FormFactor::Ground(ceilingNode, terrain, PlaneDirection::negY, TILE_WIDTH, TILE_HEIGHT);
	//ground->start();
	//prims.push_back(ground);

	//// walls
	//memset(buf, 0, 60); sprintf(buf, "Wall0-%d", id);
	//SceneNode *wallNode = node->createChildSceneNode(buf, Vector3(-float(TILE_WIDTH)*.5f, height*.5f, 0));
	//ground = new FormFactor::Ground(wallNode, terrain, PlaneDirection::posX, height, TILE_HEIGHT);
	//ground->start();
	//prims.push_back(ground);

	//memset(buf, 0, 60); sprintf(buf, "Wall1-%d", id);
	//wallNode = node->createChildSceneNode(buf, Vector3(TILE_WIDTH*.5f, height*.5, 0));
	//ground = new FormFactor::Ground(wallNode, terrain, PlaneDirection::negX, height, TILE_HEIGHT);
	//ground->start();
	//prims.push_back(ground);

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
	prims.clear();		// clear in case still full
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

Vector LevelTileEntity::handleVehicleCollision(const Vector &vel, float mass, const Vector &dir) {
	VehicleEntity *vehicle = VehicleEntity::getSingletonPtr();
	vehicle->collideWithLevelTile(this);

	//switch (curTerrain) {
	//	case LAVA:
	//		if (vehicle->isOnGround() && vehicle->getVehicleMode() != VehicleEntity::TANK) {
	//			vehicle->destroy();
	//		}
	//		break;
	//}

	Vector v = getVelocity(); 
	return Primitive::calculateVehicleCollision(v, vel, mass, dir);
}


} // end FormFactor