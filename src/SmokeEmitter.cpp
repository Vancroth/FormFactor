#include "SmokeEmitter.h"

namespace FormFactor {
const unsigned int SmokeEmitter::numSmokeParticles = 200;
const unsigned int SmokeEmitter::nSmokeEmits = 20;
const unsigned int SmokeEmitter::smokeEmitVar = 10;
const unsigned int SmokeEmitter::smokeLife = 2;
const unsigned int SmokeEmitter::smokeLifeVar = 2;
char* SmokeEmitter::smokeMatName = "Examples/Smoke";


SmokeEmitter::SmokeEmitter(Ogre::SceneNode *node, const Vector &d) : ParticleEmitter(node, numSmokeParticles, smokeMatName, nSmokeEmits, 
						smokeEmitVar, smokeLife, smokeLifeVar) {

	dir = d; dir.normalize();
}

void SmokeEmitter::updatePosition() {
	// Update position
	pos = getPosFromSceneNode(mNode);
}


Vector SmokeEmitter::produceInitVelocity() const {
	float x = getValueFromVariance(0.f, .2f) * (1.f - dir.x);
	float y = getValueFromVariance(0.f, .4f) * (1.f - dir.y);
	float z = getValueFromVariance(0.f, .4f) * (1.f - dir.z);
	Vector d = dir + Vector(x, y, z);
	d.normalize();
	return d * float(getValueFromVariance(unsigned int(40), unsigned int(20)));
}

Point SmokeEmitter::produceInitPosition() const {
	return this->pos;
}
	
void SmokeEmitter::produceColor(Vector &color, Vector &finalColor, float *alpha, float *finalAlpha) const {
	color.set(.8f, .8f, .8f);
	finalColor.set(.1f, .1f, .1f);
	*alpha = .8f;
	*finalAlpha =  0.f;
}

void SmokeEmitter::produceDimensions(Vector &dim, Vector &finalDim) const {
	dim.set(1.f, 1.f, 0.f);
	finalDim.set(8.f, 8.f, 0.f);
}
	
float SmokeEmitter::getParticleMass() const {
	return .1f;
}
	
void SmokeEmitter::getForces(std::vector<Vector> &forces) const {
	forces.push_back(-PhysicsBody::gravity.force*2.f*getParticleMass());
}

} // end FormFactor