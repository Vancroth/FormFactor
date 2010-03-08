#include "PhysicsBody.h"

namespace FormFactor {

	const float PhysicsBody::simStep = .001f;	// 1ms time step

	std::vector<Reference<PhysicsBody> > PhysicsBody::bodies;
	dWorldID PhysicsBody::world;
	Reference<KdTree> PhysicsBody::tree;

	PhysicsBody::PhysicsBody(Ogre::SceneNode *node, bool collides, const Vector &v) : Primitive(node) {
		canCollide = collides;
		body = dBodyCreate(world);

		//setMass(m, type);
		setPosition(getPosFromSceneNode(node));
		setVelocity(v);

		bodies.push_back(this);
	}

	void PhysicsBody::setMass(float m) {
		BoundingBox box;
		setMass(m, BodyType::PointMass, box);
	}

	void PhysicsBody::setMass(float m, BodyType type, const BoundingBox &box) {
		mass = m;
		dMass massStruct; 
		int shortestAxis = box.getShortestAxis();
		int longestAxis = box.getLongestAxis();
		int otherAxes[3][3] =  {{-1, 2, 1}, {2, -1, 0}, {1, 0, -1}};
		int otherAxis = otherAxes[longestAxis][shortestAxis];

		float shortestLength, longestLength, otherLength, xLength, yLength, zLength;
		switch(type) {
			case Sphere:
				shortestLength = box.getMaxPoint()[shortestAxis] - box.getOrgin()[shortestAxis];
				dMassSetSphereTotal(&massStruct, mass, .5*shortestLength);
				break;
			case Cylinder:
				shortestLength = box.getMaxPoint()[shortestAxis] - box.getOrgin()[shortestAxis];
				longestLength = box.getMaxPoint()[longestAxis] - box.getOrgin()[longestAxis];
				dMassSetCappedCylinderTotal(&massStruct, mass, longestAxis, .5*shortestLength, longestLength);
				break;
			case Box:
				xLength = box.getMaxPoint()[0] - box.getOrgin()[0];
				yLength = box.getMaxPoint()[1] - box.getOrgin()[1];
				zLength = box.getMaxPoint()[2] - box.getOrgin()[2];
				dMassSetBoxTotal(&massStruct, mass, xLength, yLength, zLength);
				break;
			default:
				dMassSetSphereTotal(&massStruct, mass, 0.f);
				break;
		}
		dBodySetMass(body, &massStruct);
	}

	void PhysicsBody::setPosition(const Point &p) {
		pos = p;
		dBodySetPosition(body, p.x, p.y, p.z);
	}

	void PhysicsBody::setVelocity(const Vector &v) {
		vel = v;
		dBodySetLinearVel(body, v.x, v.y, v.z);  
	}

	void PhysicsBody::addForce(const Vector &force) {
		forces.push_back(Force(force));
	}
		
	void PhysicsBody::addForce(const Vector &force, const Point &p)  {
		forces.push_back(Force(force, p, true));
	}

	void PhysicsBody::simulatePhysics(int timeElapse) {

		unsigned int numIterations = ceil((timeElapse/1000)/simStep);
		numIterations = (numIterations==0 ? 1 : numIterations);

		for(unsigned int i = 0; i < numIterations; i++) {
			// Add forces
			/*for(unsigned int j = 0; j < bodies.size(); j++) {
				for(unsigned int k = 0; k < bodies[i]->forces.size(); k++) {
					if(!bodies[i]->forces[k].relative)
						dBodyAddForce(bodies[i]->body, bodies[i]->forces[k].force.x, bodies[i]->forces[k].force.y, bodies[i]->forces[k].force.z);
					else 
						dBodyAddForceAtPos(bodies[i]->body, bodies[i]->forces[k].force.x, bodies[i]->forces[k].force.y, bodies[i]->forces[k].force.z,
							bodies[i]->forces[k].pos.x, bodies[i]->forces[k].pos.y, bodies[i]->forces[k].pos.z);
				}
			}*/	

			// Do simulation
			dWorldQuickStep(world, simStep);
		}

		for(unsigned int i = 0; i < bodies.size(); i++) {
			const dReal *pos = dBodyGetPosition(bodies[i]->body);
			const dReal *vel = dBodyGetLinearVel(bodies[i]->body);
			Vector amountShifted(pos[0] - bodies[i]->pos[0], pos[1] - bodies[i]->pos[1], pos[2] - bodies[i]->pos[2]);
			bodies[i]->pos.set(pos[0], pos[1], pos[2]);
			bodies[i]->vel.set(vel[0], vel[1], vel[2]);
			bodies[i]->updateGraphicalPosition(amountShifted);
		}

		// Check for/Handle collisions
		for(unsigned int i = 0; i < bodies.size(); i++) {
			if(!bodies[i]->canCollide) continue;

			Reference<Primitive> objHit;
			Reference<Primitive> temp = bodies[i].getPtr();
			if(tree->intersects(temp, objHit))
				bodies[i]->handleCollision(objHit);
		}

	}

	void PhysicsBody::initPhysics() {
		dInitODE2(0);

		world = dWorldCreate();

		dWorldSetGravity(world, 0.f, -9.8f, 0.f);
		dWorldSetERP(world, .2f);	// set error correction
	}

	void PhysicsBody::endPhysics() {
		dWorldDestroy(world);
		dCloseODE();
	}

	Point PhysicsBody::getPosFromSceneNode(Ogre::SceneNode *node) {
		return Point(node->convertLocalToWorldPosition(node->getPosition()));
	}

} // end FormFactor
