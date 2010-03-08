/*
 * The base class of all objects that exist in the scene.
 *
 * Provies the intersection of an objects graphical representation
 * and base logic.
 */

#pragma once
#include <vector>

#include "GameEntity.h"

#include "Reference.h"
#include "BoundingBox.h"


namespace FormFactor {

	class Primitive : public GameEntity {
	
	public:
		Primitive() {}

		Primitive(Ogre::SceneNode *node) : GameEntity(node) {}

		/*
		 * Return the primitive's bounding box in world coordinates.
		 *
		 * If moving object, pass full extent of movement.
		 */
		virtual BoundingBox worldBound() const = 0;

		/*
		 * Return true if the primitive can be seen by the camera.
		 * A primiive might be invisible or it might be out of viewing
		 * range.
		 */
		virtual bool isVisible() const {return true;}
		
		/*
		 * Return true if the primitive can undergo intersection
		 * tests.  Return false if it needs to be refined first.
		 */
		virtual bool canIntersect() const {return false;} 

		/*
		 * Return true if this primitive intersects with the passed in
		 * primitive.
		 *
		 * Return reference to specific primitive hit in objHit.
		 */
		virtual bool intersects(Reference<Primitive> &test, Reference<Primitive> &objHit) const = 0;

		/*
		 * Call to refine the primitive.  The refined primitives
		 * will be placed at the end of the vector.
		 */
		virtual void refine(std::vector<Reference<Primitive> > &refined) const {}

		/*
		 * Refine the primitive until it is full intersectable.
		 *
		 * Refined primitives are placed in the vector.
		 */
		void fullyRefine(std::vector<Reference<Primitive> > &refined) const;


	protected:

	private:
	};


} // end FormFactor