/*
 * Implementation for a axis aligned grid.
 *
 * The gid is used for the acceleration of 
 * collisions and for visibility checks, i.e.
 * hidden surface removal and proximity to character.
 */

#ifndef _GRIDACCEL_H_
#define _GRIDACCEL_H_

#include <vector>

#include "Primitive.h"
#include "BoundingBox.h"

namespace FormFactor {

	/*
	 * Used to track whether a primitive has already been checked
	 * in another voxel.
	 */
	struct Mailbox {
		Mailbox() {}

		Mailbox(const Reference<Primitive> &p) {
			prim = p;
			lastId = -1;
		}
		Reference<Primitive> prim;
		int lastId;
	};


	class GridAccel : public Primitive {

	public:
		GridAccel(const std::vector<Reference<Primitive> > &prims);

	private:

		std::vector<Reference<Primitive> > prims;

		BoundingBox bounds;		// global boundary

		Mailbox *mailboxes;
		unsigned int nMailboxes;

	};

} // end FormFactor

#endif