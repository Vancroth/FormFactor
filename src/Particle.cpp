#include "Particle.h"

namespace FormFactor {

unsigned int Particle::numParticles = 0;

Particle::Particle(Ogre::SceneNode *node, char *matName) : PhysicsBody(node) {
	inactive = true;

	char buf[60]; sprintf(buf, "Particle%d", numParticles++);
	particle = mSceneMgr->createBillboardSet(buf, 1);
	particle->setMaterialName(matName);
	particle->setCastShadows(true);
	Ogre::Billboard *particleBillboard = particle->createBillboard(Vector3(0, 0, 0));
	mNode->attachObject(particle);
}

Particle::~Particle() {
	mSceneMgr->destroyBillboardSet(particle);
}

void Particle::initParticle(unsigned int l, const Point &p, const Vector &v, float m, const Vector &newColor, const Vector &newColorDelta,
						float newAlpha, float newAlphaDelta, std::vector<Vector> &forces) {
		this->setPosition(p);
		this->setMass(m);
		this->setVelocity(v);
		color = newColor;
		colorDelta = newColorDelta;
		alpha = newAlpha;
		alphaDelta = newAlphaDelta;
		life = l;
		inactive = false;

		for(unsigned int i = 0; i < forces.size(); i++)
			this->addForce(forces[i]);
}

bool Particle::update(float timeElapse) {
	unsigned int time = (timeElapse > 0 ? int(ceil(timeElapse)) : 1);
	life -= time;

	if(life <= 0) {	// dying
		clearForces();
		inactive = true;
		mNode->setPosition(0, 0, 0);		// reset position
		return true;
	} else {
		color += colorDelta * time;
		alpha += alphaDelta * time;
		particle->getBillboard(0)->setColour(Ogre::ColourValue(color.x, color.y, color.z, alpha));
		return false;
	}
}

BoundingBox Particle::worldBound() const {
	return BoundingBox(particle->getWorldBoundingBox(true));
}

bool Particle::intersects(FormFactor::Reference<FormFactor::Primitive> &other, FormFactor::Reference<FormFactor::Primitive> &objHit) const {
	objHit = const_cast<Particle*>(this);
	if(other->canIntersect()) {
		return worldBound().intersects(other->worldBound());
	} else {
		std::vector<FormFactor::Reference<FormFactor::Primitive> > refined;
		other->fullyRefine(refined);
		for(unsigned int i = 0; i < refined.size(); i++) 
			if(worldBound().intersects(refined[i]->worldBound())) return true;
		return false;
	}
}

void Particle::handleCollision(FormFactor::Reference<FormFactor::Primitive> &objHit, const FormFactor::Vector &dir) {
	FormFactor::Vector newVel = objHit->handleParticleCollision(this->vel, this->mass, dir);
	this->setVelocity(newVel);
}
	
void Particle::updateGraphicalPosition(const FormFactor::Vector &shiftAmt) {
	mNode->translate(shiftAmt.getOgreVector());
}
	
void Particle::clearPhysicsState() {
	// Nothing
}

}