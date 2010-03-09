#include "MovingPlatform.h"

namespace FormFactor {
unsigned int MovingPlatform::numMovingPlatforms = 0;
const Ogre::Vector3 MovingPlatform::upVector[] = {Vector3::UNIT_Z, Vector3::UNIT_Z, Vector3::UNIT_Y, Vector3::UNIT_Z, Vector3::UNIT_Z, Vector3::UNIT_Y};
const Ogre::Vector3 MovingPlatform::planeDir[] = {Vector3::UNIT_X, Vector3::UNIT_Y, Vector3::UNIT_Z, -Vector3::UNIT_X, -Vector3::UNIT_Y, -Vector3::UNIT_Z};

MovingPlatform::MovingPlatform(Ogre::SceneNode *node, char *matName, const Point  &dest, float speed, int dir,
							   unsigned int platformWidth, unsigned int platformHeight) : Primitive(node) {
	// create a mesh for our MovingPlatform
	MeshManager::getSingleton().createPlane("MovingPlatform", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
		Plane(planeDir[dir], 0), platformWidth, platformHeight, 5, 5, true, 1, 6, 6, upVector[dir]);
		
	// create a MovingPlatform entity from our mesh and attach it to the origin
	char buf[60]; sprintf(buf, "MovingPlatform%d", numMovingPlatforms++);
	movingPlatform = mSceneMgr->createEntity(buf, "MovingPlatform");
	movingPlatform->setMaterialName(matName);
	movingPlatform->setCastShadows(true);
	node->attachObject(movingPlatform);

	direction = dest - Point(0, 0, 0);
	destination = Point(node->getPosition()) + direction;
	movementSpeed = speed;
	pathDistance = distance = direction.normalize();

	movingBounds = unmovingBound().getExpanded(direction);
}

MovingPlatform::~MovingPlatform() {
	mSceneMgr->destroyEntity(movingPlatform);
}

bool MovingPlatform::frameStarted(const FrameEvent& evt) {
	float move = movementSpeed * evt.timeSinceLastFrame;
	distance -= move;

	if(distance <= 0.f) { // at destination
		mNode->setPosition(destination.getOgrePoint());
		distance = pathDistance;
		destination -= direction * pathDistance;	// flip destination
		direction *= -1; // flip direction
	} else {
		Vector dir = direction * move;
		mNode->translate(dir.getOgreVector());
	}
    return true;
}


BoundingBox MovingPlatform::worldBound() const {
	return movingBounds;
}

BoundingBox MovingPlatform::unmovingBound() const {
	return BoundingBox(movingPlatform->getWorldBoundingBox(true));
}

bool MovingPlatform::intersects(FormFactor::Reference<FormFactor::Primitive> &other, FormFactor::Reference<FormFactor::Primitive> &objHit) const {
	objHit = const_cast<MovingPlatform*>(this);
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

/*Vector MovingPlatform::handleVehicleCollision(const Vector &vel, float mass, const Vector &dir) {
	Vector v = direction * movementSpeed; 
	if(dir.y == -1) { // on top of platform
		return v;
	} else {
		float d = v.normalize();
		float vDotDir = dir.dot(v);
		if(vDotDir > 0) return -vel * vDotDir * .01; // same direction
		Vector velNew = Vector(vel.x - dir.x * abs(vel.x), vel.y - dir.y * abs(vel.y), vel.z - dir.z * abs(vel.z));
		return velNew + v * d  * (-vDotDir + .01);
	}
}*/

} // end FormFactor


