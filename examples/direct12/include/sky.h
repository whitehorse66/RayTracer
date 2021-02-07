#ifndef SKY_H
#define SKY_H
#include <string>
#include "vec3.h"
#include "ray.h"

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#endif

class Sky {
public:
	//�󂯎�������C�̕����痈���̕��ˋP�x�̒l��Ԃ�
	//�߂�l�F��̕��ˋP�x
	//ray�F���ˋP�x���������̃��C
	virtual Vec3 getRadiance(const Ray& ray) const = 0;
};

//�P��ȋ��\��
class UniformSky :public Sky {
public:
	Vec3 color;//��̐F
	UniformSky(const Vec3& _color) :color(_color) {};

	Vec3 getRadiance(const Ray& ray) const {
		return color;
	};
};

class SimpleSky :public Sky {
public:
	SimpleSky() {};
	Vec3 getRadiance(const Ray& ray) const {
		double t = (ray.direction.y + 1) / 2;
		return (1 - t) * Vec3(1) + t * Vec3(0.7, 0.8, 1);
	};
};

class IBL :public Sky {
public:
	int width; //�摜�̉���
	int height;//�摜�̏c��
	float* hdr_image;//HDR�摜
	IBL(const std::string& filename) {
		int n;
		hdr_image = stbi_loadf(filename.c_str(), &width, &height, &n, 0);
	};
	~IBL() {
		stbi_image_free(hdr_image);
	};

	Vec3 getRadiance(const Ray& ray) const {
		//���������ʍ��W�ɕϊ�
		double theta = std::acos(ray.direction.y);
		double phi = std::atan2(ray.direction.z, ray.direction.x);
		if (phi < 0)phi += 2 * M_PI;

		//(i,j)���v�Z
		int i = phi / (2 * M_PI) * width;
		int j = theta / M_PI * height;

		//(i,j)�̔z����ɂ�����C���f�b�N�X�v�Z
		int index = 3 * i + 3 * width * j;

		return Vec3(hdr_image[index], hdr_image[index + 1], hdr_image[index + 2]);
	};
};

#endif // ! SKY_H
