#include "PhysicsBody.h"

namespace FormFactor {

	const Force PhysicsBody::gravity = Force(Vector(0, -9.8f, 0));
	std::vector<Reference<PhysicsBody> > PhysicsBody::bodies;
	Reference<KdTree> PhysicsBody::tree;

	PhysicsBody::PhysicsBody(Ogre::SceneNode *node, bool collides, float m, const Vector &v) : Primitive(node) {
		canCollide = collides;
		mass = m;
		massInv = 1.f/mass;
		vel = v;

		enableGravity(true);

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
			// Add forces
			Vector totalForce = gravity.force * bodies[i]->gravityOn;
			for(unsigned int j = 0; j < bodies[i]->forces.size(); j++)
				totalForce += bodies[i]->forces[j].force;

			// Obtain acceleration
			Vector accel = totalForce * bodies[i]->massInv;
			
			// Do simple constant acceleration approximation
			Vector newVel = bodies[i]->vel + accel * timeElapse;
			Point newPos = bodies[i]->pos + bodies[i]->vel * timeElapse + accel * .5 * timeElapse * timeElapse;

			Vector amountShifted = newPos - bodies[i]->pos;
			bodies[i]->pos = newPos;
			bodies[i]->vel = newVel;
			bodies[i]->updateGraphicalPosition(amountShifted);
		}

		// Check for/Handle collisions
		for(unsigned int i = 0; i < bodies.size(); i++) {
			bodies[i]->clearPhysicsState();
			if(!bodies[i]->canCollide) continue;

			Reference<Primitive> objHit;
			Reference<Primitive> temp = bodies[i].getPtr();
			if(tree->intersects(temp, objHit)) {
				Vector dir = bodies[i]->worldBound().getIntersectDir(objHit->worldBound());
				bodies[i]->handleCollision(objHit, dir);
			}
		}

	}

	Point PhysicsBody::getPosFromSceneNode(Ogre::SceneNode *node) {
		return Point(node->convertLocalToWorldPosition(node->getPosition()));
	}

} // end FormFactor
