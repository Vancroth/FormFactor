#pragma once
#include <Ogre.h>

#include "GameEntity.h"
#include "Primitive.h"
#include "Reference.h"

namespace FormFactor {

class PowerUp : public Primitive
{
public: 
	PowerUp(Ogre::SceneNode *node); 
	virtual ~PowerUp(); 

	virtual bool frameEvent(const Ogre::FrameEvent &evt);
	virtual bool keyPressed(const OIS::KeyEvent &evt);

	// Override Primitve functions
	virtual BoundingBox worldBound() const;
	virtual bool canIntersect() const {return true;} 
	virtual bool intersects(Reference<Primitive> &other, std::vector<Reference<Primitive> > &objsHit, bool sameTest=false) const;

protected:
	Ogre::Entity *powerUp;

private:
	static unsigned int numPowerUps;
};

} // end FormFactor