/*
 * Defines a point in 3D.
 */

#pragma once
#include <math.h>
#include <assert.h>
#include <algorithm>

#include "Vector.h"

#include <Ogre.h>

namespace FormFactor {


class Point {

public:

	Point() : x(0), y(0), z(0) {}

	Point(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}

	Point(const Ogre::Vector3 &v) : x(v.x), y(v.y), z(v.z) {}

	inline void set(float _x, float _y, float _z) {
		x = _x; y = _y; z = _z;
	}

	inline Ogre::Vector3 getOgrePoint() const {
		return Ogre::Vector3(x, y, z);
	}

	inline float distanceSquared(const Point &other) {
		return (x - other.x)*(x - other.x) + (y - other.y)*(y - other.y) + (z - other.z)*(z - other.z);
	}

	inline float distance(const Point &other) {
		return sqrt(distanceSquared(other));
	}

	inline bool operator==(const Point &rhs) const {
		return (x==rhs.x && y==rhs.y && z==rhs.z);
	}

	bool operator!=(const Point &rhs) const {
		return !(*this == rhs);
	}

	Vector operator-(const Point &other) const {
		return Vector(x - other.x, y - other.y, z - other.z);
	}

	Point& operator-=(const Vector &v) {
		x -= v.x; y -= v.y; z -= v.z;
		return *this;
	}

	Point operator+(const Vector &vec) const {
		return Point(x + vec.x, y + vec.y, z + vec.z);
	}
	
	Point& operator+=(const Vector &v) {
		x += v.x; y += v.y; z += v.z;
		return *this;
	}


	float operator[](int i) const {
		assert(i >= 0 && i <= 2);
		return (&x)[i];
	}

	float& operator[](int i) {
		assert(i >= 0 && i <= 2);
		return (&x)[i];
	}

	static Point minPt(const Point &pt1, const Point &pt2);
	static Point maxPt(const Point &pt1, const Point &pt2);

	float x, y, z;

private:

};

} // end FormFactor