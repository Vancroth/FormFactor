#include "PowerUp.h"

namespace FormFactor {
unsigned int PowerUp::numPowerUps = 0;

PowerUp::PowerUp(Ogre::SceneNode *node) : Primitive(node) {
	node->setDirection(Ogre::Vector3(0, 0, 1));
	node->scale(.1f, .1f, .1f);
	node->showBoundingBox(true);

	char buf[60]; sprintf(buf, "PowerUp%d", numPowerUps++);
	powerUp = mSceneMgr->createEntity(buf, "knot.mesh");
	mNode->attachObject(powerUp);
}

PowerUp::~PowerUp() {
	mNode->detachAllObjects();
	mSceneMgr->destroyEntity(powerUp);
}

bool PowerUp::frameEvent(const Ogre::FrameEvent &evt) {
	return true;
}

bool PowerUp::keyPressed(const OIS::KeyEvent &evt) {
	return true;
}


BoundingBox PowerUp::worldBound() const {
	return BoundingBox(powerUp->getWorldBoundingBox(true));
}

bool PowerUp::intersects(Reference<Primitive> &other, std::vector<Reference<Primitive> > &objsHit, bool sameTest) const {
	objsHit.push_back(const_cast<PowerUp*>(this));
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


