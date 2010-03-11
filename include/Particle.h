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
						float newAlpha, float newAlphaDelta, const Vector &newDim, const Vector &newDimDela, std::vector<Vector> &forces, bool collide=false);

	/*
	 * Update the particle's state.
	 * Return true if the particles died.
	 */
	bool update(float timeElapse, float cameraZ);

	// Override Primitve functions
	virtual FormFactor::BoundingBox worldBound() const;
	virtual bool canIntersect() const {return true;} 
	virtual bool intersects(Reference<Primitive> &other, std::vector<Reference<Primitive> > &objsHit, bool sameTest=false) const;

	// Override PhysicsBody functions
	virtual void handleCollision(FormFactor::Reference<FormFactor::Primitive> &objHit, const FormFactor::Vector &dir);
	virtual void updateGraphicalPosition(const FormFactor::Vector &shiftAmt);
	virtual void clearPhysicsState();
	virtual inline bool doesCollision() {return canCollide;}

protected:
	Ogre::BillboardSet* particle;		// particle is represented as a billboard

	unsigned int life;			// how long the particle lives
	Vector color, colorDelta;
	Vector dim, dimDelta;
	float alpha, alphaDelta;

private:
	static unsigned int numParticles;
};

} // end FormFactor