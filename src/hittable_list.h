#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST

#include "hittable.h"

#include <memory>
#include <vector>

using std::shared_ptr;
using std::vector;

class hittable_list : public hittable{
	public:
		hittable_list(){}
		hittable_list(shared_ptr<hittable> object){add(object);}

		void clear(){objects.clear();}
		void add(shared_ptr<hittable> object) {objects.push_back(object);}

		virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;
	public:
		vector <shared_ptr<hittable>> objects;
};

bool hittable_list::hit(const ray& r, double t_min, double t_max, hit_record& rec) const{
	hit_record temp_rec;
	bool hit_something = false;
	auto closest_so_far = t_max;

	for (const auto& object : objects) {
		if(object->hit(r, t_min, closest_so_far, temp_rec))	{
			hit_something = true;
			closest_so_far = temp_rec.t;
			rec = temp_rec;
		}
	}

	return hit_something;
}
/*
Some notes about shared_ptr
shared_ptr<type> is a pointer to some allocated type, with reference-counting semantics. 
Every time you assign its value to another shared pointer (usually with a simple assignment), 
the reference count is incremented. 
As shared pointers go out of scope (like at the end of a block or function), the reference count 
is decremented. Once the count goes to zero, the object is deleted.

shared_ptr<double> double_ptr = make_shared<double>(0.37);
shared_ptr<vec3>   vec3_ptr   = make_shared<vec3>(1.414214, 2.718281, 1.618034);
shared_ptr<sphere> sphere_ptr = make_shared<sphere>(point3(0,0,0), 1.0);

make_shared<thing>(thing_constructor_params ...) allocates a new instance of type thing, using the constructor parameters. It returns a shared_ptr<thing>.

*/

#endif

