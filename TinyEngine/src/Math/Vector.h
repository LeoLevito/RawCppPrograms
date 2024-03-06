#pragma once
#include <cmath>

//struct IntVector{
//	IntVector(const Vector& other) //copy constructor example, need to do it on both type's constructors.
//		: x(other.x), y(other.y) {}
//
//	int y{ 0 };
//	int x{ 0 };
//};
////IntVector a;
////Vector b = a; <-- type reduction.

struct Vector { //struct: a way to group variables in a neat way. //members of struct are public by default
	constexpr Vector()
		:x(0.f), y(0.f) { } //example of a default constructor, where 'Vector position compiles like this, still, more code instead of just 'Vector position'
	constexpr Vector(float scalar)
		:x(scalar), y(scalar) { }
	constexpr Vector(float inX, float inY) //constructor, function without return value (and no void). if you have a constructor, you have to use it
		: x(inX), y(inY) {} //initializer list, way to initialize values with more code compared to just 'Vector position{0,0}'.
	
	Vector(const Vector& other)	//copy constructor, just faster, pretty standard to use.
		: x(other.x), y(other.y) {

	}
	//Vector(const IntVector& other)	//allow for converting from one type to another utilizing copy constructor.
	//	: x(other.x), y(other.y) {
	//// like:
	//// IntVector b;
	//// Vector a = b; <-- copy constructor 
	//// a = b <-- assignment operator
	//}
	//Vector& operator=(const IntVector& other) { x = other.x; y = other.y; return *this; } // Assignment operator. allows setting variables of different types.
	//// like:
	//// IntVector b;
	//// Vector a;
	//// a = b <-- assignment operator

	void Normalize() { //member functions can be defined in the header.
		float lengthSqrd = (x * x + y * y);
		if (lengthSqrd < 0.001f) { //don't trust floats to make exact comparisons
			return;
		}
		float length = sqrtf(lengthSqrd);
		x = x /= length;
		y = y /= length;
	}

	//operator overloading
	void operator+=(Vector rhs) { x += rhs.x; y += rhs.y; }
	
	float x{ 0.f };
	float y{ 0.f };

	static Vector lerp(Vector a, Vector b, float t);
};

//operator overloading
inline Vector operator+(Vector lhs, Vector rhs) { return Vector(lhs.x + rhs.x, lhs.y + rhs.y); }
inline Vector operator-(Vector lhs, Vector rhs) { return Vector(lhs.x - rhs.x, lhs.y - rhs.y); }
inline Vector operator*(Vector lhs, float rhs) { return Vector(lhs.x * rhs, lhs.y * rhs); }
inline Vector operator*(float lhs, Vector rhs) { return Vector(rhs.x * lhs, rhs.y * lhs); }


//vector a
//vector b
//vector c = a + b;