#ifndef MATERIAL_H
#define MATERIAL_H
#include <cmath>
#include "vec3.h"
#include "random.h"


//calculate conTheta from local coordinate vector
//return value: cos
//v: local coordinate vector
double cosTheta(const Vec3& v) {
    return v.y;
}

//return value: abs cos
//v: local coordinate vector
double absCosTheta(const Vec3& v) {
    return std::abs(v.y);
}

//return refrection vertor
//n: Normal Vector
Vec3 reflect(const Vec3& v, const Vec3& n) {
    return -v + 2 * dot(v, n) * n;
}


//�t���l�����˗���Ԃ�
//v: ���˃x�N�g��
//n: �@��
//n1: ���ˑ����ܗ�
//n2: �o�ˑ����ܗ�
double fresnel(const Vec3& v, const Vec3& n, double n1, double n2) {
    double f0 = std::pow((n1 - n2) / (n1 + n2), 2.0);
    double cos = absCosTheta(v);
    return f0 + (1 - f0) * std::pow(1 - cos, 5.0);
}


//���ܕ������v�Z����
//�S���˂��N�����ꍇ��false��Ԃ�
//v: ���˃x�N�g��
//r: ���܃x�N�g��
//n: �@��
//n1: ���ˑ����ܗ�
//n2: �o�ˑ����ܗ�
bool refract(const Vec3& v, Vec3& r, const Vec3& n, double n1, double n2) {
    double cos = absCosTheta(v);
    double sin = std::sqrt(std::max(1 - cos * cos, 0.0));
    double alpha = n1 / n2 * sin;

    //�S����
    if (alpha * alpha > 1.0) return false;

    //���܃x�N�g��
    r = n1 / n2 * (-v + dot(v, n) * n) - std::sqrt(1 - alpha * alpha) * n;

    return true;
}


class Material {
public:
    //return BRDF
    //wi: sampling vector
    virtual Vec3 sample(const Vec3& wo, Vec3& wi, double& pdf) const = 0;
};


class Diffuse : public Material {
public:
    Vec3 rho; //refrectance

    Diffuse(const Vec3& _rho) : rho(_rho) {};

    Vec3 sample(const Vec3& wo, Vec3& wi, double& pdf) const {
        //uniform random num
        double u = rnd();
        double v = rnd();

        //(theta, phi) calculation
        double theta = 0.5 * std::acos(1 - 2 * u);
        double phi = 2 * M_PI * v;

        //(x, y, z)
        double x = std::cos(phi) * std::sin(theta);
        double y = std::cos(theta);
        double z = std::sin(phi) * std::sin(theta);
        //sampling direction
        wi = Vec3(x, y, z);

        //pdf 
        pdf = std::cos(theta) / M_PI;

        return rho / M_PI;
    };
};


class Mirror : public Material {
public:
    Mirror() {};

    Vec3 sample(const Vec3& wo, Vec3& wi, double& pdf) const {
        //refrection vector
        wi = reflect(wo, Vec3(0, 1, 0));

        //delta function was canceled 
        pdf = 1.0;
        return 1 / cosTheta(wi) * Vec3(1);
    };
};


class Glass : public Material {
public:
    double ior; //���ܗ�

    Glass(double _ior) : ior(_ior) {};

    Vec3 sample(const Vec3& wo, Vec3& wi, double& pdf) const {
        //�K���X�ɓ��˂��Ă��邩�̃t���O
        bool isEntering = cosTheta(wo) > 0;

        //���˂Əo�˂ŋ��ܗ��A�@�������ւ���
        double n1; //���ˑ����ܗ�
        double n2; //�o�ˑ����ܗ�
        Vec3 normal; //�@�� ���˂Əo�˂œ���ւ���K�v������
        if (isEntering) {
            n1 = 1.0;
            n2 = ior;
            normal = Vec3(0, 1, 0);
        }
        else {
            n1 = ior;
            n2 = 1.0;
            normal = Vec3(0, -1, 0);
        }

        //�t���l�����˗�
        double fr = fresnel(wo, normal, n1, n2);

        //����
        if (rnd() < fr) {
            wi = reflect(wo, normal);
            pdf = fr;
            return fr / absCosTheta(wi) * Vec3(1);
        }
        //����
        else {
            if (refract(wo, wi, normal, n1, n2)) {
                pdf = 1 - fr;
                return std::pow(n1 / n2, 2.0) * (1 - fr) / absCosTheta(wi) * Vec3(1);
            }
            //�S����
            else {
                wi = reflect(wo, normal);
                pdf = 1 - fr;
                return (1 - fr) / absCosTheta(wi) * Vec3(1);
            }
        }
    };
};
#endif
