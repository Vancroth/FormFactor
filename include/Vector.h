/*
 * Defines a Vector in 3D.
 */

#pragma once
#include <math.h>
#include <assert.h>
#include <algorithm>

#include <Ogre.h>

namespace FormFactor {

class Point;

class Vector {

public:

	Vector() : x(0), y(0), z(0) {}

	Vector(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}

	Vector(const Ogre::Vector3 &v) : x(v.x), y(v.y), z(v.z) {}

	inline void set(float _x, float _y, float _z) {
		x = _x; y = _y; z = _z;
	}

	inline float lengthSquared() const {
		return x*x + y*y + z*z;
	}

	inline float length() const {
		return sqrt(lengthSquared());
	}

	inline Ogre::Vector3 getOgreVector() const {
		return Ogre::Vector3(x, y, z);
	}

	inline bool operator==(const Vector &rhs) const {
		return (x==rhs.x && y==rhs.y && z==rhs.z);
	}

	bool operator!=(const Vector &rhs) const {
		return !(*this == rhs);
	}

	Vector operator+(const Vector &other) const {
		return Vector(x + other.x, y + other.y, z + other.z);
	}

	Vector& operator+=(const Vector &other) {
		x += other.x; y += other.y; z += other.z;
		return *this;
	}

	Vector operator-(const Vector &other) const {
		return Vector(x - other.x, y - other.y, z - other.z);
	}

	Vector& operator-=(const Vector &other) {
		x -= other.x; y -= other.y; z -= other.z;
		return *this;
	}

	Vector operator*(const float a) const {
		return Vector(x * a, y * a, z * a);
	}

	Vector& operator*=(const float a) {
		x *= a; y *= a; z *= a;
		return *this;
	}

	float operator[](int i) const {
		assert(i > 0 && i <= 2);
		return (&x)[i];
	}

	float& operator[](int i) {
		assert(i > 0 && i <= 2);
		return (&x)[i];
	}


	float x, y, z;

private:

};

} // end FormFactor
