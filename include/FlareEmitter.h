/*
 * Superclass for a particle emitters.
 */

#pragma once
#include "ParticleEmitter.h"

namespace FormFactor {


class FlareEmitter : public ParticleEmitter {

public: 
	FlareEmitter(Ogre::SceneNode *node1); 

protected:
	
	// Functions for emitters to override
	virtual Vector produceInitVelocity() const;
	virtual Point produceInitPosition() const;
	virtual void produceColor(Vector &color, Vector &colorDelta, float *alpha, float *finalAlpha) const;
	virtual void produceDimensions(Vector &dim, Vector &finalDim) const;
	virtual float getParticleMass() const;
	virtual void getForces(std::vector<Vector> &forces) const;
	virtual inline void updatePosition();

private:

	static const unsigned int numFlareParticles;
	static const unsigned int nFlareEmits;
	static const unsigned int flareEmitVar;
	static const unsigned int flareLife;
	static const unsigned int flareLifeVar;
	static char* flareMatName;


};

} // end FormFactor
