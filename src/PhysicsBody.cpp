#include "PhysicsBody.h"

namespace FormFactor {

	const Force PhysicsBody::gravity = Force(Vector(0, -1.8f, 0));
	std::vector<Reference<PhysicsBody> > PhysicsBody::bodies;
	unsigned int PhysicsBody::reservedCapacity = 500;
	unsigned int PhysicsBody::timeTillNextErase = 50;
	Reference<KdTree> PhysicsBody::tree;

	PhysicsBody::PhysicsBody(Ogre::SceneNode *node, bool collides, float m, bool frame, bool input, const Vector &v) : Primitive(node, frame, input) {

		canCollide = collides;
		mass = m;
		massInv = 1.f/mass;
		vel = v;
		amountShifted = Vector();

		enableGravity(true);
		inactive = false;

		setPosition(getPosFromSceneNode(node));

		if(bodies.capacity() > 3*reservedCapacity/4) {
			bodies.reserve(reservedCapacity);
			reservedCapacity *= 2;
		}
		
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
		float cameraZ = bodies[0]->mSceneMgr->getCamera("PlayerCam")->getRealPosition().z;

		for(unsigned int i = 0; i < bodies.size(); i++) {
			if(!bodies[i] || bodies[i]->inactive || bodies[i]->isOffScreen(cameraZ)) continue;

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
			if(i!=0) {
				if(!bodies[i]) {						// passed it
					continue;
				} else if(bodies[i]->nReferences==1) {	// last reference
					bodies[i] = NULL;
					continue;
				}
			}
			bodies[i]->clearPhysicsState();
			if(!bodies[i]->doesCollision() || bodies[i]->inactive || bodies[i]->isOffScreen(cameraZ)) continue;
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

		// Clear null pointers
		if(--timeTillNextErase == 0) {
			timeTillNextErase = 10;
			std::sort(bodies.begin()+1, bodies.end(), std::greater<Reference<PhysicsBody> >());
			std::vector<Reference<PhysicsBody>>::iterator it = std::find(bodies.begin()+1, bodies.end(), NULL);
			bodies.erase(it, bodies.end());
		}
	}

	Point PhysicsBody::getPosFromSceneNode(Ogre::SceneNode *node) {
		return Point(node->convertLocalToWorldPosition(node->getPosition()));
	}

} // end FormFactor
