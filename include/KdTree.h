/*
 * Implementation for a kd-tree.
 *
 * The tree is used for the acceleration of 
 * collisions and for visibility checks, i.e.
 * hidden surface removal and proximity to character.
 */

#pragma once

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

	/*
	 * A node of the tree.
	 */
	struct KdNode {

		void initLeaf(int *primNums, int numPrims, Mailbox *mb) {
			nPrims = numPrims << 1;
			leafFlag |= 1;
			if(numPrims==0) {
				mailboxes = NULL;
			} else {
				mailboxes = new Mailbox*[numPrims];
				for(int i = 0; i < numPrims; i++)
					mailboxes[i] = &mb[primNums[i]];
			}
		}

		void initInterior(const BoundingBox &box) {
			bounds = box;
			leafFlag = 0;
		}

		BoundingBox getBounds() const {return bounds;}
		int getNumPrimitives() const {return nPrims >> 1;}
		bool isLeaf() const { return (leafFlag & 1)==1;}

		BoundingBox bounds;			// interior node
		union {
			unsigned int leafFlag;		// both
			unsigned int nPrims;		// leaf node. num prims in voxel
		};
		union {
			Mailbox **mailboxes;		// leaf node
			unsigned int aboveChild;	// interior node. pos of above child
		};
	};

	struct BoundEdge {
		BoundEdge() {}

		BoundEdge(float _t, int _primNum, bool starting) {
			t = _t;
			primNum = _primNum;
			type = (starting ? START : END);
		}

		bool operator<(const BoundEdge &other) const {
			if(t == other.t)
				return int(type) < int(other.type);
			else
				return t < other.t;
		}

		float t;
		int primNum;
		enum {START, END} type;
	};

	class KdTree : public Primitive {

	public:
		KdTree(const std::vector<Reference<Primitive> > &prims, int icost, int tcost, float ebonus, int maxp, int maxDepth);

		// Check for intersection with tree
		bool intersects(Reference<Primitive> &test, Reference<Primitive> &objHit) const;

		BoundingBox worldBound() const {return bounds;}

		bool frameEvent(const FrameEvent &evt) {return true;}  // nothing

	private:
		void buildTree(unsigned int nodeNum, const BoundingBox &nodeBounds, const std::vector<BoundingBox> primBounds, int *primNums,
			unsigned int nPrims, unsigned int depth, BoundEdge *edges[3], int *prims0, int *prims1);

		bool intersectsRecurse(int nodeNum, Reference<Primitive> &other, Reference<Primitive> &objHit, int id) const;


		KdNode *nodes;
		int nAllocatedNodes, nextFreeNode;

		std::vector<Reference<Primitive> > prims;

		BoundingBox bounds;		// global boundary

		int isectCost, traversalCost, maxPrims;
		float emptyBonus;

		Mailbox *mailboxes;
		unsigned int nMailboxes;

		mutable int runId;
	};

} // end FormFactor
