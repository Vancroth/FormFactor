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
		PhysicsBody(Ogre::SceneNode *node, bool collides = true, float m = 0, const Vector &v  = Vector(0.f, 0.f, 0.f));


		void setMass(float m);
		void setPosition(const Point &p);
		void setVelocity(const Vector &v);
		void setVelocityX(const float x);
		void setVelocityY(const float y);
		void setVelocityZ(const float z);

		/* 
		 * Add a force.
		 * 
		 * The second version applies the force from the
		 * passed in point.
		 */
		void addForce(const Vector &force);
		void addForce(const Vector &force, const Point &p);
		inline void clearForces() {forces.clear();}

		void enableGravity(bool gOn) {gravityOn = (gOn ? 1 : 0);}
		
		/*
		 * Get the current position of the object's center of mass.
		 */
		Point getPosition() const {return pos;}
		Vector getAmtShifted() const {return amountShifted;}

		virtual Vector getVelocity() const {return vel;}

		/*
		 * Any object which wants to handle collision must override,
		 * this function.
		 */
		virtual void handleCollision(Reference<Primitive> &objsHit, const Vector &dir) {}

		virtual void clearPhysicsState() {}

		/*
		 * All physical bodies must implement a method
		 * for updating their graphical represenation
		 * given their new position.
		 */
		virtual void updateGraphicalPosition(const Vector &shiftAmt) = 0;

		/*
		 * Set kd-tree to be used for collisions.
		 */
		static void setTree(Reference<KdTree> &t) {tree = t;}

		/*
		 * Run an iteration of the simulation
		 */
		static void simulatePhysics(int timeElapse);


		static const Force gravity;

			virtual bool getOnGround() {return false;}

	protected:
		float mass, massInv;
		Point pos;
		Vector vel;
		int gravityOn;
		bool canCollide;
		bool inactive;

		std::vector<Force> forces;
		Vector amountShifted;		// amount shifted on last update

	private:
		static std::vector<Reference<PhysicsBody> > bodies;
		static Reference<KdTree> tree;

		static inline Point getPosFromSceneNode(Ogre::SceneNode *node);
	};


} // end FormFactor
