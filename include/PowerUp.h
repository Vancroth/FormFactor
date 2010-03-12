#pragma once
#include <Ogre.h>

#include "GameEntity.h"
#include "Primitive.h"
#include "Reference.h"

namespace FormFactor {

class PowerUp : public Primitive
{
public: 
	PowerUp(Ogre::SceneNode *node, std::vector<Point> &points, float speed); 
	virtual ~PowerUp(); 

	BoundingBox unmovingBound() const;

	virtual bool frameEvent(const Ogre::FrameEvent &evt) {return true;}
	virtual bool keyPressed(const OIS::KeyEvent &evt) {return true;}
	virtual bool frameStarted(const FrameEvent& evt);

	// Override Primitve functions
	virtual BoundingBox worldBound() const;
	virtual bool canIntersect() const {return true;} 
	virtual bool intersects(Reference<Primitive> &other, std::vector<Reference<Primitive> > &objsHit, bool sameTest=false) const;

protected:
	Ogre::Entity *powerUp;
	unsigned int dest;
	Vector direction;
	float distance, speed;
	std::vector<Point> points;
	BoundingBox movingBounds;

private:
	static unsigned int numPowerUps;
};

} // end FormFactor