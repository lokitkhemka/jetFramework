#include <Arrays/array1_accessor.h>
#include <Arrays/array1.h>
#include <gtest/gtest.h>

TEST(ArrayAccessor1, Constructors)
{
    double data[5];
    for (int i=0; i<5; ++i)
    {
        data[i] = static_cast<double>(i);
    }

    jet::ArrayAccessor1<double> acc(5, data);

    EXPECT_EQ(5u, acc.Size());
    EXPECT_EQ(data, acc.Data());
}


TEST(ArrayAccessor1, Iterators)
{
    jet::Array1<float> arr1 = {6.f, 4.f, 1.f, -5.f};
    auto acc = arr1.Accessor();

    size_t i = 0;
    for(float& elem: acc)
    {
        EXPECT_FLOAT_EQ(acc[i], elem);
        ++i;
    }

    i=0;
    for (const float& elem: acc)
    {
        EXPECT_FLOAT_EQ(acc[i], elem);
        ++i;
    }
}

TEST(ArrayAccessor1, ForEach)
{
    jet::Array1<float> arr1 = {6.f, 4.f, 1.f, -5.f};
    auto acc = arr1.Accessor();

    size_t i = 0;
    acc.ForEach([&](float val){
        EXPECT_FLOAT_EQ(arr1[i],val);
        ++i;
    });
}

TEST(ArrayAccessor1, ForEachIndex)
{
    jet::Array1<float> arr1 = {6.f, 4.f, 1.f, -5.f};
    auto acc = arr1.Accessor();

    size_t count = 0;
    acc.ForEachIndex([&](size_t i){
        EXPECT_EQ(count, i);
        ++count;
    });
}

TEST(ArrayAccessor1, ParallelForEach) {
    jet::Array1<float> arr1(200);
    auto acc = arr1.Accessor();

    acc.ForEachIndex([&](size_t i) {
        arr1[i] = static_cast<float>(200.f - i);
    });

    acc.ParallelForEach([](float& val) {
        val *= 2.f;
    });

    acc.ForEachIndex([&](size_t i) {
        float ans = 2.f * static_cast<float>(200.f - i);
        EXPECT_FLOAT_EQ(ans, arr1[i]);
    });
}

TEST(ArrayAccessor1, ParallelForEachIndex) {
    jet::Array1<float> arr1(200);
    auto acc = arr1.Accessor();

    acc.ForEachIndex([&](size_t i) {
        arr1[i] = static_cast<float>(200.f - i);
    });

    acc.ParallelForEachIndex([&](size_t i) {
        float ans = static_cast<float>(200.f - i);
        EXPECT_EQ(ans, arr1[i]);
    });
}


TEST(ConstArrayAccessor1, Constructors)
{
    double data[5];
    for (int i=0; i<5; ++i)
    {
        data[i] = static_cast<double>(i);
    }
    {
        jet::ArrayAccessor1<double> acc(5, data);
        jet::ConstArrayAccessor1<double> cacc(acc);

        EXPECT_EQ(5u, cacc.Size());
        EXPECT_EQ(data, cacc.Data());
    }
    {
        jet::ConstArrayAccessor1<double> cacc((size_t)5, data);
        EXPECT_EQ(5u, cacc.Size());
        EXPECT_EQ(data, cacc.Data());
    }
}

TEST(ConstArrayAccessor1, Iterators)
{
    jet::Array1<float> arr1 = {6.f, 4.f, 1.f, -5.f};
    auto acc = arr1.ConstAccessor();

    size_t i = 0;
    for (const float& elem:acc)
    {
        EXPECT_FLOAT_EQ(acc[i], elem);
        ++i;
    }
}

TEST(ConstArrayAccessor1, ForEach)
{
    jet::Array1<float> arr1 = {6.f, 4.f, 1.f, -5.f};
    auto acc = arr1.ConstAccessor();

    size_t i = 0;
    acc.ForEach([&](float val){
        EXPECT_EQ(arr1[i], val);
        ++i;
    });
}

TEST(ConstArrayAccessor1, ForEachIndex)
{
    jet::Array1<float> arr1 = {6.f, 4.f, 1.f, -5.f};
    auto acc = arr1.ConstAccessor();

    size_t count = 0;
    acc.ForEachIndex([&](size_t i){
        EXPECT_EQ(count, i);
        ++count;
    });
}

TEST(ConstArrayAccessor1, ParallelForEachIndex) {
    jet::Array1<float> arr1(200);
    auto acc = arr1.ConstAccessor();

    acc.ForEachIndex([&](size_t i) {
        arr1[i] = static_cast<float>(200.f - i);
    });

    acc.ParallelForEachIndex([&](size_t i) {
        float ans = static_cast<float>(200.f - i);
        EXPECT_EQ(ans, arr1[i]);
    });
}