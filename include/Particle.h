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

	Particle(Ogre::SceneNode *node, char *matName);

	virtual ~Particle();

	/*
	 * Initialize a particle.
	 */
	void initParticle(unsigned int l, const Point &p, const Vector &v, float m, const Vector &newColor, const Vector &newColorDelta,
						float newAlpha, float newAlphaDelta, std::vector<Vector> &forces);

	/*
	 * Update the particle's state.
	 * Return true if the particles died.
	 */
	bool update(float timeElapse);


	virtual bool frameEvent(const FrameEvent &evt) {return true;}

	// Override Primitve functions
	virtual FormFactor::BoundingBox worldBound() const;
	virtual bool canIntersect() const {return true;} 
	virtual bool intersects(FormFactor::Reference<FormFactor::Primitive> &other, FormFactor::Reference<FormFactor::Primitive> &objHit) const;

	// Override PhysicsBody functions
	virtual void handleCollision(FormFactor::Reference<FormFactor::Primitive> &objHit, const FormFactor::Vector &dir);
	virtual void updateGraphicalPosition(const FormFactor::Vector &amountShifted);
	virtual void clearPhysicsState();

protected:
	Ogre::BillboardSet* particle;		// particle is represented as a billboard

	unsigned int life;			// how long the particle lives
	Vector color, colorDelta;
	float alpha, alphaDelta;

private:
	static unsigned int numParticles;

};

} // end FormFactor