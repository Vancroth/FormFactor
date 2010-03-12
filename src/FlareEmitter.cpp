#include "FlareEmitter.h"

namespace FormFactor {
const unsigned int FlareEmitter::numFlareParticles = 100;
const unsigned int FlareEmitter::nFlareEmits = 20;
const unsigned int FlareEmitter::flareEmitVar = 10;
const unsigned int FlareEmitter::flareLife = 7;
const unsigned int FlareEmitter::flareLifeVar = 3;
char* FlareEmitter::flareMatName = "Examples/Flare";


FlareEmitter::FlareEmitter(Ogre::SceneNode *node) : ParticleEmitter(node, numFlareParticles, flareMatName, nFlareEmits, 
						flareEmitVar, flareLife, flareLifeVar) {

}

void FlareEmitter::updatePosition() {
	// Update position
	pos = getPosFromSceneNode(mNode);
}


Vector FlareEmitter::produceInitVelocity() const {
	float x = getValueFromVariance(0.f, 1.f);
	float y = getValueFromVariance(0.f, 1.f);
	float z = getValueFromVariance(0.f, 1.f);
	Vector d = Vector(x, y, z);
	d.normalize();
	return d * float(getValueFromVariance(unsigned int(20), unsigned int(5)));
}

Point FlareEmitter::produceInitPosition() const {
	return this->pos;
}
	
void FlareEmitter::produceColor(Vector &color, Vector &finalColor, float *alpha, float *finalAlpha) const {
	color.set(.6f, .6f, .6f);
	finalColor.set(.1f, .1f, .1f);
	*alpha = .5f;
	*finalAlpha =  0.f;
}

void FlareEmitter::produceDimensions(Vector &dim, Vector &finalDim) const {
	dim.set(10.f, 10.f, 0.f);
	finalDim.set(30.f, 30.f, 0.f);
}
	
float FlareEmitter::getParticleMass() const {
	return .1f;
}
	
void FlareEmitter::getForces(std::vector<Vector> &forces) const {
}

} // end FormFactor