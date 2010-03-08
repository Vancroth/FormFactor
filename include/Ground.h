#pragma once
#include <Ogre.h>

#include "GameEntity.h"
#include "Primitive.h"
#include "Reference.h"

namespace FormFactor {

class Ground : public Primitive {
public: 
	Ground(Ogre::SceneNode *node, char *matName, unsigned int tileWidth, unsigned int tileHeight); 
	virtual ~Ground(); 

	virtual bool frameEvent(const Ogre::FrameEvent &evt);
	virtual bool keyPressed(const OIS::KeyEvent &evt);

	// Override Primitve functions
	virtual BoundingBox worldBound() const;
	virtual bool canIntersect() const {return true;} 
	virtual bool intersects(Reference<Primitive> &other, Reference<Primitive> &objHit) const;

protected:
	Ogre::Entity *ground;

private:
	static unsigned int numGrounds;
};

} // end FormFactor