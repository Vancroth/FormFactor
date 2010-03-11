/*
 * Superclass for a particle emitters.
 */

#pragma once
#include "ParticleEmitter.h"

namespace FormFactor {


class SmokeEmitter : public ParticleEmitter {

public: 
	SmokeEmitter(Ogre::SceneNode *node, const Vector &d); 
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
	
	Vector dir;			// direction smoke shoots

private:
	static const unsigned int numSmokeParticles;
	static const unsigned int nSmokeEmits;
	static const unsigned int smokeEmitVar;
	static const unsigned int smokeLife;
	static const unsigned int smokeLifeVar;
	static char* smokeMatName;

};

} // end FormFactor
