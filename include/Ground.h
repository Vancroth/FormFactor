#pragma once
#include <Ogre.h>

#include "GameEntity.h"
#include "Primitive.h"
#include "Reference.h"

namespace FormFactor {

class Ground : public Primitive {
public: 
	Ground(Ogre::SceneNode *node, char *matName, int dir, unsigned int tileWidth, unsigned int tileHeight); 
	virtual ~Ground(); 

	virtual bool frameEvent(const Ogre::FrameEvent &evt);
	virtual bool keyPressed(const OIS::KeyEvent &evt);

	// Override Primitve functions
	virtual BoundingBox worldBound() const;
	virtual bool canIntersect() const {return true;} 
	virtual bool intersects(Reference<Primitive> &other, std::vector<Reference<Primitive> > &objsHit, bool sameTest = false) const;

protected:
	Ogre::Entity *ground;
	int direction;

private:
	static unsigned int numGrounds;
	static const Ogre::Vector3 upVector[];
	static const Ogre::Vector3 planeDir[];
};

} // end FormFactor