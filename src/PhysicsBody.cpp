#include "PhysicsBody.h"

namespace FormFactor {

	const Force PhysicsBody::gravity = Force(Vector(0, -1.8f, 0));
	std::vector<Reference<PhysicsBody> > PhysicsBody::bodies;
	Reference<KdTree> PhysicsBody::tree;

	PhysicsBody::PhysicsBody(Ogre::SceneNode *node, bool collides, float m, const Vector &v) : Primitive(node) {
		canCollide = collides;
		mass = m;
		massInv = 1.f/mass;
		vel = v;
		amountShifted = Vector();

		enableGravity(true);
		inactive = false;

		setPosition(getPosFromSceneNode(node));
		
		bodies.push_back(this);
	}

	void PhysicsBody::setMass(float m) {
		mass = m;
		massInv = 1.f/mass;
	}

	void PhysicsBody::setPosition(const Point &p) {
		pos = p;
	}

	void PhysicsBody::setVelocity(const Vector &v) {
		vel = v;
	}

	void PhysicsBody::setVelocityX(const float x) {
		vel.x = x;
	}

	void PhysicsBody::setVelocityY(const float y) {
		vel.y = y;
	}

	void PhysicsBody::setVelocityZ(const float z) {
		vel.z = z;
	}

	void PhysicsBody::addForce(const Vector &force) {
		forces.push_back(Force(force));
	}
		
	void PhysicsBody::addForce(const Vector &force, const Point &p)  {
		forces.push_back(Force(force, p, true));
	}

	void PhysicsBody::simulatePhysics(int timeElapseMS) {
		timeElapseMS = (timeElapseMS==0 ? 1 : timeElapseMS);
		float timeElapse = .005 * timeElapseMS;

		for(unsigned int i = 0; i < bodies.size(); i++) {
			if(bodies[i]->inactive) continue;
			// Add forces
			Vector totalForce = gravity.force * bodies[i]->mass * bodies[i]->gravityOn;
			for(unsigned int j = 0; j < bodies[i]->forces.size(); j++)
				totalForce += bodies[i]->forces[j].force;

			// Obtain acceleration
			Vector accel = totalForce * bodies[i]->massInv;
			
			// Do simple constant acceleration approximation
			Vector newVel = bodies[i]->vel + accel * timeElapse;
			if(newVel.y < 0)
				int y = 0;
			Point newPos = bodies[i]->pos + bodies[i]->vel * timeElapse + accel * .5 * timeElapse * timeElapse;

			Vector amountShifted = newPos - bodies[i]->pos;
			bodies[i]->pos = newPos;
			bodies[i]->vel = newVel;
			bodies[i]->amountShifted = amountShifted;		// store for collision detection
			bodies[i]->updateGraphicalPosition(amountShifted);
		}

		// Check for/Handle collisions
		for(unsigned int i = 0; i < bodies.size(); i++) {
			bodies[i]->clearPhysicsState();
			if(!bodies[i]->doesCollision() || bodies[i]->inactive) continue;
			std::vector<Reference<Primitive> > objsHit;
			Reference<Primitive> temp = bodies[i].getPtr();
			
			Vector shiftAmt = bodies[i]->getAmtShifted();
			float dist = shiftAmt.length();
			FormFactor::BoundingBox box = bodies[i]->worldBound();
			int axis = box.getShortestAxis();
			unsigned int nIterations = (dist/(box.getMaxPoint()[axis] - box.getOrigin()[axis])) + 1;
			nIterations *= 4;
			bodies[i]->updateGraphicalPosition(-shiftAmt);		// return to original loc
			bodies[i]->pos -= shiftAmt;
			
			// Do multiple checks
			shiftAmt /= nIterations;
			bodies[i]->updateGraphicalPosition(shiftAmt);		// make 1 initial step
			bodies[i]->pos += shiftAmt;

			for(unsigned int j = 0; j < nIterations; j++) {
				if(tree->intersects(temp, objsHit)) {
					for(unsigned int k = 0; k < objsHit.size(); k++) {
						Vector dir = bodies[i]->worldBound().getIntersectDir(objsHit[k]->worldBound());
						bodies[i]->handleCollision(objsHit[k], dir);
					}
					break;
				}
				bodies[i]->updateGraphicalPosition(shiftAmt);		// update pos
				bodies[i]->pos += shiftAmt;
			}
		}
	}

	Point PhysicsBody::getPosFromSceneNode(Ogre::SceneNode *node) {
		return Point(node->convertLocalToWorldPosition(node->getPosition()));
	}

} // end FormFactor
