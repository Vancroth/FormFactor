/*
 * An interface for physics.
 */

#pragma once
#include <vector>

#include <ode/ode.h>

#include "Vector.h"
#include "Point.h"
#include "Primitive.h"
#include "KdTree.h"


namespace FormFactor {

	struct Force {
		Force() {}

		Force(const Vector &v, const Point &p = Point(0, 0, 0), bool rel = false) {
			force = v;
			pos = p;
			relative;
		}

		Vector force;
		Point pos;
		bool relative;
	};

	class PhysicsBody : public Primitive {
	
	public:
		enum BodyType {
			Sphere,
			Cylinder,
			Box,
			PointMass
		};

		PhysicsBody();
		PhysicsBody(Ogre::SceneNode *node, bool collides, const Vector &v  = Vector(0.f, 0.f, 0.f));

		virtual ~PhysicsBody() {dBodyDestroy(body);}

		virtual void setMass(float m);

		void setPosition(const Point &p);
		void setVelocity(const Vector &v);

		/* 
		 * Add a force.
		 * 
		 * The second version applies the force from the
		 * passed in point.
		 */
		void addForce(const Vector &force);
		void addForce(const Vector &force, const Point &p);
		void clearForces() {forces.clear();}

		void enableGravity(bool gravityOn) {dBodySetGravityMode(body, gravityOn);}
		
		/*
		 * Get the current position of the object's center of mass.
		 */
		Point getPosition() const {return pos;}

		/*
		 * Any object which wants to handle collision must override,
		 * this function.
		 */
		virtual void handleCollision(Reference<Primitive> &objHit) {}

		/*
		 * All physical bodies must implement a method
		 * for updating their graphical represenation
		 * given their new position.
		 */
		virtual void updateGraphicalPosition(const Vector &amountShifted) = 0;

		/*
		 * Iniitializes the physical world.
		 * Call this before creating any instances
		 * of PhysicsBody.
		 *
		 * Call endPhysics() to destroy ODE.
		 */
		static void initPhysics();
		static void endPhysics();

		/*
		 * Set kd-tree to be used for collisions.
		 */
		static void setTree(Reference<KdTree> &t) {tree = t;}

		/*
		 * Run an iteration of the simulation
		 */
		static void simulatePhysics(int timeElapse);


	protected:
		void setMass(float m, BodyType type, const BoundingBox &box);

		float mass;
		Point pos;
		Vector vel;
		bool canCollide;

		std::vector<Force> forces;

	private:
		dBodyID body;

		static std::vector<Reference<PhysicsBody> > bodies;
		static dWorldID world;
		static const float simStep;		// length of simulation time step
		static Reference<KdTree> tree;

		static inline Point getPosFromSceneNode(Ogre::SceneNode *node);
	};


} // end FormFactor
