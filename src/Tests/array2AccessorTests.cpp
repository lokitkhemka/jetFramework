#include <Arrays/array2.h>
#include <Arrays/array2_accessor.h>
#include <gtest/gtest.h>



TEST(ArrayAccessor2, Constructors) {
    double data[20];
    for (int i = 0; i < 20; ++i) {
        data[i] = static_cast<double>(i);
    }

    jet::ArrayAccessor2<double> acc(jet::Size2(5, 4), data);

    EXPECT_EQ(5u, acc.Size().x);
    EXPECT_EQ(4u, acc.Size().y);
    EXPECT_EQ(data, acc.Data());
}

TEST(ArrayAccessor2, Iterators) {
    jet::Array2<float> arr1(
        {{1.f,  2.f,  3.f,  4.f},
         {5.f,  6.f,  7.f,  8.f},
         {9.f, 10.f, 11.f, 12.f}});
    auto acc = arr1.Accessor();

    float cnt = 1.f;
    for (float& elem : acc) {
        EXPECT_FLOAT_EQ(cnt, elem);
        cnt += 1.f;
    }

    cnt = 1.f;
    for (const float& elem : acc) {
        EXPECT_FLOAT_EQ(cnt, elem);
        cnt += 1.f;
    }
}

TEST(ArrayAccessor2, ForEach) {
    jet::Array2<float> arr1(
        {{1.f,  2.f,  3.f,  4.f},
         {5.f,  6.f,  7.f,  8.f},
         {9.f, 10.f, 11.f, 12.f}});

    size_t i = 0;
    arr1.ForEach([&](float val) {
        EXPECT_FLOAT_EQ(arr1[i], val);
        ++i;
    });
}

TEST(ArrayAccessor2, ForEachIndex) {
    jet::Array2<float> arr1(
        {{1.f,  2.f,  3.f,  4.f},
         {5.f,  6.f,  7.f,  8.f},
         {9.f, 10.f, 11.f, 12.f}});

    arr1.ForEachIndex([&](size_t i, size_t j) {
        size_t idx = i + (4 * j) + 1;
        EXPECT_FLOAT_EQ(static_cast<float>(idx), arr1(i, j));
    });
}




TEST(ConstArrayAccessor2, Constructors) {
    double data[20];
    for (int i = 0; i < 20; ++i) {
        data[i] = static_cast<double>(i);
    }

    // Construct with ArrayAccessor2
    jet::ArrayAccessor2<double> acc(jet::Size2(5, 4), data);
    jet::ConstArrayAccessor2<double> cacc(acc);

    EXPECT_EQ(5u, cacc.Size().x);
    EXPECT_EQ(4u, cacc.Size().y);
    EXPECT_EQ(data, cacc.Data());
}

TEST(ConstArrayAccessor2, Iterators) {
    jet::Array2<float> arr1(
        {{1.f,  2.f,  3.f,  4.f},
         {5.f,  6.f,  7.f,  8.f},
         {9.f, 10.f, 11.f, 12.f}});
    auto acc = arr1.ConstAccessor();

    float cnt = 1.f;
    for (const float& elem : acc) {
        EXPECT_FLOAT_EQ(cnt, elem);
        cnt += 1.f;
    }
}

TEST(ConstArrayAccessor2, ForEach) {
    jet::Array2<float> arr1(
        {{1.f,  2.f,  3.f,  4.f},
         {5.f,  6.f,  7.f,  8.f},
         {9.f, 10.f, 11.f, 12.f}});
    auto acc = arr1.ConstAccessor();

    size_t i = 0;
    acc.ForEach([&](float val) {
        EXPECT_FLOAT_EQ(acc[i], val);
        ++i;
    });
}

TEST(ConstArrayAccessor2, ForEachIndex) {
    jet::Array2<float> arr1(
        {{1.f,  2.f,  3.f,  4.f},
         {5.f,  6.f,  7.f,  8.f},
         {9.f, 10.f, 11.f, 12.f}});
    auto acc = arr1.ConstAccessor();

    acc.ForEachIndex([&](size_t i, size_t j) {
        size_t idx = i + (4 * j) + 1;
        EXPECT_FLOAT_EQ(static_cast<float>(idx), acc(i, j));
    });
}
