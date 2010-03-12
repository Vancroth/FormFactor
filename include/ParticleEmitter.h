/*
 * Superclass for a particle emitters.
 */

#pragma once
#include <Ogre.h>

#include "Primitive.h"
#include "Particle.h"
#include "Point.h"
#include "Vector.h"

namespace FormFactor {

	// Info on Particle state
	struct ParticleInfo {
		ParticleInfo() {
			active = false;
			p = NULL;
		}

		bool active;
		Reference<Particle> p;
	};

class ParticleEmitter : public Primitive {

public: 
	ParticleEmitter(Ogre::SceneNode *node, unsigned int maxNumParticles, char* matName,
			unsigned int nEmits, unsigned int emitVar, unsigned int avgLifeLen, unsigned int lifeVar); 

	virtual ~ParticleEmitter();

	virtual bool frameStarted(const FrameEvent& evt) {emitParticles(evt.timeSinceLastFrame); return true;}
	virtual bool frameEvent(const Ogre::FrameEvent &evt) {return true;}
	virtual bool keyPressed(const OIS::KeyEvent &evt) {return true;}

	// Override Primitve functions
	virtual BoundingBox worldBound() const {return BoundingBox();}
	virtual bool canIntersect() const {return true;} 
	virtual bool intersects(Reference<Primitive> &other, std::vector<Reference<Primitive> > &objsHit, bool sameTest=false) const {return false;};

protected:
	
	// Functions for emitters to override
	virtual Vector produceInitVelocity() const = 0;
	virtual Point produceInitPosition() const = 0;
	virtual void produceColor(Vector &color, Vector &colorDelta, float *alpha, float *finalAlpha) const = 0;
	virtual void produceDimensions(Vector &dim, Vector &finalDim) const = 0;
	virtual float getParticleMass() const = 0;
	virtual void getForces(std::vector<Vector> &forces) const = 0;
	
	// Moving emitters can use this to update position if neccessary
	virtual inline void updatePosition() {}

	// Emit particles
	inline void emitParticles(float timeElapsed);

	// get num particles to emit and update emmitedParticles
	virtual unsigned int getNumParticlesToEmit(float timeElapsed);

	// Initiates a particle
	void initParticle(Reference<Particle> p);

	// retrieve value from avg & variance
	inline int getValueFromVariance(unsigned int avg, unsigned int var) const {
		return int( avg + (var * float(rand() % 3 - 1)) );
	}

	inline float getValueFromVariance(float avg, float var) const {
		return avg + (var * float(rand() % 3 - 1));
	}

	ParticleInfo *particles;
	std::deque<unsigned int> deadParticles;			// queue indexes of unused particle
	unsigned int emittedParticles, maxParticles;
	unsigned int emitsPerFrame, emitVariance;		// num particles emitted per frame
	unsigned int avgLife, lifeVariance;				// life of emitted particle

	Point pos;
	unsigned int id;				// emitter id
	char material[30];

	static unsigned int numParticleEmitters;
	
	static inline Point ParticleEmitter::getPosFromSceneNode(Ogre::SceneNode *node) {
		return Point(node->convertLocalToWorldPosition(node->getPosition()));
	}

private:
	bool firstTimeOnScreen;

};

} // end FormFactor
