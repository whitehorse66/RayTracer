#ifndef CAMERA_H
#define CAMERA_H
#include "vec3.h"
#include "ray.h"
#include "random.h"


class Camera {
public:
	Vec3 camPos;//�J�����̈ʒu
	Vec3 camForward;//�J�����̑O����
	Vec3 camRight;//�J�����̉�����
	Vec3 camUp;//�J�����̏����

	Camera(const Vec3& _camPos, const Vec3& _camForward) :
		camPos(_camPos), camForward(_camForward) {
		orthonormalBasis(camForward, camRight, camUp);

		std::cout << "camPos:" << camPos << std::endl;
		std::cout << "camForward:" << camForward << std::endl;
		std::cout << "camRight:" << camRight << std::endl;
		std::cout << "camUp:" << camUp << std::endl;
	};


	//(u,v)���󂯎��,����ɑΉ����郌�C��Ԃ�
	virtual Ray getRay(double u, double v) const = 0;
};



class PinholeCamera :public Camera {
public:
	double pinholeDist;//distance between the center of Image Sensor and Pinhole 

	PinholeCamera(const Vec3& _camPos, const Vec3& _camForward, double _pinholeDist) :Camera(_camPos, _camForward),
		pinholeDist(_pinholeDist) {};

	Ray getRay(double u, double v) const {
		//Pinhole Position
		Vec3 pinholePos = camPos + pinholeDist * camForward;
		
		//Sensor Position correponding to (u,v)
		Vec3 sensorPos=camPos + u * camRight + v * camUp;

		return Ray(sensorPos, normalize(pinholePos - sensorPos));
	};
};

class ThinLensCamera :public Camera {
public:
	double a;//�C���[�W�Z���T�[���烌���Y���S�܂ł̋���
	double b;//�����Y���S����s���g�̍������ʂ܂ł̋���
	double f;//�œ_����
	double lensRadius;//�����Y�̔��a
	Vec3 lensCenter;//�����Y�̒��S�ʒu

	//focusPoint:�|�C���g�̍����ʒu
	//_a:a�̒l
	//_lensRadius:�����Y�̔��a
	ThinLensCamera(const Vec3& _camPos, const Vec3& _camForward, const Vec3& focusPoint, double _a, double _lensRadius) :
		Camera(_camPos, _camForward), a(_a), lensRadius(_lensRadius) {
		double cos = dot(camForward, normalize(focusPoint - camPos));
		b = cos * (focusPoint - camPos).length() - a;
		f = 1 / (1 / a + 1 / b);//�����Y�̌������
		lensCenter = camPos + a * camForward;
	};
	Ray getRay(double u, double v) const {
		//�C���[�W�Z���T�[��̓_
		Vec3 sensorPos = camPos + u * camRight + v * camUp;
		//�C���[�W�Z���T�[���烌���Y�̒��S�֌���������
		Vec3 r = normalize(lensCenter - sensorPos);
		//�s���g�̍����ʒu
		Vec3 pf = sensorPos + (a + b) / dot(camForward, r) * r;

		//�����Y�̏�̓_���T���v�����O
		double x, y;
		sampleDisk(x, y);
		Vec3 l = lensCenter + lensRadius * (x * camRight + y * camUp);
		return Ray(l, normalize(pf - l));
	}
};


#endif