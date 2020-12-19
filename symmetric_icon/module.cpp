#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include <tuple>
#include <fstream>
#include <stdexcept>
#include <iostream>
#include <vector>
#include <omp.h>

// The dynamic scale uses a formula derived via linear regression to find
// a scale that won't cause a segfault. However, it does not always work
// and it is very slow. The formula takes the min and max of the x and y
// coordinates, resulting an in O(4n) computation.
#define DYNAMIC_SCALE 0

uint8_t mix_channel(uint8_t FG, uint8_t BG, double opacity) {
    return static_cast<uint8_t>(
        (1 - opacity) * BG + (opacity * FG)
    );
}

extern "C" {
    PyObject* generate(
        double lambda, double alpha, double beta, double gamma, double omega,
        int deg,
        double xi, double yi,
        int WIDTH, int HEIGHT,
        int FG, int BG,
        long ITER,
        #if !DYNAMIC_SCALE
        double scale
        #endif
    ) {
        std::vector<std::tuple<double, double>> positions;
        positions.push_back(std::pair(xi, yi));

        auto WH = WIDTH * HEIGHT, 
             WH2 = WH * 2;

        std::vector<uint16_t> vec(WH, 0);

        PyObject* rst = PyList_New(WH * 3);

        double stats[4] = { 0.0 };

        // Symbolic Icon Attractor calculations start
        for (auto _ = 0; _ < ITER; _++) {
            auto [x, y] = positions.back();

            auto p = alpha * (std::pow(x, 2) + std::pow(y, 2)) + lambda,
                zr = x,
                zi = y;

            for (auto i = 0; i < (deg - 2); i++) {
                auto zr1 = zr * x - zi * y,
                    zi1 = zi * x + zr * y;

                zr = zr1;
                zi = zi1;
            }

            p = p + beta * (x * zr - y * zi);

            positions.push_back(
                std::pair(
                    p * x + gamma * zr - omega * y,
                    p * y - gamma * zi + omega * x
                )
            );
        }

        #if DYNAMIC_SCALE
        [&]<std::size_t ...I>(std::index_sequence<I...>) -> void {
            ([&]<typename A>(bool min, A &&a) -> void {
                    a[I % 10] = std::get<I / 100>(
                        *std::max_element(positions.begin(), positions.end(),
                            [&min]<typename T>(const T &i, const T &j) -> double {
                                return (min) ?
                                    std::get<I / 100>(i) > std::get<I / 100>(j) :
                                    std::get<I / 100>(i) < std::get<I / 100>(j);
                            }
                        )
                    );
                }(I / 10 % 10, stats), ...
            );
        }(std::index_sequence<0, 11, 102, 113>{ });

        auto scale = -48.9629 * (stats[0] - stats[1]) * (stats[2] - stats[3]) + 592.417;
        #endif

        #pragma omp declare reduction(vec : std::vector<uint16_t> : \
            std::transform( \
                omp_out.begin(), omp_out.end(), \
                omp_in.begin(), \
                omp_out.begin(), \
                std::plus<uint16_t>()\
            )\
        ) initializer(omp_priv = decltype(omp_orig)(omp_orig.size()))

        #pragma omp parallel for reduction(vec : vec)
        for (auto i = 0; i < ITER; i++) {
            auto [x, y] = positions[i];
            uint32_t ix = scale * x + WIDTH / 2;
            uint32_t iy = WIDTH / 2 - scale * y;

            vec[ix + WIDTH * iy]++;
        }

        auto max = vec[0];
        #pragma omp parallel for reduction(max : max)
        for (auto i = 0; i < WH; i++) {
            if (vec[i] > max) {
                #pragma omp critical
                max = vec[i];
            }
        }

        for (auto i = 0; i < WH; i++) {
            auto lux = std::pow(static_cast<double>(vec[i]) / max, 1.0 / 3.0);

            //std::cout << vec[i] << '\t' << max << '\t' << lux << '\n';

            // Use unsafe PyList_SET_ITEM macro for faster performance.
            PyList_SET_ITEM(
                rst,
                i,
                PyLong_FromSize_t(mix_channel(FG >> 16, BG >> 16, lux))
            );
            PyList_SET_ITEM(
                rst,
                i + WH,
                PyLong_FromSize_t(mix_channel((FG >> 8) & 0xFF, (BG >> 8) & 0xFF, lux))
            );
            PyList_SET_ITEM(
                rst,
                i + WH2,
                PyLong_FromSize_t(mix_channel(FG & 0xFF, BG & 0xFF, lux))
            );
        }
        return rst;
    }
}