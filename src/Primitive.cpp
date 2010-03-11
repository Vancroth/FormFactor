#include "Primitive.h"

namespace FormFactor {

	void Primitive::fullyRefine(std::vector<Reference<Primitive> > &refined) const {
		std::vector<Reference<Primitive> > toRefine;
		toRefine.push_back(const_cast<Primitive *>(this));
		while(toRefine.size()) {
			Reference<Primitive> p = toRefine.back();
			toRefine.pop_back();
			if(p->canIntersect()) {
				refined.push_back(p);
			} else {
				p->refine(toRefine);
			}
		}
	}

	Vector Primitive::calculateVehicleCollision(Vector &v, const Vector &vel, float mass, const Vector &dir) {
		if(dir.y == -1) { // on top of platform
			float newX = (abs(v.x) > .001 ? v.x : vel.x);	// replace z coordinate
			float newZ = vel.z + v.z;						// Add z coordinate
			return Vector(newX, v.y, newZ);	
		} else {
			float d = v.normalize();
			float vDotDir = dir.dot(v);
			if(vDotDir > 0) return -vel * vDotDir * .01; // same direction
			Vector velNew = Vector(vel.x - dir.x * abs(vel.x), vel.y - dir.y * abs(vel.y), vel.z - dir.z * abs(vel.z));
			return velNew + v * d  * (-vDotDir + .01);
		}
	}

	Vector Primitive::handleVehicleCollision(const Vector &vel, float mass, const Vector &dir) {
		Vector v = getVelocity(); 
		return Primitive::calculateVehicleCollision(v, vel, mass, dir);
	}

	Vector Primitive::handleParticleCollision(const Vector &vel, float mass, const Vector &dir) {
		Vector v = getVelocity(); 
		float d = v.normalize();
		float vDotDir = dir.dot(v);
		if(vDotDir > 0) return -vel * vDotDir * .01; // same direction
		Vector velNew = Vector(vel.x - dir.x * abs(vel.x), vel.y - dir.y * abs(vel.y), vel.z - dir.z * abs(vel.z));
		return velNew + v * d  * (-vDotDir + .01);
	}

}