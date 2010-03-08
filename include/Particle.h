/*
 * Defines a Particle in 3D.
 */

#pragma once
#include <math.h>

#include "PhysicsBody.h"
#include "Vector.h"
#include "Point.h"

namespace FormFactor {


	class Particle : public PhysicsBody {

public:

	Particle() : PhysicsBody() {}

	Particle(Point p, Vector velocity, float m) : PhysicsBody(true, m, p, v) {}

private:
	int life;			// how long the particle lives

};

} // end FormFactor