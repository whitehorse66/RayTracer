#ifndef HIT_H
#define HIT_H

#include "vec3.h"
//sphereクラスプロトタイプ宣言
//単純にincludeすると循環参照になってしまうため

class Sphere;

class Hit {
public:
	double t; //collision distance
	Vec3 hitPos; //collision position
	Vec3 hitNormal;//collision normal
	const Sphere* hitSphere;//collision object

	Hit() {
		//initial value
		t = 10000;
	};
};
#endif
