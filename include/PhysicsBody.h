/*
 * An interface for physics.
 */

#pragma once
#include <vector>

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

		PhysicsBody();
		PhysicsBody(Ogre::SceneNode *node, bool collides, float m, const Vector &v  = Vector(0.f, 0.f, 0.f));


		void setMass(float m);
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

		void enableGravity(bool gOn) {gravityOn = (gOn ? 1 : 0);}
		
		/*
		 * Get the current position of the object's center of mass.
		 */
		Point getPosition() const {return pos;}

		/*
		 * Any object which wants to handle collision must override,
		 * this function.
		 */
		virtual void handleCollision(Reference<Primitive> &objHit, const Vector &v) {}

		/*
		 * All physical bodies must implement a method
		 * for updating their graphical represenation
		 * given their new position.
		 */
		virtual void updateGraphicalPosition(const Vector &amountShifted) = 0;

		/*
		 * Set kd-tree to be used for collisions.
		 */
		static void setTree(Reference<KdTree> &t) {tree = t;}

		/*
		 * Run an iteration of the simulation
		 */
		static void simulatePhysics(int timeElapse);


		static const Force gravity;
	protected:
		float mass, massInv;
		Point pos;
		Vector vel;
		bool canCollide;
		int gravityOn;

		std::vector<Force> forces;

	private:
		static std::vector<Reference<PhysicsBody> > bodies;
		static Reference<KdTree> tree;

		static inline Point getPosFromSceneNode(Ogre::SceneNode *node);
	};


} // end FormFactor
