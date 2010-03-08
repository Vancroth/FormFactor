#include "KdTree.h"

namespace FormFactor {

	KdTree::KdTree(const std::vector<Reference<Primitive> > &p, int icost, int tcost, float ebonus, int maxp, int maxDepth)  {
		runId = 0;

		isectCost = icost;
		traversalCost = tcost;
		maxPrims = maxp;
		emptyBonus = ebonus;
		prims = p;

		// Initialize mailboxes
		nMailboxes = p.size();
		mailboxes = new Mailbox[nMailboxes];
		for(unsigned int i = 0; i < nMailboxes; i++)
			mailboxes[i] = Mailbox(prims[i]);

		// Store primitive bounding boxes
		nextFreeNode = nAllocatedNodes = 0;
		std::vector<BoundingBox> primBounds;
		primBounds.reserve(prims.size());
		for(unsigned int i = 0; i < prims.size(); i++) {
			BoundingBox b = prims[i]->worldBound();
			bounds = bounds.getUnion(b);
			primBounds.push_back(b);
		}

		// intialize overlap array
		int *primNums = new int[prims.size()];
		for(unsigned int i = 0; i <prims.size(); i++)
			primNums[i] = i;

		// Allocate edge mem
		BoundEdge *edges[3];
		for(unsigned int i = 0; i < 3; i++)
			edges[i] = new BoundEdge[2*prims.size()];

		int *prims0 = new int[prims.size()];
		int *prims1 = new int[(maxDepth+1) * prims.size()];

		// Construct Tree
		buildTree(0, bounds, primBounds, primNums, prims.size(), maxDepth, edges, prims0, prims1);

		for(unsigned int i = 0; i < 3; i++)
			delete[] edges[i];

		delete[] prims0;
		delete[] prims1;
	}

