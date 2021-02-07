#ifndef HIT_H
#define HIT_H

#include "vec3.h"
//sphere�N���X�v���g�^�C�v�錾
//�P����include����Əz�Q�ƂɂȂ��Ă��܂�����

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
