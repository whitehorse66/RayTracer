#ifndef RANDOM_H
#define RANDOM_H
#define M_PI 3.14159265359

//use Mersenne Twister
std::random_device rnd_dev;//device random number decides seeds
std::mt19937 mt(rnd_dev());//Mersenne Twister
std::uniform_real_distribution<> density(0, 1);//[0,1]uniform random number
inline double rnd() {
	return density(mt);
}
//in radius 1 cercle uniform sampling 
//x:sampling x
//y:sampling y
void sampleDisk(double& x, double& y) {
	double u1 = rnd();
	double u2 = rnd();
	x = std::sqrt(u1) * std::cos(2 * M_PI * u2);
	y = std::sqrt(u1) * std::sin(2 * M_PI * u2);
}

#endif