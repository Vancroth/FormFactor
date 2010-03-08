#include "Ground.h"

namespace FormFactor {
unsigned int Ground::numGrounds = 0;

Ground::Ground(Ogre::SceneNode *node, char *matName, unsigned int tileWidth, unsigned int tileHeight) : Primitive(node) {
	// create a mesh for our ground
	MeshManager::getSingleton().createPlane("ground", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
		Plane(Vector3::UNIT_Y, 0), tileWidth, tileHeight, 20, 20, true, 1, 6, 6, Vector3::UNIT_Z);
		
	// create a ground entity from our mesh and attach it to the origin
	char buf[60]; sprintf(buf, "Ground%d", numGrounds++);
	ground = mSceneMgr->createEntity(buf, "ground");
	ground->setMaterialName(matName);
	ground->setCastShadows(false);
	node->attachObject(ground);
}

Ground::~Ground() {
	mSceneMgr->destroyEntity(ground);
}

bool Ground::frameEvent(const Ogre::FrameEvent &evt) {
	return true;
}

bool Ground::keyPressed(const OIS::KeyEvent &evt) {
	return true;
}


BoundingBox Ground::worldBound() const {
	return BoundingBox(ground->getWorldBoundingBox(true));
}

bool Ground::intersects(FormFactor::Reference<FormFactor::Primitive> &other, FormFactor::Reference<FormFactor::Primitive> &objHit) const {
	objHit = const_cast<Ground*>(this);
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


