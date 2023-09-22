
#include<parallel.h>
#include<timer.h>
#include<gtest/gtest.h>

#include<algorithm>
#include<functional>
#include<random>
#include<vector>

#include<iostream>

using namespace jet;

TEST(Parallel, For) {
    size_t N = (1 << 24) + 7;
    std::vector<double> a(N), b(N), c(N);

    std::mt19937 rng;
    std::uniform_real_distribution<> d(0.0, 1.0);

    for (size_t i = 0; i < N; ++i) {
        a[i] = d(rng);
        b[i] = d(rng);
    }

    Timer timer;

    for (int iter = 0; iter < 20; ++iter) {
        for (size_t i = 0; i < N; ++i) {
            c[i] = 1.0 / std::sqrt(a[i] / b[i] + 1.0);
        }
    }

    std::cout<<"Serial Time For: " << timer.DurationInSeconds()/20.0 << " secs" << std::endl;

    timer.Reset();

    for (int iter = 0; iter < 20; ++iter) {
        ParallelFor(kZeroSize, N, [&] (size_t i) {
            c[i] = 1.0 / std::sqrt(a[i] / b[i] + 1.0);
        });
    }
    std::cout<<"ParallelFor Time: " << timer.DurationInSeconds()/20.0 << " secs" << std::endl;
}