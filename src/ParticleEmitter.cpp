#include "ParticleEmitter.h"

namespace FormFactor {

unsigned int ParticleEmitter::numParticleEmitters = 0;

ParticleEmitter::ParticleEmitter(Ogre::SceneNode *node, unsigned int maxNumParticles, char* matName,
			unsigned int nEmits, unsigned int emitVar, unsigned int avgLifeLen, unsigned int lifeVar) : Primitive(node) {

	pos = this->getPosFromSceneNode(node);

	maxParticles = maxNumParticles;
	emitsPerFrame = nEmits;
	emitVariance = emitVar;
	avgLife = avgLifeLen;
	lifeVariance = lifeVar;

	id = numParticleEmitters++;
	firstTimeOnScreen = true;

	emittedParticles = 0;
	strcpy(material, matName);

	particles = NULL;
	
	/*particles = new ParticleInfo[maxNumParticles];
	for(unsigned int i = 0; i < maxNumParticles; i++) {
		particles[i].p = new Particle(node->createChildSceneNode(), matName);
		deadParticles.push_back(i);
	}*/
}

ParticleEmitter::~ParticleEmitter() {
	if(particles) delete[] particles;
}

unsigned int ParticleEmitter::getNumParticlesToEmit(float timeElapsed) {
		unsigned int time = (timeElapsed > 0 ? int(ceil(timeElapsed)) : 1);
		unsigned int n = abs(float(time * getValueFromVariance(emitsPerFrame, emitVariance)));		// ensure is positive
		unsigned int totalNum = emittedParticles + n;
		int exceed = totalNum - maxParticles;
		n = (exceed < 0 ? n : n - exceed); 
		emittedParticles += n;
		return n;
}

void ParticleEmitter::emitParticles(float timeElapsed) {
	updatePosition();
	float cameraZ = mSceneMgr->getCamera("PlayerCam")->getRealPosition().z;
	if(cameraZ < pos.z || cameraZ > pos.z + 500.f) return;	// only update while on screen; 

	// Allocate particles on demand
	if(firstTimeOnScreen) {
		firstTimeOnScreen = false;	
		particles = new ParticleInfo[maxParticles];
		deadParticles.resize(maxParticles);
		for(unsigned int i = 0; i < maxParticles; i++) {
			particles[i].p = new Particle(mNode->createChildSceneNode(), material);
			deadParticles[i] = i;
		}
	}

	// Update old particles
	for(unsigned int i = 0; i < maxParticles; i++) {
		if(!particles[i].active) continue;

		bool died = particles[i].p->update(timeElapsed, cameraZ);
		if(died) {
			emittedParticles--;
			particles[i].active = false;
			deadParticles.push_back(i);
		}
	}

	// Get num particles to emit
	unsigned int nNewParticles = getNumParticlesToEmit(timeElapsed);

	// Emit new particles
	for(unsigned int i = 0; i < nNewParticles; i++) {
		unsigned int index = deadParticles.front(); deadParticles.pop_front(); // get first unused particle
		initParticle(particles[index].p);
		particles[index].active = true;
	}

}

void ParticleEmitter::initParticle(Reference<Particle> p) {
		unsigned int life = getValueFromVariance(avgLife, lifeVariance);
		float lifeInv = 1.f/life;
		Vector dir = produceInitVelocity();

		Point point = produceInitPosition();
		
		Vector color, finalColor, colorDelta;
		float alpha, finalAlpha, alphaDelta;
		produceColor(color, finalColor, &alpha, &finalAlpha);
		colorDelta = (finalColor - color)*lifeInv;
		alphaDelta = (finalAlpha - alpha)*lifeInv;

		Vector dim, finalDim, dimDelta;
		produceDimensions(dim, finalDim);
		dimDelta = (finalDim - dim)*lifeInv;

		float mass = getParticleMass();
		
		std::vector<Vector> forces;
		getForces(forces);

		p->initParticle(life, point, dir, mass, color, colorDelta, alpha, alphaDelta, dim, dimDelta, forces);
}


} // end FormFactor