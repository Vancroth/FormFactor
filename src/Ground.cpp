#include "Ground.h"

namespace FormFactor {
unsigned int Ground::numGrounds = 0;
const Ogre::Vector3 Ground::upVector[] = {Vector3::UNIT_Z, Vector3::UNIT_Z, Vector3::UNIT_Y, Vector3::UNIT_Z, Vector3::UNIT_Z, Vector3::UNIT_Y};
const Ogre::Vector3 Ground::planeDir[] = {Vector3::UNIT_X, Vector3::UNIT_Y, Vector3::UNIT_Z, -Vector3::UNIT_X, -Vector3::UNIT_Y, -Vector3::UNIT_Z};

Ground::Ground(Ogre::SceneNode *node, char *matName, int dir, unsigned int tileWidth, unsigned int tileHeight) : Primitive(node) {
	// create a mesh for our ground
	char buf[60]; sprintf(buf, "ground%d", numGrounds++);
	MeshManager::getSingleton().createPlane(buf, ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
		Plane(planeDir[dir], 0), tileWidth, tileHeight, 20, 20, true, 1, 6, 6, upVector[dir]);
	direction = dir;	

	// create a ground entity from our mesh and attach it to the origin
	char buf2[60]; sprintf(buf2, "Ground%d", numGrounds++);
	ground = mSceneMgr->createEntity(buf2, buf);
	ground->setMaterialName(matName);
	ground->setCastShadows(false);
	node->attachObject(ground);
}

Ground::~Ground() {
	mSceneMgr->destroyEntity(ground);
}

BoundingBox Ground::worldBound() const {
	return BoundingBox(ground->getWorldBoundingBox(true)).getExpanded(planeDir[direction] * -10.f);
}

bool Ground::intersects(Reference<Primitive> &other, std::vector<Reference<Primitive> > &objsHit, bool sameTest) const {
	objsHit.push_back(const_cast<Ground*>(this));
	if(other->canIntersect()) {
		return worldBound().intersects(other->worldBound());
	} else {
		std::vector<FormFactor::Reference<FormFactor::Primitive> > refined;
		other->fullyRefine(refined);
		for(unsigned int i = 0; i < refined.size(); i++) 
			if(worldBound().intersects(refined[i]->worldBound())) return true;
		return false;
	}
}

} // end FormFactor


