#pragma once
#include <Ogre.h>

#include "Primitive.h"
#include "KdTree.h"
#include "Reference.h"

namespace FormFactor {

class LevelEntity;

class LevelTileEntity : public Primitive {

public:
	enum PlaneDirection { posX, posY, posZ, negX, negY, negZ};
	enum TerrainType { GRASS, METAL, LAVA };

	LevelTileEntity(SceneNode *node, Reference<LevelEntity> l, unsigned int tileID);
	~LevelTileEntity(void);
	virtual bool frameEvent(const FrameEvent &evt);

	/*
	 * Build and destroy a collision accelerator for this tile.
	 */
	void buildAccelerator();
	void destroyAccelerator();

	/*
	 * Override Primitive methods.
	 */

	virtual BoundingBox worldBound() const {return bounds;}
	virtual bool isVisible() const {return false;}
	virtual bool canIntersect() const {return true;}
	virtual bool intersects(Reference<Primitive> &test, Reference<Primitive> &objHit) const;
	virtual Vector handleVehicleCollision(const Vector &vel, float mass, const Vector &dir);

	static const unsigned int TILE_WIDTH, TILE_HEIGHT;

protected:
	std::vector<Reference<Primitive> > prims;		// All primitives in tile
	Reference<KdTree> tree;							// tile's collision accelerator

	Reference<LevelEntity> level;					// the level to which the tile belongs

	BoundingBox bounds;

private:
	void makeRoom(SceneNode *node, std::vector<Reference<Primitive> > &prims, char* terrain);

	TerrainType curTerrain;

	mutable bool firstIntersect;
	unsigned int id;
};

} // end FormFactor
