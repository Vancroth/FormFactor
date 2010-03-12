#include "SmokeEmitter.h"

namespace FormFactor {
const unsigned int SmokeEmitter::numSmokeParticles = 300;
const unsigned int SmokeEmitter::nSmokeEmits = 30;
const unsigned int SmokeEmitter::smokeEmitVar = 10;
const unsigned int SmokeEmitter::smokeLife = 4;
const unsigned int SmokeEmitter::smokeLifeVar = 3;
char* SmokeEmitter::smokeMatName = "Examples/Smoke";


SmokeEmitter::SmokeEmitter(Ogre::SceneNode *node, const Vector &d, int strength) : ParticleEmitter(node, numSmokeParticles, smokeMatName, nSmokeEmits, 
						smokeEmitVar, smokeLife, smokeLifeVar) {

	dir = d; dir.normalize();
	str = strength;
}

void SmokeEmitter::updatePosition() {
	// Update position
	pos = getPosFromSceneNode(mNode);
}


Vector SmokeEmitter::produceInitVelocity() const {
	int pos = dir.dot(Vector(1,1,1));
	float x = getValueFromVariance(0.f, .05f) * (1.f - dir.x*pos);
	float y = getValueFromVariance(0.f, .3f) * (1.f - dir.y*pos);
	float z = getValueFromVariance(0.f, .4f) * (1.f - dir.z*pos);
	Vector d = dir + Vector(x, y, z);
	d.normalize();
	return d * float(getValueFromVariance(unsigned int(20), unsigned int(10)));
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
	dim.set(1.f, 1.f, 0.f); dim *= str;
	finalDim.set(10.f, 10.f, 0.f);
}
	
float SmokeEmitter::getParticleMass() const {
	return .1f;
}
	
void SmokeEmitter::getForces(std::vector<Vector> &forces) const {
	forces.push_back(-PhysicsBody::gravity.force*1.5f*getParticleMass());
}

} // end FormFactor