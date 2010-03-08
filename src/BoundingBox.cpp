#include "BoundingBox.h"

namespace FormFactor {

BoundingBox::BoundingBox() {
	origin.set(INFINITY, INFINITY, INFINITY);
	maxPoint.set(-INFINITY, -INFINITY, -INFINITY);
}

BoundingBox::BoundingBox(float _x, float _y, float _z, float _width, float _height, float _depth) { 

	origin.set(_x, _y, _z);
	maxPoint.set(_x + _width, _y + _height, _z + _depth);
}

BoundingBox::BoundingBox(const Point &pt1, const Point &pt2, bool ordered) {

	if(ordered) {
		origin = pt1;
		maxPoint = pt2;
	} else {
		origin = Point::minPt(pt1, pt2);
		maxPoint = Point::maxPt(pt1, pt2);
	}
}

BoundingBox::BoundingBox(const Ogre::AxisAlignedBox &ogreBox) {
	origin = Point(ogreBox.getMinimum());
	maxPoint = Point(ogreBox.getMaximum());
}

BoundingBox BoundingBox::getUnion(const Point &pt) {
	Point min = Point::minPt(origin, pt);
	Point max = Point::maxPt(maxPoint, pt);
	return BoundingBox(min, max, true);
}

BoundingBox BoundingBox::getUnion(const BoundingBox &other) {
	return BoundingBox(Point::minPt(origin, other.origin), Point::maxPt(maxPoint, other.maxPoint));
}

BoundingBox BoundingBox::getExpanded(const Vector &v) {
	return BoundingBox(Point::minPt(origin, origin + v), Point::maxPt(maxPoint, maxPoint + v));
}

bool BoundingBox::intersects(const BoundingBox &other) const {

	// Check x-axis
	if(maxPoint.x < other.origin.x || other.maxPoint.x < origin.x)
		return false;

	// Check y-axis
	if(maxPoint.y < other.origin.y || other.maxPoint.y < origin.y)
		return false;
	
	// Check z-axis
	if(maxPoint.z < other.origin.z || other.maxPoint.z < origin.z)
		return false;
	
	return true;
}

Vector BoundingBox::getIntersectDir(const BoundingBox &other) const {
	BoundingBox neg(origin, other.maxPoint, true);
	BoundingBox pos(other.origin, maxPoint, true);

	if(neg.calcVolume() > pos.calcVolume()) {
		int shortest = pos.getShortestAxis();
		return (shortest==0 ? Vector(1, 0, 0) : (shortest==1 ? Vector(0, 1, 0) : Vector(0, 0, 1)));
	} else {
		int shortest = neg.getShortestAxis();
		return (shortest==0 ? Vector(-1, 0, 0) : (shortest==1 ? Vector(0, -1, 0) : Vector(0, 0, -1)));
	}
}

bool BoundingBox::contains(const Point &pt) const {
	return Point::minPt(origin, pt)==origin && Point::maxPt(maxPoint, pt)==maxPoint;
}

bool BoundingBox::contains(const BoundingBox &other) const {
	return contains(other.origin) && contains(other.maxPoint);
}

void BoundingBox::getChildBoxes(int axis, float cut, BoundingBox &childBelow, BoundingBox &childAbove) const {
	childBelow = childAbove = *this;
	childBelow.maxPoint[axis] = cut;
	childAbove.origin[axis] = cut;
}

} // end FormFactor