
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
		

	public:
		double e[3];
}