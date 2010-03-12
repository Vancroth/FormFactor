#include "LevelTileEntity.h"
#include "LevelEntity.h"

#include "LevelTilePrims.h"

namespace FormFactor {
	const unsigned int LevelTileEntity::TILE_WIDTH =  400;
	const unsigned int LevelTileEntity::TILE_HEIGHT = 400;

	char* terrains[] = {"Examples/GrassFloor", "Examples/BeachStones", "LevelTiles/Lava"};

LevelTileEntity::LevelTileEntity(SceneNode *node, Reference<LevelEntity> l, unsigned int tileID) : Primitive(node, true)
{
	id = tileID;
	firstIntersect = true;
	level = l;

	// Procedurally build tile and add Primitives to prims

	char *terrain = terrains[rand() % 3];
		
	makeRoom(node, prims, terrain);

	char buf[60]; sprintf(buf, "Platform-%d", id);
	SceneNode *platformNode = node->createChildSceneNode(buf, Vector3(20, 20, 0));
	MovingPlatform *platform = new MovingPlatform(platformNode, terrain, Point(0, 0, 30), 2.f, PlaneDirection::negZ, 10, 10);
	platform->start();
	prims.push_back(platform);

	//memset(buf, 0, 60); sprintf(buf, "Volcano-%d", id);
	//SceneNode *rampNode = node->createChildSceneNode(buf, Vector3(0, 20, -40));
	//FlareEmitter *volc = new FlareEmitter(rampNode);
	//FireEmitter *volc = new FireEmitter(groundNode, Vector(0, 1, 0), terrain);
	//LaunchPad *volc = new LaunchPad(rampNode, Vector(0, 0, -1), 50, terrain);
	//volc->start();
	//prims.push_back(volc);

	std::vector<Point> pts;
	pts.push_back(Point(-50, 0, 0));  pts.push_back(Point(0, 50, -50)); 
	memset(buf, 0, 60); sprintf(buf, "PowerUp-%d", id);
	SceneNode *powerNode = node->createChildSceneNode(buf, Vector3(20, 0, -100));
	PowerUp *powerUp = new PowerUp(powerNode, pts, 30.f);
	powerUp->start();
	prims.push_back(powerUp);

	// Make bounding box encompass all prims
	for(unsigned int i = 0; i < prims.size(); i++)
		bounds = bounds.getUnion(prims[i]->worldBound());
}

LevelTileEntity::~LevelTileEntity(void)
{
}

void LevelTileEntity::makeRoom(SceneNode *node, std::vector<Reference<Primitive> > &prims, char* terrain) {
	float height = 2*TILE_WIDTH/3.f;

	// floor
	char buf[60]; sprintf(buf, "Ground-%d", id);
	SceneNode *groundNode = node->createChildSceneNode(buf, Vector3(0, 0, 0));
	FormFactor::Ground *ground = new FormFactor::Ground(groundNode, terrain, PlaneDirection::posY, TILE_WIDTH, TILE_HEIGHT);
	ground->start();
	prims.push_back(ground);



	// ceiling
	memset(buf, 0, 60); sprintf(buf, "Ceiling-%d", id);
	SceneNode *ceilingNode = node->createChildSceneNode(buf, Vector3(0, height, 0));
	ground = new FormFactor::Ground(ceilingNode, terrain, PlaneDirection::negY, TILE_WIDTH, TILE_HEIGHT);
	ground->start();
	prims.push_back(ground);

	// walls
	memset(buf, 0, 60); sprintf(buf, "Wall0-%d", id);
	SceneNode *wallNode = node->createChildSceneNode(buf, Vector3(-float(TILE_WIDTH)*.5f, height*.5f, 0));
	ground = new FormFactor::Ground(wallNode, terrain, PlaneDirection::posX, height, TILE_HEIGHT);
	ground->start();
	prims.push_back(ground);

	memset(buf, 0, 60); sprintf(buf, "Wall1-%d", id);
	wallNode = node->createChildSceneNode(buf, Vector3(TILE_WIDTH*.5f, height*.5, 0));
	ground = new FormFactor::Ground(wallNode, terrain, PlaneDirection::negX, height, TILE_HEIGHT);
	ground->start();
	prims.push_back(ground);

}

bool LevelTileEntity::frameEvent(const FrameEvent &evt) {
	return true;
}

bool LevelTileEntity::frameStarted(const FrameEvent &evt) {
	bool keepGoing = true;
	for(unsigned int i = 0; i < prims.size(); i++)
		keepGoing = keepGoing && prims[i]->frameStarted(evt);
	return keepGoing;
}

void LevelTileEntity::buildAccelerator() {
	tree = new KdTree(prims, 40, 1, .5f, 4, 20);

	// Now tree is built, can clear prims
	//prims.clear();
}

void LevelTileEntity::destroyAccelerator() {
	// Destroy only reference
	tree = NULL;
	prims.clear();		// clear in case still full
}

bool LevelTileEntity::intersects(Reference<Primitive> &test, std::vector<Reference<Primitive> > &objsHit, bool sameTest) const {
	if(!worldBound().intersects(test->worldBound()))
		return false;

	if(firstIntersect) {
		firstIntersect = false;
		level->allocateTileTrees(id);
		if(!tree) const_cast<LevelTileEntity*>(this)->buildAccelerator();  // if tree not built, build own tree
	}

	return tree && tree->intersects(test, objsHit, true);
}

} // end FormFactor