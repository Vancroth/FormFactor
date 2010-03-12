#include "LaunchPad.h"

namespace FormFactor {
unsigned int LaunchPad::numLaunchPads = 0;

LaunchPad::LaunchPad(Ogre::SceneNode *node, const Vector &d, float s, char *terrain) : Primitive(node) {
	
	direction = d; direction.normalize();
	speed = s;
	activated = false;

	float a = .4472136f;
	float b = .894427f;
	float c = .57735027;

	char* name;
	name = strchr(terrain, '/');
	if(name) {
		sprintf(terrainTransform, "ActivatedRamp%s", name);
	} else {
		sprintf(terrainTransform, "ActivatedRamp/%s", terrain);
	}

	char buf[60]; sprintf(buf, "LaunchPad%d", numLaunchPads++);
	ramp = mSceneMgr->createManualObject(buf);
	ramp->setCastShadows(true);
	ramp->begin(terrain);
	ramp->position(10, 0, 10); ramp->normal(c, c, c); ramp->textureCoord(0, 0);	// 0
	ramp->position(10, 10, -10); ramp->normal(a, b, 0); ramp->textureCoord(0, 1);
	ramp->position(-10, 10, -10); ramp->normal(-a, b, 0); ramp->textureCoord(1, 1);
	ramp->position(-10, 0, 10); ramp->normal(-c, c, c); ramp->textureCoord(1, 0);
	
	//ramp->position(10, 0, -10); ramp->normal(c, -c, c); ramp->textureCoord(1, 0);	// 4
	//ramp->position(-10, 0, -10); ramp->normal(-c, -c, -c); ramp->textureCoord(0, 0);
	
	ramp->position(10, 0, 10); ramp->normal(c, c, c); ramp->textureCoord(0, 0);	// 0
	ramp->position(10, 0, -10); ramp->normal(c, -c, c); ramp->textureCoord(1, 0);
	ramp->position(10, 10, -10); ramp->normal(a, b, 0); ramp->textureCoord(0, 1);

	ramp->position(-10, 10, -10); ramp->normal(-a, b, 0); ramp->textureCoord(1, 1);
	ramp->position(-10, 0, -10); ramp->normal(-c, -c, -c); ramp->textureCoord(0, 0);
	ramp->position(-10, 0, 10); ramp->normal(-c, c, c); ramp->textureCoord(0, 0);

	ramp->position(10, 10, -10); ramp->normal(a, b, 0); ramp->textureCoord(0, 1);
	ramp->position(10, 0, -10); ramp->normal(c, -c, c); ramp->textureCoord(1, 0);
	ramp->position(-10, 0, -10); ramp->normal(-c, -c, -c); ramp->textureCoord(0, 0);
	ramp->position(-10, 10, -10); ramp->normal(-a, b, 0); ramp->textureCoord(1, 1);

	ramp->position(10, 0, 10); ramp->normal(c, c, c); ramp->textureCoord(0, 0);	// 0
	ramp->position(-10, 0, 10); ramp->normal(-c, c, c); ramp->textureCoord(1, 0);
	ramp->position(-10, 0, -10); ramp->normal(-c, -c, -c); ramp->textureCoord(0, 0);
	ramp->position(10, 0, -10); ramp->normal(c, -c, c); ramp->textureCoord(1, 0);

	ramp->quad(0, 1, 2, 3);
	ramp->triangle(4, 5, 6);
	ramp->triangle(7, 8, 9); 
	ramp->quad(10, 11, 12, 13);
	ramp->quad(14, 15, 16, 17);
	ramp->end();

	mNode->rotate(Ogre::Vector3::UNIT_Z.getRotationTo(-direction.getOgreVector()));
	mNode->attachObject(ramp);
}

LaunchPad::~LaunchPad() {
	mNode->detachAllObjects();
	mSceneMgr->destroyManualObject(ramp);
}


BoundingBox LaunchPad::worldBound() const {
	return BoundingBox(ramp->getWorldBoundingBox(true));
}

Vector LaunchPad::handleVehicleCollision(const Vector &vel, float mass, const Vector &dir) {
		Vector v = getVelocity(); 
		if(activated) {
			return vel;
		} else if(direction==dir) {
			activated = true;
			ramp->setMaterialName(0, terrainTransform);
			Vector launch = direction * 2.f + Vector(0, 1, 0);
			launch.normalize();
			return launch * speed;
		} else if(dir.y == -1) { // on top of platform
			float newX = (abs(v.x) > .001 ? v.x : vel.x);	// replace z coordinate
			float newZ = vel.z + v.z;						// Add z coordinate
			return Vector(newX, v.y, newZ);	
		} else {
			int add = 1;
			float d = v.normalize();
			float vDotDir = dir.dot(v);
			if(vDotDir > 0) add = 0; // same direction. don't add its velocity
			Vector velNew = Vector(vel.x - dir.x * abs(vel.x) * 1.002f, vel.y - dir.y * abs(vel.y) * 1.002f, vel.z - dir.z * abs(vel.z) * 1.002f);
			return velNew + v * add * d  * (-vDotDir + .01);
		}
}


bool LaunchPad::intersects(Reference<Primitive> &other, std::vector<Reference<Primitive> > &objsHit, bool sameTest) const {
	objsHit.push_back(const_cast<LaunchPad*>(this));
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

} // end FormFactor


