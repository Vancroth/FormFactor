/*
 * Implementation of an axis aligned bounding box.
 */
#ifndef _BOUNDINGBOX_H_
#define _BOUNDINGBOX_H_

#include <float.h>

#include <Ogre.h>

#include "Point.h"


#ifndef INFINITY
#define INFINITY FLT_MAX
#endif

namespace FormFactor {

class BoundingBox {

public:

	BoundingBox();

	BoundingBox(float x, float y, float z, float width, float height, float depth);

	// Build box from two points
	BoundingBox(const Point &pt1, const Point &pt2, bool ordered=true);

	// Build box from Ogre AABB
	BoundingBox(const Ogre::AxisAlignedBox &ogreBox);

	BoundingBox getUnion(const Point &pt);

	BoundingBox getUnion(const BoundingBox &other);

	// return true if the boxes intersect
	bool intersects(const BoundingBox &other) const;

	void shift(const Vector &v) {
		orgin += v;
		maxPoint += v;
	}

	bool contains(const Point &pt) const;

	bool contains(const BoundingBox &other) const;

	void getChildBoxes(int axis, float cut, BoundingBox &childBelow, BoundingBox &childAbove) const;

	inline Point getOrgin() const {return orgin;}
	inline Point getMaxPoint() const {return maxPoint;}

	inline float calcVolume() const {return (maxPoint.x-orgin.x)*(maxPoint.y-orgin.y)*(maxPoint.z-orgin.z);}

	inline float calcSurfaceArea() const {
		float dx = (maxPoint.x-orgin.x);
		float dy = (maxPoint.y-orgin.y);
		float dz = (maxPoint.z-orgin.z);
		return 2*(dx*dy + dx*dz + dy*dz);
	}

	inline int getLongestAxis()const  {
		float dx = (maxPoint.x-orgin.x);
		float dy = (maxPoint.y-orgin.y);
		float dz = (maxPoint.z-orgin.z);
		return (dx > dy && dx > dz ? 0 : (dy > dz ? 1 : 2));
	}

	inline int getShortestAxis() const {
		float dx = (maxPoint.x-orgin.x);
		float dy = (maxPoint.y-orgin.y);
		float dz = (maxPoint.z-orgin.z);
		return (dx < dy && dx < dz ? 0 : (dy < dz ? 1 : 2));
	}

private:

	Point orgin, maxPoint;

};

} // end FormFactor

#endif // _BOUNDINGBOX_H_