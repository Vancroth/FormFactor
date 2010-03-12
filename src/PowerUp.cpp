#include "PowerUp.h"

namespace FormFactor {
unsigned int PowerUp::numPowerUps = 0;

PowerUp::PowerUp(Ogre::SceneNode *node, std::vector<Point> &points, float speed) : Primitive(node) {
	node->setDirection(Ogre::Vector3(0, 0, 1));
	node->scale(.1f, .1f, .1f);
	this->points = points;
	this->speed = speed;

	char buf[60]; sprintf(buf, "PowerUp%d", numPowerUps++);
	powerUp = mSceneMgr->createEntity(buf, "knot.mesh");
	powerUp->setCastShadows(true);
	mNode->attachObject(powerUp);

	// Get full extent of bounds
	movingBounds = BoundingBox();
	for(unsigned int i = 0; i < points.size(); i++) {
		Vector dir = points[i] - Point(0, 0, 0);
		movingBounds = movingBounds.getExpanded(dir);
	}
	
	this->points.push_back(Point(0, 0, 0));
	dest = 0;
	direction = points[dest] - Point(0, 0, 0);
	distance = direction.normalize();

}

PowerUp::~PowerUp() {
	mNode->detachAllObjects();
	mSceneMgr->destroyEntity(powerUp);
}


bool PowerUp::frameStarted(const FrameEvent& evt) {
	float move = speed * evt.timeSinceLastFrame;
	distance -= move;

	if(distance <= 0.f) { // at destination
		mNode->setPosition(points[dest].getOgrePoint());
		int startLoc = dest;
		dest = (++dest) % points.size();
		direction = points[dest] - points[startLoc];
		distance = direction.normalize();
	} else {
		Vector dir = direction * move;
		mNode->translate(dir.getOgreVector());
	}
    return true;
}


BoundingBox PowerUp::worldBound() const {
	return movingBounds;
}

BoundingBox PowerUp::unmovingBound() const {
	return BoundingBox(powerUp->getWorldBoundingBox(true));
}

bool PowerUp::intersects(Reference<Primitive> &other, std::vector<Reference<Primitive> > &objsHit, bool sameTest) const {
	objsHit.push_back(const_cast<PowerUp*>(this));
	if(other->canIntersect()) {
		return unmovingBound().intersects(other->worldBound());
	} else {
		std::vector<FormFactor::Reference<FormFactor::Primitive> > refined;
		other->fullyRefine(refined);
		for(unsigned int i = 0; i < refined.size(); i++) 
			if(unmovingBound().intersects(refined[i]->worldBound())) return true;
		return false;
	}
}

} // end FormFactor


