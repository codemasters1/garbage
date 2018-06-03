#include <iostream>
#include <vector>
#include <cmath>
#include <ctime>
#include <immintrin.h>

struct Vector3 {
    float x;
    float y;
    float z;

    Vector3(float x,float y, float z) : x{x}, y{y}, z{z}{}
};

struct Vector4 {
    float x;
    float y;
    float z;
    float w;
    Vector4(float x,float y, float z, float w) : x{x}, y{y}, z{z}, w{w}{}
};

inline float distance(const Vector4& line, const Vector4& point)
{
    return std::fabs(
            line.x * point.x +
            line.y * point.y +
            line.z * point.z +
            line.w)
           / std::sqrt(
            point.x * point.x +
            point.y * point.y +
            point.z * point.z
    );
}

//inline void distance2(const Vector4& line, const Vector4& p1, const Vector4& p2, float& d1, float& d2)
//{
////    float u[8];
////    u[0] = p1.x;
////    u[0] = p1.x;
////    u[0] = p1.x;
////    u[0] = p1.x;
////
////    __m128 a = ;
//
//    __m256 x = _mm256_set_ps(p1.x, p1.y, p1.z, p1.w, p2.x, p2.y, p2.z, p2.w);
//    __m256 y = _mm256_mul_ps(x, x);
//    __m256 z = _mm256_set_ps(1, 1, 1, 0, 1, 1, 1, 0);
//    __m256 w = _mm256_mul_ps(x, z);
//
//
////    __m256 xx = (*reinterpret_cast<__m128*>(&p1) << 32) | *reinterpret_cast<__m128*>(&p2);
////    __m256 zz =
////    xx = _mm_mul_ps(xx, yy);
////    const Vector4 c = *reinterpret_cast<Vector4*>(&xx);
//
//
//
////    float x[8];
////    x[0] = line.x * p1.x;
////    x[1] = line.y * p1.y;
////    x[2] = line.z * p1.z;
////    x[3] = line.w * p1.w;
////    x[4] = line.x * p2.x;
////    x[5] = line.y * p2.y;
////    x[6] = line.z * p2.z;
////    x[7] = line.w * p2.w;
//
//    float a = x[0] + x[1] + x[2] + x[3];
//    float b = x[4] + x[5] + x[6] + x[7];
//
////    x[0] = p1.x * p1.x;
////    x[1] = p1.y * p1.y;
////    x[2] = p1.z * p1.z;
////    x[3] = 0;
////    x[4] = p2.x * p2.x;
////    x[5] = p2.y * p2.y;
////    x[6] = p2.z * p2.z;
////    x[7] = 0;
//
//    float c = x[0] + x[1] + x[2] + x[3];
//    float d = x[4] + x[5] + x[6] + x[7];
//
//    d1 = std::fabs(a) / std::sqrt(c);
//    d2 = std::fabs(b) / std::sqrt(d);
//}

int main() {
    std::srand(std::time(nullptr));

    size_t numPoints = 100000000;

    std::vector<Vector4> points;
    points.reserve(numPoints);
    for (int i = 0; i < numPoints; ++i) {
        points.emplace_back((float)(std::rand() + 1), (float)(std::rand()), (float)(std::rand()), 1);
    }

    for (int i =0;i<10;++i) {
        clock_t begin = clock();

        Vector4 line{1, 0, 0, 0};
        Vector4 prevPoint = points[0];
        float prevDist = distance(line, prevPoint);

        const __m256 z = _mm256_set_ps(1, 1, 1, 0, 1, 1, 1, 0);

        auto ptr = (float*) points.data();
        float* endPtr = ptr + points.size() * 4;

        for (; ptr < endPtr; ) {

            __m256 x = _mm256_load_ps((float*)&ptr);
            __m256 y = _mm256_mul_ps(x, x);
            y = _mm256_mul_ps(x, z);
            _m256_add

            ptr+=8;


//            if (d1 < prevDist) {
//                prevDist = d1;
//                prevPoint = p1;
//            }
//
//            if (d2 < prevDist) {
//                prevDist = d2;
//                prevPoint = p2;
//            }
        }

        clock_t end = clock();
        double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;

        std::cout <<elapsed_secs << " | "<< prevPoint.x << " " << prevPoint.y << " " << prevPoint.z << "\n";
    }

    return 0;
}