	void KdTree::buildTree(unsigned int nodeNum, const BoundingBox &nodeBounds, const std::vector<BoundingBox> primBounds, int *primNums,
		unsigned int nPrims, unsigned int depth, BoundEdge *edges[3], int *prims0, int *prims1) {

			// allocate memory
			if(nextFreeNode == nAllocatedNodes) {
				int allocSize = std::max(2*nAllocatedNodes, 512);
				KdNode *newNodes = new KdNode[allocSize];
				if(nAllocatedNodes > 0) {
					memcpy(newNodes, nodes, nAllocatedNodes * sizeof(KdNode));
					delete[] nodes;
				}
				nodes = newNodes;
				nAllocatedNodes = allocSize;
			}
			nextFreeNode++;

			// create leaf node if small num of prims or max depth reached
			if(nPrims <=maxPrims || depth==0) {
				nodes[nodeNum].initLeaf(primNums, nPrims, mailboxes);
				return;
			}

			int bestAxis, bestOffset; bestAxis = bestOffset = -1;
			float bestCost = INFINITY;
			float oldCost = isectCost * float(nPrims);
			float invSA = nodes[nodeNum].bounds.calcSurfaceArea();
			int axis = nodes[nodeNum].bounds.getLongestAxis();
			int splitTries = 0;

			while(true) {
				for(unsigned int i = 0; i < nPrims; i++) {
					unsigned int primNum = primNums[i];
					edges[axis][2*i] = BoundEdge(primBounds[primNum].getorigin()[axis], primNum, true);
					edges[axis][2*i+1] = BoundEdge(primBounds[primNum].getMaxPoint()[axis], primNum, false);
				}
				std::sort(&edges[axis][0], &edges[axis][2*nPrims]);

				// Compute cost of splits
				unsigned int nBelow = 0;
				unsigned int nAbove = nPrims;
				for(unsigned int i = 0; i < 2*nPrims; i++) {
					if(edges[axis][i].type == BoundEdge::END) nAbove--;
					float edgeT = edges[axis][i].t;
					if(edgeT > nodeBounds.getorigin()[axis] && edgeT < nodeBounds.getMaxPoint()[axis]) {
						BoundingBox above, below;
						nodeBounds.getChildBoxes(axis, edgeT, below, above);

						float probBelow = below.calcSurfaceArea() * invSA;
						float probAbove = above.calcSurfaceArea() * invSA;
						float eb = (nAbove==0 || nBelow==0 ? emptyBonus : 0.f);
						float cost = traversalCost + isectCost * (1.f - eb) * (probBelow * nBelow + probAbove * nAbove);

						if(cost < bestCost) {
							bestCost = cost;
							bestAxis = axis;
							bestOffset = i;
						}
					}
					if(edges[axis][i].type == BoundEdge::START) nBelow++;
				}

				if(bestAxis != -1 || splitTries == 2)
					break;
				splitTries++;
				axis = (axis + 1) % 3;
			}

			if((bestCost > 4.f * oldCost && nPrims < 8) || bestAxis == -1) {
				nodes[nodeNum].initLeaf(primNums, nPrims, mailboxes);
				return;
			}

			// Classify prims as above or below
			unsigned int n0 = 0;
			unsigned int n1 = 0;
			for(unsigned int i = 0; i < bestOffset; i++)
				if(edges[bestAxis][i].type == BoundEdge::START)
					prims0[n0++] = edges[bestAxis][i].primNum;
	
			for(unsigned int i = bestOffset + 1; i < 2*nPrims; i++)
				if(edges[bestAxis][i].type == BoundEdge::END)
					prims0[n1++] = edges[bestAxis][i].primNum;

			// Initialize node & children
			float tsplit = edges[bestAxis][bestOffset].t;
			nodes[nodeNum].initInterior(nodeBounds);
			BoundingBox child0Bounds, child1Bounds;
			nodeBounds.getChildBoxes(bestAxis, tsplit, child0Bounds, child1Bounds);
			buildTree(nodeNum+1, child0Bounds, primBounds, prims0, n0, depth-1, edges, prims0, prims1 + nPrims);

			nodes[nodeNum].aboveChild;
			buildTree(nodes[nodeNum].aboveChild, child1Bounds, primBounds, prims1, n1, depth-1, edges, prims0, prims1 + nPrims);
	}

	bool KdTree::intersects(Reference<Primitive> &test, Reference<Primitive> &objHit) const {

		unsigned int nodeNum = 0;
		runId = (runId + 1) % 10;
		return intersectsRecurse(nodeNum, test, objHit, runId);
	}

	bool KdTree::intersectsRecurse(int nodeNum, Reference<Primitive> &test, Reference<Primitive> &objHit, int id) const {
		KdNode *node = &nodes[nodeNum];

		if(node->isLeaf()) { // leaf
			Mailbox **mailboxes = node->mailboxes;
			for(unsigned int i = 0; i < node->getNumPrimitives(); i++) {
				if(mailboxes[i]->lastId == runId) continue;		// already tested
				mailboxes[i]->lastId = runId;
				if(mailboxes[i]->prim->canIntersect()) {
					Reference<Primitive> hitObj;
					if(mailboxes[i]->prim->intersects(test, hitObj)) {
						objHit = hitObj;
						return true;
					}
				} else {			// need to refine first
					std::vector<Reference<Primitive> > refined;
					mailboxes[i]->prim->fullyRefine(refined);
					for(unsigned int i = 0; i < refined.size(); i++) {
						Reference<Primitive> hitObj;
						if(refined[i]->intersects(test, hitObj)) {
							objHit = hitObj;
							return true;
						}
					}
				}
			}
			return false;
		} else { // interior
			if(!node->getBounds().intersects(test->worldBound())) // No overlap
				return false;

			// Do kids
			bool intersected = intersectsRecurse(nodeNum+1, test, objHit, id); 
			intersected = intersected || intersectsRecurse(node->aboveChild, test, objHit, id);
			return intersected;
		}
	}
}