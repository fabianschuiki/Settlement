/* Copyright © 2013 Fabian Schuiki */
#pragma once
#include <cmath>
#include <sstream>

/**
 * Generic 3D vector.
 */

template <typename T>
class Vector
{
public:
	T x;
	T y;
	T z;
	
	//Constructors
	Vector()
	{
		x = 0;
		y = 0;
		z = 0;
	}
	Vector(T x, T y, T z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}
	
	//Operators
	Vector<T>& operator= (Vector<T> v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
		return *this;
	}
	Vector<T> operator- ()	{ return Vector<T>(-x, -y, -z); }
	
	//CVector-CVector
	Vector<T> operator+ (Vector<T> v)	{ return Vector<T>(x+v.x, y+v.y, z+v.z); }
	Vector<T> operator- (Vector<T> v)	{ return Vector<T>(x-v.x, y-v.y, z-v.z); }
	Vector<T> operator* (Vector<T> v)	{ return Vector<T>(x*v.x, y*v.y, z*v.z); }
	Vector<T> operator/ (Vector<T> v)	{ return Vector<T>(x/v.x, y/v.y, z/v.z); }
	//CVector-Scalar
	Vector<T> operator+ (T v)	{ return Vector<T>(x+v, y+v, z+v); }
	Vector<T> operator- (T v)	{ return Vector<T>(x-v, y-v, z-v); }
	Vector<T> operator* (T v)	{ return Vector<T>(x*v, y*v, z*v); }
	Vector<T> operator/ (T v)	{ return Vector<T>(x/v, y/v, z/v); }
	
	//CVector-CVector
	Vector<T>& operator+= (Vector<T> v)	{ x += v.x; y += v.y; z += v.z; return *this; }
	Vector<T>& operator-= (Vector<T> v)	{ x -= v.x; y -= v.y; z -= v.z; return *this; }
	Vector<T>& operator*= (Vector<T> v)	{ x *= v.x; y *= v.y; z *= v.z; return *this; }
	Vector<T>& operator/= (Vector<T> v)	{ x /= v.x; y /= v.y; z /= v.z; return *this; }
	
	//CVector-Scalar
	Vector<T>& operator+= (T v)	{ x += v; y += v; z += v; return *this; }
	Vector<T>& operator-= (T v)	{ x -= v; y -= v; z -= v; return *this; }
	Vector<T>& operator*= (T v)	{ x *= v; y *= v; z *= v; return *this; }
	Vector<T>& operator/= (T v)	{ x /= v; y /= v; z /= v; return *this; }
	
	//Operations
	inline T length2() { return x*x + y*y + z*z; }
	inline T length()  { return sqrt(length2()); }
	
	inline void normalize()
	{
		T length = this->length();
		
		x /= length;
		y /= length;
		z /= length;
	}
	inline Vector<T> unit() { Vector<T> v(*this); v.normalize(); return v; }
	
	inline T dot(Vector<T> v)
	{
		return (x*v.x + y*v.y + z*v.z);
	}
	
	inline Vector<T> cross(Vector<T> v)
	{
		return Vector<T>(y*v.z - z*v.y, z*v.x - x*v.z, x*v.y - y*v.x);
	}
};

/// Integer 3D vector.
typedef Vector<int> ivec3;
/// Double 3D vector.
typedef Vector<double> vec3;
