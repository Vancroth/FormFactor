#include "BoundingBox.h"

namespace FormFactor {

BoundingBox::BoundingBox() {
	orgin.set(INFINITY, INFINITY, INFINITY);
	maxPoint.set(-INFINITY, -INFINITY, -INFINITY);
}

BoundingBox::BoundingBox(float _x, float _y, float _z, float _width, float _height, float _depth) { 

	orgin.set(_x, _y, _z);
	maxPoint.set(_x + _width, _y + _height, _z + _depth);
}

BoundingBox::BoundingBox(const Point &pt1, const Point &pt2, bool ordered) {

	if(ordered) {
		orgin = pt1;
		maxPoint = pt2;
	} else {
		orgin = Point::minPt(pt1, pt2);
		maxPoint = Point::maxPt(pt1, pt2);
	}
}

BoundingBox::BoundingBox(const Ogre::AxisAlignedBox &ogreBox) {
	orgin = Point(ogreBox.getMinimum());
	maxPoint = Point(ogreBox.getMaximum());
}

BoundingBox BoundingBox::getUnion(const Point &pt) {
	Point min = Point::minPt(orgin, pt);
	Point max = Point::maxPt(maxPoint, pt);
	return BoundingBox(min, max, true);
}

BoundingBox BoundingBox::getUnion(const BoundingBox &other) {
	return BoundingBox(Point::minPt(orgin, other.orgin), Point::maxPt(maxPoint, other.maxPoint));
}

bool BoundingBox::intersects(const BoundingBox &other) const {

	// Check x-axis
	if(maxPoint.x < other.orgin.x || other.maxPoint.x < orgin.x)
		return false;

	// Check y-axis
	if(maxPoint.y < other.orgin.y || other.maxPoint.y < orgin.y)
		return false;
	
	// Check z-axis
	if(maxPoint.z < other.orgin.z || other.maxPoint.z < orgin.z)
		return false;
	
	return true;
}

bool BoundingBox::contains(const Point &pt) const {
	return Point::minPt(orgin, pt)==orgin && Point::maxPt(maxPoint, pt)==maxPoint;
}

bool BoundingBox::contains(const BoundingBox &other) const {
	return contains(other.orgin) && contains(other.maxPoint);
}

void BoundingBox::getChildBoxes(int axis, float cut, BoundingBox &childBelow, BoundingBox &childAbove) const {
	childBelow = childAbove = *this;
	childBelow.maxPoint[axis] = cut;
	childAbove.orgin[axis] = cut;
}

} // end FormFactor