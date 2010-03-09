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

	Vector Primitive::handleVehicleCollision(const Vector &vel, float mass, const Vector &dir) {
		Vector v = getVelocity(); 
		if(dir.y == -1) { // on top of platform
			return v;
		} else {
			float d = v.normalize();
			float vDotDir = dir.dot(v);
			if(vDotDir > 0) return -vel * vDotDir * .01; // same direction
			Vector velNew = Vector(vel.x - dir.x * abs(vel.x), vel.y - dir.y * abs(vel.y), vel.z - dir.z * abs(vel.z));
			return velNew + v * d  * (-vDotDir + .01);
		}
	}

}