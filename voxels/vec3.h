#ifndef vec3_h
#define vec3_h

#include <cmath>
#include <ostream>

struct vec3 {
	union {
		struct {float x, y, z;};
		float raw[3];
	};
	
	vec3(): x(0), y(0), z(0) {}
	vec3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
	
	float dot(const vec3 &v) const {
		return x * v.x + y * v.y + z * v.z;
	}
	
	vec3 cross(const vec3 &v) const {
		return vec3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
	}
	
	float len2() const { return x * x + y * y + z * z; }
	
	float len() const { return std::sqrt(len2()); }
	
	vec3 nor(float l=1) {
		float mul = l / len();
		return vec3(x * mul, y * mul, z * mul);
	}
	
	inline vec3 operator +(const vec3 &v) const {
		return vec3(x + v.x, y + v.y, z + v.z);
	}
	
	inline vec3 operator -(const vec3 &v) const {
		return vec3(x - v.x, y - v.y, z - v.z);
	}
	
	inline vec3 operator *(float s) const {
		return vec3(x * s, y * s, z * s);
	}
	
	inline vec3 operator +=(const vec3 &v) {
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}
	
	inline vec3 operator -=(const vec3 &v) {
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return *this;
	}
	
	inline vec3 operator *=(float s) {
		x *= s;
		y *= s;
		z *= s;
		return *this;
	}
	
	inline bool operator ==(const vec3 &v) const {
		return fabs(x - v.x) < std::numeric_limits<float>::epsilon()
		&& fabs(y - v.y) < std::numeric_limits<float>::epsilon()
		&& fabs(z - v.z) < std::numeric_limits<float>::epsilon();
	}
	
	inline bool operator !=(const vec3 &v) const {
		return x != v.x || y != v.y || z != v.z;
	}
	
	friend std::ostream& operator<<(std::ostream &os, const vec3 &v) {
		return os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
	}
};

#endif
