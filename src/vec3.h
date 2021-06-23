#ifndef VEC3_H
#define VEC3_H

#include <iostream>
#include<cmath>
class vec3{
	public:
		// constructors for vec3
		vec3(): e{0,0,0}{}
		vec3(double e0,double e1, double e2): e{e0,e1,e2}{}
		// vecor component accessors
		double x() const {return e[0];}
		double y() const {return e[1];}
		double z() const {return e[2];}

		double r() const {return e[0];}
		double g() const {return e[1];}
		double b() const {return e[2];}

		// some operator overloading to create
		vec3 operator-() const{return vec3(-e[0],-e[1],-e[2]); }
		//overlaod subscripting operator
		double operator[](int i) const{return e[i]; }
		//not sure why we need this
		// double &operator[](int i) {return e[i]; }	
		
		// overload +=
		vec3 &operator+=(const vec3 &v){
			e[0] += v.e[0];
			e[1] += v.e[1];
			e[2] += v.e[2];
			return *this;
		}
		// overload *=
		vec3 &operator*=(const double t){
			e[0] += t;
			e[1] += t;
			e[2] += t;
			return *this;
		}
		// overload /=
		vec3 &operator/=(const double t){
			// use the already overloaded operator
			return *this *= 1/t;
		}

		// len of the vector
		double length() const{
			return std::sqrt(len_squared());
		}

		double len_squared() const{
			return e[0]*e[0] + e[1]*e[1] + e[2]*e[2];
		}

		inline static vec3 random() {
			return vec3(random_double(),random_double(),random_double());
		}

		inline static vec3 random(double min, double max) {
			return vec3(random_double(min, max),random_double(min, max),random_double(min, max));
		}

		
		

	public:
		double e[3];
};

// Add more aliases foe vec3 because color, 3d points, directions etc can be represented as vec3.
// This is just to improve the readability
using point3 = vec3; // (x,y,z) 3D point
using color = vec3; // RGB

// Utility functions

inline std::ostream& operator<<(std::ostream &out, const vec3 &v){
	return out << v.e[0] << " " << v.e[1] << " " << v.e[2];
}

inline vec3 operator+(const vec3 &u, const vec3 &v){
	return vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

inline vec3 operator-(const vec3 &u, const vec3 &v){
	return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

inline vec3 operator*(const vec3 &u, const vec3 &v){
	return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

inline vec3 operator+(double t, const vec3 &v){
	return vec3(t + v.e[0], t + v.e[1], t + v.e[2]);
}

inline vec3 operator+(const vec3 &v, double t ){
	return t + v;
}

inline vec3 operator*(double t, const vec3 &v){
	return vec3(t * v.e[0], t * v.e[1], t * v.e[2]);
}

inline vec3 operator*(const vec3 &v, double t){
	// because double * vec3 is already overloaded
	return t * v;
}

inline vec3 operator/(const vec3 v, double t){
	return (1/t) * v;
}

inline double dot(const vec3 &u, const vec3 &v){
	return u.e[0] * v.e[0] + u.e[1] * v.e[1] + u.e[2] * v.e[2];
}

inline vec3 cross(const vec3 &u, const vec3 &v){
	/*
	| i    j    k  |
	| a0   a1   a2 |
	| b0   b1   b2 |

	The cross product is (a1b2 - b1a2)i + (b0a2 - a0b2)j + (a0b1 - b0a1)k
	*/
	return vec3(u.e[1] * v.e[2] - v.e[1] * u.e[2],
				v.e[0] * u.e[2] - u.e[0] * v.e[2],
				u.e[0] * v.e[1] - v.e[0] * u.e[1]);
}

inline vec3 unit_vector(const vec3 v){
	return v / v.length();
}

inline vec3 random_in_unit_sphere() {
	while (true) {
		auto p = vec3::random(-1,1);
		if (p.len_squared() > 1.0 ) {
			continue;
		}
		return p;
	}
}

inline vec3 random_unit_vector() {
	return unit_vector(random_in_unit_sphere());
}

inline vec3 random_in_hemisphere(const vec3& normal) {
	vec3 in_unit_sphere = random_in_unit_sphere();
	if (dot(in_unit_sphere, normal) > 0.0) {
		// angle betwen 0 and 90
		// means this vector is in the same hemisphere as the normal
		return in_unit_sphere;
	} else {
		return -in_unit_sphere;
	} 
}
#endif