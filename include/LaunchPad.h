#pragma once
#include <Ogre.h>

#include "GameEntity.h"
#include "Primitive.h"
#include "Reference.h"

namespace FormFactor {

class LaunchPad : public Primitive
{
public: 
	LaunchPad(Ogre::SceneNode *node, const Vector &d, float s, char *terrain); 
	virtual ~LaunchPad(); 

	virtual bool frameEvent(const Ogre::FrameEvent &evt) {return true;}

	// Override Primitve functions
	virtual BoundingBox worldBound() const;
	virtual bool canIntersect() const {return true;} 
	virtual bool intersects(Reference<Primitive> &other, std::vector<Reference<Primitive> > &objsHit, bool sameTest=false) const;
	virtual Vector handleVehicleCollision(const Vector &vel, float mass, const Vector &dir);

protected:
	bool activated;
	char terrainTransform[60];
	float speed;
	Vector direction;					// direction to launch
	Ogre::ManualObject *ramp;


private:
	static unsigned int numLaunchPads;
};

} // end FormFactor