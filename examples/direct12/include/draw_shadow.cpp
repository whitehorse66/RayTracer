#include <memory>
#include <random>
#include<omp.h>//openMPを使うために必要
#include "vec3.h"
#include "ray.h"
#include "hit.h"
#include "camera.h"
#include "sphere.h"
#include "image.h"
#include "aggregate.h"

#define NUM_THREAD 8

//メルセンヌツイスタを乱数生成器として使用する
std::random_device rnd_dev;//シールドを決めるデバイス乱数
std::mt19937 mt(rnd_dev());//メルセンヌツイスタ
std::uniform_real_distribution<> dist(0, 1);//[0,1]の一様乱数
inline double rnd() {
	return dist(mt);
}

//スーパーサンプリング
int main() {

	const int N = 100;

	//512×512の画像
	Image img(512, 512);

	//原点(0,0,0)に置かれ、方向(0,0,-1)に向いているカメラ
	PinholeCamera cam(Vec3(0, 0, 0), Vec3(0, 0, -1), 1.0);

	Aggregate aggregate;

	//(0,0,-3)に置かれ、半径1の球
	aggregate.add(std::make_shared<Sphere>(Vec3(0, 0, -3), 1));

	//(0,-10001,0)に半径10000の球、床として使える
	aggregate.add(std::make_shared<Sphere>(Vec3(0, -10001, 0), 10000));
	Vec3 sunDir = normalize(Vec3(1, 1, 1));

	//for文を並列化する
	//schedule(dynamic,1)を指定すると動的に計算ブロックが割り当てられる
	//画像を作る処理
	#pragma omp parallel for schedule(dynamic,1) num_threads(NUM_THREAD)
	for (int k = 0; k < N; k++) {
		for (int i = 0; i < img.width; i++) {
			for (int j = 0; j < img.height; j++) {
				//(u,v)計算かつ乱数を用いてピクセル内の位置をずらす
				double u = (2.0 * (i+rnd()) - img.width) / img.width;
				double v = (2.0 * (j+rnd()) - img.height) / img.height;

				//レイを生成
				Ray ray = cam.getRay(-u, -v);

				//colはデフォルトで(0,0,0)黒が入る
				Vec3 col;

				Hit res;
				if (aggregate.intersect(ray, res)) {
					Ray shadowRay(res.hitPos + 0.001 * res.hitNormal, sunDir);
					Hit res_temp;

					//光線の方向へ飛ばしたレイが衝突しなかったら光の強さを計算する
					if (!aggregate.intersect(shadowRay, res_temp)) {
						col = std::max(dot(res.hitNormal, sunDir), 0.0) * Vec3(1);
					}
				}
				//ピクセルに色を追加
				img.addPixel(i, j, col);
			}
		}

	}
	//平均を取る
	img.divide(N);

	//PPM出力
	img.ppm_output("super_sumpling_parallel.ppm");

	return 0;
}