#include <memory>
#include <random>
#include<omp.h>//openMP���g�����߂ɕK�v
#include "vec3.h"
#include "ray.h"
#include "hit.h"
#include "camera.h"
#include "sphere.h"
#include "image.h"
#include "aggregate.h"

#define NUM_THREAD 8

//�����Z���k�c�C�X�^�𗐐�������Ƃ��Ďg�p����
std::random_device rnd_dev;//�V�[���h�����߂�f�o�C�X����
std::mt19937 mt(rnd_dev());//�����Z���k�c�C�X�^
std::uniform_real_distribution<> dist(0, 1);//[0,1]�̈�l����
inline double rnd() {
	return dist(mt);
}

//�X�[�p�[�T���v�����O
int main() {

	const int N = 100;

	//512�~512�̉摜
	Image img(512, 512);

	//���_(0,0,0)�ɒu����A����(0,0,-1)�Ɍ����Ă���J����
	PinholeCamera cam(Vec3(0, 0, 0), Vec3(0, 0, -1), 1.0);

	Aggregate aggregate;

	//(0,0,-3)�ɒu����A���a1�̋�
	aggregate.add(std::make_shared<Sphere>(Vec3(0, 0, -3), 1));

	//(0,-10001,0)�ɔ��a10000�̋��A���Ƃ��Ďg����
	aggregate.add(std::make_shared<Sphere>(Vec3(0, -10001, 0), 10000));
	Vec3 sunDir = normalize(Vec3(1, 1, 1));

	//for������񉻂���
	//schedule(dynamic,1)���w�肷��Ɠ��I�Ɍv�Z�u���b�N�����蓖�Ă���
	//�摜����鏈��
	#pragma omp parallel for schedule(dynamic,1) num_threads(NUM_THREAD)
	for (int k = 0; k < N; k++) {
		for (int i = 0; i < img.width; i++) {
			for (int j = 0; j < img.height; j++) {
				//(u,v)�v�Z��������p���ăs�N�Z�����̈ʒu�����炷
				double u = (2.0 * (i+rnd()) - img.width) / img.width;
				double v = (2.0 * (j+rnd()) - img.height) / img.height;

				//���C�𐶐�
				Ray ray = cam.getRay(-u, -v);

				//col�̓f�t�H���g��(0,0,0)��������
				Vec3 col;

				Hit res;
				if (aggregate.intersect(ray, res)) {
					Ray shadowRay(res.hitPos + 0.001 * res.hitNormal, sunDir);
					Hit res_temp;

					//�����̕����֔�΂������C���Փ˂��Ȃ���������̋������v�Z����
					if (!aggregate.intersect(shadowRay, res_temp)) {
						col = std::max(dot(res.hitNormal, sunDir), 0.0) * Vec3(1);
					}
				}
				//�s�N�Z���ɐF��ǉ�
				img.addPixel(i, j, col);
			}
		}

	}
	//���ς����
	img.divide(N);

	//PPM�o��
	img.ppm_output("super_sumpling_parallel.ppm");

	return 0;
}