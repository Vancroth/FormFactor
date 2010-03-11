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
						float newAlpha, float newAlphaDelta, const Vector &newDim, const Vector &newDimDelta, std::vector<Vector> &forces, bool collide) {
		this->setPosition(p);
		this->setMass(m);
		this->setVelocity(v);
		dim = newDim;
		dimDelta = newDimDelta;
		color = newColor;
		colorDelta = newColorDelta;
		alpha = newAlpha;
		alphaDelta = newAlphaDelta;
		life = l;
		inactive = false;
		this->canCollide = collide;

		for(unsigned int i = 0; i < forces.size(); i++)
			this->addForce(forces[i]);
}

bool Particle::update(float timeElapse, float cameraZ) {
	unsigned int time = (timeElapse > 0 ? int(ceil(timeElapse)) : 1);
	life -= time;

	bool offscreen = (cameraZ < pos.z || cameraZ > pos.z + 500.f);	// only update while on screen; 

	if(life <= 0 || offscreen) {	// dying
		clearForces();
		inactive = true;
		mNode->setPosition(0, 0, 0);		// reset position
		particle->getBillboard(0)->setColour(Ogre::ColourValue(color.x, color.y, color.z, 0.f));
		particle->getBillboard(0)->setDimensions(0.f, 0.f);
		return true;
	} else {
		color += colorDelta * time;
		alpha += alphaDelta * time;
		dim += dimDelta * time;
		particle->getBillboard(0)->setColour(Ogre::ColourValue(color.x, color.y, color.z, alpha));
		particle->getBillboard(0)->setDimensions(dim[0], dim[1]);
		return false;
	}
}

BoundingBox Particle::worldBound() const {
	return BoundingBox(particle->getWorldBoundingBox(true));
}

bool Particle::intersects(Reference<Primitive> &other, std::vector<Reference<Primitive> > &objsHit, bool sameTest) const {
	objsHit.push_back(const_cast<Particle*>(this));
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