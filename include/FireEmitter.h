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

private:
	static const unsigned int numFireParticles;
	static const unsigned int nFireEmits;
	static const unsigned int fireEmitVar;
	static const unsigned int fireLife;
	static const unsigned int fireLifeVar;
	static char* fireMatName;

};

} // end FormFactor
