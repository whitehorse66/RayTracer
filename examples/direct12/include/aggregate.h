#ifndef AGGREGATE_H
#define AGGREGATE_H
#include<memory>
#include<vector>
#include "ray.h"
#include "hit.h"
#include "sphere.h"

class Aggregate{

public:
	std::vector<std::shared_ptr<Sphere>> spheres;//object array


	Aggregate() {}
	Aggregate(const std::vector<std::shared_ptr<Sphere>>& _spheres) : spheres(_spheres) {};

	//add object
	void add(const std::shared_ptr<Sphere>& s) {
		spheres.push_back(s);
	};

	//Collision Detection: ray and all objects, if intersecting, the objects are stored in res array and return true
	bool intersect(const Ray& ray, Hit& res)const {
		bool hit = false;
		for (auto s : spheres) {
			Hit res_temp;
			if (s->intersect(ray, res_temp)) {
				if (res_temp.t < res.t) {
					hit = true;
					res = res_temp;
				}
			}
		}
		return hit;
	};
};




#endif
