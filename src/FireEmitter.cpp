#include "FireEmitter.h"

#include "SmokeEmitter.h"

namespace FormFactor {
const unsigned int FireEmitter::numFireParticles = 200;
const unsigned int FireEmitter::nFireEmits = 50;
const unsigned int FireEmitter::fireEmitVar = 20;
const unsigned int FireEmitter::fireLife = 4;
const unsigned int FireEmitter::fireLifeVar = 2;
char* FireEmitter::fireMatName = "Fire";


FireEmitter::FireEmitter(Ogre::SceneNode *node, const Vector &d, char *terrain) : ParticleEmitter(node, numFireParticles, fireMatName, nFireEmits, 
						fireEmitVar, fireLife, fireLifeVar) {

	dir = d; dir.normalize();

	float a = .48507125f;
	float b = .7276069f;
	float c = 2.f/3.f;
	float e = 1.f/3.f;

	char buf[60]; sprintf(buf, "FireShooter%d", this->id);
	man = mSceneMgr->createManualObject(buf);
	man->begin(terrain);
	man->position(5, 10, 5); man->normal(a, b, -a); man->textureCoord(1, 1);	// 0
	man->position(5, 10, -5); man->normal(a, b, a); man->textureCoord(1, 0);
	man->position(-5, 10, -5); man->normal(-a, b, a); man->textureCoord(0, 0);
	man->position(-5, 10, 5); man->normal(-a, b, -a); man->textureCoord(0, 1);
	
	man->position(10, 0, 10); man->normal(c, e, -c); man->textureCoord(0, 0);		// 4
	man->position(10, 0, -10); man->normal(c, e, c); man->textureCoord(0, 1);
	man->position(-10, 0, -5); man->normal(-c, e, c); man->textureCoord(1, 1);
	man->position(-10, 0, 10); man->normal(-c, e, -c); man->textureCoord(1, 0);
	
	man->position(5, 10, 5); man->normal(a, b, -a); man->textureCoord(1, 1);	// 8
	man->position(10, 0, 10); man->normal(c, e, -c); man->textureCoord(0, 0);
	man->position(10, 0, -10); man->normal(c, e, c); man->textureCoord(0, 1);
	man->position(5, 10, -5); man->normal(a, b, a); man->textureCoord(1, 0);		

	man->position(5, 10, -5); man->normal(a, b, a); man->textureCoord(1, 0);;	// 12
	man->position(10, 0, -10); man->normal(c, e, c); man->textureCoord(0, 1);
	man->position(-10, 0, -5); man->normal(-c, e, c); man->textureCoord(1, 1);
	man->position(-5, 10, -5); man->normal(-a, b, a); man->textureCoord(0, 0);

	man->position(-5, 10, -5); man->normal(-a, b, a); man->textureCoord(0, 0);	// 16
	man->position(-10, 0, -5); man->normal(-c, e, c); man->textureCoord(1, 1);
	man->position(-10, 0, 10); man->normal(-c, e, -c); man->textureCoord(1, 0);
	man->position(-5, 10, 5); man->normal(-a, b, -a); man->textureCoord(0, 1);

	man->position(5, 10, 5); man->normal(a, b, -a); man->textureCoord(1, 1);	// 20
	man->position(-5, 10, 5); man->normal(-a, b, -a); man->textureCoord(0, 1);
	man->position(-10, 0, 10); man->normal(-c, e, -c); man->textureCoord(1, 0);
	man->position(10, 0, 10); man->normal(c, e, -c); man->textureCoord(0, 0);

	man->quad(0, 1, 2, 3);
	man->quad(7, 6, 5, 4);
	man->quad(8, 9, 10, 11);
	man->quad(12, 13, 14, 15);
	man->quad(16, 17, 18, 19);
	man->quad(20, 21, 22, 23);
	man->end();


	mNode->rotate(Ogre::Vector3::UNIT_Y.getRotationTo(dir.getOgreVector()));
	mNode->attachObject(man);

}

FireEmitter::~FireEmitter() {
	mNode->detachAllObjects();
}

BoundingBox FireEmitter::worldBound() const {
	BoundingBox box(man->getWorldBoundingBox(true));
	return box.getExpanded(dir * 15.f);
}

bool FireEmitter::intersects(Reference<Primitive> &other, std::vector<Reference<Primitive> > &objsHit, bool sameTest) const {
	objsHit.push_back(const_cast<FireEmitter*>(this));
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


void FireEmitter::updatePosition() {
	// Update position
	pos = getPosFromSceneNode(mNode) - dir*3.f;
}


Vector FireEmitter::produceInitVelocity() const {
	int pos = dir.dot(Vector(1,1,1));
	float x = getValueFromVariance(0.f, .4f) * (1.f - dir.x*pos);
	float y = getValueFromVariance(0.f, .4f) * (1.f - dir.y*pos);
	float z = getValueFromVariance(0.f, .4f) * (1.f - dir.z*pos);
	Vector d = dir + Vector(x, y, z);
	d.normalize();
	return d * float(getValueFromVariance(unsigned int(10), unsigned int(10)));
}

Point FireEmitter::produceInitPosition() const {
	return this->pos + dir*5.f;;
}
	
void FireEmitter::produceColor(Vector &color, Vector &finalColor, float *alpha, float *finalAlpha) const {
	color.set(1.f, .7f, .2f);
	finalColor.set(1.0f, .4f, 0.f);
	*alpha = 1.f;
	*finalAlpha =  .7f;
}

void FireEmitter::produceDimensions(Vector &dim, Vector &finalDim) const {
	dim.set(7.f, 7.f, 0.f);
	finalDim.set(1.f, 1.f, 0.f);
}
	
float FireEmitter::getParticleMass() const {
	return 5.f;
}
	
void FireEmitter::getForces(std::vector<Vector> &forces) const {
	//forces.push_back(-PhysicsBody::gravity.force*1.5f*getParticleMass());
}

} // end FormFactor