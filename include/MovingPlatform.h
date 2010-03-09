#pragma once
#include <Ogre.h>

#include "GameEntity.h"
#include "Primitive.h"
#include "Reference.h"

namespace FormFactor {

class MovingPlatform : public Primitive
{
public: 
	/*
	 * Create a moving platform.
	 *
	 * dest is relative to platform's coordinate system.
	 * dir: x==0, y==1, z==2
	 */
	MovingPlatform(Ogre::SceneNode *node, char *matName, const Point  &dest, float speed, int dir,
							   unsigned int platformWidth, unsigned int platformHeight); 
	virtual ~MovingPlatform(); 

	virtual bool frameEvent(const FrameEvent &evt) {return true;}
	virtual bool MovingPlatform::frameStarted(const FrameEvent& evt);

	virtual BoundingBox unmovingBound() const;

	// Override Primitve functions
	virtual BoundingBox worldBound() const;
	virtual bool canIntersect() const {return true;} 
	virtual bool intersects(Reference<Primitive> &other, Reference<Primitive> &objHit) const;
	virtual Vector getVelocity() const {return direction * movementSpeed;}


protected:
	Ogre::Entity *movingPlatform;

	Vector direction;
	Point destination;
	float movementSpeed, pathDistance, distance;

private:
	static unsigned int numMovingPlatforms;
	BoundingBox movingBounds;

	static const Ogre::Vector3 upVector[];
	static const Ogre::Vector3 planeDir[];

};

} // end FormFactor