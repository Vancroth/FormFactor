#pragma once
#include <Ogre.h>

#include "GameEntity.h"
#include "Reference.h"
#include "LevelTileEntity.h"

namespace FormFactor {

	struct LevelTileInfo {
		LevelTileInfo() {nSuccessors = 0;}
		LevelTileInfo(unsigned int numSuccessors, int* s, unsigned int pred) {
			nSuccessors = numSuccessors;
			predecessor = pred;
			successors = new int[numSuccessors];
			for(unsigned int i = 0; i < numSuccessors; i++)
				successors[i] = s[i];
		}
		int* successors;		// which tiles are reachable from this one
		int predecessor;
		unsigned int nSuccessors;
	};

class LevelEntity : public GameEntity {

public:
	LevelEntity(SceneNode *node, std::vector<Reference<Primitive> > &worldPrims);
	~LevelEntity(void);
	virtual bool frameEvent(const FrameEvent &evt);

	/*
	 * Allocate the kd-tree for successors and deallocate tree for parent.
	 * 
	 * Operates based on the assumption that any previous tiles, have a lower
	 * tileID number.
	 */
	void allocateTileTrees(unsigned int tileID) const;

protected:

private:
	unsigned int nTiles;
	std::vector<Reference<LevelTileEntity> > tiles;		// the level tiles
	LevelTileInfo *tileInfo;	
};

} // end FormFactor