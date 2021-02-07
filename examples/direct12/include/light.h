#ifndef LIGHT_H
#define LIGHT_H
#include "vec3.h"


class Light {
public:
	Vec3 color;//light color
	Light(const Vec3& _color) :color(_color) {};
	//Le in rendering equation
	//return holding color
	Vec3 Le() const {
		return color;
	}
};


#endif