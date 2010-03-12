/*
 * Superclass for a particle emitters.
 */

#pragma once
#include "ParticleEmitter.h"

namespace FormFactor {


class FireEmitter : public ParticleEmitter {

public: 
	FireEmitter(Ogre::SceneNode *node, const Vector &d, char *terrain); 
	virtual ~FireEmitter();

	void setDirection(const Vector &d) {dir = d; dir.normalize();}

	virtual BoundingBox worldBound() const;
	virtual bool intersects(Reference<Primitive> &other, std::vector<Reference<Primitive> > &objsHit, bool sameTest=false) const;


protected:	
	// Functions for emitters to override
	virtual Vector produceInitVelocity() const;
	virtual Point produceInitPosition() const;
	virtual void produceColor(Vector &color, Vector &colorDelta, float *alpha, float *finalAlpha) const;
	virtual void produceDimensions(Vector &dim, Vector &finalDim) const;
	virtual float getParticleMass() const;
	virtual void getForces(std::vector<Vector> &forces) const;
	virtual inline void updatePosition();
	
	Vector dir;			// direction fire shoots
	Ogre::ManualObject *man;

private:
	static const unsigned int numFireParticles;
	static const unsigned int nFireEmits;
	static const unsigned int fireEmitVar;
	static const unsigned int fireLife;
	static const unsigned int fireLifeVar;
	static char* fireMatName;

};

} // end FormFactor
