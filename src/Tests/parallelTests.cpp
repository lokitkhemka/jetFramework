#include<Arrays/array2.h>
#include<Arrays/array3.h>
#include<constants.h>
#include <parallel.h>
#include<gtest/gtest.h>

#include<algorithm>
#include<functional>
#include<random>
#include<vector>

using namespace jet;

static unsigned int NumCores = std::thread::hardware_concurrency();

TEST(Parallel, Fill) {
    size_t N = std::max(20u, (3 * NumCores) / 2);
    std::vector<double> a(N);

    ParallelFill(a.begin(), a.end(), 3.0);

    for (double val : a) {
        EXPECT_EQ(3.0, val);
    }
}

TEST(Parallel, For) {
    size_t N = std::max(20u, (3 * NumCores) / 2);
    std::vector<double> a(N);

    for (size_t i = 0; i < N; ++i) {
        a[i] = static_cast<double>(i);
    }

    ParallelFor(kZeroSize, a.size(), [&a] (size_t i) {
        double expected = static_cast<double>(i);
        EXPECT_DOUBLE_EQ(expected, a[i]);
    });
}

TEST(Parallel, For2D) {
    size_t nX = std::max(20u, (3 * NumCores) / 2);
    size_t nY = std::max(30u, (3 * NumCores) / 2);
    Array2<double> a(nX, nY);

    for (size_t j = 0; j < nY; ++j) {
        for (size_t i = 0; i < nX; ++i) {
            a(i, j) = static_cast<double>(i + j * nX);
        }
    }

    ParallelFor(
        kZeroSize, a.Width(),
        kZeroSize, a.Height(),
        [&] (size_t i, size_t j) {
        double expected = static_cast<double>(i + j * nX);
        EXPECT_DOUBLE_EQ(expected, a(i, j));
    });
}

TEST(Parallel, For3D) {
    size_t nX = std::max(20u, (3 * NumCores) / 2);
    size_t nY = std::max(30u, (3 * NumCores) / 2);
    size_t nZ = std::max(30u, (3 * NumCores) / 2);
    Array3<double> a(nX, nY, nZ);

    for (size_t k = 0; k < nZ; ++k) {
        for (size_t j = 0; j < nY; ++j) {
            for (size_t i = 0; i < nX; ++i) {
                a(i, j, k) = static_cast<double>(i + (j + k * nY) * nX);
            }
        }
    }

    ParallelFor(
        kZeroSize, a.Width(),
        kZeroSize, a.Height(),
        kZeroSize, a.Depth(),
        [&] (size_t i, size_t j, size_t k) {
        double expected = static_cast<double>(i + (j + k * nY) * nX);
        EXPECT_DOUBLE_EQ(expected, a(i, j, k));
    });
}

TEST(Parallel, Sort) {
    size_t N = std::max(20u, (3 * NumCores) / 2);
    std::vector<double> a(N);

    std::mt19937 rng;
    std::uniform_real_distribution<> d(0.0, 1.0);

    for (size_t i = 0; i < N; ++i) {
        a[i] = d(rng);
    }

    ParallelSort(a.begin(), a.end());

    for (size_t i = 0; i + 1 < a.size(); ++i) {
        EXPECT_LE(a[i], a[i + 1]) << i;
    }

    std::vector<double> b(N);
    for (size_t i = 0; i < N; ++i) {
        b[i] = d(rng);
    }

    std::vector<double> c = b;

    ParallelSort(b.begin(), b.end());

    for (size_t i = 0; i + 1 < a.size(); ++i) {
        EXPECT_LE(b[i], b[i + 1]) << i;
    }

    std::vector<size_t> idx(N);
    for (size_t i = 0; i < N; ++i) {
        idx[i] = i;
    }

    ParallelSort(idx.begin(), idx.end(), [&](size_t a, size_t b) {
        return c[a] < c[b];
    });

    for (size_t i = 0; i + 1 < a.size(); ++i) {
        EXPECT_LE(c[idx[i]], c[idx[i + 1]]);
    }
}
